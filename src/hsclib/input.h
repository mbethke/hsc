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
 * hsclib/input.h
 *
 * basic functions for parsing input
 */

#ifndef HSCLIB_INPUT_H
#define HSCLIB_INPUT_H

#include "ugly/utypes.h"
#include "ugly/infile.h"

/*
 * global funcs
 */

extern BOOL hsc_whtspc(int ch);
extern BOOL hsc_normch(int ch);
extern BOOL hsc_normch_tagid(int ch);

extern STRPTR infget_tagid(HSCPRC * hp);
extern STRPTR infget_attrid(HSCPRC * hp);

extern BOOL parse_wd(HSCPRC * hp, STRPTR expstr);
extern BOOL parse_eq(HSCPRC * hp);
extern BOOL parse_gt(HSCPRC * hp);

#endif /* HSCLIB_INPUT_H */

