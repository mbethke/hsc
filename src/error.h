/*
** error.h
**
** error funcs for hsc
**
** updated:  3-Sep-1995
** created:  9-Jul-1995
*/

#ifndef HSC_ERROR_H
#define HSC_ERROR_H

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"

#include "global.h"

#define panic(text) call_panic(text,__FILE__,__LINE__)

/*
** global funcs
*/
#ifndef NOEXTERN_HSC_ERROR

extern ULONG message_limit;
extern BOOL fatal_error;

extern BOOL open_error( void );
extern void close_error ( void );

extern int errch( char ch );
extern int errlf( void );
extern int errstr( CONSTRPTR str );
extern int errqstr( CONSTRPTR str );
extern int errtag( CONSTRPTR str );
extern int errctag( CONSTRPTR str );
extern int errsym( CONSTRPTR str );
extern int errlong( ULONG num );

extern int message( ULONG id, INFILE *f );

extern int err_eof( INFILE *inpf );
extern int err_wst( INFILE *inpf, STRPTR );

extern int err_mem( INFILE *inpf );
extern int err_write( FILE *inpf );
extern int err_streol( INFILE *inpf );
extern int err_longstr( INFILE *inpf );

extern void call_panic( STRPTR text, STRPTR file, ULONG line );

#endif /* NOEXTERN_HSC_ERROR */
#endif /* HSC_ERROR_H */
