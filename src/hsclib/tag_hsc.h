/*
** tag_hsc.h
**
** tag handles for "<HSC_xx>"
**
*/

#ifndef HSCLIB_TAG_HSC_H
#define HSCLIB_TAG_HSC_H

/*
**
** extern references
**
*/
extern BOOL handle_hsc_comment( HSCPRC *hp, HSCTAG *tag );

extern BOOL handle_hsc_defent( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_define( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_deftag( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_exec( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_insert( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_include( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_let( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_message( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_onlycopy( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_source( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_insert_expression( HSCPRC *hp, HSCTAG *tag );

#endif /* HSCLIB_TAG_HSC_H */
