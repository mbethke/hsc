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
 * hsc - html sucks completely
 *
 * Another stupid html-preprocessor
 *
 *-------------------------------------------------------------------
 *
 * Author : Thomas Aglassinger (Tommy-Saftwörx)
 * Email  : agi@giga.or.at, agi@sbox.tu-graz.ac.at
 *
 *-------------------------------------------------------------------
 *
 * hsc/hsc.c
 *
 * updated: 17-Dec-1997
 * created:  1-Jul-1995
 */


#include "hsc/hsc_rev.h"

#include "hsc/global.h"
#include "ugly/prginfo.h"
#include "hscprj/project.h"
#ifndef BETA
#define BETA 0
#endif

/*
 * ugly includes
 */
#include "ugly/uargs.h"
#include "ugly/returncd.h"

/*
 * local includes
 */
#include "hsc/args.h"
#include "hsc/callback.h"
#include "hsc/output.h"
#include "hsc/status.h"

/* import AmigaOS version string from "hsc/hsc_rev.h" */
#ifdef AMIGA
static const char AmigaOS_version[] = VERSTAG;
#endif

/* hsc-process structure that is used during
 * the wohle conversion */
static HSCPRC *hp = NULL;

/*
 * attempt_atexit
 *
 * try to add another atexit()-function; if it fails,
 * abort with error message
 */
static BOOL attempt_atexit(VOID (*func) (VOID))
{
#define ERRMSG_LEN 300
    BOOL ok = FALSE;

    errno = 0;
    if (atexit(func))
    {
        /* atexit() failed; display error message */
        char errmsg[ERRMSG_LEN];
        strncpy(errmsg, "atexit() failed: ", ERRMSG_LEN);
        strncat(errmsg, strerror(errno), ERRMSG_LEN);
        status_error(errmsg);
    }
    else
    {
        ok = TRUE;
    }

    return ok;
}

/*
 * includes_ok
 *
 * process include-files passed via command line
 */
static BOOL include_ok(HSCPRC * hp)
{
    BOOL ok = TRUE;

    if (incfile)
    {
        DLNODE *nd = dll_first(incfile);

        while (nd && ok)
        {
            STRPTR filename = (STRPTR) dln_data(nd);
            ok = hsc_include_file(hp, filename, IH_IS_INCLUDE);
            nd = dln_next(nd);
        }
    }
    return (ok);
}

/*
 * cleanup: free all resources
 * (called in any case)
 */
static VOID cleanup(VOID)
{
#if DEBUG
    /* just because I'm curious how long cleanup lasts */
    /* NOTE: obviously, it last very long */
    fputs("(cleanup)\r", stderr);
    fflush(stderr);
#endif

    del_hscprc(hp);
    cleanup_global();
    cleanup_output();
    cleanup_msgfile();
    cleanup_hsc_args();

#if DEBUG
    fputs("         \r", stderr);
    fflush(stderr);
#endif

}

/*
 * callback to display "project-file corrupt"-message
 */
static VOID msg_corrupt_pf(HSCPRJ * project, STRPTR reason)
{
    EXPSTR *msg = init_estr(32);
    set_estr(msg, infget_fname(project->inpf));
    app_estr(msg, ": project file corrupt");
    if (reason)
    {
        app_estr(msg, " (");
        app_estr(msg, reason);
        app_estr(msg, ")");
    }
    status_error(estr2str(msg));
    del_estr(msg);
}


/*
 * hsc_init_project
 *
 * read project-file
 */
static BOOL hsc_init_project(HSCPRC * hp, STRPTR project_fname)
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
            fprintf(stderr, "%s: error reading project file: %s\n",
                    project_fname, strerror(errno));
            ok = FALSE;
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
 * hsc_main()
 */
int hsc_main(HSCPRC ** hpVar, int argc, char *argv[])
{
    BOOL init_hp = FALSE;

    if (!(*hpVar))
    {
        hp = new_hscprc();      /* alloc new hsc-process */
        init_hp = TRUE;
        *hpVar = hp;
    }
    else
    {
        hp = *hpVar;
        reset_hscprc(hp);
    }

    if (hp
        && init_global()        /* init global vars */
        && args_ok(hp, argc, argv)      /* process user args */
        )
    {
        STRPTR inpfname = NULL; /* input-filename */
        BOOL ok = TRUE;

        /* display programm-info if requested */
        if (disp_status_version)
        {
            fprintf_prginfo(stderr);
        }

        if (init_msgfile(hp, msgfilename)       /* open message file */
            && init_output(hp)) /* open output file */
        {
            /* init return code; later modified by message() */
            return_code = RC_OK;

            /* evaluate input-filename; use NULL for stdin */
            inpfname = estr2str(inpfilename);
            if (!inpfname[0])
                inpfname = NULL;

            /*
             * init process, read preferences and project (for new process)
             */
            if (ok && init_hp)
            {
                ok = (init_callback(hp)         /* assign callbacks */
                      && hsc_init_hscprc(hp, prefsfilename)     /* init hsc-process */
                      && hsc_init_project(hp, prjfilename));    /* read project */
            }

            /*
             * process user defines and files, write output
             */
            if (ok
                && user_defines_ok(hp)  /* process user defines */
                && include_ok(hp)       /* read include files (macros) */
                && hsc_include_file(hp, inpfname,
                                    IH_PARSE_END | IH_IS_SOURCE
                                    | IH_UPDATE_PRJ)    /* read main file */
                )
            {
                if (write_output(hp))   /* write output file */
                {
                    /* write project file */
                    hsc_project_write_data(hp->project, prjfilename, FALSE);
                }
            }
        }
    }

    return (return_code);
}

/*
 *
 * main function
 *
 */
int main(int argc, char *argv[])
{
    int main_return_code = RC_FAIL;
    BOOL atexit_ok = TRUE;

    /* set program information */
    set_prginfo("hsc", UGLY_AUTHOR, VERSION, REVISION, BETA,
                "html sucks completely",
                "Freeware, type `hsc LICENSE' for details.");

#if DEBUG
    /* display a memory tracking report
     * at end of execution */
    atexit_ok = attempt_atexit(atexit_uglymemory);
#endif

    /* install nomem-handler; this one displays an error message
     * and aborts the program */
    ugly_nomem_handler = hsc_nomem_handler;

    /* use cleanup() as additional exit func
     * (even called in case of out-of-memory) */
    if (atexit_ok)
    {
        atexit_ok = attempt_atexit(cleanup);
    }

    /* if still all right, let's dance */
    if (atexit_ok)
    {
        HSCPRC *hpVar = NULL;
        main_return_code = hsc_main(&hpVar, argc, argv);
    }

#ifdef SINGLE_CLIENT_SERVER
    /*
     * The following code is needed for project data
     * exchage with the hsc-single-client-project-server.
     *
     * Concepts and implementation by Andreas Gassner
     */
    strcmp("a", "b");
#endif

    return (main_return_code);
}

