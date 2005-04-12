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
 * hsctools/hscdepp.c
 *
 * hsc dependency procreator
 *
 * updated: 28-Mar-1997
 * created:  8-Jul-1996
 */

/* ANSI includes */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/* include revision data */
#include "hsctools/depp_rev.h"

/* ugly includes */
#include "ugly/ustring.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/infile.h"
#include "ugly/uargs.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"

/* hsclib includes */
#include "hscprj/document.h"
#include "hscprj/project.h"
#include "hscprj/license.h"

#ifdef AMIGA
/* AmigaOS version string
 * (imported from "hscdepp_rev.h")
 */
static const STRPTR AmigaOS_version = VERSTAG;
#endif

/* prefix for messages */
#define HD ""
#define DHD "*hscdep* "
#define SHIT "*** "             /* prefix for total failure */

/* debugging define */
#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#endif

#ifdef D
#undef D
#endif

#if DEBUG
#define D(x) if (debug) {x;}
#else
#define D(x) {/*nufin*/}
#endif

/* step-sizes for input-file/depency-string */
#define CHUNKSIZE_INPUTFILE (64*1024)
#define CHUNKSIZE_DEPENDSTR (16*1024)

/* default parameters */
#define DEFAULT_PROJECT "hsc.project"   /* project-filename */
#define DEFAULT_NAMEALL "all_hsc"       /* "all"-rule */

/* size of buffer for fgets() */
#define MAXBUFSIZE 1024

#define STR_DEPENDS_PRECEDE \
    "# --- DO NOT MODIFY THIS LINE -- hsc-dependencies precede ---\n"
#define STR_DEPENDS_FOLLOW \
    "# --- DO NOT MODIFY THIS LINE -- hsc-dependencies follow ---\n"

/*
 * global vars
 */
static int return_code = RC_FAIL;       /* exit code of program */

static STRPTR makefile = NULL;
static STRPTR prjfile = NULL;
static STRPTR nameall = NULL;
static BOOL verbose = FALSE;
static BOOL nobackup = FALSE;
static BOOL notaglines = FALSE;
static BOOL debug = FALSE;

static EXPSTR *lines_precede = NULL;
static EXPSTR *lines_follow = NULL;
static EXPSTR *lines_depend = NULL;

static HSCPRJ *project = NULL;

/*
 * cleanup: free all resources
 * (called in any case)
 */
static void cleanup(void)
{
    D(fprintf(stderr, "(cleanup)\n"));
    del_project(project);
    del_estr(lines_precede);
    del_estr(lines_follow);
    del_estr(lines_depend);
    D(fprintf(stderr, "         "));
}

static void set_return_code(int new_code)
{
    if (new_code > return_code)
        return_code = new_code;
}

/*
 * hsc_nomem_handler
 *
 * called from ugly/umalloc, if malloc() did return NULL
 */
static BOOL hscdepp_nomem_handler(size_t size)
{
    fputs(SHIT "out of memory\n", stderr);

    return_code = RC_FAIL;

    exit(return_code);

    return (FALSE);             /* immediatly abort */
}

void msg_corrupt_pf(HSCPRJ * hp, STRPTR reason)
{
    fprintf(stderr, "project-file corrupt: %s\n", reason);
}

/*
 * args_ok
 *
 * prepare args, check & parse user args, display error and
 * help message if neccessary
 *
 * result: TRUE, if all args ok and no request for HELP or
 *         LICENSE has been detected
 */
static BOOL args_ok(int argc, char *argv[])
{
    struct arglist *hscdepp_args;       /* argument structure */
    BOOL arg_help = FALSE;
    BOOL arg_license = FALSE;
    BOOL ok = FALSE;

    /* create arg-table */
    hscdepp_args = prepare_args
        ("HSCDEPP_ARGS",
         "FILE/T", &makefile, "makefile to update",
         "PRJFILE/T/K", &prjfile, "project file",
         "NAMEALL/T/K", &nameall, "name for `all_hsc' rule",
         "VERBOSE/S", &verbose, "verbose output",
         "NOBACKUP/S", &nobackup, "do not backup makefile",
         "NOTAGLINES/S", &notaglines, "do not write taglines",
         "-DEBUG/S", &debug, "enable debugging output",
         "HELP=?=-h=--help/S", &arg_help, "display this text",
         "LICENSE/S", &arg_license, "display license",
         NULL);

    ok = (hscdepp_args != NULL);

    /* set & test args */
    if (ok)
    {
        ok = set_args(argc, argv, hscdepp_args);

        /* display argument error message */
        if (!ok)
        {
            pargerr();
            set_return_code(RC_ERROR);
        }
        else if (arg_help || arg_license)
        {
            /*
             * display help or license text
             */
            fprintf_prginfo(stderr);
            if (arg_help)
                fprintf_arghelp(stderr, hscdepp_args);
            else
                show_license();
            set_return_code(RC_WARN);
            ok = FALSE;
        }
        else
        {
            /* auto-enable verbose in debug-mode */
            if (debug)
                verbose = TRUE;

            /* display copyright in verbose-mode */
            if (verbose)
                fprintf_prginfo(stderr);

            /* set default-parameters if neccessary */
            if (!prjfile)
            {
                prjfile = DEFAULT_PROJECT;
                if (verbose)
                    fprintf(stderr, HD "%s: using default project-file\n",
                            prjfile);
            }
            if (!nameall)
            {
                nameall = DEFAULT_NAMEALL;

            }

            /* debugging control output */
            D(
                 if (makefile)
                 {
                 fprintf(stderr, DHD "makefile=`%s'\n", makefile);
                 fprintf(stderr, DHD "makefile=DEFAULT\n");
                 fprintf(stderr, DHD "prjfile =`%s'\n", prjfile);
                 fprintf(stderr, DHD "nameall =`%s'\n", nameall);
                 }
            );
        }

        /* release mem used by args */
        free_args(hscdepp_args);
    }
    else
    {
        /* only for developer */
        D(fprintf(stderr, SHIT "ArgDef error: %lu\n", prep_error_num));
    }

#if 1
    return (ok);
#else
    return (FALSE);             /* for arg-debugging */
#endif
}

/*
 * read_makefile
 *
 * scan for fitting makefile, read all it's data into a list
 * of strings, exept those which are between taglines
 */
static BOOL read_makefile(void)
{
    STRPTR scanfile[] =
    {"GNUmakefile", "Makefile", "makefile", NULL};
    FILE *file = NULL;
    BOOL ok = FALSE;

    lines_precede = init_estr(1024);
    lines_follow = init_estr(1024);

    /*
     * open makefile
     */
    errno = 0;
    if (!makefile)
    {
        /* scan for makefile */
        int i = 0;

        D(fprintf(stderr, DHD "scanning makefile\n"));
        makefile = scanfile[0];
        while (!file && makefile)
        {
            D(fprintf(stderr, DHD "  try `%s'\n", makefile));
            file = fopen(makefile, "r");
            if (!file)
            {
                i += 1;
                makefile = scanfile[i];
            }
        }

    }
    else
    {
        /* use makefile specified by user */
        D(fprintf(stderr, DHD "makefile `%s' specified by user\n", makefile));
        file = fopen(makefile, "r");
    }

    if (!file)
    {
        if (!makefile)
            makefile = "Makefile";
        fprintf(stderr, HD "%s: creating new makefile\n", makefile);
        ok = TRUE;
    }
    else
    {
        static STRARR buf[MAXBUFSIZE];  /* buffer for fgets() */
        BOOL found = FALSE;     /* flag: tag-line found */
        STRPTR line = NULL;     /* current line read */

        /*
         * read Makefile
         */

        /* reset error variable */
        errno = 0;

        /*
         * read preceding lines
         */
        do
        {
            line = fgets(buf, MAXBUFSIZE, file);
            if (line) {
                if (!strcmp(line, STR_DEPENDS_FOLLOW))
                    found = TRUE;
                else
                    app_estr(lines_precede, line);
            }
        }
        while (!found && !errno && line);

        if (errno)
        {
            fprintf(stderr, HD "error reading `%s': %s\n", makefile, strerror(errno));
            set_return_code(RC_ERROR);
        }
        else if (!line)
        {
            /* tag-line not found */
            if (verbose)
                fprintf(stderr, HD "%s: no starting tag-line; "
                        "appending dependencies\n", makefile);
        }
        else
        {
            /*
             * skip old dependencies
             */
            D(fprintf(stderr, DHD "starting tagline found\n"));

            found = FALSE;
            do
            {
                line = fgets(buf, MAXBUFSIZE, file);
                if (line)
                    if (!strcmp(line, STR_DEPENDS_PRECEDE))
                        found = TRUE;
            }
            while (!found && !errno && line);

            if (errno)
            {
                fprintf(stderr, HD "error reading `%s': %s\n", makefile, strerror(errno));
                set_return_code(RC_ERROR);
            }
            else if (!line)
            {
                /* tag-line not found */
                if (verbose)
                    fprintf(stderr, HD "%s: no ending tag-line; "
                            "appending dependencies\n", makefile);
            }
            else
            {
                /*
                 * read following lines
                 */
                D(fprintf(stderr, DHD "ending tagline found\n"));
                do
                {
                    line = fgets(buf, MAXBUFSIZE, file);
                    if (line)
                        app_estr(lines_follow, line);
                }
                while (!errno && line);

                if (errno)
                {
                    fprintf(stderr, HD "error reading `%s': %s\n", makefile, strerror(errno));
                    set_return_code(RC_ERROR);
                }
            }
        }

        if (!errno)
        {
            if (verbose)
                fprintf(stderr, HD "%s: makefile read\n", makefile);
            ok = TRUE;
        }
        /* close makefile */
        fclose(file);
    }

    return (ok);
}

/*
 * read_project
 *
 * read data from project file
 */
static BOOL read_project(void)
{
    BOOL ok = FALSE;
    INFILE *inpf = NULL;

    if (verbose)
    {
        fprintf(stderr, HD "%s: reading..\n", prjfile);
        fflush(stderr);
    }

    /* assign message-callback for corrupt project-file */
    project->CB_msg_corrupt_pf = msg_corrupt_pf;

    /* read project-file */
    errno = 0;
    inpf = infopen(prjfile, CHUNKSIZE_INPUTFILE);
    if (inpf)
    {
        if (hsc_project_read_data(project, inpf))
        {
            if (verbose)
                fprintf(stderr, HD "%s: project-file read\n", prjfile);
            ok = TRUE;
        }
        infclose(inpf);
    }

    if (!ok)
    {
        fprintf(stderr, HD "error reading `%s'", prjfile);
        if (errno)
            fprintf(stderr, HD ": %s", strerror(errno));
        fprintf(stderr, "\n");
    }

    return (ok);
}

/*
 * update_makefile
 *
 * create dependency-lines from project-info,
 */


/* append string to dependency-line */
static void depline_appstr(STRPTR s, ULONG * linelen)
{
#define LEADING_BLANKS "   "
    size_t slen = strlen(s);

    /* check if line would become too long after appending
     * the current word */
    if ((*linelen + slen) >= 75)
    {
        D(fprintf(stderr, DHD "break line after %lu chars to avoid %lu\n",
                  *linelen, *linelen + slen));

        app_estr(lines_depend, " \\\n");
        app_estr(lines_depend, LEADING_BLANKS);
        *linelen = strlen(LEADING_BLANKS);
    }

    app_estrch(lines_depend, ' ');
    app_estr(lines_depend, s);
    *linelen = *linelen + slen + 1;
}

/*
 * update_makefile - main function
 */
static BOOL update_makefile(void)
{
    BOOL ok = FALSE;
    ULONG linelen = 0;          /* length of current line */
    DLNODE *docnode = dll_first(project->documents);
    BOOL bak_ok = TRUE;

    lines_depend = init_estr(CHUNKSIZE_DEPENDSTR);

    /* append tagline */
    if (!notaglines)
    {
        app_estr(lines_depend, STR_DEPENDS_FOLLOW);
    }

    /* append some header info */
    if (!notaglines)
    {
#define MAXTIMEBUF 40
        time_t now = time(NULL);
        STRARR timebuf[MAXTIMEBUF];

        if (strftime(timebuf, MAXTIMEBUF, "%A %d-%b-%Y %H:%M:%S",
                     localtime(&now)))
        {
            app_estr(lines_depend, "\n# dependencies updated: ");
            app_estr(lines_depend, timebuf);
            app_estr(lines_depend, "\n\n");
        }
    }

    /*
     * append all-rule
     */
    app_estr(lines_depend, nameall);
    app_estr(lines_depend, " :");
    linelen = strlen(nameall) + 2;

    while (docnode)
    {
        HSCDOC *document = dln_data(docnode);
        depline_appstr(document->docname, &linelen);

        docnode = dln_next(docnode);
    }
    app_estr(lines_depend, "\n\n");

    /*
     * append document data
     */
    docnode = dll_first(project->documents);
    while (docnode)
    {
        HSCDOC *document = dln_data(docnode);
        DLNODE *incnode = dll_first(document->includes);

        D(fprintf(stderr, DHD "document `%s'\n", document->docname));

        app_estr(lines_depend, document->docname);
        app_estr(lines_depend, " :");

        linelen = strlen(document->docname) + 2;

        /* dependency for main source */
        depline_appstr(document->sourcename, &linelen);

        /* dependencies for includes */
        while (incnode)
        {
            HSCINC *include = dln_data(incnode);
            depline_appstr(include->name, &linelen);

            incnode = dln_next(incnode);
        }

        /* append linefeed after dependency-list */
        app_estr(lines_depend, "\n\n");

        docnode = dln_next(docnode);
    }

    /* append tagline */
    if (!notaglines)
        app_estr(lines_depend, STR_DEPENDS_PRECEDE);

    /*
     * create backup
     */
    if (!nobackup)
    {
        EXPSTR *makefile_bak = init_estr(32);   /* filename for backup */

        set_estr(makefile_bak, makefile);
        app_estr(makefile_bak, ".bak");

        /* remove old backup */
        remove(estr2str(makefile_bak));

        /* rename old makefile to backup
         *
         * NOTE: if this fails, this can also be because
         * there wasn't any earlier copy, therefor no
         * error-message is displayed */
        if (!rename(makefile, estr2str(makefile_bak)))
        {
            if (verbose)
                fprintf(stderr, HD "%s: backup created\n",
                        estr2str(makefile_bak));
        }

        del_estr(makefile_bak);
    }

    /*
     * write makefile
     */
    if (bak_ok)
    {
        FILE *outf = NULL;

        /* open output makefile */
        errno = 0;
        outf = fopen(makefile, "w");
        if (outf)
        {
            /* write output */
            errno = 0;
            fprintf(outf, "%s%s%s",
                    estr2str(lines_precede), estr2str(lines_depend),
                    estr2str(lines_follow));

            if (errno)
            {
                /* write error */
                fprintf(stderr, "error writing to `%s': %s\n",
                        makefile, strerror(errno));
            }
            else
                ok = TRUE;

            /* close output */
            fclose(outf);
        }
        else
        {
            /* error opening output */
            fprintf(stderr, "error opening `%s' for output: %s\n",
                    makefile, strerror(errno));
        }
    }

    return (ok);
}

/*
 *
 * main function
 *
 */
int main(int argc, char *argv[])
{
#ifndef BETA
#define BETA 0
#endif
    /* set program information */
    set_prginfo("hscdepp", UGLY_AUTHOR, VERSION, REVISION, BETA,
                "hsc dependency procreator",
                "Freeware, type `hscdepp LICENSE' for details.");

#if DEBUG
    /* display a memory tracking report */
    /* at end of execution */
    atexit(atexit_uglymemory);
#endif

    /* install nomem-handler */
    ugly_nomem_handler = hscdepp_nomem_handler;

    /* use cleanup() as additional exit func */
    if (!atexit(cleanup))
    {
        /*
         * main procedure
         */
        return_code = RC_OK;
        project = new_project();
        if (project
            && args_ok(argc, argv)
            && read_makefile()
            && read_project()
            && update_makefile()
        )
        {
            if (verbose)
            {
                fprintf(stderr, HD "%s: updated using `%s'\n",
                        makefile, prjfile);
            }
            return_code = RC_OK;
        }
    }
    else
    {
        fputs(SHIT "atexit() failed ", stderr);
    }
    return (return_code);
}
