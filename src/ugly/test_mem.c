/*
** test_mem
**
** test functions of ugly/memory.c
*/

#include "types.h"
#include "memory.h"
#include "string.h"

int main( void )
{
    APTR hugo1, hugo2, hugo3, hugo4;

    atexit( atexit_uglymemory );

    hugo1 = umalloc( 100 );
    hugo2 = umalloc( 200 );
    ufree( hugo2 );
    hugo3 = strclone( "hugo3" );
    hugo4 = (APTR) 0x123;
    umem_report( "hugo after alloc" );
    ufree( hugo1 );
    ufreestr( hugo3 );
    ufree( hugo2 );
    ufree( hugo4 );
    hugo4 = umalloc(400);

    umem_report( "hugo after free" );

    return( 0 );
}
