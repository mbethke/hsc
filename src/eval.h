/*
** eval.h
**
** attribute value evaluation functions
*/

#ifndef HSC_EVAL_H
#define HSC_EVAL_H

#include "ugly/types.h"
#include "ugly/infile.h"

#include "tag.h"
#include "vars.h"

/*
**
** extern references
**
*/
#ifndef NOEXTERN_EVAL_H

extern BOOL check_attrname ( STRPTR name, INFILE *inpf );

extern STRPTR eval_expression( HSCVAR *dest, INFILE *inpf, STRPTR endstr );

#endif /*  NOEXTERN_EVAL_H */

#endif /* HSC_EVAL_H */
