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

