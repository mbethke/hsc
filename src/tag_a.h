/*
** tag_a.h
**
** tag handle for "<A xx>"
**
** updated: 10-Sep-1995
** created:  3-Aug-1995
*/

#ifndef HSC_TAG_A_H
#define HSC_TAG_A_H

#include "ugly/types.h"
#include "ugly/infile.h"

extern BOOL handle_anchor( INFILE *inpf, HSCTAG *tag );

#if 0
extern BOOL handle_anchor( INFILE *inpf );
extern BOOL handle_canchor( INFILE *inpf );
#endif

#endif /* HSC_TAG_A_H */
