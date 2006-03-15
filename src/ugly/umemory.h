/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef UGLY_UMEMORY_H
#define UGLY_UMEMORY_H          /* avoid include twice */

/*
 * ugly/umemory.h
 *
 * additional memory manegment, tracking and debugging functions, header file
 *
 */

#include "sysdep.h"
#include "utypes.h"

/*
 * ufree() - macro to free memory and set var
 *           pointing to memory to NULL,
 *           but only if it has been allocated
 *           successfully before
 */
struct uglymem
{
    struct uglymem *next;
    void *ptr;                  /* ptr to mem area allocated before */
    UBYTE *lower;               /* lower wall */
    UBYTE *upper;               /* upper wall */
    size_t size;                /* size of this area */
    STRPTR file;                /* filename from which call came */
    ULONG line;                 /* line num in this file */
    UBYTE fillchar;             /* fill character for wall */

};

typedef struct uglymem UGLYMEM;

#if DEBUG_UGLY_MEMORY

/* ugly function calls with memory tracking ENABLED */
#define umalloc(size) ugly_malloc_tracking( size, __FILE__, __LINE__ )
#define ufree(ptr)    if ( ptr ) { ugly_free( ptr, __FILE__, __LINE__ ); ptr = NULL; }
#define urealloc(ptr,size) ugly_realloc( ptr, size, __FILE__, __LINE__ );
#define ucalloc(count,size) ugly_calloc( count,size,__FILE__,__LINE__ );

#define umem_report(msg) uglymem_report( msg, __FILE__, __LINE__, __DATE__, __TIME__ )
#define umem_stats(msg) uglymem_stats( msg, __FILE__, __LINE__, __DATE__, __TIME__ )
#define umem_wallcheck(msg) uglymem_wallcheck( msg, __FILE__, __LINE__ )

#define atexit_uglymemory atexit_uglymemory_real

#else

/* ugly function calls with memory tracking disabled */
#define umalloc(size) ugly_malloc_notracking(size)
#define ufree(ptr) free(ptr)
#define urealloc(ptr,size)  realloc( ptr, size );
#define ucalloc(count,size) calloc( count,size )

#define umem_report(msg)        /* nufin */
#define umem_stats(msg)         /* nufin */
#define umem_wallcheck(msg)     /* nufin */

#define atexit_uglymemory atexit_uglymemory_dummy

#endif /* DEBUG_UGLY_MEMORY */

#ifndef NOEXTERN_UGLY_UMEMORY_H

extern void display_panic_message(char *msg, char *file, size_t line);

extern void *ugly_malloc_tracking(size_t size, STRPTR file, ULONG line);
extern void *ugly_malloc_notracking(size_t size);
extern void ugly_free(void *ptr, STRPTR file, ULONG line);
extern void *ugly_realloc(void *ptr, size_t size, STRPTR file, ULONG line);
extern void *ugly_calloc(size_t count, size_t size, STRPTR file, ULONG line);

extern void uglymem_stats(STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time);
extern void uglymem_report(STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time);
extern void uglymem_wallcheck(STRPTR msg, STRPTR file, ULONG line);
extern void atexit_uglymemory(void);

extern BOOL(*ugly_nomem_handler) (size_t size);

#endif

#endif
