/*
** tag.h
**
** tag structure and functions
**
*/

#ifndef HSC_TAG_H
#define HSC_TAG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"

/*
** defines
*/

/* TODO: remove tag kinds */
/* tag kinds */
#define TK_NONE  0                     /* no kind; only after init */
#define TK_TAG   1                     /* normal tag */
#define TK_MACRO 2                     /* macro (file) */
#define TK_ARG   3                     /* arg text */

#define HSC_TAGID        "$"
#define HSC_COMMENT_STR  "*"
#define HSC_ONLYCOPY_STR "|"
#define HSC_DEFENT_STR   "DEFENT"
#define HSC_DEFTAG_STR   "DEFTAG"
#define HSC_MACRO_STR    "MACRO"
#define HSC_INCLUDE_STR  "INCLUDE"
#define HSC_INSERT_STR   "INSERT"

#define HSC_TEXT_STR     "TEXT"
#define HSC_TIME_STR     "TIME"

/*
** structure & typdef for tag
*/
typedef struct hsctag {
    STRPTR name;                       /* tag name, eg "TITLE" */
    ULONG  option;                     /* tag options, eg HT_CLOSE|HT_REQUIRED */
    LONG   vers;                       /* tag version, eg 10 for 1.0 */
    BOOL   (*o_handle)(INFILE *inpf, struct hsctag *tag );  /* function to handle tag */
    BOOL   (*c_handle)(INFILE *inpf, struct hsctag *tag );  /* function to handle closing tag */
    DLLIST *op_args;                   /* list of arguments (open/close) */
    DLLIST *cl_args;
    EXPSTR *op_text;                   /* macro text (open/close) */
    EXPSTR *cl_text;
    ULONG  nest_count;                 /* nesting counter */
    STRPTR only_inside;                /* string that tells inside which */
                                       /*   tag this tag has to be */
                                       /*   e.g. for <LI>: "ul|ol|dir|menu" */
    BOOL   occured;                    /* TRUE, if tag already occured */
} HSCTAG;

/*
** defines
*/
#define HT_NOCOPY      (1<<0)  /* avoid copying of tag text */
#define HT_CLOSE       (1<<1)  /* closing tag required */
#define HT_REQUIRED    (1<<2)  /* tag required at least once in file */
#define HT_ONLYONCE    (1<<3)  /* tag required at most once in file */
#define HT_IGNOREARGS  (1<<4)  /* all tag args are ignored */
#define HT_OBSOLETE    (1<<5)  /* tag is already obsolete */
#define HT_JERK        (1<<6)  /* netscape externsion & co. */
#define HT_NONEST      (1<<7)  /* TODO: remove tag allows no nesting */
#define HT_NOBP        (1<<8)  /* TODO: warning if <P> before tag */
#define HT_NOAP        (1<<9)  /* TODO: -"- after tag */
#define HT_MACRO       (1<<10) /* macro tag */
#define HT_NOHANDLE    (1<<11) /* don't call tag handles */
#define HT_NONESTING   (1<<12) /* not nestable (<A>) */
#define HT_SKIPLF      (1<<13) /* skip possible LF after tag */

/* tag options that can be set via DEFTAG */
#define TO_CLOSE_STR       "CLOSE"
#define TO_CLOSE_SHT       "C"
#define TO_IGNOREARGS_STR  "IGNOREARGS"
#define TO_IGNOREARGS_SHT  "IGA"
#define TO_JERK_STR        "JERK"
#define TO_JERK_SHT        "J"
#define TO_NOCOPY_STR      "NOCOPY"
#define TO_NOCOPY_SHT      "NCP"
#define TO_NOHANDLE_STR    "NOHANDLE"
#define TO_NOHANDLE_SHT    "NHD"
#define TO_NONESTING_STR   "NONESTING"
#define TO_NONESTING_SHT   "NST"
#define TO_ONLYONCE_STR    "ONLYONCE"
#define TO_ONLYONCE_SHT    "1"
#define TO_OBSOLETE_STR    "OBSOLETE"
#define TO_OBSOLETE_SHT    "O"
#define TO_REQUIRED_STR    "REQUIRED"
#define TO_REQUIRED_SHT    "R"
#define TO_SKIPLF_STR      "SKIPLF"
#define TO_SKIPLF_SHT      "S"

/* todo: think about this tag-options */
#define TO_VERS_STR        "VERS"
#define TO_VERS_SHT        "V"



/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_TAG_H

/*
** global vars
*/
extern DLLIST *deftag;
extern DLLIST *cltags;
extern DLLIST *hsctags; /* TODO: remove this */

extern STRPTR  last_anchor;

extern ULONG tag_call_id;

/*
** global funcs
*/
extern HSCTAG *new_hsctag( STRPTR newid );
extern void del_tag( APTR data );

extern int     cmp_strtag( APTR cmpstr, APTR tagdata );
extern HSCTAG *find_strtag( DLLIST *taglist, STRPTR name );
extern int     cmp_strctg( APTR cmpstr, APTR tagstr );

extern HSCTAG *app_tag( DLLIST *taglist, STRPTR tagid );

extern HSCTAG *def_tag_name( DLLIST *taglist, INFILE *inpf, BOOL *open_tag );
extern BOOL def_tag_args( DLLIST *taglist, HSCTAG *tag, INFILE *inpf, BOOL *open_tag );

extern ULONG set_tag_args( HSCTAG *tag, INFILE *inpf, BOOL open_tag );


#endif /* NOEXTERN_HSC_TAG_H */

#endif /* HSC_TAG_H */
