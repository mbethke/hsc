/*
** ugly/args_fre.c
**
** ugly argument freeing functions
** sub-module for ugly/args.c
**
** Copyright (C) 1994,95,96  Thomas Aglassinger
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
** updated:  2-Nov-1995
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
            ufree( arg->ai_misc1.ai_enum );

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

