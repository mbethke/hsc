/*
** tag_list.h
**
** tag handles for <UL>, <DL>, <LI>
**
** updated: 30-jul-95
** created: 29-jul-95
*/

#ifndef HSC_TAG_LIST_H
#define HSC_TAG_LIST_H

/*
**
** extern references
**
*/
extern BOOL handle_ul( INFILE *inpf );
extern BOOL handle_dl( INFILE *inpf );

extern BOOL handle_cul( INFILE *inpf );
extern BOOL handle_cdl( INFILE *inpf );

extern BOOL handle_li( INFILE *inpf );

#endif /* HSC_TAG_LIST_H */
