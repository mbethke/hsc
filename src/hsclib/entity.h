/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * entity.h
 *
 * entity structure, variables and functions
 *
 */

#ifndef HSCLIB_ENTITY_H
#define HSCLIB_ENTITY_H

/*
 * defines
 */
/* convert numeric representation to text. pass an array as "buffer", not a pointer! */
#define ENTITY_NUM2TEXT(buffer,num) \
   (snprintf((buffer), sizeof(buffer), "&%d;",(int)(unsigned short)(num)))
/* value for hscent.numeric to mark icon entities */
#define ICON_ENTITY (-1)
/* flag values for entities */
#define HSCENTF_PREFNUM (0x01)
#define HSCENTF_NONSTD  (0x02)

/*
 * structures & typdefs for entities
 */
typedef struct hscent {
    STRPTR name;        /* name/id (e.g. &"uuml"; ) */
    int numeric;        /* numeric code of entity (e.g. 252) */
    char replace[2];    /* replaces this 8-bit character (e.g. "ü" ) */
    char flags;         /* see definitions above */
} HSCENT;


/*
 *
 * extern references
 *
 */
#ifndef NOEXTERN_HSCLIB_ENTITY_H

extern HSCENT *new_hscent(STRPTR newid);
extern VOID del_entity(APTR data);
extern HSCENT *cpy_hscent(HSCENT * oldent);

extern int cmp_strent(APTR cmpstr, APTR entdata);
extern int cmp_nument(APTR cmpstr, APTR entdata);
extern int cmp_rplcent(APTR cmpstr, APTR entdata);

extern HSCENT *app_entnode(DLLIST * entlist, STRPTR entid);
extern void add_ent(DLLIST * entlist, STRPTR entid, char entreplace, short num, char flags);

#endif /* NOEXTERN_HSCLIB_ENTITY_H */
#endif /* HSCLIB_ENTITY_H */

/* $Id$ */
/* vi: set ts=4: */

