/*
** parse.h
**
** functions for parsing of hsc files
**
*/

#ifndef HSC_PARSE_H
#define HSC_PARSE_H

#include <stdio.h>

#include "ugly/types.h"

/*
** flags for include_hsc()
*/
#define IH_PARSE_END    (1<<0) /* check for unclosed tags & co at end */
#define IH_PARSE_PRE    (1<<1) /* parse with replacing "><&" */
#define IH_PARSE_MACRO  (1<<2) /* include macro */
#define IH_PARSE_HSC    (1<<3) /* include text of a special */
                               /* hsc-command (eg $insert time) */
#define IH_PARSE_SOURCE (1<<4) /* include source (replace special chars) */

/*
** global funcs
*/
#ifndef NOEXTERN_PARSE_H

extern int cmp_strmac( APTR cmpstr, APTR macdata );

extern BOOL parse_end( INFILE *inpf );

extern BOOL include_ok( VOID );

extern BOOL include_hsc( STRPTR filename, INFILE *inpf, OUTFILE *outf, ULONG optn );
extern BOOL include_hsc_file( STRPTR filename, OUTFILE *outf, ULONG optn );
extern BOOL include_hsc_string( STRPTR filename, STRPTR s, OUTFILE *outf, ULONG optn );

#endif /* NOEXTERN_PARSE_H */
#endif /* HSC_PARSE_H */

