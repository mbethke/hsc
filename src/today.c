#include <stdio.h>
#include <string.h>
#include <time.h>


int main( void )
{
    char buf[20];

    time_t now = time( NULL );

    strftime( buf, 20, "%d-%b-%Y", localtime( &now ) );
    printf( "%s", buf );
}
