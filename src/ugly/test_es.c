/*
**
** test_es.c
**
** test expandable-string functions
**
** (W) by Tommy-Saftwörx in 1995
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "memory.h"
#include "string.h"
#include "expstr.h"


EXPSTR *es, *res;

/* print current estr */
void pe( STRPTR comment )
{
    printf( "es:%s: (%2d/%2d/%2d) \"%s\"\n", comment,
            es->es_len, es->es_size, es->es_step, estr2str( es ) ) ;
}
void pr( STRPTR comment )
{
    printf( "rs:%s: (%2d/%2d/%2d) \"%s\"\n", comment,
            res->es_len, res->es_size, res->es_step, estr2str( res ) ) ;
}

int main( void )
{
    LONG i;

    atexit( atexit_uglymemory );

#if 0
    for ( i=0; i<20; i++ )
        printf( "modadj(%-2d/%-2d) = %d\n",
                i,EXPSTR_MEMSTEP,modadj(i,EXPSTR_MEMSTEP) );
#endif

    es  = init_estr( 8 ); pe( "init  ") ;
    res = init_estr( 8 ); pr( "init  ") ;

    printf( "** test set & append\n" );
    set_estr( es, "hugo ist doof." ); pe( "set   " );

    app_estrch( es, ' ' ); pe("appch ");
    app_estrch( es, 's' ); pe("appch ");
    app_estr( es, "epp auch." ); pe("appstr");
    app_estr( es, " und resi." ); pe("appstr");

    /* test cutting functions */
    estrcpy( res, es );           pr( "copy  " );
    get_mid_estr( es, res, 5, 3); pe( "mid   " );          /* "ist" */
    get_right_estr( es, res, 5 ); pe( "right " );          /* "auch." */
    get_left_estr( es, res, 4 );  pe( "left  " );          /* "hugo" */

    /* test special cases for cutting funtions */
    printf( "** test get-part\n" );
    set_estr( res, "hugo" );      pr( "res=hugo " );
    get_mid_estr( es, res, 4, 3); pe( "mid(4,5) " );
    get_mid_estr( es, res, 3, 2); pe( "mid(3,2) " );
    get_mid_estr( es, res, 0, 9); pe( "mid(0,9) " );
    get_left_estr( es, res, 5 );  pe( "lef(5)   " );
    get_left_estr( es, res, 4 );  pe( "lef(4)   " );
    get_right_estr( es, res, 5 ); pe( "rig(5)   " );
    get_right_estr( es, res, 4 ); pe( "rig(4)   " );

    del_estr( es ); del_estr( res );

    return( 0 ); 
}

