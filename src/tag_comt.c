/*
** tag_comt.c
**
** tag handles for <!xx>, <*> (comment)
**
** updated: 30-Jul-1995
** created: 29-Jul-1995
*/

#include <stdio.h>
#include <time.h>

#include <ugly/types.h>
#include <ugly/dllist.h>
#include <ugly/infile.h>
#include <ugly/string.h>

#include "global.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "error.h"

/*
**
** global funs
**
*/

/*
** handle_comment
**
**
*/
BOOL handle_comment( INFILE *inpf )
{
    /*
    ** TODO: - correct abort
    **       - copy chars
    */
    skip_until_gt( inpf );

    return (TRUE);
}

/*
** handle_hsc_comment
**
**
*/
BOOL handle_hsc_comment( INFILE *inpf )
{
    int ch;                            /* current char */
    int prev_ch = 'x';                /* prev char read (dummy init) */
    BOOL abort = FALSE;

    do {

        ch = infgetc( inpf );
        abort = ( infeof(inpf) || ((prev_ch=='*') && (ch=='>')) );
        prev_ch = ch;

    } while ( !abort );

    if ( infeof(inpf) && (ch!='>') )
        err_eof(inpf);

    return (TRUE);
}

