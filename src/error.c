/*
** error.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** error vars & funs for hsc
**
** updated: 14-Dec-1995
** created:  9-Jul-1995
**
** TODO: programable desription string to format error messages
** TODO: redirect error output to error-file
**
** NOTE: see "msgid.h" for message-id's and
**       how a message-id is build.
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "ugly/types.h"
#include "ugly/outfile.h"
#include "ugly/string.h"

#include "global.h"
#define NOEXTERN_HSC_ERROR
#include "error.h"
#include "msgid.h"


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

/* return code for shell */
/* (modified by msg_prt() */
int return_code = RC_OK;

BOOL ignore_msg[ MAX_MSGID ]; /* flag: ignore message */
BOOL error_msg[ MAX_MSGID ];  /* flag: treat message as error */

/*
**-------------------------------------
** ignore warnings
**-------------------------------------
*/
VOID clr_ignore_msg( VOID )
{
    LONG i;

    for ( i=0; i<MAX_MSGID; i++ )
        disable_ignore( i );
}

VOID clr_error_msg( VOID )
{
    LONG i;

    for ( i=0; i<MAX_MSGID; i++ )
        /*TODO: disbale_error( i )*/;
}

/*
**-------------------------------------
** basic output to error file
**-------------------------------------
*/

/*
** errch
*/
int errch( char ch )
{
    if ( display_message )
        return( fputc( ch, errfile ) );
    else
        return( 0 );
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
    if ( display_message )
        return( fputs( str, errfile ) );
    else
        return( 0 );
}

/*
** errqstr
*/
int errqstr( CONSTRPTR str )
{
    int ctr = 2;

    errch( '\"' );

    while ( str[0] ) {

        switch ( str[0]) {

            case '\n':
                errstr( "\n" );
                break;
            case '\"':
                errstr( "\\\"" );
                break;
            default:
                if ( str[0] < ' ' ) {

                    errch ( '\\' );
                    errstr( long2str( (LONG) str[0] ) );
                    errch ( ';' );

                } else
                    errch( str[0] );

        }
        str++; /* process next char */
        ctr++;
    }

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
** errsym: print attribute name
*/
int errsym( CONSTRPTR str )
{
    return ( errstr( " attribute " ) + errstr( str ) + errstr( " " ) );
}


/*
** errlong
*/
int errlong( ULONG num )
{
    return ( errstr(long2str(num)) );
}

/*
**-------------------------------------
** open/close error file
**-------------------------------------
*/


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

        } else if ( verbose )
            fprintf( stderr, "Opened \"%s\" as error file\n", errfilename );

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
**-------------------------------------
** more complex output to error file
**-------------------------------------
*/

/*
** msg_ptr
*/
int msg_prt( ULONG id, STRPTR s, INFILE *f )
{
    int ctr=0;

    /*
    ** update return code
    */
    if ( (id>WARN) && (return_code<RC_WARN) )
        return_code = RC_WARN;
    else if ( (id>ERROR) && (return_code<RC_ERROR) )
        return_code = RC_ERROR;
    else if ( (id>FATAL) && (return_code<RC_FATAL) )
        return_code = RC_FATAL;

    /*
    ** display file info
    */
    if ( f ) {
        if ( strcmp( FNAME_STDIN, infget_fname(f) ) ) {

            ctr += errqstr( infget_fname(f) );
            ctr += errstr( " (" );
            ctr += errlong( infget_y(f) );
            ctr += errstr( "," );
            ctr += errlong( infget_x(f)-1 ); /* TODO: why "-1"? */
            ctr += errstr( ") " );

        }
    }

    /*
    ** display message id (eg "Error: ")
    */
    if (s) {
        ctr += errstr( s );
        ctr += errstr( " " );
        ctr += errlong( id & MASK_MESSAGE );
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
** cmp_msg: find a message in a message list
**          (find-methode for find_dlnode())
**
** (message lists exist for ignore/error/fatal massage
** and can be set by the user. see "args.c")
*/
int cmp_msg( APTR data1, APTR data2 )
{
    if (data1 && data2)
        if ( (ULONG) data1 == (ULONG) data2 )
            return (1);
        else
            return (0);
    else
        return (0);
}


/*
** message
*/
int message( ULONG id, INFILE *f )
{
    int ctr = 0;

    /* oppress message if an fatal error occured before */
    display_message = ( !(fatal_error) );

    /* check if message is in ignore list */
    if ( display_message && ignore( id ) ) {

        display_message = FALSE;
        D( fprintf( stderr, "** oppressed message #%d (0x%x)\n",
                            id & MASK_MESSAGE, id ) );

    }

    if ( display_message ) {

        if ( id>=FATAL )    {

            ctr = msg_prt( id, "Fatal error", f );
            fatal_error = TRUE;

        } else if ( id>=ERROR ) ctr = msg_prt( id, "Error", f );
        else if ( id>=WARN )  ctr = msg_prt( id, "Warning", f );
        else if ( id>=STYLE ) ctr = msg_prt( id, "Bad style", f );
        else if ( id>=MSG )   ctr = msg_prt( id, "Note", f );

    } else if ( debug )
        /* in debug mode, display a dot ('.') for every oppressed message */
        fputc( '#', errfile );


    return (ctr);

}


/*
**-------------------------------------
** often occurable errors
**-------------------------------------
*/


VOID err_eof( INFILE *inpf, STRPTR descr )
{
    message( MSG_UNEX_EOF, inpf );

    errstr( "unexpected end of file" );
    if ( descr ) {

        errstr( " (" );
        errstr( descr );
        errstr( ")" );

    }
    errlf();
}

VOID err_eol( INFILE *inpf )
{
    message( MSG_UNEX_EOL, inpf );
    errstr( "unexpected end of line" );
    errlf();
}

VOID err_streol( INFILE *inpf )
{
    message( MSG_STR_EOL, inpf );
    errstr( "string exeeds line" );
    errlf();
}

#if 0
VOID err_mem( INFILE *inpf ) /* TODO: remove this */
{
    message( MSG_NO_MEM, inpf );
    errstr( "out of memory" );
    errlf();
}
#endif

VOID err_write( OUTFILE *outf )
{
    STRPTR outfnm = outfilename;

    if ( outf )
        outfnm = outfget_fname( outf );

    message( MSG_WRITE_ERR, NULL );
    errstr( "error writing to " );
    errqstr( outfnm );
    errstr( ": " );
    errstr( strerror( errno ) );
    errlf();

    errno = NULL;
}

VOID err_longstr( INFILE *inpf )
{
    message( FATAL_LONG_STR, inpf );
    errstr( "maximum string length exceeded (Thank K&R)" );
    errlf();
}

VOID err_illgwspc( INFILE *inpf )
{
    message( MSG_ILLG_WHTSPC, inpf );
    errstr( "illegal white-space" );
    errlf();

}



/*
** err_wst
**
** bad style: white space around tag
**
*/
VOID err_wst( INFILE *inpf )
{
    message( MSG_WSPC_AROUND_TAG, inpf );
    errstr( "White space around tag" );
    errlf();
}

/*
** call_panic
**
** panic message; use "panic()"-macro
*/
void call_panic( STRPTR text, STRPTR file, ULONG line )
{
    fprintf( stderr, "\n##\n## illegal state in \"%s\" (%d): %s\n##\n",
             file, line, text );
}

/*
**-------------------------------------
** nomem-handler
**-------------------------------------
*/
BOOL hsc_nomem_handler( size_t size )
{
    message( MSG_NO_MEM, NULL );
    errstr( "out of memory" );
    errlf();

    return( FALSE ); /* immediatly abort */
}


