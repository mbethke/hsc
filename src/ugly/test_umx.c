/*
** test_umx.c
**
** ugly macro expansion function, test file
**
** Version 1.01, (W) by Tommy-Saftwörx
**
** updated: 31-Mar-94
** created: 30-Mar-94                                      compt: ANSI
**
*/

/*
**
** BUG: UMX_FLOAT doesn't work (wrong result)
**
*/


#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "types.h"
#include "string.h"
#include "umx.h"


#define SIZE_S 200

int main()
{

    char       s[ SIZE_S ];
    struct tm *t;
    time_t     now;
    char   timbuf[99];

    struct umacro macrodef[] =
        { 
          { UMX_TM   , 't' },
#if 0
          { UMX_FLOAT, 'f', NULL, "%3.3f" },
#endif
          { UMX_CHAR , 'c', (UPTR) '*' },
          { UMX_INT  , 'i', (UPTR) (int ) -1234 },
          { UMX_LONG , 'l', (UPTR) (LONG) -1234567 },
          { UMX_STR  , 's', (UPTR) "string" },
          END_UMX
        };


    
    now = time( NULL );
    t = localtime( &now );
    strftime( timbuf, 99, "%d-%b-%y", t );

    setumx_exp( macrodef, 't', (UPTR) t );
    setumx_fmt( macrodef, 't', "%d-%b-%y" );

    // macrodef[0].umx_exp = (UPTR)  t;

    printf( "** test_umx\n" );

    strfumx( s, SIZE_S, "t1 = '%-10t'  t2 = '%10t'", macrodef );
    printf( "%s\n", s );

    strfumx( s, SIZE_S, "s1 = '%-10s'  s2 = '%10s'", macrodef );
    printf( "%s\n", s );

    strfumx( s, SIZE_S, "c1 = '%-10c'  c2 = '%10c'", macrodef );
    printf( "%s\n", s );

    strfumx( s, SIZE_S, "i1 = '%-10i'  i2 = '%10i'", macrodef );
    printf( "%s\n", s );

    strfumx( s, SIZE_S, "l1 = '%-10l'  l2 = '%10l'", macrodef );
    printf( "%s\n", s );

    return( 0 );

}


