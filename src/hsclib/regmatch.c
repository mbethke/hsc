/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2004-2005 Matthias Bethke
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
 * hsclib/regmatch.c
 *
 * Wrapper functions for the GNU regular expression matching library
 *
 * created:  01-Oct-2004
 */

#include "hsclib/regmatch.h"
#include "hsclib/inc_base.h"

static char lowercasemap[256];

/* P may be NULL, only used for error reporting */
BOOL hscregmatch_pc(CONSTRPTR s, CONSTRPTR pattern, regex_t *re)
{
   int regerr;
   const int slen = strlen(s);

   if(0 == (regerr = re_search(re,s,slen,0,slen,NULL)))
      return TRUE;
   if(-2 == regerr) {
      /* should not happen */
      fprintf(stderr,"** re_search(%s,%s): internal error\n",pattern?pattern:"<UNKNOWN-REGEX>",s);
   }
   return FALSE;
}

BOOL hscregmatch(HSCPRC *hp, CONSTRPTR s, CONSTRPTR pattern, BOOL nocase)
{
   BOOL ret = FALSE;
   regex_t re;
   if(hscregcomp_re(hp,&re,pattern,nocase,NULL)) {
      ret = hscregmatch_pc(s,pattern,&re);
      regfree(&re);
   }
   return ret;
}

/* precompile a pattern to an existing regex_t */
BOOL hscregcomp_re(HSCPRC *hp, regex_t *re, CONSTRPTR pattern, BOOL nocase, char *fastmap)
{
   static BOOL lcmap_init = TRUE;
   const char *regerrs;

   if(lcmap_init) {
      /* Only initialize the translate map once. */
      int i;
      for(i=0; i<sizeof(lowercasemap); ++i)
         lowercasemap[i] = toupper((char)i);
      lcmap_init = FALSE;
      re_set_syntax(RE_SYNTAX_POSIX_EGREP);
   }

   /* initialize regex structure before compiling */
   re->buffer = NULL;
   re->allocated = 0;
   re->fastmap = fastmap;
   re->translate = nocase ? lowercasemap : NULL;
   re->no_sub = 1;
   
   if((regerrs = re_compile_pattern(pattern,strlen(pattern),re))) {
      if(hp) {
         hsc_message(hp, MSG_ILLG_REGEX, "error in regular expression %q: %s",
               pattern,regerrs);
      }
      return FALSE;
   }
   if(fastmap && (-2 == re_compile_fastmap(re))) {
      regfree(re);
      fprintf(stderr,"** re_compile_fastmap(): internal error\n");
      return FALSE;
   }
   return TRUE;
}

/* precompile a pattern to a new regex_t */
regex_t *hscregcomp(HSCPRC *hp, CONSTRPTR pattern, BOOL nocase, BOOL fastmap)
{
   char *fmap = fastmap ? umalloc(256) : NULL;
   regex_t *re = ucalloc(1,sizeof(regex_t));
   
   if(!hscregcomp_re(hp,re,pattern,nocase,fmap)) {
      ufree(re);
      re = NULL;
      if(fmap) ufree(fmap);
   }
   return re;
}

/* $Id$*/
/* vi: set ts=4: */
