/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * hsclib/ldebug.h
 *
 * debugging defines for hsc
 */

#ifndef HSCLIB_LDEBUG_H
#define HSCLIB_LDEBUG_H

/*
 * debugging switches
 *
 * if switch is set to "1", requested debugging information is
 * displayed if also the "-debug" switch is enabled when
 * starting hsc
 */
#ifdef DEBUG

/* this debugging info is displayed only
 * if the  switch "-debug" has been enabled
 */
#undef  DEBUG
#define DEBUG         1         /* misc. debugging */
#define DEBUG_ATTRVAL 1         /* display attribute values */
#define DEBUG_CONFIG  1         /* display config messages */
#define DEBUG_DEFATTR 1         /* display defined attributes */
#define DEBUG_DEFENT  1         /* display defined entities */
#define DEBUG_DEFTAG  1         /* display defined tags */
#define DEBUG_IF      1         /* display if/else */
#define DEBUG_MACRO   1         /* display macro */
#define DEBUG_SIZE    1         /* display getsize-info */
#define DEBUG_URI     1         /* display uri conversion */
#define DEBUG_ID      1         /* display IDs added/checked/.. */
#define DEBUG_HSCLIB_OUTPUT 0

/* this debugging info is displayed always,
 * independant of the hscprc's debug-flag */
#define DEBUG_ATTR    0         /* display new/del attribute */
#define DEBUG_ENTITY  0         /* display new/del entity */

/* ugly debugging info */
#ifndef DEBUG_UGLY
#define DEBUG_UGLY
#endif

#else

#define DEBUG         0         /* misc. debugging */
#define DEBUG_ATTRVAL 0         /* display attribute values */
#define DEBUG_CONFIG  0         /* display config messages */
#define DEBUG_DEFATTR 0         /* display defined attributes */
#define DEBUG_DEFENT  0         /* display defined entities */
#define DEBUG_DEFTAG  0         /* display defined tags */
#define DEBUG_IF      0         /* display if/else */
#define DEBUG_MACRO   0         /* display macro */
#define DEBUG_SIZE    0         /* display getsize-info */
#define DEBUG_URI     0         /* display uri conversion */
#define DEBUG_ID      0         /* display uri conversion */
#define DEBUG_HSCLIB_OUTPUT 0

#define DEBUG_ATTR    0         /* display new/del attribute */
#define DEBUG_ENTITY  0         /* display new/del entity */

#endif

/* debug message prefix for hsclib-modules */
#define DHL "*hsclib* "

/*
 * debugging defines
 */
#if DEBUG
#define D(x) if (hp->debug) x
#else
#define D(x)                    /* nufin */
#endif

#if DEBUG
#define DMSG(msg) ((hp->debug) ? (fprintf( stderr, DHL msg "\n" )) : 0 )
#else
#define DMSG(msg)               /* nufin */
#endif

#if DEBUG_ATTRVAL
#define DAV(x) if ( hp->debug ) x
#else
#define DAV(x)                  /* nufin */
#endif

#if DEBUG_CONFIG
#define DC(x) if ( hp->debug ) x
#else
#define DC(x)                   /* nufin */
#endif

#if DEBUG_DEFATTR
#define DDA(x) if ( hp->debug ) x
#else
#define DDA(x)                  /* nufin */
#endif

#if DEBUG_DEFENT
#define DDE(x) if ( hp->debug ) x
#else
#define DDE(x)                  /* nufin */
#endif

#if DEBUG_DEFTAG
#define DDT(x) if ( hp->debug ) x
#else
#define DDT(x)                  /* nufin */
#endif

#if DEBUG_IF
#define DIF(x) if ( hp->debug ) x
#else
#define DIF(x)                  /* nufin */
#endif

#if DEBUG_MACRO
#define DMC(x) if ( hp->debug ) x
#else
#define DMC(x)                  /* nufin */
#endif

#if DEBUG_SIZE
#define DSZ(x) if ( hp->debug ) x
#else
#define DSZ(x)                  /* nufin */
#endif

#if DEBUG_URI
#define DU(x) if ( hp->debug ) x
#else
#define DU(x)                   /* nufin */
#endif

#endif /* HSCLIB_LDEBUG_H */

