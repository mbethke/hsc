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
 * hsclib/eval.h
 *
 * attribute value evaluation functions
 */

#ifndef HSCLIB_EVAL_H
#define HSCLIB_EVAL_H

/*
 *
 * extern references
 *
 */
#ifndef NOEXTERN_HSCLIB_EVAL_H

extern BOOL check_attrname(HSCPRC * hp, STRPTR name);

extern STRPTR eval_expression(HSCPRC * hp, HSCATTR * dest, STRPTR endstr);
extern STRPTR eval_cloneattr(HSCPRC * hp, HSCATTR *dest);

#endif /*  NOEXTERN_HSCLIB_EVAL_H */

#endif /* HSCLIB_EVAL_H */

