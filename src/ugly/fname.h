/*
**
** fname.h
**
** header file for filename manipulaton functions
**
** (W) by Tommy-Saftwörx in 1994
**
** updated:  2-Aug-1995
** created: 24-May-1994
**
**-------------------------------------------------------------------
**
*/

#include "types.h"

/*
** system depandant defines
*/

/*
** PATH_SEPARATOR    to separate diretories and devices
** DIR_SEPARATOR     to separate directories
** PARENT_DIR        to be inserted to refer to parent directory
** FNAME_IGNORE_CASE flag; ignore case within filenames
*/

#ifdef AMIGA
#define PATH_SEPARATOR    "/:"
#define DIR_SEPARATOR     "/"
#define PARENT_DIR        "/"
#define FNAME_IGNORE_CASE TRUE
#endif

#ifdef __MSDOS__
#define PATH_SEPARATOR    "\\:"
#define DIR_SEPARATOR     "\\"
#define PARENT_DIR        "..\\"
#define FNAME_IGNORE_CASE TRUE
#endif

#ifdef UNIX
#define PATH_SEPARATOR    "/"
#define DIR_SEPARATOR     "/"
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE FALSE
#endif

#ifndef PATH_SEPARATOR
#error "Operating system not supported: #define PATH_SEPARATOR"
#endif

/*
** defines
*/
#define MAX_PATH 254                   /* max. file path length */
#define MAX_FNAME 31                   /* max. file name length */


/*
** external function prototypes
*/

#ifndef NOEXTERN_UGLY_FNAME_H

extern STRPTR get_fext( CONSTRPTR fn );
extern STRPTR get_fname( CONSTRPTR fn );
extern STRPTR get_fpath( CONSTRPTR fn );
extern STRPTR get_nsdir( CONSTRPTR fn );
extern STRPTR get_fdrive( CONSTRPTR fn );

extern STRPTR set_fext( CONSTRPTR fn, CONSTRPTR newext );
extern STRPTR clr_fext( CONSTRPTR fn );
extern STRPTR app_fext( CONSTRPTR fn, CONSTRPTR newext );
extern STRPTR set_fnameIdx( CONSTRPTR fn, int idx );

extern STRPTR app_fname( STRPTR dir, STRPTR fn );


#endif
