/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2003 Matthias Bethke
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

/* P may be NULL, only used for error reporting */
BOOL hscregmatch_pc(CONSTRPTR s, CONSTRPTR p, regex_t *re)
{
   int regerr;
   const int slen = strlen(s);

   re_set_syntax(RE_SYNTAX_POSIX_EGREP);
   if(0 == (regerr = re_search(re,s,slen,0,slen,NULL)))
      return TRUE;
   if(-2 == regerr) {
      /* should not happen */
      fprintf(stderr,"** re_search(%s,%s): internal error\n",p?p:"<UNKNOWN-REGEX>",s);
   }
   return FALSE;
}

BOOL hscregmatch(HSCPRC *hp, CONSTRPTR s, CONSTRPTR p, BOOL nocase)
{
   static char translatemap[256];
   static BOOL tmap_init = TRUE;
   BOOL ret = FALSE;
   regex_t re;
   const char *regerrs;

   if(tmap_init) {
      /* Only initialize the translate map once. */
      int i;
      for(i=0; i<sizeof(translatemap); ++i)
         translatemap[i] = toupper((char)i);
      tmap_init = FALSE;
   }

   /* initialize regex structure before compiling */
   re.buffer = NULL;
   re.allocated = 0;
   re.fastmap = NULL;
   re.translate = nocase ? translatemap : NULL;
   re.no_sub = 1;
   
   if((regerrs = re_compile_pattern(p,strlen(p),&re))) {
      /* TODO: proper error reporting */
      fprintf(stderr,"regcomp(%s): %s\n",p,regerrs);
   } else {
      ret = hscregmatch_pc(s,p,&re);
      regfree(&re);
   }
   return ret;
}

/* $Id$*/
/* vi: set ts=4: */
