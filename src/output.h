/*
** output.h
**
** output functions for hsc
*/

#ifndef HSC_OUTPUT
#define HSC_OUTPUT

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"

extern BOOL open_output( void );
extern VOID close_output( VOID );
extern int  outch( char ch );
extern int  outstr( CONSTRPTR str );

#endif /* HSC_OUTPUT */
