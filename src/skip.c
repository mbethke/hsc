/*
** skip.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** functions for skipping several things
**
** updated: 27-Nov-1995
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

#if 0
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
#endif

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
/* TODO: skip references & expressions */
#if 0
                else if ( !strcmp( nw, "<" ) )
                    *state = TGST_REF;
#endif
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
BOOL skip_until_eot_state( INFILE *inpf, BYTE *state, EXPSTR *logstr )
{
    while( !eot_reached( inpf, state ) )
        if ( logstr ) {

            app_estr( logstr, infgetcws( inpf ) );
            app_estr( logstr, infgetcw( inpf ) );

        }

    /* append ">" */
    if ( logstr ) {

        app_estr( logstr, infgetcws( inpf ) );
        app_estr( logstr, infgetcw( inpf ) );

    }

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
BOOL skip_until_eot( INFILE *inpf, EXPSTR *logstr )
{
    BYTE state = TGST_TAG;

    return( skip_until_eot_state( inpf, &state, logstr ) );
}

/*
**-----------------
** skip comment
**-----------------
*/


/*
** eoc_reched
**
** check if end of an hsc-comment is reached
**
** params:
**   inpf...where to read next word from
**   state..state var; has to be initiales by
**          calling func with CMST_TEXT
**   nest...comment netsing counter; has to be
**          initiales by calling func with 0
** result: TRUE, if end of comment reached
*/
BOOL eoc_reached( INFILE *inpf, BYTE *state, LONG *nest )
{
    STRPTR nw = infgetw( inpf );

    if ( nw ) {

        switch ( *state ) {

            case CMST_TEXT:
                if ( !strcmp( nw, "*" ) )
                    *state = CMST_STAR;
                else if ( !strcmp( nw, "<" ) )
                    *state = CMST_TAG;
                break;

            case CMST_STAR:
                if ( !strcmp( nw, "*" ) )
                    *state = CMST_STAR;
                else if ( !strcmp( nw, "<" ) )
                    *state = CMST_TAG;
                else if ( !strcmp( nw, ">" ) )
                    if ( *nest ) {

                        (*nest)--;
                        *state = CMST_TEXT;

                    } else
                        *state = CMST_END;

                break;

            case CMST_TAG:
                if ( !strcmp( nw, "<" ) )
                    *state = CMST_TAG;
                else {

                    if ( !strcmp( nw, "*" ) )
                        (*nest)++;
                    *state = CMST_TEXT;

                }
                break;

        }
    } else {

        err_eof( inpf, "missing end of comment (\"*>\")" );
        *state = CMST_ERR;

    }

    return( (BOOL) ( (*state == CMST_END) || (*state == CMST_ERR ) ) );
}

/*
** skip_hsc_comment
**
** skip text until '*>' occures;
** nested commets are supported
**
*/
BOOL skip_hsc_comment( INFILE *inpf )
{
    BYTE cstate = CMST_TEXT; /* vars for eoc_reached() */
    LONG cnest  = 0;
    BOOL end    = FALSE;     /* end of comment reached? */

    while ( !end && !fatal_error ) {

        end = eoc_reached( inpf, &cstate, &cnest );

    }

    return ( (BOOL) !fatal_error);
}

