#ifndef UGLY_BNTREE_H
#define UGLY_BNTREE_H                  /* avoid include twice */

/*
** ugly/bntree.h
**
**
** (W) by Tommy-Saftwörx in 1994
**
** updated:  4-Jul-94
** created:  4-Jul-94
**
*/

/*
** includes
*/

#include "types.h"


/*
** struct btnode, bntree
*/
struct btnode {
#if 0
    struct btnode *fath;               /* father node */
#endif
    struct btnode *prev;               /* previous and next item */
    struct btnode *next;
    STRPTR         data;               /* user data */
};


struct bntree {                        /* PRIVATE */
    struct btnode *root;               /*   root node */
    LONG           entry_num;          /*   num of entries in tree */
    cmp_func      *compare;            /*   compare data function */
    del_func      *delete;             /*   delete data function */
                                       /* PUBLIC */
    UPTR           user_data;          /*   user data */
};




/*
** external functions prototypes
*/

#ifndef NOPROTOS_UGLY_BNTREE_H



#endif                                 /* NOPROTOS_UGLY_BNTREE_H */

#endif                                 /* UGLY_BNTREE_H */

