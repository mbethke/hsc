/*
** config.c
**
** config handling for hsc; reads "hscdef.cfg"
**
** updated:  5-Aug-1995
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


#define NOEXTERN_HSC_CONFIG_H
#include "config.h"

#define HSC_CONFIG_BUFSIZE 512
#define HSC_CONFIG_WORDLEN  64




/*
** read_deftag: read a new tag from configfile
**
** TODO: check for redefinition
*/
BOOL read_deftag( INFILE *cfgf )
{
    BOOL result = TRUE;
    HSCTAG *newtag;
    STRPTR nxtwd;

    nxtwd = infgetw( cfgf );           /* read tag id */
    newtag = new_hsctag( nxtwd );      /* ..and created new tag with it */
    /* TODO: check out of mem */

    if ( newtag && (!infeof(cfgf)) ) {

        /*
        ** read tag options
        */
        nxtwd = upstr( infgetw( cfgf ) );

        while ( (!infeof( cfgf)) && (nxtwd[0]!='#') && (nxtwd[0]!=0x0a) ) {

            if ( !strcmp( nxtwd, HT_CLOSE_ID ) )           /* CLOSE */
                newtag->option |= HT_CLOSE;
            else if ( !strcmp( nxtwd, HT_REQUIRED_ID ) )   /* REQUIRED */
                newtag->option |= HT_REQUIRED;
            else if ( !strcmp( nxtwd, HT_ONLYONCE_ID ) )   /* ONLYONCE */
                newtag->option |= HT_ONLYONCE;
            else if ( !strcmp( nxtwd, HT_VERS_ID ) ) {     /* VERS=xx */
                nxtwd = infgetw( cfgf );
                if (!strcmp( nxtwd, "=" ) ) {
                    /*
                    ** TODO: set version
                    */
                    infgotoeol( cfgf );
                } else  {                                  /* unknown option */

                    message( ERROR, cfgf );
                    errstr( "\"=\" expected after VERS\n" );

                }

            } else {

                message( ERROR, cfgf );
                errstr( "Warning: Unknown tag option: " );
                errqstr( nxtwd );
                errlf();
            }

            nxtwd = upstr( infgetw(cfgf) );

        }

        /* append new tag to list */
        app_dlnode( deftag, (APTR) newtag );
        result = TRUE;
    }

    return result;
}


/*
** read_defent
**
** define new entity, eg "DEFENT szlig"
*/
BOOL read_defent( INFILE *cfgf )
{
    BOOL result = FALSE;
    HSCENT *newent = (HSCENT*) malloc( sizeof(HSCENT) );

    if (newent) {

        newent->name = strclone( infgetw(cfgf) );
        newent->source = NULL;

        app_dlnode( defent, (APTR) newent );
        result = TRUE;
    }

    return result;
}


/*
** read_config
*/
BOOL read_config( INFILE *cfgf )
{
    STRPTR nxtwd;
    BOOL result = TRUE;


    nxtwd = upstr( infgetw( cfgf ) );

    while ( !infeof( cfgf ) ) {


        if ( !strcmp( "#", nxtwd ) )

            /* skip comment */;

        else if ( !upstrcmp( "DEFTAG", nxtwd ) )

            result = read_deftag( cfgf );        /* define new tag */

        else if ( !upstrcmp( "DEFENT", nxtwd ) )

            result = read_defent( cfgf );        /* define new entity */

        else if ( (nxtwd[0] != 0x0a) ) {

            message( ERROR,  cfgf );
            errstr( "Unknown config command: " );
            errqstr( nxtwd );
            errlf();
            result = TRUE;

        }

        if (nxtwd[0]!=0x0a)
            infgotoeol( cfgf );
        nxtwd = infgetw( cfgf );

    }

    return result;
}


