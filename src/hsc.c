/*
** hsc
**
** A dirty, clumsy and stupid HTML somehow-preprocessor
**
** Copyright (C) 1995  Thomas Aglassinger
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
**-------------------------------------------------------------------
**
** Author : Thomas Aglassinger
** Email  : agi@sbox.tu-graz.ac.at
** Address: Lissagasse 12/II/9
**          8020 Graz
**          AUSTRIA
**
**-------------------------------------------------------------------
**
** hsc.c
**
** updated: 28-Nov-1995
** created:  1-Jul-1995
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/args.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"

/*
** local includes
*/
#include "global.h"
#include "args.h"
#include "cleanup.h"
#include "config.h"
#include "output.h"
#include "parse.h"
#include "error.h"
#include "status.h"

/*
**
** main function
**
*/
int main( int argc, char *argv[] )
{
    BOOL ok = FALSE;

    /* set program information */
    set_prginfo( "hsc", "Tommy-Saftwörx", 0, 9, 4,
        "HTML Sucks Completely",
        "Freeware, type \"hsc LICENSE\" for details." );

#ifdef UMEM_TRACKING

    /* display a memory tracking report */
    /* at end of execution */
    atexit( atexit_uglymemory );

#endif

    /* install nomem-handler */
    ugly_nomem_handler = hsc_nomem_handler;


    /* use cleanup() as additional exit func */
    atexit( cleanup );

    /*
    ** main procedure
    */
    if ( args_ok( argc, argv ) ) {

        /* display programm-info if requested */
        if ( verbose )
            fprintf_prginfo( stderr );

        if ( open_error()              /* init error file */
             && open_output()          /* open output file */
             && config_ok()            /* read config */
             && include_ok() )         /* read include files (macros) */
        {
            /* process main file */
            ok = include_hsc_file( inpfilename, outfile, IH_PARSE_END );
        }

    }

    return( return_code );
}

