/*
**
** skip.c
**
** functions for skipping several things
**
** updated: 18-Oct-1995
** created:  8-Oct-1995
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/string.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"

#include "vars.h"

#define NOEXTERN_SKIP_H
#include "skip.h"

/*
** skip_lf
**
** ignore '\n'
**
** params: inpf...input file to read char from
** result: -1 if successful, 0 if wrong char found
*/
BOOL skip_lf( INFILE *inpf )
{
    int nc = infgetc( inpf );

    if ( nc != '\n' )
        inungetc( nc, inpf );

    return ( (BOOL) (nc==EOF) );
}

/*
** copy_until_gt
*/
BOOL copy_until_gt( INFILE *inpf )
{
    BOOL end_gt = FALSE; /* TRUE after ">" */
    BOOL quote  = FALSE; /* TRUE, if inside "'" */
    BOOL dquote = FALSE; /* TRUE, if inside "\"" */
    STRPTR nw   = NULL;  /* next word read from input */

    do  {

        nw = infgetw( inpf );
        if ( !nw )
            err_eof( inpf, "\">\" expected" );
        else if ( !strcmp( nw, ">" ) ) {
            end_gt = TRUE;
        } else if ( !strcmp( nw, "\"" ) )
            dquote = !dquote;
        else if ( !strcmp( nw, "'" ) )
            quote = !quote;
        else if ( !strcmp( nw, "\n" ) ) {

            /* TODO: check for LF inside quote */
        }

        if ( nw )
            outstr( nw );

    } while ( nw && !end_gt );

    return (BOOL)(fatal_error);
}


/*
** skip_until_gt
*/
BOOL skip_until_gt( INFILE *inpf )
{
    BOOL old_suppress = suppress_output;

    /* copy until ">", but with suppress output */
    suppress_output = TRUE;
    copy_until_gt( inpf );
    suppress_output = old_suppress;

    return (BOOL)(fatal_error);
}

BOOL eot_reached( INFILE *inpf, BYTE *state )
{
    STRPTR nw = infgetw( inpf );

    if ( nw ) {

        switch ( *state ) {

            case TGST_TAG:
                if ( !strcmp( nw, "\"" ) )
                    *state = TGST_DQUOTE;
                else if ( !strcmp( nw, "'" ) )
                    *state = TGST_QUOTE;
                else if ( !strcmp( nw, "<" ) )
                    *state = TGST_REF;
                else if ( !strcmp( nw, ">" ) )
                    *state = TGST_END;
                break;

            case TGST_REF   :
            case TGST_QUOTE :
            case TGST_DQUOTE:

                if ( strcmp( nw, "\n" ) ) {

                    switch ( *state ) {

                    case TGST_REF:
                        if ( !strcmp( nw, ">" ) )
                            *state = TGST_TAG;
                        break;

                    case TGST_QUOTE:
                        if ( !strcmp( nw, "'" ) )
                            *state = TGST_TAG;
                        break;

                    case TGST_DQUOTE:
                        if ( !strcmp( nw, "\"" ) )
                            *state = TGST_TAG;
                        break;
                    }

                } else {

                    /* unexpected end of line */
                    err_eol( inpf );
                    *state = TGST_TAG; /* go on reading inside tag */

                }


                break;

        }
    } else {

        err_eof( inpf, "\">\" expected" );
        *state = TGST_ERR;

    }

    return( (BOOL) ( (*state == TGST_END) || (*state == TGST_ERR ) ) );
}

/*
** skip_until_eot_args
**
** skip until end of tag reached,
** with user definable status vars
**
** params: inpf.....input file
**         quote....status for quote (TRUE=inside quote)
**         dquote...status for double quote
**         argattr..status for quote
** result: TRUE, if no fatal error
** errors: return FALSE
*/
BOOL skip_until_eot_state( INFILE *inpf, BYTE *state )
{
    while( !eot_reached( inpf, state ) )
         /* do nufin */;

    return( (BOOL) !fatal_error );
}


/*
** skip_until_eot
**
** skip until end of tag reached
**
** params: inpf..input file
** result: TRUE, if no fatal error
** errors: return FALSE
*/
BOOL skip_until_eot( INFILE *inpf )
{
    BYTE state = TGST_TAG;

    return( skip_until_eot_state( inpf, &state ) );
}

/*
** skip_hsc_comment
**
** skip text until '*>' occures;
** nested commets are supported
**
*/
BOOL skip_hsc_comment( INFILE *inpf, BOOL copy )
{
    int ch;                            /* current char */
    int prev_ch = 'x';                 /* prev char read (dummy init) */
    BOOL abort = FALSE;
    ULONG nesting = 1;

    do {

        STRPTR nw = infgetw( inpf );

        if ( nw ) {

            /* tokenize next word */
            if ( !strcmp( nw, "*" ) )
                ch = '*';
            else if ( !strcmp( nw, ">" ) )
                ch = '>';
            else
                ch = 'x'; /* dummy value */

            /* check for token sequences */
            if ( (prev_ch=='<') && (ch=='*') ) nesting++;
            if ( (prev_ch=='*') && (ch=='>') ) nesting--;

            /* set abort value, if "*>" found and
            ** no other comments have been opened
            */
            abort = ( (nesting==0) );

            /* remember prev. token */
            prev_ch = ch;

            /* write out word, if requested */
            if ( copy ) {

                outstr( infgetcws( inpf ) );
                outstr( infgetcw( inpf ) );

            }

        } else {

            err_eof( inpf, "missing end of hsc-comment (\"*>\")" );
            abort = TRUE;

        }

    } while ( !abort );

#if 0
    if ( infeof(inpf) && nesting )
        err_eof(inpf);
#endif

    return ( (BOOL) !fatal_error);
}

