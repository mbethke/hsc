/*
** hsc.c - HTML sucks completely
**
** updated:  7-Aug-1995
** created:  1-Jul-1995
*/

/*
** TODO:
** - HT_NONEST, HT_NOBP. HT_NOAP (no <P> before/after tag allowd)
** - ignore warnings
** - &&, &>, &<
**
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
#ifdef AMIGA
    STRPTR version_string = "$VER: 0.7.4 hsc (8.9.1995)";
#endif
    BOOL ok = FALSE;

    /* set program information */
    set_prginfo( "hsc", "Tommy-Saftwörx", 0, 7, 4,
        "HTML Sucks Completely", "This is FreeWare!" );

#ifdef UMEM_TRACKING
    /* display a memory tracking report */
    /* at end of execution */
    atexit( atexit_uglymemory );
#endif

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
             && config_ok() )          /* read config */
        {
            /* include file parsed in args */
            ok = include_hsc( inpfilename, outfile, IH_PARSE_END );

        }

    }

    /* TODO: set return value 0,5,10,20 */
    /* ->define symbols like RC_FAIL, vars like any_warn */
    if ( ok )
        return( 0 ); /* successful */
    else
        return( 20 );
}

