/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * hsclib/deftag.c
 *
 * define new tag from input file
 *
 * updated: 11-Jan-1998
 * created: 13-Oct-1995
 */

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/eval.h"
#include "hsclib/input.h"
#include "hsclib/skip.h"

/*
 *-------------------------------------
 * define a new tag from input file
 *-------------------------------------
 */

/*
 * def_tag_name
 *
 */
HSCTAG *def_tag_name(HSCPRC * hp, BOOL * start_tag)
{
   STRPTR nw = NULL;
   HSCTAG *tag = NULL;
   DLLIST *taglist = hp->deftag;

   /* get tag name */
   nw = infget_tagid(hp);

   /* create new tag */
   if (nw)
   {
      *start_tag = (BOOL) (strcmp(nw, "/"));
      if (!(*start_tag))
      {
         /* add closing tag */
         nw = infget_tagid(hp);
         if (nw)
         {
            tag = find_strtag(taglist, nw);
            if (tag)
            {
               if ((tag->option & HT_CLOSE)
                     ||
                     (tag->option & HT_CONTENT))
               {
                  /* tried to redefine end tag */
                  tag = NULL;
                  hsc_message(hp, MSG_REDEFINE_ENDTAG,
                        "redefined %c", nw);
               }
               else
               {
                  /* mark macro as a container */
                  tag->option |= HT_CLOSE;
               }
            }
            else
            {
               /* tried to define end tag without previous start tag */
               tag = NULL;
               hsc_message(hp, MSG_DEFTAG_NO_OPEN,
                     "no start tag for %c", nw);
            }
         }                   /* err_eof already called in infget_tagid() */
      }
      else
      {
         tag = find_strtag(taglist, nw);
         if (tag)
         {
            /* find tag-node in list to delete it
             * NOTE: this is rather stupid, 'cause the list
             * has to be searched twice this way; but who cares? */
            DLNODE *nd = find_dlnode(hp->deftag->first,
                  (APTR) nw, cmp_strtag);

            /* new tag/macro replaces old tag/macro */
            tag->occured = FALSE;

            hsc_message(hp, MSG_REDEFINE_TAG, "redefined %T", tag);

            del_dlnode(hp->deftag, nd);

         }

         /* create a new opening tag */
         tag = app_tag(taglist, nw);
      }
   }                           /* err_eof already called in infget_tagid() */

   return (tag);
}

/*
 * check_tag_option
 *
 * check if a tag-option-string is equal to an id/short id.
 * if so, set the corresponding option value within the tag.
 *
 * params: option..option string to check for (read from input)
 *         tag.....tag to update option value for
 *         id......id string of option (eg "REQUIRED")
 *         sid.....short id string (eg "REQ")
 *         value...option value to OR with old tag's option value
 * result: TRUE, if tag's option value updated
 */
static BOOL check_tag_option(HSCPRC * hp, STRPTR option, HSCTAG * tag,
      STRPTR id, STRPTR sid, ULONG value)
{
   BOOL found = FALSE;

   if (!((upstrcmp(option, id)) && (upstrcmp(option, sid))))
   {
      DDT(fprintf(stderr, DHL "  option %s\n", id));
      tag->option |= value;
      found = TRUE;
   }

   return (found);

}

/*
 * parse_lazy_option
 *
 * allowed abbrevations:
 *
 * c  CLASS:string
 * d  DIR:enum("ltr|rtl")
 * h  HREF:uri
 * i  ID:id
 * k  CLEAR:enum("left|right|all|none")
 * l  LANG:enum(...)
 * m  MD:string
 * s  SRC:URI
 * t  TITLE:string
 * y  STYLE:string
 */
static HSCATTR *def_lazy_attr(HSCPRC * hp, HSCTAG * tag,
      STRPTR attrname, BYTE attrtype, ULONG flags)
{
   HSCATTR *newattr = app_var(tag->attr, attrname);
   DDA(fprintf(stderr, DHL "new attr: `%s'\n", attrname));
   newattr->vartype = attrtype;
   newattr->varflag = flags;
   return (newattr);
}

static BOOL parse_lazy_option(HSCPRC * hp, HSCTAG * tag, STRPTR lazy)
{
   BOOL ok = TRUE;
   HSCATTR *attr;

   while (lazy[0])
   {
      switch (lazy[0])
      {
         case 'c':
            def_lazy_attr(hp, tag, "CLASS", VT_STRING, 0);
            break;
         case 'd':
            attr = def_lazy_attr(hp, tag, "DIR", VT_ENUM, 0);
            attr->enumstr = strclone("ltr|rtl");
            break;
         case 'h':
            def_lazy_attr(hp, tag, "HREF", VT_URI, 0);
            break;
         case 'i':
            def_lazy_attr(hp, tag, "ID", VT_ID, 0);
            break;
         case 'k':
            attr = def_lazy_attr(hp, tag, "CLEAR", VT_ENUM, 0);
            attr->enumstr = strclone("left|right|all|none");
            break;
         case 'l':
            def_lazy_attr(hp, tag, "LANG", VT_STRING, 0);
            break;
         case 'm':
            /* what's this supposed to be? -mb */
            def_lazy_attr(hp, tag, "MD", VT_STRING, 0);
            break;
         case 's':
            def_lazy_attr(hp, tag, "SRC", VT_URI, 0);
            break;
         case 't':
            def_lazy_attr(hp, tag, "TITLE", VT_STRING, 0);
            break;
         case 'v':
            /* bulk declaration for all %events in HTML4 */
            def_lazy_attr(hp, tag, "ONCLICK", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONDBLCLICK", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONMOUSEDOWN", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONMOUSEUP", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONMOUSEOVER", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONMOUSEMOVE", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONMOUSEOUT", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONKEYDOWN", VT_STRING, 0);
            def_lazy_attr(hp, tag, "ONKEYUP", VT_STRING, 0);
            break;
         case 'y':
            def_lazy_attr(hp, tag, "STYLE", VT_STRING, 0);
            break;
         default:
            hsc_message(hp, MSG_UNKN_TAG_OPTION,
                  "unknown tag modifier %q (arg %q)",
                  "LAZY", ch2str(lazy[0]));
            break;
      }
      lazy++;
   }

   return (ok);
}

/*
 * parse_tag_option
 *
 * check a tag-modifier, read additional values (if necessary), update
 * tag structure
 *
 * params: hp......hsc process
 *         option..string that contains modifier (eg. "REQUIRED")
 *         tag.....tag to modify
 * result: TRUE, if modifier could be handled
 * errors: return FALSE, output message
 */
static BOOL parse_tag_option(HSCPRC * hp, STRPTR option, HSCTAG * tag) {
   BOOL ok = FALSE;
   HSCATTR *attr = new_hscattr(PREFIX_TMPATTR "mbi.naw");

   attr->vartype = VT_STRING;

   if (!(upstrcmp(option, TO_MBI_STR)
            && upstrcmp(option, TO_MBI_SHT)))
   {                           /* must be inside */
      if (parse_eq(hp)) {
         STRPTR strmbi = eval_expression(hp, attr, NULL);

         if (strmbi) {
            tag->mbi = strclone(strmbi);
            DDT(fprintf(stderr, DHL "  mbi = `%s'\n", tag->mbi));
            ok = TRUE;
         }
      }
   } else if (!(upstrcmp(option, TO_NAW_STR)
            && upstrcmp(option, TO_NAW_SHT)))
   {                           /* not allowed within */
      if (parse_eq(hp)) {
         STRPTR strnaw = eval_expression(hp, attr, NULL);

         if (strnaw) {
            tag->naw = strclone(strnaw);
            DDT(fprintf(stderr, DHL "  mbi = `%s'\n", tag->naw));
            ok = TRUE;
         }
      }
   } else if (!(upstrcmp(option, TO_LAZY_STR)
            && upstrcmp(option, TO_LAZY_SHT)))
   {                           /* lazy standard attribs */
      if (parse_eq(hp)) {
         STRPTR strlazy = eval_expression(hp, attr, NULL);

         if (strlazy) {
            DDT(fprintf(stderr, DHL "  lazy= `%s'\n", strlazy));
            ok = parse_lazy_option(hp, tag, strlazy);
         }
      }
   } else {
      ok |= check_tag_option(hp, option, tag, TO_CLOSE_STR, TO_CLOSE_SHT,
            HT_CLOSE);

      /* now check for all the other stuff */
      ok |= check_tag_option(hp, option, tag,
            TO_SPECIAL_STR, TO_SPECIAL_SHT, HT_SPECIAL);
      ok |= check_tag_option(hp, option, tag,
            TO_JERK_STR, TO_JERK_SHT, HT_JERK);
      ok |= check_tag_option(hp, option, tag,
            TO_AUTOCLOSE_STR, TO_AUTOCLOSE_SHT,
            (ULONG)(hp->xhtml ? HT_CLOSE : HT_AUTOCLOSE));
      ok |= check_tag_option(hp, option, tag,
            TO_EMPTY_STR, TO_EMPTY_SHT, HT_EMPTY);
      ok |= check_tag_option(hp, option, tag,
            TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE);
      ok |= check_tag_option(hp, option, tag,
            TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE);
      ok |= check_tag_option(hp, option, tag,
            TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED);
      ok |= check_tag_option(hp, option, tag,
            TO_RECOMMENDED_STR, TO_RECOMMENDED_SHT, HT_RECOMMENDED);
      ok |= check_tag_option(hp, option, tag,
            TO_SKIPLF_STR, TO_SKIPLF_SHT, HT_SKIPLF);
      ok |= check_tag_option(hp, option, tag,
            TO_WHTSPC_STR, TO_WHTSPC_SHT, HT_WHTSPC);

      /* in XHTML mode, HT_EMPTY excludes HT_CLOSE */
      if((tag->option & HT_EMPTY) && hp->xhtml) {
         DDT(if(tag->option & HT_CLOSE)
               fprintf(stderr, DHL "  EMPTY set, disabling CLOSE\n");)
            tag->option &= ~HT_CLOSE;
      }

      if (!ok)
         hsc_message(hp, MSG_UNKN_TAG_OPTION,
               "unknown tag modifer %q", option);
   }

   /* remove temp. attribute */
   del_hscattr(attr);

   return (ok);
}

/*
 * parse_tag_var
 */
static BOOL parse_tag_var(HSCPRC * hp, HSCTAG * tag) {
   BOOL ok = FALSE;
   HSCATTR *var = NULL;

   /* define new attribute */
   var = define_var(hp, tag->attr, VF_CONST | VF_GLOBAL);

   /* set several values of tag structure, if attribute has
    * some special flags set
    */
   if (var) {
      /* attribute is uri that tells the size */
      if (var->varflag & VF_GETSIZE)
         tag->uri_size = var;

      /* attribute is uri that tells if the tag should be stripped */
      if (var->varflag & VF_STRIPEXT)
         tag->uri_stripext = var;

      /* set attribute flag to keep quotes */
      if (tag->option & HT_KEEP_QUOTES)
         var->varflag |= VF_KEEP_QUOTES;

      /* set macro attribute flag for macro tags */
      if (tag->option & HT_MACRO)
         var->varflag |= VF_MACRO;

      ok = TRUE;
   }

   return (ok);
}

/*
 * def_tag_args
 *
 */
BOOL def_tag_args(HSCPRC * hp, HSCTAG * tag) {
   BOOL ok = FALSE;
   STRPTR nw;
   INFILE *inpf = hp->inpf;

   if (tag) {
      ok = TRUE;

      /* read args */
      nw = infgetw(inpf);

      /*
       * set tag options
       */
      while (nw && (!strcmp(nw, "/"))) {
         nw = infgetw(inpf);
         if (nw) {
            ok &= parse_tag_option(hp, nw, tag);
            nw = infgetw(inpf);
         }
      }

      /* auto-set HT_KEEP_QUOTES */
      if (!strncmp(tag->name, HSC_TAGID, strlen(HSC_TAGID)))
         tag->option |= HT_KEEP_QUOTES;

      /*
       * set tag attributes
       */
      while (nw && (strcmp(nw, ">"))) {
         if (strcmp(nw, "[")) {
            /* define classic attribute */
            inungetcw(inpf);
            ok &= parse_tag_var(hp, tag);
         } else {
            /* insert attribute list */
            STRPTR name = infget_tagid(hp);
            if (nw) {
               HSCTAG *lazy = find_strtag(hp->deflazy, name);
               if (lazy)
                  copy_local_varlist(tag->attr, lazy->attr, MCI_GLOBAL);
               else
                  hsc_message(hp, MSG_UNKN_LAZY, "unknown %l", name);
            }
            parse_wd(hp, "]");
         }
         nw = infgetw(inpf);

      }

      /* check for ">" at end */
      if (nw) {
         inungetcw(inpf);
         ok = parse_gt(hp);
      }
   }

   return (ok);
}

/*
 * set_tag_arg
 *
 * parse & set one single tag argument
 *
 */
static BOOL set_tag_arg(HSCPRC * hp, DLLIST * varlist, STRPTR varname, STRPTR tagname, BOOL tag_unknown, BOOL is_macro_tag) {
   HSCATTR *attr = find_varname(varlist, varname);
   INFILE *inpf = hp->inpf;
   STRPTR arg = NULL;
   BOOL ok = FALSE;
   BOOL inheritage_failed = FALSE;     /* flag: set, if "?=" failed */
   STRPTR nw;
   HSCATTR skipvar;            /* dummy-attribute to skip unknown */
   EXPSTR *attr_str = init_estr(40);   /* string for attribute name */
   EXPSTR *val_str = init_estr(40);    /* string for "=" and value */
   const BOOL is_styleattr = (0 == upstrcmp(varname,"STYLE"));
   
   DAV(fprintf(stderr, DHL "   set attr %s\n", varname));

   /* don't process pseudo-attribute "/" in XHTML mode */
   if(!strcmp(varname,"/")) return TRUE;

   /* append attribute name to attr_str */
   if (hp->compact)
      app_estr(attr_str, " ");
   else
      app_estr(attr_str, infgetcws(inpf));

   app_estr(attr_str, infgetcw(inpf));

   /* lowercase attribute name if requested */
   if(hp->lctags)
      lowstr(estr2str(attr_str));

   if (!attr) {
      /* attribute not found: assign to dummy-attribute */
      attr = &skipvar;
      attr->name = varname;
      attr->deftext = NULL;
      attr->text = NULL;
      attr->enumstr = NULL;
      attr->vartype = VT_STRING;
      attr->varflag = 0;

      /* launch message about unknown attribute
       *
       * if the whole tag is unknown, no message is launched;
       * if it is a normal tag, this causes a warning
       * if it is a macro tag, it causes an error */
      if (!tag_unknown) {
         if (is_macro_tag)
            hsc_msg_unkn_attr_macro(hp, varname, tagname);
         else
            hsc_msg_unkn_attr_tag(hp, varname, tagname);
      }
   }

   /* get argument */
   nw = infgetw(inpf);
   if (nw) {
      if (!strcmp(nw, "=")) {
         /* append "=" to log - always strips WS b/w attribute and value */
         if(!is_styleattr) app_estr(val_str, infgetcw(inpf));

         /* parse expression */
         arg = eval_expression(hp, attr, NULL);

         /* append value to log */
         if(!is_styleattr && (attr->quote != VQ_NO_QUOTE))
            app_estrch(val_str, attr->quote);
         if(get_vartext(attr))
            app_estr(val_str, get_vartext(attr));
         if (!is_styleattr && (attr->quote != VQ_NO_QUOTE))
            app_estrch(val_str, attr->quote);
 

         if (arg) {
            DAV(fprintf(stderr, DHL "  `%s'\n", arg));
            ok = TRUE;
         }
      } else if (!strcmp(nw, "?")) {
         /* process "?="-assignment */
         if (!hp->compact)
            app_estr(val_str, infgetcws(inpf));
         if (parse_eq(hp)) {
            if(!is_styleattr)
               app_estr(val_str, "=");

            arg = eval_conditional_assignment(hp, attr);

            /* append value to log */
            if(!is_styleattr && (attr->quote != VQ_NO_QUOTE))
               app_estrch(val_str, attr->quote);
            if (get_vartext(attr))
               app_estr(val_str, get_vartext(attr));
            if (!is_styleattr && (attr->quote != VQ_NO_QUOTE))
               app_estrch(val_str, attr->quote);

            if (arg) {
               DAV(fprintf(stderr, DHL "  inherited `%s'\n", arg));
            } else {
               DAV(fprintf(stderr, DHL "  inheritage failed\n"));
               inheritage_failed = TRUE;
            }
            ok = TRUE;
         }
      } else {
         /* handle boolean attribute */
         arg = NULL;
         inungetcwws(inpf);
         if (attr == &skipvar)
            attr->vartype = VT_BOOL;
         ok = TRUE;
      }
   } else hsc_msg_eof(hp, "read attribute value");

   if (ok) {
      if (arg) {
         if (attr->vartype == VT_BOOL) {
            /* set boolean attribute depending on expression */
            set_varbool(attr, get_varbool(attr));

            /* if the expression returned FALSE, remove
             * the boolean  switch from tag-call
             */
            if (!get_varbool(attr))
               clr_estr(attr_str);
         } else if (!inheritage_failed) {
            /* append value to attribute string */
            estrcat(attr_str, val_str);
         }
      } else if (inheritage_failed) {
         /* if attribute to inherit from was empty,
          * remove the attribute from tag-call
          */
         clr_estr(attr_str);
      } else {
         /* no value has been passed to the attribute */
         if (attr->vartype == VT_BOOL) {
            /* for boolean attributes, this is legal,
             * and enables the attribute
             * but: see below for XHTML normalization!
             */
            set_varbool(attr, TRUE);
         } else if (!tag_unknown) {
            /* for non-boolean attributes, display
             * error message
             */
            hsc_message(hp, MSG_NOARG_ATTR,
                  "missing value for %A", attr);
         }
      }
   }

   /* for XHTML, normalize boolean attributes */
   if(hp->xhtml && (VT_BOOL == attr->vartype)) {
      char *s,*t,*q;
      /* choose quotes */
      q = (QMODE_SINGLE == hp->quotemode) ? "'" : "\"";
      /* clone attribute string */ 
      s = t = strclone(estr2str(attr_str));
      /* skip leading blanks */
      while(isspace(*t)) ++t;
      /* turn attr into attr="attr" or attr='attr' */
      app_estr(attr_str,"=");
      app_estr(attr_str,q);
      app_estr(attr_str,t);
      app_estr(attr_str,q);
      ufreestr(s);
   } else if(hp->lctags && (VT_ENUM == attr->vartype)) {
      /* lowercase entire attribute + value for ENUM attributes if req. */
      lowstr(estr2str(attr_str));
   }

   /* warn on obsolete attribute */
   if((attr->varflag & VF_OBSOLETE) &&
      (strlen(estr2str(val_str)))   &&
      (strlen(estr2str(attr_str)))) {
      hsc_message(hp,MSG_ATTR_OBSOLETE,"%A is obsolete",attr);
   }

   /* cleanup pseudo-attr */
   if (attr == &skipvar)
      clr_vartext(attr);

   if(is_styleattr && strlen(estr2str(val_str)) && strlen(estr2str(attr_str))) {
      /* attr_str contains gibberish here, don't use it */
      BOOL done = FALSE;
      STRPTR cstyle, nstyle, value;

      cstyle = estr2str(val_str);
      /* TODO: move this to css.c */
      do {
         /* check if there is more than one property-value-pair in
          * this string
          */ 
         if(NULL != (nstyle = strchr(cstyle,';'))) {
            /* terminate string there */
            *nstyle++ = '\0';
            /* skip leading blanks of next style pair */
            while(isspace(*nstyle)) ++nstyle;
            /* if rest is empty, we're done anyway */
            if('\0' == *nstyle)
               done = TRUE;
         } else {
            done = TRUE;
         }
         value = strchr(cstyle,':');
         if(NULL != value) {
            /* separate property/value */
            *value++ = '\0';
            /* skip leading blanks on value */
            while(isspace(*value)) ++value;
            /* check whether both property and value are there */
            if(strlen(cstyle) && strlen(value)) {
               add_styleattr(hp,cstyle,value);
            } else {
               hsc_message(hp, MSG_INVALID_STYLE,
                     "invalid CSS style definition `%s:%s'", cstyle,value);
            }
         } else {
            hsc_message(hp, MSG_INVALID_STYLE,
                  "invalid CSS style definition %q", cstyle);
         }
         /* continue with next style pair */
         cstyle = nstyle;
      } while(!done);
   } else {
      /* append attribute */
      app_estr(hp->tag_attr_str, estr2str(attr_str));
   }
   del_estr(attr_str);
   del_estr(val_str);
   return (ok);
}

/*
 * set_tag_defaults
 *
 * append attributes which have not been set by user but contain
 * a default value to the hp->tag_attr_str
 */
static VOID set_tag_defaults(HSCPRC * hp, HSCTAG * tag)
{
   DLNODE *nd = dll_first(tag->attr);

   while (nd)
   {
      HSCATTR *attr = (HSCATTR *) dln_data(nd);
      STRPTR value = get_vartext(attr);
      STRPTR defval = get_vardeftext(attr);

      if (!value && defval)
      {
         /* there is no current value, but a default value:
          * set value with default, */
         set_vartext(attr, defval);

         /* append attribute name to tag_attr_str */
         app_estr(hp->tag_attr_str, " ");
         app_estr(hp->tag_attr_str, attr->name);

         DAV(fprintf(stderr, DHL "  `%s:%s' defaults to `%s'\n",
                  tag->name, attr->name, defval));

         /* if it is a non-bool attrib, also append value */
         if (attr->vartype != VT_BOOL)
         {
            app_estr(hp->tag_attr_str, "=");

            /* decide which quote to use for default value */
            attr->quote = DOUBLE_QUOTE;
            choose_quote(hp, attr);

            /* append quote */
            if (attr->quote != VQ_NO_QUOTE)
            {
               app_estrch(hp->tag_attr_str, attr->quote);
            }
            /* append value */
            app_estr(hp->tag_attr_str, attr->name);
            /* append quote */
            if (attr->quote != VQ_NO_QUOTE)
            {
               app_estrch(hp->tag_attr_str, attr->quote);
            }
         }
      }

      nd = dln_next(nd);
   }

}

/*
 * set_tag_args
 *
 * parse & set all arguments of a tag
 */
ULONG set_tag_args(HSCPRC * hp, HSCTAG * tag)
{
   INFILE *inpf = hp->inpf;
   BOOL ok = FALSE;
   DLLIST *varlist;
   ULONG result_tci = get_mci(hp);     /* resulting tag_call_id */
   STRPTR nw = infgetw(inpf);

   /* evaluate which varlist to use */
   varlist = tag->attr;

   /* clear string that logs all attributes passed to tag */
   clr_estr(hp->tag_attr_str);

   /* read args */
   do {
      if (!nw) {
         hsc_msg_eof(hp, "read attributes");
      } else {
         /*
          * process next attribute
          */
         if (!strcmp(nw, ">")) {
            nw = NULL;
            ok = TRUE;
         } else {
            /* if in XHTML mode, ensure there is no attribute after
             * the closing slash */
            if(hp->xhtml && (tag->option & HT_EMPTY) && !hp->xhtml_emptytag) {
               hsc_message(hp, MSG_ATTR_AFTER_SLASH,
                     "%a after closing slash in empty tag",
                     nw);
            }
            /* process attribute */
            if (NULL != (nw = check_attrname(hp, NULL, nw, FALSE)))
            {
               set_tag_arg(hp, varlist, nw, tag->name,
                     (BOOL)(tag->option & HT_UNKNOWN),
                     (BOOL)(tag->option & HT_MACRO));
            } else {
               /* append empty value */
#if 0
               app_estr(hp->tag_attr_str, "\"\"");
               skip_until_eot(hp, NULL);
               nw = NULL;
#endif
            }

            /* read next attribute */
            if (nw)
               nw = infgetw(inpf);
         }
      }
   }
   while (nw);

   /* for all attributes with defaults, but no value,
    * append it to the tag call */
   set_tag_defaults(hp, tag);

   /* unset scope */
   unget_mci(hp);

   /* set all undefined bool. attr to FALSE */
   clr_varlist_bool(varlist);

   /* check for required attributes */
   if (ok)
   {
      ok = check_varlist(hp, varlist);
      if (!ok)
         inungetcw(inpf);
   }

   if (!ok)
      result_tci = MCI_ERROR;

   return (result_tci);
}

/* vi: set ts=4: */
