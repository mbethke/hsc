#include <stdio.h>
#include <stdarg.h>

typedef char  *va_list;

void hugo( int argtypep, ... )
{
    va_list ap;

#if 1
    int argtype;

    va_start( ap, argtypep );

    va_end( ap );
#endif
    printf( "hello\n" );
}
