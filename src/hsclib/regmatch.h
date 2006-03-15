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

/* a simple check for match, compiling the pattern on the fly */
extern BOOL hscregsearch(HSCPRC *hp, CONSTRPTR s, CONSTRPTR pattern, BOOL nocase);
/* a simple check for match using a precompiled regexp */
extern BOOL hscregsearch_pc(CONSTRPTR s, CONSTRPTR pattern, regex_t *re, struct re_registers *regs);
/* precompile a pattern to an existing regex_t */
extern BOOL hscregcomp_re(HSCPRC *hp, regex_t *re, CONSTRPTR pattern, BOOL nocase,  char *fastmap);
/* precompile a pattern to a new regex_t */
extern regex_t *hscregcomp(HSCPRC *hp, CONSTRPTR pattern, BOOL nocase, BOOL fastmap);
/* free a regex_t structure initialized by hscregcomp or hscregcomp_re, including subfields */
void hscregfreeall(regex_t *r);

#endif /* HSC_REGMATCH_H */

/* $Id$*/
/* vi: set ts=4: */
