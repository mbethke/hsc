/*
** hsclib/tag_misc.h
**
** misc. tag-callbacks
**
*/

#ifndef HSC_TAG_MISC_H
#define HSC_TAG_MISC_H

/*
**
** extern references
**
*/
extern BOOL handle_sgml_comment( HSCPRC *hp, HSCTAG *tag );

extern BOOL handle_base( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_blink( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_heading( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_img( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_pre( HSCPRC *hp, HSCTAG *tag );
extern BOOL handle_end_pre( HSCPRC *hp, HSCTAG *tag );

#endif /* HSC_TAG_MISC_H */
