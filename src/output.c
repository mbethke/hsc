/*
** output.c
**
** output functions for hsc
**
** updated: 29-Jul-1995
** created: 01-Jul-1995
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "ugly/types.h"
#include "ugly/prginfo.h"

#include "global.h"
#include "error.h"

/*
** open_output:
**
** open output to file passed in arguments (outfilename)
** or to stdout if no filename passed.
**
** result: TRUE if sucessful, else FALSE
*/
BOOL open_output( void )
{
    BOOL result; /* return value */

    if ( outfilename ) {                                   /* output filename defined? */

        outfile = fopen( outfilename, "w" );               /* Y-> try to open it */

        result = ( outfile != NULL );

        if ( !result ) {

            errqstr( outfilename );
            errstr( ": can not open output file: " );
            errstr( strerror( errno ) );
            errch( '\n' );

        } else if ( debug ) {

            errqstr( outfilename );
            errstr( ": opened as output file\n" );

        }

    } else {                                               /* N-> set stdout as output file */

        if ( debug )
            errstr( "opened stdout as output file\n" );

        outfile = stdout;              /* redirect output to stdout */
        result = TRUE;

    }

    return (result);

}

/*
** outch
*/
int outch( char ch )
{
    int this_ch = EOF;
    if ( !fatal_error ) {
        this_ch = fputc( ch, outfile );
        if ( this_ch == EOF )
             err_write( outfile );
   }

    return ( this_ch );
}

/*
** outstr
*/
int outstr( CONSTRPTR str )
{
    return ( fputs( str, outfile ) );
}

/*
** copy_until_gt
*/
BOOL copy_until_gt( INFILE *inpf )
{
    char nxtch = '\0';

    while ( (!infeof(inpf)) && (nxtch!='>') ) {
        nxtch = infgetc(inpf);
        outch( nxtch );
    }

    return (BOOL)(fatal_error);
}


/*
** skip_until_gt
*/
BOOL skip_until_gt( INFILE *inpf )
{
    char nxtch = '\0';

    while ( (!infeof(inpf)) && (nxtch!='>') ) {
        nxtch = infgetc(inpf);
    }

    return (BOOL)(fatal_error);
}


