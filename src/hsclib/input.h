/*
 * hsclib/input.h
 *
 * basic functions for parsing input
 */

#ifndef HSCLIB_INPUT_H
#define HSCLIB_INPUT_H

#include "ugly/utypes.h"
#include "ugly/infile.h"

/*
 * global funcs
 */

extern BOOL hsc_whtspc(int ch);
extern BOOL hsc_normch(int ch);
extern BOOL hsc_normch_tagid(int ch);

extern STRPTR infget_tagid(HSCPRC * hp);
extern STRPTR infget_attrid(HSCPRC * hp);

extern BOOL parse_wd(HSCPRC * hp, STRPTR expstr);
extern BOOL parse_eq(HSCPRC * hp);
extern BOOL parse_gt(HSCPRC * hp);

#endif /* HSCLIB_INPUT_H */

