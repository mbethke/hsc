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
 *
 * test_time.c
 *
 * test time functions
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
#include "utime.h"

EXPSTR *es, *res;

int main(void)
{
    time_t tnow;
    struct tm *now;
    struct tm now2;
    STRPTR timestr = NULL;
    STRARR timebuf[200];

    /* get current time */
    tnow = time(NULL);
    now = localtime(&tnow);

#if DEBUG_UGLY_MEMORY
    atexit(atexit_uglymemory);
#endif

#if 0
    for (i = 0; i < 20; i++)
        printf("modadj(%-2d/%-2d) = %d\n",
               i, EXPSTR_MEMSTEP, modadj(i, EXPSTR_MEMSTEP));
#endif

    es = init_estr(8);
    res = init_estr(8);

#if 1
    /* display current time */
    strftime(timebuf, 200, UGLY_TIME_TEMPLATE_STRFTIME, now);
    printf("strftime : `%s'\n", timebuf);

    /* convert current time to string */
    time2estr(es, now);
    printf("time2estr: `%s'\n", estr2str(es));

    /* convert string to time */
    timestr = estr2str(es);
    str2time(&now2, timestr);
    strftime(timebuf, 200, UGLY_TIME_TEMPLATE_STRFTIME, &now2);
    printf("_timestr : `%s'\n", timestr);
    printf("str2time : `%s'\n", timebuf);

#endif

    del_estr(es);
    del_estr(res);

    return (0);
}

