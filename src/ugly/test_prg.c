/* test prginfo.c */

#include <stdio.h>

#include "utypes.h"
#include "prginfo.h"

void main( void )
{
    set_prginfo( "my_prog", "Tommy-Saftwörx", 1, 3, 7,
                 "just a program", "This is FreeWare!" );

    printf( "\nthis is my program\n\n" );
}
