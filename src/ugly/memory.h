#ifndef UGLY_MEMORY_H
#define UGLY_MEMORY_H                  /* avoid include twice */

/*
** ugly/memory.h
**
** additional memory manegment functions, header file
**
** updated:  1-Aug-1995
** created: 29-Mar-1994                                      compt: ANSI
**
*/


/*
** TODO:
** - realloc()
** - calloc()
** - some options (log every alloc/free, atexit_report optional )
** - memory log file
** - statistic (max. mem allocated, max.num. of areas alloc )
*/

#include <stdlib.h>

#include "types.h"

/*
** ufree() - macro to free memory and set var
**           pointing to memory to NULL, 
**           but only if it has been allocated 
**           successfully before
**
** IMPORTANT: You MUST inititalisie vars possibly unallocated with
**            _ufree() by e.g. "STRPTR my_var = NULL;"
**
*/
#define ufree( ptr ) if ( ptr ) { free( ptr ); ptr = NULL; }


typedef struct uglymem {

    struct uglymem *next;
    void  *ptr;  /* ptr to mem area allocated before */
    size_t size; /* size of this area */
    STRPTR file; /* filename from which call came */
    ULONG  line; /* line num in this file */

} UGLYMEM;




#ifndef NOEXTERN_UGLY_MEMORY_H

extern void *ugly_malloc( size_t size, STRPTR file, ULONG line );
extern void ugly_free( void *ptr, STRPTR file, ULONG line );
extern void uglymem_report( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  );
extern void atexit_uglymemory( void );

#include "memory2.h"

#endif

#endif
