/*
** tag_a.c
**
** tag handle for "<A..>" (anchor)
**
** updated:  8-Sep-1995
** created:  3-Aug-1995
*/

#include <stdio.h>
#include <string.h>

#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/infile.h"

#include "global.h"
#include "error.h"
#include "tagargs.h"
#include "msgid.h"
#include "output.h"
#include "parse.h"

#include "tag.h"


/*
** TODO:
** - strip only HREF part, if a NAME is within <A>
** - insabg, insaed: configurable format str for inserted anchors
** - local INSANCH option for A HREF tag
*/

/*
** this flag is set by handle_anchor if the HREF goes to
** an global URI. if this  flag and the stripuri
** option is enabled, handle_clanchor will not write
** a </A> to the output
*/
BOOL was_ext_uri = FALSE;

/*
** handle_anchor
**
** handle tag <A>:
** - check for HREF="xx",
** - increase anchor_nesting
** - set new value of last_anchor
**
** TODO: handle NAME="xx"
*/
BOOL handle_anchor( INFILE *inpf )
{
    STRPTR href   = NULL;
    STRPTR name   = NULL;
    STRPTR nxtoptn;

    /* reset some vars */
    ufreestr( last_anchor );
    last_anchor = NULL;
    was_ext_uri = FALSE;

    /* read first option */
    nxtoptn = parse_tagoptn( inpf );
    while ( nxtoptn ) {

        strcat( this_tag, infgetcws( inpf) );
        strcat( this_tag, infgetcw( inpf) );

        if ( !upstrcmp( nxtoptn, "NAME" ) ) {

            name = parse_strarg( inpf );          /* get name */
            if ( name ) {

                strcat( this_tag, infgetcws( inpf) );
                strcat( this_tag, "=\"" );
                strcat( this_tag, name );
                strcat( this_tag, "\"" );
                if ( debug )
                     fprintf( stderr, "** NAME for \"%s\"\n", name );

            }

#if 0
        } else if ( !upstrcmp( nxtoptn, "ALIGN" ) ) {

            if ( parse_eq( inpf ) )
                align = parse_mutex( "TOP:MIDDLE:BOTTOM", inpf, &match, TRUE );
                if ( align ) {

                    strcat( this_tag, "=" );
                    strcat( this_tag, align );

                }
#endif

        } else if ( !upstrcmp( nxtoptn, "HREF" ) ) {

            href = parse_uri( inpf );          /* get filename */
            if ( href ) {

                strcat( this_tag, "=\"" );
                strcat( this_tag, href );
                strcat( this_tag, "\"" );

                /* remember current anchor to write out it after </A> */
                if ( insanch )
                    last_anchor = strclone( href );

            }
            /* if a ':' is found in the uri-path, it's an global uri */
            was_ext_uri = ( (!href) || strchr(href,':') );


        } else {

            /* just copy unknown options */
            /* TODO: a bit silly, cause also a "=" and args after */
            /*  unknown options are treted as unknwon option; */
            /*  possible problems, if eg: HUGO=NAME */
            strcat( this_tag, infgetcws( inpf ) );
            strcat( this_tag, infgetcw( inpf ) );

        }

        nxtoptn = parse_tagoptn( inpf );

    }

    /* check for HREF missing */
    if ( (!href) && (!name) ) {

        message( ERROR_A_NOARG, inpf );
        errtag( "A" );
        errstr( " without HREF or NAME\n" );

    }


    /* write whole tag */
    if ( !(was_ext_uri && stripuri) ) {

        outstr( this_tag );
        copy_until_gt( inpf );

    } else {
        size_t i = 0;

        /* write out white spaces before tag */
        while ( inf_isws( this_tag[i], inpf ) ) {

            outch( this_tag[i] );
            i++;

        }

        skip_until_gt( inpf );

        if ( href ) {

            message( MSG, inpf );
            errstr( "Stripped external HREF to " );
            errqstr( href );
            errlf();

        }
    }

    return (TRUE);
}


/*
** handle_cancher
**
** closing handle for <A>
** - decrease anchor_nesting
*/
BOOL handle_canchor( INFILE *inpf )
{
    if ( anchor_nesting) anchor_nesting--;

    if ( !(was_ext_uri && stripuri) ) {

        outstr( this_tag );
        copy_until_gt( inpf );

    } else {

        /* skip rest of tag */
        skip_until_gt( inpf );

        /* write URI of anchor */
        if ( last_anchor ) {

            /* write out italic URI */
            outstr( " (" );
            outstr( last_anchor );
            outstr( ") " );

            /* release mem used by URI */
            ufreestr( last_anchor );
            last_anchor = NULL;
        }

    }

    return (TRUE);

}

