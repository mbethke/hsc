/*
** ugly/expstr.c
**
** ugly expandable-string functions
**
** (W) by Tommy-Saftwörx in 1995
**
** updated: 18-Nov-1995
** created: 12-Sep-1995
**
** $VER: expstr.c 1.0.4 (18.11.95)
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

#define DEBUG_UGLY_EXPSTR 0

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
BOOL set_estr_mem( EXPSTR *es, size_t new_size )
{
    BOOL ok = TRUE;
    STRPTR new_data = umalloc( new_size );

    if ( new_data ) {

#if DEBUG_UGLY_EXPSTR
        /* debugging message */
        fprintf( stderr, "** expstr.c: set to %d (%p->%p)\n",
                 new_size, es->es_data, new_data );
#endif
        es->es_size = new_size;
        es->es_data = new_data;

    } else
        ok = FALSE;

    return( ok );
}

/*
** set_estr
**
** set es_data with chars copied from a string
*/
BOOL set_estr( EXPSTR *es, CONSTRPTR s )
{
    BOOL ok = TRUE;
    size_t new_len  = strlen(s)+1;
    STRPTR old_data = es->es_data;

    if ( (es->es_size==es->es_step)
         && (es->es_size > new_len ) )
    {

        strcpy( es->es_data, s );      /* copy new data */
        es->es_len = new_len;          /* set new len */

    } else if ( set_estr_mem( es, modadj( new_len, es->es_step ) ) ) {

        strcpy( es->es_data, s );      /* copy new & release old data */
        ufreestr( old_data );

        es->es_len = new_len;          /* set new len */

    } else
        ok = FALSE;

    return( ok );
}

/*
** clr_estr
**
** clear expstr (set es_data to "")
*/
BOOL clr_estr( EXPSTR *es )
{
    return( set_estr( es, "" ) );
}

/*
**-------------------------------------
** constructor / destructor
**-------------------------------------
*/


EXPSTR *init_estr( size_t step_size )
{
    EXPSTR *es = umalloc( sizeof( EXPSTR ) );

    if ( es ) {

        if ( step_size < ES_MIN_MEMSTEP )
            step_size = ES_MIN_MEMSTEP;
        es->es_data = NULL;
        es->es_step = step_size;
        if ( !clr_estr( es ) ) {

            ufree( es );
            es = NULL;

        }

    }

    return( es );

}

void del_estr( EXPSTR *es )
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
BOOL app_estrch( EXPSTR *es, int ch )
{
    BOOL   ok = TRUE;
    STRPTR s;

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
        s = es->es_data;
        s[es->es_len-1] = ch;          /* append new char to expstr */
        s[es->es_len]   = 0;
        es->es_len++;                  /* incr. expstr length */
    }

    return( ok );
}

BOOL app_estr( EXPSTR *es, CONSTRPTR s )
{
    size_t i;
    BOOL   ok = TRUE;

    for ( i=0; ( (s[i]) && ok ); i++ )
        ok &= app_estrch( es, s[i] );

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
** get left part from a expstr; compare BASIC's "RIGHT$()"
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

size_t estrlen( EXPSTR *es )
{
    return( es->es_len - 1 );
}

