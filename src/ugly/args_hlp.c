/*
 * ugly/args_hlp.c
 *
 * sub-module for ugly/args.c
 *
 * ugly argument handling help functions
 *
 * Copyright (C) 1994,95  Thomas Aglassinger
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
 * updated: 10-Sep-1996
 * created:  3-Jul-1994
 *
 */

/*
 * includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "dllist.h"

#define NOEXTERN_UGLY_ARGS_H
#include "uargs.h"

/*
 * strcat_flag
 */
static VOID strcat_flag(STRPTR s, struct arginfo *ai, ULONG chk_flag, char ch)
{
    char flag[3] = "/x";

    if ((ai->ai_flags) & chk_flag)
    {
        flag[1] = ch;
        strcat(s, flag);
    }
}

static STRPTR ai2str(struct arginfo *ai)
{
    static STRARR s[100];

    strncpy(s, ai->ai_id, 100 - 20);

    switch (ai->ai_type)
    {
    case ARG_SWITCH:
        strcat(s, "/S");
        break;
    case ARG_LONG_RANGE:
    case ARG_LONG:
        strcat(s, "/N");
        break;
    case ARG_TEXT:
    case ARG_ENUM:
    case ARG_HANDLEFUNC:
        break;
    default:
        strcat(s, "/?");
        break;
    }

    strcat_flag(s, ai, ARG_KEYWORD, 'K');
    strcat_flag(s, ai, ARG_REQUIRED, 'A');
    strcat_flag(s, ai, ARG_MULTIPLE, 'M');
    strcat_flag(s, ai, ARG_CASESENS, 'C');

    return (s);
}

/*
 * fprintf_arghelp
 */
int fprintf_arghelp(FILE * stream, struct arglist *al)
{
    int err = 0;

    if (al)
    {
        struct dlnode *nd;
        struct arginfo *ai;
        size_t maxidlen = 0;      /* max. length if arg id */

        /* compute maximum length */
        nd = al->al_list->first;
        while (nd)
        {
            ai = (struct arginfo *) dln_data(nd);

            if (ai)
            {
                if (ai->ai_help)
                {
                    STRPTR s = ai2str(ai);
                    if (strlen(s)>maxidlen)
                        maxidlen = strlen(s);
                }
            }
            nd = dln_next(nd);
        }                       /*while */
        maxidlen += 2;

        nd = al->al_list->first;
        while (nd)
        {
            ai = (struct arginfo *) dln_data(nd);

            if (ai)
            {
                if (ai->ai_help)
                {
                    STRPTR s = ai2str(ai);

                    if (ai->ai_help)
                        fprintf(stream, " %-*s  %s",
                                (int) maxidlen, s, ai->ai_help);
                    else
                        fprintf(stream, "%s", s);

                    fprintf(stream, "\n");
                }
            }

            nd = dln_next(nd);
        }                       /*while */
    }

    return err;
}

/*
 * fprintf_arghelp_short
 *
 * print short template line help
 */
int fprintf_arghelp_short(FILE * stream, struct arglist *al)
{
    int err = 0;

    if (al)
    {
        struct dlnode *no;
        struct arginfo *ai;

        no = al->al_list->first;

        if (no)
            fprintf(stream, "Usage: ");

        while (no)
        {
            ai = (struct arginfo *) no->data;

            if (ai)
                fprintf(stream, "%s", ai2str(ai));

            no = no->next;
            if (no)
                fprintf(stream, ",");
        }                       /*while */

        fprintf(stream, "\n");
    }

    return err;
}

