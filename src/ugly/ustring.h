/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
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
#ifndef UGLY_USTRING_H
#define UGLY_USTRING_H

/*
 * ugly/string.h
 *
 * additional string functions, header
 *
 * (W) by Tommy-Saftwörx in 1993,94,95
 *
 */

#include <stddef.h>

#include "utypes.h"

#define freestr( s ) ugly_freestr(s,__FILE__,__LINE__)
#define ufreestr( s ) freestr(s),s=NULL
#define strclone( s ) ugly_strclone(s,__FILE__,__LINE__)
#define reallocstr( o,n ) ugly_reallocstr(o,n,__FILE__,__LINE__)

/* options for strenum */
#define STEN_CASE   0           /* case sensitive search */
#define STEN_NOCASE 1           /* case insensitive search */

/*
 * external prototypes
 */

#ifndef NOEXTERN_UGLY_USTRING_H

extern STRPTR upstr(STRPTR s);
extern int upstrcmp(CONSTRPTR s1, CONSTRPTR s2);
extern int upstrncmp(CONSTRPTR s1, CONSTRPTR s2, size_t n);
extern STRPTR upstrstr(CONSTRPTR s1, CONSTRPTR s2);

extern void ugly_freestr(STRPTR s, STRPTR file, ULONG line);
extern STRPTR ugly_strclone(CONSTRPTR oldstr, STRPTR file, ULONG line);
extern void ugly_reallocstr(STRPTR * oldstr, CONSTRPTR newstr, STRPTR file, ULONG line);

extern STRPTR ustrrpbrk(CONSTRPTR str, CONSTRPTR set);
extern LONG strenum(STRPTR str, STRPTR set, char sep, BYTE options);

extern int last_ch(STRPTR s);
extern STRPTR ch2str(const char ch);
extern BOOL str2long(STRPTR s, LONG * num);
extern STRPTR long2str(LONG num);

#endif /* NOEXTERN_UGLY_USTRING_H */

#endif /* UGLY_USTRING_H */

