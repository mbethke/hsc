/*
** parse.h
**
** functions for parsing of hsc files
**
** updated:  5-Aug-1995
** created:  1-Jul-1995
*/

#ifndef HSC_PARSE_H
#define HSC_PARSE_H

#include <stdio.h>

#include "ugly/types.h"

/*
** flags for include_hsc()
*/
#define IH_PARSE_END   (1<<0) /* check for unclosed tags & co at end */
#define IH_PARSE_PRE   (1<<1) /* ??? what the hell did i want with this ??? */
#define IH_PARSE_MACRO (1<<2) /* include macro */

/*
** global funcs
*/
#ifndef NOEXTERN_PARSE_H

extern int cmp_strmac( APTR cmpstr, APTR macdata );

extern BOOL parse_end( INFILE *inpf );
extern BOOL include_hsc( STRPTR inpfnm, FILE *outf, ULONG optn );

#endif /* NOEXTERN_PARSE_H */
#endif /* HSC_PARSE_H */

