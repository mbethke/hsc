/*
 * This source code is part of hsc, an html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * entity.c
 *
 * entity structure, variables and functions ( "&xx;")
 *
 */

#define NOEXTERN_HSCLIB_ENTITY_H
#include "hsclib/inc_base.h"
#include "hsclib/entity.h"

/*
 *-------------------------------------
 * constructor/destructor
 *-------------------------------------
 */

/*
 * new_hscent
 *
 * alloc & init a new hscentity
 */
HSCENT *new_hscent(STRPTR newid) {
    HSCENT *newent = (HSCENT*) umalloc(sizeof(HSCENT));

#if DEBUG_ENTITY
    fprintf(stderr, DHL "   new_enty %s\n", newid);
#endif

    if (newent) {
       /* init new entity item */
       newent->name = strclone(newid);
       newent->replace[0] = newent->replace[1] = '\0';
       newent->numeric = 0;
       newent->prefnum = FALSE;
    }
    return (newent);
}

/*
 * del_entity
 */
VOID del_entity(APTR data) {
    HSCENT *ent = (HSCENT*)data;

#if DEBUG_ENTITY
    fprintf(stderr, DHL "   del_enty %s\n", ent->name);
#endif

    ufreestr(ent->name);
    ent->replace[0] = '\0';
    ent->numeric = 0;
    ufree(ent);
}

/*
 * cpy_hscent
 *
 * create a copy of an already existing entity
 */
HSCENT *cpy_hscent(HSCENT * oldent) {
    HSCENT *newent = new_hscent(oldent->name);

    if (newent) {
        newent->replace[0] = oldent->replace[0];
        newent->numeric = oldent->numeric;
    }
    return (newent);
}

/*
 *---------------------------
 * find entity string
 *---------------------------
 */

/*
 * cmp_strent
 *
 * compares a entity-string with the name
 * of a HSCENT-entry
 */
int cmp_strent(APTR cmpstr, APTR entdata) {
    STRPTR entstr = NULL;

    if (entdata)
        entstr = ((HSCENT*)entdata)->name;

    if (entstr) {
        if (!strcmp(cmpstr, entstr))
            return -1;
        else
            return 0;
    }
    return 0;
}

/*
 * cmp_nument
 *
 * compares an entity-string with the numeric
 * data of an HSCENT-entry
 */
int cmp_nument(APTR cmpstr, APTR entdata) {
    LONG num = -1;
    LONG cmpnum = (LONG) cmpstr;

    if (entdata)
        num = ((HSCENT *) entdata)->numeric;

    return ((num != -1) && (num == cmpnum));
}

/*
 * cmp_rplcent
 *
 * compares an entity-string with the replace-item
 * of an HSCENT-entry
 */
int cmp_rplcent(APTR cmpstr, APTR entdata) {
    STRPTR entstr = NULL;

    if (entdata)
        entstr = ((HSCENT*)entdata)->replace;

    if (entstr) {
        if (!strcmp(cmpstr, entstr))
            return -1;
        else
            return 0;
    }
    return 0;
}

/*
 *-------------------------------------
 * append entity functions
 *-------------------------------------
 */

/*
 * app_entnode
 *
 * create a new entity and append it to entity-list
 *
 * params: entid..name of the new entity (eg "uuml")
 * result: ptr to the new entity or NULL if no mem
 */
HSCENT *app_entnode(DLLIST * entlist, STRPTR entid) {
    HSCENT *newent;

    newent = new_hscent(entid);
    if(NULL != newent) {
       if (app_dlnode(entlist, newent) == NULL) {
          del_entity((APTR) newent);
          newent = NULL;
       }
    }
    return (newent);
}

/*
 * add_ent
 */
void add_ent(DLLIST * entlist, STRPTR entid, char entreplace, short num, BOOL prefnum) {
    HSCENT *newent = app_entnode(entlist, entid);

    if(NULL != newent) {
       newent->replace[0] = entreplace;
       newent->numeric = num;
       newent->prefnum = prefnum;
    }
}

/* $Id$ */
/* vi: set ts=4: */
