/*
** global.c
**
** global vars & funs for hsc
**
** updated: 15-Oct-1995
** created:  8-Jul-1995
*/

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"

#define NOEXTERN_HSC_GLOBAL_H
#include "global.h"

/*
** global vars for user args
*/
STRPTR inpfilename = NULL;             /* name of input file (def: stdin) */
STRPTR outfilename = NULL;             /* name of output file (def: stdout) */
STRPTR errfilename = NULL;             /* name of error file (def: stderr) */
STRPTR destdir     = NULL;             /* destination dir */
STRPTR projdir     = NULL;             /* project main directory */

ULONG   max_error = 20;                /* abort after too many errors */
DLLIST *ignore  = NULL;                /* list of warning to ignore */
DLLIST *werror  = NULL;                /* warnings that should be handled as errors */
DLLIST *incfile = NULL;                /* list of files that should be
                                       /*   included before main file */
BOOL chkuri    = FALSE;                /* check existence of local URIs */
BOOL debug     = FALSE;                /* flag: debugging output */
BOOL insanch   = FALSE;                /* flag: insert text of anchor */
BOOL need_help = FALSE;                /* flag: display help text */
BOOL pipe_in   = FALSE;                /* flag: use stdin as input file */
BOOL rplc_ent  = FALSE;                /* replace entities */
BOOL smart_ent = FALSE;                /* replace special entities (&<>") */
BOOL stripcmt  = FALSE;                /* flag: strip SGML comments */
BOOL stripuri  = FALSE;                /* flag: strip external URIs */
BOOL statusmsg = FALSE;                /* flag: status message (line num) */
BOOL verbose   = FALSE;                /* flag: verbose output */

/*
** global vars
*/
FILE *outfile = NULL;                  /* file var for output */
time_t now;                            /* used to store current time */
                                       /*     (initialised in config.c) */

STRPTR destfname   = NULL;             /* filename part of outfilename */
STRPTR rel_destdir = NULL;             /* dir part of input filename */

EXPSTR *tmpstr   = NULL;               /* temp. string used by several fncs */
                                       /* parse.c   - parse_text() */
EXPSTR *IF_stack = NULL;               /* IF-stack pointer */
BOOL    suppress_output = TRUE;        /* flag checked by outch() */
BOOL    docbase_set = FALSE;           /* TRUE, if <BASE ..> occured */

/*
**
** global funs
**
*/

/*
** fprintf_spc
**
** printf num blanks to file
**
** params: file...destination file
**         num....num. of blanks
**
** result: value <0 if error
*/

int fprintf_spc( FILE *file, size_t num )
{
    int result = 0;

    while ( (num--) > 0 )              /* loop.. */
        result = fputc( ' ', file );   /*   write single blank to file */

    return (result);
}

