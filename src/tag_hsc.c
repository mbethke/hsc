/*
** tag_hsc.c
**
** tag handles for "<$xx>"
**
** (for macro handles, see "tag_macr.c")
**
** updated:  6-Nov-1995
** created: 23-Jul-1995
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/memory.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "skip.h"
#include "status.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"

#include "deftag.h"
#include "tag_macr.h"
#include "tag_if.h"

#define TIMEBUF_INC 20
#define ES_STEP_SOURCE 40

/* states for handle_hsc_source */
#define SRST_TEXT    0 /* inside text      */
#define SRST_LT      1 /* after "<"        */
#define SRST_SLASH   2 /* after "</"       */
#define SRST_CSOURCE 3 /* after "</$SOURCE" */
#define SRST_TAG     8 /* inside a tag     */
#define SRST_COMT    9 /* inside comment   */
#define SRST_ERR    99 /* error occured    */

/*
**
** global funs
**
*/

/*
** unkn_hscoptn
**
** write out error message: "Option <optn> unknown for hsc-tag <tag>"
*/
void unkn_hscoptn( STRPTR tag, STRPTR optn, INFILE *inpf )
{
    /* error: unknown option */
    message( MSG_HSC_ERR, inpf );
    errstr( "Option " );
    errqstr( optn );
    errstr( " unknown for hsc-tag " );
    errtag( tag );
    errlf();
}


/*
**-------------------------------------
** comment & skip handle (<* *>, <| |>)
**-------------------------------------
*/

/*
** handle_hsc_comment
**
** skip text until '*>' occures;
** nested commets are supported
**
*/
BOOL handle_hsc_comment( INFILE *inpf, HSCTAG *tag )
{
    return ( skip_hsc_comment( inpf ) );
}

/*
** handle_hsc_skip
**
** copy text until '|>' occures;
** no syntax check or whatsoever is performed
**
*/
BOOL handle_hsc_onlycopy( INFILE *inpf, HSCTAG *tag )
{
    int  ch      = EOF;                /* current char */
    int  prev_ch = EOF;                /* prev char read */
    BOOL abort   = FALSE;

    do {

        ch = infgetc( inpf );

        if ( (prev_ch=='|') && (ch=='>') ) {

            abort = TRUE;

        } else if ( (prev_ch=='|') && (ch!='|') ) {

            outch( prev_ch );
            outch( ch );

        } else if ( ch != '|' )
            outch( ch );

        abort |= infeof(inpf);
        prev_ch = ch;

    } while ( !abort );

    if ( prev_ch == EOF )
        err_eof(inpf, "skipping source");

    return (TRUE);
}

/*
**-------------------------------------
** $INCLUDE handle
**-------------------------------------
*/

/*
** handle_hsc_include
**
** include a sub file
*/
BOOL handle_hsc_include( INFILE *inpf, HSCTAG *tag )
{
    STRPTR  fname  = get_vartext( tag->attr, "FILE" );
    BOOL    source = get_varbool( tag->attr, "SOURCE" );
    BOOL    pre    = get_varbool( tag->attr, "PRE" );
    ULONG   optn   = 0;

    if ( source )
        optn |= IH_PARSE_SOURCE;
    if ( pre )
        include_hsc_string( "[include <PRE>]", "<PRE>\n",
                            outfile, IH_PARSE_HSC );
    if ( fname )
        include_hsc_file( fname, outfile, optn );
    if ( pre )
        include_hsc_string( "[include </PRE>]", "</PRE>\n",
                            outfile, IH_PARSE_HSC );

    return (TRUE);
}


/*
**-------------------------------------
** $EXEC handle
**-------------------------------------
*/

/*
** handle_hsc_exec
**
** exec a sub file
*/
BOOL handle_hsc_exec( INFILE *inpf, HSCTAG *tag )
{
    STRPTR cmd = get_vartext( tag->attr, "COMMAND" );

    if ( cmd ) {

        int     result;
        EXPSTR *msg = init_estr( 0 );

        if ( msg
             && app_estr( msg, "execute: " )
             && app_estr( msg, cmd ) )
        {

            /* status message */
            status_msg( estr2str( msg ) );
            if ( verbose )
                status_lf();

            /* call command */
            result = system( cmd );

            /* check for non-zero-result */
            if ( result ) {

                 message( MSG_SYSTEM_RETURN, inpf );
                 errstr( "Calling external command returned " );
                 errstr( long2str( (LONG) result ) );
                 errlf();

            }

        } else
            err_mem( inpf );

        del_estr( msg );

    }

    return (TRUE);
}

/*
**-------------------------------------
** $INSERT handle
**-------------------------------------
*/


/*
** handle_hsc_time
**
** insert current time
*/
BOOL handle_hsc_time( INFILE *inpf, HSCTAG *tag )
{
    STRPTR  timefmt = get_vartext( tag->attr, "FORMAT" );
    EXPSTR *timebuf = init_estr( TIMEBUF_INC );
    BOOL    strftrc = 0; /* result of strftime() */
    size_t  i; /* loop var */

    /* set default time format */
    if ( !timefmt )
        timefmt = "%d-%b-%Y, %H:%M";

    while ( !fatal_error && !strftrc ) {

        /* expand timebuffer */
        for ( i=0; i<TIMEBUF_INC; i++ )
            if ( !app_estrch( timebuf, '.' ) )
                err_mem( inpf );

        D( fprintf( stderr, "**   timebuf: inc+%d\n", TIMEBUF_INC ) );

        /* output time */
        strftrc = strftime( estr2str( timebuf ), estrlen( timebuf ),
                            timefmt, localtime(&now) );

    }

    if ( strftrc )
        include_hsc_string( "[insert TIME]", estr2str( timebuf ),
                            outfile, IH_PARSE_HSC );

    del_estr( timebuf );

    return (TRUE);
}

/*
** handle_hsc_text
**
** insert text
*/
BOOL handle_hsc_text( INFILE *inpf, HSCTAG *tag )
{
    STRPTR text = get_vartext( tag->attr, "TEXT" );

    /* include text */
    include_hsc_string( "[insert TEXT]", text, outfile, IH_PARSE_HSC );

    return (TRUE);
}


/*
** hsc_insert
**
** main insert handle
*/
BOOL handle_hsc_insert( INFILE *inpf, HSCTAG *tag )
{
    BOOL insert_text = FALSE;
    BOOL insert_time = get_varbool( tag->attr, HSC_TIME_STR );

    if ( get_vartext( tag->attr, HSC_TEXT_STR ) )
        insert_text = TRUE;

    if ( insert_text )
        handle_hsc_text( inpf, tag );
    else if ( insert_time )
        handle_hsc_time( inpf, tag );
    else {

        /* unknown option for $insert */
        message( MSG_MISSING_ATTR, inpf );
        errstr( "required attribute for <" );
        errstr( HSC_INSERT_STR );
        errstr( "> missing\n" );

    }

    /* clear attributes */
    clr_varlist( tag->attr );

    return( TRUE );
}

/*
**-------------------------------------
** <$DEFTAG> define a new tag
**-------------------------------------
*/
BOOL handle_hsc_deftag( INFILE *inpf, HSCTAG *tag )
{
    BOOL   ok = FALSE;
    BOOL   open_tag;

    tag = def_tag_name( deftag, inpf, &open_tag );
    ok = ( tag && def_tag_args( deftag, tag, inpf, &open_tag ) );

    return ( ok );
}

/*
**-------------------------------------
** <$DEFENT> define a new entity
**-------------------------------------
*/
BOOL handle_hsc_defent( INFILE *inpf, HSCTAG *tag )
{
    STRPTR name = get_vartext( tag->attr, "NAME" );
    STRPTR rplc = get_vartext( tag->attr, "RPLC" );
    LONG   num  = 0; /* TODO: get numeric entity */
    BOOL   ok;

    ok = add_ent( name, rplc, num );

    return ( ok );
}

/*
**-------------------------------------
** <$LET> set a new global attribute
**        or overwrite a defined one
**-------------------------------------
*/
BOOL handle_hsc_let( INFILE *inpf, HSCTAG *tag )
{
    STRPTR varname = infgetw( inpf );
    BOOL   ok = FALSE;

    /* create copy of varname */
    if ( varname )
        varname = strclone( varname );
    else
        err_eof( inpf, "missing attribute name" );

    if ( varname ) {

        ok = parse_wd( inpf, ":" );
        if ( ok && define_var( varname, vars, inpf, 0 ) )
            ok = TRUE;
        if ( ok )
            ok = parse_gt( inpf );
    } else
        err_mem( inpf );

    /* release mem */
    ufreestr( varname );

    /* if error occured, skip rest of tag */
    if ( !ok )
        skip_until_eot( inpf );

    return ( ok );
}

/*
**-------------------------------------
** <$SOURCE> include a source part
**-------------------------------------
*/
BOOL handle_hsc_source( INFILE *inpf, HSCTAG *tag )
{
    BOOL    pre     = get_varbool( tag->attr, "PRE" );
    BOOL    ok      = TRUE;
    EXPSTR *bufstr = NULL;
    EXPSTR *srcstr  = NULL;
    LONG    nesting = 0;
    BYTE    state   = SRST_TEXT;
    STRPTR  nw      = NULL;

    /*
    ** read until </$SOURCE> found
    */

    /* init bufstr */
    bufstr = init_estr( ES_STEP_SOURCE );
    srcstr  = init_estr( ES_STEP_SOURCE );

    if ( !(bufstr && srcstr ) )
        err_mem( inpf );

    while ( !(fatal_error || (state==SRST_CSOURCE) ) ) {

        /* read next word */
        if ( state == SRST_SLASH )
            nw = infget_tagid( inpf  );
        else if ( state != SRST_TAG )
            nw = infgetw( inpf  );

        if ( nw ) {

            if ( state == SRST_TAG ) {

                /*
                ** skip inside tags
                */
                BYTE   tag_state = TGST_TAG; /* state var passe to */
                                             /*     eot_reached() */

                do {

                    if ( eot_reached( inpf, &tag_state ) );
                        state = SRST_TEXT;

                    if ( !( app_estr( srcstr, infgetcws( inpf ) )
                            && app_estr( srcstr, infgetcw( inpf ) )
                    ) )
                        err_mem( inpf );

                } while ( (tag_state!=TGST_END) && !fatal_error );

            } else {

                switch ( state ) {

                    case SRST_TEXT:
                        if ( !strcmp( nw, "<" ) )
                            state = SRST_LT;
                        break;

                    case SRST_LT:
                        if ( !strcmp( nw, "/" ) )
                            state = SRST_SLASH;
                        else if ( !upstrcmp( nw, HSC_COMMENT_STR ) ) {

                            state = SRST_COMT;

                        } else {

                            /* handle "<$SOURCE" (open source) */
                            if ( !upstrcmp( nw, HSC_SOURCE_STR ) )
                                nesting++; /* incr. source nesting */
                            state = SRST_TAG;

                        }
                        break;

                    case SRST_SLASH:
                        if ( !upstrcmp( nw, HSC_SOURCE_STR ) )

                            /* handle "</$SOURCE" (close source) */
                            if ( nesting )
                                nesting--;   /* decr. source nesting */
                            else
                                state = SRST_CSOURCE; /* end of source */
                        else
                            state = SRST_TAG;
                        break;

                }

                if ( state == SRST_TEXT ) {

                    /* append current white spaces & word to srcstr */
                    if ( !( app_estr( srcstr, infgetcws( inpf ) )
                            && app_estr( srcstr, infgetcw( inpf ) )
                    ) )
                        err_mem( inpf );

                } else if ( ( state == SRST_COMT )
                            || ( state == SRST_TAG ) )
                {

                    /* append bufstr to srcstr, clear bufstr,
                    ** append current word to srcstr
                    */
                    if ( !( app_estr( srcstr, estr2str(bufstr) )
                            && set_estr( bufstr, "" )
                            && app_estr( srcstr, infgetcws( inpf ) )
                            && app_estr( srcstr, infgetcw( inpf ) )
                    ) )
                        err_mem( inpf );

                } else {

                    /* append current white spaces & word to srcstr */
                    if ( !( app_estr( bufstr, infgetcws( inpf ) )
                            && app_estr( bufstr, infgetcw( inpf ) )
                    ) )
                        err_mem( inpf );

                }

                /*
                ** skip hsc comment
                */
                if ( state == SRST_COMT ) {

                    BYTE cstate = CMST_TEXT; /* vars for eoc_reached() */
                    LONG cnest  = 0;
                    BOOL end    = FALSE;     /* end of comment reached? */

                    while ( !end && !fatal_error ) {

                        end = eoc_reached( inpf, &cstate, &cnest );
                        if ( !( app_estr( srcstr, infgetcws( inpf ) )
                                && app_estr( srcstr, infgetcw( inpf ) )
                        ) )
                            err_mem( inpf );

                    }

                    state = SRST_TEXT; /* reset state after comment */
                }
            }
        } else {

            err_eof( inpf, "missing </" HSC_SOURCE_STR ">" );
            state = SRST_ERR;

        }

    } /* while */

    /* check for legal end state */
    if ( state == SRST_CSOURCE ) {

        ok = parse_wd( inpf, ">" );

    }

    /* include source */
    if ( ok ) {

        if ( pre )
            include_hsc_string( "[include <PRE>]", "<PRE>\n",
                                outfile, IH_PARSE_HSC );
        include_hsc_string( "[SOURCE]", estr2str( srcstr ),
                                outfile, IH_PARSE_SOURCE );
        if ( pre )
            include_hsc_string( "[include </PRE>]", "</PRE>\n",
                                outfile, IH_PARSE_HSC );


    }

    del_estr( bufstr );
    del_estr( srcstr );

    return ( ok );
}


