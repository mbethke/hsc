/*
**
** fname.c
**
** filename processing functions
**
** (W) by Tommy-Saftwörx in 1994,95
**
** version 0.8
**
** updated:  2-Aug-1995
** created: 24-May-1994
**
** $VER: fname.c 0.8 (2.8.1995)
**
**-------------------------------------------------------------------
** TODO:
** - app_fext: check, if filename gets too long; handle this case
** - xxx_fext: handle NULL-string as filename parameter
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "string.h"

#define NOEXTERN_UGLY_FNAME_H
#include "fname.h"


/*
** global vars
*/
char fn_buffer[ MAX_PATH ];            /* result string */


/*
** get_fext
**
** get filename extension
**
** params: fn...full filename to examine
** result: extension of _fn or NULL, if fn was also NULL
**
*/
STRPTR get_fext( CONSTRPTR fn )
{
    STRPTR fn_ext = NULL;              /* result var */
    size_t i;                          /* string index counter */

    if ( fn ) {

        i = strlen( fn )-1;

        if ( i > 0 ) {
        
            while ( ( i ) && ( fn[i] != '.' )
                    && ( strchr( PATH_SEPARATOR, fn[i] ) == NULL ) )
                i--;
            
            if ( fn[i] != '.' )                /* no extension? */
                i = strlen( fn );
        }

        strncpy( fn_buffer, &(fn[i]), strlen( fn) -i +1 );
        fn_ext = fn_buffer;

    }

    return fn_ext;
}


/*
** get_fname
**
**
*/
STRPTR get_fname( CONSTRPTR fn )
{
    STRPTR fn_name;                    /* result var */

    fn_name = ustrrpbrk( fn, PATH_SEPARATOR );

    if ( fn_name )
        fn_name++;
    else
        fn_name = ( STRPTR ) fn;


    if ( fn_name ) {
        strcpy( fn_buffer, fn_name );
        fn_name = fn_buffer;

    }

    return fn_name;
}


/*
** get_nsdir: get next subdirectory (including separator)
**
*/
STRPTR get_nsdir( CONSTRPTR fn )
{
    STRPTR fn_name;

    fn_buffer[0] = '\0';               /* reset buffer */

    fn_name = strpbrk( fn, PATH_SEPARATOR );

    if ( fn_name ) {

        size_t idx = strlen(fn) - strlen(fn_name)+1;

        fn_name++;
        strncpy( fn_buffer, fn, idx );
        fn_buffer[idx] = '\0';

    }

    return fn_buffer;
}


/*
** get_fpath
**
**
*/
STRPTR get_fpath( CONSTRPTR fn )
{
    STRPTR pa_name = ustrrpbrk( fn, PATH_SEPARATOR );

    fn_buffer[0] = '\0';

    if ( pa_name ) {

        size_t idx = strlen(fn) - strlen(pa_name)+1;

        pa_name++;
        strncpy( fn_buffer, fn, idx );
        fn_buffer[idx] = '\0';

    }

    return fn_buffer;
}


/*
** get_fdrive
**
**
*/
STRPTR get_fdrive( CONSTRPTR fn )
{
    return NULL;                       /* <- to be implemented */
}


/*
** fextidx: get filename extension index of string
**
** params: fn...filename to examine
** result: index in _fn, where extension starts
**
** NOTE: if _fn has an extension at all, _fn[i] is equal to '.', else
**       _fn has no extension (e.g. "TESTFILE") and _fn[i] is equal
**       to '\0'.
**
** (internal function)
**
*/
size_t fextidx( CONSTRPTR fn )
{
    size_t i;                          /* string index counter */

    i = strlen( fn )-1;                /* scan string backwards... */
    while ( ( i ) && ( fn[i] != '.' )
            && ( strchr( PATH_SEPARATOR, fn[i] ) == NULL ) )
        i--;
    if ( fn[i] != '.' )
        i = strlen( fn );

    return i;                          /* return result */
}


/*
** set_fext
**
**
**
*/
STRPTR set_fext( CONSTRPTR fn, CONSTRPTR newext )
{
    size_t extidx;                     /* index, where extension starts */

    extidx = fextidx( fn );            /* get extension index */

    strncpy( fn_buffer, fn, extidx );
    fn_buffer[extidx]   = '.';
    fn_buffer[extidx+1] = '\0';

    strcat( fn_buffer, newext );

    return fn_buffer;                  /* return new filename */
}


/*
** clr_fext: clear filename extension
**
** params: fn...filename to strip
** result: filename without extension
**
** EXAMPLE: "testfile.txt" -> "testfile"
**
*/
STRPTR clr_fext( CONSTRPTR fn )
{
    size_t extidx;                     /* index, where extension starts */

    extidx = fextidx( fn );            /* get extension index */

    strncpy( fn_buffer, fn, extidx );
    fn_buffer[extidx] = '\0';

    return fn_buffer;                  /* return new filename */
}


/*
** app_fext: append extension to filename
**
** EXAMPLE: ("testfile.txt", "lha") -> "testfile.txt.lha"
**                                  -> "testfile.lha"     (msdos)
*/
STRPTR app_fext( CONSTRPTR fn, CONSTRPTR newext )
{
    size_t extidx;                     /* index, where extension starts */

    extidx = fextidx( fn );            /* get extension index */

    strncpy( fn_buffer, fn, extidx );
    if ( fn_buffer[extidx] != '.' ) {
        fn_buffer[extidx]   = '.';
        extidx++;
    }
    fn_buffer[extidx] = '\0';

    strcat( fn_buffer, newext );

    return fn_buffer;
}


/*
** set_fextIdx: set file name index extension
**
** params: dest..destination string with new extension
**         fn....source string with extension to replace
**         idx...extension index
*/
STRPTR set_fnameIdx( CONSTRPTR fn, int idx )
{
    char   fn_ext[ 4 ];                /* index extension */
    size_t extidx;                     /* index, where extension starts */

    extidx = fextidx( fn );            /* get extension index */

    strncpy( fn_buffer, fn, extidx );
    fn_buffer[extidx]   = '.';
    fn_buffer[extidx+1] = '\0';

    sprintf( fn_ext, "%03d", idx );    /* create index string */

    strcat( fn_buffer, fn_ext );

    return fn_buffer;
}

/*
** app_fname: append filename to directory
**
** params: dir...directoryname
**         fn....filename to append
**
** NOTE: a PATHSEPARATOR[0] is append to dir, if none exists
*/
STRPTR app_fname( STRPTR dir, STRPTR fn )
{
    BOOL anydir;                        /* TRUE, if any dir passed as arg */

    /* reset buffer */
    fn_buffer[0] = '\0';

    /* is a dir passed? */
    anydir = (dir!=NULL);
    if ( anydir ) anydir = (strlen(dir)!=0);

    if ( anydir ) {

        strcat( fn_buffer, dir );
        /* check, if the last char of dir is a path separator */
        /* ->if not, append a direcory separator */
        if ( !strchr( PATH_SEPARATOR, dir[strlen(dir)-1] ) )
            strcat( fn_buffer, DIR_SEPARATOR );

    }

    /* append filename */
    if ( fn )
        strcat( fn_buffer, fn );

    return fn_buffer;
}
