/*
** global.h
**
** global variables and functions for hsc
**
** updated:  2-Sep-1995
** created:  8-Jul-1995
*/

#ifndef HSC_GLOBAL_H
#define HSC_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"

/*
** defines
*/
#define HSC_DEFNAME "hscdef.cfg"       /* file that defines tags & entities */
#define HSC_OPTNAME "hscopt.cfg"       /* file that sets default options */

#define MAXLINELEN  1023               /* input line */
#define MAX_ARGLEN   511               /* arguments */
#define MAX_TAGLEN    63               /* tag id */
#define MAX_PATHLEN  255               /* directory path & filename */
#define MAX_URLLEN   255               /* URLs */

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
** structures & typdefs for entities, tags and macros
*/
typedef struct hscent {
    STRPTR name;
    STRPTR source;
} HSCENT;

typedef struct hsctag {
    STRPTR name;                       /* tag name, eg "TITLE" */
    ULONG  option;                     /* tag options, eg HT_CLOSE|HT_REQUIRED */
    LONG   vers;                       /* tag version, eg 10 for 1.0 */
    BOOL   (*o_handle)(INFILE *inpf);  /* function to handle tag */
    BOOL   (*c_handle)(INFILE *inpf);  /* function to handle closing tag */
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
#define HT_WARNARGS    (1<<31) /* display warning when unknown args found */


#define HT_CLOSE_ID       "CLOSE"
#define HT_REQUIRED_ID    "REQUIRED"
#define HT_ONLYONCE_ID    "ONLYONCE"
#define HT_IGNORE_ARGS_ID "IGNOREARGS"
#define HT_VERS_ID        "VERS"

/*
** char constants
*/
#define CH_QUOTE '\"'
#define CH_SPACE ' '
#define CH_LF    '\n'
#define CH_CR    '\r'
#define CH_ESC   27
#define CH_NULL  '\0'

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_GLOBAL_H

/*
** global vars
*/
extern STRPTR inpfilename;
extern STRPTR outfilename;
extern STRPTR errfilename;
extern STRPTR destdir;

extern ULONG max_error;

extern BOOL absurl;
extern BOOL chkurl;
extern BOOL debug;
extern BOOL insanch;
extern BOOL need_help;
extern BOOL pipe_in;
extern BOOL stripurl;
extern BOOL statusmsg;
extern BOOL verbose;


extern FILE *outfile;
extern time_t now;

extern STRPTR destfname;
extern STRPTR rel_destdir;

extern DLLIST *deftag;
extern DLLIST *defent;
extern DLLIST *cltags;

extern ULONG anchor_nesting;
extern ULONG body_nesting;
extern ULONG head_nesting;
extern ULONG list_nesting;
extern ULONG title_nesting;

extern char    this_tag[MAX_TAGLEN];
extern HSCTAG *this_tag_data;
extern STRPTR  last_anchor;


/*
** global funcs
*/
extern int fprintf_spc( FILE *file, size_t num );
extern HSCTAG *new_hsctag( STRPTR newid );
extern HSCENT *new_hscent( STRPTR newid );


#endif /* NOEXTERN_HSC_GLOBAL_H */

#endif /* HSC_GLOBAL_H */
