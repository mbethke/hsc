/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
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
 * ugly/prginfo.c
 *
 * ugly program info functions
 *
 * updated: 27-Oct-1996
 * created:  3-Jul-1994
 *
 *=========================================================
 * TODO:
 * - set_prginfo()-macro: replace "name" by "( name ? name : argv[0] )"
 *
 */

/* ANSI includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"

#define SIZE_DATESTR 24

#ifdef AMIGA
/* system version string on amiga systems */
STRPTR amiga_version = "";
#endif

/*
 * local global vars
 */
STRPTR pi_progname = NULL;      /* exported */
STRPTR pi_authname = NULL;
int pi_version = 0;
int pi_release = 0;
int pi_revision = 0;
char pi_rel_date[SIZE_DATESTR];
STRPTR pi_rel_time = NULL;
STRPTR pi_descript = NULL;
STRPTR pi_copystat = NULL;

STRPTR pi_dt_day = NULL;
STRPTR pi_dt_month = NULL;
STRPTR pi_dt_year = NULL;

/*
 * call_set_prginfo
 *
 * set pi_xxx vars; called by macro set_info()
 *
 */
void call_set_prginfo(STRPTR name, STRPTR auth, int ver, int rel, int rev,
        STRPTR rel_date, STRPTR rel_time, STRPTR infostr, STRPTR copystatus)
{
    pi_progname = name;
    pi_authname = auth;
    pi_version = ver;
    pi_release = rel;
    pi_revision = rev;
    pi_rel_time = rel_time;
    pi_descript = infostr;
    pi_copystat = copystatus;

    strncpy(pi_rel_date, rel_date, SIZE_DATESTR);
    pi_rel_date[3] = '\0';
    pi_rel_date[6] = '\0';
    pi_dt_month = pi_rel_date;
    pi_dt_day = &(pi_rel_date[4]);
    if (pi_dt_day[0] == ' ')
        pi_dt_day++;
    pi_dt_year = &(pi_rel_date[7]);
}

/*
 * call_set_prginfo2
 *
 * set pi_xxx vars; called by macro set_info()
 *
 * NOTE: this version expects date "DD.MM.YY"
 */
void call_set_prginfo2(STRPTR name, STRPTR auth, int ver, int rel, int rev,
        STRPTR rel_date, STRPTR rel_time, STRPTR infostr, STRPTR copystatus)
{
    STRPTR monthName[] =
    {"XXX", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    pi_progname = name;
    pi_authname = auth;
    pi_version = ver;
    pi_release = rel;
    pi_revision = rev;
    pi_rel_time = rel_time;
    pi_descript = infostr;
    pi_copystat = copystatus;

    strncpy(pi_rel_date, rel_date, SIZE_DATESTR);

    /* eval day */
    pi_dt_day = pi_rel_date;

    /* eval month */
    pi_dt_month = strchr(pi_rel_date, '.');
    pi_dt_month[0] = '\0';
    pi_dt_month++;

    /* eval year */
    pi_dt_year = strchr(pi_dt_month, '.');
    pi_dt_year[0] = '\0';
    pi_dt_year++;

    /* convert numeric month */
    pi_dt_month = monthName[strtol(pi_dt_month, NULL, 10)];
}

/*
 * fprintf_prginfo
 *
 * display program information
 *
 */
int fprintf_prginfo(FILE * stream)
{
    int err = 0;

    err = fprintf(stream, "%s - %s, v%d.%d",
                  pi_progname, pi_descript,     /* name & description */
                  pi_version, pi_release);      /* version */
    if (pi_revision)
        err += fprintf(stream, ".%d",   /* revision */
                       pi_revision);
    err += fprintf(stream, " (%s-%s-%s)\n",     /* date */
                   pi_dt_day, pi_dt_month, pi_dt_year);
    err += fprintf(stream, "(C) %s. %s\n",      /* copyright */
                   pi_authname, pi_copystat);

    return (err);

}

