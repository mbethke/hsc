#include <stdio.h>
#include <stdlib.h>


void main( void )
{

    char *hugo = NULL;

    if ( hugo[0] ) ;

    hugo = (char *) malloc( 10 );
    hugo[16] = 11;
    system( "munglist ONLY test SHOWHUNK" );
    free( hugo );

}
