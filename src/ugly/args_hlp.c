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
 * updated: 30-Jul-1995
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
 * fprintf_ah_flag
 */
int fprintf_ah_flag(FILE * stream, struct arginfo *ai,
                    ULONG chk_flag, char ch)
{
    int err = 0;

    if ((ai->ai_flags) & chk_flag)
        err = fprintf(stream, "/%c", ch);

    return err;

}

/*
 * fprintf_arghelp
 */
int fprintf_arghelp(FILE * stream, struct arglist *al)
{
    int err = 0;
    int maxidlen = 18;          /* max. length if arg id */

    if (al)
    {

        struct dlnode *no;
        struct arginfo *ai;

        no = al->al_list->first;

        while (no)
        {

            ai = (struct arginfo *) no->data;

            if (ai)
            {

#if 0
                char ch;
#endif

                if (ai->ai_help)
                {

#if 0
                    switch (ai->ai_type)
                    {

                    case ARG_SWITCH:
                        ch = 'S';
                        break;
                    case ARG_TEXT:
                        ch = 'T';
                        break;
                    case ARG_LONG_RANGE:
                        ch = 'R';
                        break;
                    default:
                        ch = '?';
                        break;

                    }
                    fprintf(stream, "/%c", ch);

                    fprintf_ah_flag(stream, ai, ARG_KEYWORD, 'K');
                    fprintf_ah_flag(stream, ai, ARG_REQUIRED, 'R');
                    fprintf_ah_flag(stream, ai, ARG_MULTIPLE, 'M');
                    fprintf_ah_flag(stream, ai, ARG_CASESENS, 'C');
#endif
                    if (ai->ai_help)
                        fprintf(stream, " %-*s  %s",
                                maxidlen, ai->ai_id, ai->ai_help);
                    else
                        fprintf(stream, "%s", ai->ai_id);

                    fprintf(stream, "\n");

                }

            }

            no = no->next;

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
            {

                char ch;

                fprintf(stream, "%s", ai->ai_id);

                switch (ai->ai_type)
                {

                case ARG_SWITCH:
                    ch = 'S';
                    break;
                case ARG_TEXT:
                    ch = '\0';
                    break;
                case ARG_LONG_RANGE:
                    ch = 'R';
                    break;
                case ARG_LONG:
                    ch = 'N';
                    break;
                case ARG_ENUM:
                    ch = 'E';
                    break;
                default:
                    ch = '?';
                    break;

                }
                if (ch)
                    fprintf(stream, "/%c", ch);

                fprintf_ah_flag(stream, ai, ARG_KEYWORD, 'K');
                fprintf_ah_flag(stream, ai, ARG_REQUIRED, 'A');
                fprintf_ah_flag(stream, ai, ARG_MULTIPLE, 'M');
                fprintf_ah_flag(stream, ai, ARG_CASESENS, 'C');
                fprintf_ah_flag(stream, ai, ARG_OVERWRITE, 'O');

            }

            no = no->next;
            if (no)
                fprintf(stream, ",");

        }                       /*while */

        fprintf(stream, "\n");
    }

    return err;
}

