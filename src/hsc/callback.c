/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 */
/*
 * hsc/callback.c
 *
 * callbacks of hsc for hsclib
 *
 * updated:  9-Sep-1996
 * created: 17-Mar-1996
 *
 */

#include "hsc/global.h"
#include "hsc/output.h"
#include "hsc/status.h"

#include "ugly/returncd.h"

#define NOEXTERN_HSC_CALLBACK_H
#include "hsc/callback.h"


/*
 * defines for ANSI-sequences
 */
#define ANSI_RESET     "\033[0m"
#define ANSI_BOLD      "\033[1m"
#define ANSI_ITALIC    "\033[3m"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_INVERT    "\033[7m"

#define ANSI_TEXT_BACKGR "\033[30m"
#define ANSI_TEXT_SHADOW "\033[31m"
#define ANSI_TEXT_HILITE "\033[32m"
#define ANSI_TEXT_STRESS "\033[31m"


static FILE *msgfile = NULL;

/*
 * hsc_nomem_handler
 *
 * called from ugly/umemory/umalloc(), if malloc() did return NULL
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
    STRPTR msg_class_seq = ANSI_BOLD;
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
            msg_class_seq = ANSI_BOLD ANSI_TEXT_HILITE;
            break;
        case MSG_FATAL:
            msg_class_str = "Fatal error";
            msg_class_seq = ANSI_BOLD ANSI_TEXT_HILITE;
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
                    if (msg_ansi)
                        app_estr(msgbuf, msg_class_seq);
                    app_estr( msgbuf, msg_class_str );
                    if (msg_ansi)
                        app_estr(msgbuf, ANSI_RESET);
                    break;
                case 'f':
                    app_estr( msgbuf, fname );
                    break;
                case 'i':
                    if (msg_ansi)
                        app_estr(msgbuf, msg_class_seq);
                    app_estr( msgbuf, long2str( msg_id ) );
                    if (msg_ansi)
                        app_estr(msgbuf, ANSI_RESET);
                    break;
                case 'm':
                    app_estr( msgbuf, msg_text );
                    break;
                case 'n':
                    app_estrch( msgbuf, '\n' );
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
            fprintf( stderr, DHSC "msg `%s (%ld,%ld): %s %ld: %s'\n",
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
     HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
     STRPTR fname, ULONG x, ULONG y,
     STRPTR msg_text)
{
    if ( msg_text && msg_text[0] )
        message( hp, msg_class, msg_id, fname, x, y, msg_text );
    else
        message( hp, msg_class, msg_id, fname, x, y,
                 "(location of previous call)");
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
    hsc_set_status_file_begin( hp, status_file_begin );
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
        D( fprintf( stderr, DHSC "write msg to `%s'\n", fname ) );

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
        D( fprintf( stderr, DHSC "write msg to <stderr>\n" ) );
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
