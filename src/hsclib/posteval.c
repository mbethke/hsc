/*
 * hsclib/posteval.c
 *
 * functions to postprocess attributes
 * (remember IDs, references, etc)
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * updated:  8-Jul-1995
 * created:  7-Jul-1995
 */

#define NOEXTERN_HSCLIB_EVAL_H

#include <ctype.h>

#include "hsclib/inc_base.h"

#include "hsclib/document.h"
#include "hsclib/idref.h"
#include "hsclib/uri.h"

/*
 * postprocess_attributes
 *
 * This functions scans a tag's list of attributes for an URI-attributes
 * refering to an external URI. If it succeeds, and the hsc-process
 * has it's hp->strip_ext flag enabled, the function exits.
 *
 * Otherwise, it scans the attributes for new IDs and references
 * and updates the document-data if neccessary (but only for start-tags)
 *
 * params:
 *   hp ....... hsc-process
 *   tag ... tag whichs attribute list chould be examined
 *   open_tag .. for end-tags, the document-data should not be
 *               updated again
 * result:
 *   TRUE, if tag should NOT be stripped
 */
BOOL postprocess_tagattr(HSCPRC * hp, HSCTAG * tag, BOOL open_tag)
{
    BOOL dontstrip = TRUE;

    if (tag->attr) {

        /*
         * find out, if list should be refused
         */
        if (hp->strip_ext
            && tag->uri_stripext
            && get_vartext(tag->uri_stripext)
            && (uri_kind(get_vartext(tag->uri_stripext)) == URI_ext)
            ) {
            D(fprintf(stderr, DHL "strip external\n"));
            dontstrip = FALSE;
        } else if (open_tag) {
            /*
             * search for new IDs and references
             */
            DLNODE *nd = dll_first(tag->attr);
            while (nd) {
                HSCATTR *attrib = (HSCATTR *) dln_data(nd);
                STRPTR value = get_vartext(attrib);

                if (value) {
                    if (attrib->vartype == VT_URI) {
                        /* new reference */
                        INFILEPOS *fpos = new_infilepos( hp->inpf);
                        CALLER *newcaller = fpos2caller( fpos);
                        HSCREF *newref = app_reference(hp->document, value);

                        newref->caller = newcaller;

                        del_infilepos(fpos);
                        D(fprintf(stderr, DHL "new REFERENCE: `%s'\n", value));

                    } else if (attrib->vartype == VT_ID) {
                        /* new id defined */
                        D(fprintf(stderr, DHL "new ID: `%s'\n", value));
                        add_local_iddef(hp, value);
                    }
                }
                nd = dln_next(nd);
            }
        }
    }
    return (dontstrip);
}

