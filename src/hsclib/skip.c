/*
 * skip.c
 *
 * functions for skipping several things
 *
 * Copyright (C) 1995,96 Thomas Aglassinger
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
 * updated: 30-Oct-1996
 * created:  8-Oct-1995
 */

#define NOEXTERN_HSCLIB_SKIP_H

#include "hsclib/inc_base.h"

#include "hsclib/input.h"
#include "hsclib/skip.h"

/* debug skip */
#if 1
#define DS(x) if(hp->debug) x
#else
#define DS(x)
#endif
#define DHLS "*hsclib* skip: "

/*
 * skip_next_lf
 *
 * ignore '\n'
 *
 * params: inpf...input file to read char from
 * result: TRUE if skipped
 */
BOOL skip_next_lf(HSCPRC * hp)
{
    /* TODO: skip white-spaces after linefeed,
     * if COMPACT set; but this has to be performed
     * at another part of the code... */
    INFILE *inpf = hp->inpf;

    int nc = infgetc(inpf);

    if (nc != '\n')
        inungetc(nc, inpf);

    return ((BOOL) (nc == EOF));
}

BOOL eot_reached(HSCPRC * hp, BYTE * state)
{
    INFILE *inpf = hp->inpf;
    STRPTR nw = infgetw(inpf);

    if (nw)
    {
        switch (*state)
        {
        case TGST_TAG:
            if (!strcmp(nw, "\""))
                *state = TGST_DQUOTE;
            else if (!strcmp(nw, "'"))
                *state = TGST_QUOTE;
/* TODO: skip references & expressions */
#if 0
            else if (!strcmp(nw, "<"))
                *state = TGST_REF;
#endif
            else if (!strcmp(nw, ">"))
                *state = TGST_END;
            break;

        case TGST_REF:
        case TGST_QUOTE:
        case TGST_DQUOTE:

            if (strcmp(nw, "\n"))
            {
                switch (*state)
                {

                case TGST_REF:
                    if (!strcmp(nw, ">"))
                        *state = TGST_TAG;
                    break;

                case TGST_QUOTE:
                    if (!strcmp(nw, "'"))
                        *state = TGST_TAG;
                    break;

                case TGST_DQUOTE:
                    if (!strcmp(nw, "\""))
                        *state = TGST_TAG;
                    break;
                }
            }
            else
            {
                /* unexpected end of line */
                hsc_msg_eol(hp);
                *state = TGST_TAG;      /* go on reading inside tag */
            }

            break;
        }
    }
    else
    {
        hsc_msg_eof(hp, "`>' expected");
        *state = TGST_ERR;
    }

    return ((BOOL) ((*state == TGST_END) || (*state == TGST_ERR)));
}

/*
 * skip_until_eot_args
 *
 * skip until end of tag reached,
 * with user definable status vars
 *
 * params: inpf.....input file
 *         quote....status for quote (TRUE=inside quote)
 *         dquote...status for double quote
 *         argattr..status for quote
 * result: TRUE, if no fatal error
 * errors: return FALSE
 */
BOOL skip_until_eot_state(HSCPRC * hp, BYTE * state, EXPSTR * logstr)
{
    INFILE *inpf = hp->inpf;

    while (!eot_reached(hp, state))
        if (logstr)
        {
            app_estr(logstr, infgetcws(inpf));
            app_estr(logstr, infgetcw(inpf));
        }

    /* append ">" */
    if (logstr)
    {
        app_estr(logstr, infgetcws(inpf));
        app_estr(logstr, infgetcw(inpf));
    }

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_until_eot
 *
 * skip until end of tag reached
 *
 * params: inpf..input file
 * result: TRUE, if no fatal error
 * errors: return FALSE
 */
BOOL skip_until_eot(HSCPRC * hp, EXPSTR * logstr)
{
    BYTE state = TGST_TAG;

    return (skip_until_eot_state(hp, &state, logstr));
}

/*
 *-----------------
 * skip comment
 *-----------------
 */

/*
 * eoc_reched
 *
 * check if end of an hsc-comment is reached
 *
 * params:
 *   inpf...where to read next word from
 *   state..state var; has to be initiales by
 *          calling func with CMST_TEXT
 *   nest...comment netsing counter; has to be
 *          initiales by calling func with 0
 * result: TRUE, if end of comment reached
 */
BOOL eoc_reached(HSCPRC * hp, BYTE * state, LONG * nest)
{
    INFILE *inpf = hp->inpf;
    STRPTR nw = infgetw(inpf);

    if (nw)
    {
        switch (*state)
        {
        case CMST_TEXT:
            if (!strcmp(nw, "*"))
                *state = CMST_STAR;
            else if (!strcmp(nw, "<"))
                *state = CMST_TAG;
            break;

        case CMST_STAR:
            if (!strcmp(nw, "*"))
                *state = CMST_STAR;
            else if (!strcmp(nw, "<"))
                *state = CMST_TAG;
            else if (!strcmp(nw, ">"))
                if (*nest)
                {
                    (*nest)--;
                    *state = CMST_TEXT;
                }
                else
                    *state = CMST_END;

            break;

        case CMST_TAG:
            if (!strcmp(nw, "<"))
                *state = CMST_TAG;
            else
            {
                if (!strcmp(nw, "*"))
                    (*nest)++;
                *state = CMST_TEXT;
            }
            break;
        }
    }
    else
    {
        hsc_msg_eof(hp, "missing end of comment (\"*>\")");
        *state = CMST_ERR;
    }

    return ((BOOL) ((*state == CMST_END) || (*state == CMST_ERR)));
}

/*
 * skip_hsc_comment
 *
 * skip text until '*>' occures;
 * nested commets are supported
 *
 */
BOOL skip_hsc_comment(HSCPRC * hp)
{
    BYTE cstate = CMST_TEXT;    /* vars for eoc_reached() */
    LONG cnest = 0;
    BOOL end = FALSE;           /* end of comment reached? */

    while (!end && !(hp->fatal))
    {
        end = eoc_reached(hp, &cstate, &cnest);
    }

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_until_tag
 *
 * skip everythin, until a specific tag (one of tagstoplist or tagnest)
 * is found.
 *
 * params:
 *  hp hsc-process to work with
 *  tagfound     destination string that will store name of tag that lead
 *               to abortion of skip (eg "$else"); if this string is NULL,
 *               it will be ignored
 *  tagstoplist  list of tags to stop on, sparated with vertical bars `|'
 *               eg. "$else|$elseif"
 *  tagnest      single tag, that maintains a nesting-counter, depending
 *               on wheter it occures as a start-tag or not; if the
 *               nesting-counter is 0 and it occures as a stop-tag, it
 *               will also stop skipping (eg "$if")
 */
#define STATE_TEXT            1 /* normal text */
#define STATE_TAG             2 /* after "<" */
#define STATE_COMMENT         3 /* inside hsc-comment */
#define STATE_COMMENT_STAR    4 /* inside hsc-comment, after "*" */
#define STATE_TAG_STOP        5 /* found tag in stoplist */
#define STATE_SKIP            6 /* inside `skip section' "<|..|>" */
#define STATE_VBAR            7 /* inside `skip section', after "|" */
#define STATE_TAGNAME         8 /* after tagname */
#define STATE_ENDTAGNAME      9 /* after end-tagname */

#define STATE_TAGATTR        10 /* parsing tag-attribs */
#define STATE_TAGATTR_EQ     11 /* "=" inside tag */
#define STATE_TAGATTR_DQUOTE 12 /* double quote after "=" inside tag */
#define STATE_TAGATTR_SQUOTE 13 /* single quote after "=" inside tag */
#define STATE_COMMENT_TAG    14 /* found "<" inside comment (nest comment) */

#define STATE_EXIT_ERROR_EOF 99 /* unexpected eof */

BOOL skip_until_tag(HSCPRC * hp, EXPSTR * tagfound, STRPTR tagstoplist, STRPTR tagnest)
{
    UBYTE state = STATE_TEXT;   /* */
    INFILE *inpf = hp->inpf;    /* input file */
    LONG nesting = 0;           /* tag-nesting */
    LONG nesting_comment = 0;   /* comment-nesting */
    STRPTR nw = NULL;
    BOOL quit = FALSE;          /* flag: exit from skipping */
    EXPSTR *ungetstr = init_estr(32);

    clr_estr(tagfound);

    do
    {
        /* get next word or tag-id */
        if (state != STATE_TAG)
            nw = infgetw(inpf);
        else
        {
            nw = infget_tagid(hp);
            if (nw)
            {
                app_estr(ungetstr, infgetcws(inpf));
                app_estr(ungetstr, infgetcw(inpf));
                if (strcmp(nw, "/"))
                {
                    DS(fprintf(stderr, DHLS "start-tag <%s>\n", nw));
                    state = STATE_TAGNAME;      /* tag detected */
                }
                else
                {
                    nw = infget_tagid(hp);
                    DS(fprintf(stderr, DHLS "end-tag </%s>\n", nw));
                    app_estr(ungetstr, infgetcws(inpf));
                    app_estr(ungetstr, infgetcw(inpf));
                    state = STATE_ENDTAGNAME;   /* end-tag detected */
                }
            }
        }

        if (nw)
        {
            switch (state)
            {

                /* check if tag starts */
            case STATE_TEXT:
                if (!strcmp(nw, "<"))
                {
                    DS(fprintf(stderr, DHLS "tag\n"));
                    set_estr(ungetstr, nw);
                    state = STATE_TAG;
                }
                break;

                /* check which tag it is and how to act */
            case STATE_TAGNAME:
                {
                    /* check, if nesting-tag should be incr. */
                    if (!upstrcmp(nw, tagnest))
                    {
                        DS(fprintf(stderr, DHLS "nest-tag (%ld)\n", nesting));
                        state = STATE_TAGATTR;
                        nesting++;
                    }
                    /* check, if stop-tag reached */
                    else if (!nesting
                             && strenum(nw, tagstoplist, '|', STEN_NOCASE))
                    {
                        DS(fprintf(stderr, DHLS "stop-tag `%s'\n", nw));
                        set_estr(tagfound, nw);
                        quit = TRUE;
                    }
                    /* check, if commant-tag reached */
                    else if (!strcmp(nw, HSC_COMMENT_STR))
                    {
                        DS(fprintf(stderr, DHLS "comment-tag (0)\n"));
                        state = STATE_COMMENT;
                    }
                    /* any tag; just skip attributes */
                    else
                    {
                        DS(fprintf(stderr, DHLS "any tag\n"));
                        state = STATE_TAGATTR;
                    }
                    break;
                }

            case STATE_ENDTAGNAME:
                {
                    if (!upstrcmp(nw, tagnest))
                    {
                        if (nesting)
                        {
                            nesting--;
                            DS(fprintf(stderr, DHLS "nest-tag (%ld)\n", nesting));
                        }
                        else
                        {
                            DS(fprintf(stderr, DHLS "nest-tag ending\n"));
                            quit = TRUE;
                        }
                    }
                    else
                        state = STATE_TEXT;     /* no attr for endtag */

                    break;
                }

                /*
                 * process tag attributes
                 */
            case STATE_TAGATTR:
                {
                    if (!strcmp(nw, "="))
                        state = STATE_TAGATTR_EQ;
                    else if (!strcmp(nw, ">"))
                    {
                        DS(fprintf(stderr, DHLS "back to text\n"));
                        state = STATE_TEXT;
                    }
                    break;
                }

            case STATE_TAGATTR_EQ:
                {
                    if (!strcmp(nw, "\""))
                    {
                        DS(fprintf(stderr, DHLS "tagarg (double quote)\n"));
                        state = STATE_TAGATTR_DQUOTE;
                    }
                    else if (!strcmp(nw, "'"))
                    {
                        DS(fprintf(stderr, DHLS "tagarg (single quote)\n"));
                        state = STATE_TAGATTR_SQUOTE;
                    }
                    else
                        state = STATE_TAGATTR;
                    break;
                }

            case STATE_TAGATTR_DQUOTE:
                {
                    if (!strcmp(nw, "\""))
                    {
                        DS(fprintf(stderr, DHLS "end tagarg (double quote)\n"));
                        state = STATE_TAGATTR;
                    }
                    break;
                }

            case STATE_TAGATTR_SQUOTE:
                {
                    if (!strcmp(nw, "'"))
                    {
                        DS(fprintf(stderr, DHLS "end tagarg (single quote)\n"));
                        state = STATE_TAGATTR;
                    }
                    break;
                }

                /*
                 * comment processing
                 */
            case STATE_COMMENT:
                {
                    /* check for <" */
                    if (!strcmp(nw, "<"))
                        state = STATE_COMMENT_TAG;
                    else if (!strcmp(nw, HSC_COMMENT_STR))
                        state = STATE_COMMENT_STAR;
                    break;
                }

            case STATE_COMMENT_TAG:
                {
                    /* check for comment-nesting */
                    if (!strcmp(nw, HSC_COMMENT_STR))
                    {
                        nesting_comment++;
                        DS(fprintf(stderr, DHLS "comment-tag (%ld)\n", nesting_comment));
                        state = STATE_COMMENT;
                    }
                    else
                        state = STATE_COMMENT;

                    break;
                }

            case STATE_COMMENT_STAR:
                {
                    /* check for end comment */
                    if (!strcmp(nw, ">"))
                        if (nesting_comment)
                        {
                            nesting_comment--;
                            DS(fprintf(stderr, DHLS "end comment-tag (%ld)\n", nesting_comment));
                            state = STATE_COMMENT;
                        }
                        else
                        {
                            DS(fprintf(stderr, DHLS "end comment-tag (%ld)\n", nesting_comment));
                            state = STATE_TEXT;
                        }
                    else
                        state = STATE_COMMENT;
                    break;
                }

                /*
                 * unhandled tag
                 */
            default:
                panic("unhandled state");
                break;
            }
        }
    }
    while (nw && !quit && !(hp->fatal));

    if (nw)
    {
        inungets(estr2str(ungetstr), inpf);
    }
    else
    {
        EXPSTR *tagstr = init_estr(0);

        set_estr(tagstr, "</");
        app_estr(tagstr, tagnest);
        app_estr(tagstr, "> expected");
        hsc_msg_eof(hp, estr2str(tagstr));

        del_estr(tagstr);
    }

    del_estr(ungetstr);

    return ((BOOL) (nw != NULL));
}

