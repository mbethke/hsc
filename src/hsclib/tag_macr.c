/*
 * hsclib/tag_macr.c
 *
 * tag callbacks for "<$MACRO>" and "<macro>"
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
 * updated: 30-Oct-1996
 * created:  5-Aug-1995
 */

#include "hsclib/inc_tagcb.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/include.h"
#include "hsclib/parse.h"

/* TODO: skip hsc comments */

/* states for read_macro_text */
#define MAST_TEXT   0           /* inside text      */
#define MAST_LT     1           /* after "<"        */
#define MAST_SLASH  2           /* after "</"       */
#define MAST_CMACRO 3           /* after "</$MACRO" */
#define MAST_TAG    8           /* inside a tag     */
#define MAST_COMT   9           /* inside comment   */
#define MAST_ERR   99           /* error occured    */

/*
 *-------------------------------------
 * handlers for opening/closing macro
 *-------------------------------------
 */

/*
 * handle_macro
 *
 * handle for macro: add local attributes to global attributes,
 * include macro text, remove local attributes
 *
 * params: open_mac..TRUE, if called by an opening macro
 *         inpf......input file
 */
static BOOL handle_macro(HSCPRC * hp, HSCTAG * macro, BOOL open_mac)
{
    BOOL ok = FALSE;
    EXPSTR *text;               /* macro text */
    EXPSTR *fname;              /* pseudo-filename */
    DLLIST *args;
    ULONG mci = get_mci(hp);
    INFILEPOS *fpos = NULL;

    /* determine filename & args */
    args = macro->attr;
    if (open_mac)
    {
        text = macro->op_text;
        fpos = macro->start_fpos;
    }
    else
    {
        text = macro->cl_text;
        fpos = macro->end_fpos;
    }
    fname = init_estr(0);

    if (fname)
    {
        /* debugging message */
        DMC(
               {
               fprintf(stderr, DHL "--MACRO <");
               if (!open_mac)
               fprintf(stderr, "/");
               fprintf(stderr, "%s> from %p\n", macro->name, text);
               }
        );

        /* create pseudo-filename */
        ok = set_estr(fname, SPECIAL_FILE_ID "macro ");
        if (!open_mac)
            app_estrch(fname, '/');
        ok &= app_estr(fname, macro->name);
    }

    if (ok)
    {
        /* copy local vars to global list */
        ok = copy_local_varlist(hp->defattr, args, mci);
        DMC(prt_varlist(hp->defattr, "global attr (after copy_local_vars)"));
    }

    if (ok)
    {
        /* include macro file */
        ok = hsc_base_include_string(hp, estr2str(fname), estr2str(text),
                                     IH_PARSE_MACRO, fpos);
    }

    /* cleanup */
    if (mci != MCI_ERROR)
        remove_local_varlist(hp->defattr, mci);         /* remove local vars */
    del_estr(fname);            /* release pseudo-filename */
    unget_mci(hp);  /* restore scope */

    /* debugging message */
    DMC(fprintf(stderr, DHL "--ENDMACRO <%s>\n", macro->name));

    return (FALSE);
}

/*
 * handle_op_macro
 *
 * handle for opening macro
 */
static BOOL handle_op_macro(HSCPRC * hp, HSCTAG * tag)
{
    return (handle_macro(hp, tag, TRUE));
}

/*
 * handle_cl_macro
 *
 * handle for closing macro
 */
static BOOL handle_cl_macro(HSCPRC * hp, HSCTAG * tag)
{
    return (handle_macro(hp, tag, FALSE));
}

/*
 *-------------------------------------
 * macro creation functions
 *-------------------------------------
 */

/*
 * read_macro_text
 *
 */
static BOOL read_macro_text(HSCPRC * hp, HSCTAG * macro, BOOL open_mac)
{
    INFILE *inpf = hp->inpf;
    BOOL ok = FALSE;
    EXPSTR *macstr = NULL;
    STRPTR nw = NULL;           /* word read from input */
    BYTE state = MAST_TEXT;     /* current state */
    LONG nesting = 0;

    /* skip first LF if any */
    skip_next_lf(hp);

    /* init an EXPSTR for macro text,
     * remember location of macro-def (for messages)
     */
    if (open_mac)
    {
        macro->op_text = init_estr(ES_STEP_MACRO);
        macstr = macro->op_text;
        macro->start_fpos = new_winfilepos(hp->inpf);
    }
    else
    {
        macro->cl_text = init_estr(ES_STEP_MACRO);
        macstr = macro->cl_text;
        macro->end_fpos = new_winfilepos(hp->inpf);
    }

    /* reset rmt_str */
    clr_estr(hp->rmt_str);      /* TODO: probably unneccessary; remove this */

    while (!((hp->fatal) || (state == MAST_CMACRO)))
    {
        /* read next word */
        if (state == MAST_SLASH)
            nw = infget_tagid(hp);
        else if (state != MAST_TAG)
            nw = infgetw(inpf);

        if (nw)
        {
            if (state == MAST_TAG)
            {
                /*
                 * skip inside tags
                 */
                BYTE tag_state = TGST_TAG;      /* state var passe to */
                /*     eot_reached() */

                do
                {
                    if (eot_reached(hp, &tag_state))
                        state = MAST_TEXT;

                    app_estr(macstr, infgetcws(inpf));
                    app_estr(macstr, infgetcw(inpf));
                }
                while ((tag_state != TGST_END) && !(hp->fatal));
            }
            else
            {
                switch (state)
                {
                case MAST_TEXT:
                    if (!strcmp(nw, "<"))
                        state = MAST_LT;
                    break;

                case MAST_LT:
                    if (!strcmp(nw, "/"))
                        state = MAST_SLASH;
                    else if (!upstrcmp(nw, HSC_COMMENT_STR))
                    {
                        state = MAST_COMT;
                    }
                    else
                    {
                        /* handle "<$MACRO" (open macro) */
                        if (!upstrcmp(nw, HSC_MACRO_STR))
                            nesting++;  /* incr. macro nesting */
                        state = MAST_TAG;
                    }
                    break;

                case MAST_SLASH:
                    if (!upstrcmp(nw, HSC_MACRO_STR))

                        /* handle "</$MACRO" (close macro) */
                        if (nesting)
                            nesting--;  /* decr. macro nesting */
                        else
                            state = MAST_CMACRO;        /* end of macro */
                    else
                        state = MAST_TAG;
                    break;
                }

                if (state == MAST_TEXT)
                {
                    /* append current white spaces & word to macstr */
                    app_estr(macstr, infgetcws(inpf));
                    app_estr(macstr, infgetcw(inpf));
                }
                else if ((state == MAST_COMT)
                         || (state == MAST_TAG))
                {
                    /* append rmt_str to macstr, clear rmt_str,
                     * append current word to macstr
                     */
                    app_estr(macstr, estr2str(hp->rmt_str));
                    set_estr(hp->rmt_str, "");
                    app_estr(macstr, infgetcws(inpf));
                    app_estr(macstr, infgetcw(inpf));
                }
                else
                {
                    /* append current white spaces & word to macstr */
                    app_estr(hp->rmt_str, infgetcws(inpf));
                    app_estr(hp->rmt_str, infgetcw(inpf));
                }

                /*
                 * skip hsc comment
                 */
                if (state == MAST_COMT)
                {
                    BYTE cstate = CMST_TEXT;    /* vars for eoc_reached() */
                    LONG cnest = 0;
                    BOOL end = FALSE;   /* end of comment reached? */

                    while (!end && !(hp->fatal))
                    {
                        end = eoc_reached(hp, &cstate, &cnest);
                        app_estr(macstr, infgetcws(inpf));
                        app_estr(macstr, infgetcw(inpf));
                    }

                    state = MAST_TEXT;  /* reset state after comment */
                }
            }
        }
        else
        {
            hsc_msg_eof(hp, "missing </" HSC_MACRO_STR ">");
            state = MAST_ERR;
        }
    }                           /* while */

    /* check for legal end state */
    if (state == MAST_CMACRO)
    {
        ok = parse_wd(hp, ">");
    }

    /* if last char of macrotext is LF,
     * replace it with a white space
     */
    if (ok)
    {
        STRPTR ms = estr2str(macstr);
        size_t len = strlen(ms);

        /* TODO: würg! better call leftstr() here and strip last ch */
        if (len && (ms[len - 1] == '\n'))
            ms[len - 1] = '\0';

        DMC(fprintf(stderr, DHL "Macro text: \"%s\"\n", estr2str(macstr)));
    }

    /* clear rmt_str */
    clr_estr(hp->rmt_str);

    return (ok);
}

/*
 *-------------------------------------
 * main handler function for creation
 *-------------------------------------
 */

/*
 * handle_hsc_macro
 *
 * define a new macro tag
 */
BOOL handle_hsc_macro(HSCPRC * hp, HSCTAG * tag)
{
    BOOL ok = FALSE;
    BOOL open_mac = FALSE;

    /* get name and argumets */
    tag = def_tag_name(hp, &open_mac);
    if (tag)
    {
        /* enable macro-flag */
        tag->option |= HT_MACRO;
        DDT(fprintf(stderr, DHL "def macro %s\n", tag->name));
    }

    ok = (tag && def_tag_args(hp, tag));

    if (ok)
    {
        /* copy macro text to temp. file */
        ok = read_macro_text(hp, tag, open_mac);
        if (ok)
        {
            /* set tag handles & flags */
            tag->option |= HT_NOCOPY;
            tag->o_handle = handle_op_macro;
            tag->c_handle = handle_cl_macro;
        }
    }

    return (FALSE);
}

