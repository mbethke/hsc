/*
** skip.h
**
** functions for skipping several things
**
*/

#ifndef HSC_SKIP_H
#define HSC_SKIP_H

#include "ugly/types.h"
#include "ugly/infile.h"

/* states for eot_reached() */
#define TGST_TAG     0 /* in tag */
#define TGST_REF     1 /* in attribute reference */
#define TGST_QUOTE   2 /* inside single quote */
#define TGST_DQUOTE  3 /* inside double quote */
#define TGST_END    10 /* end reached */
#define TGST_ERR    99 /* error occured */

/* states for eoc_reached() */
#define CMST_TEXT     0 /* after normal text (or inside tag) */
#define CMST_STAR     1 /* after "*" */
#define CMST_TAG      2 /* after "<" */
#define CMST_END     10 /* after "*>" */
#define CMST_ERR     99 /* error occured */

#ifndef NOEXTERN_SKIP_H
/*
** global funcs
*/
extern BOOL skip_lf( INFILE *inpf );
extern BOOL skip_hsc_comment( INFILE *inpf );
extern BOOL copy_until_gt( INFILE *inpf ); /* TODO: remove this */

extern BOOL skip_until_eot_state( INFILE *inpf, BYTE *state );
extern BOOL skip_until_eot( INFILE *inpf );
extern BOOL eot_reached( INFILE *inpf, BYTE *state );
extern BOOL eoc_reached( INFILE *inpf, BYTE *state, LONG *nest );

#endif /* NOEXTERN_SKIP_H */

#endif /* HSC_SKIP_H */

