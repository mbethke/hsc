/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * updated: 15-Apr-1997
 * created: 19-Feb-1996
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

#include "ugly/fname.h"

#if DEBUG
/*
 * debugging print functions
 */
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
            fprintf(stream, "/sc");
        if (tag->option & HT_SPECIAL)
            fprintf(stream, "/a");
        fprintf(stream, ">");
    }
    else
        fprintf(stream, " <NULL>");
}
#endif

/* function to temporarily disable debuggin output */
#if 1
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
 */
static STRPTR find_prefs_fname(HSCPRC * hp)
{
    STRPTR prefs_fname = NULL;
    STRPTR paths[] =            /* paths to search for config file */
    {"", "", CONFIG_PATH,
     NULL, NULL};
    STRPTR path = NULL;
    UBYTE path_ctr = 0;
    FILE *cfgf = NULL;          /* prefs file */
    STRPTR hscenv = NULL;
    EXPSTR *hscpathstr = init_estr(32);         /* buffer to read $HSCPATH */

    static STRARR cfgfn[300];   /* TODO: expstr; buffer to create
                                 *   filename of config file */
    /* setup path from envvar */
    hscenv = getenv(ENV_HSCPATH);
    if (hscenv)
    {
#if 1
        if (link_envfname(hscpathstr, ENV_HSCPATH, NULL, NULL))
        {
            /* add hscenv to paths */
            paths[1] = estr2str(hscpathstr);
        }
#else
        /* copy envvar to own memory area */
        hscenv = strclone(hscenv);

        /* strip linefeeds from hscenv */
        while (strlen(hscenv) && (hscenv[strlen(hscenv)] == '\n'))
            hscenv[strlen(hscenv)] = '\0';

        /* add hscenv to paths */
        paths[1] = hscenv;
#endif
    }

    /* try to open any prefs-file */
    do
    {                           /* loop: */
        path = paths[path_ctr]; /*   get next path */
        if (path)
        {                       /*   is it the last one? */
            strcpy(cfgfn, path);        /*   N->generate filename */
            strcat(cfgfn, CONFIG_FILE);

            DC(fprintf(stderr, DHL "try \"%s\"\n", cfgfn));

            cfgf = fopen(cfgfn, "r");   /*      try to open file */
        }
        path_ctr++;             /*   process next path */
    }
    while (path && (!cfgf));    /* until no path left or file opened */

    if (cfgf)
    {
        prefs_fname = cfgfn;
        fclose(cfgf);
    }

    del_estr(hscpathstr);

    return (prefs_fname);
}

/*
 * hsc_read_base_info
 *
 * try to open base-file and read where in memory
 * information about defined tags/attributes/entities
 * is located.
 *
 * result: dummy-hsc-process that only contains
 *   the information read from base-file
 *
 * see "LoadHscPrefs/LoadHscPrefs.c"
 */
HSCPRC *hsc_read_base_info(VOID)
{
    HSCPRC *dummy_hp = NULL;

#ifdef HSCBASE_FILE

    FILE *inpf = fopen(HSCBASE_FILE, "r");
    DLLIST *hp_deftag = NULL;
    DLLIST *hp_defattr = NULL;
    DLLIST *hp_defent = NULL;

    if (inpf)
    {
        STRARR s[32];
        APTR p = NULL;

        while (fscanf(inpf, "%s %p\n", &(s[0]), &p) != EOF)
        {
            if (!strcmp("DEFTAG", s))
            {
                hp_deftag = (DLLIST *) p;
                printf(DHL "deftag  %p\n", hp_deftag);
            }
            else if (!strcmp("DEFATTR", s))
            {
                hp_defattr = (DLLIST *) p;
                printf(DHL "defattr %p\n", hp_defattr);
            }
            else if (!strcmp("DEFENT", s))
            {
                hp_defent = (DLLIST *) p;
                printf(DHL "defent  %p\n", hp_defent);
            }
            else
            {
                printf(DHL "%s %p (unknown)\n", s, p);
            }
        }
    }

    fclose(inpf);

    if (hp_deftag && hp_defattr && hp_defent)
    {
        /* assign information to dummy-process */
        dummy_hp = new_hscprc();

        del_dllist(dummy_hp->defattr);
        del_dllist(dummy_hp->defent);
        del_dllist(dummy_hp->deftag);

        dummy_hp->deftag = hp_deftag;
        dummy_hp->defattr = hp_defattr;
        dummy_hp->defent = hp_defent;

        /* assign new del_data-methodes */
        dummy_hp->defattr->del_data = del_hscattr;
        dummy_hp->defent->del_data = del_entity;
        dummy_hp->deftag->del_data = del_hsctag;
    }

#endif
    return (dummy_hp);
}

BOOL hsc_copy_base_info(HSCPRC * dest_hp, HSCPRC * dummy_hp)
{
    DLNODE *nd = dummy_hp->deftag->first;

    /* copy defined tags */
    while (nd)
    {
        HSCTAG *newtag = cpy_hsctag((HSCTAG *) nd->data);
        app_dlnode(dest_hp->deftag, (APTR) newtag);
        nd = nd->next;
    }

    return (TRUE);
}

/*
 * hsc_read_prefs
 *
 * try to open (any) config file and read preferences
 * from it
 */
BOOL hsc_read_prefs(HSCPRC * hp, STRPTR prefs_fname)
{
    BOOL ok = FALSE;

    /* find prefs file */
    if (!prefs_fname)
        prefs_fname = find_prefs_fname(hp);

    /* status message */
    if (prefs_fname)
    {
        dbg_disable(hp);

        hsc_status_file_begin(hp, prefs_fname);
        ok = hsc_include_file(hp, prefs_fname,
                              IH_PARSE_HSC | IH_NO_STATUS);
        dbg_restore(hp);

        if (ok)
        {
            EXPSTR *msg = init_estr(32);
            set_estr(msg, prefs_fname);
            app_estr(msg, ": preferences read");
            hsc_status_misc(hp, estr2str(msg));
            del_estr(msg);
        }
    }
    else
    {
        hsc_message(hp, MSG_NO_CONFIG,
                    "can not open preferences file");
    }

    return (ok);
}

/*
 * callback to display "project-file corrupt"-message
 */
static VOID msg_corrupt_pf(HSCPRJ * project, STRPTR reason)
{
    STRPTR prjtxt = "project-file corrupt";
    HSCPRC *hp = (HSCPRC *) project->user_data;

    if (reason)
        hsc_message(hp, MSG_CORRUPT_PRJFILE, "%s (%s)", prjtxt, reason);
    else
        hsc_message(hp, MSG_CORRUPT_PRJFILE, "%s", prjtxt);
}

/*
 * hsc_init_project
 *
 * read project-file
 */
BOOL hsc_init_project(HSCPRC * hp, STRPTR project_fname)
{
    BOOL ok = FALSE;

    /* init project */
    hp->project = new_project();
    hp->project->user_data = (APTR) hp;
    hp->project->debug = hp->debug;
    hp->project->CB_msg_corrupt_pf = msg_corrupt_pf;

    if (project_fname)
    {
        /*
         * read project-data
         */
        D(fprintf(stderr, DHL "read project-file `%s'\n", project_fname));

        hsc_status_file_begin(hp, project_fname);

        /* read project-file */
        hp->inpf = infopen(project_fname, 0);

        if (hp->inpf)
        {
            ok = hsc_project_read_data(hp->project, hp->inpf);
            infclose(hp->inpf);
            if (ok)
            {
                /* message about success */
                EXPSTR *msg = init_estr(32);
                set_estr(msg, project_fname);
                app_estr(msg, ": project-file read");
                hsc_status_misc(hp, estr2str(msg));
                del_estr(msg);
            }

            hp->inpf = NULL;
        }
        else
        {
            D(fprintf(stderr, DHL "  can't read project-file\n"));
            ok = TRUE;
            /* TODO: message "creating new one" */
        }
    }
    else
    {
        D(fprintf(stderr, DHL "no project-file to load\n"));
        ok = TRUE;
    }

    if (ok)
    {
        /* dettach current document */
        hsc_project_set_document(hp->project, hp->filename_document);
    }

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

    if (tag && !(tag->option & HT_NOHANDLE))
    {
        /* set handles */
        DC(fprintf(stderr, DHL "add handles for <%s> (%p,%p)\n",
                   name, op_hnd, cl_hnd));
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
BOOL hsc_init_tagsNattr(HSCPRC * hp)
{
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
        HSC_DEFENT_STR " /SKIPLF NAME:string/r RPLC:string NUM:num>",
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
        NULL, NULL};

    /* temporarily disable debugging output */
    dbg_disable(hp);

    /* define hsc-tags */
    i = 0;
    while (!(hp->fatal) && hsc_prefs[i])
    {
        STRARR infname[20];

        sprintf(infname, SPECIAL_FILE_ID "init%3d", i);
        hp->inpf = infopen_str(infname, hsc_prefs[i], 60);

        if (hp->inpf)
        {
            tag = def_tag_name(hp, &open_tag);
            ok = (tag && def_tag_args(hp, tag));
            infclose(hp->inpf);
        }

        i++;
    }

    /* init hsc-attributes */
    i = 0;
    while (!(hp->fatal) && hsc_attribs[i])
    {
        define_attr_by_text(hp, hsc_attribs[i], hsc_attribs[i+1], 0);
        i+=2;
    }

    /* assign "\n" to linefeed-attribute */
    set_vartext(find_varname(hp->defattr, LINEFEED_ATTR), "\n");

    /* assign tag-callbacks to hsc-tags */
    if (ok)
    {
        hsc_set_tagCB(hp, HSC_COMMENT_STR, handle_hsc_comment, NULL);
        hsc_set_tagCB(hp, HSC_CONTENT_STR, handle_hsc_content, NULL);
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
 * create basic entities (&amp;, &gt;, &lt;, &quot;)
 * (should be called BEFORE hsc_read_prefs())
 */
BOOL hsc_init_basicEntities(HSCPRC * hp)
{
    BOOL ok = TRUE;

    /* entities */
    ok &= add_ent(hp->defent, "amp", NULL, 0);  /* & */
    ok &= add_ent(hp->defent, "lt", NULL, 0);   /* < */
    ok &= add_ent(hp->defent, "gt", NULL, 0);   /* > */
    ok &= add_ent(hp->defent, "quot", NULL, 0);         /* q */

    return (ok);
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

    if (hsc_init_tagsNattr(hp)
        && hsc_init_basicEntities(hp)
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
