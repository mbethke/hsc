/*
 *
 * ugly/ufile.h
 *
 * header file for misc. filename functions
 *
 * (W) by Tommy-Saftwörx in 1996
 *
 */

#ifndef UGLY_UFILE_H
#define UGLY_UFILE_H

#include "utypes.h"
#include "expstr.h"

/*
 * external function prototypes
 */

#ifndef NOEXTERN_UGLY_UFILE_H

extern BOOL fexists( STRPTR filename);
extern LONG getfsize( STRPTR filename);

#endif

#endif

