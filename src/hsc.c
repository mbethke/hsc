/*
** hsc.c - HTML sucks completely
**
** updated: 15-Oct-1995
** created:  1-Jul-1995
*/

/*
** TODO:
** - input.c
** - check for comments inside $macro and $if
** - set error messages <$ERROR "..">
** - set configurable error string
** - handler for break signal
** - HT_NOBP. HT_NOAP (no <P> before/after tag allowd)
** - HT_SMART_CLOSE for <P> and <LI>
** - HT_STRIP_EXTERNAL to strip whole tag
**   if it references to an external URI
**
** - <$GOTO> and <$LABEL> (??)
** - <$WHILE> (??)
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
    BOOL ok = FALSE;

    /* set program information */
    set_prginfo( "hsc", "Tommy-Saftwörx", 0, 9, 3,
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
             && config_ok()            /* read config */
             && include_ok() )        /* read include files */
        {
            /* include file parsed in args */
            ok = include_hsc_file( inpfilename, outfile, IH_PARSE_END );
        }

    }

    return( return_code );
}

