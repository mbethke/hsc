/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001  Matthias Bethke 
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
 * hsclib/linit.c
 *
 * functions to init & read preferences for a hsc-process
 *
 */

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/include.h"
#include "hsclib/parse.h"

#include "hsclib/tag_a.h"
#include "hsclib/tag_hsc.h"
#include "hsclib/tag_if.h"
#include "hsclib/tag_macro.h"
#include "hsclib/tag_misc.h"

#include "hsc/hsc_rev.h"

#include "ugly/fname.h"

#include "hsclib/entities.c"

#if DEBUG
/*
 * debugging print functions
 */
#if 0
static VOID prt_ent(FILE * stream, APTR data)
{
    HSCENT *ent = (HSCENT *) data;

    if (ent)
    {
        fprintf(stream, " %s", ent->name);
        if (ent->replace)
            fprintf(stream, "=%s", ent->replace);
        if (ent->numeric)
            fprintf(stream, "=%ld", ent->numeric);
    }
    else
        fprintf(stream, " <NULL>");
}
#endif

static VOID prt_tag(FILE * stream, APTR data)
{
    if (data)
    {
        HSCTAG *tag = (HSCTAG *) data;
        fprintf(stream, " <");
        if (tag->option & HT_CLOSE)
            fprintf(stream, "/");
        if (tag->o_handle)
            fprintf(stderr, "*");
        fprintf(stream, "%s", tag->name);
        if (tag->c_handle)
            fprintf(stderr, "*");
        if (tag->option & HT_REQUIRED)
            fprintf(stream, "/r");
        if (tag->option & HT_RECOMMENDED)
            fprintf(stream, "/rcmd");
        if (tag->option & HT_ONLYONCE)
            fprintf(stream, "/1");
        if (tag->option & HT_AUTOCLOSE)
            fprintf(stream, "/ac");
        if (tag->option & HT_EMPTY)
            fprintf(stream, "/e");
        if (tag->option & HT_SPECIAL)
            fprintf(stream, "/a");
        fprintf(stream, ">");
    }
    else
        fprintf(stream, " <NULL>");
}

/* function to temporarily disable debuggin output */
static BOOL dbg_old = FALSE;

#define dbg_disable(hp) {dbg_old = hp->debug; hp->debug=FALSE;}
#define dbg_restore(hp) {hp->debug=dbg_old;}

#else
#define dbg_disable(hp)
#define dbg_restore(hp)
#endif

/*
 * find_prefs_fname
 *
 * find preferences file: first check, if it is located
 * somewhere in the paths given via CONFIG_PATH (which
 * is a system depandent symbol); if not, check if it
 * is in the path described in the envvar HSCPREFS
 *
 * result: full path & filename of prefs or NULL if not found
 *
 */
static STRPTR find_prefs_fname(HSCPRC * hp, EXPSTR *cfgfn) {
#define ENV_HOME "HOME"
    STRPTR prefs_fname = NULL;
    STRPTR paths[] =            /* paths to search for config file */
    {"", "", "", CONFIG_PATH,
     NULL, NULL};
    STRPTR path = NULL;
    UBYTE path_ctr = 0;
    FILE *cfgf = NULL;          /* prefs file */
    EXPSTR *hscpathstr = init_estr(32);         /* buffer to read $HSCPATH */
    EXPSTR *homepathstr = init_estr(32);        /* buffer to read $HOME */

    /* add "$HSCPATH/hsc.prefs" to files-to-be-checked */
    if (link_envfname(hscpathstr, ENV_HSCPATH, NULL, NULL)) {
        /* add envval to paths */
        paths[1] = estr2str(hscpathstr);
    }

    /* add "$HOME/lib/hsc.prefs" to files-to-be-checked */
    if (link_envfname(homepathstr, ENV_HOME, "lib", NULL)) {
        /* add envval to paths */
        paths[2] = estr2str(homepathstr);
    }

    /* try to open any prefs-file */
    do {                           /* loop: */
        path = paths[path_ctr]; /*   get next path */
        if (path) {                       /*   is it the last one? */
            set_estr(cfgfn, path);        /*   N->generate filename */
            app_estr(cfgfn, CONFIG_FILE);

            DC(fprintf(stderr, DHL "try \"%s\"\n", estr2str(cfgfn)));

            cfgf = fopen(estr2str(cfgfn), "r");   /*      try to open file */
        }
        path_ctr++;             /*   process next path */
    } while (path && (!cfgf));    /* until no path left or file opened */

    if (cfgf) {
        prefs_fname = estr2str(cfgfn);
        fclose(cfgf);
    }

    del_estr(homepathstr);
    del_estr(hscpathstr);

    return (prefs_fname);
}

/*
 * hsc_read_prefs
 *
 * try to open (any) config file and read preferences
 * from it
 */
BOOL hsc_read_prefs(HSCPRC * hp, STRPTR prefs_fname) {
    BOOL ok = FALSE;
    EXPSTR *prefs_name_buffer = init_estr(32);

    /* find prefs file */
    if (!prefs_fname)
        prefs_fname = find_prefs_fname(hp, prefs_name_buffer);

    /* status message */
    if (prefs_fname) {
        dbg_disable(hp);

        hsc_status_file_begin(hp, prefs_fname);
        ok = hsc_include_file(hp, prefs_fname,
                              IH_PARSE_HSC | IH_NO_STATUS);
        dbg_restore(hp);

        if (ok) {
            EXPSTR *msg = init_estr(32);
            set_estr(msg, prefs_fname);
            app_estr(msg, ": preferences read");
            hsc_status_misc(hp, estr2str(msg));
            del_estr(msg);
        }
    } else hsc_message(hp, MSG_NO_CONFIG,
          "can not open preferences file");

    del_estr(prefs_name_buffer);

    return (ok);
}


/*
 * hsc_set_tagCB
 */
VOID hsc_set_tagCB(HSCPRC * hp, STRPTR name,
                   BOOL(*op_hnd) (HSCPRC * inpf, HSCTAG * tag),
                   BOOL(*cl_hnd) (HSCPRC * inpf, HSCTAG * tag))
{
    HSCTAG *tag = find_strtag(hp->deftag, name);

    if (tag && !(tag->option & HT_NOHANDLE)) {
        /* set handles */
        DC(fprintf(stderr, DHL "add handles for <%s> (%p,%p)\n",
                   name, (void*)op_hnd, (void*)cl_hnd));
        tag->o_handle = op_hnd;
        tag->c_handle = cl_hnd;
    }
}

/*
 * init_hsctags
 *
 * define hsc tags & attributes; assign tagCBs for them
 *
 * NOTE: this ones tricky, but a bit perverted somehow
 */
BOOL hsc_init_tagsNattr(HSCPRC * hp) {
#define INCLUDE_ATTR " PRE:bool SOURCE:bool TEMPORARY:bool" \
                     " INDENT:num TABSIZE:num=\"4\" "
    BYTE i = 0;
    BOOL ok = TRUE;
    BOOL open_tag;
    HSCTAG *tag;

    /* define hsc internal tags */
    STRPTR hsc_prefs[] =
    {
    /* tags with special chars as name
     *
     * IMPORTANT: When adding new tags with names not starting with
     *   HSC_TAGID, make sure to update `tag.c:is_hsc_tag()'
     */
        HSC_COMMENT_STR " /SKIPLF /SPECIAL>",
        HSC_VERBATIM_STR" /SPECIAL>",
        HSC_INSEXPR_STR " /SPECIAL>",
    /* tags starting with HSC_TAGID */
        HSC_CONTENT_STR " /SKIPLF>",
        HSC_DEFENT_STR " /SKIPLF NAME:string RPLC:string NUM:num PREFNUM:bool NONSTD:bool>",
        HSC_DEFSTYLE_STR " /SKIPLF NAME:string/r VAL:string>",
        HSC_DEFICON_STR " /SKIPLF NAME:string/r>",
        HSC_DEFINE_STR " /SKIPLF /SPECIAL>",
        HSC_DEFTAG_STR " /SKIPLF /SPECIAL>",
        HSC_DEPEND_STR " /SKIPLF ON:string/r FILE:bool>",
        HSC_ELSE_STR " /SKIPLF /MBI=\"" HSC_IF_STR "\">",
        HSC_ELSEIF_STR " /SKIPLF /MBI=\"" HSC_IF_STR "\"" CONDITION_ATTR ":bool>",
        HSC_MESSAGE_STR " /SKIPLF TEXT:string/r CLASS:enum(\"note|warning|error|fatal\")='note'>",
        HSC_EXEC_STR " /SKIPLF COMMAND:string/r REMOVE:enum(\"on|off|auto\")=\"auto\" ATTRIBUTE:string INCLUDE:bool FILE:string " INCLUDE_ATTR ">",
        HSC_EXPORT_STR " /SKIPLF FILE:string/r DATA:string/r APPEND:bool>",
        HSC_IF_STR " /SKIPLF /CLOSE " CONDITION_ATTR ":bool>",
        HSC_INSERT_STR " /OBSOLETE TEXT:string TIME:bool FORMAT:string>",
        HSC_INCLUDE_STR " /SKIPLF FILE:string/r " INCLUDE_ATTR ">",
        HSC_LAZY_STR " /SKIPLF /SPECIAL>",
        HSC_LET_STR " /SKIPLF /SPECIAL>",
        HSC_MACRO_STR " /SKIPLF /SPECIAL>",
        HSC_SOURCE_STR " /SKIPLF PRE:bool>",
        HSC_STRIPWS_STR " TYPE:enum(\"" STRIPWS_ENUM "\")=\"" STRIPWS_BOTH "\">",
        NULL
    };

    STRPTR hsc_attribs[] =
    {
    /*
     * define hsc attributes
     */
     /* name                 : type     : default value */
        SYSTEM_ATTR         ":string/c" , SYSTEM_ATTR_ID,
        ANCHOR_ATTR         ":string"   , "this is a feature, not a bug",
        CONTENT_ATTR        ":string/c" , NULL,
        RESULT_ATTR         ":num=\"0\"", NULL, /* a bit strange */
        FILESIZEFORMAT_ATTR ":string"   , "%a%u",
        TIMEFORMAT_ATTR     ":string"   , "%d-%b-%Y, %H:%M",
        LINEFEED_ATTR       ":string>"  , NULL,
        HSCVERSION_ATTR     ":num/c="STRINGIFY(VERSION), NULL,
        HSCREVISION_ATTR    ":num/c="STRINGIFY(REVISION), NULL,
        NULL, NULL};

    /* temporarily disable debugging output */
    dbg_disable(hp);

    /* define hsc-tags */
    i = 0;
    while (!(hp->fatal) && hsc_prefs[i]) {
        STRARR infname[20];

        sprintf(infname, SPECIAL_FILE_ID "init%3d", i);
        hp->inpf = infopen_str(infname, hsc_prefs[i], 60);

        if (hp->inpf) {
            tag = def_tag_name(hp, &open_tag);
            ok = (tag && def_tag_args(hp, tag));
            infclose(hp->inpf);
        }
        i++;
    }

    /* init hsc-attributes */
    i = 0;
    while (!(hp->fatal) && hsc_attribs[i]) {
        define_attr_by_text(hp, hsc_attribs[i], hsc_attribs[i+1], 0);
        i+=2;
    }

    /* assign "\n" to linefeed-attribute */
    set_vartext(find_varname(hp->defattr, LINEFEED_ATTR), "\n");

    /* assign tag-callbacks to hsc-tags */
    if (ok) {
        hsc_set_tagCB(hp, HSC_COMMENT_STR, handle_hsc_comment, NULL);
        hsc_set_tagCB(hp, HSC_CONTENT_STR, handle_hsc_content, NULL);
        hsc_set_tagCB(hp, HSC_DEFSTYLE_STR, handle_hsc_defstyle, NULL);
        hsc_set_tagCB(hp, HSC_DEFENT_STR, handle_hsc_defent, NULL);
        hsc_set_tagCB(hp, HSC_DEFICON_STR, handle_hsc_deficon, NULL);
        hsc_set_tagCB(hp, HSC_DEFINE_STR, handle_hsc_define, NULL);
        hsc_set_tagCB(hp, HSC_DEFTAG_STR, handle_hsc_deftag, NULL);
        hsc_set_tagCB(hp, HSC_DEPEND_STR, handle_hsc_depend, NULL);
        hsc_set_tagCB(hp, HSC_ELSE_STR, handle_hsc_else, NULL);
        hsc_set_tagCB(hp, HSC_ELSEIF_STR, handle_hsc_elseif, NULL);
        hsc_set_tagCB(hp, HSC_EXEC_STR, handle_hsc_exec, NULL);
        hsc_set_tagCB(hp, HSC_EXPORT_STR, handle_hsc_export, NULL);
        hsc_set_tagCB(hp, HSC_IF_STR, handle_hsc_if, handle_hsc_cif);
        hsc_set_tagCB(hp, HSC_INCLUDE_STR, handle_hsc_include, NULL);
        hsc_set_tagCB(hp, HSC_INSERT_STR, handle_hsc_insert, NULL);
        hsc_set_tagCB(hp, HSC_INSEXPR_STR, handle_hsc_insert_expression, NULL);
        hsc_set_tagCB(hp, HSC_COMMENT_STR, handle_hsc_comment, NULL);
        hsc_set_tagCB(hp, HSC_LAZY_STR, handle_hsc_lazy, NULL);
        hsc_set_tagCB(hp, HSC_LET_STR, handle_hsc_let, NULL);
        hsc_set_tagCB(hp, HSC_MACRO_STR, handle_hsc_macro, NULL);
        hsc_set_tagCB(hp, HSC_MESSAGE_STR, handle_hsc_message, NULL);
        hsc_set_tagCB(hp, HSC_VERBATIM_STR, handle_hsc_verbatim, NULL);
        hsc_set_tagCB(hp, HSC_SOURCE_STR, handle_hsc_source, NULL);
        hsc_set_tagCB(hp, HSC_STRIPWS_STR, handle_hsc_stripws, NULL);
    }

    /* restore debugging output */
    dbg_restore(hp);

    return (ok);
}

/*
 * hsc_init_basicEntities
 *
 * create internal entities
 * (should be called BEFORE hsc_read_prefs())
 */
BOOL hsc_init_basicEntities(HSCPRC * hp) {
   unsigned i;
   
   for(i=0; i < NMEMBERS(HSCInternalEntities); ++i) {
      add_ent(hp->defent,
            HSCInternalEntities[i].name,
            '\0',
            HSCInternalEntities[i].numeric,
            HSCInternalEntities[i].prefnum);
   }
   return (TRUE);
}

/*
 * hsc_assign_tagCBs
 *
 * assign tag-callbacks to standard html-tags
 * (MUST ONLY be called AFTER hsc_read_prefs())
 */
BOOL hsc_assign_tagCBs(HSCPRC * hp)
{
    hsc_set_tagCB(hp, "!", handle_sgml_comment, NULL);
    hsc_set_tagCB(hp, "A", handle_anchor, handle_canchor);
    hsc_set_tagCB(hp, "BASE", handle_base, NULL);
    hsc_set_tagCB(hp, "BLINK", handle_blink, NULL);
    hsc_set_tagCB(hp, "H1", handle_heading, NULL);
    hsc_set_tagCB(hp, "H2", handle_heading, NULL);
    hsc_set_tagCB(hp, "H3", handle_heading, NULL);
    hsc_set_tagCB(hp, "H4", handle_heading, NULL);
    hsc_set_tagCB(hp, "H5", handle_heading, NULL);
    hsc_set_tagCB(hp, "H6", handle_heading, NULL);
    hsc_set_tagCB(hp, "IMG", handle_img, NULL);
    hsc_set_tagCB(hp, "PRE", handle_pre, handle_end_pre);

    return (TRUE);
}

/*
 * hsc_init_hscprc
 *
 * - init all items of hsc-process
 * - create hsc-tags & special atttributes
 * - read preferences file
 * - assign tag-callbacks
 */
BOOL hsc_init_hscprc(HSCPRC * hp, STRPTR prefs_fname)
{
    BOOL ok = FALSE;            /* return value */

    if ( hsc_init_basicEntities(hp)
        && hsc_read_prefs(hp, prefs_fname)
        && hsc_assign_tagCBs(hp)
        )
    {
        /* return sucess */
        ok = TRUE;

        /* printf list of defined tags & entities */
        DC(
              {
              DLNODE * nd;
              HSCENT * ent;

              nd = hp->defent->first;
              if (nd)
              {
              fprintf(stderr, DHL "Known entities:");

              while (nd)
              {
              ent = (HSCENT *) nd->data;
              fprintf(stderr, " &%s;", ent->name);
              nd = nd->next;
              }
              fprintf(stderr, "\n");
              }

              nd = hp->deftag->first;
              if (nd)
              {
              fprintf(stderr, DHL "Known tags:");
              while (nd)
              {
              prt_tag(stderr, nd->data);
              nd = nd->next;
              }
              fprintf(stderr, "\n");
              }

              }
        );                      /* DC */
    }

    hp->click_here_str =
        get_vartext_byname(hp->defattr, CLICK_HERE_ATTR);
    hp->color_names =
        get_vartext_byname(hp->defattr, COLOR_NAMES_ATTR);

    return (ok);
}

/* $Id$ */
/* vi: set ts=4: */
