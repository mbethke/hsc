/*
** tag_varr.c
**
** tag handles for "<HSC_VAR>" and "<var>"
**
** updated:  8-Sep-1995
** created:  2-Sep-1995
*/

#include <stdio.h>
#include <string.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/memory.h"
#include "ugly/string.h"

#include "global.h"
#include "config.h"
#include "cleanup.h"
#include "error.h"
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"

#include "vars.h"

FILE *fvarout;


/*
**-------------------------------------
** var creation functions
**-------------------------------------
*/

/*
** outvar
**
** write a string to var file
*/
BOOL outvar( STRPTR s, INFILE *inpf )
{
    BOOL ok = ( fputs( s, fvarout ) != EOF );

    if ( !ok ) {
        message( FATAL_WRITE_VAR, inpf );
        errstr( "Error writing var" );
    }

    return ( ok );
}

/*
** get_var_args
**
*/
HSCVAR *get_var_args( INFILE *inpf )
{
    BOOL    ok = FALSE;
    STRPTR  nw = infgetw( inpf );
    HSCVAR *var = NULL;
    HSCTAG *tag;

    /* create new var */
    if ( nw ) {

        open_var = (BOOL)(strcmp( nw, "/" ));
        if ( !open_var ) {

            /* add closing var */
            nw = infgetw( inpf );
            if ( nw ) {

                var = find_strvar( nw );
                if ( var ) {

                    tag = find_strtag( nw );
                    if ( tag )
                        tag->option |= HT_CLOSE;
                    else
                        panic( "no tag for var" );

                } else {

                }


            } else
                err_eof( inpf );

        } else {

            /* create a new opening var */
            var = app_var( nw );
        }
    } else
        err_eof( inpf );

    if ( var ) {

        /* convert var name to upper  case */
        upstr( var->name );

        /* read args */
        nw = infgetw( inpf );
        if ( strcmp( nw, ">" ) ) {

            while ( nw ) {

                if ( !strcmp( nw, ">" ) ) {

                    nw = NULL;
                    ok = TRUE;

                } else {


                    nw = infgetw( inpf );
                }
            }

            if ( !(ok || fatal_error) )
                err_eof( inpf );

            if ( !ok ) {
                del_dlnode(
                    vars,
                    find_dlnode( vars->first, (APTR) var->name, cmp_strvar )
                );
                var = NULL;
            }
        }
    }

    return ( var );
}

/*
**-------------------------------------
** main handler function for creation
**-------------------------------------
*/


/*
** handle_hsc_var
**
** define a new var tag
*/
BOOL handle_hsc_var( INFILE *inpf )
{
    BOOL    ok = FALSE;
    HSCVAR *var;

    /* get name and argumets */
    var = get_var_args( inpf );

    /* copy var text to temp. file */
    if ( var )
        ok = read_var_text( var, inpf );

    /* create a new tag for var */
    if ( ok ) {
        ok = (BOOL) add_tag( var->name, HT_NOCOPY | HT_VAR, 0,
                             handle_op_var, handle_cl_var );
    }

    return ( ok );
}

