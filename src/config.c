/*
** config.c
**
** config handling for hsc; reads "hscdef.cfg"
**
** updated:  3-Nov-1995
** created: 12-Jul-1995
*/


/*
** ansi includes
*/
#include <stdio.h>
#include <string.h>


/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/string.h"


/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "cleanup.h"
#include "msgid.h"
#include "parse.h"
#include "status.h"

/*
** include hsc structs
*/
#include "entity.h"
#include "tag.h"
#include "vars.h"

#include "deftag.h"

/*
** include tag handles
*/
#include "tagargs.h"
#include "tag_a.h"
#include "tag_hsc.h"
#include "tag_macr.h"
#include "tag_misc.h"

#define NOEXTERN_HSC_CONFIG_H
#include "config.h"

/*
** debugging print functions
*/
void prt_ent( FILE *stream, APTR data )
{
    HSCENT *ent = (HSCENT*) data;

    if ( ent ) {

        fprintf( stream, " %s", ent->name );
        if ( ent->replace )
            fprintf( stream, "=%s", ent->replace );
        if ( ent->numeric )
            fprintf( stream, "=%d", ent->numeric );
    } else
        fprintf( stream, " <NULL>" );
}


void prt_tag( FILE *stream, APTR data )
{
    if ( data ) {

        HSCTAG *tag = (HSCTAG*) data;
        fprintf( stream, " <" );
        if ( tag->option & HT_CLOSE )
            fprintf( stream, "/" );
        if ( tag->o_handle ) fprintf( stderr, "*" );
        fprintf( stream, "%s", tag->name );
        if ( tag->c_handle ) fprintf( stderr, "*" );
        if ( tag->option & HT_REQUIRED )
            fprintf( stream, "/r" );
        if ( tag->option & HT_ONLYONCE )
            fprintf( stream, "/1" );
        if ( tag->option & HT_NOCOPY )
            fprintf( stream, "/c" );
        if ( tag->option & HT_NOHANDLE )
            fprintf( stream, "/h" );
        if ( tag->option & HT_SMARTCLOSE )
            fprintf( stream, "/sc" );
        if ( tag->option & HT_IGNOREARGS )
            fprintf( stream, "/a" );
        if ( tag->vers )
            fprintf( stream, "/v=%d", tag->vers );
        fprintf( stream, ">" );

    } else
        fprintf( stream, " <NULL>" );
}


/*
** read_config_file
**
** try to open (any) config file
*/
BOOL read_config_file( void )
{
    STRPTR  paths[]  =                 /* paths to search for config file */
       { CONFIG_PATH,                  /*    (defined in "global.h") */
         NULL, NULL };
    STRPTR  path;
    UBYTE   path_ctr = 0;
    STRARR  cfgfn[ MAX_PATHLEN ];      /* filename of config file */
    INFILE *cfgf    = NULL;            /* config file */
    BOOL    ok      = FALSE;
    STRPTR  hscenv  = getenv( HSCPREFS_ENVVAR );

    /* add hscenv to paths */
    paths[ (sizeof(paths) / sizeof(STRPTR)) - 2 ] = hscenv;

    do {                               /* loop: */

        path = paths[path_ctr];        /*   get next path */
        if ( path ) {                  /*   is it the last one? */

            strcpy( cfgfn, path );     /*   N->generate filename */
            strcat( cfgfn, CONFIG_FILE );

            cfgf = infopen( cfgfn, 0 );/*      try to open file */

        }
        path_ctr++;                    /*   process next path */

    } while ( path && (!cfgf) );       /* until no path left or file opened */

    /* status message */
    if ( cfgf ) {

        char msg[ 2*MAX_PATHLEN ];
        /* status message */
        sprintf( msg, "Reading prefs from \"%s\"", cfgfn );
        status_msg( msg );

        ok = include_hsc( "[config]", cfgf, outfile, IH_PARSE_HSC );

    } else {

        message( MSG_NO_CONFIG, NULL );
        errstr( "can not open config file " );
        errqstr( CONFIG_FILE );
        errlf();

    }

    return( ok );
}


/*
** add_tag_handle
*/
void add_tag_handle(
    DLLIST *taglist,
    STRPTR name,
    BOOL (*op_hnd)(INFILE *inpf, HSCTAG *tag ),
    BOOL (*cl_hnd)(INFILE *inpf, HSCTAG *tag ) )
{
    HSCTAG *tag = find_strtag( taglist, name );

    if ( tag && !(tag->option & HT_NOHANDLE ) ) {

        /* set handles */
        DC( fprintf( stderr, "** add handles for <%s> (%p,%p)\n",
                     name, op_hnd, cl_hnd ) );
        tag->o_handle = op_hnd;
        tag->c_handle = cl_hnd;

    }
}


/*
** read_hsctags: define hsc tags
** (tricky, but a bit perverted somehow)
*/
BOOL read_hsctags( void )
{

    BYTE    i  = 0;
    BOOL    ok = TRUE;
    BOOL    open_tag;
    HSCTAG *tag;

    /* string to define hsc tags */
    STRPTR hsc_prefs[] = {
        HSC_COMMENT_STR  " NOCOPY SKIPLF IGNOREARGS>",
        HSC_ONLYCOPY_STR " NOCOPY IGNOREARGS>",
        HSC_DEFENT_STR   " NOCOPY SKIPLF NAME:string/r RPLC:string>",
        HSC_DEFTAG_STR   " NOCOPY SKIPLF IGNOREARGS>",
        HSC_ELSE_STR     " NOCOPY SKIPLF>",
        HSC_EXEC_STR     " NOCOPY SKIPLF COMMAND:string/r>",
        HSC_IF_STR       " NOCOPY SKIPLF IGNOREARGS CLOSE>",
        HSC_INSERT_STR   " NOCOPY TEXT:string TIME:bool FORMAT:string>",
        HSC_INCLUDE_STR  " NOCOPY SKIPLF FILE:string/r PRE:bool SOURCE:bool>",
        HSC_LET_STR      " NOCOPY SKIPLF IGNOREARGS>",
        HSC_MACRO_STR    " NOCOPY SKIPLF IGNOREARGS>",
        HSC_SOURCE_STR   " NOCOPY SKIPLF PRE:bool>",
        NULL
    };

#if 0
    /*
    ** NOTE: the tags <|> and <*> are NOT added with
    ** the other hsc-tags above simpy because they
    ** do not require a leading "$"
    */

    /* create coment tag */
    tag = app_tag( deftag, HSC_COMMENT_STR );
    if ( tag ) {
        tag->option   = HT_NOCOPY | HT_IGNOREARGS | HT_SKIPLF;
        tag->o_handle = handle_hsc_comment;
    } else
        err_mem( NULL );

    /* create only-copy tag */
    tag = app_tag( deftag, HSC_ONLYCOPY_STR );
    if ( tag ) {
        tag->option   = HT_NOCOPY | HT_IGNOREARGS;
        tag->o_handle = handle_hsc_onlycopy;
    } else
        err_mem( NULL );
#endif

    /* define hsc-tags */
    while ( !fatal_error && hsc_prefs[i] ) {

        INFILE *inpf =
            infopen_str( "[init]", hsc_prefs[i], 60 );

        if ( inpf ) {

            tag = def_tag_name( deftag, inpf, &open_tag );
            ok = ( tag && def_tag_args( deftag, tag, inpf, &open_tag ) );
            infclose( inpf );

        } else
            err_mem( NULL );

        i++;

    }

    /* add tag handles for hsc-tags */
    if ( ok ) {

        add_tag_handle( deftag, HSC_COMMENT_STR , handle_hsc_comment , NULL );
        add_tag_handle( deftag, HSC_DEFENT_STR  , handle_hsc_defent  , NULL );
        add_tag_handle( deftag, HSC_DEFTAG_STR  , handle_hsc_deftag  , NULL );
        add_tag_handle( deftag, HSC_ELSE_STR    , handle_hsc_else    , NULL );
        add_tag_handle( deftag, HSC_EXEC_STR    , handle_hsc_exec    , NULL );
        add_tag_handle( deftag, HSC_IF_STR      , handle_hsc_if      , handle_hsc_cif );
        add_tag_handle( deftag, HSC_INCLUDE_STR , handle_hsc_include , NULL );
        add_tag_handle( deftag, HSC_INSERT_STR  , handle_hsc_insert  , NULL );
        add_tag_handle( deftag, HSC_LET_STR     , handle_hsc_let     , NULL );
        add_tag_handle( deftag, HSC_MACRO_STR   , handle_hsc_macro   , NULL );
        add_tag_handle( deftag, HSC_ONLYCOPY_STR, handle_hsc_onlycopy, NULL );
        add_tag_handle( deftag, HSC_SOURCE_STR  , handle_hsc_source  , NULL );

    }

    return( ok );
}


/*
** config_tag_handles
**
** append tags with handles
*/
BOOL config_tag_handles( void )
{
    BOOL ok = TRUE;

    add_tag_handle( deftag, "!"   , handle_sgml_comment, NULL );
    add_tag_handle( deftag, "A"   , handle_anchor, handle_canchor );
    add_tag_handle( deftag, "BASE", handle_base, NULL );
    add_tag_handle( deftag, "H1"  , handle_heading, NULL );
    add_tag_handle( deftag, "H2"  , handle_heading, NULL );
    add_tag_handle( deftag, "H3"  , handle_heading, NULL );
    add_tag_handle( deftag, "H4"  , handle_heading, NULL );
    add_tag_handle( deftag, "H5"  , handle_heading, NULL );
    add_tag_handle( deftag, "H6"  , handle_heading, NULL );


    /* entities */
    ok &= add_ent( "amp", NULL, 0 );      /* & */
    ok &= add_ent( "lt", NULL, 0 );       /* < */
    ok &= add_ent( "gt", NULL, 0 );       /* > */
    ok &= add_ent( "quot", NULL, 0 );     /* q */

    return (ok);
}


/*
** config_ok
**
** init list for tags & entities,
** append tags with handles,
** read other tags & entities from config file
**
** result: TRUE, if config ok
*/
BOOL config_ok( void )
{
    BOOL   ok = FALSE;             /* return value */

    /* get current time */
    now = time( NULL );

    /*
    ** init lists
    */
    defent  = init_dllist( del_entity);
    deftag  = init_dllist( del_tag );
    cltags  = init_dllist( del_tag );
    vars    = init_dllist( del_var );

    /* init vararg (defined in "vars.c") */
    IF_stack = init_estr( 0 );
    vararg   = init_estr( ES_STEP_VARARG );
    tmpstr   = init_estr( 0 );

    if ( cltags && defent && deftag && IF_stack
         && tmpstr && vararg )
    {

        ok = read_hsctags();
        if ( ok ) ok = read_config_file();
        if ( ok ) ok = config_tag_handles();

        /* printf list of entities & tags */
        DC( if ( ok ) {

            DLNODE *nd;
            HSCENT *ent;

            nd = defent->first;
            if ( nd ) {

                fprintf( stderr, "** Known entities:" );
                while (nd) {

                    ent = (HSCENT*) nd->data;
                    fprintf( stderr, " &%s;", ent->name );
                    nd = nd->next;

                }
                fprintf( stderr, "\n" );

            }

            nd = deftag->first;
            if ( nd ) {

                fprintf( stderr, "** Known tags:" );
                while (nd) {

                    prt_tag( stderr, nd->data );
                    nd = nd->next;

                }
                fprintf( stderr, "\n" );


            }
        } ) /* DC */

    } else
        err_mem( NULL );

    return( ok );
}

