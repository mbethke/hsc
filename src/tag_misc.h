/*
** tag_misc.h
**
** misc tag handles
**
*/

#ifndef HSC_TAG_MISC_H
#define HSC_TAG_MISC_H

/*
**
** extern references
**
*/
extern BOOL handle_sgml_comment( INFILE *inpf, HSCTAG *tag );

extern BOOL handle_heading( INFILE *inpf, HSCTAG *tag );
#if 0
extern BOOL handle_heading2( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_heading3( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_heading4( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_heading5( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_heading6( INFILE *inpf, HSCTAG *tag );
#endif

#endif /* HSC_TAG_MISC_H */
