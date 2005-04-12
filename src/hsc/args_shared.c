/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2005 Matthias Bethke
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
 * hsc/args.c
 *
 * user argument handling, interface style independent parts
 *
 * created: 12-Apr-2005
 */

#include "hsc/global.h"
#include "hsc/status.h"
#include "hsc/callback.h"

#include "ugly/fname.h"
#include "ugly/uargs.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"

#ifdef AMIGA_STYLE_ARGS
#define ARG_ALTERNATES_SEPARATOR_STR "|"
#define ARG_ALTERNATES_SEPARATOR_CHAR '|'
#else /* AMIGA_STYLE_ARGS */
#define ARG_ALTERNATES_SEPARATOR_STR ","
#define ARG_ALTERNATES_SEPARATOR_CHAR ','
#endif /* AMIGA_STYLE_ARGS */

HSCPRC *arg_hp = NULL;
EXPSTR *fileattr_str = NULL;

/*
 * arg_ignore_CB
 *
 * argument handler for special values that are passed
 * to "IGNORE=.." several messages are set to be ignored
 * with the old messages left active
 */
STRPTR arg_ignore_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR arg_clone = strclone(arg);   /* copy of arg; written by strtok() */
    HSCPRC *hp = arg_hp;
    STRPTR nxt_arg = strtok(arg_clone, ARG_ALTERNATES_SEPARATOR_STR);    /* use system-dependent string to tokenize */

    while (nxt_arg) {
        D(fprintf(stderr, DHSC "  ignore `%s'\n", nxt_arg));
        if (!upstrcmp(nxt_arg, IGNORE_ALL_STR)) {
            /* ignore all non-error messages */
            HSCMSG_ID i;

            for (i = 0; i < MAX_MSGID; i++)
                hsc_set_msg_ignore(hp, i, TRUE);
        } else if (!upstrcmp(nxt_arg, IGNORE_NOTES_STR)) {
            /* ignore note messages */
            hsc_set_msg_ignore_notes(hp, TRUE);
        } else if (!upstrcmp(nxt_arg, IGNORE_BADSTYLE_STR)) {
            /* ignore bad style messages */
            hsc_set_msg_ignore_style(hp, TRUE);
        } else if (!upstrcmp(nxt_arg, IGNORE_PORTABILITY_STR)) {
            /* ignore potability problems */
            hsc_set_msg_ignore_port(hp, TRUE);
        } else {
            /* ignore message # */
            LONG ignnum;

            if (!str2long(nxt_arg, &ignnum))
                errmsg = "unknown `ignore'";
            else
                hsc_set_msg_ignore(hp, ignnum, ignore);
        }

        /* next arg */
        nxt_arg = strtok(NULL, ARG_ALTERNATES_SEPARATOR_STR);
    }                           /* while(nxt_arg) */

    /* cleanup */
    ufreestr(arg_clone);

    return (errmsg);
}

/*
 * arg_enable_CB
 *
 * argument handler for special values that are passed
 * to "enable=.." several messages are set to be enabled
 * with the old messages left active
 */
STRPTR arg_enable_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR arg_clone = strclone(arg);   /* copy of arg; written by strtok() */
    HSCPRC *hp = arg_hp;
    STRPTR nxt_arg = strtok(arg_clone, ARG_ALTERNATES_SEPARATOR_STR);    /* use system dependent string to tokenize */

    while (nxt_arg) {
        D(fprintf(stderr, DHSC "  enable `%s'\n", nxt_arg));
        if (!upstrcmp(nxt_arg, IGNORE_ALL_STR)) {
            /* enable all non-error messages */
            HSCMSG_ID i;

            for (i = 0; i < MAX_MSGID; i++)
                hsc_set_msg_ignore(hp, i, enable);
        } else if (!upstrcmp(nxt_arg, IGNORE_NOTES_STR)) {
            /* enable note messages */
            hsc_set_msg_ignore_notes(hp, FALSE);
        } else if (!upstrcmp(nxt_arg, IGNORE_BADSTYLE_STR)) {
            /* enable bad style messages */
            hsc_set_msg_ignore_style(hp, FALSE);
        } else if (!upstrcmp(nxt_arg, IGNORE_PORTABILITY_STR)) {
            /* enable potability problems */
            hsc_set_msg_ignore_port(hp, FALSE);
        } else {
            /* ignore message # */
            LONG ignnum;

            if (!str2long(nxt_arg, &ignnum))
                errmsg = "unknown `enable'";
            else
                hsc_set_msg_ignore(hp, ignnum, enable);
        }

        /* next arg */
        nxt_arg = strtok(NULL, ARG_ALTERNATES_SEPARATOR_STR);
    }                           /* while(nxt_arg) */

    /* cleanup */
    ufreestr(arg_clone);

    return (errmsg);
}

/*
 * arg_include_CB
 *
 * argument handler for include directory (IDIR=..)
 */
STRPTR arg_incdir_CB(STRPTR arg)
{
    HSCPRC *hp = arg_hp;
    hsc_add_include_directory(hp, arg);
    return (NULL);
}

/*
 * arg_mode
 *
 * argument handler for values that are passed
 * to "MODE=..". this one resets all ignored
 * messages.
 */
STRPTR arg_mode_CB(CONSTRPTR arg)
{
    STRPTR errmsg = NULL;
    size_t mode = strenum(arg, MODE_ENUMSTR, ARG_ALTERNATES_SEPARATOR_CHAR, STEN_NOCASE);
    HSCPRC *hp = arg_hp;

    D(fprintf(stderr, DHSC "args: mode=%s\n", arg));

    if (!mode)
        errmsg = "unknown mode";
    else if (mode == MODE_PEDANTIC) {
        /* pedantic */
        /* enable all messages */
        HSCMSG_ID i;

        for (i = 0; i < MAX_MSGID; i++)
            hsc_set_msg_ignore(hp, i, make_my_day);

        /* enable all classes */
        hsc_set_msg_ignore_notes(hp, FALSE);
        hsc_set_msg_ignore_style(hp, FALSE);
        hsc_set_msg_ignore_port(hp, FALSE);
    } else if (mode == MODE_NORMAL) {
        /* normal */
        /* ignore note messages */
        arg_mode_CB(MODE_PEDANTIC_STR);
        arg_ignore_CB(IGNORE_NOTES_STR);
        hsc_set_msg_ignore(hp, MSG_WRONG_HEADING, TRUE);
        hsc_set_msg_ignore(hp, MSG_LF_IN_COMMENT, TRUE);
    } else if (mode == MODE_RELAXED) {
        /* relaxed */
        arg_mode_CB(MODE_NORMAL_STR);
        arg_ignore_CB(IGNORE_BADSTYLE_STR);
        arg_ignore_CB(IGNORE_PORTABILITY_STR);
        arg_ignore_CB(IGNORE_JERKS_STR);
        hsc_set_msg_ignore(hp, MSG_MISS_REQTAG, TRUE);
        hsc_set_msg_ignore(hp, MSG_TAG_OBSOLETE, TRUE);
        hsc_set_msg_ignore(hp, MSG_TAG_TOO_OFTEN, TRUE);
        hsc_set_msg_ignore(hp, MSG_CTAG_NESTING, TRUE);
    } else {
        /* ignore message # */
        LONG ignnum;

        if (!str2long(arg, &ignnum))
            errmsg = "illegal argument";
        else
            hsc_set_msg_ignore(hp, ignnum, TRUE);
    }
    return (errmsg);
}

/*
 * arg_status
 *
 * argument handler for values that are passed
 * to "STATUS=..".
 */
STRPTR arg_status_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR argstr = strclone(arg);
    STRPTR argold = argstr;     /* used to call ufree() */
#if DEBUG
    HSCPRC *hp = arg_hp;
#endif
    D(fprintf(stderr, DHSC "args: status=%s\n", arg));

    arg = strtok(argstr, ARG_ALTERNATES_SEPARATOR_STR);
    while (arg) {
        if (!upstrcmp(arg, STATUS_QUIET_STR))
            disp_status = STATUS_QUIET;
        else if (!upstrcmp(arg, STATUS_LINE_STR))
            disp_status |= STATUS_LINE;
        else if (!upstrcmp(arg, STATUS_VERSION_STR))
            disp_status |= STATUS_VERSION;
        else if (!upstrcmp(arg, STATUS_VERBOSE_STR))
            disp_status |= STATUS_VERBOSE;
        else if (!upstrcmp(arg, STATUS_FULL_STR))
            disp_status = -1;
        else
            errmsg = "illegal argument";

        arg = strtok(NULL, ARG_ALTERNATES_SEPARATOR_STR);
    }
    ufreestr(argold);
    return (errmsg);
}

/*
 * set/define_dest_attribs, define_source_attribs
 *
 * set and define attributes for destination uri
 */
VOID set_dest_attribs(HSCPRC * hp,
                STRPTR destpath, STRPTR reldestpath, STRPTR destname)
{
    set_estr(fileattr_str, "<$define HSC.DOCUMENT.NAME:string/c=\"");
    if (destname)
        app_estr(fileattr_str, destname);
    app_estr(fileattr_str, "\">\n<$define HSC.DOCUMENT.PATH:string/c=\"");
    if (reldestpath)
        app_estr(fileattr_str, reldestpath);
    app_estr(fileattr_str, "\">\n<$define HSC.DESTPATH:string/c=\"");
    if (destpath)
        app_estr(fileattr_str, destpath);
    app_estr(fileattr_str, "\">\n<$define HSC.DOCUMENT.URI:string/c=\"");
    if (destname) {
        app_estr(fileattr_str, reldestpath);
        app_estr(fileattr_str, destname);
    }
    app_estr(fileattr_str, "\">\n");
}

/* set_source_attribs */
VOID set_source_attribs(HSCPRC * hp, STRPTR sourcepath, STRPTR sourcename)
{
    app_estr(fileattr_str, "<$define HSC.SOURCE.NAME:string/c=\"");
    if (sourcename)
        app_estr(fileattr_str, sourcename);
    app_estr(fileattr_str, "\">\n<$define HSC.SOURCE.PATH:string/c=\"");
    if (sourcename)
        app_estr(fileattr_str, sourcepath);
    app_estr(fileattr_str, "\">\n<$define HSC.SOURCE.FILE:string/c=\"");
    if (sourcename) {
        app_estr(fileattr_str, sourcepath);
        app_estr(fileattr_str, sourcename);
    }
    app_estr(fileattr_str, "\">\n");
}

/* set global attributes to query HSC options from source */
VOID set_global_attribs(HSCPRC * hp)
{
   STRPTR emode;

   if (hp->xhtml)
      app_estr(fileattr_str, "<$define HSC.OPTS.XHTML:bool/c=\"1\">\n");
   app_estr(fileattr_str, "<$define HSC.OPTS.ENTITYMODE:string/c=\"");
   switch(hp->entitymode) {
      case EMODE_REPLACE :
         emode = "replace";
         break;
      case EMODE_NUMERIC :
         emode = "numeric";
         break;
      case EMODE_SYMBOLIC :
         emode = "symbolic";
         break;
      case EMODE_UTF8 :
         emode = "utf-8";
         break;
      case EMODE_KEEP :
      default :
         emode = "keep";
         break;
   }
   app_estr(fileattr_str, emode);
   app_estr(fileattr_str, "\">\n");
}


/* set_file_attribs */
VOID define_file_attribs(HSCPRC * hp)
{
    hsc_include_string(hp, "[define destattr]", estr2str(fileattr_str),
                       IH_PARSE_HSC | IH_NO_STATUS);

    D(fprintf(stderr, DHSC "destination attributes defines:\n%s",
              estr2str(fileattr_str))
        );
}


