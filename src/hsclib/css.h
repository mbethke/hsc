/*
 * This source code is part of hsc, a html-preprocessor,
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
 * hsclib/css.h
 *
 * Support functions for Cascading Stylesheet values
 *
 */

#ifndef HSC_CSS_H
#define HSC_CSS_H

#ifndef NOEXTERN_HSCLIB_CSS_H
HSCSTYLE *new_styleattr(CONSTRPTR name, CONSTRPTR value);
extern BOOL add_styleattr(HSCPRC *hp, CONSTRPTR property, CONSTRPTR value);
extern void del_styleattr(APTR data);
extern int cmp_style_node(ubi_btItemPtr item, ubi_btNodePtr node);
extern void free_style_node(ubi_btNode *node);
extern HSCSTYLE *add_styledef(hsctree *styles, CONSTRPTR name, CONSTRPTR value);
extern BOOL add_width_height_attrs(HSCPRC *hp, ULONG width, ULONG height);
#endif /* NOEXTERN_CSS_H */

#endif /* HSC_CSS_H */

/* $Id$ */
/* vi: set ts=4: */

