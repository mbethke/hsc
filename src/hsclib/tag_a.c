/*
** hsclib/tag_a.c
**
** tag-callback for "<A..>" (anchor)
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
** updated:  7-Jul-1996
** created:  3-Aug-1995
*/

#include "hsclib/inc_tagcb.h"
#include "hsclib/document.h"

/*
** TODO: strip only HREF part, if a NAME is within <A>
** TODO: insabg, insaed: configurable format str for inserted anchors
** TODO: local $INSANCH option for A HREF tag
*/

/*
** handle_anchor
**
** handle tag <A>:
** - check for HREF or NAME set
** - update value for attribute __anchor__
**
** TODO: handle NAME="xx"
*/
BOOL handle_anchor( HSCPRC *hp, HSCTAG *tag )
{
    HSCVAR *vhref  = find_varname( tag->attr, "HREF" );
    HSCVAR *vname  = find_varname( tag->attr, "NAME" );
    STRPTR  href   = NULL;
    STRPTR  name   = NULL;

    /* set attribute values */
    if ( vhref ) href = vhref->text;
    if ( vname ) name = vname->text;

    /* tell parser that he is inside an anchor */
    if ( href ) {

        HSCATTR *anchor_attr = find_varname( hp->defattr, ANCHOR_ATTR );

        if ( anchor_attr ) {
            set_vartext( anchor_attr, href );
        } else {
            panic( "no anchor-attribute" );
        }

        hp->inside_anchor = TRUE;
    }

    /* check for both HREF and NAME missing */
    if ( (!href) && (!name) ) {

        hsc_message( hp, MSG_ANCH_NO_NMHR,
                     "%T without HREF or NAME", tag );

    }

    return (TRUE);
}



/*
** handle_cancher
**
** closing handle for <A>
*/
BOOL handle_canchor( HSCPRC *hp, HSCTAG *tag)
{
    hp->inside_anchor = FALSE;

    /* write whole tag */

    return (TRUE);

}

