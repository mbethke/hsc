#ifndef UGLY_UMX_H
#define UGLY_UMX_H                     /* avoid include twice

/*
** ugly/umx.h
**
** ugly macro expansion function, header
**
** Version 1.01, (W) by Tommy-Saftwörx
**
** updated: 31-Mar-94
** created: 30-Mar-94                                      compt: ANSI
**
*/

/*
**-------------------------------------------------------------------
**
** BUG: UMX_FLOAT doesn't work (wrong result??)
**
**-------------------------------------------------------------------
*/

#include <stddef.h>

#include "types.h"


/*
**
** buffer sizes
**
*/
#define UMX_MACRO_MAX 300              /* size of macro buffer */
#define UMX_TIMEBUF_MAX 40             /* size of time buffer */


/*
**
** legal values for
**
*/
#define UMX_END   0                    /* mark end */
#define UMX_INT   1                    /* integer value */
#define UMX_LONG  2                    /* long integer value */
#define UMX_CHAR  3                    /* single character value */
#define UMX_STR   4                    /* string (ptr) */

#define UMX_FLOAT 5 /* !!! not supported !!! */

#define UMX_TM    6                    /* struct tm (ptr) (ANSI) */


/*
** mark end of macro
*/
#define END_UMX   { UMX_END, '\0', NULL }  


/*
**
** struct: umacro
**     (later refered as 'umx' (see typedef))
**
*/
struct umacro {
    BYTE   umx_type;                   /* macro type (see defines) */
    char   umx_id;                     /* id char (e.g.'s' for '%s') */
    UPTR   umx_exp;                    /* where id to expand as */
    STRPTR umx_fmt;                    /* optional format (e.g for UMX_TM */
};

/*
**
** typedef: umx
**
*/
typedef struct umacro umx;             /* umx typedef */


/*
**
** external prototypes
**
*/
#ifndef NOEXTERN_UGLY_UMX_H

extern BOOL setumx_exp( struct umacro mac[], char id, UPTR exp );
extern BOOL setumx_fmt( struct umacro mac[], char id, STRPTR fmt );
extern size_t strfumx( STRPTR s, size_t maxsize, STRPTR fmt, struct umacro mac[] );

#endif                                 /* NOEXTERN_UGLY_UMX_H */


#endif                                 /* UGLY_UMX_H */
