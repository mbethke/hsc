/*
** deftag.h
**
** define tag from input file
**
*/

#ifndef HSC_DEFTAG_H
#define HSC_DEFTAG_H

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_DEFTAG_H

/*
** global funcs
*/

extern LONG get_mci( VOID );

extern HSCTAG *def_tag_name( DLLIST *taglist, INFILE *inpf, BOOL *open_tag );
extern BOOL def_tag_args( DLLIST *taglist, HSCTAG *tag, INFILE *inpf, BOOL *open_tag );

extern ULONG set_tag_args( HSCTAG *tag, INFILE *inpf, BOOL open_tag );

extern STRPTR infget_tagid( INFILE *inpf );

#endif /* NOEXTERN_HSC_DEFTAG_H */

#endif /* HSC_DEFTAG_H */
