/*
** test_file.c
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "ugly/types.h"
#include "ugly/infile.h"

#define MAXLINELEN 256
#define GETSTRLEN 10

void main( int argc, char *argv[] )
{
    INFILE *inpf;
    char *fname;
    char ch;
    size_t i;
    char getstr[GETSTRLEN+1];

    if ( argc == 1 )
        fname = "hugo.txt";
    else
        fname = argv[1];

    errno = 0;
    inpf = infopen( fname, MAXLINELEN );

    if ( inpf ) {

        fprintf( stderr, "** file: \"%s\" at %p\n", inpf->filename, inpf );

        /*
        ** get 10 chars, print them,
        ** and put them back again
        */
        ch = '*'; /* dummy */
        for ( i=0; i<GETSTRLEN; i++ )
            getstr[i] = infgetc( inpf );

        getstr[i] = '\0';
        printf( "got : \"%s\"\nback: \"", getstr );

        for ( ; i>0; i-- ) {
            ch = getstr[i-1];
            printf( "%c", ch );
            inungetc( ch, inpf );
        }
        printf( "\"\n" );

        /* print log */
        printf( "log: \"%s\"\n", infget_log( inpf ) );

        fprintf( stderr, "** file: \"%s\" at %p\n", inpf->filename, inpf );
    } else
        perror( "Can't open input file" );

    infclose( inpf );
}

