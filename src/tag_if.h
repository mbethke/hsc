/*
** tag_if.h
**
** tag handles for <$IF> and <$ELSE>
**
*/

#ifndef HSC_TAG_IF_H
#define HSC_TAG_IF_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"

/*
**
** extern references
**
*/
extern BOOL handle_hsc_if( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_cif( INFILE *inpf, HSCTAG *tag );
extern BOOL handle_hsc_else( INFILE *inpf, HSCTAG *tag );

#endif /* HSC_TAG_IF_H */
