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
 * hsclib/parse.c
 *
 * parse file: handle for entities & tags
 *
 * updated: 16-Dec-1997
 * created:  1-Jul-1995
 *
 */

#include <ctype.h>

#define NOEXTERN_HSCLIB_PARSE_H

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/idref.h"
#include "hsclib/include.h"
#include "hsclib/input.h"
#include "hsclib/parse.h"
#include "hsclib/posteval.h"
#include "hscprj/project.h"
#include "hsclib/skip.h"
#include "hsclib/size.h"
#include "hsclib/uri.h"

/*
 *---------------------------
 * misc. functions
 *---------------------------
 */

/*
 * message_rplc
 *
 * message that tells user that a special char
 * was replaced by its entity
 */
static VOID message_rplc(HSCPRC * hp, STRPTR what, STRPTR by)
{
   hsc_message(hp, MSG_RPLC_ENT,
         "replaced %q by %q", what, by);
}

/*
 * check_mbinaw
 *
 * check if tag occurs in allowed context with other tags
 */
static BOOL check_mbinaw(HSCPRC * hp, HSCTAG * tag)
{
   BOOL ok = TRUE;
   const BOOL xhtml_strictmbi = (hp->xhtml && (!((tag->option & HT_MACRO) || is_hsc_tag(tag))));

   /* check for tags that must be called before */
   if (tag->mbi)
   {
      DLNODE *nd = hp->container_stack->last;
      LONG found = 0;

      D(fprintf(stderr,DHL "Checking /MBI for %s (%s): ", tag->name, tag->mbi);)
      while (nd && !found)
      {
         HSCTAG *ctag = (HSCTAG *) nd->data;

         nd = nd->prev;
         found = strenum(ctag->name, tag->mbi, '|', STEN_NOCASE);
         /* skip enclosing macros in XHTML mode to avoid false errors below */
         if(xhtml_strictmbi && (ctag->option & (HT_SPECIAL | HT_MACRO))) {
            D(fprintf(stderr,"[%s] ",ctag->name);)
            continue;
         }
         D(fprintf(stderr, "%s ",ctag->name);)
         if(xhtml_strictmbi && !is_hsc_tag(ctag)) break;
      }
      D(fprintf(stderr,"\n");)

      if (!found)
      {
         hsc_message(hp, MSG_MBI,
               "%T must be inside %t", tag, tag->mbi);
         ok = FALSE;
      }
   }

   /* check for tags that are not to be called before */
   if (tag->naw)
   {
      DLNODE *nd = hp->container_stack->last;
      LONG found = 0;

      while (nd)
      {
         HSCTAG *ctag = (HSCTAG *) nd->data;

         found = strenum(ctag->name, tag->naw, '|', STEN_NOCASE);
         if (found)
         {
            hsc_message(hp, MSG_NAW,
                  "%T not allowed within %T", tag, ctag);

            ok = FALSE;
         }
         nd = dln_prev(nd);
      }
   }
   return ok;
}

/* check if an AUTOCLOSE should be done despite the current opening tag not
 * being the same as the last on the container stack. If so, remove all
 * intervening tags and return TRUE.
 */
static BOOL check_autoclose_anyway(HSCPRC *hp, DLNODE *nd) {
   DLNODE *cnd; /* current node */
   DLNODE *tnd; /* temp node */
   BOOL anyway = TRUE;

   for(cnd = dll_last(hp->container_stack);
         (cnd != nd) && anyway;
         cnd = dln_prev(cnd)) {
      anyway = (BOOL)(((HSCTAG*)dln_data(cnd))->option & HT_AUTOCLOSE);
      D(fprintf(stderr, DHL "  /AC-stackcheck: <%s> [%s]\n",
               ((HSCTAG*)dln_data(cnd))->name, anyway ? "YES" : "NO"));
   }

   if(anyway) {
      /* there are only /AUTOCLOSE tags between the end of stack
       * and the current tag, so remove all of them.
       * Example: <dl><dt>foo<dd>bar</dl>
       * -> <dd> autocloses <dt> (which is /AC) but not <dl>
       */
      for(cnd = dll_last(hp->container_stack);
            cnd != nd;
            cnd = tnd) {
         D(fprintf(stderr,
                  DHL "  autoclosing extra <%s> \n",
                  ((HSCTAG*)dln_data(cnd))->name));
         tnd = cnd->prev;
         del_dlnode(hp->container_stack,cnd);
      }
      return TRUE;
   }
   return FALSE;
} 

/* enable output for a process */
void hp_enable_output(HSCPRC * hp, STRPTR cause)
{
   if (hp->suppress_output)
   {
      clr_estr(hp->whtspc);
      D(fprintf(stderr, DHL "output enabled (%s)\n", cause));
   }
   hp->suppress_output = FALSE;
}

/*
 *---------------------------
 * remove/append end tag
 * from/to container_stack
 *---------------------------
 */

/*
 * find_end_tag_node
 *
 * return first node of an end tag on the container stack;
 * if not found, return NULL
 */
DLNODE *find_end_tag_node(HSCPRC * hp, STRPTR tagname)
{
   return find_dlnode_bw(dll_last(hp->container_stack),
         (APTR) tagname, cmp_strtag);
}

/*
 * find_end_tag
 *
 * return first end tag on the container stack;
 * if not found, return NULL
 */
HSCTAG *find_end_tag(HSCPRC * hp, STRPTR tagname)
{
   DLNODE *nd = find_dlnode_bw(dll_last(hp->container_stack),
         (APTR) tagname, cmp_strtag);

   return (HSCTAG*) ((NULL != nd) ? dln_data(nd) : NULL);
}

/*
 * find_end_container
 *
 * search container stack for the first macro which is a
 * container macro and return the cerresponding tag structure
 */
HSCTAG *find_end_container_macro(HSCPRC * hp)
{
   HSCTAG *tag = NULL;
   DLNODE *nd = dll_last(hp->container_stack);

   while (nd) {
      HSCTAG *nd_tag = (HSCTAG *) dln_data(nd);

      if (nd_tag->option & HT_CONTENT) {
         tag = nd_tag;
         nd = NULL;
      } else {
         nd = dln_prev(nd);
      }
   }
   return tag;
}

/*
 * append_end_tag
 *
 * create end tag and append it to tag-list;
 * also clone options & attribute list of parent
 * tag, if tag is a macro and has a closing tag.
 *
 * params: hp.....hscprc with container_stack to be modified
 *         tagid..name of the new tag (eg "IMG")
 * result: ptr to the new tag or NULL if no mem
 */
HSCTAG *append_end_tag(HSCPRC * hp, HSCTAG * tag)
{
   HSCTAG *end_tag;
   DLLIST *taglist = hp->container_stack;

   end_tag = new_hsctag(tag->name);
   if (end_tag)
   {
      BOOL ok = TRUE;
      DLNODE *nd = NULL;

      /* copy important data of tag */
      end_tag->option = tag->option;

      /* clone attributes, if tag is a
       * macro tag and has a closing tag
       */
      if ((tag->option & HT_MACRO) && (tag->option & HT_CLOSE))
         ok = copy_local_varlist(end_tag->attr, tag->attr, MCI_APPCTAG);

      /* remeber position where start tag has been called */
      /* (for message "end tag missing) */
      end_tag->start_fpos = new_infilepos(hp->inpf);

      /* insert tag in list */
      if (ok)
      {
         nd = app_dlnode(taglist, end_tag);
         if (!nd)
         {
            del_hsctag((APTR) end_tag);
            end_tag = NULL;
         }
      }
   }
   return (end_tag);
}

/*
 * remove_end_tag
 *
 * remove tag from container stack, check for legal nesting,
 * show up message if necessary.
 *
 * params: hp....hsc process
*         tag...tag to be removed
*/
VOID remove_end_tag(HSCPRC * hp, HSCTAG * tag)
{
   /* search for tag on stack of occured tags */
   DLNODE *nd = find_dlnode_bw(dll_last(hp->container_stack),
         (APTR) tag->name, cmp_strtag);

   if (nd == NULL) {
      /* closing tag not found on stack */
      /* ->unmatched closing tag without previous opening tag */
      hsc_message(hp, MSG_UNMA_CTAG, "unmatched %C", tag);
   } else {
      /* closing tag found on stack */
      HSCTAG *end_tag, *last_tag;
      STRPTR foundnm, lastnm;

      /* check for any /AC tags and remove them */
      if(!is_hsc_tag(tag))
         check_autoclose_anyway(hp, nd);

      end_tag = (HSCTAG*) dln_data(nd);
      foundnm = (STRPTR) end_tag->name;
      lastnm = ((HSCTAG*)dln_data(dll_last(hp->container_stack)))->name;

      /* check if name of closing tag is -not- equal
       * to the name of the last tag last on stack
       * ->illegal tag nesting
       */
      if(upstrcmp(lastnm, foundnm) &&
            !(tag->option & HT_MACRO) && !is_hsc_tag(tag)) {
         if(NULL != (last_tag = find_strtag(hp->deftag,lastnm))) {
            if(!is_hsc_tag(last_tag))
               hsc_message(hp, MSG_CTAG_NESTING,
                     "illegal end tag nesting (expected %c, found %C)",
                     lastnm, tag);
         }
      }

      /* if closing tag has any attributes defined, it must be a closing macro
       * tag. so copy the attributes of the closing tag to the attributes of the
       * macro tag. therefore, the closing macro tag inherits the attributes of
       * its opening macro
       */
      if (end_tag->attr) set_local_varlist(tag->attr, end_tag->attr,
            MCI_APPCTAG);

      /* remove node for closing tag from container_stack */
      del_dlnode(hp->container_stack, nd);
   }
}

/*
 *---------------------------
 * parse tag functions
 *---------------------------
 */

/*
 * hsc_parse_tag
 *
 * parse tag (after "<")
 */
BOOL hsc_parse_tag(HSCPRC * hp)
{
   INFILE *inpf = hp->inpf;
   STRPTR nxtwd = NULL;
   DLNODE *nd = NULL;
   HSCTAG *tag = NULL;
   HSCTAG *now_tag_strip_whtspc = NULL;
   ULONG tci = 0;              /* tag_call_id returned by set_tag_args() */
   BOOL(*hnd) (HSCPRC * hp, HSCTAG * tag) = NULL;
   BOOL open_tag;
   DLLIST *taglist = hp->deftag;
   BOOL rplc_lt = FALSE;       /* TRUE, if replace spc. char "<" */
   BOOL hnd_result = TRUE;     /* result returned by handle */
   BOOL unknown_tag = FALSE;   /* TRUE, if tag has not been defined before */
   BOOL preceeding_whtspc = estrlen(hp->whtspc);

   /* init strings used inside tag-handles */
   set_estr(hp->tag_name_str, infgetcw(inpf));
   clr_estr(hp->tag_attr_str);
   clr_estr(hp->tag_close_str);

   /* default is no trailing slash */
   hp->xhtml_emptytag = FALSE;

   if (hp->smart_ent && preceeding_whtspc)
   {
      /*
       * check for special char "<"
       */
      int ch = infgetc(inpf);

      /* check if next char is a white space */
      if (hsc_whtspc(ch))
      {
         rplc_lt = TRUE;

         /* write "&lt;" and white spaces */
         message_rplc(hp, "<", "&lt;");
         hsc_output_text(hp, "", "&lt;");
      }
      inungetc(ch, inpf);
   }

   if (!rplc_lt)
   {
      /* get tag id */
      nxtwd = infget_tagid(hp);
      if (!hp->fatal)
      {
         /* append tag-name to tag_name_str */
         if (!hp->compact)
         {
            app_estr(hp->tag_name_str, infgetcws(inpf));
         }
         app_estr(hp->tag_name_str, infgetcw(inpf));

         if (!hp->suppress_output)
         {
            /* output tag currently processing
             * NOTE: the first tag of the source is skipped because
            *   hp->supress_ouptut is disabled later */
            D(fprintf(stderr, DHL "tag <"));
         }
      }
   }

   if (!hp->fatal && !rplc_lt)
   {
      BOOL write_tag = FALSE; /* flag: write tag text & attrs to output? */

      if (strcmp("/", nxtwd)) /* is it a closing tag? */
      {
         /*
          *
          * process start-tag
          *
          */
         open_tag = TRUE;
         if (!hp->suppress_output)
            D(fprintf(stderr, "%s>\n", nxtwd));

         /* search for tag in list */
         nd = find_dlnode(taglist->first, (APTR) nxtwd, cmp_strtag);
         if(NULL == nd) {
            /* tag not found */
            hsc_message(hp, MSG_UNKN_TAG, "unknown %t", nxtwd);
            tag = new_hsctag(nxtwd);
            tag->option |= HT_UNKNOWN;
            unknown_tag = TRUE;
         } else {
            tag = (HSCTAG *) nd->data;
         }

         /* set handle-function */
         hnd = tag->o_handle;

         /*
          * handle options
          */

         /* check for obsolete tag */
         if (tag->option & HT_OBSOLETE) {
            hsc_message(hp, MSG_TAG_OBSOLETE,
                  "%T is obsolete", tag);
         }

         /* check for /EMPTY tag and XHTML */
         hp->xhtml_emptytag = 
            (BOOL)((tag->option & HT_EMPTY) && hp->xhtml);

         /* check for jerk-tag */
         if (tag->option & HT_JERK) {
            hsc_message(hp, MSG_TAG_JERK,
                  "%T is only used by %j", tag);
         }

         /* only-once-tag occured twice? */
         if ((tag->option & HT_ONLYONCE) && (tag->occured)) {
            hsc_message(hp, MSG_TAG_TOO_OFTEN,
                  "%T occured too often", tag);
         }

         /* set occured-flag */
         if (tag->option & (HT_ONLYONCE | HT_REQUIRED)) {
            tag->occured = TRUE;
         }

         /* check for "must be inside"/"not allowed within"-tags */
         if (!check_mbinaw(hp, tag)) {
            hnd = NULL;
         }

         /* clear (reset to default) attribute values of tag */
         clr_varlist(tag->attr);

         /* set attributes or check for ">" */
         if (!(tag->option & HT_SPECIAL)) {
            tci = set_tag_args(hp, tag);
            if (tci == MCI_ERROR) {
               skip_until_eot(hp, NULL);
               hnd = NULL;
            }

            if (!hp->fatal)
            {
               /* set ">" in string that contains closing text */
               if(!hp->compact)
                  set_estr(hp->tag_close_str, infgetcws(inpf));
               else
                  clr_estr(hp->tag_close_str);
               app_estr(hp->tag_close_str, infgetcw(inpf));

               /* check for succeeding white-space */
               if ((tag->option & HT_WHTSPC) && !infeof(inpf))
               {
                  int ch = infgetc(inpf);

                  if (hsc_whtspc(ch))
                  {
                     if (hp->strip_badws)
                        hp->strip_next2_whtspc = TRUE;
                     else if (!hp->strip_next2_whtspc)
                        now_tag_strip_whtspc = tag;
                  }
                  inungetc(ch, inpf);
               }
            }
         }

         /* for AUTOCLOSE-tags, remove potential end tags on stack */
         if (tag->option & HT_AUTOCLOSE) {
            DLNODE *nd = find_end_tag_node(hp, tag->name);

            if (nd) {
               if ((nd == dll_last(hp->container_stack)) ||
                     check_autoclose_anyway(hp,nd))
               {
                  D(fprintf(stderr, DHL "  autoclose </%s> before\n",
                           tag->name));
                  remove_end_tag(hp, tag);
               } else {
                  D(fprintf(stderr,
                           DHL "  no autoclose because of <%s> \n",
                           ((HSCTAG*)
                            dln_data(dll_last(hp->container_stack)))->name));
               }
            } else {
                  D(fprintf(stderr, DHL "  no autoclose neccessary\n"));
            }
         }

         /* end-tag required? */
         if ((tag->option & HT_CLOSE) || (tag->option & HT_AUTOCLOSE)) {
            /* yes: push current tag to container stack;
             * (current values of attributes will be
             * remembered */
            append_end_tag(hp, tag);
         }
      } else {
         /*
          *
          * process end-tag
          *
          */

         /* get tag id */
         nxtwd = infget_tagid(hp);   /* get tag id */
         open_tag = FALSE;

         /* append tag-name to tag_name_str */
         if (!hp->compact)
            app_estr(hp->tag_name_str, infgetcws(inpf));
         app_estr(hp->tag_name_str, infgetcw(inpf));

         if (!hp->suppress_output)
            D(fprintf(stderr, "/%s>\n", nxtwd));

         /* search for tag in taglist */
         /* (see if it exists at all) */
         nd = find_dlnode(taglist->first, (APTR) nxtwd, cmp_strtag);
         if (nd == NULL) {
            /* closing tag is absolutely unknown */
            hsc_message(hp, MSG_UNKN_TAG,   /* tag not found */
                  "unknown %c", nxtwd);
            skip_until_eot(hp, hp->tag_attr_str);
         } else {
            tag = (HSCTAG *) nd->data;      /* fitting tag in taglist */

            /* check for preceding white-spaces */
            if ((tag->option & HT_WHTSPC) && anyWhtspc(hp)) {
               if (hp->strip_badws)
                  hp->strip_next_whtspc = TRUE;
               else if (!hp->strip_next_whtspc)
                  hsc_message(hp, MSG_PREC_WHTSPC,
                        "preceding white space for %C", tag);
            }

            if (tag->option & (HT_CLOSE | HT_AUTOCLOSE)) {
               /* set closing handle */
               hnd = tag->c_handle;

               /* check for no args */
               if (!parse_wd(hp, ">")) {
                  hsc_message(hp, MSG_CL_TAG_ARG,
                        "no attributes allowed for end-tags");
               } else {
                  /* set ">" in string that contains closing text */
                  if (!hp->compact)
                     set_estr(hp->tag_close_str, infgetcws(inpf));
                  app_estr(hp->tag_close_str, infgetcw(inpf));
               }

               /* set values of attributes stored
                * in end-tag,
                * remove end-tag from stack
                */
               remove_end_tag(hp, tag);
            } else {
               /* illegal closing tag */
               hsc_message(hp, MSG_ILLG_CTAG,      /* tag not found */
                     "illegal %c", nxtwd);
               parse_gt(hp);
               tag = NULL;
            }
         }
      }

      /*
       * processed for opening AND closing tag
       */
      write_tag = (!(tag) || !(tag->option & HT_NOCOPY));

      if (tag) {
         /*
          * check if tag should be stripped
          */
         if (!postprocess_tagattr(hp, tag, open_tag)) {
            /* stripped tag with external reference */
            if (open_tag)
            {
               hsc_msg_stripped_tag(hp, tag, "external reference");
            }
            hnd = NULL;     /* don't call handle */
            write_tag = FALSE;      /* don't output tag */
         } else if (hp->strip_tags &&
                    strenum(tag->name, hp->strip_tags, '|', STEN_NOCASE)) {
            /* strip tag requested by user */
            if (!(tag->option & HT_SPECIAL)) {
               if (open_tag)
                  hsc_msg_stripped_tag(hp, tag, "as requested");
               hnd = NULL; /* don't call handle */
               write_tag = FALSE;  /* don't output tag */
            }
            else hsc_message(hp, MSG_TAG_CANT_STRIP,
                  "can not strip special tag %T", tag);

            /*
             * get values for size from reference
             */
         } else if (tag->uri_size && get_vartext(tag->uri_size))
            get_attr_size(hp, tag);

         /* flush all CSS properties to a STYLE attribute */
         if(NULL != hp->tag_styles->first) {
            BOOL semicolon = FALSE;
            STRPTR quote = (QMODE_SINGLE == hp->quotemode) ? "'" : "\"";
            HSCSTYLE *stylend;
            DLNODE *nd;

            /* append attribute and quote */
            app_estr(hp->tag_attr_str, hp->lctags ? " style=" : " STYLE=");
            app_estr(hp->tag_attr_str, quote);
            /* loop over all nodes in styles list */
            while(NULL != (nd = hp->tag_styles->first)) {
               stylend = (HSCSTYLE*)(nd->data);
               /* if there is more than one pair, they have
                * to be separated */
               if(semicolon) app_estr(hp->tag_attr_str, "; ");
               /* append <name>:<value> */
               app_estr(hp->tag_attr_str, stylend->name);
               app_estr(hp->tag_attr_str, ":");
               app_estr(hp->tag_attr_str, stylend->value);
               /* remove node from list */
               del_dlnode(hp->tag_styles, nd);
               semicolon = TRUE;
            }
            /* closing quote */
            app_estr(hp->tag_attr_str, quote);
         }
      }

      /* call handle if available */
      if (hnd && !hp->fatal)
         hnd_result = (*hnd) (hp, tag);

      /* write whole tag out */
      if (write_tag && hnd_result) {
         VOID(*tag_callback) (HSCPRC * hp, HSCTAG * tag,
               STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close) = NULL;

         if (open_tag)
            tag_callback = hp->CB_start_tag;
         else
            tag_callback = hp->CB_end_tag;

         /* enable output if necessary */
         if (hp->suppress_output)
            hp_enable_output(hp, "non-internal tag occured");

         /* write (flush) white spaces */
         hsc_output_text(hp, "", "");

         if (tag_callback) {
            (*tag_callback) (hp, tag,
                             estr2str(hp->tag_name_str),
                             estr2str(hp->tag_attr_str),
                             estr2str(hp->tag_close_str));
         }
      }

      /* skip LF if requested */
      if (tag && (tag->option & HT_SKIPLF))
         skip_next_lf(hp);

      /* if tag should check for succeeding white spaces,
       * tell this hscprc now */
      if (now_tag_strip_whtspc) {
         D(fprintf(stderr, "  requested to check for succ.whtspc\n"));
         hp->tag_next_whtspc = now_tag_strip_whtspc;
      }

      /* remove temporary created tag */
      if (unknown_tag)
         del_hsctag(tag);

   }

   return (BOOL) (!hp->fatal);
}

/*
 *---------------------------
 * other parse functions
 *---------------------------
 */

/* replace icon-entity by image */
static VOID replace_icon(HSCPRC * hp, STRPTR icon)
{
   INFILEPOS *base = new_infilepos(hp->inpf);
   EXPSTR *image = init_estr(0);
   STRPTR s = estr2str(hp->iconbase);

   /* create string like <IMG SRC=":icons/back.gif" ALT="back"> */
   set_estr(image, "<IMG SRC=\"");

   /* use iconbase with "*" replaced  by iconname as uri */
   while (s[0])
   {
      if (s[0] == '*')
         app_estr(image, icon);
      else
         app_estrch(image, s[0]);
      s++;
   }

   /* set ALT attribute to iconname */
   app_estr(image, "\" ALT=\"");
   app_estr(image, icon);
   app_estr(image, "\">");

   hsc_message(hp, MSG_RPLC_ICON, "replacing icon-%e", icon);

   hsc_include_string(hp, SPECIAL_FILE_ID "include icon",
         estr2str(image),
         IH_PARSE_HSC | IH_NO_STATUS | IH_POS_PARENT);
   del_estr(image);
   del_infilepos(base);
}

/*
 * hsc_parse_amp
 *
 * parse ampersand ("&")
 */
BOOL hsc_parse_amp(HSCPRC * hp)
{
   INFILE *inpf = hp->inpf;
   EXPSTR *amp_str = init_estr(0);

   if (!hp->fatal) {
      BOOL rplc = hp->smart_ent;      /* TRUE, if "&" should be replaced */

      if (rplc) {
         /*
          * test if char before and
          * after "&" is white-space
          */
         int ch = infgetc(inpf);

         inungetc(ch, inpf);

         if (!(hsc_whtspc(ch)) || !(estrlen(hp->whtspc))) {
            /* no, it is not */
            rplc = FALSE;
         }
      }

      if (rplc) {
         /* replace ampersand */
         message_rplc(hp, "&", "&amp;");
         set_estr(amp_str, "&amp;");
      } else {
         /*
          * get entity-id, test for unknown entity
          */
         char *nxtwd;
         DLNODE *nd;
         BOOL app_entity = TRUE;

         /* remember "&" */
         set_estr(amp_str, infgetcw(inpf));

         /* get entity id */
         nxtwd = infgetw(inpf);

         /* check for illegal white-space */
         if (strlen(infgetcws(inpf)))
            hsc_msg_illg_whtspc(hp);

         if (nxtwd == NULL) {
            hsc_msg_eof(hp, "missing entity");
         } else if (!strcmp(nxtwd, "\\")) {
            /* flush white spaces */
            clr_estr(hp->whtspc);
            clr_estr(amp_str);
            infskip_ws(inpf);
         } else if (!strcmp(nxtwd, "/")) {
            /* replace white spaces by a single blank */
            set_estr(hp->whtspc, " ");
            clr_estr(amp_str);
            infskip_ws(inpf);
         } else {
            hp_enable_output(hp, "entity");

            /* append (illegal anyway) white space */
            app_estr(amp_str, infgetcws(inpf));

            if (!strcmp(nxtwd, "#")) {
               /*
                * process numeric entity
                */
               int base = 10;      /* numeric encoding base */
               STRPTR digit_start = NULL;  /* start of numeric digits */

               /* append "#" */
               app_estr(amp_str, infgetcw(inpf));

               /* get the digit sequence */
               nxtwd = infgetw(inpf);

               /* check for illegal white-space */
               if (strlen(infgetcws(inpf)))
                  hsc_msg_illg_whtspc(hp);

               /* find out wheter it is an decimal or a hexadecimal
                * entity and set base according to it */
               if (toupper(nxtwd[0]) == 'X') {
                  base = 16;      /* hexadecimal */
                  digit_start = nxtwd + 1;
               } else {
                  base = 10;      /* decimal */
                  digit_start = nxtwd;
               }

               /* try to convert entity to number */
               errno = 0;
               strtoul(digit_start, NULL, base);
               if (errno) {
                  hsc_message(hp, MSG_ILLG_NUM,   /* illegal numeric entity */
                        "illegal numeric value %n for entity", nxtwd);
               }

               /* append entity specifier */
               app_estr(amp_str, nxtwd);
            } else {
               /*
                * process text entity
                */
               HSCVAR *attr = NULL;

               /* search for entity in list */
               nd = find_dlnode(hp->defent->first, (APTR) nxtwd, cmp_strent);

               if (hp->jens && (nd == NULL)) {
                  /* assume that entity is an attribute,
                   * try to find it and append its value
                   */
                  attr = find_varname(hp->defattr, nxtwd);
                  if (attr) {
                     set_estr(amp_str, get_vartext(attr));
                     app_entity = FALSE;
                  }
               }

               if ((nd == NULL) && (attr == NULL)) {
                  hsc_message(hp, MSG_UNKN_ENTITY,
                        "unknown %e", nxtwd);
               } else if(NULL != nd) {
                  /* check for icon-entity and warn about */
                  /* portability problem */
                  HSCENT *entity = dln_data(nd);

                  if (entity->numeric == ICON_ENTITY) {
                     if (estrlen(hp->iconbase)) {
                        replace_icon(hp, nxtwd);
                        set_estr(amp_str, "");
                        app_entity = FALSE;
                     } else {
                        hsc_message(hp, MSG_ICON_ENTITY,
                              "icon %e found", nxtwd);
                     }
                  }
               }

               if (app_entity) {
                  /* append entity specifier */
                  app_estr(amp_str, nxtwd);
               }
            }

            /* check for closing ';' */
            nxtwd = infgetw(inpf);
            if (nxtwd) {
               if (!strcmp(nxtwd, ";")) {
                  if (strlen(infgetcws(inpf)))
                     hsc_msg_illg_whtspc(hp);

                  if (app_entity) {
                     app_estr(amp_str, infgetcws(inpf));
                     app_estr(amp_str, infgetcw(inpf));
                  }
               } else {
                  hsc_message(hp, MSG_EXPT_SEMIC, "%q expected after entity", ";");
                  inungetcwws(inpf);
               }
            } else {
               hsc_msg_eof(hp, "expected \";\") for entity");
            }
         }
      }

      /* output whole entity */
      if (estrlen(amp_str))
         hsc_output_text(hp, "", estr2str(amp_str));

      del_estr(amp_str);
   }

   return (BOOL) (!hp->fatal);
}

/*
 * hsc_parse_text
 */
BOOL hsc_parse_text(HSCPRC * hp)
{
   INFILE *inpf = hp->inpf;
   STRPTR nw = infgetcw(inpf);

   if (nw && hp->suppress_output)
   {
      hp_enable_output(hp, "some text");
   }

   if (nw)
   {                           /* do test below only if not end-of-file */
      /*
       * check unmatched ">"
       */
      if (!strcmp(nw, ">"))
      {
         BOOL rplc = hp->smart_ent;  /* TRUE, if ">" should be replaced */

         if (rplc)
         {
            /*
             * test if char before and
             * after ">" is white-space
             */
            int ch = infgetc(inpf);

            inungetc(ch, inpf);

            if (!(hsc_whtspc(ch) && estrlen(hp->whtspc)))
            {
               rplc = FALSE;
            }
         }
         if (rplc)
         {
            /* replace gt */
            message_rplc(hp, nw, "&gt;");
            nw = "&gt;";
         }
         else
         {
            hsc_message(hp, MSG_UNMA_GT, "unmatched %q", ">");
         }
      }
      /*
       * check for quote
       */
      else if (!strcmp(nw, "\""))
      {
         if (hp->rplc_quote)
         {
            /* replace quote */
            message_rplc(hp, nw, "&quot;");
            nw = "&quot;";
         }
      }
      /*
       * check for entities to replace
       */
      else
      {
         DLNODE *nd = NULL;  /* entity search result */

         if (hp->rplc_ent && (strlen(nw) == 1)
               && (((UBYTE) nw[0]) >= 127))
         {
            nd = find_dlnode(hp->defent->first, (APTR) nw, cmp_rplcent);

            if (nd)
            {
               BOOL ok = TRUE;

               /* copy replaced entity to buffer */
               ok &= set_estr(hp->tmpstr, "&");
               ok &= app_estr(hp->tmpstr, ((HSCENT *) nd->data)->name);
               ok &= app_estr(hp->tmpstr, ";");

               if (ok)
               {
                  /* replace-message */
                  message_rplc(hp, nw, estr2str(hp->tmpstr));
                  nw = estr2str(hp->tmpstr);
               }
            }
         }
         /*
          * check for "click here" syndrome
          */
         if (hp->inside_anchor && hp->click_here_str)
         {
            ULONG found = strenum(nw, hp->click_here_str, '|', STEN_NOCASE);
            if (found)
            {
               hsc_message(hp, MSG_CLICK_HERE,
                     "%q-syndrome detected", "click here");
            }
         }
      }
   }

   if (nw)
      hsc_output_text(hp, "", nw);    /* output word */

   return (BOOL) (!hp->fatal);
}

/*
 * hsc_parse
 *
 * parse input chars with full hsc support
 *
 * params: inpf...input file
 *
 * result: TRUE, if no error
 */
BOOL hsc_parse(HSCPRC * hp)
{
   if (!hp->fatal)
   {
      STRPTR nxtwd = infgetw(hp->inpf);
      STRPTR cws = infgetcws(hp->inpf);       /* current WhtSpcs */

      /* add white spaces to buffer */
      if (cws)
         app_estr(hp->whtspc, cws);

      /* parse text */
      if (nxtwd)
      {
         if (!strcmp(nxtwd, "<"))
            /* parse tag */
            hsc_parse_tag(hp);
         else if (!strcmp(nxtwd, "&"))
            /* parse entity */
            hsc_parse_amp(hp);
         else
            /* handle text */
            hsc_parse_text(hp);
      }
   }

   return (BOOL) (!hp->fatal);
}

/*
 * hsc_parse_source
 *
 * parse input chars with full hsc support
 *
 * params: inpf...input file
 *
 * result: TRUE, if no error
 */
BOOL hsc_parse_source(HSCPRC * hp)
{
   if (!hp->fatal)
   {
      STRPTR nxtwd = infgetw(hp->inpf);
      STRPTR cws = infgetcws(hp->inpf);       /* current WhtSpcs */

      /* add white spaces to buffer */
      if (cws)
         app_estr(hp->whtspc, cws);

      if (nxtwd)
      {
         /* process next word */
         if (!strcmp(nxtwd, "<"))
            hsc_output_text(hp, "", "&lt;");
         else if (!strcmp(nxtwd, ">"))
            hsc_output_text(hp, "", "&gt;");
         else if (!strcmp(nxtwd, "&"))
            hsc_output_text(hp, "", "&amp;");
         else
            hsc_parse_text(hp);
      }
   }
   return (BOOL) (!hp->fatal);
}

/*
 *---------------------------
 * parse end functions
 *---------------------------
 */

/*
 * hsc_parse_end
 *
 * check for all tags closed and required
 * tags occured
 */
BOOL hsc_parse_end(HSCPRC * hp)
{
   if (!hp->fatal)
   {
      /* remember current file position */
      INFILEPOS *infpos = new_infilepos(hp->inpf);
      DLNODE *nd = NULL;

      /* check for unclosed containers:
       * for every container tag still on stack launch a message,
       * exept for autoclose tags */
      nd = hp->container_stack->last;
      while (nd)
      {
         HSCTAG *endtag = (HSCTAG *) dln_data(nd);

         if (!(endtag->option & HT_AUTOCLOSE))
         {
            set_infilepos(hp->inpf, endtag->start_fpos);
            hsc_message(hp, MSG_MISS_CTAG,
                  "%c missing", endtag->name);
         }

         nd = dln_prev(nd);
      }

      /* restore file position */
      set_infilepos(hp->inpf, infpos);
      del_infilepos(infpos);

      /* check for required and recommended tags missing */
      nd = dll_first(hp->deftag);
      while (nd)
      {
         HSCTAG *tag = (HSCTAG *) dln_data(nd);

         if ((tag->option & HT_REQUIRED
                  && (tag->occured == FALSE)))
         {
            hsc_message(hp, MSG_MISS_REQTAG,
                  "required %T missing", tag);
         }
         else if ((tag->option & HT_RECOMMENDED
                  && (tag->occured == FALSE)))
         {
            hsc_message(hp, MSG_MISS_RCMDTAG,
                  "recommended %T missing", tag);
         }
         nd = dln_next(nd);
      }

      /* output last white spaces at eof */
      hsc_output_text(hp, "", "");
   }
   return (BOOL) (!hp->fatal);
}

/*
 *---------------------------
 * parse IDs functions
 *---------------------------
 */

/*
 * hsc_parse_end_id
 *
 * append all locally defined IDs to global IDs,
 * check all before referenced local IDs
 *
 */
BOOL hsc_parse_end_id(HSCPRC * hp)
{
   if (!hp->fatal)
      check_all_local_idref(hp);      /* check local IDs */

   return (BOOL) (!hp->fatal);
}
