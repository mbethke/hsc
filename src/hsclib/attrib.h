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
 * hsclib/attribute.h
 *
 * attribute structure and funcs for hsc
 */

#ifndef HSC_ATTRIBUTE_H
#define HSC_ATTRIBUTE_H

/* variable types */
#define VT_NONE   0             /* no type; only after init */
#define VT_URI    1             /* uri */
#define VT_STRING 2             /* text string */
#define VT_BOOL   3             /* boolean */
#define VT_NUM    4             /* TODO: numeric */
#define VT_ENUM   5             /* enumerator */
#define VT_ID     6             /* TODO: id (name reference) */
#define VT_COLOR  7             /* TODO: jerk's color */

#define VT_STR_URI    "URI"     /* uri */
#define VT_STR_STRING "STRING"  /* text string */
#define VT_STR_BOOL   "BOOL"    /* boolean */
#define VT_STR_NUM    "NUM"     /* numeric */
#define VT_STR_ENUM   "ENUM"    /* enumerator */
#define VT_STR_ID     "ID"      /* id (name reference) */
#define VT_STR_COLOR  "COLOR"   /* color */

/* variable flags */
#define VF_ONLYONCE     (1<<0)  /* attribute may occure only once */
#define VF_REQUIRED     (1<<1)  /* attribute is required */
#define VF_CONST        (1<<2)  /* attribute is read-only: <$DEFINE> */
#define VF_JERK         (1<<3)  /* attribute only used by jerks */
#define VF_STRIPEXT     (1<<4)  /* URI: strip tag, if external */
#define VF_GETSIZE      (1<<5)  /* URI: get WIDTH & HEIGHT from here */
#define VF_RECOMMENDED  (1<<6)  /* attribute is recommended to appear */
#define VF_OBSOLETE     (1<<7)  /* attribute is obsolete/deprecated */

#define VF_KEEP_QUOTES  (1<<28) /* keep quotes untouched */
#define VF_GLOBAL       (1<<29) /* attribute is global: <$DEFINE> */
#define VF_MACRO        (1<<30) /* macro-attr */
#define VF_TAG          (1<<31) /* tag-attr (see note below) */

/*
 * NOTE on VF_TAG:
 *
 * Within uri-attributes, there is one problem: if you pass
 * an uri-attr to a macro or <$define>, the uri is parsed twice,
 * when attribute is passed to tag.
 * This produces shit when the uri is parsed the second time
 * (eg absolute uri is converted again)
 *
 * Therefore, uris are only parsed, if the VF_TAG-flag is
 * enabled. By default, VF_TAG is disabled and can only be
 * enabled when copying local macro attribute to the global
 * attribute list. (see "copy_local_varlist()" in "attrib.c")
 */

#define VF_CONST_STR       "CONST" /* attr is read only <$DEFINE> */
#define VF_CONST_SHT       "C"
#define VF_GLOBAL_STR      "GLOBAL"        /* global attribute <$DEFINE> */
#define VF_GLOBAL_SHT      "G"
#define VF_JERK_STR        "JERK"  /* attr only used by jerks */
#define VF_JERK_SHT        "J"
#define VF_ONLYONCE_STR    "ONLYONCE"      /* attr may appear only once in tag */
#define VF_ONLYONCE_SHT    "1"
#define VF_REQUIRED_STR    "REQUIRED"      /* attr is required */
#define VF_REQUIRED_SHT    "R"
#define VF_STRIPEXT_STR    "STRIPEXT"      /* strip tag, if URI is external */
#define VF_STRIPEXT_SHT    "X"
#define VF_GETSIZE_STR     "GETSIZE"      /* follow URI to get WIDTH & HEIGHT */
#define VF_GETSIZE_SHT     "Z"
#define VF_RECOMMENDED_STR "RECOMMENDED"   /* attribute is recommended */
#define VF_RECOMMENDED_SHT "RCMD"
#define VF_OBSOLETE_STR    "OBSOLETE"      /* attribute is obsolete */
#define VF_OBSOLETE_SHT    "O"

/* prefix for temporary attributes */
#define PREFIX_HSCATTR "HSC."
#define PREFIX_TMPATTR "HSC.TMP."

/* chars that act like opening/closing quote */
#define VQ_STR_QUOTE "\"'"

/* "no quote" value for quote in HSCATTR */
#define VQ_NO_QUOTE (0)

/* error return value for set_macro_args() to set var->macro_id with */
#define MCI_GLOBAL  (0)           /* indicate global attributes */
#define MCI_ERROR   (0xffffffff)
#define MCI_APPCTAG (0xfffffffe ) /* used by app_ctag(); see "tag.c" */

/* attribute structure */
typedef struct hscattr
{
    STRPTR name;                /* macro id */
    STRPTR deftext;             /* deftext text */
    STRPTR text;                /* text to be expanded to */
    STRPTR enumstr;             /* enumerator string */
    ULONG macro_id;             /* macro-call-id for local var */
    ULONG varflag;              /* flags; see VF_xx */
    int quote;                  /* quote char */
    BYTE vartype;               /* type; see VT_xx */
}
HSCATTR;

#define HSCVAR HSCATTR          /* TODO: remove */

/* a CSS element */
typedef struct hscstyle {
   STRPTR name;
   STRPTR value;
} HSCSTYLE;

/*
 * global funcs
 */
#ifndef NOEXTERN_HSCLIB_VARS_H

extern VOID prt_varlist(DLLIST * varlist, STRPTR title);

extern HSCATTR *new_hscattr(CONSTRPTR newname);
extern VOID del_hscattr(APTR data);
extern HSCATTR *cpy_hscattr(HSCATTR * oldvar);
extern HSCSTYLE *new_styleattr(CONSTRPTR name, CONSTRPTR value);
extern VOID del_styleattr(APTR data);

extern DLNODE *find_attrnode(DLLIST * varlist, STRPTR name);
extern HSCATTR *find_varname(DLLIST * varlist, STRPTR name);
extern HSCSTYLE *find_stylename(DLLIST *varlist, STRPTR name);
extern HSCATTR *app_var(DLLIST * varlist, STRPTR newname);

extern BOOL check_enumstr(HSCATTR * var, STRPTR value, INFILE * inpf);

extern STRPTR set_vartext(HSCATTR * var, STRPTR newtext);
extern BOOL set_varbool(HSCATTR * attr, BOOL value);
extern BOOL clr_vartext(HSCATTR * var);
extern VOID clr_attrdef(HSCATTR * attr);
extern BOOL clr_varlist(DLLIST * varlist);
extern VOID clr_varlist_bool(DLLIST * varlist);

extern STRPTR get_vartext_byname(DLLIST * varlist, STRPTR name);
extern STRPTR get_vartext(HSCATTR * var);
extern BOOL get_varbool_byname(DLLIST * varlist, STRPTR name);
extern BOOL get_varbool(HSCATTR * attr);
extern LONG get_varnum(HSCATTR * attr);
extern LONG get_varnum_byname(DLLIST * varlist, STRPTR name);

extern STRPTR get_vardeftext(HSCATTR * var);

#endif /* NOEXTERN_HSCLIB_ATTRIBUTE_H */
#endif /* HSCLIB_ATTRIBUTE_H */

