/*
 * This source code is part of hsc, an html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * ugly/unikot.h - unicode support functions
 */

#include <stdlib.h>

typedef long int ucs4_t;
typedef short int ucs2_t;
typedef unsigned char utf8_t;

/*
 * defines
 */ 
#define UCS4_TO_UTF8_STR(utf8str,ucs4) \
   (utf8str)[ucs4_to_utf8((utf8str),(ucs4))] = '\0'
/*
 * error codes returned by utf8_to_ucs4()
 */
#define ERR_UNIKOT_FIRST_CHAR (-1)
#define ERR_UNIKOT_LATER_CHAR (-2)

#ifndef UNIKOT_INLINE
#define UNIKOT_INLINE           /* nufin */
#endif /* UNIKOT_INLINE */

#ifndef NOEXTERN_UGLY_UNIKOT_H
extern int utf8_to_ucs4(ucs4_t * ucs4, const utf8_t * utf8str);
extern int ucs4_to_utf8(utf8_t * utf8str, ucs4_t ucs4);
#endif /* NOEXTERN_UGLY_UNIKOT_H */

