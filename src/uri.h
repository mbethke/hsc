/*
** tagargs.h
**
** functions for parsing of tag arguments
**
*/

#ifndef HSC_URI_H
#define HSC_URI_H

#include "ugly/types.h"
#include "ugly/infile.h"

#define ABSURI_ID ":"

typedef enum {
               URI_abs, /* starts with ":" */
               URI_ext, /* contains ":" before first "/" */
               URI_rel  /* no ":" */
               } URIKIND; /* uri kinds */

/*
** global funcs
*/
#ifndef NOEXTERN_HSC_URI_H
extern STRPTR parse_uri( STRPTR uri, INFILE *inpf );
extern STRPTR get_relfname( STRPTR absnam, STRPTR curdir );
extern URIKIND uri_kind( STRPTR uri );
#endif /* NOEXTERN_HSC_URI_H */

#endif /* HSC_URI_H */

