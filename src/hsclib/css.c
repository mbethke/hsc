/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2001-2004  Matthias Bethke
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
 * hsclib/css.c
 *
 * Support functions for Cascading Stylesheet values
 *
 */

#include "hsclib/inc_base.h"
#include "hsclib/css.h"
#include "hsclib/uri.h"
#include "regex/regex.h"

/*
 * cmp_stylename
 *
 * compares a string with a style element's name (for find_dlnode)
 */
static int cmp_stylename(const APTR cmpstr, const APTR vardata)
{
   return upstrcmp((STRPTR)cmpstr, ((HSCSTYLE*)vardata)->name) ? 0 : -1;
}

/*
 * find_style_in_tree
 */
static HSCSTYLE *find_style_in_tree(hsctree *styles, CONSTRPTR name)
{
   ubi_trNode *node = ubi_trFind(styles, name);
   return node ? *HSCTREENODEDP(node,HSCSTYLE*) : NULL;
}

/*
 * find_stylename: find named style in linked list
 */
static HSCSTYLE *find_stylename(DLLIST *stylelist, CONSTRPTR name)
{
   DLNODE *nd = find_dlnode(stylelist->first, (APTR) name, cmp_stylename);
   return nd ? (HSCSTYLE*)nd->data : NULL;
}

/*
 * new_styleattr
 *
 * alloc & init a new style element
 */
HSCSTYLE *new_styleattr(CONSTRPTR name, CONSTRPTR value)
{
   HSCSTYLE *newvar = (HSCSTYLE*)umalloc(sizeof(HSCSTYLE));

#if DEBUG_ATTR
   fprintf(stderr, DHL "   new_styleattr\n");
#endif

   if(newvar) {
      newvar->name  = strclone(name);
      newvar->value = strclone(value);
   }
   return newvar;
}

/*
 * del_styleattr
 *
 * delete style element
 */
void del_styleattr(APTR data)
{
#if DEBUG_ATTR
   fprintf(stderr, DHL "   del_styleattr %s (mci=%d)\n",
         var->name, var->macro_id);
#endif
   ufreestr(((HSCSTYLE*)data)->name);
   ufreestr(((HSCSTYLE*)data)->value);
   ufree(data);
}

/* ubiqx-tree comparison function for finding styles given a name */
int cmp_style_node(ubi_btItemPtr item, ubi_btNodePtr node)
{
   return upstrcmp((STRPTR)item, (*HSCTREENODEDP(node,HSCSTYLE*))->name);
}

/* free a style linked into a tree */
void free_style_node(ubi_btNode *node)
{
   del_styleattr(*HSCTREENODEDP(node,HSCSTYLE*));
   ufree(node);
}

HSCSTYLE *add_styledef(hsctree *styles, CONSTRPTR name, CONSTRPTR value)
{
   HSCSTYLE *newstyle = new_styleattr(name,value);
   if(newstyle) {
      ubi_trNode *oldnd, *node =  new_hsctreenode(newstyle);
      if(node) {
         if(ubi_sptInsert(&styles->r, node, newstyle->name, &oldnd)) {
            if(oldnd) {
               styles->delfunc(oldnd);
            }
            return newstyle;
         }
         ufree(node);
      }
      del_styleattr(newstyle);
   }
   return NULL;
}

/* non-ISO isblank() function is currently needed only here */
static int isblank(int c)
{
   if((c == ' ') || (c == '\t'))
      return TRUE;
   return FALSE;
}

#define nelems(a) (sizeof(a)/sizeof(*(a)))
static BOOL parse_stringset(CONSTRPTR *sp, CONSTRPTR * const values, int nvalues)
{
   int i;
   for(i=0; i<nvalues; ++i) {
      if(0 == upstrncmp(values[i],*sp,strlen(values[i]))) {
         *sp += strlen(values[i]);
         /* fprintf(stderr, "parse_stringset() found match '%s'\n",values[i]); */
         return TRUE;
      }
   }
   /* fprintf(stderr, "parse_stringset() no match\n"); */
   return FALSE;
}

static BOOL parse_units(CONSTRPTR *sp)
{
   static const char* values[] = {"pt","pc","in","mm","cm","px","em","ex"};
   return parse_stringset(sp,values,nelems(values));
}

static BOOL parse_frequency(CONSTRPTR *sp)
{
   static const char* values[] = {"Hz","kHz"};
   return parse_stringset(sp,values,nelems(values));
}

static BOOL parse_singlechar(CONSTRPTR *sp, const char c)
{
   if(c == **sp) {
      ++*sp;
      return TRUE;
   }
   return FALSE;
}

static BOOL parse_singlecharclass(CONSTRPTR *sp, int(*f)(int))
{
   if(f(**sp)) {
      ++*sp;
      return TRUE;
   }
   return FALSE;
}

static BOOL parse_d(CONSTRPTR *sp) /* decimal digits only */
{
   for(; '\0' != **sp; ++*sp) {
      if(!isdigit(**sp))
         return FALSE;
   }
   return TRUE;
}

static BOOL parse_n(CONSTRPTR *sp) /* numeric, positive only */
{
   CONSTRPTR s = *sp;

   parse_d(sp);                  /* first digits */
   if(s == *sp)
      return FALSE;
   if(parse_singlechar(sp,'.'))  /* optional point and more digits */
      s = *sp;
      parse_d(sp);
      if(s == *sp)
         return FALSE;
   return parse_units(sp);       /* finally, units */
}

static BOOL parse_N(CONSTRPTR *sp) /* numeric, positive/negative */
{
   parse_singlechar(sp,'-');     /* optional minus sign */
   return parse_n(sp);           /* next a numeric value with units */
}

static BOOL parse_p(CONSTRPTR *sp) /* numeric, positive only; percent */
{
   if(parse_n(sp))               /* a numeric value with units */
      return TRUE;
   else 
      return parse_singlechar(sp,'%'); /* check for percent */
}

static BOOL parse_P(CONSTRPTR *sp, BOOL eosonly) /* numeric, positive/negative; percent */
{
   BOOL negative = FALSE, ret=FALSE;

   if(parse_singlechar(sp,'-'))  /* optional minus sign */
      negative = TRUE;
   if(parse_n(sp)) {             /* next a numeric value with units */
      if(!eosonly || ('\0' == **sp))  /* successful if at EOS or flag */
         ret = TRUE;
   } else {
      if(parse_singlechar(sp,'%'))          /* check for percent */
         ret = !eosonly || (('\0' == **sp) && !negative); /* must be at EOS and not negative */
   }
   return ret;
}

static BOOL parse_rgbvalue(CONSTRPTR *sp)
{
   STRPTR e;
   long i = strtol(*sp,&e,10);
   
   if(!isdigit(**sp))
      return FALSE;     /* strtol() accepts signs, we don't */
   if(('%' == *e) && (i <= 100)) {
      *sp = e + 1; /* skip '%' too */
      return TRUE;
   }
   if(i > 255)
      return FALSE;
   *sp = e;
   return TRUE;
}

static BOOL parse_c(HSCPRC *hp, CONSTRPTR *sp) /* color */
{
   int l,i;

   if(strenum(*sp,hp->color_names,'|',STEN_CASE) > 0) {
      *sp += strlen(*sp);  /* advance past string */
      return TRUE;
   }
   if('#' == **sp) {
      ++*sp;
      l = strlen(*sp);
      if((l != 3) && (l != 6))
         return FALSE;
      for(; l; --l) {
         if(!isxdigit(**sp))
            return FALSE;
         ++*sp;
      }
      return TRUE;
   }
   if(0 == upstrncmp(*sp,"rgb(",4)) {
      *sp += 4;
      for(i=0; i<3; ++i) {
         if(!parse_rgbvalue(sp))
            return FALSE;
         if(i<2)
            if(!parse_singlechar(sp,','))
               return FALSE;
      } 
      if(!parse_singlechar(sp,')'))
         return FALSE;
      return ('\0' == **sp);
   }
   return FALSE;
}

static BOOL parse_u(HSCPRC *hp, CONSTRPTR *sp) /* URI */
{
   BOOL ret = FALSE;
   EXPSTR *dest = init_estr(256);
   parse_uri(hp,dest,(STRPTR)*sp);
   if(0 == strcmp(*sp,estr2str(dest))) {
      *sp += strlen(*sp);
      ret = TRUE;
   }
   fprintf(stderr,"parse_u(%s): %s [%s]\n",*sp,estr2str(dest),ret?"TRUE":"FALSE");
   del_estr(dest);
   return ret;
}

static BOOL parse_h(CONSTRPTR *sp) /* hertz/kilohertz */
{
   CONSTRPTR s = *sp;

   parse_d(sp);                  /* first digits */
   if(s == *sp)
      return FALSE;
   if(parse_singlechar(sp,'.')) {/* optional point and more digits */
      s = *sp;
      parse_d(sp);
      if(s == *sp);
         return FALSE;
   }
   return parse_frequency(sp);   /* then a Hz/kHz specification */
}

static BOOL parse_r(CONSTRPTR *sp) /* clipping rectangle */
{
   int i;

   if(0 == upstrncmp(*sp,"rect(",5)) {
      *sp += 5;
      while(parse_singlecharclass(sp,&isblank)) ;
      for(i=0; i<4; ++i) {
         if(!parse_P(sp,FALSE)) {
            if(0 != upstrncmp(*sp,"auto",4))
               return FALSE;
            *sp += 4;
         }
         if(i<3) {
            if(!parse_singlecharclass(sp,&isblank))
               return FALSE;
            while(parse_singlecharclass(sp,&isblank)) ;
         }
      } 
      while(parse_singlecharclass(sp,&isblank)) ;
      if(!parse_singlechar(sp,')'))
         return FALSE;
      while(parse_singlecharclass(sp,&isblank)) ;
      return ('\0' == **sp);
   }
   return FALSE;
}

 
/*
 * check_css_value
 *
 * Check the value of a style against a <$defstyle> pattern
 */
static BOOL check_css_value(HSCPRC *hp, CONSTRPTR def, CONSTRPTR val)
{
   BOOL ret = FALSE;
   CONSTRPTR s = val;

   /* fprintf(stderr,"#### Checking CSS: '%s' <-> '%s'\n",def,val); */
   
   /* a NULL in <def> permits any value for <val> */
   if(NULL == def)
      return TRUE;
   
   /* a NULL <val> is definitely illegal */
   if(NULL == val)
      return FALSE;
   
   /* if no special pattern specified, just check enumeration */
   if('%' != def[0])
      return (strenum(val,def,'|',STEN_CASE) > 0);

   /* actual enumeration, if present, is after "%X|" string */
   if(('\0' != def[2]) && (strenum(val,def+3,'|',STEN_CASE) > 0))
      return TRUE;
   
   /* enumeration did not match, so try an algorithmic check */
   switch(def[1]) {
      case 'd' : /* decimal digits only */
         ret = parse_d(&s);
         break;
      case 'n' : /* numeric, positive only */
         ret = parse_n(&s);
         break;
      case 'N' : /* numeric, positive/negative */
         ret = parse_N(&s);
         break;
      case 'p' : /* numeric, positive only; percent */
         ret = parse_p(&s);
         break;
      case 'P' : /* numeric, positive/negative; percent */
         ret = parse_P(&s,TRUE);
         break;
      case 'c' : /* color */
         ret = parse_c(hp,&s);
         break;
      case 'u' : /* URI */
         ret = parse_u(hp,&s);
         break;
      case 'h' :  /* hertz/kilohertz */
         ret = parse_h(&s);
         break;
      case 'r' : /* clipping rectangle */
         ret = parse_r(&s);
         break;
      default :
         panic("Unknown/uncaught format error in CSS definition");
   }
   /*
   fprintf(stderr, "## RET=%s, *s=%d -- ",(ret?"true":"false"),(int)*s);
   fprintf(stderr, (ret && ('\0' == *s)) ? "## SUCCESS\n" : "## FAILURE\n");
   */
   return (ret && ('\0' == *s));
}

/*
 * add_styleattr
 *
 * Adds a CSS style definition (property/value) to its list in hp. If a
 * property is present already, its value will be overwritten and a warning
 * issued. A TRUE return value means the property did not exist yet.
 */
BOOL add_styleattr(HSCPRC *hp, CONSTRPTR property, CONSTRPTR value)
{
   HSCSTYLE *styledef;

   /* check if CSS property is already defined for this tag */
   if(NULL != (styledef = find_stylename(hp->tag_styles,property))) {
      hsc_message(hp, MSG_STYLE_REDEFINED,
            "CSS property %q redefined, previous value was %q",
            property,styledef->value);
      ufreestr(styledef->value);
      styledef->value = strclone(value);
      return FALSE;
   } else {
      /* check if this is a valid property unless check was disabled */
      if(hp->validate_css) {
         if(NULL == (styledef = find_style_in_tree(hp->defstyle,property))) {
            /* this property is unknown */
            hsc_message(hp, MSG_INVALID_STYLE,
                  "unknown CSS property %q", property);
         }else {
            /* property is OK, so check value */
            if(!check_css_value(hp,styledef->value,value))
               hsc_message(hp, MSG_INVALID_STYLE,
                     "value %q invalid for CSS property %q", value, property);

         }
      }
      styledef = new_styleattr(property,value);
      app_dlnode(hp->tag_styles,styledef);
      return FALSE;
   }
}

/*
 * add_width_height_attrs
 *
 * Adds "width" and "height" properties to the current tag
 */
BOOL add_width_height_attrs(HSCPRC *hp, ULONG width, ULONG height)
{
   char buf[10+2+1]; /* 10 chars for MAX_ULONG, 2 for "px", 1 '\0' */
   BOOL rw,rh;

   /* make string from integers */
   sprintf(buf,"%ldpx",width);
   rw = add_styleattr(hp,"width",buf);
   sprintf(buf,"%ldpx",height);
   rh = add_styleattr(hp,"height",buf);
   return (BOOL)(rw && rh);
} 

/* $Id$ */
