/*
** tag_a.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** tag handle for "<A..>" (anchor)
**
** updated: 27-Nov-1995
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
#include "uri.h"

#include "tag.h"
#include "vars.h"

/*
** TODO:
** - strip only HREF part, if a NAME is within <A>
** - insabg, insaed: configurable format str for inserted anchors
** - local INSANCH option for A HREF tag
*/

/* flag that tells parse if he is currently inside
** an anchor (need for "click here" syndrome)
*/
BOOL inside_anchor = FALSE; /* TODO: move this to "global.c" */

/*
** handle_anchor
**
** handle tag <A>:
** - check for HREF or NAME set
** - set new value of last_anchor
**
** TODO: handle NAME="xx"
*/
BOOL handle_anchor( INFILE *inpf, HSCTAG *tag )
{
    DLLIST *vars   = tag->attr;
    HSCVAR *vhref  = find_varname( vars, "HREF" );
    HSCVAR *vname  = find_varname( vars, "NAME" );
    STRPTR  href   = NULL;
    STRPTR  name   = NULL;

    /* set attribute values */
    if ( vhref ) href = vhref->text;
    if ( vname ) name = vname->text;

    /* tell parser that he is inside an anchor */
    if ( href )
        inside_anchor = TRUE;

    /* check for both HREF and NAME missing */
    if ( (!href) && (!name) ) {

        message( MSG_ANCH_NO_NMHR, inpf );
        errtag( "A" );
        errstr( " without HREF or NAME\n" );

    }


    /* write whole tag */
/*    outstr( infget_log(inpf) ); */

#if 0 /* TODO: remove this */
    } else {

        if ( href ) {

            message( MSG_STRIPPED_TAG, inpf );
            errstr( "Stripped external HREF to " );
            errqstr( href );
            errlf();

        }
    }
#endif

    return (TRUE);
}



/*
** handle_cancher
**
** closing handle for <A>
*/
BOOL handle_canchor( INFILE *inpf, HSCTAG *tag)
{
    inside_anchor = FALSE;

    /* write whole tag */
/*    outstr( infget_log(inpf) ); */

    return (TRUE);

}

