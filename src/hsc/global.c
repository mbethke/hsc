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
 * hsc/global.c
 *
 * global vars & funs for hsc
 *
 * updated: 16-May-1997
 * created:  8-Jul-1995
 */

#include <time.h>

#include "hsclib/hsclib.h"
#include "ugly/returncd.h"

#define NOEXTERN_HSC_GLOBAL_H
#include "hsc/global.h"

/*
 * global vars for user args
 */
EXPSTR *inpfilename = NULL;     /* name of input file (def: stdin) */
EXPSTR *outfilename = NULL;     /* name of output file (def: stdout) */

STRPTR msgfilename = NULL;      /* name of message file (def: stderr) */
STRPTR prjfilename = NULL;      /* name for project-file (def: none) */
STRPTR prefsfilename = NULL;    /* name for prefs-file (default: search) */

ULONG max_error = 20;           /* abort after too many errors */
DLLIST *define_list = NULL;     /* defines from user-args */
DLLIST *incfile = NULL;         /* list of files that should be */
                                       /*   included before main file */
int return_code = RC_FAIL;      /* exit code of program */

BOOL msg_ansi = FALSE;          /* use ANIS-sequences in messages */
STRPTR msg_format = NULL;       /* message format */
EXPSTR *msgbuf = NULL;          /* buffer for message */

#if defined MSDOS
STRARR msdosbuf[4097];          /* misc. buffer */
#endif

/*
 * init_global
 *
 * init global data
 */
BOOL init_global(VOID)
{
    BOOL ok = TRUE;

    return_code = RC_OK;

    /* init random generator */
    srand((int) time(NULL));

    /* init some string */
    inpfilename = init_estr(32);
    msgbuf = init_estr(64);

    ok = (inpfilename && msgbuf);

#if (defined MSDOS) /* HSC_PUT */
#define MEM_SIZE (15*1024*1024)
    {
        /* alloc 15MB ram, fill them twice
         * and never release them;
         * espececially funny for Dos-externders */
        STRPTR mem = umalloc(MEM_SIZE);
        memset(mem, 0xaBadCafe, MEM_SIZE);
        memset(mem, 0xDeadF00d, MEM_SIZE);
    }
#endif

    return (ok);
}

/*
 * cleanup_global
 *
 * cleanup global data
 */
VOID cleanup_global(VOID)
{
    del_estr(inpfilename);
    del_estr(outfilename);
    del_dllist(define_list);
    del_dllist(incfile);
    del_estr(msgbuf);
}

/*
 * get_outfilename
 *
 * return output filename or `<stdout>'
 */
STRPTR get_outfilename(VOID)
{
    if (outfilename)
        return (estr2str(outfilename));
    else
        return (STDOUT_NAME);
}

