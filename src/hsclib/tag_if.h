/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * tag_if.h
 *
 * tag handles for <$IF> and <$ELSE>
 *
 */

#ifndef HSCLIB_TAG_IF_H
#define HSCLIB_TAG_IF_H

/*
 *
 * extern references
 *
 */
extern BOOL handle_hsc_if(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_cif(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_else(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_elseif(HSCPRC * hp, HSCTAG * tag);

extern VOID del_select_stack_node(APTR data);

#endif /* HSCLIB_TAG_IF_H */

