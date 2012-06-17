/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 */
/*
 * hsclib/input.c
 *
 * basic functions for parsing input
 */

#include <ctype.h>

#include "hsclib/inc_base.h"

/*
 *---------------------------
 * overloaded methods for
 * INFILE class
 *---------------------------
 */

/*
 * hsc_whtspc
 *
 * decides if an char is a white-space
 *
 * params: ch...char to test
 * result: TRUE, if ch is a 'normal' ch
 *
 * NOTE: this function is used as is_ws-methode
 *       for the INFILE class
 */
BOOL hsc_whtspc(int ch)
{
    if ((ch == ' ')
        || (ch == '\n')
        || (ch == '\r')
        || (ch == '\t')
        )
    {
        return TRUE;
    }
    else
        return FALSE;
}

/*
 * hsc_normch
 *
 * decides if an char is an 'normal' char
 *
 * params: ch...char to test
 * result: TRUE, if ch is a 'normal' ch
 *
 * NOTE: this function is used as is_nc-method
 *       for the INFILE class
 */
BOOL hsc_normch(int ch)
{
    if (((ch >= '0') && (ch <= '9'))
        || ((ch >= 'a') && (ch <= 'z'))
        || ((ch >= 'A') && (ch <= 'Z'))
        || (ch == '_') || (ch == '-') || (ch == '.')
        )
    {
        return TRUE;
    }
    else
        return FALSE;
}

/*
 * hsc_normch_tagid
 *
 * decides if an char is an 'normal' char for tagnames
 *
 * params: ch...char to test
 * result: TRUE, if ch is a 'normal' ch
 *
 * NOTE: this function is used as is_nc-methode
 *       for the infile class
 */
BOOL hsc_normch_tagid(int ch)
{
    BOOL found = hsc_normch(ch);

    if (!found)
        if (strchr(HSC_TAGID, ch))
            found = TRUE;

    return (found);
}

/*
 *-------------------------------------
 * read a tag/attribute name from input file
 *-------------------------------------
 */

/*
 * infget_tagid
 *
 * read next word from input, but with a
 * different is_nc-method that also handles
 * the id for hsc-tags (usually "$")
 */
STRPTR infget_tagid(HSCPRC * hp)
{
    INFILE *inpf = hp->inpf;
    STRPTR tagid = NULL;

    BOOL(*old_is_nc) (int ch);

    old_is_nc = inpf->is_nc;    /* remember old is_nc-method */
    inpf->is_nc = hsc_normch_tagid;
    tagid = infgetw(inpf);      /* read tagid */
    if (!tagid)
        hsc_msg_eof(hp, "reading tag name");
    inpf->is_nc = old_is_nc;    /* remember old is_nc-method */

    /* if LOWERCASETAGS option is set, force tag to lowercase */
    if(hp->lctags && (NULL != tagid)) {
       lowstr(tagid);
    }
    return (tagid);
}

/*
 * infget_attrid
 *
 * read next word from input, but with a
 * different is_nc-methode that also handles
 * the id for hsc-attribs (usually "$")
 */
STRPTR infget_attrid(HSCPRC * hp)
{
    INFILE *inpf = hp->inpf;
    STRPTR attrid = NULL;

    BOOL(*old_is_nc) (int ch);

    old_is_nc = inpf->is_nc;    /* remember old is_nc-method */
    inpf->is_nc = hsc_normch_tagid;
    attrid = infgetw(inpf);     /* read attrid */
    if (!attrid)
        hsc_msg_eof(hp, "reading attribute name");
    inpf->is_nc = old_is_nc;    /* restore old is_nc-method */

    return (attrid);
}

/*
 *-------------------------------------
 * parse simple chars/words
 *-------------------------------------
 */

/*
 * parse_wd
 *
 * check if a expected word really occured and
 * display error message if neccessary
 *
 * params: inpf.....input file to read char from
 *         expstr...expected word
 * result: TRUE if successful, FALSE if wrong char found
 */
BOOL parse_wd(HSCPRC * hp, STRPTR expstr)
{
    INFILE *inpf = hp->inpf;
    BOOL value = TRUE;

    if (expstr)
    {
        STRPTR nw = infgetw(inpf);

        /* check for expeted word */
        if (!nw || upstrcmp(nw, expstr))
        {
            if (!nw)
                nw = "<EOF>";

            hsc_message(hp, MSG_UNEXPT_CH,
                        "expected %q, found %q", expstr, nw);
            value = FALSE;
        }
    }
    else
    {
        panic("no data to expect");
    }

    return (value);
}

/*
 * parse_eq
 *
 * check for '='
 *
 * params: inpf...input file to read char from
 * result: -1 if successful, 0 if wrong char found
 */
BOOL parse_eq(HSCPRC * hp)
{
    return (parse_wd(hp, "="));
}

/*
 * parse_gt
 *
 * check for '>'
 *
 * params: inpf...input file to read char from
 * result: -1 if successful, 0 if wrong char found
 */
BOOL parse_gt(HSCPRC * hp)
{
    return (parse_wd(hp, ">"));
}

/* $Id: input.c,v 1.2 2003/07/06 04:37:34 mb Exp $ */
/* vi: set ts=4: */

