#ifndef UGLY_EXPSTR_H
#define UGLY_EXPSTR_H

/*
** ugly/expstr.h
**
** self-expanding string functions, header
**
** (W) by Tommy-Saftwörx in 1995
**
*/

#include <stddef.h>

#include "types.h"

#define EXPSTR_MEMSTEP 16
#define modadj(x,by) ((by)*(((x)+(by))/(by)))

typedef struct {

    STRPTR  es_data; /* ptr to string data */
    size_t  es_len;  /* current len */
    size_t  es_size; /* current size of mem allocated */

} EXPSTR;

/*
** external prototypes
*/

#ifndef NOEXTERN_UGLY_EXPSTR_H

extern EXPSTR *init_estr( void );
extern void del_estr( EXPSTR *es );

extern BOOL set_estr( EXPSTR *es, CONSTRPTR s );
extern BOOL clr_estr( EXPSTR *es );
extern BOOL app_estrch( EXPSTR *es, int ch );
extern BOOL app_estr( EXPSTR *es, CONSTRPTR s );

extern STRPTR estr2str( EXPSTR *es );
extern BOOL   estrcpy( EXPSTR *dest, EXPSTR *src );
extern size_t estrlen( EXPSTR *es );

extern BOOL get_mid_estr( EXPSTR *dest, EXPSTR *src, size_t from, size_t num );
extern BOOL get_right_estr( EXPSTR *dest, EXPSTR *src, size_t num );
extern BOOL get_left_estr( EXPSTR *dest, EXPSTR *src, size_t num );


#endif  /* NOEXTERN_UGLY_EXPSTR_H */


#endif  /* UGLY_EXPSTR_H */
