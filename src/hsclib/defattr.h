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
 * defattr.h
 *
 * functions to define new attribute
 * and manipulate attribute lists
 *
 */

#ifndef HSCLIB_DEFATTR_H
#define HSCLIB_DEFATTR_H

/*
 * global funcs
 */
#ifndef NOEXTERN_HSCLIB_DEFATTR

/* scope rules support */
extern LONG get_mci( HSCPRC *hp );
extern VOID unget_mci( HSCPRC *hp );
extern LONG get_current_mci( HSCPRC *hp );

/* attribute list manipulation */
extern BOOL copy_local_varlist(DLLIST * destlist, DLLIST * varlist, ULONG mci);
extern BOOL set_local_varlist(DLLIST * destlist, DLLIST * varlist, ULONG mci);
extern VOID remove_local_varlist(DLLIST * varlist, ULONG mci);
extern BOOL check_varlist(HSCPRC * hp, DLLIST * varlist);

/* define attribute */
extern HSCVAR *define_var(HSCPRC * hp, DLLIST * varlist, ULONG unmasked_flags);
extern HSCATTR *define_attr_by_hp(HSCPRC * hp, STRPTR default_value, ULONG unmasked_flags);
extern HSCATTR *define_attr_by_text(HSCPRC * hp, STRPTR attr_text, STRPTR default_value, ULONG unmasked_flags);

#endif /* NOEXTERN_HSCLIB_DEFATTR */
#endif /* HSCLIB_DEFATTR_H */
