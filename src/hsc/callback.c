/*
 * hsc/callback.c
 *
 * callbacks of hsc for hsclib
 *
 * Copyright (C) 1995,96  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * updated: 15-May-1996
 * created: 17-Mar-1996
 *
 */

#include "hsc/global.h"
#include "hsc/output.h"
#include "hsc/status.h"

#include "ugly/returncode.h"

#define NOEXTERN_HSC_CALLBACK_H
#include "hsc/callback.h"

static FILE *msgfile = NULL;

/*
 * hsc_nomem_handler
 *
 * called from ugly/umalloc, if malloc() did return NULL
 */
BOOL hsc_nomem_handler( size_t size )
{
    status_error( "out of memory" );

    return_code = RC_FAIL;

    exit( return_code );

    return( FALSE ); /* immediatly abort */
}



/*
 * message
 *
 * hsc-callback to display message coming from parser
 */
static VOID message(HSCPRC *hp,
     HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
     STRPTR fname, ULONG x, ULONG y,
     STRPTR msg_text)
{
    STRPTR msg_class_str = "*UNKNOWN*";
    STRPTR msgfmt = msg_format;

    if  ( !msgfmt )
#if 1
        msgfmt = "%f (%y,%x): %c %i: %m"; /* hsc format */
#else
        msgfmt = "%f:%y: %c %i: %m";      /* 'gcc' format */
#endif

    switch ( msg_class ) {

        case MSG_NOTE:
            msg_class_str = "Note";
            break;
        case MSG_STYLE:
            msg_class_str = "Bad style";
            break;
        case MSG_PORT:
            msg_class_str = "Portability problem";
            break;
        case MSG_WARN:
            msg_class_str = "Warning";
            break;
        case MSG_ERROR:
            msg_class_str = "Error";
            break;
        case MSG_FATAL:
            msg_class_str = "Fatal error";
            break;

    }

    /* update returncode if necessary */
    if ( msg_class == MSG_WARN )
        set_return_code( RC_WARN );
    else if ( msg_class == MSG_ERROR )
        set_return_code( RC_ERROR );
    else if ( msg_class == MSG_FATAL )
        set_return_code( RC_FAIL );

    /* couldn't open include file */
    if ( !fname )
    {
        fname = estr2str( inpfilename );
        x = 1; y = 1;
    }

    clr_estr( msgbuf );
    while ( msgfmt[0] )
    {
        if ( msgfmt[0]=='%' )
        {
            msgfmt++;
            switch ( msgfmt[0] )
            {
                case 'c':
                    app_estr( msgbuf, msg_class_str );
                    break;
                case 'f':
                    app_estr( msgbuf, fname );
                    break;
                case 'i':
                    app_estr( msgbuf, long2str( msg_id ) );
                    break;
                case 'm':
                    app_estr( msgbuf, msg_text );
                    break;
                case 'x':
                    app_estr( msgbuf, long2str( x ) );
                    break;
                case 'y':
                    app_estr( msgbuf, long2str( y ) );
                    break;
                default :
                    app_estrch( msgbuf, '%' );
                    app_estrch( msgbuf, msgfmt[0] );
                    break;
            }
        }
        else
        {
            app_estrch( msgbuf, msgfmt[0] );
        }
        msgfmt++;
    }
    app_estrch( msgbuf, '\n' ); /* append LF */
    fputs( estr2str( msgbuf ), msgfile );

    D(
        if ( msgfile != stderr )
            fprintf( stderr, DHP "msg `%s (%ld,%ld): %s %ld: %s'\n",
                     fname, y, x,
                     msg_class_str, msg_id, msg_text )
    );

}

/*
 * message_ref
 *
 * hsc-callback to display ref-message coming from parser
 */
static VOID message_ref(HSCPRC *hp,
     STRPTR fname, ULONG x, ULONG y,
     STRPTR msg_text)
{
    if ( !fname )
        fname = estr2str( inpfilename );
    fprintf( stderr, "%s (%ld,%ld): location of call\n",
             fname, y, x );
}

static VOID do_hsc_id( HSCPRC *hp, HSCATTR *attr, STRPTR id )
{
#if 0
    fprintf( stderr, "new id   : `%s' for attr `%s'\n",
             id, attr->name );
#endif
}

/*
 * do_hsc_start_tag
 *
 * hsc-callback for processing start tag
 */
static VOID do_hsc_start_tag( HSCPRC *hp, HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close )
{
#if 0
    if ( tag )
        fprintf( stderr, "start tag: <%s>\n", tag->name );
    else
        fprintf( stderr, "start tag: <UNKNOWN>\n" );
#endif
    append_output( tag_name );
    append_output( tag_attr );
    append_output( tag_close );
}

static VOID do_hsc_end_tag( HSCPRC *hp, HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close )
{
#if 0
    if ( tag )
        fprintf( stderr, "end tag  : </%s>\n", tag->name );
    else
        fprintf( stderr, "start tag: <UNKNOWN>\n" );
#endif
    append_output( tag_name );
    append_output( tag_attr );
    append_output( tag_close );
}

static VOID do_hsc_text( HSCPRC *hp, STRPTR whtspc, STRPTR text )
{
#if 0
    fprintf( stderr, "text     : \"%s\", \"%s\"\n", whtspc, text );
#endif
    append_output( whtspc );
    append_output( text );
}

/*
 * init_callback
 *
 * assign hsc's callbacks to a hsc-process
 */
BOOL init_callback( HSCPRC *hp )
{
    /* status-messages */
    hsc_set_status_file_end( hp, status_file_end );
    hsc_set_status_line( hp, status_line );
    hsc_set_status_misc( hp, status_misc );

    /* messages */
    hsc_set_message( hp, message );
    hsc_set_message_ref( hp, message_ref );

    /* processing tags & text */
    hsc_set_start_tag( hp, do_hsc_start_tag );
    hsc_set_end_tag( hp, do_hsc_end_tag );
    hsc_set_id( hp, do_hsc_id );
    hsc_set_text( hp, do_hsc_text );

    return( TRUE );
}

/*
 * message file functiuons
 */

/*
 * init_msg_file
 *
 * open messagefile or assign it to stdderr
 */
BOOL init_msgfile( HSCPRC *hp, STRPTR fname )
{
    BOOL ok = TRUE;

    if ( fname )
    {
        D( fprintf( stderr, DHP "write msg to `%s'\n", fname ) );

        errno = 0;
        msgfile = fopen( fname, "w" );
        if ( !msgfile )
        {
#define MSGBUFLEN 79
            STRARR msgbuf[ MSGBUFLEN+1 ];

            strncpy( msgbuf, "unable to open message file `", MSGBUFLEN );
            strncat( msgbuf, fname , MSGBUFLEN - strlen(msgbuf) );
            strncat( msgbuf, "': " , MSGBUFLEN - strlen(msgbuf) );
            strncat( msgbuf, strerror( errno ) , MSGBUFLEN - strlen(msgbuf) );
            status_error( msgbuf );
            ok = FALSE;
        }
    }
    else
    {
        msgfile = stderr;
        D( fprintf( stderr, DHP "write msg to <stderr>\n" ) );
    }

    return( ok );
}

/*
 * cleanup_msg_file
 *
 * close message file if neccessary
 */
VOID cleanup_msgfile( VOID )
{
    if ( msgfile && (msgfile != stderr) )
        fclose( msgfile );
}
