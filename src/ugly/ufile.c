/*
 * ugly/ufile.c
 *
 * misc. filename functions
 *
 * Copyright (C) 1996  Thomas Aglassinger
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
 * updated: 15-Oct-1996
 * created: 14-Oct-1996
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_UFILE_H
#include "ufile.h"

/*
 * fexists
 *
 * check if file exists ( = could have been opened for input)
 *
 * result: TRUE, if file exists, else FALSE
 *
 */
BOOL fexists( STRPTR filename)
{
    FILE *file = fopen( filename, "r");
    if (file)
    {
        fclose(file);
    }
    return((BOOL)(file!=NULL));
}

/*
 * getfsize
 *
 * get size of file
 *
 * parans: filename...path and name of file to examine
 * result: size of file
 */
LONG getfsize( STRPTR filename)
{
    FILE *file = fopen(filename, "rb");
    LONG filesize = 0;
    if (file)
    {
        /* retrieve size */
        fseek(file, 0L, SEEK_END);
        filesize = ftell(file);
        fclose(file);
    }
    return(filesize);
}
