/*
** ugly/args_fre.c
**
** ugly argument freeing functions
** sub-module for ugly/args.c
**
** updated:  1 Aug-1995
** created:  3-Jul-1994
**
*/


/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "memory.h"
#include "string.h"
#include "dllist.h"

#include "args.h"


/*
** del_arginfo
**   ( called by _free_args() )
**
*/
void del_arginfo( APTR data )
{

    struct arginfo *arg = (struct arginfo *) data;

    if ( arg ) {

        /* type dependent cleanup */
        if ( arg->ai_type == ARG_ENUM )
            ufree( arg->ai_misc1 );

        /* cleanup entities */
        ufree( arg->ai_id );
        ufree( arg->ai_help );
        arg->ai_type = 0;
        arg->ai_dest = NULL;
        arg->ai_func = NULL;

        /* cleanup whole structure */
        ufree( arg );

    }

}


/*
** free_args
**
** free all memory allocated by _prepare_args()
**
** MUST be called after _set_args()
**
*/
void free_args( struct arglist *al )
{
    if ( al ) {
        del_dllist( al->al_list );
        ufree( al );
    }
}

