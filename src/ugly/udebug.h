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
#ifndef UGLY_DEBUG_H
#define UGLY_DEBUG_H
/*
 * ugly/debug.h
 *
 * ugly debugging defines.
 *
 * (C) by Tommy-Saftwörx 1996
 *
 */

#ifdef DEBUG_UGLY

/* NOTE: for some defines, a value of "2" enables
 * extended debuggin information:
 *
 * DEBUG_UGLY_EXPSTR=2 display every call to set_estr()
 * DEBUG_UGLY_MEMORY=2 display every call to umalloc/ufree
 */
#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 1
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 1
#define DEBUG_UGLY_TIME   1

#else

#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 0
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 0
#define DEBUG_UGLY_TIME   0

#endif /* DEBUG_UGLY */

#define panic(msg) {                                 \
    fprintf(stderr, "** PANIC at \"%s\" (%u): %s\n", \
            __FILE__, __LINE__, (msg)); exit(255); }

#endif /* UGLY_DEBUG_H */

