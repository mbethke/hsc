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
 * hsclib/linit.h
 *
 * configuration & initialisation functions
 */
#ifndef HSCLIB_LINIT_H          /* avoid include twice */
#define HSCLIB_LINIT_H

#ifndef NOEXTERN_HSCLIB_LINIT_H

extern HSCPRC *hsc_read_base_info(VOID);
extern BOOL hsc_copy_base_info(HSCPRC * dest_hp, HSCPRC * dummy_hp);

extern BOOL hsc_init_hscprc(HSCPRC * hp, STRPTR prefs_fname);

extern BOOL hsc_init_tagsNattr(HSCPRC * hp);
extern BOOL hsc_init_basicEntities(HSCPRC * hp);
extern BOOL hsc_assign_tagCBs(HSCPRC * hp);

#endif /* NOEXTERN_HSCLIB_LINIT_H */
#endif /* HSCLIB_LINIT_H */

/* $Id$ */
/* vi: set ts=4: */

