/*
** global.c
**
** global vars & funs for hsc
**
** updated:  7-Aug-1995
** created:  8-Jul-1995
*/

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/string.h"
#include "ugly/memory.h"

#define NOEXTERN_HSC_GLOBAL_H
#include "global.h"

/*
** global vars for user args
*/
STRPTR inpfilename = NULL;             /* name of input file (def: stdin) */
STRPTR outfilename = NULL;             /* name of output file (def: stdout) */
STRPTR errfilename = NULL;             /* name of error file (def: stderr) */
STRPTR destdir     = "";               /* destination dir */

ULONG max_error = 20;                  /* abort after too many errors */

BOOL absurl    = FALSE;                /* work with absolute URLs */
BOOL chkurl    = FALSE;                /* check existence of local URLs */
BOOL debug     = FALSE;                /* flag: debugging output */
BOOL insanch   = FALSE;                /* flag: insert text of anchor */
BOOL need_help = FALSE;                /* flag: display help text */
BOOL pipe_in   = FALSE;                /* flag: use stdin as input file */
BOOL stripurl  = FALSE;                /* flag: strip external urls */
BOOL statusmsg = FALSE;                /* flag: status message (line num) */
BOOL verbose   = FALSE;                /* flag: verbose output */

/*
** global vars
*/
FILE *outfile = NULL;                  /* file var for output */

DLLIST *deftag = NULL;                 /* list for defined tags */
DLLIST *defent = NULL;                 /* list for defined entites */
DLLIST *cltags = NULL;                 /* history for closing tags */
DLLIST *macros = NULL;                 /* list for macros */

time_t now;                            /* used to store current time */
                                       /*     (initialised in config.c) */

STRPTR destfname   = NULL;             /* filename part of outfilename */
STRPTR rel_destdir = NULL;             /* dir part of input filename */

/* nesting vars */
ULONG anchor_nesting = 0;
ULONG body_nesting   = 0;
ULONG head_nesting   = 0;
ULONG list_nesting   = 0;
ULONG title_nesting  = 0;

char this_tag[MAX_TAGLEN];             /* id of tag currently processed */

STRPTR last_anchor = NULL;             /* stores prev URL of last anchor; */
                                       /* used within tag_a.c, but must be */
                                       /* released within cleanup.c */
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

/*
** new_hsctag
**
** alloc & init a new hsctag
*/
HSCTAG *new_hsctag( STRPTR newid )
{

    HSCTAG *newtag = (HSCTAG*) malloc( sizeof(HSCTAG) );

    if (newtag) {

        /* init new tag item */
        newtag->name     = upstr( strclone(newid) );        /* set id */
        newtag->option   = 0;
        newtag->vers     = 0;
        newtag->o_handle = NULL;       /* no handle functions */
        newtag->c_handle = NULL;
        newtag->occured  = FALSE;
    }

    return (newtag);

}


/*
** new_hscent
**
** alloc & init a new hscentity
*/
HSCENT *new_hscent( STRPTR newid )
{

    HSCENT *newent = (HSCENT*) malloc( sizeof(HSCENT) );

    if (newent) {

        /* init new entity item */
        newent->name    = strclone(newid);       /* set id */
        newent->source  = NULL;
    }

    return (newent);

}

/*
** new_hscmac
**
** alloc & init a new hsc-macro
*/
HSCMAC *new_hscmac( STRPTR newname, STRPTR newtext )
{

    HSCMAC *newmac = (HSCMAC*) malloc( sizeof(HSCMAC) );

    if (newmac) {

        /* init new macity item */
        newmac->name = strclone(newname);
        newmac->text = strclone(newtext);
    }

    return (newmac);

}


