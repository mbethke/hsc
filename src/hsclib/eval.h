/*
 * eval.h
 *
 * attribute value evaluation functions
 */

#ifndef HSCLIB_EVAL_H
#define HSCLIB_EVAL_H

/*
 *
 * extern references
 *
 */
#ifndef NOEXTERN_HSCLIB_EVAL_H

extern BOOL check_attrname(HSCPRC * hp, STRPTR name);

extern STRPTR eval_expression(HSCPRC * hp, HSCATTR * dest, STRPTR endstr);

#endif /*  NOEXTERN_HSCLIB_EVAL_H */

#endif /* HSCLIB_EVAL_H */

