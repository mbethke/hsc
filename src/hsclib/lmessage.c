/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * hsclib/message.c
 *
 * message functions for hsc
 *
 * updated: 10-Aug-1998
 * created: 10-Mar-1996
 *
 */

#define NOEXTERN_HSCLIB_MESSAGE_H

#include "hsclib/inc_base.h"

#include "ugly/returncd.h"

/* forward reference */
static VOID handle_too_many_messages(HSCPRC * hp);

/*
 * NOTE: see "hsclib/msgid.h" for message-id's and
 *       how a message-id is build.
 */

static VOID msg_tag(EXPSTR * msgstr, CONSTRPTR tagname)
{
    app_estr(msgstr, "tag <");
    app_estr(msgstr, tagname);
    app_estr(msgstr, ">");
}

static VOID msg_endtag(EXPSTR * msgstr, CONSTRPTR tagname)
{
    app_estr(msgstr, "end tag </");
    app_estr(msgstr, tagname);
    app_estr(msgstr, ">");
}

static VOID msg_attr(EXPSTR * msgstr, CONSTRPTR attrname)
{
    app_estr(msgstr, "attribute ");
    app_estr(msgstr, attrname);
}

static VOID msg_lazy(EXPSTR * msgstr, CONSTRPTR lazy_name)
{
    app_estr(msgstr, "var-list ");
    app_estr(msgstr, lazy_name);
}

static VOID msg_entity(EXPSTR * msgstr, CONSTRPTR entname)
{
    app_estr(msgstr, "entity `");
    app_estr(msgstr, entname);
    app_estr(msgstr, "'");
}

static VOID msg_idname(EXPSTR * msgstr, CONSTRPTR idname)
{
    app_estr(msgstr, "id ");
    app_estr(msgstr, "\"#");
    app_estr(msgstr, idname);
    app_estrch(msgstr, '"');
}

/*
 * hsc_message
 *
 * create message string and send it to call-back
 *
 * legal placeholders inside format:
 *  %A ptr to HSCATTR
 *  %a string for attribute-name
 *  %C ptr to HSCTAG, displayed as end-tag
 *  %c string to end-tag
 *  %d dezimal number (LONG)
 *  %E ptr to HSCENT
 *  %e string to entity-name
 *  %i string to id-name
 *  %j string to jerk/prostitute
 *  %L ptr to var-list
 *  %l string to var-list
 *  %q quoted string
 *  %s string
 *  %T ptr to HSCTAG
 *  %t string for tag-name
 *
 * example:
 * ---
 *  HSCTAG *mytag;
 *  STRPTR expected_tag = "dummy";
 *
 *  hsc_message( hp, MSG_my_tag_expected,
 *               "Expected tag %T insted of %t",
 *               mytag, expected_tag );
 * ---
 */

/* checks if filename starts with PARENT_FILE_ID */
static BOOL is_child_file(STRPTR filename)
{
    return ((BOOL) ! strncmp(filename, PARENT_FILE_ID,
                             strlen(PARENT_FILE_ID)));
}

/* decides, if a message should be ignored or display */
static BOOL really_display_message(HSCPRC * hp, HSCMSG_ID msg_id)
{
    HSCMSG_CLASS msg_class = hsc_get_msg_class(hp, msg_id);
    BOOL disp_msg = TRUE;       /* function result */
#if DEBUG
    HSCMSG_ID msg_id_unmasked = msg_id & MASK_MESSAGE;
#endif

    if (hp->fatal)
    {

        /* oppress all messages after fatal errors */
        disp_msg = FALSE;
    }
    else if (
                (hsc_get_msg_ignore(hp, msg_id) == ignore)
                &&
                (msg_class <= MSG_WARN)
        )
    {
        /* oppress message if it is marked as ignored
         * and it is no ERROR/FATAL message */
        D(fprintf(stderr, DHL "ignore msg#%ld: ignore enabled\n",
                  msg_id_unmasked));
        disp_msg = FALSE;
    }
    else if (((msg_class == MSG_NOTE) && (hp->msg_ignore_notes))
             || ((msg_class == MSG_STYLE) && (hp->msg_ignore_style))
             || ((msg_class == MSG_PORT) && (hp->msg_ignore_port))
        )
    {
        /* class should be ignored; however, if this message is set
         * to enable, still display it */
        if (hsc_get_msg_ignore(hp, msg_id) != enable)
        {
            /* suppress message if its class is
             * marked as to be ignored */
            D(fprintf(stderr, DHL "ignore msg#%ld: ignore whole class (%06lx)\n",
                      msg_id_unmasked, msg_class));
            disp_msg = FALSE;
        }
        else
        {
            D(fprintf(stderr, DHL "enable msg#%ld: only ignore whole class (%06lx)\n",
                      msg_id_unmasked, msg_class));
        }
    }

    return disp_msg;
}

VOID hsc_message(HSCPRC * hp, HSCMSG_ID msg_id, const char *format,...) {
    HSCMSG_CLASS msg_class = hsc_get_msg_class(hp, msg_id);
    HSCMSG_ID msg_id_unmasked = msg_id & MASK_MESSAGE;
    INFILE *msg_inpf = NULL;
    STRPTR msg_fname = "unknown";
    ULONG msg_x = 0;
    ULONG msg_y = 0;
    BOOL disp_msg = really_display_message(hp, msg_id);     /* display message? */

    if (disp_msg) {
        va_list ap;

        /* increase message-counter */
        hp->msg_count++;

        /* set fatal-flag, if this is a fatal message */
        if (msg_id > MSG_FATAL)
            hp->fatal = TRUE;

        /* clear message buffer */
        clr_estr(hp->curr_msg);

        /* create message string */
        va_start(ap, format);
        while (format[0]) {
            if (format[0] == '%') {
                STRPTR s = NULL;
                HSCTAG *tag = NULL;
                HSCTAG *lazy = NULL;
                HSCATTR *attr = NULL;
                HSCENT *ent = NULL;

                format++;
                switch (format[0]) {

                case 'd':
                    /*
                     * append decimal number
                     */
                    app_estr(hp->curr_msg,
                             long2str(va_arg(ap, LONG)));
                    break;

                case 'q':
                    /*
                     * append quoted string
                     */
                    s = va_arg(ap, STRPTR);

                    app_estrch(hp->curr_msg, '`');
                    while (s[0]) {
                        switch (s[0]) {

                        case '\n':
                            app_estr(hp->curr_msg, "\\n");
                            break;
                        case '\"':
                            app_estr(hp->curr_msg, "\\\"");
                            break;
                        default:
                            if (((unsigned char) s[0]) < ' ') {
                                app_estrch(hp->curr_msg, '\\');
                                app_estr(hp->curr_msg,
                                         long2str((LONG) s[0]));
                                app_estrch(hp->curr_msg, ';');
                            } else
                                app_estrch(hp->curr_msg, s[0]);
                        }
                        s++;    /* process next char */
                    }
                    app_estrch(hp->curr_msg, '\'');

                    break;

                case 's':
                    /*
                     * append simple string
                     */
                    app_estr(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'T':
                    /* append tag-pointer */
                    tag = va_arg(ap, HSCTAG *);
                    msg_tag(hp->curr_msg, tag->name);
                    break;

                case 't':
                    /* append tag */
                    msg_tag(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'C':
                    /* append end tag-pointer */
                    tag = va_arg(ap, HSCTAG *);
                    msg_endtag(hp->curr_msg, tag->name);
                    break;

                case 'c':
                    /* append end tag */
                    msg_endtag(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'A':
                    /* append attribute-pointer */
                    attr = va_arg(ap, HSCATTR *);
                    msg_attr(hp->curr_msg, attr->name);
                    break;

                case 'a':
                    /* append attribute */
                    msg_attr(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'E':
                    /* append entity-pointer */
                    ent = va_arg(ap, HSCENT *);
                    msg_entity(hp->curr_msg, ent->name);
                    break;

                case 'e':
                    /* append entity */
                    msg_entity(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'i':
                    /* append ID */
                    msg_idname(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                case 'j':
                    /* append jerk/prostitute */
                    if (hp->prostitute)
                        app_estr(hp->curr_msg, "prostitutes");
                    else
                        app_estr(hp->curr_msg, "jerks");
                    break;

                case 'L':
                    /* append var-list-pointer */
                    lazy = va_arg(ap, HSCTAG *);
                    msg_lazy(hp->curr_msg, lazy->name);
                    break;

                case 'l':
                    /* append var-list */
                    msg_lazy(hp->curr_msg, va_arg(ap, STRPTR));
                    break;

                default:
                    /*
                     * append unknown
                     */
                    app_estrch(hp->curr_msg, '%');
                    if (format[0] && (format[0] != '%')) {
                        app_estrch(hp->curr_msg, '%');
                        format--;
                    }
                    break;
                }
            }
            else
                app_estrch(hp->curr_msg, format[0]);

            if (format[0])
                format++;
        }
        va_end(ap);

        /* evaluate message position */
        if (hp->inpf) {
            msg_inpf = hp->inpf;

            msg_fname = infget_fname(msg_inpf);

            /* use parent file for position? */
            if (is_child_file(msg_fname)) {
                DLNODE *nd = dll_first(hp->inpf_stack);

                msg_inpf = NULL;
                msg_fname = NULL;

                if (nd)
                    do {
                        D(fprintf(stderr, DHL "skip parent file `%s'\n", msg_fname));

                        /* use position of file on stack */
                        msg_inpf = (INFILE *) dln_data(nd);
                        msg_fname = infget_fname(msg_inpf);
                        nd = dln_next(nd);
                    } while (nd && is_child_file(msg_fname));
            }

            if (msg_inpf) {
                msg_x = infget_wx(msg_inpf) + 1;
                msg_y = infget_wy(msg_inpf) + 1;
            } else {
                msg_fname = "hsc-internal.hsc";
                msg_x = 0;
                msg_y = 0;
            }
        } else {
            msg_fname = NULL;
            msg_x = 0;
            msg_y = 0;
        }

        /* send message via callback */
        if (hp->CB_message)
            (*(hp->CB_message))
                (hp,
                 msg_class, msg_id_unmasked,
                 msg_fname, msg_x, msg_y,
                 estr2str(hp->curr_msg)
                );

        /* process nested files */
        if (hp->CB_message_ref && hp->nested_errors) {
            DLNODE *nd = dll_first(hp->inpf_stack);

            while (nd) {
                msg_inpf = dln_data(nd);
                msg_fname = infget_fname(msg_inpf);
                msg_x = infget_wx(msg_inpf) + 1;
                msg_y = infget_wy(msg_inpf) + 1;

                (*(hp->CB_message_ref))
                    (hp,
                     msg_class, msg_id_unmasked,
                     msg_fname, msg_x, msg_y,
                     ""
                    );

                nd = dln_next(nd);
            }
        }

        /* check, if already too many messages or errors have
         * occured and abort process in case */
        handle_too_many_messages(hp);
    } else
        D(fprintf(stderr, DHL "suppressed msg#%ld\n", msg_id_unmasked));
}

/* check if there are already too many errors and view an
 * fatal error "too many messages" */
static VOID handle_too_many_messages(HSCPRC * hp)
{
    if (hp->max_errors != MAXIMUM_MESSAGE_INFINITE)
    {
        hp->max_errors -= 1;
    }
    if (hp->max_messages != MAXIMUM_MESSAGE_INFINITE)
    {
        hp->max_messages -= 1;
    }

    if ((hp->max_messages == 0) || (hp->max_errors == 0))
    {
        hsc_message(hp, MSG_TOO_MANY, "too many errors or messages");
    }
}

/*
 *-------------------------------------
 * often occurable errors & messages
 *-------------------------------------
 */

VOID hsc_msg_eof(HSCPRC * hp, STRPTR descr)
{
    STRPTR eoftxt = "unexpected end of context";

    if (descr)
    {
        hsc_message(hp, MSG_UNEX_EOF, "%s (%s)", eoftxt, descr);
    }
    else
    {
        hsc_message(hp, MSG_UNEX_EOF, "%s", eoftxt);
    }
}

VOID hsc_msg_illg_whtspc(HSCPRC * hp)
{
    hsc_message(hp, MSG_ILLG_WHTSPC, "illegal white space");
}

VOID hsc_msg_stripped_tag(HSCPRC * hp, HSCTAG * tag, STRPTR why)
{
    if (why)
        hsc_message(hp, MSG_TAG_STRIPPED,
                    "stripped %T (%s)", tag, why);
    else
        hsc_message(hp, MSG_TAG_STRIPPED,
                    "stripped %T", tag);
}

VOID hsc_msg_unkn_attr_ref(HSCPRC * hp, STRPTR attr)
{
    hsc_message(hp, MSG_UNKN_ATTR_REF,
                "unknown %a", attr);
}

VOID hsc_msg_unkn_attr_tag(HSCPRC * hp, STRPTR attr, STRPTR tag)
{
    hsc_message(hp, MSG_UNKN_ATTR_TAG,
                "unknown %a for %t", attr, tag);
}

VOID hsc_msg_unkn_attr_macro(HSCPRC * hp, STRPTR attr, STRPTR macro)
{
    hsc_message(hp, MSG_UNKN_ATTR_MACRO,
                "unknown %a for %t", attr, macro);
}

VOID hsc_msg_noinput(HSCPRC * hp, STRPTR filename)
{
    hsc_message(hp, MSG_NO_INPUT,
                "can not open %q for input: %s",
                filename, strerror(errno));
}

VOID hsc_msg_read_error(HSCPRC * hp, STRPTR filename)
{
    hsc_message(hp, MSG_READ_ERROR,
                "error reading %s: %s",
                filename, strerror(errno));
}

VOID hsc_msg_nouri(HSCPRC * hp, STRPTR filename, STRPTR uriname, STRPTR note)
{
    if (note)
    {
        hsc_message(hp, MSG_NO_URIPATH,
                    "file %q for URI %q not found (%s)",
                    filename, uriname, note);
    }
    else
    {
        hsc_message(hp, MSG_NO_URIPATH,
                    "file %q for URI %q not found",
                    filename, uriname);
    }
}

