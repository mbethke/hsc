/*
** test_file.c
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "types.h"
#include "outfile.h"

int main( int argc, char *argv[] )
{
    OUTFILE *outf;
    char *fname;

    if ( argc == 1 )
        fname = "hugo2.txt";
    else
        fname = argv[1];

    errno = 0;
    outf = outfopen( fname, 0 );

    if ( outf ) {

        outfputc( 'h', outf );
        outfputs( "ugo ist doof.\n", outf );

        outfclose( outf );

    } else
        perror( "Can't open output file" );


    outf = outfopen( NULL, 0 );

    if ( outf ) {

        outfputc( 'h', outf );
        outfputs( "ugo ist doof.\n", outf );

        outfclose( outf );

    } else
        perror( "Can't open output file" );

    return( 0 );
}

