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
 * hsclib/tag_misc
 *
 * misc. tag-callbacks
 */

#include "hsclib/inc_base.h"

#include "hsclib/parse.h"
#include "hsclib/skip.h"

/*
 *
 * global funs
 *
 */

/*
 * handle_base: tag handle for <BASE>
 *
 * enable switch docbase_set; this affects "uri.c"
 * and from now on, all relative uris will be absolute
 */
BOOL handle_base(HSCPRC * hp, HSCTAG * tag)
{
    STRPTR href_arg = get_vartext_byname(tag->attr, "HREF");

    if (href_arg)
    {
        D(fprintf(stderr, DHL "BASE SET `%s'\n", href_arg));
        hp->docbase_set = TRUE;
    }

    return TRUE;
}

/*
 * handle_blink: tag handle for <BLINK>
 *
 * just tell the user that blink sucks
 */
BOOL handle_blink(HSCPRC * hp, HSCTAG * tag)
{
    hsc_message(hp, MSG_BLINK_SUX, "%t sucks", "BLINK");

    return TRUE;
}

/*
 * handle_heading: tag handle for <H1>..<H6>
 *
 * compute number of haeding,
 * compare it with previous heading,
 * check first heading to be <H1>
 *
 */
BOOL handle_heading(HSCPRC * hp, HSCTAG * tag)
{
    BYTE num = (tag->name[1] - '0');    /* num of heading (1..6) */

    D(fprintf(stderr, DHL "  heading %d\n", num));

    /* check for <Hx> inside <A> */
    if (find_strtag(hp->container_stack, "A"))
    {
        hsc_message(hp, MSG_ANCHOR_HEADING, "heading inside anchor");
    }

    if ((hp->prev_heading_num - num) < (-1))
    {
        char hstr[4];

        sprintf(hstr, "H%ld", hp->prev_heading_num + 1);
        hsc_message(hp, MSG_WRONG_HEADING,
                    "expected heading %t", hstr);
    }

    hp->prev_heading_num = num;
    return TRUE;
}

/*
 * handle_img: tag handle for <IMG>
 *
 *
 */
BOOL handle_img(HSCPRC * hp, HSCTAG * tag)
{
    /* ...and I'm happy when it rains */
    return TRUE;
}

/*
 * handle_pre: tag handle for <PRE>
 *
 * enable switch inside_pre; this avoid stripping
 * of whtspcs with option "compact" enabled
 */
BOOL handle_pre(HSCPRC * hp, HSCTAG * tag)
{
    hp->inside_pre = TRUE;
    return TRUE;
}

/*
 * handle_end_pre: tag handle for </PRE>
 *
 * disable switch inside_pre
 */
BOOL handle_end_pre(HSCPRC * hp, HSCTAG * tag)
{
    hp->inside_pre = FALSE;
    return TRUE;
}

/*
 * handle_sgml_comment: tag handle for <!..>
 *
 */
BOOL handle_sgml_comment(HSCPRC * hp, HSCTAG * tag)
{
    BOOL stripped = FALSE;
    EXPSTR *content = hp->tag_attr_str;

    /* skip data */
    skip_sgml_special(hp, content, &stripped);

    return !stripped;
}

/* $Id$ */
/* vi: set ts=4: */
