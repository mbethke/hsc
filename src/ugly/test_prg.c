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
/* test prginfo.c */

#include <stdio.h>

#include "utypes.h"
#include "prginfo.h"

void main( void )
{
    set_prginfo( "my_prog", "Tommy-Saftwörx", 1, 3, 7,
                 "just a program", "This is FreeWare!" );

    printf( "\nthis is my program\n\n" );
}
