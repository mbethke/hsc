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

/*
 * structures & typdefs for entities
 */
typedef struct hscent
{
    STRPTR name;                /* name/id (eg &"uuml"; ) */
    short numeric;               /* numeric code of entity ( eg &#"123"; ) */
    char replace[2];            /* replace by (eg "ü" ) */
}
HSCENT;

/* value for hscent.numeric to mark icon entities */
#define ICON_ENTITY (-1)

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
extern void add_ent(DLLIST * entlist, STRPTR entid, char entreplace, short num);

#endif /* NOEXTERN_HSCLIB_ENTITY_H */

#endif /* HSCLIB_ENTITY_H */
