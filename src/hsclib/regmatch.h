/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2004 Matthias Bethke
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
 * hsclib/regmatch.h
 *
 * Wrapper functions for the GNU regular expression matching library
 *
 * created:  01-Oct-2004
 */

#ifndef HSC_REGMATCH_H
#define HSC_REGMATCH_H

#include "hsclib/hscprc.h"

#define hscregfree(r) regfree(r)
#define hscregfreeall(r) regfree(r); ufree(r)

/* a simple check for match, compiling the pattern on the fly */
extern BOOL hscregmatch(HSCPRC *hp, CONSTRPTR s, CONSTRPTR p, BOOL nocase);
/* a simple check for match using a precompiled regexp */
extern BOOL hscregmatch_pc(CONSTRPTR s, CONSTRPTR p, regex_t *re);
/* precompile a pattern to an existing regex_t */
extern BOOL hscregcomp_re(HSCPRC *hp, regex_t *re, CONSTRPTR p, BOOL nocase, BOOL fastmap);
/* precompile a pattern to a new regex_t */
extern regex_t *hscregcomp(HSCPRC *hp, CONSTRPTR p, BOOL nocase, BOOL fastmap);

#endif /* HSC_REGMATCH_H */

/* $Id$*/
/* vi: set ts=4: */
