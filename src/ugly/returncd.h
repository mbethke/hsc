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
/*
 * ugly/returncode.h
 *
 * defines for program return codes
 *
 */

#ifndef UGLY_RETURNCODE_H       /* avoid include twice */
#define UGLY_RETURNCODE_H

/*
 * define values for return code
 */
#if defined(AMIGA) || defined(AROS)

#define RC_OK     0
#define RC_WARN   5
#define RC_ERROR 10
#define RC_FAIL  20

#elif (defined RISCOS) || (defined BEOS) || (defined NEXTSTEP) || (defined UNIX) || (defined WINNT)

#define RC_OK     0
#define RC_WARN   0
#define RC_ERROR  1
#define RC_FAIL   2

#else
#error "system not supported: return codes"

#endif /* return code */

#endif /* UGLY_RETURNCODE_H */

