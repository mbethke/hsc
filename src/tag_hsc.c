/*
** tag_hsc.c
**
** tag handles for "<$xx>"
**
** (for macro handles, see "tag_macr.c")
**
** updated: 19-Oct-1995
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

#include "entity.h"
#include "tag.h"
#include "vars.h"

#include "deftag.h"
#include "tag_macr.h"
#include "tag_if.h"

#define TIMEBUF_SIZE 40

/*
** TODO: make <| |> nestable
*/

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
** insert_var_value
*/
BOOL hsc_insert_text( STRPTR text )
{
    BOOL ok = FALSE;
    STRPTR fname = tmpnamstr();

    if ( fname ) {

        FILE *fout = fopen( fname, "w" );

        /* write text to temp. file */
        if ( fout ) {

            if ( fputs( text, fout ) == EOF )
                /* todo: error */;
            fclose( fout );

            ok = include_hsc_file( fname, outfile, IH_PARSE_HSC );

        } else
            /* todo: better error msg */
            err_write( NULL ) ;

        ufreestr( fname );

    } else
        err_mem( NULL );

    return (ok);
}

#if 0
/*
**-------------------------------------
** pseudo-handle for closing hsc-tag
**-------------------------------------
*/

/*
** handle_hsc_close
**
** handle closing hsc-tag (after </$.. >
*/
BOOL handle_hsc_close( INFILE *inpf, HSCTAG *tag )
{
    BOOL ok = TRUE;
    STRPTR nw = infgetw( inpf );

    if ( !nw )
        err_eof( inpf );
    else if ( !upstrcmp( nw, HSC_IF_STR ) )
        handle_hsc_cif( inpf, tag );
    else if ( !upstrcmp( nw, HSC_MACRO_STR ) ) {

        message( MSG_UNMA_CTAG, inpf );
        errstr( "Unmatched closing hsc-tag <$" );
        errstr( nw );
        errstr( ">" );
        errlf();

    } else {

        message( MSG_UNKN_TAG, inpf );
        errstr( "Unknown closing hsc-tag <$" );
        errstr( nw );
        errstr( ">" );
        errlf();

    }

    return ( ok );
}
#endif


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
    return ( skip_hsc_comment( inpf, FALSE ) );
}

/*
** handle_hsc_skip
**
** copy text until '|>' occures;
** no syntax check or whatsoever is performed
**
** TODO: implement that
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
    STRPTR fname  = get_vartext( tag->attr, "FILE" );
    BOOL   source = get_varbool( tag->attr, "SOURCE" );
    ULONG  optn   = 0;

    if ( source )
        optn |= IH_PARSE_SOURCE;
    if ( fname )
        include_hsc_file( fname, outfile, optn );

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

       int result = system( cmd );

       if ( result ) {

            message( MSG_SYSTEM_RETURN, inpf );
            errstr( "Calling external command returned " );
            errstr( long2str( (LONG) result ) );
            errlf();

       }

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
    STRARR timebuf[TIMEBUF_SIZE];
    STRPTR timefmt = get_vartext( tag->attr, "FORMAT" );

    /* set default time format */
    if ( !timefmt )
        timefmt = "%d-%b-%Y, %H:%M";

    /* output time */
    strftime( timebuf, TIMEBUF_SIZE, timefmt,
              localtime(&now) );
    include_hsc_string( "[insert TIME]", timebuf, outfile, IH_PARSE_HSC );

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
    LONG   num  = 0; /* TODO: get numeric */
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

    if ( varname ) {

        ok = parse_wd( inpf, ":" );
        if ( ok || define_var( varname, vars, inpf, 0 ) )
            ok = TRUE;
        if ( ok )
            ok = parse_gt( inpf );
    }

    /* if error occured, skip rest of tag */
    if ( !ok )
        skip_until_eot( inpf );

    return ( ok );
}


