/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2001-2003  Matthias Bethke
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
 * created: 23-Mar-2003
 */

#include "hsclib/inc_base.h"
#include "hsclib/css.h"

/*
 * cmp_stylename
 *
 * compares a string with a style element's name (for find_dlnode)
 */
static int cmp_stylename(const APTR cmpstr, const APTR vardata) {
   return upstrcmp((STRPTR)cmpstr, ((HSCSTYLE*)vardata)->name) ? 0 : -1;
}

/*
 * find_stylename
 */
static HSCSTYLE *find_stylename(DLLIST *stylelist, CONSTRPTR name) {
   DLNODE *nd = find_dlnode(stylelist->first, (APTR) name, cmp_stylename);

   if(nd) return (HSCSTYLE*)nd->data;
   return NULL;
}


/*
 * new_styleattr
 *
 * alloc & init a new style element
 */
static HSCSTYLE *new_styleattr(CONSTRPTR name, CONSTRPTR value)
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
VOID del_styleattr(APTR data)
{
#if DEBUG_ATTR
   fprintf(stderr, DHL "   del_styleattr %s (mci=%d)\n",
         var->name, var->macro_id);
#endif

   ufreestr(((HSCSTYLE*)data)->name);
   ufreestr(((HSCSTYLE*)data)->value);
   ufree(data);
}

/*
 * add_styleattr
 *
 * Adds a CSS style definition (property/value) to its list in hp. If a
 * property is present already, its value will be overwritten and a warning
 * issued. A TRUE return value means the property did not exist yet.
 */
BOOL add_styleattr(HSCPRC *hp, CONSTRPTR property, CONSTRPTR value) {
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
         if(NULL == (styledef = find_stylename(hp->defstyle,property))) {
            /* this property is unknown */
            hsc_message(hp, MSG_INVALID_STYLE,
                  "unknown CSS property %q", property);
         }else {
            /* property is OK, so check value */
            if((NULL != styledef->value) &&
                  !strenum(value,styledef->value,'|',STEN_CASE)) {
               /* defined property has non-empty value, but actual value was not
                * found therein */
               hsc_message(hp, MSG_INVALID_STYLE,
                     "value %q invalid for CSS property %q", value, property);

            }
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
BOOL add_width_height_attrs(HSCPRC *hp, ULONG width, ULONG height) {
   char buf[10+2+1];
   BOOL rw,rh;

   /* make string from integers */
   sprintf(buf,"%ldpx",width);
   rw = add_styleattr(hp,"width",buf);
   sprintf(buf,"%ldpx",height);
   rh = add_styleattr(hp,"height",buf);
   return (BOOL)(rw && rh);
} 

