/*
 *
 * utime.h
 *
 * header file for filename manipulaton functions
 *
 * (W) by Tommy-Saftwörx in 1996
 *
 */

#include "utypes.h"
#include "expstr.h"

/* template used to store time in a string: YYYY/MM/DD HH:MM:SS */
#define UGLY_TIME_TEMPLATE_SSCANF   "%04d/%02d/%02d %02d:%02d:%02d"
#define UGLY_TIME_TEMPLATE_STRFTIME "%Y/%m/%d %H:%M:%S"

#define UGLY_TIMESTR_LEN (strlen(UGLY_TIME_TEMPLATE_SSCANF))
/*
 * external function prototypes
 */

#ifndef NOEXTERN_UGLY_UTIME_H

extern VOID clrtime(struct tm *time);
extern BOOL time2estr(EXPSTR * dest, struct tm *time);
extern BOOL str2time(struct tm *time, STRPTR timestr);

#endif

