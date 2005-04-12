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
 * ugly/ustrlist.h
 *
 * header file for string list functions
 *
 * (W) by Tommy-Saftwörx in 1996
 *
 */

#ifndef UGLY_USTRLIST_H
#define UGLY_USTRLIST_H

#include "utypes.h"
#include "ustring.h"
#include "dllist.h"

/*
 * external function prototypes
 */


#ifndef NOEXTERN_UGLY_USTRLIST_H

#define find_strnode(nd,str) find_dlnode((nd),(APTR)(str), cmp_string_node)

extern void del_string_node(APTR data);
extern STRPTR new_string_node(STRPTR data);
extern int cmp_string_node(APTR cmp_data, APTR lst_data);

extern void clr_strlist(DLLIST *list);
extern void del_strlist(DLLIST *list);
extern DLLIST *init_strlist(void);

extern DLNODE *app_strnode(DLLIST * list, STRPTR str);
extern DLNODE *add_strnode(DLLIST * list, STRPTR str);

#endif

#endif

