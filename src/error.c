/*
** error.c
**
** error vars & funs for hsc
**
** updated:  7-Aug-1995
** created:  9-Jul-1995
**
** TODO:
** - programable desription string to format error messages
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "ugly/types.h"
#include "ugly/string.h"

#include "global.h"
#define NOEXTERN_HSC_ERROR
#include "error.h"
#include "msgid.h"


/*
** TODO:
** - redirect error output to error-file
**
*/


/*
** global vars
*/
FILE *errfile = NULL;

/* flag set by several functions which write to output or */
/* try to alloc mem. if fatal error is set, many functions */
/* return a FALSE als result */
BOOL fatal_error = FALSE;

/* flag set by message() and checked by errch() to determine if*/
/* a message really should be printed out */
BOOL display_message = FALSE;
ULONG message_limit = 0;

/*
** errch
*/
int errch( char ch )
{
    if ( display_message )
        return ( fputc( ch, errfile ) );
    else
        return ( 1 );
}

/*
** errlf
*/
int errlf( void )
{
    return ( errch( '\n') );
}


/*
** errstr
*/
int errstr( CONSTRPTR str )
{
    return fputs( str, errfile );
}

/*
** errqstr
*/
int errqstr( CONSTRPTR str )
{
    int ctr = 2;

    errch( '\"' );
    ctr += errstr( str );
    errch( '\"' );

    return (ctr);
}


/*
** errtag
*/
int errtag( CONSTRPTR str )
{
    int ctr = 2;

    errch( '<' );
    ctr += errstr( str );
    errch( '>' );

    return (ctr);
}


/*
** errctag
*/
int errctag( CONSTRPTR str )
{
    int ctr = 3;

    errstr( "</" );
    ctr += errstr( str );
    errch( '>' );

    return (ctr);
}


/*
** errlong
*/
int errlong( ULONG num )
{
    return ( errstr(long2str(num)) );
}


/*
** open_error:
**
** open error-output to file passed in arguments (errfilename)
** or to stderr if no filename passed.
**
** result: TRUE if sucessful, else FALSE
*/
BOOL open_error( void )
{
    BOOL result; /* return value */

    if ( errfilename ) {                                   /* error filename defined? */

        errfile = fopen( errfilename, "w" );               /* Y-> try to open it */

        result = ( errfile != NULL );

        if ( !result ) {

            errqstr( errfilename );
            errstr( ": can not open error file: " );
            errstr( strerror( errno ) );
            errch( '\n' );

        } else {

            if ( verbose ) {

                errqstr( errfilename );
                errstr( ": opened as error file\n" );
            }

        }

    } else {                           /* N-> set stdout as output file */

        errfile = stderr;              /* redirect output to stdout */
        result = TRUE;

    }

    return result;

}

/*
** close_error:
**
** close error output file (if it was not stderr)
*/
void close_error ( void )
{
    if ( errfile && (errfile!=stderr) )
        fclose( errfile );
}


/*
** msg_ptr
*/
int msg_prt( STRPTR s, INFILE *f )
{
    int ctr=0;

    /*
    ** display file info
    */
    if ( f ) {
        if ( strcmp( FNAME_STDIN, infget_fname(f) ) ) {

            ctr += errqstr( infget_fname(f) );
            ctr += errstr( " (" );
            ctr += errlong( infget_y(f) );
            ctr += errstr( "," );
            ctr += errlong( infget_x(f) );
            ctr += errstr( ") " );

        }
    }

    /*
    ** display message id (eg "Error: ")
    */
    if (s) {
        ctr += errstr( s );
        ctr += errstr( ": " );
    }

    return (ctr);
}



/*
**
** global funs
**
*/

/*
** message
*/
int message( ULONG id, INFILE *f )
{
    int ctr = 0;

    display_message = ( (id>=message_limit) && !(fatal_error) );

    if ( display_message ) {

        if ( id>=FATAL )    {

            ctr = msg_prt( "Fatal error", f );
            fatal_error = TRUE;

        } else if ( id>=ERROR ) ctr = msg_prt( "Error", f );
        else if ( id>=WARN )  ctr = msg_prt( "Warning", f );
        else if ( id>=STYLE ) ctr = msg_prt( "Bad style", f );
        else if ( id>=MSG )   ctr = msg_prt( "Note", f );

    }

    return (ctr);

}


/*
** err_eof
**
** error: unexpected end-of-file
**
*/
int err_eof( INFILE *inpf )
{
    message( ERROR_EOF, inpf );
    return ( errstr( "Unexpected end of file" ) );
}

int err_mem( INFILE *inpf )
{
    message( FATAL_NO_MEM, inpf );
    return ( errstr( "Out of memory" ) );
}

int err_write( FILE *outf )
{
    message( FATAL_WRITE_ERR, NULL );
    return ( errstr( "Write error" ) );
}

int err_longstr( INFILE *inpf )
{
    message( FATAL_LONG_STR, inpf );
    return ( errstr( "Maximum string length exceeded (Thank K&R)" ) );
}



/*
** err_wst
**
** bad style: white space around tag
**
*/
int err_wst( INFILE *inpf )
{
    message( STYLE_WHITE_SPACE, inpf );
    errstr( "White space around tag" );
    errlf();

    /* always return 1; ishould change that to the */
    /*num of chars printed, but i'm too lazy.. */
    return ( 1 );
}

