/*
** tagargs.h
**
** functions for parsing of tag arguments
**
*/

#ifndef HSC_TAGARGS_H
#define HSC_TAGARGS_H

#include "ugly/types.h"
#include "ugly/infile.h"

/*
** global funcs
*/
extern STRPTR parse_strarg( INFILE *inpf );
extern STRPTR parse_tagoptn( INFILE *inpf );

extern BOOL parse_ch( INFILE *inpf, int exptch );
extern BOOL parse_eq( INFILE *inpf );
extern BOOL parse_gt( INFILE *inpf );
extern char parse_quote( INFILE *inpf );

#endif /* HSC_TAGARGS_H */

