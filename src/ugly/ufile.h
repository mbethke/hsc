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
 *
 * ugly/ufile.h
 *
 * header file for misc. filename functions
 *
 * (W) by Tommy-Saftwörx in 1996
 *
 */

#ifndef UGLY_UFILE_H
#define UGLY_UFILE_H

#include "utypes.h"
#include "expstr.h"

enum fcopy_e
{
    FC_OK,                      /* sucessfull */
    FC_ERR_NoMemory,            /* out of memory */
    FC_ERR_OpenInput,           /* can't open input file */
    FC_ERR_OpenOutput,          /* can't open output file */
    FC_ERR_Read,                /* error reading input */
    FC_ERR_Write                /* error writing output */
};

typedef enum fcopy_e fcopy_t;

/*
 * external function prototypes
 */

#ifndef NOEXTERN_UGLY_UFILE_H

extern BOOL fexists(STRPTR filename);
extern LONG getfsize(STRPTR filename);

extern fcopy_t fcopy(CONSTRPTR oldname, CONSTRPTR newname);
extern fcopy_t fmove(CONSTRPTR oldname, CONSTRPTR newname);

#endif

#endif
