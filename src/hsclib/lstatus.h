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
 * hsclib/lstatus.h
 *
 * status messages
 *
 */

#ifndef HSCLIB_LSTATUS_H
#define HSCLIB_LSTATUS_H

extern VOID hsc_status_misc(HSCPRC * hp, STRPTR s);
extern VOID hsc_status_file_begin(HSCPRC * hp, STRPTR filename);
extern VOID hsc_status_file_end(HSCPRC * hp);
extern VOID hsc_status_line(HSCPRC * hp);

#endif /* HSCLIB_LSTATUS_H */

