/*
**
** test_fn.c
**
** test filename manipulaton functions
**
** (W) by Tommy-Saftwörx in 1994,95
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "string.h"
#include "fname.h"

#define NUM_NAME 6

void test_app_fname( STRPTR dir, STRPTR fn )
{
    STRPTR appfn =  app_fname( dir, fn );

    if ( !dir ) dir = "NULL";
    if ( !fn  ) fn  = "NULL";
    printf( "file \"%s\" in \"%s\" -> \"%s\"\n", fn, dir, appfn );


}

void test_getfne( void )
{
    STRPTR name[ NUM_NAME ] = {
        "c:/hugo/sucks.txt", 
        "//silly/file.tar.gz",
        "test:dir/no_ext",
        "test:no_ext",
        "no_ext",
        "test:emptyext."
        };
    STRPTR res;
    int i;

    for ( i=0; i < NUM_NAME; i++ ) {
        res = get_fext( name[i] );
        printf( "get_fext (\"%-25s\") -> \"%s\"\n",
                name[i], res );
        printf( "get_fname(\"%-25s\") -> \"%s\"\n",
                name[i], get_fname( name[i] ) );

        printf( "\n" );

    }

}

void old_test_reldir( STRPTR absn, STRPTR curp )
{
    /*
    ** TODO: call upstrcmp() only when non-unix
    */
    STRPTR fname = strclone( get_fname( absn ) );  /* filename */
    STRPTR absop  = strclone( get_fpath( absn ) ); /* absolute path */
    STRPTR ocurp  = strclone( curp ); /* copy of curp for output */
    STRPTR absp   = absop; /* working copy of absolute path */

    char newp[ MAX_PATH ]; /* new path (result) */
    char tmpb[ MAX_PATH ]; /* temp buffer */
    STRPTR tmpp1, tmpp2; /* temp pointer */
    STRPTR rest_absp;    /* rest of current path */

    newp[0] = 0;

    /*
    ** strip equal subdir
    */
    do {
        tmpp1 = get_nsdir( absp );
        strcpy( tmpb, tmpp1 );
        tmpp2 = get_nsdir( curp );
        if (!upstrcmp( tmpb, tmpp2 )) {

            absp += strlen( tmpb );
            curp += strlen( tmpp2 );

        }

    } while ( tmpb[0] && tmpp2[0] && (!upstrcmp( tmpb, tmpp2 )) );

    rest_absp = absp;


    /*
    ** for every subdir in absp unequal to
    ** corresponding subdir curp, insert a parent dir
    */
    do {

        tmpp1 = get_nsdir( absp );
        strcpy( tmpb, tmpp1 );
        tmpp2 = get_nsdir( curp );
        if ( upstrcmp( tmpb, tmpp2 )) {

            absp += strlen( tmpb );
            curp += strlen( tmpp2 );
            strcat( newp, PARENT_DIR );

        }


    } while ( tmpp2[0] && (upstrcmp( tmpb, tmpp2 )));

    strcat( newp, rest_absp );
    strcat( newp, fname );

    printf( "\"%s\" and \"%s\" -> \"%s\"\n", absn, ocurp, newp );

    ufreestr( fname );
    ufreestr( ocurp );
    ufreestr( absop );
}


STRPTR get_relfname( STRPTR absn, STRPTR curp )
{
    char newp[ MAX_PATH ];   /* new path (result) */
    char tmpb[ MAX_PATH ];   /* temp buffer */
    char fname[ MAX_FNAME ]; /* file name only */
    char abspa[ MAX_PATH ];  /* absolute path only */
    STRPTR tmpp1, tmpp2;     /* temp pointer */
    STRPTR rest_absp;        /* rest of current path */
    STRPTR absp;

    /*
    ** TODO: handle out of mem
    */

    /* init string array */
    newp[0] = 0;
    strcpy( fname, get_fname( absn ) );
    strcpy( abspa, get_fpath( absn ) );
    absp = abspa;

    /*
    ** strip all equal subdirs
    */
    do {
        tmpp1 = get_nsdir( absp );
        strcpy( tmpb, tmpp1 );
        tmpp2 = get_nsdir( curp );
        if (!upstrcmp( tmpb, tmpp2 )) {

            absp += strlen( tmpb );
            curp += strlen( tmpp2 );

        }

    } while ( tmpb[0] && tmpp2[0] && (!upstrcmp( tmpb, tmpp2 )) );

    rest_absp = absp;


    /*
    ** for every subdir in absp unequal to
    ** corresponding subdir curp, insert a parent dir
    */
    if ( curp[0] )
        do {

            tmpp1 = get_nsdir( absp );
            strcpy( tmpb, tmpp1 );
            tmpp2 = get_nsdir( curp );
            if ( upstrcmp( tmpb, tmpp2 )) {

                absp += strlen( tmpb );
                curp += strlen( tmpp2 );
                strcat( newp, PARENT_DIR );

            }


        } while ( tmpp2[0] && (upstrcmp( tmpb, tmpp2 )));

    strcat( newp, rest_absp );
    strcat( newp, fname );

    return ( newp );
}


void test_reldir( STRPTR absn, STRPTR curp )
{
    /*
    ** TODO: call upstrcmp() only when non-unix
    */
    printf( "\"%s\" and \"%s\" -> \"%s\"\n",
            absn, curp, get_relfname( absn, curp ) );

}

void test_nsdir( STRPTR path )
{
    STRPTR newp = get_nsdir( path );

    printf( "get_nsdir: \"%s\" -> \"%s\"\n", path, newp );

}

void test_fpath( STRPTR path )
{
    STRPTR newp = get_fpath( path );

    printf( "get_fpath: \"%s\" -> \"%s\"\n", path, newp );

}


void test_setfext( STRPTR fn, STRPTR ext )
{
    STRPTR newfn = set_fext( fn, ext );

    if ( !fn  ) fn  = "NULL";
    if ( !ext ) ext = "NULL";

    printf( "set_fext: \"%s\" with \"%s\" -> \"%s\"\n", fn, ext, newfn );
}

void main( void )
{
    LONG i;
    STRPTR nam;

    printf( "Testing ugly fname functions:\n\n" );

    printf( "temp names (%d):", L_tmpnam );
    for ( i=0; i<10; i++ ) {

        nam = tmpnamstr();
        if ( nam ) {
            printf( " \"%s\"", nam );
            ufreestr( nam );
        } else
            printf( " <NULL>" );

    }
    printf( "\n" );

#if 0
    /*
    ** test get_fext & get_fname
    */
    test_getfne();
#endif

#if 0
    /*
    ** test app_fname
    */
    test_app_fname( "c:tools", "sucktool.exe" );
    test_app_fname( "c:", "sucktool.exe" );
    test_app_fname( "", "sucktool.exe" );
    test_app_fname( NULL, "sucktool.exe" );
    test_app_fname( NULL, NULL );
#endif

#if 0
    /*
    ** test set_fext: set a new file extension
    */
    test_setfext( "hugo.tmp", "suck" );
    test_setfext( "hugo", "suck" );
    test_setfext( "hugo.oh.yeah.honey", "sucker" );
    test_setfext( "", "suck" );

    /* these 3 calls now lead to an enforcer hit. */
    /* but is there any need to handle these cases? */
    test_setfext( "hugo", NULL );
    test_setfext( NULL, "suck" );
    test_setfext( NULL, NULL );
#endif

#if 0
    /*
    ** test get_fpath: get only dirs from path
    */
    test_fpath( "quite/long/path/for/hugo" );
    test_fpath( "hugo/heini/" );
    test_fpath( "hugo/heini" );
    test_fpath( "hugo/" );
    test_fpath( "hugo" );
    test_fpath( "" );
    test_fpath( NULL );  /* ->enforcer hit */
#endif

#if 0
    /*
    ** test get_nsdir: get first subdir from path
    */
    test_nsdir( "hugo/heini/" );
    test_nsdir( "hugo/hein" );
    test_nsdir( "hugo/" );
    test_nsdir( "hugo" );
    test_nsdir( "" );
    test_nsdir( NULL );  /* ->enforcer hit */
#endif

#if 1
#if 1
    test_reldir( "image/back.gif", "image/hugo/" );
#endif
    test_reldir( "image/back.gif", "" );
#if 1
    test_reldir( "image/back.gif", "people/" );
    test_reldir( "image/image.gif", "image/" );

    test_reldir( "image/symbol/main.gif", "people/hugo/picture/" );
    test_reldir( "image/symbol/main.gif", "image/hugo/picture/" );

    test_reldir( "image.gif", "people/" );
#endif
    test_reldir( "image.gif", "people/hugo/" );

#endif



}

