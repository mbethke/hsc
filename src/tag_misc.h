/*
** tag_misc.h
**
** misc tag handles
**
** updated: 30-jul-95
** created: 30-jul-95
*/

#ifndef HSC_TAG_MISC_H
#define HSC_TAG_MISC_H

/*
**
** extern references
**
*/
extern BOOL handle_comment( INFILE *inpf );
extern BOOL handle_heading1( INFILE *inpf );
extern BOOL handle_heading2( INFILE *inpf );
extern BOOL handle_heading3( INFILE *inpf );
extern BOOL handle_heading4( INFILE *inpf );
extern BOOL handle_heading5( INFILE *inpf );
extern BOOL handle_heading6( INFILE *inpf );

#endif /* HSC_TAG_MISC_H */
