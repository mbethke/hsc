/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hsc/status.c
 *
 * status message functions for hsc
 *
 * updated: 30-Sep-1997
 * created: 30-Jul-1995
 *
 */

/*
 * local includes
 */
#include "hsc/global.h"

#define NOEXTERN_HSC_STATUS
#include "hsc/status.h"

#include "ugly/returncd.h"

LONG disp_status = STATUS_LINE; /* mode for status message */

static size_t prev_stmsg_len = 0;       /* length of previos status message */
static ULONG prev_status_line = 0;      /* previous line num in input file */

#define ST_LINE_QUANTUM 25      /* num of lines that have to be processed */
                            /* before a status message is printed */

#define MAX_STATUSLEN 160
static STRARR status_buf[MAX_STATUSLEN + 2];    /* buffer for status messages */

/*
 * set_return_code
 *
 * Set new value for return-code of program. If the old retcode
 * is greater than the new one, nothing is changed.
 */
VOID set_return_code(int newrc)
{
    if (newrc > return_code)
    {
#if DEBUG_HSCPRE_RETCODE
        fprintf(stderr, DHSC "returncode: set to %d\n", return_code);
#endif
        return_code = newrc;
    }
#if DEBUG_HSCPRE_RETCODE
    else
    {
        fprintf(stderr, DHSC "returncode: NOT set to %d (is: %d)\n",
                newrc, return_code);
    }
#endif
}

/*
 * status_msg
 *
 * display a status message
 *
 * params: s.....message to display
 */
VOID status_msg(STRPTR s)
{
    size_t new_stmsg_len = strlen(s);
    size_t i;

    if (disp_status)
    {
        /* display message */
        if (s[0])
            fputs(s, stderr);

        /* clear tail of old message */
        for (i = new_stmsg_len; i < prev_stmsg_len; i++)
            fputs(" ", stderr);

        /* perform cr & flush stdder */
        fputs("\r", stderr);
        fflush(stderr);

        prev_stmsg_len = new_stmsg_len;
    }
}

/*
 * status_clear
 *
 * clear status message
 */
VOID status_clear(VOID)
{
    status_msg("");
}

/*
 * status_file_and_line
 *
 * copy status message for current file & line
 * processing into status_buf[]
 *
 * NOTE: messages >79 chars are truncated
 */
static VOID status_file_and_line(HSCPRC * hp)
{
    STRPTR filename = hsc_get_file_name(hp);

    if (filename)
    {
        /* create status-string */
        /* NOTE: this is not done via sprintf(), because
         *   no check for a too long string would be done */
        strncpy(status_buf, filename, MAX_STATUSLEN);
        strncat(status_buf, " (", MAX_STATUSLEN - strlen(status_buf));
        strncat(status_buf, long2str(hsc_get_file_line(hp)),
                MAX_STATUSLEN - strlen(status_buf));
        strncat(status_buf, ")", MAX_STATUSLEN - strlen(status_buf));
    }
    else
    {
        strcpy(status_buf, "");
    }

    status_msg(status_buf);
}

/*
 * status_lf
 *
 * perform a linfeed with status messages
 * (old message will stay visible)
 */
VOID status_lf(VOID)
{
    if (disp_status)
    {
        fputs("\n", stderr);
        prev_status_line = 0;
    }
}

/*
 * status_file_end
 *
 * hsc-callback for file fully processed: display
 * file and total number of line, perform liefeed
 */
VOID status_file_begin(HSCPRC * hp, STRPTR filename)
{
    if (filename)
    {
        /* create status-string: "<filename> (reading)" */
        /* NOTE: this is not done via sprintf(), because
         *   no check for a too long string would be done */
        strncpy(status_buf, filename, MAX_STATUSLEN);
        strncat(status_buf, " (reading)", MAX_STATUSLEN - strlen(status_buf));
        status_msg(status_buf);
    }
}

/*
 * status_file_end
 *
 * hsc-callback for file fully processed: display
 * file and total number of line, perform liefeed
 */
VOID status_file_end(HSCPRC * hp)
{
    status_file_and_line(hp);
    status_msg(status_buf);
    status_lf();
}

/*
 * status_line
 */
VOID status_line(HSCPRC * hp)
{
    if (disp_status_line
#if 1
    /* set this to '0' to see every line displayed in status */
        && ((hsc_get_file_line(hp) - prev_status_line) > ST_LINE_QUANTUM)
#endif
        )
    {
        status_file_and_line(hp);
        status_msg(status_buf);
        prev_status_line = hsc_get_file_line(hp);
    }
}

/*
 * status_misc
 *
 * display misc. status messages
 */
VOID status_misc(HSCPRC * hp, STRPTR s)
{
    if (disp_status_verbose)
    {
        strcpy(status_buf, "");
#if 0
        status_file_and_line(hp);
#endif
        strncat(status_buf, s, MAX_STATUSLEN - strlen(status_buf));
        status_msg(status_buf);
        status_lf();
    }
}

/*
 * status_error
 *
 * display error status messages
 */
VOID status_error(STRPTR s)
{
    /* store current status flags, enable status output */
    LONG old_disp_status = disp_status;
    disp_status = STATUS_LINE;

    /* display error message */
    strncpy(status_buf, "*** ", MAX_STATUSLEN);
    strncat(status_buf, s, MAX_STATUSLEN);
    status_msg(status_buf);
    status_lf();

    /* restore status flags */
    disp_status = old_disp_status;

    set_return_code(RC_ERROR);
}
