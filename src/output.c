/*
** output.c
**
** output functions for hsc
**
** updated:  8-Oct-1995
** created:  1-Jul-1995
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "ugly/types.h"
#include "ugly/prginfo.h"

#include "global.h"
#include "error.h"
#include "msgid.h"

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

            message( MSG_NO_OUTPUT, NULL );
            errstr( "can not open " );
            errqstr( outfilename );
            errstr( " for output: " );
            errstr( strerror( errno ) );
            errlf();

        } else if ( debug ) {

            fprintf( stderr, "\"%s\": opened as output file\n",  outfilename );

        }

    } else {                                               /* N-> set stdout as output file */

        if ( debug )
            fprintf( stderr, "opened stdout as output file\n" );

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

        if ( suppress_output )
            this_ch = ch;
        else
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
    int ch_written = 0;

    if ( suppress_output )
        ch_written = strlen( str );
    else
        ch_written = fputs( str, outfile );

    return( ch_written );
}

