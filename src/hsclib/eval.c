/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001 Matthias Bethke
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
 */
/*
 * hsclib/eval.c
 *
 * attribute value evaluation functions
 *
 * updated: 19-Oct-2001
 * updated: 07-May-2001
 * updated: 14-Sep-1998
 * created: 11-Oct-1995
 */

#define NOEXTERN_HSCLIB_EVAL_H

#include <ctype.h>
#include <time.h>

#include "hsclib/inc_base.h"

#include "hsclib/eval.h"
#include "hsclib/input.h"
#include "hsclib/skip.h"
#include "hsclib/uri.h"

/* maximul length of an operator identifer */
#define MAX_OP_LEN 8

/* step-size for temp. string */
#define TMP_STEPSIZE 128

/*
 * equation operators
 */
#define OP_EQ_STR  "="
#define OP_CEQ_STR "=="
#define OP_NEQ_STR "<>"
#define OP_NUMGT_STR ">"
#define OP_NUMLT_STR "<"
#define OP_NUMGE_STR ">="
#define OP_NUMLE_STR "<="
#define OP_GT_STR "GT"
#define OP_LT_STR "LT"
#define OP_GE_STR "GE"
#define OP_LE_STR "LE"
#define OP_INSIDE_STR "IN"
#define OP_CL_BRACKET_STR ")"   /* closing bracket */
#define OP_CL_BRACE_STR "}"     /* closing brace */

/*
 * boolean operators
 */
#define OP_AND_STR "AND"
#define OP_NOT_STR "NOT"
#define OP_OR_STR  "OR"
#define OP_XOR_STR "XOR"

/*
 * string/arithmetic operators
 */
#define OP_CAT_STR "+"
#define OP_ADD_STR "&"          /* this sucks */
#define OP_SUB_STR "-"
#define OP_MUL_STR "*"
#define OP_DIV_STR "/"
#define OP_MOD_STR "MOD"        /* modulo */

typedef enum {OP_NONE=0, OP_EQ, OP_NEQ, OP_CEQ,
        OP_GT, OP_LT, OP_GE, OP_LE,
        OP_NUMGT, OP_NUMLT, OP_NUMGE, OP_NUMLE,
        OP_INSIDE, 
        OP_AND, OP_NOT, OP_OR, OP_XOR,
        OP_CAT, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
        OP_CL_BRACKET, OP_CL_BRACE} op_t;

/*
 * forward references
 */
STRPTR eval_expression(HSCPRC * hp, HSCATTR * dest, STRPTR endstr);
static VOID check_integer(HSCPRC * hp, HSCATTR * dest, STRPTR value);

/*
 * 
 * global funcs
 *
 */

/*
 * err_op: unknown binary operator
 */
static VOID err_op(HSCPRC * hp, STRPTR opstr)
{
   hsc_message(hp, MSG_UNKN_BINOP, "unknown binary operator %q", opstr);
}

/*
 * eval_boolstr
 */
static BOOL eval_boolstr(STRPTR s)
{
   if (s[0])
      return (TRUE);
   else
      return (FALSE);
}

/* check for empty brakets (after GetTime/GetGmTime) */
static VOID check_brakets(HSCPRC * hp)
{
   if (parse_wd(hp, "("))
      parse_wd(hp, ")");
}

/*
 * gettimestr
 */
static EXPSTR *gettimestr(HSCPRC * hp, struct tm *time)
{
#define TIMEBUF_INC 20
   STRPTR timefmt = get_vartext_byname(hp->defattr, TIMEFORMAT_ATTR);
   EXPSTR *timebuf = init_estr(TIMEBUF_INC);
   BOOL strftrc = 0;            /* result of strftime() */
   size_t i;                    /* loop var */

   /* set default time format */
   if (!timefmt)
   {
      timefmt = "%d-%b-%Y, %H:%M";
   }

   while (!(hp->fatal) && !strftrc)
   {
      /* expand timebuffer */
      for (i = 0; i < TIMEBUF_INC; i++)
         app_estrch(timebuf, '.');

      D(fprintf(stderr, DHL "    timebuf: inc+%d\n", TIMEBUF_INC));

      /* output time */
      strftrc = strftime(estr2str(timebuf), estrlen(timebuf),
                         timefmt, time);
   }

   if (!strftrc)
   {
      del_estr(timebuf);
      timebuf = NULL;
   }

   return (timebuf);
}

/*
 * getfilesize
 *
 * get size of a specific file
 *
 * templates for HSC.FORMAT.FILESIZE:
 * %b   size in byte
 * %k   size in Kbyte
 * %m   size in MByte
 * %g   size in Gbyte
 * %a   size, unit computed automatically
 * %u   unit for %A (none, "K", "M" or "G")
 */
static STRPTR getfilesize(HSCPRC * hp, EXPSTR * dest, STRPTR uri)
{
   STRPTR filesizestr = NULL;
   FILE *file = NULL;
   LONG filesize = 0;           /* filesize in byte */
   LONG filesize_k = 0;         /* filesize in Kbyte */
   LONG filesize_m = 0;         /* filesize in Mbyte */
   LONG filesize_g = 0;         /* filesize in Gbyte */
   LONG filesize_auto = 0;      /* filesize in auto-units (%A) */
   EXPSTR *efilename = init_estr(32);
   STRPTR filename = NULL;
   STRPTR sizeunit = "";
   STRPTR s = get_vartext_byname(hp->defattr,
                                 FILESIZEFORMAT_ATTR);

   conv_hscuri2file(hp, efilename, uri);
   filename = estr2str(efilename);

   D(fprintf(stderr, DHL "  GETFILESIZE(`%s')\n", filename));
   errno = 0;
   file = fopen(filename, "rb");
   if (file)
   {
      /* retrieve size */
      fseek(file, 0L, SEEK_END);
      filesize = ftell(file);
      fclose(file);

      /* compute size in units, */
      filesize_k = (filesize + 512) >> 10;
      filesize_m = (filesize_k + 512) >> 10;
      filesize_g = (filesize_m + 512) >> 10;

      /* compute auto-size */
      if (filesize_g > 10)
      {
         filesize_auto = filesize_g;
         sizeunit = "G";
      }
      else if (filesize_m > 10)
      {
         filesize_auto = filesize_m;
         sizeunit = "M";
      }
      else if (filesize_k > 10)
      {
         filesize_auto = filesize_k;
         sizeunit = "K";
      }
      else
      {
         filesize_auto = filesize;
         sizeunit = "";
      }
   }
   else
   {
      /* file not found */
      filesize = 0;
      filesize_k = 0;
      filesize_m = 0;
      filesize_g = 0;
      filesize_auto = 0;
      sizeunit = "";
      hsc_msg_nouri(hp, filename, uri, "get filesize");
   }

   /* parse template */
   clr_estr(dest);
   if (s)
   {
      while (s[0])
      {
         if (s[0] == '%')
         {
            if (s[1])
               s++;
            switch (s[0])
            {
            case 'b':
               app_estr(dest, long2str(filesize));
               break;

            case 'k':
               app_estr(dest, long2str(filesize_k));
               break;

            case 'm':
               app_estr(dest, long2str(filesize_m));
               break;

            case 'g':
               app_estr(dest, long2str(filesize_g));
               break;

            case 'a':
               app_estr(dest, long2str(filesize_auto));
               break;

            case 'u':
               app_estr(dest, sizeunit);
               break;

            default:
               app_estrch(dest, '%');
               app_estrch(dest, s[0]);
               break;
            }
         }
         else
            app_estrch(dest, s[0]);
         s++;
      }
   }
   else
   {
      D(panic("no template for filesize-format"));
   }

   /* set filesize-str */
   filesizestr = estr2str(dest);
   D(fprintf(stderr, DHL "  =`%s'\n", filesizestr));

   del_estr(efilename);

   return (filesizestr);
}

/*
 * set_var_basename_ext
 *
 * Set HSC variable <dest> to basename or extension of filename stored
 * in string <src>, depending on the "ext" flag.
 * Returns contents of <dest> as a read-only C string or NULL on error.
 */
static const STRPTR set_var_basename_ext(STRPTR src, HSCATTR *dest, BOOL ext)
{
STRPTR end;
EXPSTR *tmp = init_estr(0);
BOOL ok=FALSE;

	if(tmp)
	{
		end   = src + strlen(src) - 1;
		while((*end != '.') && (end > src)) end--;
      if((end == src) && (*end != '.')) {
         /* no period */ 
         if(ext) set_vartext(dest,"");
         else set_vartext(dest,src);
      } else {
   		if(ext) ok = set_estr(tmp,end+1);
	   	else ok = set_estrn(tmp,src,end-src);
		   if(ok) set_vartext(dest,estr2str(tmp));
   		del_estr(tmp);
      }
	}
	return ok ? get_vartext(dest) : NULL;
}

/*
 * check_attrname
 *
 * check string for legal attribute name
 * "dest" must be set to an HSC attribute initialized by new_hscattr() if and
 * only if allow_expr==TRUE.
 */
STRPTR check_attrname(HSCPRC * hp, HSCATTR *dest, STRPTR name, BOOL allow_expr)
{
   STRPTR res = NULL;

   if (hsc_normch(name[0])) {
      res = name;
   } else if(('/' == name[0]) && ('\0' == name[1]) && hp->xhtml_emptytag) {
         hp->xhtml_emptytag = FALSE; /* only allow once */
         res = name;
   } else if(('{' == name[0]) && allow_expr) {
      dest->vartype = VT_STRING;
      res = eval_expression(hp,dest,"}");
   } else {
      hsc_message(hp, MSG_ILLG_ATTRNAME,
            "illegal attribute identifier %q%s",
            name,strcmp(name,"/") ? "" : " (XHTML mode?)");
   }
   return res;
}

/*
 * eval_attrname
 *
 * read next word and check it for a legal
 * attribute identifier
 */
static STRPTR eval_attrname(HSCPRC *hp, HSCATTR *tmpdest)
{
   STRPTR result = NULL;
   STRPTR nw = infgetw(hp->inpf);
   if (nw) {
      if (NULL != (nw = check_attrname(hp, tmpdest, nw, TRUE))) {
         result = nw;
      }
   } else {
      hsc_msg_eof(hp, "attribute identifier expected");
   }
   return (result);
}

/*
 * quotestr
 *
 * return readable string for quote-kind
 */
STRPTR quotestr(int quote)
{
   STRPTR s = "UNKNOWN";

   if (quote == DOUBLE_QUOTE)
   {
      s = "[double]";
   }
   else if (quote == SINGLE_QUOTE)
   {
      s = "[single]";
   }
   else if (quote == BACK_QUOTE)
   {
      s = "[back]";
   }
   else if (quote == VQ_NO_QUOTE)
   {
      s = "[none]";
   }
   else
   {
      STRARR tmp[60];
      sprintf(tmp, "unknown quote-kind: $%02x #%03d", quote, quote);
      panic(tmp);
   }

   return (s);
}

/*
 * choose_quote
 *
 * choose quote to be used for attr, depending on
 * hp->quotemode and quotes used inside the value
 */
VOID choose_quote(HSCPRC * hp, HSCATTR * attr)
{
   int quote = attr->quote;
   LONG qm = hp->quotemode;     /* lazy.. */
   BOOL single_quote = FALSE;
   BOOL double_quote = FALSE;
   BOOL nasty_char = FALSE;

   STRPTR value = get_vartext(attr);

   D(fprintf(stderr, DHL "  choosing quote\n"));

   if (attr->vartype == VT_BOOL)
   {
      D(fprintf(stderr, DHL "    forget it, it's just a boolean attrib\n"));
   }
   else if (value[0])
   {
      /* scan attribute value for quotes */
      while (value[0])
      {
         if (value[0] == SINGLE_QUOTE)
         {
            D(fprintf(stderr, DHL "    single quote detected\n"));
            single_quote = TRUE;
            nasty_char = TRUE;
         }
         else if (value[0] == DOUBLE_QUOTE)
         {
            D(fprintf(stderr, DHL "    double quote detected\n"));
            double_quote = TRUE;
            nasty_char = TRUE;
         }
         else if ((!hsc_normch(value[0]) || (value[0] == '_'))
                  && !nasty_char)
         {
            D(fprintf(stderr, DHL "    nasty-char #%d detected\n", value[0]));
            nasty_char = TRUE;
         }

         value++;
      }
   }
   else
   {
      /* empty value */
      nasty_char = TRUE;
   }

   if (qm == QMODE_KEEP)
   {
      /* check, if quote is missing */
      if ((attr->quote == VQ_NO_QUOTE)
          && nasty_char)
      {
         hsc_message(hp, MSG_REQU_QUOTE,
                     "value for %A requires quotes", attr);
      }
   }
   else
   {
      /* choose quote */
      if (single_quote && double_quote)
      {
         /* both kind of quotes appeared in value:
          * replace double-quotes by "&quot;" */
         EXPSTR *newval = init_estr(32);  /* new attribute value */

         /* scan old value for `\"', replace them by `&quot'
          * and store new value in newval */
         value = get_vartext(attr);
         while (value[0])
         {
            if (value[0] == DOUBLE_QUOTE)
            {
               D(fprintf(stderr, DHL "    replace by `&quot;' in value\n"));
               /* TODO: message */
               app_estr(newval, "&quot;");
            }
            else
               app_estrch(newval, value[0]);
            value++;
         }

         /* update attribute value */
         set_vartext(attr, estr2str(newval));

         quote = DOUBLE_QUOTE;

         del_estr(newval);
      }
      else
      {
         if (single_quote)
         {
            D(fprintf(stderr, DHL "    double quote forced\n"));
            quote = DOUBLE_QUOTE;
         }
         else if (double_quote)
         {
            D(fprintf(stderr, DHL "    single quote forced\n"));
            quote = SINGLE_QUOTE;
         }
         else
         {
            /* no quote in value: choose quote user prefers */
            if (qm == QMODE_SINGLE)
            {
               D(fprintf(stderr, DHL "    single quote preferred\n"));
               quote = SINGLE_QUOTE;
            }
            else if (qm == QMODE_DOUBLE)
            {
               D(fprintf(stderr, DHL "    double quote preferred\n"));
               quote = DOUBLE_QUOTE;
            }
            else if (qm == QMODE_NONE)
            {
               if (nasty_char)
               {
                  D(fprintf(stderr, DHL "    quote needed (nasty char)\n"));
                  quote = DOUBLE_QUOTE;
               }
               else
               {
                  D(fprintf(stderr, DHL "    no quote needed\n"));
                  quote = VQ_NO_QUOTE;
               }
            }
            else
            {
               panic("illegal quote-mode");
            }
         }
      }
      /* check, if quote has changed */
      if (attr->quote != quote)
      {
         hsc_message(hp, MSG_CHANGED_QUOTE,
                     "changed quotes for %A from %s to %s",
                     attr, quotestr(attr->quote), quotestr(quote));
      }
   }

   attr->quote = quote;
}

/*
 * try_eval_unary_op
 *
 * reads next word and tries to interpret it as an unary
 * operator; if no fitting operator exists, return NULL,
 * else immediatly process the operator and return its
 * result
 */
static STRPTR try_eval_unary_op(HSCPRC * hp, HSCATTR * dest, BOOL * err)
{
   STRPTR eval_result = NULL;
   INFILE *inpf = hp->inpf;
   HSCATTR *eadest = new_hscattr(PREFIX_TMPATTR "eval_attrname");
   STRPTR nw = eval_attrname(hp,eadest);
   HSCATTR *tmpdest = new_hscattr(PREFIX_TMPATTR "unary.operator");

   tmpdest->vartype = VT_STRING;

   *err = FALSE;
   if (nw)
   {
      if (!upstrcmp(nw, "NOT"))
      {
         /* TODO: this part looks a bit stupid... */
         STRPTR nw = infgetw(inpf);

         if (nw)
         {
            BOOL err_rec = FALSE;   /* error var for recursive call */
            STRPTR endstr = NULL;

            if (strcmp(nw, "("))
            {
               /* try to process another unary operator */
               inungetcw(inpf);
               eval_result = try_eval_unary_op(hp, dest, &err_rec);
            }
            else
               endstr = ")";

            /* if not, process another expression */
            if (!eval_result && !err_rec)
               eval_result = eval_expression(hp, dest, endstr);
         }
         else
            hsc_msg_eof(hp, "after NOT");

         /* set result or return error */
         if (eval_result)
         {
            set_varbool(dest, (BOOL)!get_varbool(dest));
            eval_result = get_vartext(dest);
         }
         else
            *err = TRUE;
      }
      else if (!upstrcmp(nw, "DEFINED"))
      {
         nw = eval_attrname(hp,eadest);
         if (nw)
         {
            HSCATTR *attr = find_varname(hp->defattr, nw);

            if (attr)
               set_varbool(dest, TRUE);
            else
               set_varbool(dest, FALSE);
            eval_result = get_vartext(dest);
         }
      }
		else if (!upstrcmp(nw, "ORD"))
		{
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            char ts[4];
            D(fprintf(stderr, DHL "  ORD(`%s')\n", eval_result));
            sprintf(ts,"%d",(int)eval_result[0]);
            set_vartext(dest,ts);
				eval_result = get_vartext(dest); 
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
			}
		}
  		else if (!upstrcmp(nw, "CHR"))
		{
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            char ts[2];
            D(fprintf(stderr, DHL "  CHR(`%s')\n", eval_result));
            check_integer(hp,tmpdest,eval_result);
            ts[0] = (char)atoi(eval_result);
            ts[1] = '\0';
            set_vartext(dest,ts);
				eval_result = get_vartext(dest); 
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
			}
		}
      else if (!upstrcmp(nw, "EXISTS"))
      {
         /* check existence of file (relative to destination dir) */
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            FILE *file = NULL;
            EXPSTR *dest_fname = init_estr(64);

            D(fprintf(stderr, DHL "  EXISTS(`%s')\n", eval_result));

            conv_hscuri2file(hp, dest_fname, eval_result);
            file = fopen(estr2str(dest_fname), "r");
            if (file)
            {
               fclose(file);
               set_varbool(dest, TRUE);
            }
            else
               set_varbool(dest, FALSE);

            del_estr(dest_fname);
            eval_result = get_vartext(dest);
         }
      }
      else if (!upstrcmp(nw, "FEXISTS"))
      {
         /* check existence of file */
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            FILE *file = NULL;
            EXPSTR *dest_fname = init_estr(64);

            D(fprintf(stderr, DHL "  EXISTS(`%s')\n", eval_result));

            file = fopen(eval_result, "r");
            if (file)
            {
               fclose(file);
               set_varbool(dest, TRUE);
            }
            else
               set_varbool(dest, FALSE);

            del_estr(dest_fname);
            eval_result = get_vartext(dest);
         }
      }
      else if (!upstrcmp(nw, "GETENV"))
      {
         /* get environment variable */
         eval_result = eval_expression(hp, dest, NULL);
         if (eval_result)
         {
            STRPTR env_value = getenv(get_vartext(dest));

            D(fprintf(stderr, DHL "  GETENV(`%s')\n", eval_result));
            if (!env_value)
            {
               hsc_message(hp, MSG_UNKN_ENVVAR,
                           "unknown environment variable %q",
                           get_vartext(dest));

               env_value = "";
            }
            set_vartext(dest, env_value);
            eval_result = get_vartext(dest);
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
         }
      }
      else if (!upstrcmp(nw, "GETFILESIZE"))
      {
         /* retrieve size of a file */
         EXPSTR *filesizestr = init_estr(0);
         HSCATTR *filedestattr = new_hscattr(PREFIX_TMPATTR "get.filesize");
         STRPTR filename = NULL;

         eval_result = NULL;
         filedestattr->vartype = VT_STRING;
         filename = eval_expression(hp, filedestattr, NULL);
         if (filename)
         {
            eval_result = getfilesize(hp, filesizestr, filename);
         }
         if (eval_result)
         {
            set_vartext(dest, eval_result);
            eval_result = get_vartext(dest);
         }
         del_hscattr(filedestattr);
         del_estr(filesizestr);
      }
      else if (!upstrcmp(nw, "GETTIME"))
      {
         /* get local time */
         EXPSTR *timestr = gettimestr(hp, localtime(&(hp->start_time)));

         D(fprintf(stderr, DHL "  GETTIME\n"));
         if (timestr)
         {
            set_vartext(dest, estr2str(timestr));
            del_estr(timestr);
            eval_result = get_vartext(dest);
         }
         check_brakets(hp);
      }
      else if (!upstrcmp(nw, "GETGMTIME"))
      {
         /* get greenwich mean time */
         EXPSTR *timestr = gettimestr(hp, gmtime(&(hp->start_time)));

         D(fprintf(stderr, DHL "  GETGMTIME\n"));
         if (timestr)
         {
            set_vartext(dest, estr2str(timestr));
            del_estr(timestr);
            eval_result = get_vartext(dest);
         }
         check_brakets(hp);
      }
      else if (!upstrcmp(nw, "SET"))
      {
         nw = eval_attrname(hp,eadest);
         if (nw)
         {
            HSCATTR *attr = find_varname(hp->defattr, nw);

            if (attr)
            {
               if (attr->vartype == VT_BOOL)
               {
                  set_varbool(dest, get_varbool(attr));
               }
               else if (get_vartext(attr))
                  set_varbool(dest, TRUE);
               else
                  set_varbool(dest, FALSE);
               eval_result = get_vartext(dest);
            }
            else
            {
               hsc_msg_unkn_attr_ref(hp, nw);
               *err = TRUE;
            }
         }
      }
		else if (!upstrcmp(nw, "BASENAME"))
		{
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            D(fprintf(stderr, DHL "  BASENAME(`%s')\n", eval_result));
				set_var_basename_ext(eval_result,dest,FALSE);
				eval_result = get_vartext(dest); 
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
			}
		}
 		else if (!upstrcmp(nw, "EXTENSION"))
		{
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
            D(fprintf(stderr, DHL "  EXTENSION(`%s')\n", eval_result));
				set_var_basename_ext(eval_result,dest,TRUE);
				eval_result = get_vartext(dest); 
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
			}
		}
		else if (!upstrcmp(nw, "URIKIND"))
		{
         eval_result = eval_expression(hp, tmpdest, NULL);
         if (eval_result)
         {
				static const STRPTR kinds[] = {"abs","ext","rel","rsv"};

            D(fprintf(stderr, DHL "  URIKIND(`%s')\n", eval_result));
				set_vartext(dest,kinds[uri_kind(eval_result)]);
				eval_result = get_vartext(dest); 
            D(fprintf(stderr, DHL "  =`%s'\n", eval_result));
			}
		}
      else
         inungetcw(inpf);
   }

   del_hscattr(tmpdest);
   del_hscattr(eadest);

   if (!nw)
      *err = TRUE;

   return (eval_result);
}

/*
 * eval_op
 *
 * evaluate binary operator string
 */
static BYTE eval_op(HSCPRC * hp)
{
   op_t op = OP_NONE;
   BOOL op_eof = FALSE;         /* flag: end of file reached */
   INFILE *inpf = hp->inpf;
   STRPTR nw = infgetw(inpf);

   D(fprintf(stderr, DHL "  operator \"%s", nw));

   if (nw)
   {
      /* boolean operators */
      if (!upstrcmp(nw, OP_AND_STR))         op = OP_AND;
      else if (!upstrcmp(nw, OP_OR_STR))     op = OP_OR;
      else if (!upstrcmp(nw, OP_XOR_STR))    op = OP_XOR;
      else if (!strcmp(nw, OP_CAT_STR))      op = OP_CAT; /* concatenation operator */
      else if (!strcmp(nw, OP_ADD_STR))      op = OP_ADD; /* concatenation operator */
      else if (!strcmp(nw, OP_SUB_STR))      op = OP_SUB; /* subtraction operator */
      else if (!strcmp(nw, OP_MUL_STR))      op = OP_MUL; /* multiplication operator */
      else if (!strcmp(nw, OP_DIV_STR))      op = OP_DIV; /* division operator */
      else if (!strcmp(nw, OP_MOD_STR))      op = OP_MOD; /* modulo operator */
      else if (!upstrcmp(nw, OP_GT_STR))     op = OP_GT;  /* string greater-than */
      else if (!upstrcmp(nw, OP_LT_STR))     op = OP_LT;  /* string less-than */
      else if (!upstrcmp(nw, OP_LE_STR))    op = OP_LE; /* string less-or-equal */
      else if (!upstrcmp(nw, OP_GE_STR))    op = OP_GE;  /* string greater-or-equal */
      else if (!upstrcmp(nw, OP_INSIDE_STR)) op = OP_INSIDE; /* substring search */
      else if (!strcmp(nw, OP_CL_BRACKET_STR)) op = OP_CL_BRACKET; /* closing bracket */
      else if (!strcmp(nw, OP_CL_BRACE_STR)) op = OP_CL_BRACE; /* closing brace */
      else if (strenum(nw, "<|=|>", '|', STEN_CASE))
      {
         /* comparison operators */
         STRARR opstr[3];
         int ch;

         /* determine whole comparison operator:
          * take first word, and check for next
          * single character, if it is one of
          * "<", "=" or ">", too. if so, append
          * it to the string that tells the
          * operator.
          */
         strcpy(opstr, nw);
         ch = infgetc(inpf);
         if (ch != EOF)
         {
            D(fprintf(stderr, "%c", (char) ch));

            if (strchr("<=>", ch))
            {
               opstr[1] = ch;
               opstr[2] = 0;
            }
            else
            {
               inungetc(ch, inpf);
            }
         }
         else
            op_eof = TRUE;

         /* find out comparison operator */
         if (!strcmp(opstr, OP_EQ_STR))
            op = OP_EQ;
         else if (!strcmp(opstr, OP_NEQ_STR))
            op = OP_NEQ;
         else if (!strcmp(opstr, OP_NUMGT_STR))
            op = OP_NUMGT;
         else if (!strcmp(opstr, OP_NUMLT_STR))
            op = OP_NUMLT;
         else if (!strcmp(opstr, OP_NUMLE_STR))
            op = OP_NUMLE;
         else if (!strcmp(opstr, OP_NUMGE_STR))
            op = OP_NUMGE;
         else if (!strcmp(opstr, OP_CEQ_STR))
            op = OP_CEQ;
         else
            err_op(hp, opstr);
      }
      else
      {
         err_op(hp, nw);
      }
   }
   else
   {
      op_eof = TRUE;
   }

   D(fprintf(stderr, "\"\n"));

   if (op_eof)
   {
      hsc_msg_eof(hp, "operator expected");
   }

   return (op);
}

/*
 * stroptol
 *
 * convert string operand to long int; treat "" as 0,
 * any non-numeric values as 1
 *
 * this enables to use boolean vars in numeric expressions
 *
 * result: TRUE
 */
static BOOL stroptol(HSCPRC * hp, LONG * dest, STRPTR str)
{
   BOOL ok = TRUE;

   *dest = 0;

   if (str[0])
   {
      STRPTR last_char = NULL;
      *dest = strtol(str, &last_char, 0);
      if (!last_char || (last_char[0]))
      {
         *dest = 1;
      }
   }
   else
   {
      /* empty string "" counts as 0, too */
   }

   return ok;
}

/*
 * process_arithmetic_op
 */
static BOOL process_arithmetic_op(HSCPRC * hp, EXPSTR * result, BYTE op, STRPTR str1, STRPTR str2)
{
   BOOL result_set = TRUE;
   LONG int1 = 0;               /* integer value of string operands */
   LONG int2 = 0;
   LONG intr = 0;               /* integer result */

   /* convert both string operands to integers */
   result_set = stroptol(hp, &int1, str1);
   result_set &= stroptol(hp, &int2, str2);

   if ((int2 == 0) && ((op == OP_DIV) || (op == OP_MOD)))
   {
      /* division by zero */
      hsc_message(hp, MSG_ARITHMETIC_ERROR,
                  "arithmetic error: division by zero");
      result_set = FALSE;
   }
   else
   {
      /* compute it */
      switch (op)
      {
      case OP_ADD:
         intr = int1 + int2;
         break;
      case OP_SUB:
         intr = int1 - int2;
         break;
      case OP_MUL:
         intr = int1 * int2;
         break;
      case OP_DIV:
         intr = int1 / int2;
         break;
      case OP_MOD:
         intr = int1 % int2;
         break;
      default:
         panic("unknown arithmetic operator");
         break;
      }
   }

   /* set result */
   if (result_set)
   {
      STRARR buf[20];
      sprintf(buf, "%ld", intr);
      set_estr(result, buf);
   }

   return result_set;
}

/*
 * process_boolean_op
 */
static BOOL process_boolean_op(HSCPRC * hp, HSCATTR * dest, BYTE op, STRPTR str1, STRPTR str2)
{
   BOOL bool_val1 = eval_boolstr(str1);
   BOOL bool_val2 = eval_boolstr(str2);
   BOOL bool_valr;

   switch (op)
   {
   case OP_AND: bool_valr = bool_val1 && bool_val2;
      break;

   case OP_OR:
      bool_valr = bool_val1 || bool_val2;
      break;

   case OP_XOR:
      bool_valr = bool_val1 ^ bool_val2;
      break;

   default:
      bool_valr = 0; /* just to make gcc happy */
      panic("unknown boolean operator");
      break;
   }

   set_varbool(dest, bool_valr);

   return FALSE;
}

/*
 * process_comparison_op
 */
static BOOL process_comparison_op(HSCPRC * hp, HSCATTR * dest, BYTE op, STRPTR str1, STRPTR str2)
{
   BOOL comparison_matches = FALSE;
   EXPSTR *uri1 = NULL;
   EXPSTR *uri2 = NULL;
   EXPSTR *fname = NULL;
   LONG int1,int2;

   /* If dest is an URI, apply the URI conversion on the two operands */
   if (dest->vartype == VT_URI)
   {
      uri1 = init_estr(0);
      uri2 = init_estr(0);
      fname = init_estr(64);

      D(fprintf(stderr, DHL "    URI comparison; conversion required\n"));

      /* copy operands to URI strings */
      set_estr(uri1, str1);
      set_estr(uri2, str2);

      /* convert URIs */
      conv_hscuri2fileNuri(hp, uri1, fname, str1);
      conv_hscuri2fileNuri(hp, uri2, fname, str2);

      /* set operands with URI values */
      str1 = estr2str(uri1);
      str2 = estr2str(uri2);

      D(fprintf(stderr, DHL "    str1=`%s'\n", str1));
      D(fprintf(stderr, DHL "    str2=`%s'\n", str2));
   }

   /* if using a numeric operator, convert operands */
   if((op == OP_NUMGT) ||
      (op == OP_NUMLT) ||
      (op == OP_NUMGE) ||
      (op == OP_NUMLE)) {
     stroptol(hp, &int1, str1);
     stroptol(hp, &int2, str2);
   }

   switch (op)
   {

   case OP_EQ:
      /* string comparison, ignore case */
      comparison_matches = !upstrcmp(str1, str2);
      break;

   case OP_NEQ:
      /* string comparison "<>" */
      comparison_matches = upstrcmp(str1, str2);
      break;

   case OP_NUMGT:
      /* integer comparison ">" */
      comparison_matches = (int1 > int2);
      break;

   case OP_NUMLT:
      /* integer comparison "<" */
      comparison_matches =  (int1 < int2);
      break;

   case OP_NUMGE:
      /* integer comparison ">=" */
      comparison_matches =  (int1 >= int2);
      break;

   case OP_NUMLE:
      /* integer comparison "<=" */
      comparison_matches =  (int1 <= int2);
      break;

   case OP_GT:
      /* string comparison "GT" */
      comparison_matches = (upstrcmp(str2, str1) > 0);
      break;

   case OP_LT:
      /* string comparison "LT" */
      comparison_matches = (upstrcmp(str2, str1) < 0);
      break;

   case OP_GE:
      /* string comparison "GE" */
      comparison_matches = (upstrcmp(str2, str1) >= 0);
      break;

   case OP_LE:
      /* string comparison "LE" */
      comparison_matches = (upstrcmp(str2, str1) <= 0);
      break;

   case OP_CEQ:
      /* string comparison, case sensitive */
      comparison_matches = !strcmp(str1, str2);
      break;

   default:
      panic("unknown comparison operator");
      break;
   }

   /* return comparison result */
   set_varbool(dest, comparison_matches);

   /* cleanup */
   del_estr(uri1);
   del_estr(uri2);
   del_estr(fname);

   return FALSE;
}

/*
 * process_op
 */
static VOID process_op(HSCPRC * hp, HSCATTR * dest, BYTE op, STRPTR str1, STRPTR str2)
{
   EXPSTR *result = init_estr(40);
   BOOL result_set = FALSE;

   D(fprintf(stderr, DHL "  \"%s\", \"%s\"\n", str1, str2));
   if (str2 && (op != OP_NONE))
   {
      switch (op)
      {
      case OP_CAT:

         /* concat two expressions */
         set_estr(result, str1);
         app_estr(result, str2);
         result_set = TRUE;

         break;

      case OP_INSIDE:
         /* sub-string search, ignore case */
         set_varbool(dest,(upstrstr(str2, str1)==NULL) ? FALSE : TRUE);
         break;

      case OP_AND:
      case OP_OR:
      case OP_XOR:
         result_set = process_boolean_op(hp, dest, op, str1, str2);
         break;

      case OP_ADD:
      case OP_SUB:
      case OP_MUL:
      case OP_DIV:
      case OP_MOD:
         result_set = process_arithmetic_op(hp, result, op, str1, str2);
         break;

      case OP_EQ:
      case OP_NEQ:
      case OP_NUMGT:
      case OP_NUMLT:
      case OP_NUMGE:
      case OP_NUMLE:
      case OP_GT:
      case OP_LT:
      case OP_GE:
      case OP_LE:
      case OP_CEQ:
         result_set = process_comparison_op(hp, dest, op, str1, str2);
         break;

      default:
         panic("empty operator");
         break;
      }
   }
   /* store result in destination attribute,
    * if this has not happened yet
    */
   if (result_set)
   {
      set_vartext(dest, estr2str(result));
   }

   /* remove temp. string for result */
   del_estr(result);
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
static STRPTR eval_string_expr(HSCPRC * hp, HSCATTR * dest)
{
   INFILE *inpf = hp->inpf;
   STRPTR eval_result = NULL;
   EXPSTR *tmpstr = init_estr(TMP_STEPSIZE);
   int quote;

   /* get quote char */
   quote = infgetc(inpf);
   if (quote != EOF)
   {
      BOOL end = FALSE;

      while (!end)
      {
         int ch = infgetc(inpf);
         if (ch == EOF)
         {
            hsc_msg_eof(hp, "reading string constant");
            eval_result = NULL;
            end = TRUE;
         }
         else if (ch != quote)
         {
            /* check for LF inside string */
            if (ch == '\n')
               hsc_message(hp, MSG_STR_LF,
                           "linefeed found inside string");

            /* append next char to string */
            app_estrch(tmpstr, ch);
         }
         else
         {
            /* closing quote reached */
            eval_result = estr2str(tmpstr);
            end = TRUE;
         }
      }
   }
   else
      hsc_msg_eof(hp, "reading string constant");

   /* set new attribute value */
   if (eval_result)
   {
      /* set new quotes */
      dest->quote = quote;
      /* set new value */
      set_vartext(dest, eval_result);
      eval_result = get_vartext(dest);
   }
   /* remove temp. string */
   del_estr(tmpstr);

   return (eval_result);
}

/*
 * eval_string_expr_noquote
 *
 * evaluate string expression WITHOUT enclosing quotes
 */
STRPTR eval_string_expr_noquote(HSCPRC * hp, HSCATTR * dest)
{
   INFILE *inpf = hp->inpf;
   STRPTR eval_result = NULL;
   EXPSTR *tmpstr = init_estr(TMP_STEPSIZE);
   BOOL end = FALSE;

   /*
    * read next char from input file until a
    * closing quote if found.
    * if the arg had no quote, a white space
    * or a '>' is used to detect end of arg.
    * if a LF is found, view error message
    */
   while (!end)
   {

      int ch = infgetc(inpf);
      if (ch == EOF)
      {
         hsc_msg_eof(hp, "reading attribute");

         end = TRUE;
      }
      else if ((ch == '\n')
               || (inf_isws((char)ch, inpf) || (ch == '>')))
      {
         /* end of arg reached */
         inungetc(ch, inpf);
         eval_result = estr2str(tmpstr);
         end = TRUE;
      }
      else
      {
         /* append next char to tmpstr */
         app_estrch(tmpstr, ch);
      }
   }

   /* set new attribute value */
   if (eval_result)
   {
      set_vartext(dest, eval_result);
      eval_result = get_vartext(dest);
   }
   /* remove temp. string */
   del_estr(tmpstr);

   return (eval_result);
}

/*
 * eval_attrref
 *
 * evaluate reference to attribute
 *
 */
static STRPTR eval_attrref(HSCPRC * hp, HSCATTR * destattr)
{
   STRPTR eval_result = NULL;
   HSCATTR *eadest = new_hscattr(PREFIX_TMPATTR "eval_attrname");
   STRPTR nw = eval_attrname(hp,eadest);

   if (nw)
   {
      HSCATTR *refvar = find_varname(hp->defattr, nw);

      if (refvar)
      {
         destattr->quote = refvar->quote;
         destattr->vartype = refvar->vartype;

         eval_result = refvar->text;

         /* check empty/circular reference */
         if (!eval_result)
            hsc_message(hp, MSG_EMPTY_SYMB_REF,
                        "empty reference to %A", destattr);

         /* debugging message */
         DDA(fprintf(stderr, DHL "   %s refers to (%s)\n",
                     destattr->name, refvar->name));
      }
      else
      {
         /* reference to unknown destattr */
         hsc_msg_unkn_attr_ref(hp, nw);
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
      set_vartext(destattr, eval_result);

   del_hscattr(eadest);
   return (eval_result);
}

/*
 * check_color - check if a color constant is legal
 */
static VOID check_color(HSCPRC * hp, HSCATTR * dest, STRPTR value)
{
   BOOL ok = FALSE;
   size_t color_len = strlen("#rrggbb");

   if (value[0] == '#')
   {
      /* check RGB-value */
      if (strlen(value) == color_len)
      {
         size_t i = 1;

         ok = TRUE;
         for (; i < color_len; i++)
         {
            if (!isxdigit(value[i]))
            {
               ok = FALSE;
            }
         }
      }
   }
   else
   {
      /* check color name */
      if (hp->color_names)
      {
         if (strenum(value, hp->color_names, '|', STEN_NOCASE))
            ok = TRUE;
      }
      else
         ok = TRUE;
   }

   if (!ok)
   {
      /* illegal color value */
      hsc_message(hp, MSG_ILLG_COLOR,
                  "illegal color value %q for %A",
                  value, dest);
   }
}

/*
 * check_integer - check if a integer constant is legal
 */
static VOID check_integer(HSCPRC * hp, HSCATTR * dest, STRPTR value)
{
   BOOL ok = FALSE;
   int i = 0;

   if ((value[0] == '+')
       || (value[0] == '-'))
   {
      i = 1;
   }

   if (strlen(value) - i)
   {
      ok = TRUE;
      while (value[i] && ok)
      {
         if (!isdigit(value[i]))
            ok = FALSE;
         else
            i++;
      }
   }

   if (!ok)
   {
      /* illegal integer value */
      hsc_message(hp, MSG_ILLG_NUM,
                  "illegal numeric value %q for %A",
                  value, dest);
   }
}

/*
 * new_cloned_attr
 *
 * create a new attribute and clone the type and flags
 * of another attribute.
 *
 * Normally the cloned attribute has the same name as the
 * initial target attribute, but in debug mode a more
 * informative name is used
 */
static HSCATTR *new_cloned_attr(STRPTR debug_name, HSCATTR * attr)
{
#if DEBUG
   STRPTR cloned_name = debug_name;
#else
   STRPTR cloned_name = attr->name;
#endif
   HSCATTR *cloned_attr = new_hscattr(cloned_name);

   /* init cloned attribute */
   cloned_attr->vartype = attr->vartype;
   cloned_attr->varflag = attr->varflag;
   cloned_attr->quote = attr->quote;

   return cloned_attr;
}

void do_tag_checks(HSCPRC *hp, HSCATTR *dest, STRPTR str)  {
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
         EXPSTR *dest_uri = init_estr(32);

         parse_uri(hp, dest_uri, str);
         set_vartext(dest, estr2str(dest_uri));

         del_estr(dest_uri);
      }
   }
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
 *
 */
STRPTR eval_expression(HSCPRC * hp, HSCATTR * dest, STRPTR endstr)
{
   INFILE *inpf = hp->inpf;

   /* used as destination by eval_string_exprXX() */
   EXPSTR *vararg = init_estr(TMP_STEPSIZE);

   /* create cloned attribute to hold result */
   HSCATTR *dest1 = new_cloned_attr(PREFIX_TMPATTR "1st.operand", dest);

   STRPTR exprstr = NULL;       /* return value */
   int ch;                      /* char read from input */

   /* skip white spaces */
   infskip_ws(inpf);

   /* read dest->quote char */
   ch = infgetc(inpf);
   if (!strchr(VQ_STR_QUOTE, ch)) {
      if (ch != EOF) {
         dest1->quote = VQ_NO_QUOTE;
      } else {
         hsc_msg_eof(hp, "reading attribute");
      }
   } else {
      dest1->quote = ch;
   }

   if (ch == '(') {
      /* process bracket */
      exprstr = eval_expression(hp, dest1, ")");

      /* set generic double quote */
      if (!endstr) {
         dest1->quote = '\"';
      }
   } else if (ch == '{') {
      /* process brace */
      exprstr = eval_expression(hp, dest1, "}");
      inungets(exprstr,inpf);
      exprstr = eval_attrref(hp,dest1);
   } else if (ch != EOF) {
      /* write current char read back to input buffer */
      inungetc(ch, inpf);

      if (dest1->quote != VQ_NO_QUOTE) {
         /* process string expression with quotes */
         exprstr = eval_string_expr(hp, dest1);
      } else if (endstr) {
         BOOL err = FALSE;

         /* try to process unary operator */
         exprstr = try_eval_unary_op(hp, dest1, &err);

         /* process attribute reference */
         if (!exprstr && !err)
            exprstr = eval_attrref(hp, dest1);
      } else {
         /* process string expression without quotes */
         exprstr = eval_string_expr_noquote(hp, dest1);
      }
   }

   if (exprstr && endstr)
   {
      BYTE op;

      /* evaluate operator */
      op = eval_op(hp);

      switch(op) {
         case OP_CL_BRACKET :
         case OP_CL_BRACE :
            DMSG("  END mark operator reached");
            break;
         case OP_NONE :
            DMSG("  NO operator");
            break;
         default :
            /* read second operator */
            if (op != OP_NONE)
            {
               HSCATTR *dest2 = new_cloned_attr(PREFIX_TMPATTR "2nd.operand",
                     dest1);
               STRPTR str1 = get_vartext(dest1);
               STRPTR str2 = NULL;

               /* compute second operand */
               str2 = eval_expression(hp, dest2, endstr);

               if (str2)
                  process_op(hp, dest1, op, str1, str2);
               else
                  exprstr = NULL;

               /* remove temporary resources */
               del_hscattr((APTR) dest2);
            }
            else
            {
               exprstr = NULL;
            }

            if (exprstr)
            {
               /* store result */
               exprstr = get_vartext(dest1);
            }
      }
   }

   if (!endstr)
   {
      /* if (exprstr && !endstr) */
      if (exprstr)
      {
         /* choose quote (only for tag attributes) */
         if ((dest1->vartype != VT_BOOL)
             && !(dest1->varflag & (VF_MACRO | VF_KEEP_QUOTES)))
         {
            choose_quote(hp, dest1);
         }

         /* check enum type */
         if (dest1->vartype == VT_ENUM)
         {
            DDA(fprintf(stderr,DHL "Checking value '%s' against enum '%s'\n",
                  exprstr, dest->enumstr);)
            if (!strenum(exprstr, dest->enumstr, '|', STEN_NOCASE))
            {
               /* unknown enum value */
               hsc_message(hp, MSG_ENUM_UNKN,
                           "unknown value %q for enumerator %A",
                           exprstr, dest);
            }
         }
         else if (dest1->vartype == VT_COLOR)
         {
            /* check color value */
            check_color(hp, dest1, exprstr);
         }
         else if (dest1->vartype == VT_NUM)
         {
            /* check numeric value */
            check_integer(hp, dest1, exprstr);
         }
         else if (dest1->vartype == VT_BOOL)
         {
            /*
             * for boolean attributes, set the name
             * of the attribute if TRUE, or set an
             * empty string, if FALSE
             */
            if (eval_boolstr(exprstr))
               set_vartext(dest1, dest1->name);
            else
               set_vartext(dest1, "");
         }
         do_tag_checks(hp,dest1,exprstr);
         exprstr = get_vartext(dest1);
      }
      else
      {
         /* if error occured,
          * skip until ">", unread ">"
          */
         skip_until_eot(hp, NULL);
         if (!hp->fatal)
            inungetcw(inpf);
      }
   }

   /* assign result to destination attribute */
   if (exprstr)
   {
      set_vartext(dest, exprstr);
      dest->quote = dest1->quote;
      exprstr = get_vartext(dest);
   }

   /* remove temporary resources */
   del_hscattr((APTR) dest1);
   del_estr(vararg);

   return (exprstr);
}

/*
 * assign_conditional_attr
 *
 * validate and find name of source attrib, if set, copy value
 * to destination attribute
 *
 * params: hp...........hsc-process
 *         dest.........detination attribute where to store value
 *         source_attr..name of source attribute
 * result: value of attribute, if it has been set, or NULL
 *         if attribute is empty or unknown or other error
 *         has occured.
 */
static STRPTR assign_conditional_attr(HSCPRC * hp, HSCATTR * dest, STRPTR source_name)
{
   STRPTR attrval = NULL;

   if (source_name)
   {
      HSCATTR *tmpdest = new_hscattr(PREFIX_TMPATTR "check.attrname");

      if (NULL != (source_name = check_attrname(hp,tmpdest,source_name,TRUE)))
      {
         HSCATTR *attr = find_varname(hp->defattr, source_name);

         if (attr)
         {
            attrval = get_vartext(attr);
            dest->quote = attr->quote;
         }
         else
         {
            hsc_msg_unkn_attr_ref(hp, source_name);
         }
      }
      del_hscattr(tmpdest);
   }
   else
   {
      panic("no source attribute");
   }

   /* update attribute value and quotes */
   if (attrval)
   {
      set_vartext(dest, attrval);
      attrval = get_vartext(dest);
      do_tag_checks(hp,dest,attrval);
      if(!(dest->varflag & VF_MACRO)) choose_quote(hp, dest);
   }

   return (attrval);
}

/*
 * eval_conditional_assignment
 *
 * evaluate a conditional expression like
 *   SEPP?=HUGO or SEPP?=("hu"+"go")
 * and modify destination attribute only if the source
 * attribute really exists
 *
 * params: hp...hsc-process
 *         dest..target attribute to update
 * result: new value or NULL in case of no update or error
 */
STRPTR eval_conditional_assignment(HSCPRC * hp, HSCATTR * dest)
{
   STRPTR nw = infgetw(hp->inpf);
   STRPTR attr_val = NULL;

   D(fprintf(stderr, DHL "  conditional assignment\n"));

   if (nw) {
      /* temp. attribute to store name of source attribute if it
       * is specified with an expression */
      HSCATTR *tmp_attr = NULL;
      STRPTR source_name = NULL;    /* name of source attribute */

      if (!strcmp(nw, "(")) {
         /* get attribute name from expression */
         tmp_attr = new_hscattr(PREFIX_TMPATTR "conditional.assignment");
         source_name = eval_expression(hp, tmp_attr, ")");
      } else {
         /* attribute name was simply specified */
         source_name = nw;
      }

      if (source_name) {
         D(fprintf(stderr, DHL "    assign from %s\n", source_name));
         attr_val = assign_conditional_attr(hp, dest, source_name);
      }

      /* free resources */
      if (tmp_attr) {
         del_hscattr(tmp_attr);
      }

   } else {
      hsc_msg_eof(hp, "conditional attribute identifier expected");
   }

   return (attr_val);
}
