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
 * hsclib/skip.c
 *
 * functions for skipping several things
 */

#define NOEXTERN_HSCLIB_SKIP_H

#include "hsclib/inc_base.h"

#include "hsclib/input.h"
#include "hsclib/skip.h"

/* debug skip */
#if DEBUG
#define DS(x) if(hp->debug) x
#else
#define DS(x)
#endif
#define DHLS "*hsclib* skip: "

/*
 * forward references
 */
BOOL skip_expression(HSCPRC * hp, EXPSTR * content, int endmark);

/* append text to content, if it does not point to NULL;
 * used by several functions in this file */
#define APP_CONTENT(w) if (content) app_estr(content,(w))
#define APP_CONTENT_ESTR(s) if (content) estrcat(content, (s))
#define APP_CONTENT_CH(c) if (content) app_estrch(content,(c))
#define APP_CONTENT_CWWS(inpf)                                       \
{                                                                    \
    /* apppend current white spaces & word to content_buffer */      \
    APP_CONTENT(infgetcws((inpf)));                                  \
    APP_CONTENT(infgetcw((inpf)));                                   \
}
/* append expstring and clear it afterwards */
#define APP_CONTENT_ESTR_CLR(s)                                      \
{                                                                    \
    APP_CONTENT_ESTR((s));                                           \
    clr_estr((s));                                                   \
}

/* a small debugging-function to print a single char
 * in hex, dez and, if useful,  ascii-representation */
#if DEBUG
static VOID dbg_printc(int ch)
{
    fprintf(stderr, "%02x #%03d", ch, ch);
    if (ch > 31)
    {
        fprintf(stderr, " `%c'", (char) ch);
    }
    fprintf(stderr, "\n");
}
#endif

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
    INFILE *inpf = hp->inpf;
    int nc = infgetc(inpf);

    /* handle "\r\n", single "\r" and single "\n" */
    if (nc == '\r')
    {
        nc = infgetc(inpf);
    }
    if (nc != '\n')
    {
        inungetc(nc, inpf);
    }

    return ((BOOL) (nc == EOF));
}

/*
 * skip_until_eot
 *
 * skip until end of tag reached (">" found)
 *
 * params: inpf..input file
 * result: TRUE, if no fatal error
 * errors: return FALSE
 */
BOOL skip_until_eot(HSCPRC * hp, EXPSTR * logstr)
{
    INFILE *inpf = hp->inpf;
    STRPTR nw = NULL;

    do
    {
        nw = infgetw(inpf);
        if (nw && logstr)
        {
            app_estr(logstr, infgetcws(inpf));
            app_estr(logstr, infgetcw(inpf));
        }
    }
    while (nw && strcmp(nw, ">"));

    return ((BOOL) ! (hp->fatal));
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
            else if (!strcmp(nw, ">")) {
               if (*nest)
               {
                  (*nest)--;
                  *state = CMST_TEXT;
               } else
                  *state = CMST_END;
            }
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
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 * result:
 *  TRUE, if no fatal error occured
 */
BOOL skip_hsc_comment(HSCPRC * hp, EXPSTR * content)
{
    INFILE *inpf = hp->inpf;
    int ch = infgetc(inpf);     /* read next char */
    BYTE state = CMST_TEXT;     /* parser state */
    LONG nesting = 0;           /* nesting counter */
    BOOL end = FALSE;           /* flag: end of comment reached? */

    DS(fprintf(stderr, DHLS "skip_hsc_comment\n"));
    while ((ch != EOF) && (!end))
    {
#if 0                           /* optional debugging */
        STRPTR state_str = "UNKN";

        switch (state)
        {
        case CMST_TEXT:
            state_str = "text";
            break;
        case CMST_TAG:
            state_str = "tag ";
            break;
        case CMST_STAR:
            state_str = "star";
            break;
        }

        fprintf(stderr, DHL "  st=%s  nc=%02x #%03d", state_str, ch, ch);
        if (ch > 31)
        {
            fprintf(stderr, " '%c'", (char) ch);
        }
        fprintf(stderr, "\n");
#endif

        /* append current char to content */
        APP_CONTENT_CH(ch);

        /* handle current char */
        switch (state)
        {
        case CMST_TEXT:
            if (ch == '*')
            {
                state = CMST_STAR;
            }
            else if (ch == '<')
            {
                state = CMST_TAG;
            }
            break;

        case CMST_STAR:
            if (ch == '*')
            {
                state = CMST_STAR;
            }
            else if (ch == '<')
            {
                state = CMST_TAG;
            }
            else if (ch == '>')
            {
                if (nesting)
                {
                    DS(fprintf(stderr,
                             DHLS "  nested comment-end (%ld)\n", nesting));
                    nesting--;
                    state = CMST_TEXT;
                }
                else
                {
                    end = TRUE;
                }
            }
            else
            {
              state = CMST_TEXT;
            }

            break;

        case CMST_TAG:
            if (ch == '<')
                state = CMST_TAG;
            else
            {
                if (ch == '*')
                {
                    nesting++;
                    DS(fprintf(stderr,
                             DHLS "  nested comment-tag (%ld)\n", nesting));
                }
                state = CMST_TEXT;
            }
            break;
        }

        /* read next char */
        if (!end)
        {
            ch = infgetc(inpf);
        }
    }

    /* handle unexpected end-of-file */
    if (ch == EOF)
    {
        hsc_msg_eof(hp, "missing end of comment (\"*>\")");
    }

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_hsc_verbatim
 *
 * skip text until '|>' occures; nesting is not supported
 *
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 * result:
 *  TRUE, if no fatal error occured
 */
BOOL skip_hsc_verbatim(HSCPRC * hp, EXPSTR * content)
{
#define VBST_TEXT 1
#define VBST_VBAR 2
#define VBST_END  3

    INFILE *inpf = hp->inpf;
    int ch = infgetc(inpf);     /* read next char */
    BYTE state = VBST_TEXT;     /* parser state */

    while ((ch != EOF) && (state != VBST_END))
    {
        /* append current char to content */
        APP_CONTENT_CH(ch);

        /* handle current char */
        switch (state)
        {
        case VBST_TEXT:
            if (ch == (HSC_VERBATIM_STR[0]))
            {
                state = VBST_VBAR;
            }
            break;
        case VBST_VBAR:
            if (ch == '>')
            {
                state = VBST_END;
            }
            else if (ch == (HSC_VERBATIM_STR[0]))
            {
                state = VBST_VBAR;
            }
            else
            {
                state = VBST_TEXT;
            }
            break;
        default:
            panic("unhandled state");
            state = VBST_END;
            break;
        }

        /* read next char */
        if (state != VBST_END)
        {
            ch = infgetc(inpf);
        }
    }

    /* handle unexpected end-of-file */
    if (ch == EOF)
    {
        hsc_msg_eof(hp, "missing end verbatim section \"<|..|>\"");
    }

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_sgml_special
 *
 * skip SGML special commands (beginning with "<!":
 * - checks, if first two chars are "--"; if so,
 *   it will treat input as SGML comment
 *
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 *  stripped     will be set to TRUE if comment was not stored
 * result:
 *  TRUE, if no fatal error occured
 */

/* display message "linefeed inside sgml-comment" */
static VOID msg_lf_in_comment(HSCPRC * hp)
{
    hsc_message(hp, MSG_LF_IN_COMMENT,
                "line feed inside sgml-comment");
}

BOOL skip_sgml_special(HSCPRC * hp, EXPSTR * content, BOOL *stripped)
{
    INFILE *inpf = hp->inpf;
    int ch = infgetc(inpf);     /* read next char */
    int ch_prev = EOF;
    BOOL end = FALSE;           /* flag: end of comment reached? */

    if (ch == '>') {
        hsc_message(hp, MSG_ZERO_COMMENT, "empty sgml comment");
        end = TRUE;
    } else if (ch == '-') {
        ch_prev = ch;
        ch = infgetc(inpf);
        if (ch == '-') {
            BOOL inside_comment = TRUE;
            BOOL warned_text = FALSE;
            
            /* don't keep contents if STRIPCOMMENT was set (but do keep stuff
             * starting in "<!" but not "<!--") */
            if(hp->strip_cmt) {
               HSCTAG dummytag;
               dummytag.name = "!-- ... --";
               content = NULL;
               *stripped = TRUE;
               hsc_msg_stripped_tag(hp, &dummytag, "sgml-comment");
            }

            DS(fprintf(stderr, DHLS "skip sgml comment\n"));
            APP_CONTENT_CH(ch_prev);
            APP_CONTENT_CH(ch);

            ch_prev = EOF;
            ch = infgetc(inpf);

            while (!end && (ch != EOF)) {
                /* append current char to content */
                APP_CONTENT_CH(ch);

                if ((ch == '-') && (ch_prev == '-')) {
                    inside_comment = !inside_comment;
                    warned_text = FALSE;
                    ch_prev = EOF;
                } else if (ch == '-') {
                    ch_prev = '-';
                } else if (ch == '\r') {
                    ch_prev = '\r';
                    msg_lf_in_comment(hp);
                } else {
                    if (ch == '\n') {
                        if (ch_prev != '\r')
                            msg_lf_in_comment(hp);
                    }

                    ch_prev = EOF;
                    if (ch == '>') {
                        if (inside_comment)
                            hsc_message(hp, MSG_GT_IN_COMMENT,
                                        "%q inside sgml-comment", ">");
                        else
                            end = TRUE;
                    } else {
                        if (!inside_comment && !warned_text)
                            hsc_message(hp, MSG_TEXT_IN_COMMENT,
                                        "text outside sgml-comment context");
                        warned_text = TRUE;
                    }
                }

                if (!end) {
                    /* read next char */
                    ch = infgetc(inpf);
                }
            }

            /* push back last char */
            if (!end && (ch != EOF))
                inungetc(ch, inpf);
        } else {
            /* push back chars read until yet */
            inungetc(ch, inpf);
            inungetc(ch_prev, inpf);

            ch_prev = EOF;
        }
    }

    /* skip other "!"-tags (SSI and that bullshit) */
    if (!end) {
        DS(fprintf(stderr, DHLS "skip sgml special\n"));

        APP_CONTENT_CH(ch);
        do {
            ch = infgetc(inpf);
            if (ch != EOF) {
                APP_CONTENT_CH(ch);
                DS(
                      {
                      fprintf(stderr, DHLS "  word starting with: ");
                      dbg_printc(ch);
                      fprintf(stderr, "\n");
                      }
                );

                if (ch == '>')
                    end = TRUE;
                else
                    skip_expression(hp, content, ch);
            }
        } while ((ch != EOF) && !end);
    }

    /* handle unexpected end-of-file */
    if (ch == EOF)
        hsc_msg_eof(hp, "missing end of sgml special tag \"<!..>\"");

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_expression
 *
 * skips expressions, string constants, functions calls
 *
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 *  endmark      char that marks end-of-expression (see note below)
 * result:
 *  TRUE, if no fatal error occured
 *
 * NOTE on endmark:
 *  " .............. skip until next "
 *  ' .............. skip until next '
 *  ` .............. skip until next `
 *  ( .............. skip until next ), or recursively skip subexpression,
 *                   if one of the above shows up before
 *  anything else .. skip until white-space or ">"
 *
 * NOTE:
 *  internally, endmark='(' is immediately converted to endmark=')'
 */

/* decides, if a char is a supported endmark character */
static BOOL is_endmark(int ch)
{
    BOOL it_is = FALSE;
    if (strchr("(`'\"", ch))
    {
        it_is = TRUE;
    }
    return (it_is);
}

/* some debug macros */
#if 1
#define DBG_CH(ch)                               \
    DS(                                          \
          {                                      \
          fprintf(stderr, DHLS "  ch=");         \
          dbg_printc((ch));                      \
          }                                      \
    );
#else
#define DBG_NC                  /* nufin */
#endif

BOOL skip_expression(HSCPRC * hp, EXPSTR * content, int endmark)
{
#define IS_ENDMARK(x) ((BOOL)
    BOOL quit = FALSE;
    BOOL usual_endmark = is_endmark(endmark);
    int ch = EOF;

    DS(fprintf(stderr, DHLS "  skip expression, end=`%c'\n", endmark));

    if (endmark == '(')
    {
        endmark = ')';
    }

    do
    {
        ch = infgetc(hp->inpf);

        if (!usual_endmark)
        {
            /* skip until white-space or ">" */
            if (hsc_whtspc(ch) || (ch == '>'))
            {
                inungetc(ch, hp->inpf);
                quit = TRUE;
            }
            else
            {
                APP_CONTENT_CH(ch);
                DBG_CH(ch);
            }
        }
        else if (endmark == ')')
        {
            APP_CONTENT_CH(ch);
            if (is_endmark(ch))
            {
                DS(fprintf(stderr, DHLS "  skip sub-expression\n"));
                skip_expression(hp, content, ch);
            }
            else if (ch == endmark)
            {
                quit = TRUE;
            }
            else
            {
                DBG_CH(ch);
            }
        }
        else
        {
            APP_CONTENT_CH(ch);
            if (ch == endmark)
            {
                quit = TRUE;
            }
            else
            {
                /* do nufin, just append normal char to expression */
                DBG_CH(ch);
            }
        }
    }
    while (!quit && !(ch == EOF) && !(hp->fatal));

    /* handle unexpected end-of-file */
    if (ch == EOF)
    {
        EXPSTR *expected = init_estr(0);
        if (usual_endmark)
        {
            app_estrch(expected, '`');
            app_estrch(expected, endmark);
            app_estrch(expected, '\'');
        }
        else
        {
            app_estr(expected, "white space or `>'");
        }
        app_estr(expected, " expected");
        hsc_msg_eof(hp, estr2str(expected));
        del_estr(expected);
    }

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_tag_attribs
 *
 * skip tag attributes, until ">" shows up
 *
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 * result:
 *  TRUE, if no fatal error occured
 */
#define STATE_TAGATTR        10 /* parsing tag-attribs */
#define STATE_TAGATTR_EQ     11 /* "=" inside tag */

BOOL skip_tag_attribs(HSCPRC * hp, EXPSTR * content)
{
    /* TODO: on illegal attr name, abort wth message */
    /* TODO: on "\n" in value, display message */
    /* TODO: conditional assignments */
    UBYTE state = STATE_TAGATTR;
    INFILE *inpf = hp->inpf;    /* input file */
    STRPTR nw = NULL;
    BOOL quit = FALSE;          /* flag: exit from skipping */

    do
    {
        /* get next word  or attribute name */
        if (state == STATE_TAGATTR)
        {
            nw = infget_attrid(hp);
        }
        else
        {
            nw = infgetw(inpf);
        }

        if (nw)
        {
            app_estr(content, infgetcws(inpf));
            app_estr(content, infgetcw(inpf));

            switch (state)
            {
            case STATE_TAGATTR:
                {
                    if (!strcmp(nw, "="))
                    {
                        /* normal assignment */
                        state = STATE_TAGATTR_EQ;
                    }
                    else if (!strcmp(nw, "?"))
                    {
                        /* conditional assignment:
                         * just check for succeeding "=",
                         * and push it back into input stream */
                        parse_eq(hp);
                        inungetcw(inpf);
                    }
                    else if (!strcmp(nw, ">"))
                    {
                        DS(fprintf(stderr, DHLS "end-of-tag-call\n"));
                        quit = TRUE;
                    }
                    else
                    {
                        /* just skipped a boolean attribute;
                         * nufin to do about it */
                    }
                    break;
                }

            case STATE_TAGATTR_EQ:
                skip_expression(hp, content, nw[0]);
                DS(fprintf(stderr, DHLS "end-of-tag-expression\n"));
                state = STATE_TAGATTR;
                break;

                /* unhandled state */
            default:
                panic("unhandled state");
                break;
            }
        }
    }
    while (nw && !quit && !(hp->fatal));

    return ((BOOL) ! (hp->fatal));
}

/*
 * skip_until_tag
 *
 * skip everything, until a specific tag (one of tagstoplist or tagnest)
 * is found. if tagnest occures as a start tag, another instance of the
 * corresponding end tag has to occure to abort the skipping operation.
 *
 * params:
 *  hp           hsc-process to work with
 *  content      string where to store skipped text; NULL, if text should
 *               not be stored (which is faster)
 *  tagfound     destination string that will store name of tag that lead
 *               to abortion of skip (eg "$else"); if this string is NULL,
 *               it will be ignored
 *  tagstoplist  list of tags to stop on, sparated with vertical bars `|'
 *               eg. "$else|$elseif"
 *  tagnest      single tag, that maintains a nesting-counter, depending
 *               on wheter it occures as a start-tag or not; if the
 *               nesting-counter is 0 and it occures as a stop-tag, it
 *               will also stop skipping (eg "$if")
 *  option       options for skipping (see hsclib/skip.h):
 *                 SKUT_NO_SKIP_TAGFOUND
 *                   do not skip last tagnest; when reading from the
 *                   input next, it will show up again
 *                 SKUT_NO_CONTENT_TAGFOUND
 *                   do not append last tagnest to content
 *                 SKUT_CLEAR_CONTENT
 *                   clear content before appending anything
 *                 SKUT_NO_ANALYSE_TAGS
 *                   do not analyse tag attributes and special tags;
 *                   this is more or less only used by <$source>
 */
#define STATE_TEXT            1 /* normal text */
#define STATE_TAG             2 /* after "<" */
#define STATE_COMMENT         3 /* inside hsc-comment */
#define STATE_COMMENT_STAR    4 /* inside hsc-comment, after "*" */
#define STATE_TAG_STOP        5 /* found tag in stoplist */
#define STATE_SKIP            6 /* inside `skip section' "<|..|>" */
#define STATE_VBAR            7 /* inside `skip section', after "|" */
#define STATE_ENDTAG          8 /* after end-tagname */

#define STATE_COMMENT_TAG    14 /* found "<" inside comment (nest comment) */

#define STATE_EXIT_ERROR_EOF 99 /* unexpected eof */

BOOL skip_until_tag(HSCPRC * hp, EXPSTR * content, EXPSTR * tagfound, STRPTR tagstoplist, STRPTR tagnest, ULONG option)
{
#define RESET_TAGSTR(init)
    UBYTE state = STATE_TEXT;   /* */
    INFILE *inpf = hp->inpf;    /* input file */
    LONG nesting = 0;           /* tag-nesting */
    STRPTR nw = NULL;
    BOOL quit = FALSE;          /* flag: exit from skipping */
    EXPSTR *tagstr = init_estr(128);    /* text of current tag */

    /* clear result-var tagfound, if passed */
    if (tagfound)
        clr_estr(tagfound);

    /* clear content on request */
    if (content && (option & SKUT_CLEAR_CONTENT))
        clr_estr(content);

    do {
        /* get next word or tag-id */
        if ((state != STATE_TAG) && (state != STATE_ENDTAG))
            nw = infgetw(inpf);
        else
            nw = infget_tagid(hp);

        if (nw) {
#if 0
            /* optional debugging stuff:
             * display content and tagstr on every change */
            if (content && estrlen(content))
                DS(fprintf(stderr, DHLS "  contnt=`%s'\n", estr2str(content)));

            if (estrlen(tagstr))
                DS(fprintf(stderr, DHLS "  tagstr=`%s'\n", estr2str(tagstr)));
#endif

            switch (state) {
                /* check if tag starts */
            case STATE_TEXT:
                if (!strcmp(nw, "<")) {
                    /* add white spaces to content */
                    APP_CONTENT(infgetcws(inpf));
                    /* reset tag string with "<" */
                    set_estr(tagstr, nw);

                    state = STATE_TAG;
                } else {
                    APP_CONTENT_CWWS(inpf);
                    if (estrlen(tagstr))
                        clr_estr(tagstr);
                }
                break;

                /* check which tag it is and how to act */
            case STATE_TAG:
                if (!strcmp(nw, "<")) {
                   /* this handles constructs like ``<</$source>''
                    * correctly */
                   APP_CONTENT_ESTR(tagstr);

                   /* unget current "<" */
                   inungetcwws(inpf);

                   /* switch back to text-parsing */
                   state = STATE_TEXT;
                } else {
                   /* flag: attribs of tag should be skipped
                    *   (and appended to tagstr) */
                   BOOL skip_attribs = FALSE;
                   /* flag: tagstr should be appended to content */
                   BOOL append_tag = FALSE;

                   app_estr(tagstr, infgetcws(inpf));
                   app_estr(tagstr, infgetcw(inpf));

                   /* check for end tag */
                   if (!strcmp(nw, "/"))
                      state = STATE_ENDTAG;
                   else if (option & SKUT_NO_ANALYSE_TAGS)
                      /* abort tag scan */
                      append_tag = TRUE;
                   /* check, if hsc-comment reached */
                   else if (!upstrcmp(nw, HSC_COMMENT_STR)) {
                      DS(fprintf(stderr, DHLS "hsc-comment\n"));
                      APP_CONTENT(estr2str(tagstr));
                      skip_hsc_comment(hp, content);
                      state = STATE_TEXT;
                   } else if (!upstrcmp(nw, HSC_VERBATIM_STR)) {
                      /* check, if hsc-verbatim reached */
                      DS(fprintf(stderr, DHLS "hsc-verbatim\n"));
                      APP_CONTENT(estr2str(tagstr));
                      skip_hsc_verbatim(hp, content);
                      state = STATE_TEXT;
                   } else if (!upstrcmp(nw, HSC_SOURCE_STR)) {
                      /* check, if hsc-source reached */
                      DS(fprintf(stderr, DHLS "hsc-source\n"));
                      APP_CONTENT(estr2str(tagstr));
                      skip_tag_attribs(hp, content);
                      skip_until_tag(hp, content, NULL, NULL,
                            HSC_SOURCE_STR,
                            SKUT_NO_ANALYSE_TAGS);
                      state = STATE_TEXT;
                   } else if (!strcmp(nw, "!")) {
                      /* check, if sgml-special-tag reached */
                      BOOL dummy_stripped;
                      DS(fprintf(stderr, DHLS "sgml-special\n"));
                      APP_CONTENT(estr2str(tagstr));
                      skip_sgml_special(hp, content,&dummy_stripped);
                      state = STATE_TEXT;
                   } else {
                      HSCTAG *tag = find_strtag(hp->deftag, nw);
                      DS(fprintf(stderr, DHLS "tag <%s>\n", nw));

                      /* check, if nesting-tag should be incr. */
                      if (!upstrcmp(nw, tagnest)) {
                         DS(fprintf(stderr, DHLS "  nest-tag (%ld)\n", nesting));
                         nesting++;
                         skip_attribs = TRUE;
                         append_tag = TRUE;
                      } else if (!nesting && tagstoplist
                            && strenum(nw, tagstoplist, '|', STEN_NOCASE)) {
                         /* check, if stop-tag reached */
                         DS(fprintf(stderr, DHLS "  stop-tag <%s>\n", nw));
                         if (tagfound)
                            set_estr(tagfound, nw);
                         skip_attribs = TRUE;
                         quit = TRUE;
                      } else if (tag && (tag->option & HT_SPECIAL)) {
                         /* ignore special tags, switch back to text;
                          * TODO: this is ugly, eg a
                          *     <$define sepp:string="<bla">
                          * will cause trouble */
                         DS(fprintf(stderr, DHLS "  special tag; ignore\n"));
                         state = STATE_TEXT;
                         append_tag = TRUE;
                      } else {
                         /* for standard tags, just skip attributes */
                         append_tag = TRUE;
                         skip_attribs = TRUE;
                      }

                      /* skip tag attributes, if requested */
                      if (skip_attribs) {
                         skip_tag_attribs(hp, tagstr);
                         state = STATE_TEXT;
                      }
                   }

                   /* append tag text to content, if requested */
                   if (append_tag) {
                      DS(fprintf(stderr, DHLS "  append `%s'\n",
                               estr2str(tagstr)));
                      APP_CONTENT(estr2str(tagstr));
                      state = STATE_TEXT;
                   }
                }
                break;

           case STATE_ENDTAG:
                DS(fprintf(stderr, DHLS "end tag </%s>\n", nw));
                app_estr(tagstr, infgetcws(inpf));
                app_estr(tagstr, infgetcw(inpf));
                if (!upstrcmp(nw, tagnest)) {
                   if (nesting) {
                      nesting--;
                      DS(fprintf(stderr, DHLS "  nest-tag (%ld)\n", nesting));
                   } else {
                      DS(fprintf(stderr, DHLS "  nest-tag: ending\n"));
                      quit = TRUE;
                   }
                }

                /* skip and check ">" */
                if (quit || !(option & SKUT_NO_ANALYSE_TAGS)) {
                   nw = infgetw(inpf);
                   if (nw) {
                      app_estr(tagstr, infgetcws(inpf));
                      app_estr(tagstr, infgetcw(inpf));
                      inungetcw(inpf);
                   }
                   parse_gt(hp);
                }

                /* append end tag text */
                if (!quit)
                   APP_CONTENT(estr2str(tagstr));

                /* no attr for endtag */
                state = STATE_TEXT;

                break;
                /* unhandled state */
            default:
                panic("unhandled state in skip_until_tag()");
                break;
            }
        }
    } while (nw && !quit && !(hp->fatal));

    if (nw) {
        /* unget end tag */
        if (option & SKUT_NO_SKIP_TAGFOUND)
            inungets(estr2str(tagstr), inpf);

        /* add last tag to content */
        if (!(option & SKUT_NO_CONTENT_TAGFOUND))
            APP_CONTENT(estr2str(tagstr));
    } else {
        /* unexpected end-of-file */
        set_estr(tagstr, "</");
        app_estr(tagstr, tagnest);
        app_estr(tagstr, "> expected");
        hsc_msg_eof(hp, estr2str(tagstr));
    }

    /* cleanup */
    del_estr(tagstr);

    return ((BOOL) (nw != NULL));
}

/* $Id$ */
/* vi: set ts=4: */
