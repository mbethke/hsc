/*
** ugly/expstr.c
**
** ugly expandable-string functions
**
** Copyright (C) 1995,96  Thomas Aglassinger
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
** created: 12-Sep-1995
**
*/


#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "string.h"
#include "memory.h"

#define NOEXTERN_UGLY_STRING_H
#include "expstr.h"

#if DEBUG_UGLY_EXPSTR
#define D(x) x
#define DEXP "*expstr* "
#else
#define D(x) /* nufin */
#endif



void es_null( STRPTR func, STRPTR file, ULONG line )
{
    fprintf( stderr, "\n##\n## panic: es=NULL in %s()\n##   called from %s (%lu)\n##\n",
             func, file, line );
}

void s_null( STRPTR func, STRPTR file, ULONG line )
{
    fprintf( stderr, "\n##\n## panic: string=NULL in %s()\n##   called from %s (%lu)\n##\n",
             func, file, line );
}

/*
**-------------------------------------
** set/clear expstr
**-------------------------------------
*/

/*
** set_estr_mem
**
** set new mem for es_data
**
** params: es........expstr where to assign the new mem to
**         new_size..new size for data part
** result: TRUE if all ok, new mem in es->es_data;
** errors: return FALSE, leave es->es_data untouched
**
*/
static BOOL do_set_estr_mem( EXPSTR *es, STRPTR new_data, size_t new_size )
{
    BOOL ok = TRUE;

    if ( new_data ) {

#if DEBUG_UGLY_EXPSTR == 2
        D( fprintf( stderr, DEXP "set to %d (%p->%p)\n",
                    new_size, es->es_data, new_data ) );
#endif
        es->es_size = new_size;
        es->es_data = new_data;

    } else
        ok = FALSE;

    return( ok );
}

BOOL ugly_set_estr_mem( EXPSTR *es, size_t new_size )
{
    return( do_set_estr_mem( es, umalloc( new_size ), new_size ) );
}


BOOL ugly_dbg_set_estr_mem( EXPSTR *es, size_t new_size, STRPTR file, ULONG line )
{
    BOOL ok = FALSE;

    if ( !es )
        es_null( "set_estr_mem", file, line );
    else
    {
        ok = do_set_estr_mem( es,
                              ugly_malloc_tracking( new_size, file, line ),
                              new_size );
    }

    return( ok );
}

/*
** set_estr
**
** set es_data with chars copied from a string
*/
BOOL ugly_set_estr( EXPSTR *es, CONSTRPTR s )
{
    BOOL ok = FALSE;
    size_t new_len  = strlen(s)+1;
    STRPTR old_data = es->es_data;

    if ( (es->es_size==es->es_step)
         && (es->es_size > new_len ) )
    {
        strcpy( es->es_data, s );      /* copy new data */
        es->es_len = new_len;          /* set new len */
        ok = TRUE;
    }
    else if ( set_estr_mem( es, modadj( new_len, es->es_step ) ) )
    {

        strcpy( es->es_data, s );      /* copy new & release old data */
        ufreestr( old_data );

        es->es_len = new_len;          /* set new len */
        ok = TRUE;
    }

    return( ok );
}

BOOL ugly_dbg_set_estr( EXPSTR *es, CONSTRPTR s, STRPTR file, ULONG line )
{
    BOOL ok = FALSE;

    if ( !es )
        es_null( "set_estr_mem", file, line );
    else if ( !s )
        s_null( "set_estr_mem", file, line );
    else
    {
        size_t new_len  = strlen(s)+1;
        STRPTR old_data = es->es_data;

        if ( (es->es_size==es->es_step)
             && (es->es_size > new_len ) )
        {

            strcpy( es->es_data, s );      /* copy new data */
            es->es_len = new_len;          /* set new len */
            ok = TRUE;

        }
        else if ( ugly_dbg_set_estr_mem( es, modadj( new_len, es->es_step ), file, line ) )
        {

            strcpy( es->es_data, s );      /* copy new & release old data */
            ufreestr( old_data );

            es->es_len = new_len;          /* set new len */
            ok = TRUE;
        }
    }

    return( ok );
}

/*
** clr_estr
**
** clear expstr (set es_data to "")
*/
BOOL ugly_clr_estr( EXPSTR *es )
{
    return( set_estr( es, "" ) );
}

BOOL ugly_dbg_clr_estr( EXPSTR *es, STRPTR file, ULONG line )
{
    return( ugly_dbg_set_estr( es, "", file, line ) );
}


/*
** set_estrn
**
** set expstr with first n chars of string
*/
BOOL set_estrn( EXPSTR *es, CONSTRPTR s, size_t n)
{
    BOOL ok = FALSE;
    STRPTR s1 = NULL;
    size_t  len = strlen( s );

    if ( n > len ) n=len;

    s1 = (STRPTR) umalloc( n+1 );
    if ( s1 ) {

        memcpy( s1, s, n );
        s1[n] = 0;
        ok = set_estr( es, s1 );
        ufree( s1 );

    }
    return( ok );
}

/*
**-------------------------------------
** constructor / destructor
**-------------------------------------
*/


EXPSTR *ugly_dbg_init_estr( size_t step_size, STRPTR file, ULONG line )
{
    EXPSTR *es = ugly_malloc_tracking( sizeof( EXPSTR ), file, line );

    if ( es ) {

        if ( step_size < ES_MIN_MEMSTEP )
            step_size = ES_MIN_MEMSTEP;
        es->es_data = NULL;
	es->es_size = 0;
        es->es_step = step_size;
        if ( !clr_estr( es ) ) {

            ufree( es );
            es = NULL;

        }

    }

    return( es );

}

EXPSTR *ugly_init_estr( size_t step_size )
{
    EXPSTR *es = umalloc( sizeof( EXPSTR ) );

    if ( es ) {

        if ( step_size < ES_MIN_MEMSTEP )
            step_size = ES_MIN_MEMSTEP;
        es->es_data = NULL;
	es->es_size = 0;
        es->es_step = step_size;
        if ( !clr_estr( es ) ) {

            ufree( es );
            es = NULL;

        }

    }

    return( es );

}

VOID del_estr( EXPSTR *es )
{
    if ( es ) {

        ufreestr( es->es_data );
        es->es_len  = 0;
        es->es_size = 0;
        es->es_step = 0;
        ufree( es );

    }
}

/*
**-------------------------------------
** append char/string to expstr
**-------------------------------------
*/
BOOL ugly_app_estrch( EXPSTR *es, int ch )
{
    BOOL   ok = TRUE;

    if ( es->es_len >= es->es_size ) {           /* enough mem left? */

        STRPTR old_data = es->es_data;           /* N->remeber old data ptr */

        if ( set_estr_mem(es,
                 es->es_size + es->es_step ) )
        {                                        /*    set new mem sucessful? */

            strcpy( es->es_data,                 /*    Y->copy old data */
                    old_data );
            ufree( old_data );                   /*       release old data */

        } else {
                                                 /*    N->return error */
            ok = FALSE;
        }
    }

    if ( ok ) {

        STRPTR s;
        s = es->es_data;
        s[es->es_len-1] = ch;          /* append new char to expstr */
        s[es->es_len]   = 0;
        es->es_len++;                  /* incr. expstr length */

    }

    return( ok );
}


BOOL ugly_dbg_app_estrch( EXPSTR *es, int ch, STRPTR file, ULONG line )
{
    BOOL   ok = TRUE;

    if ( !es )
    {
        es_null( "app_estrch", file, line );
        ok = FALSE;
    }
    else if ( es->es_len >= es->es_size ) {      /* enough mem left? */

        STRPTR old_data = es->es_data;           /* N->remeber old data ptr */

        if ( ugly_dbg_set_estr_mem(es,
                 es->es_size + es->es_step, file, line ) )
        {                                        /*    set new mem sucessful? */

            strcpy( es->es_data,                 /*    Y->copy old data */
                    old_data );
            ufree( old_data );                   /*       release old data */
        }
        else
        {                                        /*    N->return error */
            ok = FALSE;
        }
    }

    if ( ok ) {

        STRPTR s;
        s = es->es_data;
        s[es->es_len-1] = ch;          /* append new char to expstr */
        s[es->es_len]   = 0;
        es->es_len++;                  /* incr. expstr length */

    }

    return( ok );
}


BOOL ugly_app_estr( EXPSTR *es, CONSTRPTR s )
{
    size_t i;
    BOOL   ok = TRUE;

    for ( i=0; ( (s[i]) && ok ); i++ )
        ok &= ugly_app_estrch( es, s[i] );

    return( ok );
}

#if 0
BOOL ugly_dbg_app_estrch( EXPSTR *es, int ch, STRPTR file, ULONG line )
{
    BOOL   ok = FALSE;

        ok = ugly_app_estrch( es, ch );

    return( ok );
}
#endif


BOOL ugly_dbg_app_estr( EXPSTR *es, CONSTRPTR s, STRPTR file, ULONG line )
{
    BOOL ok = FALSE;

    if ( !es )
        es_null( "app_estr", file, line );
    else if ( !s )
        s_null( "app_estr", file, line );
    else
    {
        size_t i;
        ok = TRUE;
        for ( i=0; ( (s[i]) && ok ); i++ )
            ok &= ugly_dbg_app_estrch( es, s[i], file, line );
    }

    return( ok );
}

/*
**-------------------------------------
** get part of expstr
**-------------------------------------
*/

/*
** todo: handle special cases like
** get_right_estr( .., "hugo", 99 );
*/

/*
** get_mid_estr
**
** get a part from a expstr; compare BASIC's "MID$()"
**
** params: dest...destination expstr where to store result part
**         src....source expstr where to get part from
**         from...position of char where to begin part (0=first char)
**         num....number of chars to get
** result: TRUE and result in dest if ok; else FALSE is returned an
**         dest is left untouched
**
** NOTE: it is possible to use the source-exstr as destination-expstr,
**       because the result is copied to a temp. expstr before
**       ( example: get_mid_estr( hugo, hugo, 3, 4 ); )
*/
BOOL get_mid_estr( EXPSTR *dest, EXPSTR *src, size_t from, size_t num )
{
    BOOL   ok = FALSE;
    EXPSTR *tmp = init_estr( dest->es_step );

    if ( tmp ) {

        STRPTR old_data = tmp->es_data;

        /* check size */
        if ( from >= src->es_len )
            from = src->es_len-1;
        if ( from + num >= src->es_len )
            num = src->es_len - from - 1;

        /* set new mem for tmp */
        ok = set_estr_mem( tmp, modadj( num+1, tmp->es_step ) );

        if ( ok ) {

            /* copy data */
            strncpy( estr2str(tmp), estr2str(src)+from, num );
            tmp->es_data[num] = 0;
            tmp->es_len       = num+1;
            ufree( old_data );

            ok = estrcpy( dest, tmp );

        }

        del_estr( tmp );

    }

    return( ok );
}

/*
** get_right_estr
**
** get right part from a expstr; compare BASIC's "RIGHT$()"
*/
BOOL get_right_estr( EXPSTR *dest, EXPSTR *src, size_t num )
{
    if ( num >= src->es_len )
        num  = src->es_len - 1;

    return( get_mid_estr( dest, src, (src->es_len-num-1), num ) );
}

/*
** get_left_estr
**
** get left part from a expstr; compare BASIC's "LEFT$()"
*/
BOOL get_left_estr( EXPSTR *dest, EXPSTR *src, size_t num )
{
    return( get_mid_estr( dest, src, 0, num ) );
}

/*
**-------------------------------------
** misc. functions
**-------------------------------------
*/

STRPTR estr2str( EXPSTR *es )
{
    return( es->es_data );
}


BOOL estrcpy( EXPSTR *dest, EXPSTR *src )
{
    return( set_estr( dest, estr2str( src ) ) );
}

BOOL estrcat( EXPSTR *dest, EXPSTR *src )
{
    return( app_estr( dest, estr2str( src ) ) );
}

size_t estrlen( EXPSTR *es )
{
    return( es->es_len - 1 );
}

