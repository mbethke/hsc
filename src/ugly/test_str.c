/*
**
** test_str.c
**
** test string functions
**
** (W) by Tommy-Saftwörx in 1995
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "memory.h"
#include "string.h"


void test_enumstr( STRPTR str, STRPTR set, char sep, BYTE options )
{
    LONG result = strenum( str, set, sep, options );

    printf( "strenum( \"%4s\",%-16s,\"%c\",%d) -> %d\n",
            str, set, sep, options, result );
}

int main( void )
{
    LONG i;
    STRPTR nam;

#ifdef UMEM_TRACKING

    /* display a memory tracking report */
    /* at end of execution */
    atexit( atexit_uglymemory );

#endif

    printf( "Testing ugly string functions:\n\n" );

#if 1
    /*
    ** test strenum:
    */
    test_enumstr( "SEPP", "sepp|hugo|resi", '|', STEN_NOCASE );
    test_enumstr( "SEPP", "sepp|hugo|resi", '|', STEN_CASE );
    test_enumstr( "HUGO", "sepp|hugo|resi", '|', STEN_NOCASE );
    test_enumstr( "HUGO", "sepp|hugo|resi", '|', STEN_CASE );
    test_enumstr( "hugo", "sepp|hugo|resi", '|', STEN_CASE );
    test_enumstr( "RESI", "sepp|hugo|resi", '|', STEN_NOCASE );
    test_enumstr( "RESI", "sepp|hugo|resi", '|', STEN_CASE );
#endif


#if 1
    /*
    ** test strclone
    */
    nam = strclone( "hugo" );
    printf( "CLONE: \"%s\"\n", nam );
    ufreestr( nam );
#endif

    return( 0 );

}

