/*
** vars.h
**
** hsc-variable funcs for hsc
**
** updated:  4-Sep-1995
** created:  2-Sep-1995
*/

#ifndef HSC_VARS_H
#define HSC_VARS_H

#include "ugly/types.h"

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
#define VT_STR_TEXT   "TEXT"           /* text (without quotes) */

/* variable flags */
#define VF_ONLYONCE   (1<<0)
#define VF_REQUIRED   (1<<1)

#define VF_STR_ONLYONCE "ONLYONCE"     /* arg mey appear only once in tag */
#define VF_STR_REQUIRED "REQUIRED"     /* arg is required */

/* chars that act like opening/closing quote */
#define VQ_STR_QUOTE "\"'<"

/* "no quote" value for quote in HSCVAR */
#define VQ_NO_QUOTE 0

/* error return value for set_macro_args() to set var->macro_id with */
#define MCI_ERROR 0xffffffff


/* variable structure */
typedef struct hscvar {
    STRPTR name;                       /* macro id */
    STRPTR deftext;                    /* deftext text */
    STRPTR text;                       /* text to be expanded to */
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

extern void prt_varlist( DLLIST *varlist, STRPTR title );

extern HSCVAR *new_hscvar( STRPTR newname );
extern void del_var( APTR data );

extern HSCVAR *find_varname( DLLIST *varlist, STRPTR name );
extern HSCVAR *app_var( STRPTR newname );

extern STRPTR parse_vararg( HSCVAR *var, INFILE *inpf);
extern STRPTR set_vartext( HSCVAR *var, STRPTR newtext );
extern BOOL   clr_vartext( HSCVAR *var );
extern BOOL   clr_varlist( DLLIST *varlist );

extern BOOL   copy_local_varlist( DLLIST *varlist, ULONG mci );
extern void   remove_local_varlist( ULONG mci );

extern HSCVAR *define_var( STRPTR varname, DLLIST *varlist, INFILE *inpf, UBYTE flag );

extern BYTE str2vartype( STRPTR s );

#endif /* NOEXTERN_HSC_VARS */
#endif /* HSC_VARS_H */
