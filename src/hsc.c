/*
** hsc.c - HTML sucks completely
**
** updated: 12-Sep-1995
** created:  1-Jul-1995
*/

/*
** TODO:
** - BUG: write whspc twice after (opening?) tag
** - support " > ", " < " and " & "
** - <$defent>
** - options for attributes
** - HT_NOBP. HT_NOAP (no <P> before/after tag allowd)
** - HT_SMART_CLOSE for <P> and <LI>
** - HT_STRIP_EXTERNAL to strip whole tag
**   if it references to an external URI
** - set error messages
** - set configurable error string
** - ignore warnings
** - also skip arg after unknown attribute
** - handler for break signal
**
** - <$IF>
** - check "NAME" with <A>
** - hsc.refs
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
    static STRPTR version_string = "$VER: 0.9.0 hsc (4.10.1995)";
#endif
    BOOL ok = FALSE;

    /* set program information */
    set_prginfo( "hsc", "Tommy-Saftwörx", 0, 9, 0,
        "HTML Sucks Completely", "This is FreeWare." );

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
            ok = include_hsc_file( inpfilename, outfile, IH_PARSE_END );

        }

    }

    return( return_code );
}

