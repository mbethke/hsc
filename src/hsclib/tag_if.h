/*
** tag_if.h
**
** tag handles for <$IF> and <$ELSE>
**
*/

#ifndef HSC_TAG_IF_H
#define HSC_TAG_IF_H

/*
**
** extern references
**
*/
extern BOOL handle_hsc_if( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_cif( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_hsc_else( HSCPRC *hp, HSCTAG *tag );

#endif /* HSC_TAG_IF_H */
