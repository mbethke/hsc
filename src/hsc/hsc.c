/*
 * hsc - HTML sucks completely
 *
 * Another stupid HTML-preprocessor
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
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
 *-------------------------------------------------------------------
 *
 * Author : Thomas Aglassinger (Tommy-Saftwörx)
 * Email  : agi@giga.or.at, agi@sbox.tu-graz.ac.at
 * Address: Lissagasse 12/II/9
 *          8020 Graz
 *          AUSTRIA
 *
 *-------------------------------------------------------------------
 *
 * hsc/hsc.c
 *
 * updated:  9-Sep-1996
 * created:  1-Jul-1995
 */

#include "hsc/global.h"
#include "hscprj/project.h"

/*
 * ugly includes
 */
#include "ugly/uargs.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"

/*
 * local includes
 */
#include "hsc/args.h"
#include "hsc/callback.h"
#include "hsc/output.h"
#include "hsc/status.h"

#if !(defined VERSTAG)
#include "hsc/hsc_rev.h"
#endif

/* import AmigaOS version string from "hsc/hsc_rev.h" */
#ifdef AMIGA
static const char AmigaOS_version[] = VERSTAG;
#endif

/* hsc-process structure that is used during
 * the wohle conversion */
static HSCPRC *hp = NULL;

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
 *
 * main function
 *
 */
int main(int argc, char *argv[])
{
    /* set program information */
    set_prginfo("hsc", "Tommy-Saftwörx", VERSION, REVISION, BETA,
                "html sucks completely",
                "Freeware, type `hsc LICENSE' for details.");

#if DEBUG
    /* display a memory tracking report
     * at end of execution */
    atexit(atexit_uglymemory);
#endif

    /* install nomem-handler; this one displays a message
     * and aborts the program */
    ugly_nomem_handler = hsc_nomem_handler;

    /* use cleanup() as additional exit func
     * (even called if out-of-memory) */
    if (!atexit(cleanup))
    {
        /*
         * main procedure
         */
        hp = new_hscprc();      /* alloc new hsc-process */
        if (hp
            && init_global()    /* init global vars */
            && args_ok(hp, argc, argv)  /* process user args */
            )
        {
            STRPTR inpfname = NULL;     /* input-filename */

            /* display programm-info if requested */
            if (disp_status_version)
                fprintf_prginfo(stderr);

            if (init_msgfile(hp, msgfilename)   /* open message file */
                && init_output(hp))     /* open output file */
            {
                /* init return code; later modified by message() */
                return_code = RC_OK;

                /* evaluate input-filename; use NULL for stdin */
                inpfname = estr2str(inpfilename);
                if (!inpfname[0])
                    inpfname = NULL;

                /* init process & read preferences */
                if (init_callback(hp)   /* assign callbacks */
                    && hsc_init_hscprc(hp, prefsfilename)       /* init hsc-process */
                    && hsc_init_project(hp, prjfilename)        /* read project */
                    && user_defines_ok(hp)      /* process user defines */
                    && include_ok(hp)   /* read include files (macros) */
                    && hsc_include_file(hp, inpfname,
                                        IH_PARSE_END | IH_IS_SOURCE
                                        | IH_UPDATE_PRJ)        /* read main file */
                    )
                {
                    if (write_output(hp))       /* write output file */
                        hsc_project_write_file(hp->project, prjfilename);
                }
            }
        }
    }
    else
    {
        status_error("atexit() failed ");
    }

    return (return_code);
}

