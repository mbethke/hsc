#ifndef UGLY_DLLIST_H
#define UGLY_DLLIST_H           /* avoid include twice */

/*
 * dllist.h
 *
 * doubel linked list functions, header
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
{                               /* PRIVATE (read-only) */
    struct dlnode *first;       /*   first entry */
    struct dlnode *last;        /*   last entry */
    LONG entry_num;             /*   num of entris in list */
    void (*del_data) (APTR data);       /*   func that removes data */
    /* PUBLIC */
    APTR user_data;             /*   user data */
};

typedef struct dllist DLLIST;
typedef struct dlnode DLNODE;

/*
 * access methodes
 */
#define dll_first( list ) (list)->first
#define dll_last( list )  (list)->last
#define dln_prev( node )  (node)->prev
#define dln_next( node )  (node)->next
#define dln_data( node )  (node)->data

/*
 *
 * external functions prototypes
 *
 */
#ifndef NOEXTERN_UGLY_DLLIST_H

extern DLLIST *init_dllist(void (*del_data) (APTR data));
extern DLNODE *new_dlnode(void);
extern DLNODE *ins_dlnode(
                       DLLIST *list, DLNODE *node, APTR data);
extern DLNODE *app_dlnode(DLLIST *list, APTR data);

extern APTR detach_dlnode(DLLIST *list, DLNODE *node);
extern void del_dlnode(DLLIST *list, DLNODE *node);
extern VOID del_all_dlnodes(DLLIST *list);
extern void del_dllist(DLLIST *list);

extern BOOL empty_dllist(DLNODE *list);

extern void fprintf_dllist(FILE * stream, DLLIST *list,
                           void (*fprintf_data) (FILE * stream, APTR data));
extern void do_dllist(DLLIST * list, void (*func) (APTR data));

extern DLNODE *find_dlnode(DLNODE *start, APTR data,
                            int (*compare) (APTR cmp_data, APTR list_data));

#endif /* NOEXTERN_UGLY_DLLIST_H */

#endif /* UGLY_DLLIST_H */

