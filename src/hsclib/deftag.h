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
 * hsclib/deftag.h
 */

#ifndef HSCLIB_DEFTAG_H
#define HSCLIB_DEFTAG_H

#ifndef NOEXTERN_HSCLIB_DEFTAG_H

extern ULONG set_tag_args(HSCPRC * hp, HSCTAG * tag);
extern BOOL def_tag_args(HSCPRC * hp, HSCTAG * tag);
extern HSCTAG *def_tag_name(HSCPRC * hp, BOOL * open_tag);

#endif /* NOEXTERN_HSCLIB_DEFTAG_H */

#endif /* HSCLIB_DEFTAG_H */
