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
 * test_str.c
 *
 * test string functions
 *
 * (W) by Tommy-Saftwörx in 1995
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"

void test_enumstr(STRPTR str, STRPTR set, char sep, BYTE options)
{
    LONG result = strenum(str, set, sep, options);

    printf("strenum( \"%4s\",%-16s,\"%c\",%d) -> %ld\n",
           str, set, sep, options, result);
}

int main(void)
{
    STRPTR nam;

#if DEBUG_UGLY_MEMORY

    /* display a memory tracking report */
    /* at end of execution */
    atexit(atexit_uglymemory);

#endif

    printf("Testing ugly string functions:\n\n");

#if 1
    /*
     * test strenum:
     */
    test_enumstr("SEPP", "sepp|hugo|resi", '|', STEN_NOCASE);
    test_enumstr("SEPP", "sepp|hugo|resi", '|', STEN_CASE);
    test_enumstr("HUGO", "sepp|hugo|resi", '|', STEN_NOCASE);
    test_enumstr("HUGO", "sepp|hugo|resi", '|', STEN_CASE);
    test_enumstr("hugo", "sepp|hugo|resi", '|', STEN_CASE);
    test_enumstr("RESI", "sepp|hugo|resi", '|', STEN_NOCASE);
    test_enumstr("RESI", "sepp|hugo|resi", '|', STEN_CASE);
#endif

#if 1
    /*
     * test strclone
     */
    nam = strclone("hugo");
    printf("CLONE: \"%s\"\n", nam);
    ufreestr(nam);
#endif

    return (0);

}

