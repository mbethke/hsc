/*
** ugly/memory.c
**
** additional memory manegment functions;
** implements some parts of Amiga-developer-tool
** "MungWall" at source-level
**
** Copyright (C) 1994,95,96  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 11-Apr-1996
** created: 29-Mar-1994
**
*/

/*
**
** Memory munging:
**
**   Except for ucalloc(), memory is pre-munged on allocation with
**   $DEADFOOD. When this is used in an Enforcer report, the caller is
**   allocating memory and doesn't initialize it before using it.
**
**   Memory is filled with $DEADBEEF before it is freed, encouraging
**   programs reusing free'ed memory to crash.
**
** Memory watching:
**
**   Null sized malloc()'s are reported. The integrity of the walls will
**   be tested according to the size specified when free'ed.
**
*/

/* TODO: build wall, do not free if wall damaged
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#define NOEXTERN_UGLY_MEMORY_H
#include "memory.h"

/*
** size of wall build around every memory block
*/
#define UMEM_WALLSIZE 8

UGLYMEM *first = NULL;

UBYTE deadbeef[4] = { 0xDE, 0xAD, 0xBE, 0xEF }; /* used to fill mem after free() */
UBYTE deadfood[4] = { 0xDE, 0xAD, 0xF0, 0x0D }; /* used to fill mem after malloc() */

UBYTE ugly_fillchar = 0x81;

ULONG ugly_umalloc_count      = 0; /* num. of calls to umalloc()/ucalloc() */
ULONG ugly_ufree_count        = 0; /* num. of calls to ufree() */
ULONG ugly_umalloc_count_fail = 0; /* num. of failed calls to umalloc() */
ULONG ugly_ufree_count_fail   = 0; /* num. of failed calls to ufree() */
ULONG ugly_maxmem_usage       = 0; /* maximum memmory used */
ULONG ugly_curmem_usage       = 0; /* current memory used */
ULONG ugly_maxnod_usage       = 0; /* maximum num. of memmory nodes used */
ULONG ugly_curnod_usage       = 0; /* current num. of memory nodes used */

/* forwar reference */
void *ugly_malloc_notracking( size_t size );

/* function pointer for nomem-handler */
BOOL (*ugly_nomem_handler)( size_t size ) = NULL;

/*
** find_umem( UGLYMEM *umem )
*/
UGLYMEM *find_umem( void *mem )
{
    UGLYMEM *nxtum = first;
    UGLYMEM *found = NULL;

    while ( nxtum && (!found) ) {

        if ( nxtum->ptr == mem )
            found = nxtum;
        nxtum = nxtum->next;

    }

    return ( found );
}

/*
** find_prev
*/
UGLYMEM *find_prev( UGLYMEM *umem )
{
    UGLYMEM *prev  = first;
    UGLYMEM *pprev = NULL;
    BOOL     found = FALSE;

    while ( prev && (!found) ) {
        found = ( prev == umem );
        if ( !found ) {
            pprev = prev;
            prev  = prev->next;
        }

    }

    return ( pprev );
}


/*
** del_uglymem
**
** free an uglymem-entry
*/
void del_uglymem( UGLYMEM *umem )
{
    UGLYMEM *prev = find_prev( umem );


    /* unlink from list */
    if ( prev ) {
        prev->next = umem->next;
    } else {
        first = umem->next;
    }

    /* free data */
    free( umem->ptr );
    umem->size = 0;
    umem->file = NULL;
    umem->line = 0;

    free( umem );
}

/*
** fill_mem4
*/
void fill_mem4( void *mem, size_t size, UBYTE value[4] )
{
    size_t i;

    for ( i=0; i<size; i++ )
        ( ((UBYTE *) mem)[i] ) = value[ i%4 ];
}

/*
** new uglymem
**
** alloc & init a new entry of ugly mem
*/
UGLYMEM *new_uglymem( size_t memsize, STRPTR memfile, ULONG memline )
{
    UGLYMEM *newmem = (UGLYMEM *) malloc( sizeof(UGLYMEM) );

    if ( newmem ) {

        newmem->ptr = ugly_malloc_notracking( memsize );
        if ( newmem->ptr ) {


            /* link to list */
            newmem->next = first;
            first = newmem;

            /* init data */
            newmem->size = memsize;
            newmem->file = memfile;
            newmem->line = memline;
            newmem->fillchar = ugly_fillchar;

            /* update fillchar */
            if ( ugly_fillchar == 0xff )
                ugly_fillchar = 0x81;
            else
                ugly_fillchar++;

            /* fill new mem area with $DEADF00D */
            fill_mem4( newmem->ptr, memsize, deadfood );

        } else
            free( newmem );
    }

    return ( newmem );
}


void uglymem_message( STRPTR msg )
{
    fprintf( stderr, "%s\n", msg );
}

void ugly_memdump( void *ptr, size_t size )
{
    STRPTR data = (STRPTR) ptr;

    /* limit size */
    if ( size > 16 )
        size = 16;

    fprintf( stderr, "  %p:", ptr );
    if ( data ) {

        size_t i;

        /* hex dump */
        for ( i=0; i<size; i++ ) {

            if ( !(i%4) )
                fprintf( stderr, " " );
            fprintf( stderr, "%02x", data[i] );

        }

        fprintf( stderr, "  \"" );
        /* ascii dump */
        for ( i=0; i<size; i++ )
            if ( data[i]<' ' )
                fprintf( stderr, "." );
            else
                fprintf( stderr, "%c", data[i] );
        fprintf( stderr, "\"\n" );

    } else
        fprintf( stderr, "NULL\n" );

}

void uglymem_meminfo( void *ptr, STRPTR file, ULONG line )
{
    fprintf( stderr, "  in \"%s\" (%lu) at %p\n", file, line, ptr );
}

void umem_info( UGLYMEM *umem )
{
    fprintf( stderr, "  %06lu (0x%04lx) bytes from \"%s\" (%lu) at %p\n",
             (ULONG) umem->size, (ULONG) umem->size,
             umem->file, umem->line, umem->ptr );
}

/*
**-------------------------------------
** memory statistics functions
**-------------------------------------
*/

/*
** ugly_mem_report
**
** displaly all memory nodes currently allocated
*/
void uglymem_report( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  )
{
    UGLYMEM *umem = first;

    if ( umem ) {

        /* report header */
        fprintf( stderr, "Memory Report (%s)\n", msg );
        if ( file )
            fprintf( stderr, "(\"%s\" (%lu), at %s, %s)\n",
                             file, line, date, time );

        /* print all elements */
        while ( umem ) {

            umem_info( umem );
            ugly_memdump( umem->ptr, umem->size );
            umem = umem->next;
        }
    }
}

/*
** ugly_mem_stats
**
** display memory statistics (nodes & size allocated)
*/
void uglymem_stats( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  )
{
    /* statistics header */
    fprintf( stderr, "Memory statistics (%s)\n", msg );
    if ( file )
        fprintf( stderr, "(\"%s\" (%lu), at %s, %s)\n",
                         file, line, date, time );

    /* memory statistics */
    fprintf( stderr, "  bytes used: %lu (max: %lu)\n",
                     ugly_curmem_usage, ugly_maxmem_usage );
    fprintf( stderr, "  nodes used: %lu (max: %lu)\n",
                     ugly_curnod_usage, ugly_maxnod_usage );
    fprintf( stderr, "  calls to: umalloc(%lu)   ufree(%lu)\n",
                     ugly_umalloc_count, ugly_ufree_count );

}


/*
**-------------------------------------
** atexit functions
**-------------------------------------
*/


/*
** atexit_uglymemory_real
*/
void atexit_uglymemory_real( void )
{
    ULONG mem_lost = ugly_curmem_usage;
    uglymem_report( "at exit:  MEMORY LEAK detected!",
                    NULL, 0, NULL, NULL );
    uglymem_stats( "[exit]", NULL, 0, NULL, NULL );

    /* release all lost mem */
    while ( first )
        del_uglymem( first );

    if ( mem_lost )
        fprintf( stderr, "\n%lu bytes of memory lost!\n",  mem_lost );
}


/*
** atexit_uglymemory_dummy
*/
void atexit_uglymemory_dummy( void )
{
    /* do nufin */
}

/*
**-------------------------------------
** memory handling functions
**-------------------------------------
*/

/*
** ugly_malloc_notracking
*/
void *ugly_malloc_notracking( size_t size )
{
    void *mem;
    BOOL  retry;

    do {

        mem = malloc( size );
        if ( !mem && ugly_nomem_handler ) {

            /* call nomem-handler */
            retry = (*ugly_nomem_handler)( size );
            if ( !retry )
                exit( EXIT_FAILURE ); /* abort programm */

        } else
            retry = FALSE;

    } while ( retry );

    return( mem );
}


/*
** ugly_malloc_tracking
*/
void *ugly_malloc_tracking( size_t size, STRPTR file, ULONG line )
{
    void    *mem = NULL;
    UGLYMEM *umem = NULL;

    if ( size ) {

        /* update num. of calls to umalloc() */
        ugly_umalloc_count++;

        /* alloc new uglymem */
        umem = new_uglymem( size, file, line );
        if ( umem ) {

            mem = umem->ptr;

            /* update memory usage and num of nodes */
            ugly_curmem_usage += size;
            if ( ugly_curmem_usage > ugly_maxmem_usage )
                ugly_maxmem_usage = ugly_curmem_usage;
            ugly_curnod_usage++;
            if ( ugly_curnod_usage > ugly_maxnod_usage )
                ugly_maxnod_usage = ugly_curnod_usage;

        }
    } else {

        /* zero-alloc */

        /* update num. of failed calls to umalloc() */
        ugly_umalloc_count_fail++;

        uglymem_message( "MALLOC: zero-sized allocation" );
        uglymem_meminfo( NULL, file, line );

    }

    return (mem);
}

/*
** ugly_free
*/
void ugly_free( void *ptr, STRPTR file, ULONG line )
{
    if ( ptr ) {

        UGLYMEM *umem = find_umem( ptr );

        if ( umem ) {

            /* update num. of calls to ufree() */
            ugly_ufree_count++;

            /* fill memory with $DEADBEEF */
            fill_mem4( umem->ptr, umem->size, deadbeef );

            /* update memory usage */
            ugly_curmem_usage -= umem->size;

            /* remove node from mem-list */
            del_uglymem( umem );
            ugly_curnod_usage--;

        } else {

            /* ptr has never been allocated */

            /* update num. of calls to ufree() */
            ugly_ufree_count_fail++;

            /* -> error message */
            uglymem_message( "FREE: memory never allocated" );
            uglymem_meminfo( ptr, file, line );
        }
    }
}

/*
** ugly_realloc
*/
void *ugly_realloc( void *ptr, size_t size, STRPTR file, ULONG line )
{
    ugly_free( ptr, file, line );                          /* free old mem */
    return( ugly_malloc_tracking( size, file, line ) );    /* alloc new mem */
}

/*
** ugly_calloc
*/
void *ugly_calloc( size_t count, size_t size, STRPTR file, ULONG line )
{
    /* alloc new mem */
    void *mem = ugly_malloc_tracking( count*size, file, line );

    /* fill mem with zero */
    if ( mem )
        memset( mem, 0, size*count );

    return( mem );
}
