/*
** tagargs.h
**
** functions for parsing of tag arguments
**
** updated:  6-Aug-1995
** created: 29-Jul-1995
*/

#ifndef HSC_TAGARGS_H
#define HSC_TAGARGS_H

#include "ugly/types.h"
#include "ugly/infile.h"

/*
** global funcs
*/
extern STRPTR parse_url( INFILE *inpf );
extern STRPTR parse_strarg( INFILE *inpf );
extern STRPTR parse_tagoptn( INFILE *inpf );
extern STRPTR parse_mutex( STRPTR line, INFILE *inpf, BOOL *match, BOOL igcs );

extern STRPTR get_relfname( STRPTR absnam, STRPTR curdir );

extern BOOL parse_ch( INFILE *inpf, int exptch );
extern BOOL parse_eq ( INFILE *inpf );
extern char parse_quote( INFILE *inpf );

#endif /* HSC_TAGARGS_H */

