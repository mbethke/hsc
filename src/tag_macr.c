/*
** tag_macr.c
**
** tag handles for "<HSC_MACRO>" and "<macro>"
**
** updated:  4-Sep-1995
** created:  5-Aug-1995
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
#include "macro.h"
#include "vars.h"

#include "config.h"
#include "cleanup.h"
#include "error.h"
#include "find.h"
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"

char macpar[ MAX_ARGLEN ];
BYTE  rmt_str[ MAXLINELEN ];
FILE *fmacout;

/*
**-------------------------------------
** handlers for opening/closing macro
**-------------------------------------
*/

/*
** handle_macro
**
** handle for macro
**
** params: open_mac..TRUE, if called by an openning macro
**         inpf......input file
*/
BOOL handle_macro( BOOL open_mac, INFILE *inpf )
{
    BOOL    ok = FALSE;
    HSCMAC *macro = find_strmac( this_tag_data->name );
    STRPTR  fname;
    DLLIST *args;
    ULONG   mci; /* macro call id returned by set_macro_args() */

    /* determine filename & args */
    if ( open_mac ) {
        fname = macro->op_fname;
        args  = macro->op_args;
    } else {
        fname = macro->cl_fname;
        args  = macro->cl_args;
    }

    /* debugging message */
    if ( debug ) {
        fprintf( stderr, "**-MACRO <" );
        if ( !open_mac )
            fprintf( stderr, "/" );
        fprintf( stderr, "%s> from \"%s\"\n", macro->name, fname );
    }

    /* parse args */
    mci = set_macro_args( macro, inpf, open_mac );
    if (debug)
        prt_varlist( args, "macro args (after set_macro_args)" );

    /* copy local vars to global list */
    ok = copy_local_varlist( args, mci );
    if (debug)
        prt_varlist( vars, "global vars (after copy_local_vars)" );

    /* include macro file */
    ok = include_hsc( fname, outfile, 0 );

    /* remove local vars */
    remove_local_varlist( mci );

    /* debugging message */
    if ( debug )
        fprintf( stderr, "**-ENDMACRO <%s>\n", macro->name );

    return ( ok );
}

/*
** handle_op_macro
**
** handle for opening macro
*/
BOOL handle_op_macro( INFILE *inpf )
{
    return( handle_macro(TRUE, inpf) );
}

/*
** handle_cl_macro
**
** handle for closing macro
*/
BOOL handle_cl_macro( INFILE *inpf )
{
    return( handle_macro(FALSE, inpf) );
}

/*
**-------------------------------------
** macro creation functions
**-------------------------------------
*/

/*
** outmac
**
** write a string to macro file
*/
BOOL outmac( STRPTR s, INFILE *inpf )
{
    BOOL ok = ( fputs( s, fmacout ) != EOF );

    if ( !ok ) {
        message( FATAL_WRITE_MACRO, inpf );
        errstr( "Error writing macro" );
    }

    return ( ok );
}

/*
** rmt_check_for
*/
BOOL rmt_check_for( INFILE *inpf, STRPTR cmpstr )
{
    STRPTR  nw = infgetw( inpf );
    BOOL    eq = FALSE;

    if ( nw ) {

        eq = !upstrcmp( nw, cmpstr );
        strcat( rmt_str, infgetcws( inpf ) );
        strcat( rmt_str, infgetcw( inpf ) );

    } else
        err_eof( inpf );

    return( eq );
}

/*
** read_macro_text
**
*/
BOOL read_macro_text( HSCMAC *macro, INFILE *inpf, BOOL open_mac )
{
    BOOL    ok = FALSE;
    STRPTR  fname;

    /* open output file for macro text */
    if ( open_mac )
        fname = macro->op_fname;
    else
        fname = macro->cl_fname;
    fmacout = fopen( fname, "w" );

    if ( fmacout ) {

        /* skip first LF if any */
        skip_lf( inpf );

        strcpy( rmt_str, "" );
        while ( !(ok || fatal_error) ) {

            if ( rmt_check_for( inpf, "<" )
                 &&  rmt_check_for( inpf, "/" )
                 &&  rmt_check_for( inpf, STR_HSC_MACRO )
               )
            {

                ok = TRUE;
                parse_gt( inpf );

            } else {

                outmac( rmt_str, inpf );
                strcpy( rmt_str, "" );

            }
        }
    } else {

        message( FATAL_WRITE_MACRO, inpf );
        errstr( "Error writing macro" );
    }

    fclose( fmacout );

    return ( ok );

}

/*
**-------------------------------------
** main handler function for creation
**-------------------------------------
*/


/*
** handle_hsc_macro
**
** define a new macro tag
*/
BOOL handle_hsc_macro( INFILE *inpf )
{
    BOOL    ok = FALSE;
    BOOL    open_mac;
    HSCMAC *macro;

    /* get name and argumets */

    macro = def_macro_name( inpf, &open_mac );

    ok = ( macro && def_macro_args( macro, inpf, &open_mac ) );
    if ( ok ) {

        /* copy macro text to temp. file */
        if ( macro )
            ok = read_macro_text( macro, inpf, open_mac );

        /* create a new tag for macro */
        if ( ok ) {
            ok = (BOOL) add_tag( macro->name, HT_NOCOPY | HT_MACRO, 0,
                                 handle_op_macro, handle_cl_macro );
        }
    }

    return ( ok );
}

