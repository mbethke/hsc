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
 * hsclib/parse.h
 *
 * functions for parsing of hsc files
 *
 */

#ifndef HSC_PARSE_H
#define HSC_PARSE_H

/*
 * global funcs
 */
#ifndef NOEXTERN_HSCLIB_PARSE_H

extern BOOL hsc_parse(HSCPRC * hp);
extern BOOL hsc_parse_tag(HSCPRC * hp);
extern BOOL hsc_parse_amp(HSCPRC * hp);
extern BOOL hsc_parse_text(HSCPRC * hp);
extern BOOL hsc_parse_source(HSCPRC * hp);
extern BOOL hsc_parse_end(HSCPRC * hp);
extern BOOL hsc_parse_end_id(HSCPRC * hp);

extern void hp_enable_output(HSCPRC * hp, STRPTR cause);
extern DLNODE *find_end_tag_node(HSCPRC *hp, STRPTR tagname);
extern HSCTAG *find_end_tag(HSCPRC *hp, STRPTR tagname);
extern HSCTAG *find_end_container_macro(HSCPRC *hp);

extern HSCTAG *append_end_tag(HSCPRC * hp, HSCTAG * tag);
extern VOID remove_end_tag(HSCPRC * hp, HSCTAG * tag);

#endif /* NOEXTERN_PARSE_H */
#endif /* HSC_PARSE_H */
