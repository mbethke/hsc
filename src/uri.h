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

/*
** global funcs
*/
extern STRPTR parse_uri( STRPTR uri, INFILE *inpf );
extern STRPTR get_relfname( STRPTR absnam, STRPTR curdir );

#endif /* HSC_URI_H */

