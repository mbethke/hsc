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
 * ugly/ustrlist.c - string list functions
 *
 * updated:  9-Mar-1997
 * created: 14-Oct-1996
 *
 */

#include "sysdep.h"
#include <stdio.h>

#include "umemory.h"
#define NOEXTERN_UGLY_USTRLIST_H
#include "ustrlist.h"

/*
 * del_string_node
 */
void del_string_node(APTR data)
{
   STRPTR s = (STRPTR)data;
   ufreestr(s);
}

/*
 * new_string_node
 */
STRPTR new_string_node(STRPTR data)
{
    return (strclone(data));
}

/*
 * cmp_string_node
 */
int cmp_string_node(APTR cmp_data, APTR lst_data)
{
    STRPTR s1 = (STRPTR) cmp_data;
    STRPTR s2 = (STRPTR) lst_data;

#ifdef DEBUG_UGLY
    if (!cmp_data)
        panic("cmp_data = NULL");
    if (!lst_data)
        panic("lst_data = NULL");
#endif

    if (!strcmp(s1, s2))
        return (-1);
    else
        return (0);
}

/*
 * del_strlist: cleanup whole list of strings
 */
void del_strlist(DLLIST * list)
{
    del_dllist(list);
}

/*
 * del_strlist: cleanup whole list of strings
 */
void clr_strlist(DLLIST * list)
{
    del_all_dlnodes(list);
}

/*
 * init_strlist: set up new list of strings
 */
DLLIST *init_strlist(void)
{
    return (init_dllist(del_string_node));
}

/*
 * app_strnode
 *
 * append new string to string-list (at end)
 */
DLNODE *app_strnode(DLLIST * list, STRPTR str)
{
    STRPTR scp = new_string_node(str);
    DLNODE *nd = NULL;

    if (scp)
        nd = app_dlnode(list, (APTR) scp);

    return nd;
}

/*
 * add_strnode
 *
 * add new string to string-list (at begining)
 */
DLNODE *add_strnode(DLLIST * list, STRPTR str)
{
    STRPTR scp = new_string_node(str);
    DLNODE *nd = NULL;

    if (scp)
    {
        nd = add_dlnode(list, (APTR) scp);
    }

    return nd;
}

