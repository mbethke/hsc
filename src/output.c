/*
** output.c
**
** output functions for hsc
**
** updated: 31-Oct-1995
** created:  1-Jul-1995
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "ugly/types.h"
#include "ugly/outfile.h"
#include "ugly/prginfo.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "status.h"

OUTFILE *outfile = NULL;

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

    outfile = outfopen( outfilename, 128 );      /* try to open output */

    result = ( outfile != NULL );

    if ( !result ) {

        message( MSG_NO_OUTPUT, NULL );
        errstr( "can not open " );
        errqstr( outfilename );
        errstr( " for output: " );
        errstr( strerror( errno ) );
        errlf();

    } else if ( verbose ) {

        fprintf( stderr, "output: \"%s\"\n",  outfget_fname( outfile ) );

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
            this_ch = outfputc( ch, outfile );
        if ( this_ch == EOF ) {

             err_write( outfile );

        }
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
    else {

        ch_written = outfputs( str, outfile );
        if ( ch_written == EOF )
            err_write( outfile );

    }

    return( ch_written );
}

/*
** close_output
**
** close output file, if it not stdout
*/
VOID close_output( VOID )
{
    if ( outfile ) {

        if ( (return_code == RC_OK)
             || (return_code == RC_WARN)
             || debug )
        {

            status_msg( "Writting output" );
            errno = 0;
            outfclose( outfile );
            if ( errno )
                err_write( outfile );
            status_clear();

        } else {

            status_msg( "No output written" );
            status_lf();
            outfclear( outfile );

        }
    }
}

