/*
** test_file.c
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "types.h"
#include "infile.h"

#define MAXLINELEN 256
#define GETSTRLEN 10

void main( int argc, char *argv[] )
{
    INFILE *inpf;
    char *fname;
    char ch;
    size_t i;
    char getstr[GETSTRLEN+1];
    char *wdbuf;

    if ( argc == 1 )
        fname = "hugo.txt";
    else
        fname = argv[1];

    errno = 0;
    inpf = infopen( fname, MAXLINELEN );

    if ( inpf ) {

        /* info about first word */
        wdbuf = infgetw( inpf );
        printf( "word#0: \"%s\", \"%s\"\n",
                infgetcws( inpf ), infgetcw( inpf ) );


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

        /*
        ** read the first 3 words
        */
        for ( i=0; i<3; i++ ) {
            wdbuf = infgetw( inpf );
            printf( "word#%d: [%d] \"%s\"\n", i, strlen(wdbuf), wdbuf );
        }

        /* info about current word */
        printf( "current word: \"%s\", \"%s\"\n",
                infgetcws( inpf ), infgetcw( inpf ) );

        /*
        ** unget last word
        */
        inungetw( wdbuf, ' ', inpf );
        wdbuf = infgetw( inpf );
        printf( "reword: [%d] \"%s\"\n", strlen(wdbuf), wdbuf );

        /*
        ** read and print rest of file
        */

        while ( !infeof( inpf ) ) {

            ch = infgetc( inpf );

            if ( !infeof( inpf ) ) {
                printf( "%c", ch );
                fflush( stdout );      /* flush output -> slow */
            }

        }
    } else
        perror( "Can't open input file" );

    infclose( inpf );
}

