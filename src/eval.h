/*
** eval.h
**
** attribute value evaluation functions
**
*/

#ifndef HSC_EVAL_H
#define HSC_EVAL_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"
#include "vars.h"

#define eval_attr_bool( inpf )    eval_attr_type( (inpf), VT_BOOL )
#define eval_attr_num( inpf )     eval_attr_type( (inpf), VT_NUM )
#define eval_attr_string( inpf )  eval_attr_type( (inpf), VT_STRING )
#define eval_attr_uri( inpf )     eval_attr_type( (inpf), VT_URI )


/*
**
** extern references
**
*/
#ifndef NOEXTERN_EVAL_H

extern HSCVAR *eval_attr( INFILE *inpf );
extern HSCVAR *eval_attr_type( INFILE *inpf, BYTE exptype );

extern BOOL eval_bool( INFILE *inpf, BOOL *err, STRPTR endstr );

#endif /*  NOEXTERN_EVAL_H */

#endif /* HSC_EVAL_H */
