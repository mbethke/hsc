/*
** uri.c
**
** functions for uri parsing of tag arguments
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
** updated: 13-Apr-1996
** created: 16-Jul-1995
*/

#define NOEXTERN_HSCLIB_URI_H

#include "ugly/fname.h"

#include "hsclib/inc_base.h"
#include "hsclib/id.h"
#include "hsclib/project.h"
#include "hsclib/uri.h"

#define PARENT_URI "../" /* string for parent dir within URIs */

/*
** conv_path2uri
**
** convert a path for local (system-dependant)
** file system to URI
*/
VOID conv_path2uri( EXPSTR *dest, STRPTR path )
{
    clr_estr( dest );

#ifdef AMIGA
    /* replace leading parent directories by "../" */
    while ( !strncmp( path, PARENT_DIR, strlen(PARENT_DIR) ) ) {

        app_estr( dest, PARENT_URI );
        path += strlen( PARENT_DIR );

    }

    while ( path[0] ) {

        /* replace all "//" by "../" */
        if ( (path[0] == '/') && (path[1] == '/' ) ) {

            app_estr( dest, PARENT_URI );
            path += 2;

        } else {

            app_estrch( dest, path[0] );
            path++;
        }
    }

#elif defined MSDOS
    /* replace all "\" by "/" */
    if ((path[0] == '\\' )
        app_estrch( dest, '/' );
    else
        app_estrch( dest, path[0] );
    path++;

#elif defined UNIX
    /* simply copy path */
    set_estr( dest, path );
#else
#error "system not supported: conv_path2uri"
#endif
}


/*
** conv_uri2path
**
** convert a uri to a path for local (system-dependant)
** file system
*/
VOID conv_uri2path( EXPSTR *dest, STRPTR uri )
{
    clr_estr( dest );

#ifdef AMIGA
    /* convert leading "../" to "/" */
    while ( !strncmp( uri, PARENT_URI, strlen( PARENT_URI ) ) ) {

        app_estr( dest, PARENT_DIR );
        uri += strlen( PARENT_URI );

    }

    /* convert inside "../" to "//" */
    while ( uri[0] ) {

        if ( !strncmp( uri, PARENT_URI, strlen( PARENT_URI ) ) ) {

            app_estrch( dest, '/' );
            uri += strlen( PARENT_URI );

        } else {

            app_estrch( dest, uri[0] );
            uri++;

        }
    }

#elif defined MS_DOS
    /* convert all "/" to "\" */
    while ( uri[0] )
        if ( uri[0] == '/' )
            app_estrch( dest, '\\' );
        else
            app_estrch( dest, uri[0] );

#elif defined UNIX
    set_estr( dest, uri );
#else
#error "system not supported: conv_2path"
#endif
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
** check uri-string for syntatic correctnes;
** if the uri refers to an local file, convert its absolute
** path to a relative path and check its existence.
**
** uri = "rsrc_type://host.domain:port/pathname#id"
*/
VOID parse_uri( HSCPRC *hp, EXPSTR *dest_uri, STRPTR uri )
{
    STRPTR host = NULL;
    STRPTR port = NULL;
    STRPTR path = NULL;
    STRPTR name = NULL;

    clr_estr( dest_uri );

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

            set_estr( dest_uri, uri );

        } else {

            /*
            ** check local uri
            */

            EXPSTR *dest_fname = init_estr(32);/* destination file name that's */
                                  /* existence is checked if chkuri is enabled */
            EXPSTR *rel_path   = init_estr(32);/* relative path */

            /* evaluate kind of URI */
            if ( kind == URI_abs )
                uri++; /* skip ":" */

            /* if a <BASE HREF=".."> was found before,
            ** also treat URI as absolute
            */
            if ( hp->docbase_set && (kind == URI_rel) )
                kind = URI_abs;

            /* extract path and #name */
            if ( uri[0] == '#' ) {

                path = NULL;  /* TODO: set path to current filename */
                name = uri+1; /* skip '#' */

            } else {

                path = strtok( uri, "#" );
                name = strtok( NULL, "" );

            }

            /* reset destination filename */
            set_estr( dest_fname, "" );

            if ( path ) {

                FILE *exist = NULL;

                if ( kind == URI_abs ) {

                    /*
                    **
                    ** parse absolute uri
                    **
                    */
                    /* debug */
                    D( fprintf( stderr, DHL "exists %s [abs]\n", path ) );

                    /* check if local uri exists */
                    estrcpy( dest_fname, hp->destdir );
                    app_estr( dest_fname, path );

                    /* debug */
                    D(
                        fprintf( stderr, DHL "  -> file %s\n",
                                 estr2str( dest_fname ) )
                    );

                    /* create path of destination file */
                    estrcpy( dest_uri, hp->reldir );
                    app_estr( dest_uri, path );

                    get_relfname( rel_path, path, estr2str( hp->reldir ) );

                    /* debug */
                    D( fprintf( stderr, DHL "  -> real path %s\n", path ) );

                    /* convert (filesystem depending) path to uri */
                    conv_path2uri( dest_uri, estr2str( rel_path ) );

                    /* debug */
                    D( fprintf( stderr, DHL "  -> real uri  %s\n", uri ) );

                } else { /* if (kind==URI_abs) */

                    /*
                    ** parse relative uri
                    */
                    EXPSTR *uri_path = init_estr(32);

                    /* debug */
                    D( fprintf( stderr, DHL "exists %s [rel]\n", path ) );

                    /* create local filename */
                    conv_uri2path( uri_path, path );
                    estrcat( dest_fname, hp->destdir );
                    estrcat( dest_fname, hp->reldir );
                    estrcat( dest_fname, uri_path );

                    /* create uri (only copy path) */
                    set_estr( dest_uri, path );

                    /* debug */
                    D( {
                        fprintf( stderr, DHL "  -> real path %s\n", estr2str(dest_fname) );
                        fprintf( stderr, DHL "  -> real uri  %s\n", estr2str(dest_uri) );
                    } );

                    del_estr( uri_path );
                }

                /*
                ** check existence of local uri
                */
                if ( hp->chkuri
                     && !( hsc_get_msg_ignore( hp, MSG_NO_URIPATH ) ) )
                {

                    exist = fopen( estr2str( dest_fname ), "r" );
                    if ( !exist )
                    {

                        hsc_message( hp, MSG_NO_URIPATH,
                                     "path to URI not found: %q",
                                     estr2str(dest_fname) );

                    } else {

                        fclose( exist );

                        /* check id */
                        if ( hp->chkid && name )
                            check_id_global( hp, estr2str( dest_fname ),
                                                 name );

                      }
                }

            } else {

                /* check existence ID referencing into current file */
                if ( hp->chkid && name )
                    check_id_local( hp, name );
              }

            /* add #name part */
            if ( name ) {
                app_estrch( dest_uri, '#' );
                app_estr( dest_uri, name );
            }

            /* free resources */
            del_estr( dest_fname );
            del_estr( rel_path );

        } /* else (rsrc) */

    } /* if (uri) */

#if 0
    STRPTR host = NULL;
    STRPTR port = NULL;
    STRPTR path = NULL;
    STRPTR name = NULL;
    EXPSTR *def_fname = init_estr(32); /* destination file name that's */
                          /* existence is checked if chkuri is enabled */
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
                uri++; /* skip ":" */

            /* if a <BASE HREF=".."> was found before,
            ** also treat URI as absolute
            */
            if ( docbase_set && (kind == URI_rel) )
                kind = URI_abs;


            /* extract path and #name */
            if ( uri[0] == '#' ) {

                path = NULL;  /* TODO: set path to current filename */
                name = uri+1; /* skip '#' */

            } else {

                path = strtok( uri, "#" );
                name = strtok( NULL, "" );

            }

            /* reset destination filename */
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
                        fprintf( stderr, DHL "exists %s [abs]\n", path );

                    /* check if local uri exists */
                    strcat( dest_fname, destdir );
                    strcat( dest_fname, path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, DHL "  -> file %s\n", dest_fname );

                    /* create path of destination file */
                    strcpy( dest_uri, rel_destdir );
                    strcat( dest_uri, path );


                    path = get_relfname( path, rel_destdir );
                    /* debug */
                    if (debug)
                        fprintf( stderr, DHL "  -> real path %s\n", path );

                    /* convert (filesystem depending) path to uri */
                    uri = conv_path2uri( path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, DHL "  -> real uri  %s\n", uri );

                } else { /* if (kind==URI_abs) */

                    /*
                    ** parse relative uri
                    */


                    /* debug */
                    if (debug)
                        fprintf( stderr, DHL "exists %s [rel]\n", path );

                    /* check if local uri exists */
                    strcat( dest_fname, destdir );
                    strcat( dest_fname, rel_destdir );
                    strcat( dest_fname, conv_uri2path(path) );

                    /* create uri (only copy path) */
                    strcpy( dest_uri, path );
                    uri = dest_uri;

                    /* debug */
                    if (debug) {
                        fprintf( stderr, DHL "  -> real path %s\n", dest_fname );
                        fprintf( stderr, DHL "  -> real uri  %s\n", dest_uri );
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
               strcpy( dest_uri, path );
               uri = dest_uri;

            }

            /* add #name part */
            if ( name ) {
                strcat( uri, "#" );
                strcat( uri, name );
            }

        } /* else (rsrc) */

    } /* if (uri) */

    return ( uri );
#endif
}

