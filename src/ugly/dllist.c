/*
 * dllist.c
 *
 * double linked list processing functions
 *
 * Copyright (C) 1994,95,96  Thomas Aglassinger
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
 * updated: 22-Mar-1996
 * created:  1-Mar-1994
 *
 *---------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "utypes.h"
#include "umemory.h"

/*
 *
 * include header file without external prototypes
 *
 */

#define NOEXTERN_UGLY_DLLIST_H

#include "dllist.h"

/*
 * init_dllist
 *
 * alloc & initialise new double linked list
 *
 * result: ptr to new list
 * errors: return NULL (out of mem)
 *
 */
struct dllist *init_dllist(void (*fn_del_data) (APTR data))
{
    struct dllist *newlist;

    newlist =                   /* alloc mem for list */
        umalloc(sizeof(struct dllist));

    if (newlist)
    {
        newlist->first = NULL;  /* init new list */
        newlist->last = NULL;
        newlist->entry_num = 0;
        newlist->user_data = NULL;
        newlist->del_data = fn_del_data;
    }

    return newlist;             /* return new list */
}

/*
 * new_dlnode()
 *
 * alloc & init new double linked list node
 *
 * result: new list node
 * errors: return NULL (out of mem)
 *
 */
struct dlnode *new_dlnode(void)
{
    struct dlnode *newnode;

    newnode =                   /* alloc mem for new node */
        umalloc(sizeof(struct dlnode));

    if (newnode)
    {                           /* alloc successful? */
        newnode->prev = NULL;   /* Y-> init node */
        newnode->next = NULL;
        newnode->data = NULL;
    }

    return newnode;             /* return newnode */
}

/* 
 * detach_dlnode()
 *
 * remove entry from double linked list,
 * but do not delete the entries data
 *
 */
APTR detach_dlnode(struct dllist * list, struct dlnode * node)
{
    APTR nd_data = NULL;

    if (list && node)
    {                           /* list & node node defined? */

        nd_data = node->data;   /*     remeber data */
        if (node->prev)         /*     remove node from list */
            node->prev->next = node->next;
        else
            list->first = node->next;
        list->entry_num--;

        if (node->next)
            node->next->prev = node->prev;
        else
            list->last = node->prev;

        ufree(node);            /*     free node */
    }

    return nd_data;
}

/* 
 * del_dlnode()
 *
 * remove entry from double linked list
 *
 */
void del_dlnode(struct dllist *list, struct dlnode *node)
{

    if (list && node)
    {                           /* list & node node defined? */

        void (*dd) (APTR) = list->del_data;

        if (node->data)         /* Y-> call destructor */
            if (dd)
                (*dd) (node->data);

        if (node->prev)         /*     remove node from list */
            node->prev->next = node->next;
        else
            list->first = node->next;
        list->entry_num--;

        if (node->next)
            node->next->prev = node->prev;
        else
            list->last = node->prev;

        node->prev = NULL;      /*     clear node */
        node->next = NULL;
        node->data = NULL;

        ufree(node);            /*     free node */
    }
}

/*
 * del_all_dlnodes()
 *
 * remove all nodes from a list
 */
VOID del_all_dlnodes(struct dllist *list)
{
    while (list->first)
        del_dlnode(list, list->first);
}

/*
 * ins_dlnode()
 *
 * insert a data entry into double linked list BEFORE node
 *
 */
struct dlnode *ins_dlnode(
                        struct dllist *list, struct dlnode *node, APTR data)
{

    struct dlnode *newnode = NULL;

    if (list)
    {

        newnode = new_dlnode();

        if (newnode)
        {

            newnode->data = data;
            list->entry_num++;

            if (node)
            {

                if (list->first == node)
                {

                    /*
                     * insert as first entry
                     */
                    list->first = newnode;

                }
                else
                {

                    /*
                     * insert somewhere in the list
                     */
                    node->prev->next = newnode;
                    newnode->prev = node->prev;

                }
                node->prev = newnode;
                newnode->next = node;

            }
            else
            {                   /* append as last entry */

                if (list->last)
                    list->last->next = newnode;
                newnode->prev = list->last;
                list->last = newnode;

            }

            if (list->first == NULL)
                list->first = newnode;
            if (list->last == NULL)
                list->last = newnode;

        }

    }

    return newnode;
}

/*
 * app_dlnode
 *
 * append new node to list (at end)
 *
 * result:
 * errors: return NULL (out of mem)
 *
 */
struct dlnode *app_dlnode(struct dllist *list, APTR data)
{

    struct dlnode *newnode = NULL;

    if (list)
    {

        newnode = ins_dlnode(list, NULL, data);

    }

    return newnode;

}

/*
 * del_dllist
 *
 * free whole list and its nodes and data
 *
 */
void del_dllist(struct dllist *list)
{

    if (list)
    {

        /* remove all nodes */
        while (list->first)
            del_dlnode(list, list->first);

        /* remove list structure */
        list->first = NULL;
        list->last = NULL;
        list->entry_num = 0;
        list->user_data = NULL;
        list->del_data = NULL;
        ufree(list);
    }

}

/*
 * do_dllist
 *
 * call a function with every node of a list
 */
void do_dllist(DLLIST * list, void (*func) (APTR data))
{
    if (list)
    {

        DLNODE *nd = list->first;

        while (nd)
        {

            (*func) (nd->data);
            nd = nd->next;
        }
    }

}

/*
 * fprintf_dllist
 *
 * output a whole list to a stream
 * (more or less only a debugging function)
 */
void fprintf_dllist(FILE * stream, struct dllist *list,
                    void (*fprintf_data) (FILE * stream, APTR data))
{

    if (list)
    {

        struct dlnode *node;
        ULONG i = 1;

        node = list->first;
        if (node)
        {

            while (node)
            {

                fprintf(stream, "%4lu: ", i++);
                if (fprintf_data)
                    (*fprintf_data) (stream, node->data);
                else
                    fprintf(stream, "%s\n", (STRPTR) node->data);
                node = node->next;

            }

        }
        else
            fprintf(stream, "  [empty list]\n");

    }
    else
        fprintf(stream, "  [no list]\n");

}

/*
 * find_dlnode
 *
 * search for specific data in a list, starting at a specific node
 *
 * params: start....startnode to begin search with
 *         data.....data to be found
 *         compare..funtions that compares two data-items;
 *                  returns -1 if items are equal, else 0
 * result: ptr to node that contains requested data
 *         or NULL if not found
 */
struct dlnode *find_dlnode(struct dlnode *start, APTR data,
                           int (*compare) (APTR cmp_data, APTR list_data))
{
    struct dlnode *search = start;
    int found = 0;

    while (search && !(found))
    {

        found = (*compare) (data, search->data);
        if (!found)
            search = search->next;

    }

    return search;

}

/*
 * empty_dllist
 *
 * check if a list is empty
 *
 * params: list..list to check for emptyness
 * result: TRUE, if list contains no nodes
 */
BOOL empty_dllist(struct dllist * list)
{
    if (list->first)
        return TRUE;
    else
        return FALSE;
}

