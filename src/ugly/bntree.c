/*
** btree.c
**
** (W) by Tommy-Saftwörx in 1994
**
** updated:  4-Jul-94
** created:  4-Jul-94
**
** $VER: btree.c 2.0.0 (4.7.1994)                           compt:ANSI
**
**===================================================================
*/

/*
** includes
*/
#include <stdlib.h>

#include "types.h"

#define NOPROTOS_BNTREE_H              /* avoid extern prototypes */
#include "bntree.h"


/*
** init_bntree
**
*/
struct bntree *init_bntree( cmp_func *compare )
{
    struct bntree *newtree;

    newtree = malloc( sizeof( struct bntree ) );

    if ( newtree ) {

        newtree->root      = NULL;
        newtree->entry_num = 0;
        newtree->compare   = compare;
        newtree->user_data = NULL;

    }

    return newtree;

}


/*
** new_btnode()
**
** new_btnode
**
** alloc and init a new node
**
*/
struct btnode *new_btnode( void )
{
    struct btnode *newnode;

    newnode = malloc( sizeof(struct btnode) );   /*  alloc new node */

    if ( newnode ) {                             /*  sucessful? */

        newnode->prev = NULL;                    /*  Y->init data */
        newnode->next = NULL;
        newnode->data = NULL;

    }

    return ( newnode );                          /*  return item */

}


/*
** del_btnode
**
** free unsed node
**
*/
void del_btnode( struct bntree *tree, struct btnode *node )
{
    if ( tree )

        if ( node ) {




            node->data = NULL;             /*  clear all data */

            free( node );                  /*  free mem */
        }

}


#if 0


/*
** del_bntree
**
** delete whole binary tree & data
**
*/
void del_bntree( struct bntree *tree, bt_del_func del_data )
{

    if ( bntree )

        if ( bntree->root ) {

            /* delete prev tree */
            while ( bntree->prev )
                del_btnode( tree, del_data );

            /* delete next tree */
            while ( bntree->next )
                del_btnode( tree, del_data );


        }

}



/*
** InsertPrevBtnode()
**
** InsertPrevBtnode
**
*/
BOOL InsertPrevBtnode( struct btnode *node, STRPTR newdata )
{
    struct btnode *oldprev;
    BOOL result;

    oldprev = node->prev;

    if ( node->prev = new_btnode() ) {

        node->prev->data = newdata;
        node->prev->prev = oldprev;
        result = TRUE;

    } else
        result = FALSE;

    return result;
}

/*
** InsertNextBtnode()
**
** InsertNextBtnode
**
*/
BOOL InsertNextBtnode( struct btnode *node, STRPTR newdata )
{
    struct btnode *oldnext;
    BOOL result;

    oldnext = node->next;

    if ( node->next = new_btnode() ) {

        node->next->data = newdata;
        node->next->next = oldnext;
        result = TRUE;

    } else

        result = FALSE;

    return result;

}

#endif
