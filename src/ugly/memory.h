#ifndef UGLY_MEMORY_H
#define UGLY_MEMORY_H                  /* avoid include twice */

/*
** ugly/memory.h
**
** additional memory manegment, tracking and debugging functions, header file
**
*/

#include <stdlib.h>

#include "types.h"

/*
** ufree() - macro to free memory and set var
**           pointing to memory to NULL, 
**           but only if it has been allocated 
**           successfully before
*/
typedef struct uglymem {

    struct uglymem *next;
    void  *ptr;      /* ptr to mem area allocated before */
    size_t size;     /* size of this area */
    STRPTR file;     /* filename from which call came */
    ULONG  line;     /* line num in this file */
    UBYTE  fillchar; /* fill character for wall */

} UGLYMEM;

#if DEBUG_UGLY_MEMORY

/* ugly function calls with memory tracking ENABLED */
#define umalloc(size) ugly_malloc_tracking( size, __FILE__, __LINE__ )
#define ufree(ptr)    if ( ptr ) { ugly_free( ptr, __FILE__, __LINE__ ); ptr = NULL; }
#define urealloc(ptr,size) ugly_realloc( ptr, size, __FILE, __LINE__ );
#define ucalloc(count,size) ugly_calloc( count,size,__FILE__,__LINE );

#define umem_report(msg) uglymem_report( msg, __FILE__, __LINE__, __DATE__, __TIME__ )
#define umem_stats(msg) uglymem_stats( msg, __FILE__, __LINE__, __DATE__, __TIME__ )

#define atexit_uglymemory atexit_uglymemory_real

#else

/* ugly function calls with memory tracking disabled */
#define umalloc(size) ugly_malloc_notracking(size)
#define ufree(ptr)    if ( ptr ) { free(ptr); ptr=NULL; } /* TODO: use only free() */
#define urealloc(ptr,size)  realloc( ptr, size );
#define ucalloc(count,size) calloc( count,size )

#define umem_report(msg)    /* nufin */
#define umem_stats(msg)     /* nufin */

#define atexit_uglymemory atexit_uglymemory_dummy

#endif /* DEBUG_UGLY_MEMORY */



#ifndef NOEXTERN_UGLY_MEMORY_H

extern void *ugly_malloc_tracking( size_t size, STRPTR file, ULONG line );
extern void *ugly_malloc_notracking( size_t size );
extern void  ugly_free( void *ptr, STRPTR file, ULONG line );
extern void *ugly_realloc( void *ptr, size_t size, STRPTR file, ULONG line );
extern void *ugly_calloc( size_t count, size_t size, STRPTR file, ULONG line );

extern void uglymem_stats( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  );
extern void uglymem_report( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  );
extern void atexit_uglymemory( void );

extern BOOL (*ugly_nomem_handler)( size_t size );

#endif

#endif
