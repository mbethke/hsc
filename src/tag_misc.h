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

extern BOOL handle_base( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_heading( INFILE *inpf, HSCTAG *tag );

#endif /* HSC_TAG_MISC_H */
