/*
 * ugly/args.c
 *
 * ugly argument handling functions
 *
 * Copyright (C) 1994,95,96  Thomas Aglassinger
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
 * updated: 30-Jul-1996
 * created:  3-Jul-1994
 *
 */

/*
 * includes
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "dllist.h"

#define NOEXTERN_UGLY_UARGS_H
#include "uargs.h"

/*
 *
 * include sub-modules
 *
 */
#include "args_fre.c"
#include "args_set.c"
#include "args_prp.c"
#include "args_hlp.c"

