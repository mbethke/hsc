#ifndef UGLY_DLLIST_H
#define UGLY_DLLIST_H                       /* avoid include twice */

/*
** dllist.h
**
** doubel linked list functions, header
**
** (W) by Tommy-Saftwörx in 1994,95
**
** updated: 28-Jul-95
** created: 01-Mar-94
**
*/

#include <stdio.h>

#include "types.h"

/*
**
** double linked list structs
**
*/
struct dlnode {
    struct dlnode *prev;               /* previous entry */
    struct dlnode *next;               /* next entry */
    APTR           data;               /* ptr to data */
};

struct dllist {                        /* PRIVATE (read-only)*/
    struct dlnode *first;              /*   first entry */
    struct dlnode *last;               /*   last entry */
    LONG           entry_num;          /*   num of entris in list */
    void (*del_data) ( APTR data );    /*   func that removes data */
                                       /* PUBLIC */
    APTR           user_data;          /*   user data */
};

typedef struct dllist DLLIST;
typedef struct dlnode DLNODE;


/*
**
** external functions prototypes
**
*/
#ifndef NOEXTERN_UGLY_DLLIST_H

extern struct dllist *init_dllist( void (*del_data) ( APTR data ) );
extern struct dlnode *new_dlnode( void );
extern struct dlnode *ins_dlnode(
         struct dllist *list, struct dlnode *node, APTR data );
extern struct dlnode *app_dlnode( struct dllist *list, APTR data );

extern void del_dlnode( struct dllist *list, struct dlnode *node );
extern void del_dllist( struct dllist *list );

extern void fprintf_dllist( FILE *stream, struct dllist *list,
         void ( *fprintf_data ) ( FILE *stream, APTR data ) );
extern void do_dllist( DLLIST *list, void (*func) (APTR data) );

extern struct dlnode *find_dlnode( struct dlnode *start, APTR data,
         int (*compare) ( APTR cmp_data, APTR list_data ) );

#endif                                 /* NOEXTERN_UGLY_DLLIST_H */


#endif                                 /* UGLY_DLLIST_H */
