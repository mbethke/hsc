/*
 * hscprj/pdebug.h
 *
 * debugging defines for hsc's project-functions
 */

#ifndef HSCPRJ_PDEBUG_H
#define HSCPRJ_PDEBUG_H

#include "hsclib/ldebug.h"

/* this debugging info is displayed only
 * if the  switch "-debug" has been enabled
 */

/* debugging control */
#define DEBUG_PRJ 1

/* debugging define */
#if (DEBUG & DEBUG_PRJ)
#define DP(x) if ( hp->debug ) x
#else
#define DP(x)                 /* nufin */
#endif

/* debug message prefix for hscprj-modules */
#define DHP "*hscprj* "

#endif /* HSCPRJ_PDEBUG_H */

