/*
** status.c
**
** status messages
**
** updated: 30-Jul-1995
** created: 30-Jul-1995
**
*/

/*
** ansi includes
*/
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/infile.h"

/*
** local includes
*/
#include "global.h"

size_t  prev_stmsg_len = 0; /* length of previos status message */
INFILE *prev_stfile = NULL; /* previous input file for status message */
ULONG   prev_stline = 0;    /* previous line num in input file */

#define ST_LINE_QUANTUM 25  /* num of lines that have to be processed */
                            /* before a status message is printed */


/*
** status_msg
**
** display a status message
**
** params: s.....message to display
*/
int status_msg( STRPTR s )
{
    size_t new_stmsg_len = strlen( s );
    int  ctr = 0;
    size_t i;

    if ( statusmsg ) {

        /* display message */
        ctr += fprintf( stderr, "%s", s );


        /* clear tail of old message */
        for ( i=new_stmsg_len; i<prev_stmsg_len; i++ )
            fprintf( stderr, " " );

        /* perform cr & flush stdder */
        fprintf( stderr, "\r" );
        fflush( stderr );

        prev_stmsg_len = new_stmsg_len;

    }

    return (ctr);
}


/*
** status_clear
**
** clear status message
*/
int status_clear( void )
{
    return ( status_msg( "" ) );
}


/*
** status_lf
**
** perform a linfeed with status messages
** (old message will stay visible)
*/
int status_lf( void )
{
    int ctr = 0;

    if (status_msg)
        ctr = ( fprintf( stderr, "\n" ) );

    return (ctr);
}


/*
** status_infile
**
** display input file status (filename & line-count)
**
** params: f......input file to display message about
**         force..display message in any case
*/
int status_infile( INFILE *f, BOOL force )
{
    char buf[MAXLINELEN];
    int  ctr = 0;

    if (f) {

        if ( prev_stfile != f )
            prev_stline = 0;

        force |= ( (prev_stfile != f)
                   || ( (infget_y(f)-prev_stline) > ST_LINE_QUANTUM )
                 );

        if (force) {

            sprintf( buf, "%s (%d)", infget_fname(f), infget_y(f) );
            ctr = status_msg( buf );
            prev_stline = infget_y(f);
            prev_stfile = f;

        }

    }

    return ctr;


}

