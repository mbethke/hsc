/*
** tag_hsc.c
**
** tag handles for "<HSC_xx>"
**
** updated:  8-Sep-1995
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

#include "tag.h"
#include "vars.h"

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
    message( ERROR_UNKN_HSCOPTN, inpf );
    errstr( "Option " );
    errqstr( optn );
    errstr( " unknown for hsc-tag " );
    errtag( tag );
    errlf();
}


/*
** handle_hsc_comment
**
** skip text until '*>' occures;
** nested commets are supported
**
*/
BOOL handle_hsc_comment( INFILE *inpf )
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
    else
        skip_lf( inpf );

    return (TRUE);
}

/*
** handle_hsc_include
**
** include a sub file
*/
BOOL handle_hsc_include( INFILE *inpf )
{
    STRPTR fname;
    STRPTR nxtoptn;

    nxtoptn = parse_tagoptn( inpf );

    while ( nxtoptn ) {

        if ( !upstrcmp( nxtoptn, "SRC" ) ) {

            fname = parse_strarg( inpf );          /* get filename */

        } else
            /* error message: unknwon hsc-option */
            unkn_hscoptn( "HSC_INCLUDE", nxtoptn, inpf );

        nxtoptn = parse_tagoptn( inpf );

    } /*while*/

    if ( fname ) {

        include_hsc( fname, outfile, 0 );

    }

    skip_until_gt( inpf );
    skip_lf( inpf );

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
BOOL handle_hsc_onlycopy( INFILE *inpf )
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
** handle_hsc_time
**
** write current time
*/
BOOL handle_hsc_time( INFILE *inpf )
{
    char timebuf[TIMEBUF_SIZE];
    char timefmt[TIMEBUF_SIZE];
    STRPTR nxtoptn;

    /* set default time format */
    strcpy( timefmt, "%d-%b-%Y, %H:%M" );

    nxtoptn = parse_tagoptn( inpf );

    while ( nxtoptn ) {

        if ( !upstrcmp("FMT",nxtoptn) ) {

            /* get time format */
            nxtoptn = parse_strarg( inpf );
            if ( nxtoptn ) {

                strncpy( timefmt, nxtoptn, TIMEBUF_SIZE );
                timefmt[TIMEBUF_SIZE-1] = CH_NULL;

            }

        } else
            /* error message: unknwon hsc-option */
            unkn_hscoptn( STR_HSC_TIME, nxtoptn, inpf );


        nxtoptn = parse_tagoptn( inpf );
    }

    /* output time */
    if ( timefmt ) {

        strftime( timebuf, TIMEBUF_SIZE, timefmt,
                  localtime(&now) );
        outstr( timebuf );

    }

    skip_until_gt( inpf );

    return (TRUE);
}

/*
**-------------------------------------
** functions for <HSC_VAR>
**-------------------------------------
*/

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

            ok = include_hsc( fname, outfile, 0 );

        } else
            /* todo: better error msg */
            err_write( NULL ) ;

        ufreestr( fname );

    } else
        err_mem( NULL );

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
                    message( VAR_UNKN, inpf );
                    errstr( "Undefined " );
                    errsym( varname );
                    errlf();

                }



            } else {

                /* syntax error */
                unkn_hscoptn( STR_HSC_VAR, nw, inpf );
                skip_until_gt( inpf );
            }

        }

        ufreestr( varname );

    } else
        err_eof( inpf );

    return (ok);
}


