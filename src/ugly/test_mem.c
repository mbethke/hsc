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
 * test_mem
 *
 * test functions of ugly/memory.c
 */

#include <stdio.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"

#define modfit(x,by) ((by)*(((x)+(by-1))/(by)))

void testfit(size_t size, size_t by)
{
    printf("fit: %4lu,%3lu) ->%4lu\n", size, by, modfit(size, by));
}

int main(void)
{
    STRPTR hugo1, hugo2, hugo3, hugo4;

    atexit(atexit_uglymemory);

    /* dummy usage of all vars to suppress warnings */
    hugo1 = "hugo1";
    hugo2 = "hugo2";
    hugo3 = "hugo3";
    hugo4 = "hugo4";

#if 0
    testfit(25, 8);
    testfit(100, 8);
    testfit(399, 8);
    testfit(400, 8);
    testfit(401, 8);
#endif

#if 1
    hugo1 = (STRPTR) umalloc(100);
    hugo1[0] = 0;
    hugo1[99] = 0x99;
    hugo1[100] = 0x10;
#endif

#if 1
    hugo2 = umalloc(200);
    ufree(hugo2);
#endif

#if 1
    hugo3 = strclone("hugo3");
#endif

#if 1
    hugo4 = (APTR) 0x123;
    umem_report("hugo after alloc");
    ufree(hugo1);
    ufreestr(hugo3);
    ufree(hugo2);
    ufree(hugo4);
    hugo4 = umalloc(401);
#endif

    umem_report("hugo after free");

    return (0);
}

