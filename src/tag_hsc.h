/*
** tag_hsc.h
**
** tag handles for "<HSC_xx>"
**
*/

#ifndef HSC_TAG_HSC_H
#define HSC_TAG_HSC_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"

/*
**
** extern references
**
*/
extern BOOL handle_hsc_comment( INFILE *inpf, HSCTAG *tag );

extern BOOL handle_hsc_defent( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_deftag( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_exec( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_insert( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_include( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_let( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_onlycopy( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_source( INFILE *inpf, HSCTAG *tag );

extern BOOL handle_hsc_if( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_cif( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_else( INFILE *inpf, HSCTAG *tag );

#endif /* HSC_TAG_HSC_H */
