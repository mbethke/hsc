/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
/* Used to create GST */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/ustrlist.h"
#include "ugly/uargs.h"
#include "ugly/returncd.h"
#include "ugly/fname.h"
#include "ugly/utypes.h"
#include "ugly/udebug.h"
#include "ugly/expstr.h"
#include "ugly/umemory.h"
#include "ugly/ustring.h"
#include "ugly/utime.h"
#include "ugly/prginfo.h"
#include "ugly/ufile.h"
