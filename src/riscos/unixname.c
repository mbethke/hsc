/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1997  Sergio Monesi and Nick Craig-Wood
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unixname.h"

extern FILE *gopen(const char *, const char *);
extern FILE *greopen(const char *, const char *, FILE *);
extern int gemove(const char *);
extern int gename(const char *, const char *);

/************************************************************
This translates a unix file name to a riscos filename

.. -> ^
/ <-> .
************************************************************/

char *unixname_to_riscos(const char *unix)
{
    int len = strlen(unix) + 1;
    char *_riscos = malloc(len);
    char *riscos;
    
/* fprintf(stderr, "unix   = '%s'\n", unix); */
    
    if (_riscos == 0)
    {
        fprintf(stderr, "Couldn't allocate memory for file name\n");
        exit(EXIT_FAILURE);
    }
    
    for (riscos = _riscos; *unix; unix++)
    {
        int c = *unix;
        switch (c)
        {
        case '.':
            if (unix[1] == '.')
            {
                unix++;
                *riscos++ = '^';
            }
            else
            {
                *riscos++ = '/';
            }
            break;
        case '/':
            *riscos++ = '.';
            break;
        default:
            *riscos++ = c;
            break;
        }
        
        *riscos = 0;
    }
    
/* fprintf(stderr, "riscos = '%s'\n\n", _riscos); */

    return _riscos;
}

/************************************************************
************************************************************/

FILE *fopen(const char *unix_filename, const char *mode)
{
    char *riscos_filename = unixname_to_riscos(unix_filename);
    FILE *handle = gopen(riscos_filename, mode);
    free(riscos_filename);
    return handle;
}

/************************************************************
************************************************************/

FILE *freopen(const char *unix_filename, const char *mode, FILE *stream)
{
    char *riscos_filename = unixname_to_riscos(unix_filename);
    FILE *handle = greopen(riscos_filename, mode, stream);
    free(riscos_filename);
    return handle;
}

/************************************************************
************************************************************/

int remove(const char *unix_filename)
{
    char *riscos_filename = unixname_to_riscos(unix_filename);
    int error = gemove(riscos_filename);
    free(riscos_filename);
    return error;
}

/************************************************************
************************************************************/

int rename(const char *unix_old, const char *unix_new)
{
    char *riscos_old = unixname_to_riscos(unix_old);
    char *riscos_new = unixname_to_riscos(unix_new);
    int error = gename(riscos_old, riscos_new);
    free(riscos_old);
    free(riscos_new);
    return error;
}
