/*
** vars.h
**
** hsc-variable funcs for hsc
**
*/

#ifndef HSC_VARS_H
#define HSC_VARS_H

#include "ugly/types.h"
#include "ugly/expstr.h"

#include "global.h"

/* variable types */
#define VT_NONE   0                    /* no type; only after init */
#define VT_URI    1                    /* uri */
#define VT_STRING 2                    /* text string */
#define VT_BOOL   3                    /* boolean */
#define VT_NUM    4                    /* numeric */
#define VT_ENUM   5                    /* enumerator */
#define VT_TEXT   6                    /* text (without quotes) */

#define VT_STR_URI    "URI"            /* uri */
#define VT_STR_STRING "STRING"         /* text string */
#define VT_STR_BOOL   "BOOL"           /* boolean */
#define VT_STR_NUM    "NUM"            /* numeric */
#define VT_STR_ENUM   "ENUM"           /* enumerator */
#define VT_STR_TEXT   "TEXT"           /* text (without quotes) TODO: remove */

/* variable flags */
#define VF_ONLYONCE   (1<<0)           /* attribute may occure only once */
#define VF_REQUIRED   (1<<1)           /* attribute is required */
#define VF_NOQUOTE    (1<<2)           /* does not require quote */
#define VF_JERK       (1<<3)           /* attribute for jerks */
#define VF_MACRO      (1<<7)           /* macro attr (see below) */

/*
** NOTE on VF_MACRO:
**
** within uri-attributes, there is one problem: if you pass
** an uri-attr to a macro, the uri is parsed twice if the
** macro is called. this produces shit when the uri is
** parsed the second time (eg absolute uri is converted again)
**
** therefor, uris are not parsed, if the VF_MACRO-flag is
** enabled. by default, VF_MACRO is disabled and can only be
** enabled when copying local macro attribute to the global
** attribute list. (see "copy_local_varlist()" in "vars.c")
*/

#define VF_JERK_STR     "JERK"         /* attr only used by jerks */
#define VF_JERK_SHT     "J"
#define VF_ONLYONCE_STR "ONLYONCE"     /* attr mey appear only once in tag */
#define VF_ONLYONCE_SHT "1"
#define VF_REQUIRED_STR "REQUIRED"     /* attr is required */
#define VF_REQUIRED_SHT "R"
#define VF_NOQUOTE_STR  "NOQUOTE"      /* value doesn't require quotes */
#define VF_NOQUOTE_SHT  "Q"

/* chars that act like opening/closing quote */
#define VQ_STR_QUOTE "\"'<"

/* "no quote" value for quote in HSCVAR */
#define VQ_NO_QUOTE 0

/* error return value for set_macro_args() to set var->macro_id with */
#define MCI_ERROR   0xffffffff
#define MCI_APPCTAG 0xfffffffe /* used by app_ctag(); see "tag.c" */


/* variable structure */
typedef struct hscvar {
    STRPTR name;                       /* macro id */
    STRPTR deftext;                    /* deftext text */
    STRPTR text;                       /* text to be expanded to */
    STRPTR enumstr;                    /* enumerator string */
    ULONG  macro_id;                   /* macro-call-id for local var */
    int    quote;                      /* quote char */
    BYTE   vartype;                    /* type; see VT_xx */
    UBYTE  varflag;                    /* flags; see VF_xx */
} HSCVAR;


/*
** global funcs
*/
#ifndef NOEXTERN_HSC_VARS

extern DLLIST *vars;
extern EXPSTR *vararg;

extern void prt_varlist( DLLIST *varlist, STRPTR title );

extern HSCVAR *new_hscvar( STRPTR newname );
extern void del_var( APTR data );

extern HSCVAR *find_varname( DLLIST *varlist, STRPTR name );
extern HSCVAR *app_var( STRPTR newname );

extern STRPTR parse_vararg( HSCVAR *var, INFILE *inpf);
extern STRPTR set_vartext( HSCVAR *var, STRPTR newtext );
extern BOOL   clr_vartext( HSCVAR *var );
extern BOOL   clr_varlist( DLLIST *varlist );
extern STRPTR get_vartext( DLLIST *varlist, STRPTR name );
extern BOOL   get_varbool( DLLIST *varlist, STRPTR name );

/* attribute list manipulation */
extern BOOL copy_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci );
extern BOOL set_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci );
extern void remove_local_varlist( DLLIST *varlist, ULONG mci );
extern BOOL check_varlist( DLLIST *varlist, INFILE *inpf );

extern HSCVAR *define_var( STRPTR varname, DLLIST *varlist, INFILE *inpf, UBYTE flag );

extern BYTE str2vartype( STRPTR s );

#endif /* NOEXTERN_HSC_VARS */
#endif /* HSC_VARS_H */
