/*
** tag_a.c
**
** tag handle for "<A..>" (anchor)
**
** updated: 10-Sep-1995
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
#include "vars.h"

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
BOOL handle_anchor( INFILE *inpf, HSCTAG *tag )
{
    DLLIST *vars   = tag->op_args;
    HSCVAR *vhref  = find_varname( vars, "HREF" );
    HSCVAR *vname  = find_varname( vars, "NAME" );
    STRPTR  href   = NULL;
    STRPTR  name   = NULL;

    /* reset some vars */
    ufreestr( last_anchor );
    last_anchor = NULL;
    was_ext_uri = FALSE;

    /* set attribute values */
    if ( vhref ) href = vhref->text;
    if ( vname ) name = vname->text;

    /* read first option */

    /* check for both HREF and NAME missing */
    if ( (!href) && (!name) ) {

        message( ERROR_A_NOARG, inpf );
        errtag( "A" );
        errstr( " without HREF or NAME\n" );

    }


    /* write whole tag */
    if ( !(was_ext_uri && stripuri) ) {

        /* TODO: write log */

    } else {

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

