/*
** find.h
**
** misc. find functions
**
** updated:  1-Sep-1995
** created:  1-Sep-1995
**
*/
#ifndef HSC_FIND_H
#define HSC_FIND_H

#include "ugly/types.h"
#include "global.h"

/*
** global funcs
*/
#ifndef NOEXTERN_FIND_H
extern int cmp_strent( APTR cmpstr, APTR entdata );
extern int cmp_strtag( APTR cmpstr, APTR tagdata );
extern int cmp_strctg( APTR cmpstr, APTR tagstr );

extern HSCTAG *find_strtag( STRPTR name );

#endif /* NOEXTERN_FIND_H */
#endif /* HSC_FIND_H */


