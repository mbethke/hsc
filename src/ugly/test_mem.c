/*
** test_mem
**
** test functions of ugly/memory.c
*/

#include "types.h"
#define UMEM_TRACKING
#include "memory.h"
#include "string.h"

void main( void )
{
    APTR hugo1, hugo2, hugo3, hugo4;

    atexit( atexit_uglymemory );

    hugo1 = malloc( 100 );
    hugo2 = malloc( 200 );
    ufree( hugo2 );
    hugo3 = strclone( "hugo3" );
    hugo4 = (APTR) 0x123;
    umem_report( "hugo after alloc" );
    ufree( hugo1 );
    ufreestr( hugo3 );
    ufree( hugo2 );
    ufree( hugo4 );
    hugo4 = malloc(400);

    umem_report( "hugo after free" );

}
