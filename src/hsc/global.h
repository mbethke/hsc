/*
** hsc/global.h
**
** global variables and functions for hsc
**
*/

#ifndef HSC_GLOBAL_H
#define HSC_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hsclib/hsclib.h"

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/expstr.h"
#include "ugly/memory.h"
#include "ugly/string.h"


/*
** defines
*/
#define DHP "*hscpre* "

/*
** mode strings for syntax check
*/
#define MODE_PEDANTIC_STR "pedantic"
#define MODE_NORMAL_STR   "normal"
#define MODE_RELAXED_STR  "relaxed"

#define MODE_ENUMSTR  MODE_PEDANTIC_STR "|" \
                      MODE_NORMAL_STR "|" \
                      MODE_RELAXED_STR

#define MODE_PEDANTIC 1
#define MODE_NORMAL   2
#define MODE_RELAXED  3

#define DEFAULT_MODE_STR MODE_NORMAL_STR

/*
** modes for attribute quotes
*/
#define QMODE_KEEP   1                 /* keep quotes from input */
#define QMODE_DOUBLE 2                 /* always use double quotes (compatible) */
#define QMODE_SINGLE 3                 /* always use single quotes */
#define QMODE_NONE   4                 /* never use any quotes (compact) */

#define QMODE_ENUMSTR "keep|double|single|none"

/*
** modes for special characters/entity extraction
*/
#define EMODE_KEEP     1               /* do not replace */
#define EMODE_REPLACE  2               /* replace by prefered value */
                   /* (depends wheter if a PREFNUM was used within $DEFENT) */
#define EMODE_NUMERIC  3               /* always replace by numeric (&#123;)*/
#define EMODE_SYMBOLIC 4               /* always replace by symbolic (&uuml;)*/

#define EMODE_ENUMSTR "keep|replace|numeric|symbolic"

/*
** special values for ignore
*/
#define IGNORE_ALL_STR      "all"
#define IGNORE_BADSTYLE_STR "badStyle"
#define IGNORE_JERKS_STR    "jerks"
#define IGNORE_NOTES_STR    "notes"

/* pseudo-name for stdout */
#define STDOUT_NAME "<stdout>"

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_GLOBAL_H

extern EXPSTR *inpfilename;
extern EXPSTR *outfilename;
extern STRPTR  msgfilename;

extern ULONG   max_error;
extern DLLIST *define_list;
extern DLLIST *incfile;
extern STRPTR msg_format;
extern EXPSTR *msgbuf;
extern int return_code;

extern BOOL init_global( VOID );
extern VOID cleanup_global( VOID );

extern STRPTR get_outfilename( VOID );

#endif /* NOEXTERN_HSC_GLOBAL_H */

#endif /* HSC_GLOBAL_H */
