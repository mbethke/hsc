/*
** debug.h
**
** debugging defines for hsc
**
** NOTE: there is no need to include this file.
**       it is automatically included within "global.h"
**
*/

#ifndef HSC_DEBUG_H
#define HSC_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

/*
** debugging switches
**
** if switch is set to "1", requested debugging information is
** displayed if also the "-debug" switch is enabled when
** starting hsc
*/
#define DEBUG         1 /* misc. debugging */
#define DEBUG_ATTRVAL 1 /* display attribute values */
#define DEBUG_CONFIG  0 /* display config messages */
#define DEBUG_DEFATTR 0 /* display defined attributes */
#define DEBUG_DEFENT  0 /* display defined entities */
#define DEBUG_DEFTAG  0 /* display defined tags */
#define DEBUG_MACRO   1 /* display macro */

/*
** debugging defines
*/
#if DEBUG
#define D(x) if ( debug ) x
#else
#define D(x) /* nufin */
#endif

#if DEBUG_ATTRVAL
#define DAV(x) if ( debug ) x
#else
#define DAV(x) /* nufin */
#endif

#if DEBUG_CONFIG
#define DC(x) if ( debug ) x
#else
#define DC(x) /* nufin */
#endif

#if DEBUG_DEFATTR
#define DDA(x) if ( debug ) x
#else
#define DDA(x) /* nufin */
#endif

#if DEBUG_DEFENT
#define DDE(x) if ( debug ) x
#else
#define DDE(x) /* nufin */
#endif

#if DEBUG_MACRO
#define DMC(x) if ( debug ) x
#else
#define DMC(x) /* nufin */
#endif

#if DEBUG_DEFTAG
#define DDT(x) if ( debug ) x
#else
#define DDT(x) /* nufin */
#endif


#endif /* HSC_DEBUG_H */
