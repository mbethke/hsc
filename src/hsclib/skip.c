/*
** skip.c
**
** functions for skipping several things
**
** Copyright (C) 1995,96 Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 15-May-1996
** created:  8-Oct-1995
*/

#define NOEXTERN_SKIP_H

#include "hsclib/inc_base.h"

#include "hsclib/skip.h"

/*
** skip_lf
**
** ignore '\n'
**
** params: inpf...input file to read char from
** result: TRUE if skipped
*/
BOOL skip_lf( HSCPRC *hp )
{
    /*
    ** TODO: check, why skip_lf() should not be allowed to
    **   change the infp's current word
    */
    /* TODO: skip white-spaces after linefeed, if COMPACT set */
    INFILE *inpf = hp->inpf;

#if 0
    int nc = infgetc( inpf );

    if ( nc != '\n' )
        inungetc( nc, inpf );

    return ( (BOOL) (nc==EOF) );
#else
    BOOL   skipped = FALSE;
    STRPTR nw = infgetw( inpf );

    if ( nw )
        if ( strcmp( nw, "\n" ) )
            inungetcw( inpf );
        else
        {
            if ( hp->compact && !(hp->inside_pre) )
            {
                if (infskip_ws( inpf )) {
                    DMSG( "skipped whtspc after LF" );
                }
            }
            skipped = TRUE;
        }

    return skipped;
#endif
}

/*
** skip_lfs
**
** skip until any <> LF is found
**
** params: inpf...input file to read(s) char from
** result: TRUE if skipped at least one LF
*/
BOOL skip_lfs( HSCPRC *hp )
{
    BOOL skipped_any = FALSE;
    INFILE *inpf = hp->inpf;
    STRPTR nw = infgetw( inpf );

    /* skip linefeeds */
    while ( nw && ( !strcmp( nw, "\n" ) ) ) {

        skipped_any = TRUE;
        nw = infgetw( inpf );

    }

    /* write back last white spaces and word */
    if ( nw )
        inungetcwws( inpf );

    return ( skipped_any );
}


BOOL eot_reached( HSCPRC *hp, BYTE *state )
{
    INFILE *inpf = hp->inpf;
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
                    hsc_msg_eol( hp );
                    *state = TGST_TAG; /* go on reading inside tag */

                }


                break;

        }
    } else {

        hsc_msg_eof( hp, "`>' expected" );
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
BOOL skip_until_eot_state( HSCPRC *hp, BYTE *state, EXPSTR *logstr )
{
    INFILE *inpf = hp->inpf;

    while( !eot_reached( hp, state ) )
        if ( logstr ) {

            app_estr( logstr, infgetcws( inpf ) );
            app_estr( logstr, infgetcw( inpf ) );

        }

    /* append ">" */
    if ( logstr ) {

        app_estr( logstr, infgetcws( inpf ) );
        app_estr( logstr, infgetcw( inpf ) );

    }

    return( (BOOL) !(hp->fatal) );
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
BOOL skip_until_eot( HSCPRC *hp, EXPSTR *logstr )
{
    BYTE state = TGST_TAG;

    return( skip_until_eot_state( hp, &state, logstr ) );
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
BOOL eoc_reached( HSCPRC *hp, BYTE *state, LONG *nest )
{
    INFILE *inpf = hp->inpf;
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

        hsc_msg_eof( hp, "missing end of comment (\"*>\")" );
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
BOOL skip_hsc_comment( HSCPRC *hp )
{
    BYTE cstate = CMST_TEXT; /* vars for eoc_reached() */
    LONG cnest  = 0;
    BOOL end    = FALSE;     /* end of comment reached? */

    while ( !end && !(hp->fatal) ) {

        end = eoc_reached( hp, &cstate, &cnest );

    }

    return ( (BOOL) !(hp->fatal));
}

