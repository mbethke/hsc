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
 * hsclib/idref.h
 *
 * functions for id-references
 *
 */

#ifndef HSCLIB_IDREF_H
#define HSCLIB_IDREF_H

typedef struct idref_node {
    STRPTR name;                /* name of id */
    INFILEPOS *fpos;            /* position where ID has been called from */
} IDREF;

#ifndef NOEXTERN_HSCLIB_IDREF_H

extern void del_string_entry(APTR data);
extern STRPTR new_string_entry(STRPTR data);
extern int cmp_string_entry(APTR cmp_data, APTR list_data);


extern void hsc_msg_unknown_id(HSCPRC * hp, STRPTR filename, STRPTR id);

extern BOOL add_local_iddef(HSCPRC * hp, STRPTR id);

extern void del_idref(APTR data);
extern void add_local_idref(HSCPRC * hp, STRPTR id);

extern BOOL check_all_local_idref(HSCPRC * hp);

#endif /* NOEXTERN_HSCLIB_IDREF_H */

#endif /* HSC_IDREF_H */

/* $Id$ */
/* vi: set ts=4: */

