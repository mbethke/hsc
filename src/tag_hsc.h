/*
** tag_hsc.h
**
** tag handles for "<HSC_xx>"
**
** updated:  2-Sep-1995
** created: 23-Jul-1995
*/

#ifndef HSC_TAG_HSC_H
#define HSC_TAG_HSC_H

#include "ugly/infile.h"
/*
**
** extern references
**
*/

extern BOOL handle_hsc_comment( INFILE *inpf );
extern BOOL handle_hsc_include( INFILE *inpf );
extern BOOL handle_hsc_onlycopy( INFILE *inpf );
extern BOOL handle_hsc_time( INFILE *inpf );
extern BOOL handle_hsc_var( INFILE *inpf );

#endif /* HSC_TAG_HSC_H */
