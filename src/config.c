/*
** config.c
**
** config handling for hsc; reads "hscdef.cfg"
**
** updated:  2-Sep-1995
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

/*
** include hsc structs
*/
#include "vars.h"
#include "macro.h"

/*
** include tag handles
*/
#include "tag_a.h"
#include "tag_hsc.h"
#include "tag_img.h"
#include "tag_list.h"
#include "tag_macr.h"
#include "tag_misc.h"

#define NOEXTERN_HSC_CONFIG_H
#include "config.h"

#define HSC_CONFIG_BUFSIZE 512
#define HSC_CONFIG_WORDLEN  64


/*
** debugging print functions
*/
void prt_ent( FILE *stream, APTR data )
{
    if ( data )
        fprintf( stream, " %s\n", ((HSCENT*) data)->name );
    else
        fprintf( stream, " <NULL>" );
}


void prt_tag( FILE *stream, APTR data )
{
    if ( data ) {

        HSCTAG *tag = (HSCTAG*) data;
        fprintf( stream, " %-12s", tag->name );
        if ( tag->option & HT_CLOSE )
            fprintf( stream, " %s", HT_CLOSE_ID );
        if ( tag->option & HT_REQUIRED )
            fprintf( stream, " %s", HT_REQUIRED_ID );
        if ( tag->option & HT_ONLYONCE )
            fprintf( stream, " %s", HT_ONLYONCE_ID );
        if ( tag->vers )
            fprintf( stream, " VERS=%d", tag->vers );
        fprintf( stream, "\n" );

    } else
        fprintf( stream, " <NULL>" );
}

/*
**-------------------------------------
** append entity functions
**-------------------------------------
*/

/*
** app_entnode
**
** create a new entity and append it to the defent-list
**
** params: entid..name of the new entity (eg "uuml")
** result: ptr to the new entity or NULL if no mem
*/
HSCENT *app_entnode( STRPTR entid )
{
    HSCENT *newent;

    newent = new_hscent( entid );
    if ( newent ) {

        if (app_dlnode( defent, newent ) == NULL ) {

            del_entity( (APTR) newent );
            newent = NULL;

        }
    } else
        err_mem( NULL );

    return (newent);
}


/*
** add_ent
*/
BOOL add_ent( STRPTR entid, STRPTR entsource )
{
    BOOL    ok = FALSE;
    HSCENT *newent = app_entnode( entid );

    if ( newent ) {

        newent->source = entsource;
        ok = TRUE;

    } else
        err_mem( NULL );


    return (ok);

}



/*
**-------------------------------------
** append tag functions
**-------------------------------------
*/

/*
** app_tagnode
**
** create a new tag and append it to the deftag-list
**
** params: tagid..name of the new tag (eg "IMG")
** result: ptr to the new tag or NULL if no mem
*/
HSCTAG *app_tagnode( STRPTR tagid )
{
    HSCTAG *newtag;

    newtag = new_hsctag( tagid );
    if ( newtag ) {
        if (app_dlnode( deftag, newtag ) == NULL ) {

            del_tag( (APTR) newtag );
            newtag = NULL;

        }
    } else
        err_mem( NULL );

    return (newtag);
}


/*
** add_tag
*/
HSCTAG *add_tag(
  STRPTR tagid,
  ULONG  optn,
  ULONG  version,
  BOOL   (*op_handle)(INFILE *inpf),
  BOOL   (*cl_handle)(INFILE *inpf)
)
{
    HSCTAG *newtag = app_tagnode( tagid );

    if ( newtag ) {

        newtag->option   = optn;
        newtag->vers     = version;
        newtag->o_handle = op_handle;
        newtag->c_handle = cl_handle;

    } else
        err_mem( NULL );

    return (newtag);

}


/*
** add_stag: add simple tag (no version & handles)
*/
HSCTAG *add_stag( STRPTR tagid, ULONG optn )
{
    return ( add_tag( tagid, optn, 0, NULL, NULL) );
}



/*
** config_tag_handles
**
** append tags with handles
*/
BOOL config_tag_handles( void )
{
    BOOL ok = TRUE;

    /* HTML 1.0 tags */
    ok &= (BOOL) add_tag ( "A"         , HT_CLOSE|HT_NOCOPY,
                                         0, handle_anchor, handle_canchor );
    ok &= (BOOL) add_stag( "ADDRESS"   , HT_CLOSE );
    ok &= (BOOL) add_stag( "B"         , HT_CLOSE );
    ok &= (BOOL) add_stag( "BASE"      , HT_ONLYONCE );
    ok &= (BOOL) add_stag( "BIG"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "BLOCKQUOTE", HT_CLOSE );
    ok &= (BOOL) add_stag( "BODY"      , HT_CLOSE|HT_REQUIRED|HT_ONLYONCE );
    ok &= (BOOL) add_stag( "BQ"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "BR"        , 0 );
    ok &= (BOOL) add_stag( "CITE"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "CODE"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "DD"        , 0 );
    ok &= (BOOL) add_stag( "DFN"       , HT_CLOSE );
    ok &= (BOOL) add_tag ( "DIR"       , HT_CLOSE, 0, handle_ul, handle_cul );
    ok &= (BOOL) add_tag ( "DL"        , HT_CLOSE, 0, handle_dl, handle_cdl );
    ok &= (BOOL) add_stag( "DT"        , 0 );
    ok &= (BOOL) add_stag( "EM"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "FORM"      , HT_CLOSE );
    ok &= (BOOL) add_tag ( "H1"        , HT_CLOSE, 0, handle_heading1, NULL );
    ok &= (BOOL) add_tag ( "H2"        , HT_CLOSE, 0, handle_heading2, NULL );
    ok &= (BOOL) add_tag ( "H3"        , HT_CLOSE, 0, handle_heading3, NULL );
    ok &= (BOOL) add_tag ( "H4"        , HT_CLOSE, 0, handle_heading4, NULL );
    ok &= (BOOL) add_tag ( "H5"        , HT_CLOSE, 0, handle_heading5, NULL );
    ok &= (BOOL) add_tag ( "H6"        , HT_CLOSE, 0, handle_heading6, NULL );
    ok &= (BOOL) add_stag( "HEAD"      , HT_CLOSE|HT_REQUIRED|HT_ONLYONCE );
    ok &= (BOOL) add_stag( "HR"        , 0 );
    ok &= (BOOL) add_stag( "HTML"      , HT_CLOSE|HT_REQUIRED|HT_ONLYONCE );
    ok &= (BOOL) add_stag( "I"         , HT_CLOSE );
    ok &= (BOOL) add_tag ( "IMG"       , 0, 0, handle_img, NULL );
    ok &= (BOOL) add_stag( "INPUT"     , 0 );
    ok &= (BOOL) add_stag( "ISINDEX"   , 0 );
    ok &= (BOOL) add_stag( "KBD"       , HT_CLOSE );
    ok &= (BOOL) add_tag ( "LI"        , 0, 0, handle_li, NULL );
    ok &= (BOOL) add_stag( "LINK"      , 0 );
    ok &= (BOOL) add_stag( "LISTING"   , HT_CLOSE );
    ok &= (BOOL) add_tag ( "MENU"      , HT_CLOSE, 0, handle_ul, handle_cul );
    ok &= (BOOL) add_tag ( "OL"        , HT_CLOSE, 0, handle_ul, handle_cul );
    ok &= (BOOL) add_stag( "P"         , 0 );
    ok &= (BOOL) add_stag( "PLAINTEXT" , HT_OBSOLETE );
    ok &= (BOOL) add_stag( "PRE"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "SAMP"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "SELECT"    , HT_CLOSE );
    ok &= (BOOL) add_stag( "STRONG"    , HT_CLOSE );
    ok &= (BOOL) add_stag( "TEXTAREA"  , HT_CLOSE );
    ok &= (BOOL) add_stag( "TITLE"     , HT_CLOSE|HT_REQUIRED|HT_ONLYONCE );
    ok &= (BOOL) add_stag( "TT"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "U"         , HT_CLOSE );
    ok &= (BOOL) add_tag ( "UL"        , HT_CLOSE, 0, handle_ul, handle_cul );
    ok &= (BOOL) add_stag( "VAR"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "XMP"       , HT_CLOSE | HT_OBSOLETE );
    ok &= (BOOL) add_tag ( "!"         , 0, 0, NULL, NULL ); /* only read until end of tag */

    /* NetScape HTML */
    ok &= (BOOL) add_stag( "FONT"      , HT_CLOSE | HT_JERK );
    ok &= (BOOL) add_stag( "CENTER"    , HT_CLOSE | HT_JERK );
    ok &= (BOOL) add_stag( "BASEFONT"  , HT_JERK );
    ok &= (BOOL) add_stag( "BLINK"     , HT_CLOSE | HT_JERK );
    ok &= (BOOL) add_stag( "NOBR"      , HT_CLOSE | HT_JERK );
    ok &= (BOOL) add_stag( "STRIKE"    , HT_CLOSE | HT_JERK );
    ok &= (BOOL) add_stag( "WBR"       , HT_CLOSE | HT_JERK );

#if 0
    /* HTML 2&3 tags (undocumented) */
    ok &= (BOOL) add_stag( "ABBREV"    , HT_CLOSE );
    ok &= (BOOL) add_stag( "ACRONYM"   , HT_CLOSE );
    ok &= (BOOL) add_stag( "AU"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "CAPTION"   , HT_CLOSE );
    ok &= (BOOL) add_stag( "CODE"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "CREDIT"    , HT_CLOSE );
    ok &= (BOOL) add_stag( "DEL"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "DFN"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "DIV"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "FIG"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "FN"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "INS"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "LANG"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "LH"        , HT_CLOSE );
    ok &= (BOOL) add_stag( "META"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "NOTE"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "OVERLAY"   , HT_CLOSE );
    ok &= (BOOL) add_stag( "PERSON"    , HT_CLOSE );
    ok &= (BOOL) add_stag( "Q"         , HT_CLOSE );
    ok &= (BOOL) add_stag( "S"         , HT_CLOSE );
    ok &= (BOOL) add_stag( "SAMP"      , HT_CLOSE );
    ok &= (BOOL) add_stag( "SMALL"     , HT_CLOSE );
    ok &= (BOOL) add_stag( "SUB"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "SUP"       , HT_CLOSE );
    ok &= (BOOL) add_stag( "TAB"       , 0 );
    ok &= (BOOL) add_stag( "TABLE"     , HT_CLOSE );
    ok &= (BOOL) add_stag( "TD"        , 0 );
    ok &= (BOOL) add_stag( "TH"        , 0 );
    ok &= (BOOL) add_stag( "TR"        , 0 );
    ok &= (BOOL) add_stag( "VAR"       , HT_CLOSE );
#endif


    /* HSC special tags */
    ok &= (BOOL) add_tag( "*"            , HT_NOCOPY, 0,
                                           handle_hsc_comment, NULL );
    ok &= (BOOL) add_tag( "|"            , HT_NOCOPY, 0,
                                           handle_hsc_onlycopy, NULL );
    ok &= (BOOL) add_tag( STR_HSC_MACRO  , HT_NOCOPY, 0,
                                           handle_hsc_macro, NULL );
    ok &= (BOOL) add_tag( STR_HSC_INCLUDE, HT_NOCOPY, 0,
                                           handle_hsc_include, NULL );
    ok &= (BOOL) add_tag( STR_HSC_TIME   , HT_NOCOPY, 0,
                                           handle_hsc_time, NULL );
    ok &= (BOOL) add_tag( STR_HSC_VAR    , HT_NOCOPY, 0,
                                           handle_hsc_var, NULL );


    /* entities */
    ok &= add_ent( "amp", NULL );      /* & */
    ok &= add_ent( "lt", NULL );       /* < */
    ok &= add_ent( "gt", NULL );       /* > */

    /* entities: german umlauts */
    ok &= add_ent( "auml", NULL );
    ok &= add_ent( "ouml", NULL );
    ok &= add_ent( "uuml", NULL );
    ok &= add_ent( "Auml", NULL );
    ok &= add_ent( "Ouml", NULL );
    ok &= add_ent( "Uuml", NULL );
    ok &= add_ent( "szlig", NULL );    

    ok &= add_ent( "copy", NULL );
    ok &= add_ent( "quot", NULL );
    ok &= add_ent( "otilde", NULL );
    ok &= add_ent( "ntilde", NULL );
    ok &= add_ent( "Icirc", NULL );

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
    BOOL   ok = TRUE;             /* return value */

    /* get current time */
    now = time( NULL );

    /*
    ** init lists
    */
    defent = init_dllist( del_entity);
    deftag = init_dllist( del_tag );
    cltags = init_dllist( NULL );
    macros = init_dllist( del_mac );
    vars   = init_dllist( del_var );
#if 0
    macarg = init_dllist( del_arg );
#endif

    if ( defent && deftag && cltags && macros /*TODO: "&& macarg"*/ ) {

        /*
        ** append tags with handles
        */
        config_tag_handles();

        /* printf list of entities & tags */
        if ( debug ) {

            DLNODE *nd;
            HSCENT *ent;
            HSCTAG *tag;


            nd = defent->first;
            if ( nd ) {

                fprintf( stderr, "** Known entities:" );
                while (nd) {

                    ent = (HSCENT*) nd->data;
                    fprintf( stderr, " &%s;", ent->name );
                    nd = nd->next;

                }

            }

            nd = deftag->first;
            if ( nd ) {

                fprintf( stderr, "\n**Known tags:" );
                while (nd) {

                    tag = (HSCTAG*) nd->data;
                    fprintf( stderr, " <" );
                    if ( tag->o_handle ) fprintf( stderr, "*" );
                    if ( tag->option & HT_CLOSE ) fprintf( stderr, "/" );
                    fprintf( stderr, "%s", tag->name );
                    if ( tag->o_handle ) fprintf( stderr, "*" );
                    fprintf( stderr, ">" );
                    nd = nd->next;

                }
                fprintf( stderr, "\n" );


            }
            errlf();

        }

    } else
        err_mem( NULL );


    return ok;
}

