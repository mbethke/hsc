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

