#include <stdio.h>
#include <string.h>

#define BUFSIZE 10

#define WIDTH_IDX  6
#define HEIGHT_IDX 8

unsigned char buf[BUFSIZE];


int get_gifsize( char *filename, int *width, int *height )
{
    FILE *fgif = fopen( filename, "r" );
    int rc = 0;

    if ( fgif ) {

        if ( fread( buf, BUFSIZE, 1, fgif ) ) {


            if ( !strncmp( "GIF", buf, 3 ) ) {

                int i;

                *width  = buf[WIDTH_IDX]  + 256*buf[WIDTH_IDX+1];
                *height = buf[HEIGHT_IDX] + 256*buf[HEIGHT_IDX+1];

                for ( i=0; i<BUFSIZE; i++ ) {

                    printf( "%-2d: $%02x %-3d", i, buf[i], buf[i] );
                    if ( buf[i] >= 32 )
                        printf( " '%c'\n", buf[i] );
                    else
                        printf( "\n" );

                }

                printf( "width : %d\nheight: %d\n", *width, *height );

                rc = 1;

            } else
                fprintf( stderr, "Not a gif file\n" );


        }

        fclose( fgif );

    }

    return( rc );
}


int main( int argc, char *argv[] )
{
    int rc = 1; /* return code */

    if ( argc == 2 ) {

        int w;
        int h;

        rc = !get_gifsize( argv[1], &w, &h );

    } else
        fprintf( stderr, "Usage: gifsize filename\n" );

    return( rc );
}
