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
#ifndef UGLY_TYPES_H
#define UGLY_TYPES_H

/*
 * ugly/types.h
 *
 * ugly data typing.
 *
 * NOTE: contains also UGLY_VER and UGLY_REV and
 * includes debuggin defines
 *
 * (W) by Tommy-Saftwörx 1994,95,96,97
 *
 * updated: 20-Mar-1997
 * created: 25-Jan-1994
 */

/* include debugging defines */
#include "udebug.h"

/*
 * are you running a amiga?
 */

#if 0 /* defined AMIGA */

/*
 * on amiga system, just include system types
 */

#include <exec/types.h>

/*
 *
 * no amiga -> normal typedefs
 *
 */

#else

#ifndef APTR_TYPEDEF
#define APTR_TYPEDEF
typedef void *APTR;             /* 32-bit untyped pointer */
#endif
typedef long LONG;              /* signed 32-bit quantity */
typedef unsigned long ULONG;    /* unsigned 32-bit quantity */
typedef short WORD;             /* signed 16-bit quantity */
typedef unsigned short UWORD;   /* unsigned 16-bit quantity */
#if __STDC__
typedef signed char BYTE;       /* signed 8-bit quantity */
#else
typedef char BYTE;              /* signed 8-bit quantity */
#endif
typedef unsigned char UBYTE;    /* unsigned 8-bit quantity */

#if defined(__cplusplus) || defined(RISCOS)
typedef char *STRPTR;           /* string pointer (NULL terminated) */
#else
typedef unsigned char *STRPTR;  /* string pointer (NULL terminated) */
#endif

/* Types with specific semantics */
typedef void VOID;
#ifndef RISCOS
typedef short BOOL;
typedef unsigned char TEXT;
#else
typedef int BOOL;
typedef char TEXT;
#endif

#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#ifndef NULL
#define NULL            0L
#endif

#define BYTEMASK        0xFF
#define WORDMASK        0xFFFF

#endif /* AMIGA */

/*
 *
 * global typedefs (on any system)
 *
 */

#ifndef RISCOS
typedef const unsigned char *CONSTRPTR;         /* string constants */
typedef unsigned char STRARR;   /* string array */
typedef unsigned char CHAR;     /* single character */
#else
typedef const char *CONSTRPTR;         /* string constants */
typedef char STRARR;   /* string array */
typedef char CHAR;     /* single character */
#endif

/*
 * UPTR as an generic pointer. C-math will not operate on UPTR.
 * UPTR can be converted to any other pointer and the other way round.
 * It is used by ugly functions, especially the umx-functions
 */
typedef void *UPTR;             /* generic pointer ( ANSI-def ) */

/*
 * compare/delete function type
 */
typedef int cmp_func(UPTR data1, UPTR data2);
typedef void del_func(UPTR data);

#endif /* UGLY_TYPES_H */

