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
 * hsclib/tag_misc.h
 *
 * misc. tag-callbacks
 *
 */

#ifndef HSC_TAG_MISC_H
#define HSC_TAG_MISC_H

/*
 *
 * extern references
 *
 */
extern BOOL handle_sgml_comment(HSCPRC * hp, HSCTAG * tag);

extern BOOL handle_base(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_blink(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_frame(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_heading(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_img(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_pre(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_end_pre(HSCPRC * hp, HSCTAG * tag);

#endif /* HSC_TAG_MISC_H */
