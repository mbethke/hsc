/*
** hsclib/message.c
**
** message functions for hsc
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
** updated: 20-Apr-1996
** created: 10-Mar-1996
**
** NOTE: see "hsclib/msgid.h" for message-id's and
**       how a message-id is build.
*/

#define NOEXTERN_HSCLIB_MESSAGE_H

#include "hsclib/inc_base.h"

static VOID msg_tag( EXPSTR *msgstr, CONSTRPTR tagname )
{
    app_estr( msgstr, "tag <" );
    app_estr( msgstr, tagname );
    app_estr( msgstr, ">" );
}

static VOID msg_endtag( EXPSTR *msgstr, CONSTRPTR tagname )
{
    app_estr( msgstr, "end tag </" );
    app_estr( msgstr, tagname );
    app_estr( msgstr, ">" );
}

static VOID msg_attr( EXPSTR *msgstr, CONSTRPTR attrname )
{
    app_estr( msgstr, "attribute " );
    app_estr( msgstr, attrname );
}

static VOID msg_entity( EXPSTR *msgstr, CONSTRPTR entname )
{
    app_estr( msgstr, "entity " );
    app_estr( msgstr, entname );
}

static VOID msg_idname( EXPSTR *msgstr, CONSTRPTR idname )
{
    app_estr( msgstr, "id " );
    app_estr( msgstr, idname );
}

/*
** hsc_message
**
** create message string and send it to call-back
**
** legal placeholders inside format:
**  %A ptr to HSCATTR
**  %a string for attribute-name
**  %d dezimal number (LONG)
**  %E ptr to HSCENT
**  %e string to entity-name
**  %i string to id-name
**  %q quoted string
**  %s string
**  %T ptr to HSCTAG
**  %t string for tag-name
**
** example:
** ---
**  HSCTAG *mytag;
**  STRPTR expected_tag = "dummy";
**
**  hsc_message( hp, MSG_my_tag_expected,
**               "Expected tag %T insted of %t",
**               mytag, expected_tag );
** ---
*/
VOID hsc_message( HSCPRC *hp, HSCMSG_ID msg_id, const char *format, ... )
{
    HSCMSG_CLASS msg_class = msg_id & MASK_MSG_CLASS;
    STRPTR       msg_fname = "unknown";
    ULONG        msg_x     = 0;
    ULONG        msg_y     = 0;
    BOOL         disp_msg  = TRUE; /* flag, if message really */
                                   /* should be displayed */
    if ( hp->fatal ) {

        /* oppress all messages after fatal errors */
        disp_msg = FALSE;

    } else if (
             ( hsc_get_msg_ignore( hp, msg_id ) )
             &&
             ( hsc_get_msg_class( hp, msg_id ) <= MSG_WARN )
            )
    {
        /* oppress message if it is marked as ignored
        ** and it is no ERROR/FATAL message
        */
        disp_msg = FALSE;
    }

    if ( disp_msg ) {

        va_list ap;

        /* set fatal-flag, if this is a fatal message */
        if ( msg_id > MSG_FATAL )
            hp->fatal = TRUE;

        /* clear message buffer */
        clr_estr( hp->curr_msg );

        /* create message string */
        va_start( ap, format );
        while ( format[0] ) {

            if ( format[0] == '%' ) {

                STRPTR   s    = NULL;
                HSCTAG  *tag  = NULL;
                HSCATTR *attr = NULL;
                HSCENT  *ent  = NULL;

                format++;
                switch ( format[0] ) {

                    case 'd':
                        /*
                        ** append decimal number
                        */
                        app_estr( hp->curr_msg,
                                 long2str( va_arg( ap, LONG ) ) );
                        break;

                    case 'q':
                        /*
                        ** append quoted string
                        */
                        s = va_arg( ap, STRPTR );

                        app_estrch( hp->curr_msg, '`' );
                        while ( s[0] ) {

                            switch ( s[0]) {

                                case '\n':
                                    app_estr( hp->curr_msg, "\\n" );
                                    break;
                                case '\"':
                                    app_estr( hp->curr_msg, "\\\"" );
                                    break;
                                default:
                                    if ( s[0] < ' ' ) {

                                        app_estrch( hp->curr_msg, '\\' );
                                        app_estr( hp->curr_msg,
                                                  long2str( (LONG) s[0] ) );
                                        app_estrch( hp->curr_msg, ';' );

                                    } else
                                        app_estrch( hp->curr_msg, s[0] );

                            }
                            s++; /* process next char */
                        }
                        app_estrch( hp->curr_msg, '\'' );

                        break;

                    case 's':
                        /*
                        ** append simple string
                        */
                        app_estr( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    case 'T':
                        /* append tag-pointer */
                        tag = va_arg( ap, HSCTAG *);
                        msg_tag( hp->curr_msg, tag->name );
                        break;

                    case 't':
                        /* append tag */
                        msg_tag( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    case 'C':
                        /* append end tag-pointer */
                        tag = va_arg( ap, HSCTAG *);
                        msg_endtag( hp->curr_msg, tag->name );
                        break;

                    case 'c':
                        /* append end tag */
                        msg_endtag( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    case 'A':
                        /* append attribute-pointer */
                        attr = va_arg( ap, HSCATTR *);
                        msg_attr( hp->curr_msg, attr->name );
                        break;

                    case 'a':
                        /* append attribute */
                        msg_attr( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    case 'E':
                        /* append entity-pointer */
                        ent = va_arg( ap, HSCENT *);
                        msg_entity( hp->curr_msg, ent->name );
                        break;

                    case 'e':
                        /* append entity */
                        msg_entity( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    case 'i':
                        /* append entity */
                        msg_idname( hp->curr_msg, va_arg( ap, STRPTR ) );
                        break;

                    default:
                        /*
                        ** append unknown
                        */
                        app_estrch( hp->curr_msg, '%' );
                        if ( format[0] && ( format[0] != '%') ) {
                            app_estrch( hp->curr_msg, '%' );
                            format--;
                        }
                        break;

                }
            } else
                app_estrch( hp->curr_msg, format[0] );

            if ( format[0] )
                format++;

        }
        va_end( format );

        /* evaluate message position */
        if ( hp->inpf )
        {
            msg_fname = infget_fname( hp->inpf );
            msg_x     = infget_wx( hp->inpf )+1;
            msg_y     = infget_wy( hp->inpf )+1;
        }
        else
        {
            msg_fname = NULL;
            msg_x     = 0;
            msg_y     = 0;
        }
        /* send message via callback */
        if ( hp->CB_message )

            (*(hp->CB_message))
                ( hp,
                  msg_class,
                  msg_id & MASK_MESSAGE,
                  msg_fname, msg_x, msg_y,
                  estr2str( hp->curr_msg )
                );

    } else {
        D( fprintf( stderr, DHL "suppressed msg#%ld\n", msg_id & MASK_MESSAGE ) );
    }
}

/*
**-------------------------------------
** often occurable errors & messages
**-------------------------------------
*/

VOID hsc_msg_eof( HSCPRC *hp, STRPTR descr )
{
    STRPTR eoftxt = "unexpected end of file";

    if ( descr )
        hsc_message( hp, MSG_UNEX_EOF, "%s (%s)", eoftxt, descr );
    else
        hsc_message( hp, MSG_UNEX_EOF, "%s", eoftxt );
}

VOID hsc_msg_illg_whtspc( HSCPRC *hp )
{
    hsc_message( hp, MSG_UNEX_EOF, "illegal white-space" );
}

VOID hsc_msg_stripped_tag( HSCPRC *hp, HSCTAG *tag, STRPTR why )
{
    if ( why )
        hsc_message( hp, MSG_TAG_STRIPPED,
                     "stripped tag %T (%s)", tag, why );
    else
        hsc_message( hp, MSG_TAG_STRIPPED,
                     "stripped tag %T", tag );
}

VOID hsc_msg_unkn_attr( HSCPRC *hp, STRPTR attr )
{
    hsc_message( hp, MSG_UNKN_ATTR,
                 "unknown %a", attr );
}

VOID hsc_msg_eol( HSCPRC *hp )
{
    hsc_message( hp, MSG_UNEX_EOL,
                 "unexpected end of line" );
}


#if 0
VOID err_eol( INFILE *inpf )
{
    message( MSG_UNEX_EOL, inpf );
    errstr( "unexpected end of line" );
}

/*
** err_unkn_attr: reference to unknown attribute
*/
VOID err_unkn_attr( INFILE *inpf, STRPTR attrname )
{
    message( MSG_UNKN_ATTR, inpf );
    errstr( "unknown" );
    errsym( attrname );
}

#endif
