/*
** tag_img.c
**
** tag handle for "<IMG ..>"
**
** updated:  2-Aug-1995
** created: 23-Jul-1995
*/

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/infile.h"

#include "global.h"
#include "error.h"
#include "tagargs.h"
#include "msgid.h"
#include "output.h"
#include "parse.h"

/*
** global vars
*/

/*
** handle_img
**
** handle tag <IMG>: check for SRC="xx", test if referenced
** file really exitst, check for ALT="xx"
*/
BOOL handle_img( INFILE *inpf )
{
    STRPTR src   = NULL;
    STRPTR alt   = NULL;
    STRPTR align = NULL;
    STRPTR nxtoptn;
    BOOL   match;

    nxtoptn = parse_tagoptn( inpf );
    while ( nxtoptn ) {

        outstr( " " );
        outstr( nxtoptn );

        if ( !upstrcmp( nxtoptn, "ALT" ) ) {

            alt = parse_strarg( inpf );          /* get alt name */
            if ( src ) {

                outstr( "=\"" );
                outstr( alt );
                outstr( "\"" );

            }


        } else if ( !upstrcmp( nxtoptn, "ALIGN" ) ) {

            if ( parse_eq( inpf ) )
                align = parse_mutex( "TOP:MIDDLE:BOTTOM", inpf, &match, TRUE );
                if ( align ) {

                    outstr( "=" );
                    outstr( align );

                }

        } else if ( !upstrcmp( nxtoptn, "SRC" ) ) {

            src = parse_uri( inpf );          /* get filename */
            if ( src ) {

                outstr( "=\"" );
                outstr( src );
                outstr( "\"" );

            }


        } else if ( !upstrcmp( nxtoptn, "ISMAP" ) ) {

            /* do nufin */;

        } else {
        }


        nxtoptn = parse_tagoptn( inpf );

    }

    /* check for SRC missing */
    if (!src) {

        message( ERROR_IMG_NOSRC, inpf );
        errstr( "missing SRC for " );
        errtag( "IMG" );
        errlf();

    }

    /* check for ALT missing */
    if ( !alt ) {

        message( WARN_IMG_NOALT, inpf );
        errstr( "missing ALT for " );
        errtag( "IMG" );
        errlf();

    }

    return (TRUE);
}
