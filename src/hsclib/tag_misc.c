/*
 * hsclib/tag_misc
 *
 * misc. tag-callbacks
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
 * updated: 13-Oct-1996
 * created: 30-Jul-1995
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

    return (TRUE);
}

/*
 * handle_blink: tag handle for <BLINK>
 *
 * just tell the user that blink sucks
 */
BOOL handle_blink(HSCPRC * hp, HSCTAG * tag)
{
    hsc_message(hp, MSG_BLINK_SUX, "%t sucks", "BLINK");

    return (TRUE);
}

/*
 * handle_blink: tag handle for <BLINK>
 *
 * just tell the user that frames are disgusting
 */
BOOL handle_frame(HSCPRC * hp, HSCTAG * tag)
{
    hsc_message(hp, MSG_FRAME_SUX, "frames are disgusting");

    return (TRUE);
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

    if ((hp->prev_heading_num - num) < (-1))
    {
        char hstr[4];

        sprintf(hstr, "H%ld", hp->prev_heading_num + 1);
        hsc_message(hp, MSG_WRONG_HEADING,
                    "expected heading %t", hstr);
    }

    hp->prev_heading_num = num;

    return (TRUE);
}

/*
 * handle_img: tag handle for <IMG>
 *
 *
 */
BOOL handle_img(HSCPRC * hp, HSCTAG * tag)
{
    /* TODO: check for GIF file */

    return (TRUE);
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

    return (TRUE);
}

/*
 * handle_end_pre: tag handle for </PRE>
 *
 * disable switch inside_pre
 */
BOOL handle_end_pre(HSCPRC * hp, HSCTAG * tag)
{
    hp->inside_pre = FALSE;

    return (TRUE);
}

/*
 * handle_sgml_comment: tag handle for <!..>
 *
 *
 */
BOOL handle_sgml_comment(HSCPRC * hp, HSCTAG * tag)
{
    INFILE *inpf = hp->inpf;
    BOOL not_stripped = TRUE;

    STRPTR nw = infgetw(inpf);

    if (nw)
    {
        BOOL comment = FALSE;
        BOOL oneword = FALSE;
        BOOL end_min = FALSE;

        /* append to attribute string */
        app_estr(hp->tag_attr_str, infgetcws(inpf));
        app_estr(hp->tag_attr_str, infgetcw(inpf));

        if (!strncmp(nw, "--", 2))
        {
            size_t slen = strlen(nw);   /* length of current word */

            comment = TRUE;

            /* check for "--" */
            if (slen >= 4)
            {
                /* word starts with "--" and ends with "--" */
                end_min = ((nw[slen - 1] == '-') && (nw[slen - 1] == '-'));
                oneword = TRUE;
            }
        }

        /* check for whitespace after "!" */
        if (strlen(infgetcws(inpf)))
            hsc_msg_illg_whtspc(hp);

        if (!strcmp(nw, ">"))   /* zero-comment */
            hsc_message(hp, MSG_ONEW_COMMENT, "empty sgml comment");
        else if (!comment)
            skip_until_eot(hp, hp->tag_attr_str);       /* unknown "!"-command: skip */
        else
        {
            /* handle comment */
            BOOL end_gt = FALSE;
            BOOL in_quote = FALSE;

            while (!(hp->fatal) && !end_gt)
            {
                /* read next word */
                nw = infgetw(inpf);

                if (nw)
                {
                    size_t slen = strlen(nw);

                    /* append word to attribute string */
                    app_estr(hp->tag_attr_str, infgetcws(inpf));
                    app_estr(hp->tag_attr_str, infgetcw(inpf));

                    /*
                     *check for "--"
                     */
                    if (slen >= 2)
                        end_min =
                            ((nw[slen - 1] == '-') && (nw[slen - 1] == '-'));

                    /*
                     * check for end-of-comment
                     */
                    if (!strcmp(nw, ">"))
                        if (end_min)
                            end_gt = TRUE;
                        else
                        {
                            hsc_message(hp, MSG_GT_IN_COMMENT,
                                        "%q inside sgml-comment", ">");
                        }
                    else
                    {
                        if (oneword)
                        {
                            oneword = FALSE;
                            end_min = FALSE;
                        }

                        /* check for LF */
                        if (!strcmp(nw, "\n"))
                        {
                            in_quote = FALSE;
                            hsc_message(hp, MSG_LF_IN_COMMENT,
                                        "line feed inside sgml-comment");
                        }
                        /* check for quote */
                        else if (!strcmp(nw, "\"") || !strcmp(nw, "'"))
                            in_quote = !in_quote;
                    }
                }
                else
                    hsc_msg_eof(hp, "reading sgml-comment");

                if (end_gt && in_quote)
                {
                    hsc_message(hp, MSG_CMTEND_QUOTE,
                                "sgml-comment ends inside quotes");
                }

                if (end_gt && oneword)
                    hsc_message(hp, MSG_ONEW_COMMENT,
                                "sgml-comment consists of a single word");
            }
        }

        /* check if comment should be stripped */
        if (hp->strip_cmt)
        {
            hsc_msg_stripped_tag(hp, tag, "sgml-comment");
            not_stripped = FALSE;
        }
    }
    else
        hsc_msg_eof(hp, "reading sgml-comment");

    return (not_stripped);
}

