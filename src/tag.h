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

/* tag kinds */
#define TK_NONE  0                     /* no kind; only after init */
#define TK_TAG   1                     /* normal tag */
#define TK_MACRO 2                     /* macro (file) */
#define TK_ARG   3                     /* arg text */

#define STR_HSC_MACRO   "HSC_MACRO"
#define STR_HSC_INCLUDE "HSC_INCLUDE"
#define STR_HSC_TIME    "HSC_TIME"
#define STR_HSC_VAR     "HSC_VAR"

/*
** structure & typdef for tag
*/
typedef struct hsctag {
    STRPTR name;                       /* tag name, eg "TITLE" */
    ULONG  option;                     /* tag options, eg HT_CLOSE|HT_REQUIRED */
    LONG   vers;                       /* tag version, eg 10 for 1.0 */
    BOOL   (*o_handle)(INFILE *inpf);  /* function to handle tag */
    BOOL   (*c_handle)(INFILE *inpf);  /* function to handle closing tag */
    DLLIST *op_args;                   /* list of arguments (open/close) */
    DLLIST *cl_args;
    STRPTR op_fname;                   /* name of temp file that holds text */
    STRPTR cl_fname;                   /*     for opening/closing macro */
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
#define HT_IGNORE_ARGS (1<<4)  /* all tag args are ignored */
#define HT_OBSOLETE    (1<<5)  /* tag is already obsolete */
#define HT_JERK        (1<<6)  /* netscape externsion & co. */
#define HT_NONEST      (1<<7)  /* tag allows no nesting */
#define HT_NOBP        (1<<8)  /* warning if <P> before tag */
#define HT_NOAP        (1<<9)  /* -"- after tag */
#define HT_MACRO       (1<<10) /* macro tag */
#define HT_NOHANDLE    (1<<11) /* don't call tag handles */
#define HT_WARNARGS    (1<<31) /* display warning when unknown args found */

/* tag options that can be set via DEFTAG */
#define TO_JERK_STR        "JERK"
#define TO_JERK_SHT        "JKR"
#define TO_NOCOPY_STR      "NOCOPY"
#define TO_NOCOPY_SHT      "NCP"
#define TO_NOHANDLE_STR    "NOHANDLE"
#define TO_NOHANDLE_SHT    "NHD"
#define TO_ONLYONCE_STR    "ONLYONCE"
#define TO_ONLYONCE_SHT    "ONC"
#define TO_OBSOLETE_STR    "OBSOLETE"
#define TO_OBSOLETE_SHT    "OBS"
#define TO_REQUIRED_STR    "REQUIRED"
#define TO_REQUIRED_SHT    "REQ"

/* todo: think about this tag-options */
#define TO_CLOSE_STR       "CLOSE"
#define TO_CLOSE_SHT       "CLS"
#define TO_IGNORE_ARGS_STR "IGNOREARGS"
#define TO_IGNORE_ARGS_SHT "IGA"
#define TO_VERS_STR        "VERS"
#define TO_VERS_SHT        "VRS"



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

extern char    this_tag[MAX_TAGLEN];
extern HSCTAG *this_tag_data;
extern STRPTR  last_anchor;

extern ULONG tag_call_id;

/* nesting vars */
extern ULONG anchor_nesting;
extern ULONG body_nesting;
extern ULONG head_nesting;
extern ULONG list_nesting;
extern ULONG title_nesting;

/*
** global funcs
*/
extern HSCTAG *new_hsctag( STRPTR newid );
extern void del_tag( APTR data );

extern int     cmp_strtag( APTR cmpstr, APTR tagdata );
extern HSCTAG *find_strtag( STRPTR name );
extern int     cmp_strctg( APTR cmpstr, APTR tagstr );

extern HSCTAG *app_tag( STRPTR tagid );
#if 0
extern HSCTAG *add_tag(
  STRPTR tagid,
  ULONG  optn,
  ULONG  version,
  BOOL   (*op_handle)(INFILE *inpf),
  BOOL   (*cl_handle)(INFILE *inpf) );

extern HSCTAG *add_stag( STRPTR tagid, ULONG optn );
#endif

extern HSCTAG *def_tag_name( INFILE *inpf, BOOL *open_tag );
extern BOOL def_tag_args( HSCTAG *tag, INFILE *inpf, BOOL *open_tag );

extern ULONG set_tag_args( HSCTAG *tag, INFILE *inpf, BOOL open_tag );


#endif /* NOEXTERN_HSC_TAG_H */

#endif /* HSC_TAG_H */
