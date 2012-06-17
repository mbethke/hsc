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
 * ugly/fname.h
 *
 * header file for filename manipulaton functions
 *
 * (W) by Tommy-Saftwörx in 1994-97
 *
 */

#ifndef UGLY_FNAME_H
#define UGLY_FNAME_H

#include "utypes.h"
#include "expstr.h"

/*
 * system dependent defines
 */

/*
 * MAX_FPATH         max. length of whole path
 * MAX_FNAME         max. length of filename
 * MAX_FEXT          max. length of file extension
 * PATH_SEPARATOR    to separate diretories and devices
 * PATHL_SEPARATOR   to separate a list of paths in a string
 * DIR_SEPARATOR     to separate directories
 * PARENT_DIR        to be inserted to refer to parent directory
 * FNAME_IGNORE_CASE flag; ignore case within filenames
 *
 * CRLF_SHIT         0=OS uses single "\n" for EOL
 */

#if defined(AMIGA) || defined(AROS)   /* AmigaOS */
#define MAX_FPATH (256)
#define MAX_FNAME  (31)
#define MAX_FEXT   (30)
#define PATH_SEPARATOR    "/:"
#define PATHL_SEPARATOR   ";"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "/"
#define FNAME_IGNORE_CASE (1)
#define SUGGEST_CRLF_SHIT (0)

#elif defined RISCOS                   /* RiscOS */
#define MAX_FPATH (255)
#define MAX_FNAME  (32)
#define MAX_FEXT   (32)
#define PATH_SEPARATOR    "/"
#define PATHL_SEPARATOR   ":"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE (1)
#define SUGGEST_CRLF_SHIT (0)

#elif defined NEXTSTEP                 /* NeXTStep */
#define MAX_FPATH (254)
#define MAX_FNAME (254)
#define MAX_FEXT  (253)
#define PATH_SEPARATOR    "/"
#define PATHL_SEPARATOR   ":"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE (0)
#define SUGGEST_CRLF_SHIT (0)

#elif defined BEOS                     /* BeOS */
#define MAX_FPATH (254)
#define MAX_FNAME  (64)
#define MAX_FEXT   (63)
#define PATH_SEPARATOR    "/"
#define PATHL_SEPARATOR   ":"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE (1)
#define SUGGEST_CRLF_SHIT (0)

#elif defined UNIX                     /* Unixoids */
#define MAX_FPATH (254)
#define MAX_FNAME (254)
#define MAX_FEXT  (253)
#define PATH_SEPARATOR    "/"
#define PATHL_SEPARATOR   ":"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE (0)
#define SUGGEST_CRLF_SHIT (0)

#elif defined WINNT                    /* bullshit 1 */
#define MAX_FPATH (254)
#define MAX_FNAME (254)
#define MAX_FEXT  (253)
#define PATH_SEPARATOR    "\\:"
#define PATHL_SEPARATOR   ";"
#define DIR_SEPARATOR     '\\'
#define PARENT_DIR        "..\\"
#define FNAME_IGNORE_CASE (1)
#define SUGGEST_CRLF_SHIT (0)

#else
#error "Operating system not supported: filename-functions"
#endif

/* if CRLF_SHIT has not been set by user, use OS-default */
#ifndef CRLF_SHIT
#define CRLF_SHIT SUGGEST_CRLF_SHIT
#endif

/* strcmp() for filenames: case-sensitive or not */
#if FNAME_IGNORE_CASE
#define fnamecmp(a,b) strcmp((a),(b))
#define fnamencmp(a,b,n) strncmp((a),(b),(n))
#else
#define fnamecmp(a,b) upstrcmp((a),(b))
#define fnamencmp(a,b,n) upstrncmp((a),(b),(n))
#endif

#define ok_fnl_fpath(x) ((BOOL)(estrlen(x)<=MAX_FPATH))
#define ok_fnl_fname(x) ((BOOL)(estrlen(x)<=MAX_FNAME))
#define ok_fnl_fext(x)  ((BOOL)(estrlen(x)<=MAX_FEXT))

/*
 * external function prototypes
 */

#ifndef NOEXTERN_UGLY_FNAME_H

extern BOOL get_fext(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fname(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fpath(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fsdir(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_relfname(EXPSTR * dest, STRPTR absn, STRPTR curp);

extern BOOL clr_fext(EXPSTR * dest);
extern BOOL set_fext(EXPSTR * dest, CONSTRPTR newext);
extern BOOL app_fext(EXPSTR * dest, CONSTRPTR newext);
extern BOOL set_fnameIdx(EXPSTR * dest, int idx);
extern BOOL link_fname(EXPSTR * dest, STRPTR dir, STRPTR fn);
extern BOOL link_envfname(EXPSTR * dest, STRPTR env, STRPTR dir, STRPTR fn);

extern STRPTR tmpnamstr(STRPTR prefix);

extern BOOL optimize_fname(STRPTR *target_name, STRPTR source_name);

#endif

#endif

