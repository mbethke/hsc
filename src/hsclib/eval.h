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
 * hsclib/eval.h
 *
 * attribute value evaluation functions
 */

#ifndef HSCLIB_EVAL_H
#define HSCLIB_EVAL_H

#define DOUBLE_QUOTE '\"'
#define SINGLE_QUOTE '\''
#define BACK_QUOTE   '`'

/*
 *
 * extern references
 *
 */
#ifndef NOEXTERN_HSCLIB_EVAL_H

extern STRPTR check_attrname(HSCPRC* hp, HSCATTR *dest, STRPTR name, BOOL allow_expr);
extern void choose_quote(HSCPRC * hp, HSCATTR * attr);

extern STRPTR eval_expression(HSCPRC * hp, HSCATTR * dest, STRPTR endstr);
extern STRPTR eval_conditional_assignment(HSCPRC *hp, HSCATTR *dest);
extern STRPTR eval_attrref(HSCPRC * hp, HSCATTR * destattr);

#endif /*  NOEXTERN_HSCLIB_EVAL_H */

#endif /* HSCLIB_EVAL_H */

/* $Id$ */
/* vi: set ts=4: */

