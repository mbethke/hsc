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
/*
 * hscprj/pdebug.h
 *
 * debugging defines for hsc's project-functions
 */

#ifndef HSCPRJ_PDEBUG_H
#define HSCPRJ_PDEBUG_H

#include "hsclib/ldebug.h"

/* this debugging info is displayed only
 * if the  switch "-debug" has been enabled
 */

/* debugging control */
#define DEBUG_PRJ 1

/* debugging define */
#if (DEBUG & DEBUG_PRJ)
#define DP(x) if ( hp->debug ) x
#else
#define DP(x)                 /* nufin */
#endif

/* debug message prefix for hscprj-modules */
#define DHP "*hscprj* "

#endif /* HSCPRJ_PDEBUG_H */

