/*
**
** uri.c
**
** functions for uri parsing of tag arguments
**
** updated: 28-Oct-1995
** created: 16-Jul-1995
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/string.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"

#include "vars.h"

#define NOEXTERN_HSC_URI_H
#include "uri.h"

#define PARENT_URI "../" /* string for parent dir within URIs */

/* TODO: do all this with EXPSTR */
char argbuf[ MAX_URILEN ];   /* buffer for URI names */
char relnam[ MAX_PATHLEN ];  /* result for get_relfname() */
char reluri[ MAX_PATHLEN ];  /* result for conv_path2uri() */
char dest_uri[MAX_URILEN];   /* result of parse_uri(); destination uri name */
                             /*   that is written to the output file */


/*
** get_relfname
**
** convert absolut filename passed to <IMG> or <A HREF>
** to a relative filename (used with absolute URIs)
**
** params: absn...absolute filename passed to tag
**         curp...current path currently processed
**                hsc-file is in. (must end with "/")
** result: ptr to a buffer that contains the relative
**         path. this buffer is overwritten with the next call
*/
STRPTR get_relfname( STRPTR absnam, STRPTR curdir )
{
    char tmpb[ MAX_PATH ];   /* temp buffer */
    char fname[ MAX_FNAME ]; /* file name only */
    char abspa[ MAX_PATH ];  /* absolute path only */
    STRPTR tmpp1, tmpp2;     /* temp pointer */
    STRPTR rest_absp;        /* rest of current path */
    STRPTR absp;

    /*
    ** TODO: - handle out of mem
    **       - handle too long paths
    **       - optimise this routine; it contains
    **         some lines that are not really required
    **
    ** (frankly speaking, i don't really understand why
    ** this routine works, but it seems that is does)
    */

    /* init string array */
    relnam[0] = 0;
    strcpy( fname, get_fname( absnam ) );
    strcpy( abspa, get_fpath( absnam ) );
    absp = abspa;

    /*
    ** strip all equal subdirs
    */
    do {
        tmpp1 = get_nsdir( absp );
        strcpy( tmpb, tmpp1 );
        tmpp2 = get_nsdir( curdir );
        if (!upstrcmp( tmpb, tmpp2 )) {

            absp += strlen( tmpb );
            curdir += strlen( tmpp2 );

        }

    } while ( tmpb[0] && tmpp2[0] && (!upstrcmp( tmpb, tmpp2 )) );

    rest_absp = absp;


    /*
    ** for every subdir in absp unequal to
    ** corresponding subdir curdir, insert a parent dir
    */
    if ( curdir[0] )
        do {

            tmpp1 = get_nsdir( absp );
            strcpy( tmpb, tmpp1 );
            tmpp2 = get_nsdir( curdir );
            if ( upstrcmp( tmpb, tmpp2 )) {

                absp += strlen( tmpb );
                curdir += strlen( tmpp2 );
                strcat( relnam, PARENT_DIR );

            }


        } while ( tmpp2[0] && (upstrcmp( tmpb, tmpp2 )));

    strcat( relnam, rest_absp );
    strcat( relnam, fname );

    return ( relnam );
}

/*
** conv_path2uri
**
** convert a path for local (system-dependant)
** file system to URI
*/
STRPTR conv_path2uri( STRPTR path )
{
    reluri[0] = '\0';               /* result for conv_path2uri() */

    while ( !strncmp( path, PARENT_DIR, strlen(PARENT_DIR) ) ) {

        strcat( reluri, PARENT_URI );
        path += strlen( PARENT_DIR );

    }

    strcat( reluri, path );

    return ( reluri );
}


/*
** conv_uri2path
**
** convert a uri to a path for local (system-dependant)
** file system
*/
STRPTR conv_uri2path( STRPTR uri )
{
    reluri[0] = '\0';                  /* result for conv_path2uri() */
#ifdef MS_DOS
    size_t i;                          /* loop var */
#endif

    while ( !strncmp( uri, PARENT_URI, strlen( PARENT_URI ) ) ) {

        strcat( reluri, PARENT_DIR );
        uri += strlen( PARENT_URI );

    }

    strcat( reluri, uri );


#ifdef MS_DOS
    /* for ms-dos, replace "/" by "\" */
    for ( i=0; reluri[i]; i++ )
        if ( reluri[i] == '/' )
            reluri[i] = '\\' )
#endif

    return ( reluri );
}


/*
** uri_kind
**
** evaluate kind of uri
*/
URIKIND uri_kind( STRPTR uri )
{
    URIKIND kind = URI_abs;

    if ( upstrncmp( uri, ABSURI_ID, strlen( ABSURI_ID ) ) ) {

        STRPTR colon_pos = strchr( uri, ':' );
        STRPTR slash_pos = strchr( uri, '/' );

        if ( colon_pos )
            if ( slash_pos )
                if ( colon_pos < slash_pos )
                    kind = URI_ext;
                else
                    kind = URI_rel;
            else
                kind = URI_ext;
        else
            kind = URI_rel;

    }

    return( kind );
}


/*
** parse_uri
**
** read an uri-string, check it for syntatic correctnes.
** if the uri refers to an local file, convert its absolute
** path to a relative path and check its existence.
**
** uri = "rsrc_type://host.domain:port/pathname#name"
*/
STRPTR parse_uri( STRPTR uri, INFILE *inpf )
{

    STRPTR host = NULL;
    STRPTR port = NULL;
    STRPTR path = NULL;
    STRPTR name = NULL;
    char dest_fname[MAX_PATHLEN]; /* destination file name that's existence */
                                  /* is checked if chkuri is enabled */
    if (uri) {

        /* check for valid uri */
        URIKIND kind = uri_kind( uri );
        if ( kind == URI_ext ) {

            /*
            ** check global uri
            */
            if (!host) host = "";
            if (!port) port = "";
            if (!host) host = "";

            /*
            ** TODO: parse global uris
            */


        } else {

            /*
            ** check local uri
            */

            /* evaluate kind of URI */
            if ( kind == URI_abs )
                uri++;

            if ( docbase_set && (kind == URI_rel) )
                kind = URI_abs;


            /* extract path and #name */
            if ( uri[0] == '#' ) {
                path = "";
                name = uri+1; /* skip '#' */
            } else {
                path = strtok( uri, "#" );
                name = strtok( NULL, "" );
            }

            strcpy( dest_fname, projdir );

            if ( path ) {

                FILE *exist;

                if ( kind == URI_abs ) {

                    /*
                    **
                    ** parse absolute uri
                    **
                    */
                    /* debug */
                    if (debug)
                        fprintf( stderr, "** exists %s [abs]\n", path );

                    /* check if local uri exists */
                    strcat( dest_fname, destdir );
                    strcat( dest_fname, path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   -> file %s\n", dest_fname );

                    /* create path of destination file */
                    strcpy( dest_uri, rel_destdir );
                    strcat( dest_uri, path );


                    path = get_relfname( path, rel_destdir );
                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   -> real path %s\n", path );

                    /* convert (filesystem depending) path to uri */
                    uri = conv_path2uri( path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   -> real uri  %s\n", uri );

                } else { /* if (kind==URI_abs) */

                    /*
                    ** parse relative uri
                    */


                    /* debug */
                    if (debug)
                        fprintf( stderr, "** exists %s [rel]\n", path );

                    /* check if local uri exists */
                    strcat( dest_fname, destdir );
                    strcat( dest_fname, rel_destdir );
                    strcat( dest_fname, conv_uri2path(path) );

                    /* create uri (only copy path) */
                    strcpy( dest_uri, path );
                    uri = dest_uri;

                    /* debug */
                    if (debug) {
                        fprintf( stderr, "**   -> real path %s\n", dest_fname );
                        fprintf( stderr, "**   -> real uri  %s\n", dest_uri );
                    }
                }

                /*
                **check existence of local uri
                */
                if ( chkuri ) {

                    exist = fopen( dest_fname, "r" );
                    if ( !exist ) {

                        message( MSG_NO_URIPATH, inpf );
                        errstr( "path to URI not found:" );
                        errqstr( dest_fname );
                        errlf();

                    } else
                        fclose( exist );
                }

            } else { /* if (path) */

               path = "";

            }

            /* add #name part */
            if ( name ) {
                strcat( uri, "#" );
                strcat( uri, name );
            }

        } /* else (rsrc) */

    } /* if (uri) */

    return ( uri );
}

