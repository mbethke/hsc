/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * user argument handling for hsc
 *
 * updated: 29-Nov-1997
 * created:  1-Jul-1995
 */

#include "hsc/global.h"
#include "hsc/status.h"

/*
 * ugly includes
 */
#include "ugly/uargs.h"
#include "ugly/fname.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"

#include "hscprj/license.h"

/* default values for some arguments */
#define DEFAULT_EXTENSION "html"
#define DEFAULT_MAXERR    "20"
#define DEFAULT_MAXMSG    "40"

static STRPTR arg_inpfname = NULL;      /* temp vars for set_args() */
static STRPTR arg_outfname = NULL;
static STRPTR arg_extension = NULL;
static STRPTR arg_server_dir = NULL;
static LONG arg_quotemode = QMODE_KEEP;
static BOOL arg_mode = FALSE;
static BOOL arg_compact = FALSE;
static BOOL arg_getsize = FALSE;
static BOOL arg_rplc_ent = FALSE;
static BOOL arg_rplc_quote = FALSE;
static BOOL arg_smart_ent = FALSE;
static BOOL arg_jens = FALSE;
static BOOL arg_strip_cmt = FALSE;
static BOOL arg_strip_badws = FALSE;
static BOOL arg_strip_ext = FALSE;
static BOOL arg_weenix = FALSE;
static BOOL arg_license = FALSE;
static BOOL arg_help = FALSE;
static BOOL arg_debug = FALSE;
static STRPTR arg_iconbase = NULL;
static STRPTR arg_striptags = NULL;
#if 0
static LONG arg_entmode = EMODE_KEEP;
#endif

static HSCPRC *arg_hp = NULL;

/* contains defines for destination-attributes */
static EXPSTR *fileattr_str = NULL;

static ARGFILE *argf = NULL;

/*
 * cleanup_hsc_args: free local resources
 */
VOID cleanup_hsc_args(VOID)
{
    del_argfile(argf);
    del_estr(fileattr_str);
}

/*
 * arg_ignore_CB
 *
 * argument handler for special values that are passed
 * to "IGNORE=.." several messages are set to be ignored
 * with the old messages left active
 */
static STRPTR arg_ignore_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR arg_clone = strclone(arg);   /* copy of arg; written by strtok() */
    HSCPRC *hp = arg_hp;
    STRPTR nxt_arg = strtok(arg_clone, "|");    /* use "|" to tokenize */

    while (nxt_arg)
    {
        D(fprintf(stderr, DHSC "  ignore `%s'\n", nxt_arg));
        if (!upstrcmp(nxt_arg, IGNORE_ALL_STR))
        {
            /* ignore all non-error messages */
            HSCMSG_ID i;

            for (i = 0; i < MAX_MSGID; i++)
                hsc_set_msg_ignore(hp, i, TRUE);
        }
        else if (!upstrcmp(nxt_arg, IGNORE_NOTES_STR))
        {
            /* ignore note messages */
            hsc_set_msg_ignore_notes(hp, TRUE);
        }
        else if (!upstrcmp(nxt_arg, IGNORE_BADSTYLE_STR))
        {
            /* ignore bad style messages */
            hsc_set_msg_ignore_style(hp, TRUE);
        }
        else if (!upstrcmp(nxt_arg, IGNORE_PORTABILITY_STR))
        {
            /* ignore potability problems */
            hsc_set_msg_ignore_port(hp, TRUE);
        }
        else
        {
            /* ignore message # */
            LONG ignnum;

            if (!str2long(nxt_arg, &ignnum))
            {
                errmsg = "unknown `ignore'";
            }
            else
            {
                hsc_set_msg_ignore(hp, ignnum, ignore);
            }
        }

        /* next arg */
        nxt_arg = strtok(NULL, "|");
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
static STRPTR arg_enable_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR arg_clone = strclone(arg);   /* copy of arg; written by strtok() */
    HSCPRC *hp = arg_hp;
    STRPTR nxt_arg = strtok(arg_clone, "|");    /* use "|" to tokenize */

    while (nxt_arg)
    {
        D(fprintf(stderr, DHSC "  enable `%s'\n", nxt_arg));
        if (!upstrcmp(nxt_arg, IGNORE_ALL_STR))
        {
            /* enable all non-error messages */
            HSCMSG_ID i;

            for (i = 0; i < MAX_MSGID; i++)
            {
                hsc_set_msg_ignore(hp, i, enable);
            }
        }
        else if (!upstrcmp(nxt_arg, IGNORE_NOTES_STR))
        {
            /* enable note messages */
            hsc_set_msg_ignore_notes(hp, FALSE);
        }
        else if (!upstrcmp(nxt_arg, IGNORE_BADSTYLE_STR))
        {
            /* enable bad style messages */
            hsc_set_msg_ignore_style(hp, FALSE);
        }
        else if (!upstrcmp(nxt_arg, IGNORE_PORTABILITY_STR))
        {
            /* enable potability problems */
            hsc_set_msg_ignore_port(hp, FALSE);
        }
        else
        {
            /* ignore message # */
            LONG ignnum;

            if (!str2long(nxt_arg, &ignnum))
            {
                errmsg = "unknown `enable'";
            }
            else
            {
                hsc_set_msg_ignore(hp, ignnum, enable);
            }
        }

        /* next arg */
        nxt_arg = strtok(NULL, "|");
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
static STRPTR arg_incdir_CB(STRPTR arg)
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
static STRPTR arg_mode_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    size_t mode = strenum(arg, MODE_ENUMSTR, '|', STEN_NOCASE);
    HSCPRC *hp = arg_hp;

    D(fprintf(stderr, DHSC "args: mode=%s\n", arg));

    if (!mode)
    {
        errmsg = "unknown mode";
    }
    else if (mode == MODE_PEDANTIC)
    {
        /* pedantic */
        /* enable all messages */
        HSCMSG_ID i;

        for (i = 0; i < MAX_MSGID; i++)
        {
            hsc_set_msg_ignore(hp, i, make_my_day);
        }

        /* enable all classes */
        hsc_set_msg_ignore_notes(hp, FALSE);
        hsc_set_msg_ignore_style(hp, FALSE);
        hsc_set_msg_ignore_port(hp, FALSE);
    }
    else if (mode == MODE_NORMAL)
    {
        /* normal */
        /* ignore note messages */
        arg_mode_CB(MODE_PEDANTIC_STR);
        arg_ignore_CB(IGNORE_NOTES_STR);
        hsc_set_msg_ignore(hp, MSG_MISS_REQTAG, TRUE);
        hsc_set_msg_ignore(hp, MSG_WRONG_HEADING, TRUE);
        hsc_set_msg_ignore(hp, MSG_LF_IN_COMMENT, TRUE);
    }
    else if (mode == MODE_RELAXED)
    {
        /* relaxed */
        arg_mode_CB(MODE_NORMAL_STR);
        arg_ignore_CB(IGNORE_BADSTYLE_STR);
        arg_ignore_CB(IGNORE_PORTABILITY_STR);
        arg_ignore_CB(IGNORE_JERKS_STR);
        hsc_set_msg_ignore(hp, MSG_TAG_OBSOLETE, TRUE);
        hsc_set_msg_ignore(hp, MSG_TAG_TOO_OFTEN, TRUE);
        hsc_set_msg_ignore(hp, MSG_CTAG_NESTING, TRUE);
        hsc_set_msg_ignore(hp, MSG_EXPT_SEMIC, TRUE);
    }
    else
    {
        /* ignore message # */
        LONG ignnum;

        if (!str2long(arg, &ignnum))
        {
            errmsg = "illegal argument";
        }
        else
        {
            hsc_set_msg_ignore(hp, ignnum, TRUE);
        }
    }

    return (errmsg);
}

/*
 * arg_status
 *
 * argument handler for values that are passed
 * to "STATUS=..".
 */
static STRPTR arg_status_CB(STRPTR arg)
{
    STRPTR errmsg = NULL;
    STRPTR argstr = strclone(arg);
    STRPTR argold = argstr;     /* used to call ufree() */
#if DEBUG
    HSCPRC *hp = arg_hp;
#endif
    D(fprintf(stderr, DHSC "args: status=%s\n", arg));

    arg = strtok(argstr, "|");
    while (arg)
    {
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

        arg = strtok(NULL, "|");
    }
    ufreestr(argold);

    return (errmsg);
}

/*
 * set/define_dest_attribs, define_source_attribs
 *
 * set and define attributes for destiantion uri
 */
static VOID set_dest_attribs(HSCPRC * hp, STRPTR destpath, STRPTR destname)
{
    set_estr(fileattr_str, "<$define HSC.DOCUMENT.NAME:string/c=\"");
    if (destname)
        app_estr(fileattr_str, destname);
    app_estr(fileattr_str, "\">\n<$define HSC.DOCUMENT.PATH:string/c=\"");
    if (destname)
        app_estr(fileattr_str, destpath);
    app_estr(fileattr_str, "\">\n<$define HSC.DOCUMENT.URI:string/c=\"");
    if (destname)
        app_estr(fileattr_str, destpath);
    if (destname)
        app_estr(fileattr_str, destname);
    app_estr(fileattr_str, "\">\n");
}

/* set_source_attribs */
static VOID set_source_attribs(HSCPRC * hp, STRPTR sourcepath, STRPTR sourcename)
{
    app_estr(fileattr_str, "<$define HSC.SOURCE.NAME:string/c=\"");
    if (sourcename)
        app_estr(fileattr_str, sourcename);
    app_estr(fileattr_str, "\">\n<$define HSC.SOURCE.PATH:string/c=\"");
    if (sourcename)
        app_estr(fileattr_str, sourcepath);
    app_estr(fileattr_str, "\">\n<$define HSC.SOURCE.FILE:string/c=\"");
    if (sourcename)
        app_estr(fileattr_str, sourcepath);
    if (sourcename)
        app_estr(fileattr_str, sourcename);
    app_estr(fileattr_str, "\">\n");
}

/* set_file_attribs */
static VOID define_file_attribs(HSCPRC * hp)
{
    hsc_include_string(hp, "[define destattr]", estr2str(fileattr_str),
                       IH_PARSE_HSC | IH_NO_STATUS);

    D(fprintf(stderr, DHSC "destination attributes defines:\n%s",
              estr2str(fileattr_str))
        );
}

/*
 * user_defines_ok
 *
 * process all defines passed via user args
 *
 * result: always TRUE
 */
BOOL user_defines_ok(HSCPRC * hp)
{
    /* define destination attributes (HSC.DOCUMENT.URI etc.) */
    define_file_attribs(hp);

    if (define_list && dll_first(define_list))
    {
        DLNODE *nd = dll_first(define_list);
        EXPSTR *defbuf = init_estr(64);
#if 0 /* TODO: remove this */
        BOOL old_ignore_quotemsg =
        hsc_get_msg_ignore(hp, MSG_ARG_NO_QUOTE);
#endif

        while (nd)
        {
            STRPTR defarg = (STRPTR) dln_data(nd);

            D(fprintf(stderr, DHSC "define using `%s'\n", defarg));

            set_estr(defbuf, "<$define ");

            /* append attribute name */
            do
            {
                app_estrch(defbuf, defarg[0]);
                defarg++;
            }
            while (defarg[0] && (defarg[0] != '=')
                   && (defarg[0] != '/') && (defarg[0] != ':'));

            /* if no type set, use "string" as default */
            if (defarg[0] != ':')
            {
                app_estr(defbuf, ":string");
            }

            /* append type (if set) and attribute-flags */
            while (defarg[0] && (defarg[0] != '='))
            {
                app_estrch(defbuf, defarg[0]);
                defarg++;
            }

            /* append value (if any) and quotes */
            if (defarg[0] == '=')
            {
                char quote_needed = 0;  /* flag: user did not use quotes */

                /* append "=" */
                app_estrch(defbuf, defarg[0]);
                defarg++;

                /* check which kind of quote should be appended */
                if ((defarg[0] != '\"') && (defarg[0] != '\''))
                {
                    BOOL single_quote = FALSE;
                    BOOL double_quote = FALSE;
                    STRPTR scanarg = defarg;

                    /* scan value for quotes */
                    while (scanarg[0])
                    {
                        if (scanarg[0] == '\"')
                            double_quote = TRUE;
                        else if (scanarg[0] == '\'')
                            single_quote = TRUE;
                        scanarg++;
                    }

                    /* choose quote to enclose value */
                    if (!double_quote)
                        quote_needed = '\"';
                    else if (!single_quote)
                        quote_needed = '\'';
                    else
                        panic("both quotes in value");
                }

                /* append quote (if not already done by user) */
                if (quote_needed)
                    app_estrch(defbuf, quote_needed);

                /* append value */
                while (defarg[0])
                {
                    app_estrch(defbuf, defarg[0]);
                    defarg++;
                }

                /* append quote (if not already done by user) */
                if (quote_needed)
                    app_estrch(defbuf, quote_needed);

            }

            /* append end ">" */
            app_estrch(defbuf, '>');

            D(fprintf(stderr, DHSC "define: `%s'\n", estr2str(defbuf)));

            hsc_include_string(hp, "DEFINE",
                             estr2str(defbuf), IH_PARSE_HSC | IH_NO_STATUS);
            nd = dln_next(nd);
        }

        del_estr(defbuf);
#if 0
        hsc_set_msg_ignore(hp, MSG_ARG_NO_QUOTE, old_ignore_quotemsg);
#endif
    }
    else
    {
        D(fprintf(stderr, DHSC "(no defines)\n"));
    }

    return ((BOOL) (return_code < RC_ERROR));
}

/*
 * args_ok
 *
 * prepare args, check & parse user args, display error and
 * help message if neccessary
 *
 * result: TRUE, if all args ok
 */
BOOL args_ok(HSCPRC * hp, int argc, char *argv[])
{
    BOOL ok;                    /* return value */
    DLLIST *ignore_list = NULL; /* dummy */
    EXPSTR *destdir = init_estr(32);    /* destination dir */
    EXPSTR *rel_destdir = init_estr(32);        /* relative destination dir */
    EXPSTR *kack_name = init_estr(0);   /* temp. str for outfilename */
    struct arglist *hsc_args;   /* argument structure */
    LONG maximum_number_of_errors = strtol(DEFAULT_MAXERR, (char **) NULL, 10);
    LONG maximum_number_of_messages = strtol(DEFAULT_MAXMSG, (char **) NULL, 10);

#if (defined MSDOS)             /* HSC_GUN */
#define SWAPSIZE     (32*4)     /* 32 MB */
#define SWAPSIZE_BUF (256*1024)
#define SWAPNAME     "hsc.swap"
    /* create swap-file if neccessary */
    FILE *swapfile = fopen(SWAPNAME, "rb");
    if (!swapfile)
    {
        fprintf(stderr, "Creating swapfile..");
        fflush(stderr);
        fclose(swapfile);
        swapfile = fopen(SWAPNAME, "wb");
        if (swapfile)
        {
            STRPTR buf = (STRPTR) umalloc(SWAPSIZE_BUF);
            STRPTR swaptext = "swap me up, before you go-go! ";
            size_t len = strlen(swaptext);
            size_t i = 0;

            /* init buffer */
            for (i = 0; i < (SWAPSIZE_BUF - len); i += len)
            {
                strcpy(&(buf[i]), swaptext);
            }

            /* write swapfile */
            for (i = 0; i < SWAPSIZE; i++)
            {
                STRPTR progress = "/-\\|";
                fprintf(stderr, "%c\b", progress[i % 4]);
                fflush(stderr);
                fwrite(buf, 1, SWAPSIZE_BUF, swapfile);
            }
            ufree(buf);
        }
        fprintf(stderr, "                    \r");
        fflush(stderr);
    }
    if (!swapfile)
    {
        fprintf(stderr, "** failed creating swapfile");
        exit(RC_FAIL);
    }
    else
    {
        fclose(swapfile);
    }
#endif

    arg_hp = hp;
    arg_mode_CB(DEFAULT_MODE_STR);

    /* create arg-table */
    hsc_args =
        prepare_args("HSC_ARGS",

    /* file args */
                     "FROM/M", &incfile,
                     "include- and input file(s)",

                     "TO/K", &arg_outfname,
                     "output file (default: stdout)",

                     "PRJFILE/T/K", &prjfilename,
                     "project file (default: none)",

                     "PREFSFILE/T/K", &prefsfilename,
                     "syntax definition file",

                     "MSGFILE=MF/T/K", &msgfilename,
                     "message file (default: stderr)",

                     "MSGFORMAT/T/K", &msg_format,
                     "how to display messages",
    /* numeric */
                     "MAXERR/N/K", &maximum_number_of_errors,
                     "max. number of errors (default: " DEFAULT_MAXERR ")",

                     "MAXMSG/N/K", &maximum_number_of_messages,
                     "max. number of messages (default: " DEFAULT_MAXMSG ")",

                     "EXTENSION/T/K", &arg_extension,
                   "output file extension (default: " DEFAULT_EXTENSION ")",

                     "DEFINE=DEF/T/K/M", &define_list,
                     "define global attribute",

                     "IGNORE=IGN/K/M/$", arg_ignore_CB, &ignore_list,
                     "ignore message number or class",

                     "ENABLE=ENA/K/M/$", arg_enable_CB, &ignore_list,
                     "enable message number or class",

                     "MODE/E/K/$", arg_mode_CB, MODE_ENUMSTR, &arg_mode,
                     "mode for syntax check (" MODE_ENUMSTR ")",

                     "QUOTESTYLE=QS/E/K", QMODE_ENUMSTR, &arg_quotemode,
                     "defines how quotes appear (" QMODE_ENUMSTR ")",
#if 0
                     "ENTITYSTYLE=ES/E/K", EMODE_ENUMSTR, &entmode,
                     "defines how special chars. appear (" EMODE_ENUMSTR ")",
#endif
                     "INCLUDEDIR=IDIR/K/M/$", arg_incdir_CB, &ignore_list,
                     "add include directory",

    /* switches */
                     "COMPACT=CO/S", &arg_compact,
                     "strip useless white spaces",

                     "GETSIZE/S", &arg_getsize,
                     "get width and height of images",

                     "MSGANSI/S", &msg_ansi,
                     "use ansi-sequences in messages",

                     "RPLCENT=RE/S", &arg_rplc_ent,
                     "replace special characters",

                     "RPLCQUOTE=RQ/S", &arg_rplc_quote,
                     "replace quotes in text by `&quot;'",

                     "SMARTENT=SA/S", &arg_smart_ent,
                     "replace special entities (`&<>')",

                     "JENS/S", &arg_jens,
                     "don't try this at home",

                     "STRIPBADWS/S", &arg_strip_badws,
                     "strip bad white spaces",

                     "STRIPCOMMENT=SC/S", &arg_strip_cmt,
                     "strip SGML comments",

                     "STRIPEXTERNAL=SX/S", &arg_strip_ext,
                     "strip tags with external URIs",

                     "STRIPTAGS=ST/K", &arg_striptags,
                     "tags to be stripped",

                     "ICONBASE/T/K", &arg_iconbase,
                     "base URI for icon entities",

                     "SERVERDIR/T/K", &arg_server_dir,
                     "base directory for server relative URIs",

                     "STATUS/E/K/$", arg_status_CB,
                     STATUS_ENUM_STR, &disp_status,
                     "status message (" STATUS_ENUM_STR ")",
#if 0
                     "WEENIX/S", &arg_weenix,
                     "weenix compatibility mode",
#endif
                     "-DEBUG/S", &arg_debug, "enable debugging output",
    /* help */
                     "HELP=?=-h=--help/S", &arg_help, "display this text",
                     "LICENSE/S", &arg_license, "display license",

                     NULL);

    /* remove dummy list TODO: this sucks */
    del_dllist(ignore_list);

    ok = (hsc_args != NULL);

    /* set & test args */
    if (ok)
    {
        BOOL use_stdout = FALSE;        /* flag: use stdout as output-file */
        BOOL any_input_passed = FALSE;  /* flag: any input specified in args */
        STRPTR argfiles[] =
        {OPTION_FILE, NULL};

        argf = new_argfilev(argfiles);

        ok = set_args_file(argf, hsc_args)
            && set_args(argc, argv, hsc_args);

        /* display help, if requested vie HELP switch, or no
         * input to pipe or read is passed */
        any_input_passed = (incfile && dll_first(incfile));
        ok &= (!arg_help && any_input_passed);

        if (arg_license)
        {
            /* display license text */
            fprintf_prginfo(stderr);
            show_license();
            set_return_code(RC_WARN);
        }
        else if (!ok)
        {
            if (arg_help || !any_input_passed)
            {
                /* display help, if HELP-switch set */
                fprintf_prginfo(stderr);
                fprintf_arghelp(stderr, hsc_args);
            }
            set_return_code(RC_WARN);
        }
        else
        {
            BOOL fnsux = FALSE; /* flag: TRUE = can't evaluate out-filename */

            /* set debugging switch */
            hsc_set_debug(hp, arg_debug);

            /* autoset depending options */
            if (hsc_get_debug(hp))
                disp_status = STATUS_VERBOSE;

            /* set default options */
            if (!arg_extension)
                arg_extension = DEFAULT_EXTENSION;

            /* disable ID-warning if no project-file */
            if (!prjfilename)
                hsc_set_msg_ignore(hp, MSG_NO_DOCENTRY, TRUE);

            /* compute name of input file */
            arg_inpfname = NULL;
            if (dll_first(incfile))
            {
                /* use last FROM as input file */
                arg_inpfname = dln_data(dll_last(incfile));

                set_estr(inpfilename, arg_inpfname);

                /* get path part of inputfilename as relative
                 * destination directory */
                get_fpath(rel_destdir, arg_inpfname);

                /* TODO: set reldir when including first file */
                /* TODO: find out why the above TODO is there */

                /* remove input filename from incfile */
                del_dlnode(incfile, dll_last(incfile));

                D(fprintf(stderr, DHSC "input : use `%s'\n"
                          DHSC "reldir: use `%s'\n",
                          estr2str(inpfilename), estr2str(rel_destdir)));
            }

            /* display include files */
            D(
                 {
                 DLNODE * nd = dll_first(incfile);

                 while (nd)
                 {
                 fprintf(stderr, DHSC "includ: use `%s'\n", (
                                                      STRPTR) dln_data(nd));
                 nd = dln_next(nd);
                 }
                 }
            );

            /*
             * if no output-filename given,
             * outfilename stays NULL. this let open_output
             * open stdout as output-file
             */
            if (arg_outfname)
            {
                /* check, if last char of outputfilename is a
                 * directory separator; if so, use the filename
                 * as destination directory
                 */
                if (arg_outfname)
                {
                    UBYTE lastch = 0;

#ifdef AMIGA
                    /* treat `TO ""' and `TO=""' the same for AmigaOS */
                    if (!strcmp(arg_outfname, "\"\""))
                    {
                        arg_outfname = "";
                        D(fprintf(stderr,
                         DHSC "AMIGA: use current dir, strange version\n"));
                    }
#endif

                    /* get last char of outfname to determine
                     * if it's a directory
                     */
                    if (strlen(arg_outfname))
                    {
                        lastch = arg_outfname[strlen(arg_outfname) - 1];
                    }

#ifdef AMIGA
                    /* for Amiga, execpt empty string for current dir */
                    if (!lastch)
                    {
                        lastch = (PATH_SEPARATOR[0]);
                        D(fprintf(stderr, DHSC "AMIGA: use current dir\n"));
                    }
#endif

                    if (strchr(PATH_SEPARATOR, lastch))
                    {
                        /* use outfilename as destdir */
                        set_estr(destdir, arg_outfname);
                        arg_outfname = NULL;
                        D(fprintf(stderr, DHSC "output: use `%s' as destdir\n",
                                  estr2str(destdir)));
                    }
                    else if (arg_inpfname)
                    {
                        /* output-filename already specified */
                        /* separate it to destdir + reldir + name */
                        EXPSTR *kack_destdir = init_estr(0);
                        EXPSTR *kack_reldir = init_estr(0);
                        STRPTR inp_reldir = estr2str(rel_destdir);
                        STRPTR out_reldir = NULL;
                        STRPTR ou2_reldir = NULL;

                        get_fname(kack_name, arg_outfname);
                        get_fpath(kack_destdir, arg_outfname);

                        /* check corresponding dirs for
                         * consistency: check if last strlen(rel_destdir)
                         * chars are equal */
                        out_reldir = estr2str(kack_destdir);
                        ou2_reldir = out_reldir;
                        out_reldir = out_reldir
                            + (strlen(out_reldir) - strlen(inp_reldir));

                        if (out_reldir[0])
                        {
                            /* search for next dir-sparator backwards */
                            /* (this ones only needed for a smart error message) */
                            while ((out_reldir != ou2_reldir)
                                 && (!strchr(PATH_SEPARATOR, out_reldir[0]))
                                )
                            {
                                out_reldir--;
                            }

                            if (out_reldir != ou2_reldir)
                            {
                                out_reldir++;
                            }
                        }
                        D(fprintf(stderr, DHSC "corr_inp: `%s'\n"
                                  DHSC "corr_out: `%s'\n",
                                  inp_reldir, out_reldir)
                            );

                        /* check if correspondig relative in/out-dirs
                         * are equal */
                        if (!fnamecmp(inp_reldir, out_reldir))
                        {
                            /* they match.. */
                            STRPTR tmp_name = NULL;     /* copy of kack_nam */

                            /* cut corresponding chars */
                            get_left_estr(kack_destdir, kack_destdir,
                                          estrlen(kack_destdir)
                                          - strlen(out_reldir));

                            set_estr(kack_reldir, inp_reldir);

                            D(fprintf(stderr, DHSC "kack_dst: `%s'\n"
                                      DHSC "kack_rel: `%s'\n"
                                      DHSC "kack_nam: `%s'\n",
                                      estr2str(kack_destdir),
                                      estr2str(kack_reldir),
                                      estr2str(kack_name))
                                );

                            /* just copy these values where they are
                             * expected to be */
                            estrcpy(destdir, kack_destdir);
                            estrcpy(rel_destdir, kack_reldir);

                            /* create output filename */
                            tmp_name = strclone(estr2str(kack_name));
                            estrcpy(kack_name, kack_destdir);
                            estrcat(kack_name, kack_reldir);
                            app_estr(kack_name, tmp_name);
                            ufreestr(tmp_name);

                            arg_outfname = estr2str(kack_name);
                        }
                        else
                        {
                            /* unmatched corresponding dirs */
                            fprintf(stderr, "unmatched corresponding relative directories:\n"
                                    "  input  `%s'\n  output `%s'\n",
                                    inp_reldir, out_reldir);
                            ok = FALSE;
                        }

                        /* free temp. vars */
                        del_estr(kack_reldir);
                        del_estr(kack_destdir);
                    }
                }
                if (arg_outfname)
                {
                    /* set outputfilename with value passed iwithin args */
                    outfilename = init_estr(32);
                    set_estr(outfilename, arg_outfname);
                    D(fprintf(stderr, DHSC "output: set to `%s'\n",
                              estr2str(outfilename)));
                }
                else
                {
                    /* no outfilename given */
                    /* ->outfilename = destdir + inpfilename + ".html" */

                    /* link destdir & input filename */
                    outfilename = init_estr(32);
                    link_fname(outfilename, estr2str(destdir),
                               arg_inpfname);
                    if (strcmp(arg_extension, "."))
                        set_fext(outfilename, arg_extension);
                    D(fprintf(stderr,
                              DHSC "output: concat destdir+inpfile+`.%s'\n"
                              DHSC "output: set to `%s'\n",
                              arg_extension, estr2str(outfilename)));
                }

                if (fnsux)
                {
                    /* no way to find out output filename */
                    status_error("unable to evaluate output filename\n");
                    arg_outfname = NULL;
                    ok = FALSE;
                }
            }
            else
            {
                D(fprintf(stderr, DHSC "output: use stdout\n"));
                use_stdout = TRUE;
            }

            if (!ok)
                set_return_code(RC_ERROR);
        }

        if (ok)
        {
            /* set server dir */
            if (arg_server_dir)
            {
                hsc_set_server_dir(hp, arg_server_dir);
            }

            /* set icon base */
            if (arg_iconbase)
            {
                hsc_set_iconbase(hp, arg_iconbase);
            }

            /* check, if stdout should be used as output */
            if (!use_stdout)
            {
                hsc_set_filename_document(hp, estr2str(outfilename));
            }
        }

        /* display argument error message */
        if (!ok)
        {
            /* NOTE: no strclone() is used on outfilename, if an
             * error already occured within set_args(). therefore,
             * you must not call ufreestr( outfilename ) */
            pargerr();
            arg_outfname = NULL;
            set_return_code(RC_ERROR);
        }
        else
        {
            EXPSTR *tmp_fname = init_estr(32);  /* filename only part */

            fileattr_str = init_estr(64);

            /* set HSC.DOCUMENT */
            if (outfilename)
            {
                get_fname(tmp_fname, estr2str(outfilename));
            }
            set_dest_attribs(hp, estr2str(rel_destdir),
                             estr2str(tmp_fname));

            /* set HSC.SOURCE */
            if (inpfilename)
                get_fname(tmp_fname, estr2str(inpfilename));
            else
                clr_estr(tmp_fname);
            set_source_attribs(hp, estr2str(rel_destdir),
                               estr2str(tmp_fname));

            D(
                 {
                 HSCMSG_ID i;

                 fprintf(stderr, "\n"
                         DHSC "input : `%s'\n", estr2str(inpfilename));
                 fprintf(stderr, DHSC "output: `%s'\n", get_outfilename());
                 fprintf(stderr, DHSC "destdr: `%s'\n", estr2str(destdir));
                 fprintf(stderr, DHSC "reldst: `%s'\n", estr2str(rel_destdir));
                 if (prjfilename)
                 fprintf(stderr, DHSC "projct: `%s'\n", prjfilename);
                 if (!use_stdout)
                 fprintf(stderr, DHSC "procss: `%s'\n", estr2str(outfilename));

                /* show classes to be ignored */
                 fprintf(stderr, DHSC "ignore class:");
                 if (hsc_get_msg_ignore_notes(hp))
                 {
                 fprintf(stderr, " notes");
                 }
                 fprintf(stderr, "\n");
                /* show messages to be ignored */
                 fprintf(stderr, DHSC "ignore:");
                 for (i = 0; i < MAX_MSGID; i++)
                 {
                 if (hsc_get_msg_ignore(hp, i) == ignore)
                 {
                 fprintf(stderr, " %lu", i);
                 }
                 }
                 fprintf(stderr, "\n");
                /* show messages to be enabled */
                 fprintf(stderr, DHSC "enable:");
                 for (i = 0; i < MAX_MSGID; i++)
                 {
                 if (hsc_get_msg_ignore(hp, i) == enable)
                 {
                 fprintf(stderr, " %lu", i);
                 }
                 }
                 fprintf(stderr, "\n");
                 }
            );

            del_estr(tmp_fname);
        }

        /*
         * set flags of hsc-process
         */
        if (ok)
        {
            hsc_set_chkid(hp, TRUE);
            hsc_set_chkuri(hp, TRUE);
            hsc_set_compact(hp, arg_compact);
            hsc_set_debug(hp, arg_debug);
            hsc_set_getsize(hp, arg_getsize);
            hsc_set_jens(hp, arg_jens);
            hsc_set_rplc_ent(hp, arg_rplc_ent);
            hsc_set_rplc_quote(hp, arg_rplc_quote);
            hsc_set_smart_ent(hp, arg_smart_ent);
            hsc_set_strip_badws(hp, arg_strip_badws);
            hsc_set_strip_cmt(hp, arg_strip_cmt);
            hsc_set_strip_ext(hp, arg_strip_ext);

            hsc_set_quote_mode(hp, arg_quotemode);
            hsc_set_strip_tags(hp, arg_striptags);

            /* set message limits; 0 means use the value set by
             * init_hscprc(), which means infinite */
            if (maximum_number_of_messages)
            {
                hsc_set_maximum_messages(hp, maximum_number_of_messages);
            }
            if (maximum_number_of_errors)
            {
                hsc_set_maximum_errors(hp, maximum_number_of_errors);
            }

            /* set directories */
            hsc_set_destdir(hp, estr2str(destdir));
            hsc_set_reldir(hp, estr2str(rel_destdir));
        }
        /* release mem used by args */
        free_args(hsc_args);
    }
    else
    {
        D(
        /* only for developer */
             fprintf(stderr, "ArgDef error: %lu\n", prep_error_num);
            );
    }

    del_estr(destdir);
    del_estr(rel_destdir);
    del_estr(kack_name);

#if 1
    return (ok);
#else
    return (FALSE);             /* for arg-debugging */
#endif
}

