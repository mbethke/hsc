/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2004  Matthias Bethke
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
 * hsclib/hsctrees.h
 *
 * Structures and support macros to use trees from the ubiqx package in hsc
 *
 */

#ifndef HSC_HSCTREES_H
#define HSC_HSCTREES_H

#include "ugly/utypes.h"
#include "ubiqx/ubi_SplayTree.h"

/* Get the Data Pointer from an HSC tree node */
#define HSCTREENODEDP(n,T) ((T*)((n)+1))

typedef struct HSCTREE {
  ubi_btRoot r;
  ubi_btKillNodeRtn delfunc;
} hsctree;

extern hsctree *init_hsctree(ubi_btKillNodeRtn, ubi_btCompFunc, unsigned);
extern void del_hsctree(hsctree*);
ubi_trNode *new_hsctreenode(APTR);

#endif  /* HSC_HSCTREES_H */

