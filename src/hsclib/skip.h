/*
 * hsclib/skip.h
 *
 * functions for skipping several things
 *
 */

#ifndef HSCLIB_SKIP_H
#define HSCLIB_SKIP_H

#include "ugly/utypes.h"
#include "ugly/infile.h"

#include "hsclib/hscprc.h"

/* states for eot_reached() */
#define TGST_TAG     0          /* in tag */
#define TGST_REF     1          /* in attribute reference */
#define TGST_QUOTE   2          /* inside single quote */
#define TGST_DQUOTE  3          /* inside double quote */
#define TGST_END    10          /* end reached */
#define TGST_ERR    99          /* error occured */

/* states for eoc_reached() */
#define CMST_TEXT     0         /* after normal text (or inside tag) */
#define CMST_STAR     1         /* after "*" */
#define CMST_TAG      2         /* after "<" */
#define CMST_END     10         /* after "*>" */
#define CMST_ERR     99         /* error occured */

#ifndef NOEXTERN_SKIP_H

/*
 * global funcs
 */
extern BOOL skip_next_lf(HSCPRC * hp);
extern BOOL skip_hsc_comment(HSCPRC * hp);

extern BOOL skip_until_eot_state(HSCPRC * hp, BYTE * state, EXPSTR * logstr);
extern BOOL skip_until_eot(HSCPRC * hp, EXPSTR * logstr);
#if 1 /* TODO: get rid of this */
extern BOOL eot_reached(HSCPRC * hp, BYTE * state);
#endif
extern BOOL eoc_reached(HSCPRC * hp, BYTE * state, LONG * nest);

extern BOOL skip_until_tag(HSCPRC * hp, EXPSTR * tagfound, STRPTR tagstoplist, STRPTR tagnest);

#endif /* NOEXTERN_HSCLIB_SKIP_H */

#endif /* HSCLIB_SKIP_H */

