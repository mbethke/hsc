/*
 *
 * fname.h
 *
 * header file for filename manipulaton functions
 *
 * (W) by Tommy-Saftwörx in 1994,95,96
 *
 */

#ifndef UGLY_FNAME_H
#define UGLY_FNAME_H

#include "utypes.h"
#include "expstr.h"

/*
 * system depandant defines
 */

/*
 * MAX_FPATH         max. length of whole path
 * MAX_FNAME         max. length of filename
 * MAX_FEXT          max. length of file extension
 * PATH_SEPARATOR    to separate diretories and devices
 * DIR_SEPARATOR     to separate directories
 * PARENT_DIR        to be inserted to refer to parent directory
 * FNAME_IGNORE_CASE flag; ignore case within filenames
 */

#ifdef AMIGA
#define MAX_FPATH 126
#define MAX_FNAME  31
#define MAX_FEXT   30
#define PATH_SEPARATOR    "/:"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "/"
#define FNAME_IGNORE_CASE TRUE

#elif defined MSDOS
#define MAX_FPATH 128
#define MAX_FNAME   8
#define MAX_FEXT    3
#define PATH_SEPARATOR    "\\:"
#define DIR_SEPARATOR     '\\'
#define PARENT_DIR        "..\\"
#define FNAME_IGNORE_CASE TRUE

#elif defined UNIX
#define MAX_FPATH 254
#define MAX_FNAME 254
#define MAX_FEXT  253
#define PATH_SEPARATOR    "/"
#define DIR_SEPARATOR     '/'
#define PARENT_DIR        "../"
#define FNAME_IGNORE_CASE FALSE

#else
#error "Operating system not supported: filename-functions"
#endif

/* strcmp() for filenames: case-sensitive or not */
#if FNAME_IGNORE_CASE == TRUE
#define fnamecmp(a,b) strcmp((a),(b))
#define fnamencmp(a,b,n) strncmp((a),(b),(n))
#else
#define fnamecmp(a,b) upstrcmp((a),(b))
#define fnamencmp(a,b,n) upstrncmp((a),(b),(n))
#endif

#define ok_fnl_fpath(x) ((BOOL)(estrlen(x)<=MAX_FPATH))
#define ok_fnl_fname(x) ((BOOL)(estrlen(x)<=MAX_FNAME))
#define ok_fnl_fext(x)  ((BOOL)(estrlen(x)<=MAX_FEXT))

/*
 * external function prototypes
 */

#ifndef NOEXTERN_UGLY_FNAME_H

extern BOOL get_fext(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fname(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fpath(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_fsdir(EXPSTR * dest, CONSTRPTR fn);
extern BOOL get_relfname(EXPSTR * dest, STRPTR absn, STRPTR curp);

extern BOOL clr_fext(EXPSTR * dest);
extern BOOL set_fext(EXPSTR * dest, CONSTRPTR newext);
extern BOOL app_fext(EXPSTR * dest, CONSTRPTR newext);
extern BOOL set_fnameIdx(EXPSTR * dest, int idx);
extern BOOL link_fname(EXPSTR * dest, STRPTR dir, STRPTR fn);

extern STRPTR tmpnamstr(STRPTR prefix);

#endif

#endif

