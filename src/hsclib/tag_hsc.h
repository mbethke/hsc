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
 * tag_hsc.h
 *
 * tag handles for hsc-tags
 *
 */

#ifndef HSCLIB_TAG_HSC_H
#define HSCLIB_TAG_HSC_H

/*
 *
 * extern references
 *
 */
extern BOOL handle_hsc_comment(HSCPRC * hp, HSCTAG * tag);

extern BOOL handle_hsc_defent(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_deficon(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_define(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_deftag(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_depend(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_exec(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_export(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_insert(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_include(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_lazy(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_let(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_message(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_verbatim(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_source(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_stripws(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_insert_expression(HSCPRC * hp, HSCTAG * tag);

#endif /* HSCLIB_TAG_HSC_H */

