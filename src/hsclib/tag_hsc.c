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
 * hsclib/tag_hsc.c
 *
 * tag callbacks for "<$xx>" and related
 * (for macro callbacks, see "tag_macro.c")
 *
 * updated: 23-May-1998
 * created: 23-Jul-1995
 */

#include "hsclib/inc_tagcb.h"

#include "ugly/fname.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/eval.h"
#include "hsclib/include.h"
#include "hsclib/parse.h"
#include "hsclib/uri.h"
#include "hsclib/css.h"

#include "hsclib/tag_macro.h"
#include "hsclib/tag_if.h"

#define TIMEBUF_INC    20
#define ES_STEP_SOURCE 1024

/* states for handle_hsc_source */
#define SRST_TEXT    0          /* inside text      */
#define SRST_LT      1          /* after "<"        */
#define SRST_SLASH   2          /* after "</"       */
#define SRST_CSOURCE 3          /* after "</$SOURCE" */
#define SRST_TAG     8          /* inside a tag     */
#define SRST_COMT    9          /* inside comment   */
#define SRST_ERR    99          /* error occured    */

/* forward reference */
BOOL handle_hsc_include(HSCPRC * hp, HSCTAG * tag);

/*
 *-------------------------------------
 * comment & skip handle (<* *>, <| |>)
 *-------------------------------------
 */

/*
 * handle_hsc_comment
 *
 * skip text until '*>' occures;
 * nested commets are supported
 *
 */
BOOL handle_hsc_comment(HSCPRC * hp, HSCTAG * tag)
{
   skip_hsc_comment(hp, NULL);
   return (FALSE);
}

/*
 * handle_hsc_verbatim
 *
 * copy text until '|>' occures;
 * no syntax check or whatever is performed
 *
 */
BOOL handle_hsc_verbatim(HSCPRC * hp, HSCTAG * tag)
{
   EXPSTR *content = init_estr(256);
   if (skip_hsc_verbatim(hp, content))
   {
      /* remove "|>" from content */
      STRPTR strend = estr2str(content) + estrlen(content);
      while (strend[0] != '|')
      {
         strend--;
      }
      strend[0] = 0;

      /* output content */
      if (content && content->es_data && hp->suppress_output)
      {
         hp_enable_output(hp, "some text");
      }

      hsc_output_text(hp, "", estr2str(content));
   }
   del_estr(content);

   return (FALSE);
}

/*
 * handle_hsc_insert_expression
 *
 * insert value of a hsc-expression
 *
 * TODO: what to use as error-location, when inserting expression?
 */
BOOL handle_hsc_insert_expression(HSCPRC * hp, HSCTAG * tag)
{
   HSCATTR *dest = new_hscattr(PREFIX_TMPATTR "insert.expression");
   STRPTR value = NULL;

   /* prepare dummy attribute */
   dest->vartype = VT_STRING;

   /* compute expression */
   value = eval_expression(hp, dest, ")");
   if (value)
   {
      parse_gt(hp);
      hsc_include_string(hp, SPECIAL_FILE_ID "insert expression", value,
            IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
   }
   del_hscattr(dest);

   return (FALSE);
}

/*
 *-------------------------------------
 * $INCLUDE handle
 *-------------------------------------
 */

static VOID do_include(HSCPRC * hp, STRPTR filename,
      BOOL nostatus, BOOL temporary, BOOL source, BOOL pre,
      LONG indent, LONG tabsize)
{
   EXPSTR *fname = init_estr(0);
   ULONG optn = 0;

   /* compute options */
   if (!temporary)
      optn |= IH_IS_INCLUDE;

   if (!nostatus)
      optn |= IH_NO_STATUS;

   if (source)
      optn |= IH_PARSE_SOURCE;

   /* compute filename (convert from URI if neccessary) */
   conv_uri2path(fname, filename, hp->weenix);

   /* insert leading <PRE> */
   if (pre) {
      hsc_include_string(hp, SPECIAL_FILE_ID "include <PRE>", "<PRE>",
            IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
   }
   /* include main file */
   if (fname)
      hsc_include_file(hp, estr2str(fname), optn);

   /* insert tailing </PRE> */
   if (pre) {
      hsc_include_string(hp, SPECIAL_FILE_ID "include </PRE>", "</PRE>\n",
            IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
   }

   del_estr(fname);
}

/*
 * handle_hsc_include
 *
 * include a sub file
 */
BOOL handle_hsc_include(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR fname_arg = get_vartext_byname(tag->attr, "FILE");
   LONG indent = get_varnum_byname(tag->attr, "INDENT");
   LONG tabsize = get_varnum_byname(tag->attr, "TABSIZE");
   BOOL source = get_varbool_byname(tag->attr, "SOURCE");
   BOOL pre = get_varbool_byname(tag->attr, "PRE");
   BOOL temporary = get_varbool_byname(tag->attr, "TEMPORARY");

   do_include(hp, fname_arg, FALSE /*nostatus */ , temporary,
         source, pre, indent, tabsize);

   return (FALSE);
}

/*
 *-------------------------------------
 * $MESSAGE handle
 *-------------------------------------
 */

/*
 * handle_hsc_error
 *
 * user error message
 */
BOOL handle_hsc_message(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR msg_text = get_vartext_byname(tag->attr, "TEXT");
   STRPTR msg_class = get_vartext_byname(tag->attr, "CLASS");

   if (msg_text)
   {
      ULONG msgid = (MSG_USER_MESSAGE & MASK_MESSAGE);

      /* compute message id */
      if (!upstrcmp(msg_class, "WARNING"))
         msgid |= MSG_WARN;
      else if (!upstrcmp(msg_class, "ERROR"))
         msgid |= MSG_ERROR;
      else if (!upstrcmp(msg_class, "FATAL"))
         msgid |= MSG_FATAL;
      else
      {
         D(if (upstrcmp(msg_class, "NOTE"))
               panic("illegal user message class")
          );
      }

      /* display message */
      hsc_message(hp, msgid, "user message: %s", msg_text);
   }
   return (FALSE);
}

/*
 *-------------------------------------
 * $EXEC handle
 *-------------------------------------
 */

/*
 * handle_hsc_exec
 *
 * execute shell command
 */
BOOL handle_hsc_exec(HSCPRC * hp, HSCTAG * tag)
{
#ifdef AMIGA
#define TMP_PREFIX "t:hsc"
#else
#define TMP_PREFIX "hsc"
#endif
   STRPTR cmd = get_vartext_byname(tag->attr, "COMMAND");
   HSCATTR *file_attr = find_varname(tag->attr, "FILE");
   HSCATTR *remove_attr = find_varname(tag->attr, "REMOVE");
   HSCATTR *result_attr = find_varname(hp->defattr, RESULT_ATTR);

   if (cmd && file_attr && result_attr && remove_attr)
   {
      int result = 0;
      BOOL remove_file = FALSE;
      BOOL read_file = FALSE;
      STRPTR remove_str = get_vartext(remove_attr);
      EXPSTR *msg = init_estr(0);
      EXPSTR *cmdstr = init_estr(32);

      /* additional attributes */
      HSCATTR *temp_attr = find_varname(tag->attr, "TEMPORARY");
      HSCATTR *include_attr = find_varname(tag->attr, "INCLUDE");
      HSCATTR *attribute_attr = find_varname(tag->attr, "ATTRIBUTE");
      STRPTR attribute_name = NULL;
      BOOL temporary = get_varbool(temp_attr);
      BOOL include = get_varbool(include_attr);
      BOOL pre = get_varbool_byname(tag->attr, "PRE");
      BOOL source = get_varbool_byname(tag->attr, "SOURCE");
      LONG indent = get_varnum_byname(tag->attr, "INDENT");
      LONG tabsize = get_varnum_byname(tag->attr, "TABSIZE");

      INFILE *outfile = NULL;
      STRPTR filename = get_vartext(file_attr);
      BOOL usetmpfile = FALSE;
      ULONG old_msg_count = hp->msg_count;

      /* check, if file should be read after execution */
      if (attribute_attr)
         attribute_name = get_vartext(attribute_attr);
      if (attribute_name || include)
         read_file = TRUE;

      /* check, if output should be redirected to temp. file */
      if (!filename && read_file) {
         usetmpfile = TRUE;
         set_vartext(file_attr, tmpnamstr(TMP_PREFIX));
         set_varbool(temp_attr, TRUE);
         filename = get_vartext(file_attr);
         D(fprintf(stderr, DHL "  use temp-file `%s'\n", filename));
      } else {
         D(fprintf(stderr, DHL "  use file `%s'\n", filename));
      }

      /* check, if output-file should be removed */
      if (remove_str) {
         if (!upstrcmp(remove_str, "ON")) {
            remove_file = TRUE;
            temporary = TRUE;
            D(fprintf(stderr, DHL "  auto-temporary (remove=ON)\n"));
         } else {
            if (!upstrcmp(remove_str, "AUTO")) {
               if (hp->msg_count == old_msg_count) {
                  remove_file = temporary;
                  D(if (!remove_file)
                        fprintf(stderr, DHL "  no auto-remove (temp)\n"));
               } else {
                  D(fprintf(stderr, DHL "  no auto-remove (count)\n"));
               }
            }
         }
      }

      /* status message */
      app_estr(msg, "execute: ");
      app_estr(msg, cmd);
      hsc_status_misc(hp, estr2str(msg));

      /* create command string */
      set_estr(cmdstr, cmd);
      if (usetmpfile) {
#ifdef RISCOS
         app_estr(cmdstr, " {");
#endif
         app_estr(cmdstr, " >");
         app_estr(cmdstr, filename);
#ifdef RISCOS
         app_estr(cmdstr, " }");
#endif
      }

      D(fprintf(stderr, DHL "  command=`%s'\n", estr2str(cmdstr)));

      /* call command */
      result = system(estr2str(cmdstr));

      /* check for non-zero-result */
      if (result) {
         hsc_message(hp, MSG_SYSTEM_RETURN,
               "shell-command returned %d", result);
      }

      /* read output to HSC.STDOUT */
      if (read_file) {
         errno = 0;
         outfile = infopen(filename, 512);
         if (outfile) {
            /* read to attribute */
            if (attribute_name) {
               HSCATTR *output_attr =
                  find_varname(hp->defattr, attribute_name);

               D(fprintf(stderr, DHL "  ATTRIB exec-output to `%s'\n",
                        attribute_name));

               if (output_attr)
                  set_vartext(output_attr, infgetall(outfile));
               else
                  hsc_msg_unkn_attr_ref(hp, attribute_name);
            }
            infclose(outfile);

            /* include output */
            if (include) {
               /*handle_hsc_include(hp, tag); */
               BOOL nostatus = usetmpfile;
               D(fprintf(stderr, DHL "  INCLUDE exec-output\n"));
               do_include(hp, get_vartext(file_attr), nostatus,
                     temporary, source, pre, indent, tabsize);
            }
         } else {
            /* couldn't open exec-output file for input */
            hsc_msg_noinput(hp, filename);
         }

         /* check, if output-file should be removed */
         remove_file = FALSE;
         if (remove_str) {
            if (!upstrcmp(remove_str, "ON"))
               remove_file = TRUE;
            if (!upstrcmp(remove_str, "AUTO")) {
               if (hp->msg_count == old_msg_count) {
                  remove_file = temporary;
                  D(if (!remove_file)
                        fprintf(stderr, DHL "  no auto-remove (temp)\n"));
               } else {
                  D(fprintf(stderr, DHL "  no auto-remove (count)\n"));
               }
            }
         }

         /* remove output file */
         if (remove_file) {
            D(fprintf(stderr, DHL "  remove `%s'\n", filename));
            errno = 0;
            remove(filename);
            if (errno) {
               hsc_message(hp, MSG_REMOVE_FAILED,
                     "error removing file `%s': %s",
                     filename, strerror(errno));
               errno = 0;
            }
         }
      } else {
         D(fprintf(stderr, DHL "  don't read exec-output\n"));
      }

      /* update result-attribute */
      if (result_attr) {
         set_vartext(result_attr, long2str((LONG) result));
      } else {
         D(panic("no result-attribute"));
      }

      del_estr(cmdstr);
      del_estr(msg);
   }
   else
      panic("attribute missing");

   return (FALSE);
}

/*
 *-------------------------------------
 * $EXPORT handle
 *-------------------------------------
 */

/*
 * handle_hsc_export
 *
 * write string to file
 */
BOOL handle_hsc_export(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR filename = get_vartext_byname(tag->attr, "FILE");
   STRPTR data = get_vartext_byname(tag->attr, "DATA");
   BOOL append = get_varbool_byname(tag->attr, "APPEND");
   BOOL relsrc = get_varbool_byname(tag->attr, "RELSRC");
   EXPSTR *real_filename = init_estr(1);

   if (filename && data) {
      FILE *outfile = NULL;
      STRPTR writemode = "w";

      if (append)
         writemode = "a";

      if(relsrc)
         set_estr(real_filename,get_vartext(find_varname(hp->defattr,"HSC.SOURCE.PATH")));
      else
         clr_estr(real_filename);
      app_estr(real_filename,filename);
      errno = 0;
      outfile = fopen(estr2str(real_filename), writemode);
      if (outfile) {
         fwrite(data, sizeof(char), strlen(data), outfile);
         fclose(outfile);
      }

      if (errno) {
         hsc_message(hp, MSG_IOERROR, "error opening/writing %q: %s",
               estr2str(real_filename), strerror(errno));
      }
   } else {
      panic("attribute missing");
   }
   del_estr(real_filename);
   return (FALSE);
}

/*
 *-------------------------------------
 * $INSERT handle
 *-------------------------------------
 */

/*
 * handle_hsc_time
 *
 * insert current time
 */
BOOL handle_hsc_time(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR timefmt = get_vartext_byname(tag->attr, "FORMAT");
   EXPSTR *timebuf = init_estr(TIMEBUF_INC);
   BOOL strftrc = 0;           /* result of strftime() */
   size_t i;                   /* loop var */

   /* set default time format */
   if (!timefmt)
      timefmt = "%d-%b-%Y, %H:%M";

   while (!(hp->fatal) && !strftrc)
   {
      /* expand timebuffer */
      for (i = 0; i < TIMEBUF_INC; i++)
         app_estrch(timebuf, '.');

      D(fprintf(stderr, DHL "  timebuf: inc+%d\n", TIMEBUF_INC));

      /* output time */
      strftrc = strftime(estr2str(timebuf), estrlen(timebuf),
            timefmt, localtime(&(hp->start_time)));
   }
   if (strftrc)
   {
      INFILEPOS *base = new_infilepos(hp->inpf);
      hsc_base_include_string(hp, SPECIAL_FILE_ID "insert time",
            estr2str(timebuf),
            IH_PARSE_HSC | IH_NO_STATUS, base);
      del_infilepos(base);
   }
   del_estr(timebuf);

   return (FALSE);
}

/*
 * handle_hsc_text
 *
 * insert text
 */
BOOL handle_hsc_text(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR text = get_vartext_byname(tag->attr, "TEXT");

   /* include text */
   INFILEPOS *base = new_infilepos(hp->inpf);
   hsc_base_include_string(hp, SPECIAL_FILE_ID "insert TEXT", text,
         IH_PARSE_HSC | IH_NO_STATUS, base);
   del_infilepos(base);

   return (FALSE);
}

/*
 * hsc_insert
 *
 * main insert handle
 */
BOOL handle_hsc_insert(HSCPRC * hp, HSCTAG * tag)
{
   BOOL insert_text = FALSE;
   BOOL insert_time = get_varbool_byname(tag->attr, HSC_TIME_STR);

   if (get_vartext_byname(tag->attr, HSC_TEXT_STR))
      insert_text = TRUE;

   if (insert_text)
      handle_hsc_text(hp, tag);
   else if (insert_time)
      handle_hsc_time(hp, tag);
   else
   {

      /* unknown option for $insert */
      hsc_message(hp, MSG_MISS_REQ_ATTR,
            "required attribute for %t missing", HSC_INSERT_STR);
   }                           /* clear attributes */
   clr_varlist(tag->attr);

   return (FALSE);
}

/*
 *-------------------------------------
 * <$DEFTAG> define a new tag
 *-------------------------------------
 */
BOOL handle_hsc_deftag(HSCPRC * hp, HSCTAG * tag)
{
   BOOL ok = FALSE;
   BOOL open_tag = FALSE;

   tag = def_tag_name(hp, &open_tag);
   ok = (tag && def_tag_args(hp, tag));

   return (FALSE);
}

/*
 *-------------------------------------
 * <$DEFENT> define a new entity
 *-------------------------------------
 */
static VOID msg_illegal_defent(HSCPRC * hp, STRPTR msg) {
   hsc_message(hp, MSG_ILLG_DEFENT,
         "illegal entity definition (%s)", msg);
}

static VOID msg_dubious_defent(HSCPRC * hp, STRPTR msg) {
   hsc_message(hp, MSG_DEFENT_WARN,
         "dubious entity definition (%s)", msg);
}

BOOL handle_hsc_defent(HSCPRC * hp, HSCTAG * tag) {
   STRPTR name = get_vartext_byname(tag->attr, "NAME");
   STRPTR rplc = get_vartext_byname(tag->attr, "RPLC");
   STRPTR nums = get_vartext_byname(tag->attr, "NUM");
   BOOL prefnum= get_varbool_byname(tag->attr, "PREFNUM");
   LONG num = 0;

   if (nums) {
      if (str2long(nums, &num)) {
         if ((NULL == rplc) || (strlen(rplc) <= 1)) {
            if ((num >= 128) && (num <= 65535)) {
               DLNODE *nd = NULL;
               STRPTR lcname = strclone(name);

               lowstr(lcname);
               if((num > 255) && (NULL != rplc) && strlen(rplc)) {
                  msg_dubious_defent(hp, "RPLC specified for NUM > 255");
               }
               if(NULL != (nd = find_dlnode(hp->defent->first, (APTR)lcname, cmp_strent))) {
                  if(num == ((HSCENT*)(dln_data(nd)))->numeric) {
                     msg_dubious_defent(hp, "duplicate entity - updating flags");
                     ((HSCENT*)(dln_data(nd)))->prefnum = prefnum;
                     ((HSCENT*)(dln_data(nd)))->replace[0] = *rplc;
                  } else
                     msg_illegal_defent(hp, "illegal entity redefinition");
               } else if(NULL != (nd = find_dlnode(hp->defent->first, (APTR)num, cmp_nument))) {
                  if(0 == strcmp(lcname,((HSCENT*)(dln_data(nd)))->name)) {
                     msg_dubious_defent(hp, "duplicate NUM - updating flags");
                     ((HSCENT*)(dln_data(nd)))->prefnum = prefnum;
                     ((HSCENT*)(dln_data(nd)))->replace[0] = *rplc;
                  } else
                     msg_illegal_defent(hp, "illegal entity redefinition");
               } else
                  add_ent(hp->defent, lcname, rplc[0], num, prefnum);
               freestr(lcname);
            } else
               msg_illegal_defent(hp, "illegal range for NUM (must be 128<=NUM<=65535)");
         } else
            msg_illegal_defent(hp, "RPLC not a single character");
      } else
         msg_illegal_defent(hp, "illegal value for NUM");
   } else
      msg_illegal_defent(hp, "NUM missing");

   return (FALSE);
}

/*
 *-------------------------------------
 * <$DEFSTYLE> define a new CSS style
 *-------------------------------------
 */
BOOL handle_hsc_defstyle(HSCPRC *hp, HSCTAG *tag)
{
   app_dlnode(hp->defstyle,
         new_styleattr(
            get_vartext_byname(tag->attr, "NAME"),
            get_vartext_byname(tag->attr, "VAL")));
   return FALSE;
}

/*
 *-------------------------------------
 * <$DEFICON> define a new icon-entity
 *-------------------------------------
 */
BOOL handle_hsc_deficon(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR name = get_vartext_byname(tag->attr, "NAME");
   DLNODE *nd = NULL;

   nd = find_dlnode(hp->defent->first, (APTR) name, cmp_strent);
   if (nd)
      msg_illegal_defent(hp, "duplicate entity");
   else
      add_ent(hp->defent, name, '\0', ICON_ENTITY, FALSE);

   return (FALSE);
}
/*
 *-------------------------------------
 * <$DEFINE> create a new (global) attribute
 *-------------------------------------
 */
BOOL handle_hsc_define(HSCPRC * hp, HSCTAG * tag)
{
   HSCVAR *attr = define_attr_by_hp(hp, NULL, 0);
   if (attr)
   {
      DDA(prt_varlist(hp->defattr, "attributes after $DEFINE"));

      /* check for closing ">" */
      parse_gt(hp);
   }

   return (FALSE);
}

/*
 *-------------------------------------
 * $LAZY handle
 *-------------------------------------
 */
static HSCTAG *def_lazy_name(HSCPRC *hp)
{
   STRPTR nw = NULL;
   HSCTAG *lazy = NULL;
   DLLIST *lazy_list = hp->deflazy;

   /* get lazy name */
   nw = infget_tagid(hp);

   /* create new lazy */
   if (nw)
   {
      lazy = find_strtag(lazy_list, nw);
      if (lazy)
      {
         hsc_message(hp, MSG_REDEFINE_LAZY, "redefined lazy ", lazy);
      }
      else
      {
         /* create a new opening lazy */
         lazy = app_tag(lazy_list, nw);
      }
   }                           /* err_eof already called in infget_tagid() */

   return (lazy);
}


BOOL handle_hsc_lazy(HSCPRC * hp, HSCTAG * tag)
{
   BOOL ok = FALSE;
   HSCTAG *lazy = def_lazy_name(hp);
   if (lazy)
   {
      ok = def_tag_args(hp, lazy);
   }

   return (FALSE);
}


/*
 * handle_hsc_depend
 *
 * add dependency to current document
 */
BOOL handle_hsc_depend(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR filename = get_vartext_byname(tag->attr, "ON");
   BOOL file = get_varbool_byname(tag->attr, "FILE");

   if (filename)
   {
      EXPSTR *dest_fname = init_estr(64);

      /* convert URI to local filename */
      if (!file)
      {
         conv_hscuri2file(hp, dest_fname, filename);
         filename = estr2str(dest_fname);
      }

      /* add dependency */
      D(fprintf(stderr, DHL "  add dependency `%s'\n", filename));
      app_include(hp->project->document, filename);

      del_estr(dest_fname);
   }
   else
   {
      panic("attribute missing");
   }

   return (FALSE);
}

/*
 *
 */

/*
 *-------------------------------------
 * <$LET> set/reset/clear attribute value
 *-------------------------------------
 */
BOOL handle_hsc_let(HSCPRC * hp, HSCTAG * tag)
{
   INFILE *inpf = hp->inpf;
   STRPTR varname = infgetw(inpf);
   HSCVAR *attr = NULL;
   BOOL ok = FALSE;

   if (varname) {
      /* create temporary dummy attribute that is
       * used to store the value, if attribute
       * passed is a constant
       */
      HSCVAR *dummy = new_hscattr(PREFIX_TMPATTR "let");

      if('{' == varname[0])
         varname = eval_expression(hp,dummy,"}");

      /* find attribute */
      attr = find_varname(hp->defattr, varname);
      if (attr) {
         STRPTR eq_sign = infgetw(inpf);

         if (attr->varflag & VF_CONST) {
            /* tried to set constant */
            hsc_message(hp, MSG_ATTR_CONST,
                  "attempt to modify constant %A", attr);

            /* assign destination to dummy attribute */
            attr = dummy;
            dummy->vartype = attr->vartype;
            dummy->varflag = attr->varflag;
         }

         /* check if a "=" comes next */
         if (eq_sign) {
            if (!strcmp(eq_sign, "=")) {
               STRPTR brace = infgetw(inpf);
               if(!strcmp(brace,"{")) {
                  /* assigning from a symbolic reference */
                  brace = eval_expression(hp, attr, "}");
                  inungets(brace,inpf);
                  eval_attrref(hp,attr);
               } else {
                  /* normal assignment */
                  inungets(brace,inpf);
                  eval_expression(hp, attr, NULL);
               }
            } else if (!strcmp(eq_sign, "?")) {
               /* conditional assignment */
               if (parse_eq(hp))
                  eval_conditional_assignment(hp, attr);
            } else {
               /* N->clear or reset attribute to default value */
               clr_vartext(attr);
               /* write previous word back (should be ">") */
               inungetcw(inpf);
            }

            DDA(prt_varlist(hp->defattr, "attributes after $LET"));
         }
         /* check for closing ">" */
         ok = parse_gt(hp);
      } else {
         hsc_msg_unkn_attr_ref(hp, varname);
      }

      /* remove dummy attribute */
      del_hscattr(dummy);

   }
   return (FALSE);
}

/*
 *-------------------------------------
 * <$SOURCE> include a source part
 *-------------------------------------
 */
BOOL handle_hsc_source(HSCPRC * hp, HSCTAG * tag)
{
   BOOL pre = get_varbool_byname(tag->attr, "PRE");
   BOOL ok = TRUE;
   EXPSTR *source_content = init_estr(ES_STEP_SOURCE);
   INFILEPOS *base = new_infilepos(hp->inpf);

   /* avoid nesting of <PRE> */
   if (hp->inside_pre)
   {
      pre = FALSE;            /* TODO: lauch warning */
   }

   /* insert leading <PRE> */
   if (pre)
   {
      hsc_include_string(hp, SPECIAL_FILE_ID "insert <PRE>", "<PRE>",
            IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
   }

   /* read source text (until </$source> found) */
   ok = skip_until_tag(hp, source_content, NULL, NULL, HSC_SOURCE_STR,
         SKUT_NO_CONTENT_TAGFOUND | SKUT_NO_ANALYSE_TAGS);

   /* include source */
   if (ok)
   {
      /* include pseudo-file */
      hsc_base_include_string(hp, SPECIAL_FILE_ID "source",
            estr2str(source_content),
            IH_PARSE_SOURCE | IH_NO_STATUS, base);

      /* insert tailing </PRE> */
      if (pre)
      {
         hsc_include_string(hp, SPECIAL_FILE_ID "insert </PRE>",
               "</PRE>\n",
               IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
      }
   }
   del_infilepos(base);
   del_estr(source_content);

   return (FALSE);
}

/*
 *-------------------------------------
 * <$StripWS> strip white spaces
 *-------------------------------------
 */
BOOL handle_hsc_stripws(HSCPRC * hp, HSCTAG * tag)
{
   STRPTR strip_type = get_vartext_byname(tag->attr, "TYPE");
   BOOL strip_prev = FALSE;
   BOOL strip_succ = FALSE;

   /* determine what to strip */
   if (!upstrcmp(strip_type, STRIPWS_BOTH))
   {
      strip_prev = TRUE;
      strip_succ = TRUE;
   }
   else if (!upstrcmp(strip_type, STRIPWS_PREV))
   {
      strip_prev = TRUE;
   }
   else if (!upstrcmp(strip_type, STRIPWS_SUCC))
   {
      strip_succ = TRUE;
   }
   else if (!upstrcmp(strip_type, STRIPWS_NONE))
   {
      /* nufin, use defaults */
   }

   /* now strip it */
   if (strip_prev)
   {
      clr_estr(hp->whtspc);
   }

   if (strip_succ)
   {
      hsc_output_text(hp, NULL, NULL);        /* flush current white spaces */
      hp->strip_next_whtspc = TRUE;
   }

   return (FALSE);
}
