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
#ifndef UGLY_DLLIST_H
#define UGLY_DLLIST_H           /* avoid include twice */

/*
 * dllist.h
 *
 * doubly linked list functions
 *
 * (W) by Tommy-Saftwörx in 1994,95
 *
 */

#include <stdio.h>

#include "utypes.h"

/*
 *
 * double linked list structs
 *
 */
struct dlnode
{
    struct dlnode *prev;        /* previous entry */
    struct dlnode *next;        /* next entry */
    APTR data;                  /* ptr to data */
};

struct dllist
{
    /* PRIVATE (read-only) */
    struct dlnode *first;       /* first entry */
    struct dlnode *last;        /* last entry */
    LONG entry_num;             /* num of entries in list */
    void (*del_data) (APTR data);  /* func that removes data */
    /* PUBLIC */
    APTR user_data;             /* user data */
};

typedef struct dllist DLLIST;
typedef struct dlnode DLNODE;

/*
 * access methodes
 */
#define dll_first( list ) ((list)->first)
#define dll_last( list )  ((list)->last)
#define dln_prev( node )  ((node)->prev)
#define dln_next( node )  ((node)->next)
#define dln_data( node )  ((node)->data)
#define dll_detach_node(list,node) { \
if(NULL != dln_prev(node)) dln_next(dln_prev(node)) = dln_next(node); \
   else dll_first(list) = dln_next(node); \
if(NULL != dln_next(node)) dln_prev(dln_next(node)) = dln_prev(node); \
   else dll_last(list) = dln_prev(node); \
   --(list)->entry_num; \
}
#define dll_append_node(list,node) { \
   dln_next(node) = NULL; \
   if(NULL != (dln_prev(node) = dll_last(list))) dln_next(dln_prev(node)) = (node); \
   dll_last(list) = (node); \
   if(NULL == dll_first(list)) dll_first(list) = (node); \
}

/*
 *
 * external functions prototypes
 *
 */
#ifndef NOEXTERN_UGLY_DLLIST_H

extern DLLIST *init_dllist(void (*del_data) (APTR data));
extern DLNODE *new_dlnode(void);
extern DLNODE *app_dlnode(DLLIST * list, APTR data);
extern DLNODE *add_dlnode(DLLIST * list, APTR data);

extern APTR detach_dlnode(DLLIST * list, DLNODE * node);
extern void del_dlnode(DLLIST * list, DLNODE * node);
void move_dlnode(DLLIST *dest, DLLIST *src, DLNODE *node);
extern void del_all_dlnodes(DLLIST * list);
extern void del_dllist(DLLIST * list);

extern BOOL empty_dllist(DLLIST * list);

extern void fprintf_dllist(FILE * stream, DLLIST * list,
                           void (*fprintf_data) (FILE * stream, APTR data));
extern void do_dllist(DLLIST * list, void (*func) (APTR data));

extern DLNODE *find_dlnode(DLNODE * start, APTR data,
                           int (*compare) (APTR cmp_data, APTR list_data));
extern DLNODE *find_dlnode_bw(DLNODE * start, APTR data,
                            int (*compare) (APTR cmp_data, APTR list_data));

#endif /* NOEXTERN_UGLY_DLLIST_H */

#endif /* UGLY_DLLIST_H */
