/*
** tag_hsc.c
**
** tag handles for "<$xx>"
**
** (for macro handles, see "tag_macr.c")
**
** updated: 17-Sep-1995
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

#include "entity.h"
#include "tag.h"
#include "vars.h"

#include "tag_macr.h"

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
    int ch;                            /* current char */
    int prev_ch = 'x';                 /* prev char read (dummy init) */
    BOOL abort = FALSE;
    ULONG nesting = 1;

    do {

        ch = infgetc( inpf );
        if ( (prev_ch=='<') && (ch=='*') ) nesting++;
        if ( (prev_ch=='*') && (ch=='>') ) nesting--;
        abort = ( (nesting==0) || infeof(inpf) );
        prev_ch = ch;

    } while ( !abort );

    if ( infeof(inpf) && nesting )
        err_eof(inpf);

    return (TRUE);
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
        err_eof(inpf);

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
    STRPTR fname = get_vartext( tag->op_args, "FILE" );

    if ( fname )
        include_hsc_file( fname, outfile, 0 );

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
    STRPTR timefmt = get_vartext( tag->op_args, "FORMAT" );

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
    STRPTR text = get_vartext( tag->op_args, "STRING" );

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
    BOOL insert_text = get_varbool( tag->op_args, HSC_TEXT_STR );
    BOOL insert_time = get_varbool( tag->op_args, HSC_TIME_STR );

    if ( insert_text )
        handle_hsc_text( inpf, tag );
    else if ( insert_time )
        handle_hsc_time( inpf, tag );
    else {

        /* unknown option for $insert */
        message( MSG_MISSING_ATTR, inpf );
        errstr( "required attribute for <$" );
        errstr( HSC_INSERT_STR );
        errstr( "> missing\n" );

    }

    /* clear attributes */
    clr_varlist( tag->op_args );

    return( TRUE );
}

/*
**-------------------------------------
** functions for <HSC_VAR>
**-------------------------------------
*/

#if 0
/*
** set_var_value
*/
BOOL set_var_value( STRPTR varname, INFILE *inpf )
{
    BOOL ok = FALSE;

    ok = (BOOL)( define_var( varname, vars, inpf, 0 ) );

    if ( !ok )
        skip_until_gt( inpf );
    else
        ok = parse_ch( inpf, '>' );

    return (ok);
}



/*
** handle_hsc_var
**
** set or insert a var
*/
BOOL handle_hsc_var( INFILE *inpf )
{
    BOOL   ok = FALSE;
    STRPTR varname = NULL;
    STRPTR nw = infgetw( inpf );

    /* get varname */
    if ( nw ) {

        varname = strclone( nw );
        if ( !varname )
            err_mem( inpf );

    } else
        err_eof( inpf );

    if ( varname ) {

        nw = infgetw( inpf );
        if ( nw ) {

            if ( !strcmp( nw, "=" ) ) {

                /* set new value */
                ok = set_var_value( varname, inpf );

            } else if ( !strcmp( nw, ">" ) ) {

                /* insert value */
                HSCVAR *var = find_varname( vars, varname );
                if ( var ) {

                    char text[MAX_ARGLEN+2];

                    if ( var->text ) {

                        strcpy( text, "" );
                        if ( var->quote != VQ_NO_QUOTE )
                            strcat( text, ch2str( (char) var->quote ) );
                        strcat( text, var->text );
                        if ( var->quote != VQ_NO_QUOTE )
                            strcat( text, ch2str( (char) var->quote ) );

                        ok = hsc_insert_text( text );
                    }
                } else {
                    /* unknown var */
                    message( MSG_UNKN_SYMB, inpf );
                    errstr( "Undefined " );
                    errsym( varname );
                    errlf();

                }



            } else {

                /* syntax error */
                unkn_hscoptn( HSC_VAR_STR, nw, inpf );
                skip_until_gt( inpf );
            }

        }

        ufreestr( varname );

    } else
        err_eof( inpf );

    return (ok);
}
#endif

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
    STRPTR name = get_vartext( tag->op_args, "NAME" );
    STRPTR rplc = get_vartext( tag->op_args, "RPLC" );
    LONG   num  = 0; /* TODO: get numeric */
    BOOL   ok;

    ok = add_ent( name, rplc, num );

    return ( ok );
}

