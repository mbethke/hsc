/*
** hsc/output.c
**
** output functions for hsc
**
** Copyright (C) 1995,96  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 15-May-1996
** created:  1-Jul-1995
*/

#include <errno.h>


#include "hsc/global.h"
#include "hsc/status.h"

#include "ugly/returncode.h"


static EXPSTR *outstring = NULL; /* string where output is created in */


/*
** init_output:
**
** init output string
**
** result: TRUE if sucessful, else FALSE
*/
BOOL init_output( HSCPRC *hp )
{
    BOOL ok = TRUE; /* return value */

    /* allocate output string */
    outstring = init_estr( 1024 );

    return (ok);
}


/*
** cleanup_output:
**
** free output string
**
*/
VOID cleanup_output( VOID )
{
    del_estr( outstring );
}


/*
** close_output
**
** close output file, if it not stdout
*/
BOOL write_output( HSCPRC *hp )
{
#define MAX_ERRORLEN 79
    STRPTR outfilenm = NULL;
    BOOL   written = FALSE;

    if ( outfilename )
        outfilenm = estr2str( outfilename );

    if ( (return_code == RC_OK)
         || (return_code == RC_WARN)
         || hp->debug )
    {
        FILE *outfile = NULL;          /* output file */
        char buf[ MAX_ERRORLEN+2 ];    /* buffer for error string */

        /*
        ** try to open output file
        */
        if ( outfilenm )
        {
            errno = 0;
            outfile = fopen( outfilenm, "w" );
            if ( !outfile )
            {
                strncpy( buf, "unable to open `", MAX_ERRORLEN );
                strncat( buf, estr2str( outfilename ),
                         MAX_ERRORLEN - strlen(buf) );
                strncat( buf, "' for output: ", MAX_ERRORLEN - strlen(buf) );
                strncat( buf, strerror( errno ), MAX_ERRORLEN - strlen(buf) );
                status_error( buf );
            }
        }
        else
        {
            outfile = stdout;
            outfilenm = STDOUT_NAME;
        }

        /*
        ** write output
        */
        if ( outfile ) {

            status_msg( "writting output.." );
            errno = 0;
#if 1
            fwrite( estr2str( outstring ), sizeof( char ),
                    estrlen( outstring ), outfile );
#endif
            if ( errno ) {

                strncpy( buf, "error writing `", MAX_ERRORLEN );
                strncat( buf, estr2str( outfilename ),
                         MAX_ERRORLEN - strlen(buf) );
                strncat( buf, "': ", MAX_ERRORLEN - strlen(buf) );
                strncat( buf, strerror( errno ), MAX_ERRORLEN - strlen(buf) );
                status_error( buf );

            } else
                written = TRUE;

            status_clear();

            /* close output file */
            if ( outfile != stdout )
                fclose( outfile );

        }
    } else {

        status_msg( "no output written" );
        status_lf();

    }

    return( written );
}

/*
** append_output
**
** append text to output string
*/
VOID append_output( STRPTR text )
{
    app_estr( outstring, text );
}


