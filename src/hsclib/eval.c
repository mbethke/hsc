/*
 * hsclib/eval.c
 *
 * attribute value evaluation functions
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * updated: 15-May-1995
 * created: 11-Oct-1995
 */


#define NOEXTERN_HSCLIB_EVAL_H

#include <ctype.h>

#include "hsclib/inc_base.h"

#include "hsclib/eval.h"
#include "hsclib/id.h"
#include "hsclib/input.h"
#include "hsclib/skip.h"
#include "hsclib/uri.h"

/* maximul length of an operator identifer */
#define MAX_OP_LEN 8

/* unknown operator */
#define OP_NONE 0

/* step-size for temp. string */
#define TMP_STEPSIZE 128

/*
 * equation operators
 */
#define OP_EQ_STR  "="
#define OP_NEQ_STR "<>"
#define OP_GT_STR  ">"
#define OP_LT_STR  "<"
#define OP_GTE_STR ">="
#define OP_LTE_STR "<="
#define OP_CEQ_STR "=="         /*  case sensitive string comparison */
#define OP_CL_BRAKET_STR ")"    /* closing braket */

#define OP_EQ   1
#define OP_NEQ  2
#define OP_GT   3
#define OP_LT   4
#define OP_GTE  5
#define OP_LTE  6
#define OP_CEQ  7
#define OP_CL_BRAKET  8

/*
 * boolean operators
 */
#define OP_AND_STR "AND"
#define OP_NOT_STR "NOT"
#define OP_OR_STR  "OR"
#define OP_XOR_STR "XOR"

#define OP_AND 11
#define OP_NOT 12
#define OP_OR  13
#define OP_XOR 14

/*
 * string operators
 */
#define OP_CAT_STR "+"
#define OP_CAT      15

typedef BYTE op_t;

/*
 * forward references
 */
STRPTR eval_expression (HSCPRC * hp, HSCATTR * dest, STRPTR endstr);

/*

 * global funcs
 *
 */

/*
 * err_op: unknown binary operator
 */
static VOID err_op (HSCPRC * hp, STRPTR opstr)
{
  hsc_message (hp, MSG_UNKN_BINOP, "unknown binary operator %q", opstr);
}

/*
 * eval_boolstr
 */
static BOOL eval_boolstr (STRPTR s)
{
  if (s[0])
    return (TRUE);
  else
    return (FALSE);
}

/*
 * check_attrname
 *
 * check string for legal attribute name
 */
BOOL check_attrname (HSCPRC * hp, STRPTR name)
{
  BOOL ok = FALSE;

  if (hsc_normch (name[0]))
    ok = TRUE;
  else
    hsc_message (hp, MSG_ILLG_ATTRNAME,
                 "illegal attribute identifier %q", name);

  return (ok);
}

/*
 * eval_attrname
 *
 * read next word and check it for a legal
 * attribute identifier
 */
static STRPTR eval_attrname (HSCPRC * hp)
{
  STRPTR result = NULL;
  STRPTR nw = infgetw (hp->inpf);

  if (nw)
    {

      if (check_attrname (hp, nw))
        result = nw;

    }
  else
    hsc_msg_eof (hp, "attribute identifier expected");

  return (result);
}


/*
 * try_eval_unary_op
 *
 * reads next word and tries to interpret it as an unary
 * operator; if no fitting operator exists, return NULL,
 * else immediatly process the operator and return its
 * result
 */
static STRPTR try_eval_unary_op (HSCPRC * hp, HSCATTR * dest, BOOL * err)
{
  STRPTR eval_result = NULL;
  INFILE *inpf = hp->inpf;
  STRPTR nw = eval_attrname (hp);


  *err = FALSE;
  if (nw)
    {

      if (!upstrcmp (nw, "NOT"))
        {

          /* TODO: this part looks a bit supid... */
          STRPTR nw = infgetw (inpf);

          if (nw)
            {

              BOOL err_rec = FALSE;     /* error var for recursive call */
              STRPTR endstr = NULL;

              if (strcmp (nw, "("))
                {

                  /* try to process another unary operator */
                  inungetcw (inpf);
                  eval_result = try_eval_unary_op (hp, dest, &err_rec);

                }
              else
                endstr = ")";

              /* if not, process another expression */
              if (!eval_result && !err_rec)
                eval_result = eval_expression (hp, dest, endstr);

            }
          else
            hsc_msg_eof (hp, "after NOT");

          /* set result or return error */
          if (eval_result)
            {

              set_vartext_bool (dest, !get_varbool (dest));
              eval_result = get_vartext (dest);

            }
          else
            *err = TRUE;

        }
      else if (!upstrcmp (nw, "DEFINED"))
        {

          nw = eval_attrname (hp);
          if (nw)
            {

              HSCATTR *attr = find_varname (hp->defattr, nw);

              if (attr)
                set_vartext_bool (dest, TRUE);
              else
                set_vartext_bool (dest, FALSE);
              eval_result = get_vartext (dest);

            }
        }
      else if (!upstrcmp (nw, "GETENV"))
        {

          /* get environment variable */
          eval_result = eval_expression (hp, dest, NULL);
          if (eval_result)
            {

              STRPTR env_value = getenv (get_vartext (dest));
              if (!env_value)
                {

                  hsc_message (hp, MSG_UNKN_ENVVAR,
                               "unknown environment variable %q",
                               get_vartext (dest));

                  env_value = "";

                }

              set_vartext (dest, env_value);

            }


        }
      else if (!upstrcmp (nw, "SET"))
        {

          nw = eval_attrname (hp);
          if (nw)
            {

              HSCATTR *attr = find_varname (hp->defattr, nw);

              if (attr)
                {
                  if (attr->vartype == VT_BOOL)
                    {

                      set_vartext_bool (dest, get_varbool (attr));

                    }
                  else if (get_vartext (attr))
                    set_vartext_bool (dest, TRUE);
                  else
                    set_vartext_bool (dest, FALSE);
                  eval_result = get_vartext (dest);

                }
              else
                {

                  hsc_msg_unkn_attr (hp, nw);
                  *err = TRUE;

                }
            }
        }
      else
        inungetcw (inpf);

    }

  if (!nw)
    *err = TRUE;

  return (eval_result);
}


/*
 * eval_op
 *
 * evaluate binary operator string
 */
static BYTE eval_op (HSCPRC * hp)
{
  BYTE op = OP_NONE;
  BOOL op_eof = FALSE;          /* flag: end of file reached */
  INFILE *inpf = hp->inpf;
  STRPTR nw = infgetw (inpf);

  D (fprintf (stderr, DHL "  operator \"%s", nw));

  if (nw)
    {

      /* boolean operators */
      if (!upstrcmp (nw, OP_AND_STR))
        op = OP_AND;
      else if (!upstrcmp (nw, OP_OR_STR))
        op = OP_OR;
      else if (!upstrcmp (nw, OP_XOR_STR))
        op = OP_XOR;

      /* concatenation operator */
      else if (!strcmp (nw, OP_CAT_STR))
        op = OP_CAT;

      /* closing braket */
      else if (!strcmp (nw, OP_CL_BRAKET_STR))
        op = OP_CL_BRAKET;

      /* comparison operators */
      else if (strenum (nw, "<|=|>", '|', STEN_CASE))
        {

          STRARR opstr[3];
          int ch;

          /* determine whole comparison operator:
           * take first word, and check for next
           * single character, if it is one of
           * "<", "=" or ">", too. if so, append
           * it to the string that tells the
           * operator.
           */
          strcpy (opstr, nw);
          ch = infgetc (inpf);
          if (ch != EOF)
            {

              D (fprintf (stderr, "%c", (char) ch));

              if (strchr ("<=>", ch))
                {

                  opstr[1] = ch;
                  opstr[2] = 0;

                }
              else
                inungetc (ch, inpf);
            }
          else
            op_eof = TRUE;

          /* find out comparison operator */
          if (!strcmp (nw, OP_EQ_STR))
            op = OP_EQ;
          else if (!strcmp (nw, OP_NEQ_STR))
            op = OP_EQ;
          else if (!strcmp (nw, OP_GT_STR))
            op = OP_GT;
          else if (!strcmp (nw, OP_LT_STR))
            op = OP_LT;
          else if (!strcmp (nw, OP_LTE_STR))
            op = OP_LTE;
          else if (!strcmp (nw, OP_GTE_STR))
            op = OP_GTE;
          else if (!strcmp (nw, OP_CEQ_STR))
            op = OP_CEQ;
          else
            err_op (hp, opstr);

        }
      else
        err_op (hp, nw);

    }
  else
    op_eof = TRUE;

  D (fprintf (stderr, "\"\n"));


  if (op_eof)
    hsc_msg_eof (hp, "operator expected");

  return (op);
}

/*
 * process_op
 */
static VOID process_op (HSCPRC * hp, HSCATTR * dest, BYTE op, STRPTR str1, STRPTR str2)
{
  EXPSTR *result = init_estr (40);
  BOOL result_set = FALSE;

  D (fprintf (stderr, DHL "  \"%s\", \"%s\"\n", str1, str2));
  if (str2 && (op != OP_NONE))
    {

      BOOL bool_val1 = eval_boolstr (str1);
      BOOL bool_val2 = eval_boolstr (str2);

      switch (op)
        {

        case OP_AND:
          if (bool_val1 && bool_val2)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_OR:
          if (bool_val1 || bool_val2)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_XOR:
          if ((bool_val1 || bool_val2)
              && !(bool_val1 && bool_val2)
            )
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;



        case OP_EQ:

          /* string comparison, ignore case */
          if (!upstrcmp (str1, str2))
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_NEQ:

          /* string comparison "<>" */
          if (upstrcmp (str1, str2))
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_GT:

          /* string comparison ">" */
          if (upstrcmp (str1, str2) > 0)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_LT:

          /* string comparison "<" */
          if (upstrcmp (str1, str2) < 0)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_GTE:

          /* string comparison ">=" */
          if (upstrcmp (str1, str2) >= 0)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_LTE:

          /* string comparison "<=" */
          if (upstrcmp (str1, str2) <= 0)
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_CEQ:

          /* string comparison, case sensitive */
          if (!strcmp (str1, str2))
            set_vartext_bool (dest, TRUE);
          else
            set_vartext_bool (dest, FALSE);
          break;

        case OP_CAT:

          /* concat two expressions */
          set_estr (result, str1);
          app_estr (result, str2);
          result_set = TRUE;

          break;

        default:
          panic ("empty operator");
          break;

        }
    }

  /* store result in destination attribute,
   * if this has not happened yet
   */
  if (result_set)
    set_vartext (dest, estr2str (result));

  /* remove temp. string for result */
  del_estr (result);
}


/*
 *-------------------------------------
 * eval_expression: evaluate expression
 *-------------------------------------
 */

/*
 * eval_string_expr
 *
 * evaluate string expression WITH enclosing quotes
 */
static STRPTR eval_string_expr (HSCPRC * hp, HSCATTR * dest)
{
  INFILE *inpf = hp->inpf;
  STRPTR eval_result = NULL;
  EXPSTR *tmpstr = init_estr (TMP_STEPSIZE);
  int quote;


  /* get quote char */
  quote = infgetc (inpf);
  if (quote != EOF)
    {
      BOOL end = FALSE;

      while (!end)
        {
          int ch = infgetc (inpf);
          if (ch == EOF)
            {
              hsc_msg_eof (hp, "reading string constant");
              eval_result = NULL;
              end = TRUE;
            }
          else if (ch != quote)
            {
              /* check for LF inside string */
              if (ch == '\n')
                hsc_message (hp, MSG_STR_LF,
                             "linefeed found inside string");

              /* append next char to string */
              app_estrch (tmpstr, ch);
            }
          else
            {
              /* closing quote reached */
              eval_result = estr2str (tmpstr);
              end = TRUE;
            }
        }
    }
  else
    hsc_msg_eof (hp, "reading string constant");

  /* set new attribute value */
  if (eval_result)
    {
      /* set new quotes */
      dest->quote = quote;
      /* set new value */
      set_vartext (dest, eval_result);
      eval_result = get_vartext (dest);
    }

  /* remove temp. string */
  del_estr (tmpstr);

  return (eval_result);
}


/*
 * eval_string_expr_noquote
 *
 * evaluate string expression WITHOUT enclosing quotes
 */
STRPTR eval_string_expr_noquote (HSCPRC * hp, HSCATTR * dest)
{
  /* TODO: check for ch==">": attrval missing */
  INFILE *inpf = hp->inpf;
  STRPTR eval_result = NULL;
  EXPSTR *tmpstr = init_estr (TMP_STEPSIZE);
  BOOL end = FALSE;

  /* TODO: check for empty expression */

  /*
   * read next char from input file until a
   * closing quote if found.
   * if the arg had no quote, a white space
   * or a '>' is used to detect end of arg.
   * if a LF is found, view error message
   */
  while (!end)
    {

      int ch = infgetc (inpf);
      if (ch == EOF)
        {

          hsc_msg_eof (hp, "reading attribute");

          end = TRUE;

        }
      else if ((ch == '\n')
               || (inf_isws (ch, inpf) || (ch == '>')))
        {

          /* end of arg reached */
          inungetc (ch, inpf);
          eval_result = estr2str (tmpstr);
          end = TRUE;

        }
      else
        {

          /* append next char to tmpstr */
          app_estrch (tmpstr, ch);

        }
    }

  /* set new attribute value */
  if (eval_result)
    {

      set_vartext (dest, eval_result);
      eval_result = get_vartext (dest);

    }

  /* remove temp. string */
  del_estr (tmpstr);

  return (eval_result);
}

/*
 * eval_attrref
 *
 * evaluate reference to attribute
 *
 */
static STRPTR eval_attrref (HSCPRC * hp, HSCATTR * destattr)
{
  STRPTR eval_result = NULL;
  STRPTR nw = eval_attrname (hp);

  if (nw)
    {

      HSCATTR *refvar = find_varname (hp->defattr, nw);

      if (refvar)
        {

          /* TODO: type checking */
          destattr->quote = refvar->quote;
          eval_result = refvar->text;

          /* check empty/circular reference */
          if (!eval_result)
            hsc_message (hp, MSG_EMPTY_SYMB_REF,
                         "empty reference to %A", destattr);

          /* debugging message */
          DDA (fprintf (stderr, DHL "   %s refers to (%s)\n",
                        destattr->name, refvar->name));

        }
      else
        {

          /* reference to unknown destattr */
          hsc_msg_unkn_attr (hp, nw);

        }

      /* set empty value for reference to NULL */
      if ((!refvar) || (!eval_result))
        {

          /* return empty destattr */
          destattr->quote = '"';
          eval_result = "";

        }

    }

  /* set value of destination attribute */
  if (eval_result)
    set_vartext (destattr, eval_result);

  return (eval_result);
}

/*
 * eval_expression
 *
 * params: dest....attribute where to store result in
 *         inpf....input file
 *         err.....flag that is set to TRUE if an error occured
 *         endstr..word that is expected at end of expession;
 *                 NULL for no special ending word
 * result: result of evaluation (IF_TRUE or FALSE)
 *
 * NOTE: if endstr==NULL, that means that eval_expression() is called
 *   immediatly after the "=" of an attribute. In this case, if no
 *   quotes are found as next char, all chars are read until the next
 *   white-space or LF occures.
 *   If no special char was found until now (only "A".."Z", "_", "-" and
 *   digits occured), we first interpret the string as name for an
 *   attribute reference; if such an attribute does not exist, it is
 *   asumed that the value passed is a string constant (same as it would
 *   have been enclosed in quotes).
 *
 */
STRPTR eval_expression (HSCPRC * hp, HSCATTR * dest, STRPTR endstr)
{
  /* TODO: endstr needs to be boolean flag only */
  INFILE *inpf = hp->inpf;
  EXPSTR *vararg = init_estr (TMP_STEPSIZE);
  /* used as destination by eval_string_exprXX() */

  STRPTR exprstr = NULL;        /* return value */
  int ch;                       /* char read from input */

  /* skip white spaces */
  infskip_ws (inpf);

  /* read dest->quote char */
  ch = infgetc (inpf);
  if (!strchr (VQ_STR_QUOTE, ch))
    if (ch != EOF)
      dest->quote = VQ_NO_QUOTE;
    else
      hsc_msg_eof (hp, "reading attribute");
  else
    dest->quote = ch;

  /* skip linefeeds, if evaluation hsc-expression */
  if (endstr)
    skip_lfs (hp);

  if (ch == '(')
    {

      /* process braket */
      exprstr = eval_expression (hp, dest, ")");

      /* set generic double quote */
      if (!endstr)
        dest->quote = '\"';

    }
  else if (ch != EOF)
    {

      /* write current char read back to input buffer */
      inungetc (ch, inpf);

      if (dest->quote != VQ_NO_QUOTE)

        /* process string expression with quotes */
        exprstr = eval_string_expr (hp, dest);

      else if (endstr)
        {

          BOOL err = FALSE;

          /* try to process unary operator */
          exprstr = try_eval_unary_op (hp, dest, &err);

          /* process attribute reference */
          if (!exprstr && !err)
            exprstr = eval_attrref (hp, dest);

        }
      else
        {

          /* process string expression without quotes */
          exprstr = eval_string_expr_noquote (hp, dest);

        }

    }

  if (exprstr && endstr)
    {

      BYTE op;

      skip_lfs (hp);            /* skip linefeeds */

      /* evaluate operator */
      op = eval_op (hp);

      if (op == OP_CL_BRAKET)
        DMSG ("  END mark operator reached");
      else if (op != OP_NONE)
        {

          /* no endmark reached */
          STRPTR str1 = exprstr;

          /* read second operator */
          if (op != OP_NONE)
            {

              STRPTR str2 = NULL;
              HSCATTR *dest1 = new_hscattr ("dummy");

              /* init dummy attribute */
              dest1->vartype = dest->vartype;
              dest1->quote = dest->quote;

              /* compute second value */
              str2 = eval_expression (hp, dest1, endstr);

              if (str2)
                process_op (hp, dest, op, str1, str2);
              else
                exprstr = NULL;

              /* remove result of second value */
              del_hscattr ((APTR) dest1);

            }
          else
            {

              /* TODO: skip expression until ">" */
              exprstr = NULL;
            }

          if (exprstr)
            {

              /* store result */
              exprstr = get_vartext (dest);

            }

        }
      else
        {
          DMSG ("  NO operator");
        }
    }

  if (!endstr)
    if (exprstr && !endstr)
      {

        /*
         * set quote, depending on quotemode
         */
        switch (hp->quotemode)
          {

          case QMODE_KEEP:
            /* do nufin */
            break;

          case QMODE_DOUBLE:
            dest->quote = '\"';
            break;

          case QMODE_SINGLE:
            dest->quote = '\'';
            break;

          case QMODE_NONE:
            dest->quote = VQ_NO_QUOTE;
            break;

          }

        /*
         * check enum type
         */
        if (dest->vartype == VT_ENUM)
          {
            if (!strenum (exprstr, dest->enumstr, '|', STEN_NOCASE))
              {
                /* unknown enum value */
                hsc_message (hp, MSG_ENUM_UNKN,
                             "unknown value %q for enumerator %A",
                             exprstr, dest);
              }
          }
        /*
         * check color value
         */
        else if (dest->vartype == VT_COLOR)
          {
            BOOL ok = FALSE;
            size_t color_len = strlen ("#rrggbb");

            if ( exprstr[0] == '#' ) {

                /* check RGB-value */
                if  ( strlen (exprstr) == color_len )
                {
                    size_t i = 1;

                    ok = TRUE;
                    for (; i < color_len; i++)
                      if (!isxdigit (exprstr[i]))
                        ok = FALSE;
                }
            } else {

                /* check color name */
                if ( hp->color_names ) {
                    if ( strenum( exprstr, hp->color_names, '|', STEN_NOCASE ) )
                        ok = TRUE;
                } else
                    ok = TRUE;

            }

            if (!ok)
              /* illegal color value */
              hsc_message (hp, MSG_ILLG_COLOR,
                           "illegal color value %q for %A",
                           exprstr, dest);
          }
        /*
         * check numeric value
         */
        else if (dest->vartype == VT_NUM)
          {
            BOOL ok = FALSE;
            int i = 0;

            if ((exprstr[0] == '+')
                || (exprstr[0] == '-'))
              {
                i = 1;
              }

            if (strlen (exprstr) - i)
              {
                ok = TRUE;
                while (exprstr[i] && ok)
                  if (!isdigit (exprstr[i]))
                    ok = FALSE;

              }

            if (!ok)
              /* unknown enum value */
              hsc_message (hp, MSG_ILLG_NUM,
                           "illegal numeric value %q for %A",
                           exprstr, dest);
          }
        /*
         * for boolean attributes, set the name
         * of the attribute if TRUE, or set an
         * empty string, if FALSE
         */
        else if (dest->vartype == VT_BOOL)

          if (eval_boolstr (exprstr))
            set_vartext (dest, dest->name);
          else
            set_vartext (dest, "");

        /*
         * checks performed only for tags,
         * but are skipped for macros
         */
        if (!(dest->varflag & VF_MACRO))
          {
            /*
             * parse uri (convert abs.uris, check existence)
             */
            if (dest->vartype == VT_URI)
              {
                EXPSTR *dest_uri = init_estr (32);

                parse_uri (hp, dest_uri, exprstr);
                set_vartext (dest, estr2str (dest_uri));

                del_estr (dest_uri);
              }
            else if (dest->vartype == VT_ID)
              {
                DAV( fprintf( stderr, DHL "new id: `%s'\n", exprstr ) );
                if ( hp->CB_id )
                    ( *(hp->CB_id) )(hp, dest, exprstr );
                hsc_add_id_ref( hp, exprstr );
              }
          }

        exprstr = get_vartext (dest);
      }
    else
      {
        /* if error occured,
         * skip until ">", unread ">"
         */
        skip_until_eot (hp, NULL);
        if (!hp->fatal)
          inungetcw (inpf);
      }

  del_estr (vararg);

  return (exprstr);
}
