/*
** ugly/memory.c
**
** additional memory manegment functions
**
** Version 1.1.1, (W) by Tommy-Saftwörx
**
** updated:  1-Aug-1995
** created: 29-Mar-1994
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define NOEXTERN_UGLY_MEMORY_H
#include "memory.h"

UGLYMEM *first = NULL;


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

    return pprev;
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
    ufree( umem->ptr );
    umem->size = 0;
    umem->file = NULL;
    umem->line = 0;

    ufree( umem );
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

        newmem->ptr = malloc( memsize );
        if ( newmem->ptr ) {

            /* link to list */
            newmem->next = first;
            first = newmem;

            /* init data */
            newmem->size = memsize;
            newmem->file = memfile;
            newmem->line = memline;

        } else
            ufree( newmem );
    }

    return ( newmem );
}


void uglymem_message( STRPTR msg )
{
    fprintf( stderr, "%s\n", msg );
}

void uglymem_meminfo( void *ptr, STRPTR file, ULONG line )
{
    fprintf( stderr, "  in \"%s\" (%d) at %08p\n", file, line, ptr );
}

void umem_info( UGLYMEM *umem )
{
    fprintf( stderr, "  %6d (0x%04x) bytes from \"%s\" (%d) at %08p\n",
             umem->size, umem->size, umem->file, umem->line, umem->ptr );
}

void uglymem_report( STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time  )
{
    UGLYMEM *umem = first;

    if ( umem ) {
        ULONG sum_alloc = 0;

        /* report header */
        fprintf( stderr, "Memory Report (%s)\n", msg );
        fprintf( stderr, "(\"%s\" (%d), at %s, %s)\n", file, line, date, time );

        /* print all elements */
        while ( umem ) {

            sum_alloc += umem->size;
            umem_info( umem );
            umem = umem->next;
        }

        /* report statistics */
        fprintf( stderr, "  %d (0x%x) bytes used\n", sum_alloc, sum_alloc );

    }
}

void atexit_uglymemory( void )
{
    UGLYMEM *old_first = first;
    uglymem_report( "at EXIT() - memory leak detected!",
                    "[at exit()]", 0, __DATE__, __TIME__ );
    if ( old_first )
        fprintf( stderr, "\nLost memory realeased\n" );
}

/*
** ugly_malloc
*/

void *ugly_malloc( size_t size, STRPTR file, ULONG line )
{
    void    *mem = NULL;
    UGLYMEM *umem = NULL;

    /*
    ** alloc new uglymem
    */
    umem = new_uglymem( size, file, line );
    if ( umem )
        mem = umem->ptr;

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

            /* remove node from mem-list */
            del_uglymem( umem );

        } else {

            /* ptr has never been allocated */
            /* -> error message */
            uglymem_message( "FREE: memory never allocated" );
            uglymem_meminfo( ptr, file, line );
        }
    }
}


