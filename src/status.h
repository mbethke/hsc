/*
** status.h
**
** status messages
**
*/

#ifndef HSC_STATUS_H
#define HSC_STATUS_H

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"


/*
** global funcs
*/
extern int status_msg( STRPTR s );
extern int status_infile( INFILE *f, BOOL force );
extern int status_clear( void );
extern int status_lf( void );

#endif /* HSC_STATUS_H */

