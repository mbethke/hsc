/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
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
 * ugly/utime.c
 *
 * misc. time-handling functions
 *
 * Copyright (C) 1994,95,96  Thomas Aglassinger
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
 * updated:  6-Dec-1996
 * created:  6-Jun-1996
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_UTIME_H
#include "utime.h"

/* debugging defines */
#ifdef D
#undef D
#endif

#if DEBUG_UGLY_TIME
#define D(x) x
#define DUT "*utime * "
#else
#define D(x)                    /* nufin */
#endif

BOOL time2estr(EXPSTR * dest, struct tm *time)
{
#define TIMEBUF_INC_STR "12345678901234567890"

    /*localtime(&(hp->start_time)) ); */
    BOOL ok = TRUE;
    size_t strftrc = 0;

    /* reset destination string */
    set_estr(dest, TIMEBUF_INC_STR);

    /* write time to string */
    while (ok && !strftrc)
    {
        /* expand dest */
        ok = app_estr(dest, TIMEBUF_INC_STR);

        D(fprintf(stderr, DUT "dest: inc+%ld\n",
                  strlen(TIMEBUF_INC_STR)));

        /* write string */
        strftrc = strftime(estr2str(dest), estrlen(dest),
                           UGLY_TIME_TEMPLATE_STRFTIME, time);
    }

    D(if (!ok) fprintf(stderr, DUT "time2estr: FAILED\n"));

    return (ok);
}

VOID clrtime(struct tm * time)
{
    time->tm_sec = 0;
    time->tm_min = 0;
    time->tm_hour = 0;
    time->tm_mday = 0;
    time->tm_mon = 0;
    time->tm_year = 0;
    time->tm_wday = 0;
    time->tm_yday = 0;
    time->tm_isdst = 0;
}

BOOL str2time(struct tm *time, STRPTR timestr)
{
    BOOL ok = FALSE;
    int scanrc = EOF;

    printf("testing ugly time funtions\n");

    clrtime(time);

    /* convert string to time */
    scanrc = sscanf(timestr, UGLY_TIME_TEMPLATE_SSCANF,
                    &time->tm_year, &time->tm_mon, &time->tm_mday,
                    &time->tm_hour, &time->tm_min, &time->tm_sec);

    if (scanrc != EOF)
    {
        time_t ttime;

        /* adjust values */
        time->tm_year -= 1990;
        time->tm_mon--;
        time->tm_mday--;

        /* fill rest of time-structure */
        ttime = mktime(time);

        if (ttime != ((time_t) - 1))
        {

            struct tm *ltime = localtime(&ttime);

            D(
                 {
                 STRARR timebuf2[200];
                 strftime(timebuf2, 200, UGLY_TIME_TEMPLATE_STRFTIME, ltime);
                 fprintf(stderr, DUT "ltime = `%s'\n", timebuf2);
                 }
            );

            if (ltime)
            {
                memcpy(time, ltime, sizeof(struct tm));
                ok = TRUE;
            }
            else
            {
                D(fprintf(stderr, DUT "str2time: localtime() FAILED\n"));
            }

        }
        else
        {
            D(fprintf(stderr, DUT "str2time: mktime() FAILED\n"));
        }
    }
    else
    {
        D(fprintf(stderr, DUT "str2time: sscanf() FAILED\n"));
    }

    return (ok);
}

