/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hsclib/tag_macro.c
 *
 * tag callbacks for "<$MACRO>" and "<macro>"
 *
 * updated: 10-May-1997
 * created:  5-Aug-1995
 */

#include "hsclib/inc_tagcb.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/include.h"
#include "hsclib/parse.h"

#include "ugly/ustrlist.h"

/*
 *-------------------------------------
 * handlers for start/end macro
 *-------------------------------------
 */

/*
 * include_macro
 *
 * process macro attributes and text:
 * - set up local attributes
 * - include macro text
 * - remove local attributes
 */
static BOOL include_macro(HSCPRC * hp, HSCTAG * macro, STRPTR macro_text, STRPTR filename, INFILEPOS * fpos)
{
    BOOL ok = TRUE;
    ULONG mci = get_mci(hp);    /* obtain local scope */

    /* copy local attributes to global list */
    ok = copy_local_varlist(hp->defattr, macro->attr, mci);
    DMC(prt_varlist(hp->defattr, "global attr (after copy_local_vars)"));

    if (ok)
    {                           /* include macro file */
        ok = hsc_base_include_string(hp, filename, macro_text,
                                     IH_PARSE_MACRO, fpos);
    }

    /* cleanup */
    if (mci != MCI_ERROR)
    {
        /* remove local attributes */
        remove_local_varlist(hp->defattr, mci);
    }
    unget_mci(hp);              /* restore scope */

    return (ok);
}

/*
 * handle_macro
 *
 * handle for macro tags:
 * - add local attributes to global attributes,
 * - include macro text
 * - remove local attributes
 *
 * params: open_mac..TRUE, if called by an opening macro
 *         inpf......input file
 */

/* just a debugging function */
static VOID dbg_print_macro(HSCPRC * hp, HSCTAG * macro, BOOL open_mac, STRPTR prefix)
{
    DMC(fprintf(stderr, DHL "--%s ", prefix));
    if (open_mac)
    {
        if ((macro)->option & HT_CLOSE)
        {
            DMC(fprintf(stderr, "start macro <%s>\n", (macro)->name));
        }
        else
        {
            DMC(fprintf(stderr, "simple macro <%s>\n", (macro)->name));
        }
    }
    else
    {
        DMC(fprintf(stderr, "end macro </%s>\n", (macro)->name));
    }
}

static BOOL handle_macro(HSCPRC * hp, HSCTAG * macro, BOOL open_mac)
{
    BOOL ok = TRUE;
    EXPSTR *text = NULL;        /* macro text */
    INFILEPOS *fpos = NULL;     /* file position of macro text */

    /* debugging message */
    dbg_print_macro(hp, macro, open_mac, "BEGIN");

    /* determine relative file position and macro text */
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

    /* include macro file */
    ok = include_macro(hp, macro, estr2str(text),
                       SPECIAL_FILE_ID "macro", fpos);

    /* debugging message */
    dbg_print_macro(hp, macro, open_mac, "END");

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
 * handlers for content macros
 *-------------------------------------
 */

/*
 * handle_content_macro
 *
 * handle for content macros
 * (with /CLOSE set at declaration)
 *
 * - scan macro content until corresponding end macro
 *   tag is found
 * - increase scope
 * - define local HSC.CONTENT
 * - include macro text (not content!)
 * - remove HSC.CONTENT
 *
 */
static BOOL handle_content_macro(HSCPRC * hp, HSCTAG * tag)
{
    EXPSTR *macro_content = init_estr(1024);    /* contains macro contents */
    HSCATTR *macro_content_attr = find_varname(hp->defattr, CONTENT_ATTR);      /* attribute that contains contents, too */
    HSCTAG *end_macro = NULL;
    STRPTR old_content = NULL;  /* to store old value of content attr */

    /* position where content starts */
    INFILEPOS *start_content_fpos = new_infilepos(hp->inpf);

    DMC(fprintf(stderr, DHL "--BEGIN content macro <%s>\n", tag->name));

    if (!macro_content_attr)
    {
        panic("no content attribute")
    }

    /* skip macro content until corresponding end macro is found; store
     * content in macro_content, but without the tag call for the end macro */
    skip_until_tag(hp, macro_content, NULL,
                   NULL, tag->name, SKUT_NO_CONTENT_TAGFOUND);

    /* store current value of content attribute */
    {
        STRPTR old = get_vartext(macro_content_attr);
        if (old)
        {
            old_content = strclone(old);
        }
    }

    /* set content attribute with current macro content */
    set_vartext(macro_content_attr, estr2str(macro_content));

    /* push content to content stack */
    add_strnode(hp->content_stack, estr2str(macro_content));

    /* some debuggin info */
    DMC(fprintf(stderr, DHL "  content=`%s'\n", estr2str(macro_content)));
    DMC(fprintf(stderr, DHL "  text   =`%s'\n", estr2str(tag->op_text)));

    /* push current tag on container stack; this is
     * only necessary for tag modifiers /MCI and
     * /NAW, which would not work otherwise */
    end_macro = append_end_tag(hp, tag);

    /* assign position of start of content to macro-tag */
    end_macro->end_fpos = start_content_fpos;

    /* now include the macro text */
    include_macro(hp, tag, estr2str(tag->op_text),
                  SPECIAL_FILE_ID "content-macro", tag->start_fpos);

    /* pull macro tag from container stack */
    end_macro->end_fpos = NULL;
    remove_end_tag(hp, tag);

    /* restore content attribute to previous value */
    set_vartext(macro_content_attr, old_content);

    /* remove content from stack */
    del_dlnode(hp->content_stack, dll_first(hp->content_stack));

    /* cleanup */
    ufreestr(old_content);
    del_estr(macro_content);
    del_infilepos(start_content_fpos);

    DMC(fprintf(stderr, DHL "--END content macro <%s>\n", tag->name));

    return (FALSE);
}

/*
 * handle_hsc_content
 *
 * handle content inside a content macro
 * (insert text of attribute HSC.CONTENT)
 */
static VOID hsc_msg_no_content(HSCPRC *hp)
{
    hsc_message(hp, MSG_NO_CONTENT,
                "no content within current context");
}

BOOL handle_hsc_content(HSCPRC * hp, HSCTAG * tag)
{
    HSCATTR *content_attr = find_varname(hp->defattr, CONTENT_ATTR);    /* attribute that contains content */
    HSCTAG *macro = find_end_container_macro(hp);

    /* use current fileposition as base for including content */
    INFILEPOS *fpos = new_infilepos(hp->inpf);

    if (!macro)
    {
        DMC(fprintf(stderr, DHL "  no container macro on stack\n"));
        hsc_msg_no_content(hp);
    }
    else if (content_attr)
    {
        /* position where content text started */
        INFILEPOS *start_content_fpos = macro->end_fpos;

        /* first node on content stack contains current content text */
        DLNODE *first_content_text_node = dll_first(hp->content_stack);

        if (first_content_text_node)
        {
            /* pull first entry from content stack */
            STRPTR content = (STRPTR) detach_dlnode(hp->content_stack,
                                                    first_content_text_node);
            STRPTR old_content = strclone(get_vartext(content_attr));
            DLLIST *old_attribs = init_dllist(del_hscattr);
            ULONG scope_id = get_current_mci(hp);

            DMC(fprintf(stderr, DHL "  content=`%s'\n", content));

            /* update content attribute */
            set_vartext(content_attr, content);

#ifndef EXPERIMENTAL_CONTAINER
            /* move local attributes from global list to buffer list */
            copy_local_varlist(old_attribs, hp->defattr, scope_id);
            remove_local_varlist(hp->defattr, scope_id);

            /* switch back to above scope */
            unget_mci(hp);
#endif

            /* now include the macro content */
            hsc_base_include_string(hp, SPECIAL_FILE_ID "macro-content",
                                    content,
                                    IH_NO_STATUS | IH_PARSE_MACRO,
                                    start_content_fpos);
            /* TODO: why IH_PARSE_MACRO? */

            /* push entry pulled above back to content stack */
            add_dlnode(hp->content_stack, content);

#ifndef EXPERIMENTAL_CONTAINER
            /* restore local attribs and scope from before */
            copy_local_varlist(hp->defattr, old_attribs, scope_id);
            get_mci(hp);
#endif
            /* restore content attribute */
            set_vartext(content_attr, old_content);

            /* free resources */
            del_dllist(old_attribs);
            ufreestr(old_content);
        }
        else
        {
            DMC(fprintf(stderr, DHL "  no content\n"));
            hsc_msg_no_content(hp);
        }
    }
    else
    {
        panic("no content attribute");
    }

    /* cleanup */
    del_infilepos(fpos);

    return (FALSE);
}

/*
 *-------------------------------------
 * macro creation functions
 *-------------------------------------
 */

/*
 * read_macro_text
 */
static BOOL read_macro_text(HSCPRC * hp, HSCTAG * macro, BOOL is_start_macro)
{
    BOOL ok = FALSE;
    EXPSTR *macstr = NULL;

    /* skip first LF if any */
    skip_next_lf(hp);

    /* init an EXPSTR for macro text,
     * remember location of macro-def (for messages) */
    if (is_start_macro)
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

    /* read macro text */
    ok = skip_until_tag(hp, macstr, NULL, NULL,
                        HSC_MACRO_STR, SKUT_NO_CONTENT_TAGFOUND);

    /* if last char of macrotext is LF, CR or CR/LF, remove it */
    if (ok)
    {
        STRPTR ms = estr2str(macstr);
        size_t len = strlen(ms);
        size_t oldlen = len;

        /* NOTE: this is a bit ugly, it would be nicer to
         * call leftstr() here and strip last ch, but this
         * a lot faster */
        if (len && (ms[len - 1] == '\n'))
        {
            ms[len - 1] = '\0';
            len = strlen(ms);
        }

        if (len && (ms[len - 1] == '\r'))
        {
            ms[len - 1] = '\0';
            len = strlen(ms);
        }

        if (oldlen != len)
        {
            DMC(fprintf(stderr, DHL "  stripped cr/lf at end\n"));
        }

        DMC(fprintf(stderr, DHL "Macro text: \"%s\"\n", estr2str(macstr)));
    }

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
    BOOL is_start_macro = FALSE;

    /* get name and argumets */
    tag = def_tag_name(hp, &is_start_macro);
    if (tag)
    {
        /* enable macro-flag */
        tag->option |= HT_MACRO;
        DDT(fprintf(stderr, DHL "def macro %s\n", tag->name));
    }

    ok = (tag && def_tag_args(hp, tag));

    if (ok)
    {
        /* assign macro text to tag structure */
        ok = read_macro_text(hp, tag, is_start_macro);
        if (ok)
        {
            /* set tag handles & flags */
            tag->option |= HT_NOCOPY;

            if (is_start_macro)
            {
                if (tag->option & HT_CLOSE)
                {
                    /* if macro is declared using a name not starting with "/",
                     * it must be a content macro; therfor, assign callback
                     * for content macros */
                    tag->o_handle = handle_content_macro;

                    /* disable the HT_CLOSE flag because the end tag is
                     * used to mark the end of the macro.content and is
                     * skipped anyway */
                    tag->option &= ~HT_CLOSE;
                    tag->option |= HT_CONTENT;
                    DMC(fprintf(stderr, DHL "  kind: container macro\n"));
                }
                else
                {
                    /* assign callback which processed macro text for
                     * start macros; this is the same for container macros
                     * and non-containers that have not been declared as
                     * content macros */
                    tag->o_handle = handle_op_macro;
                    DMC(fprintf(stderr, DHL "  kind: simple macro (for now)\n"));
                }
            }
            else
            {
                /* assign callback which processes the macro text
                 * for the end macro */
                tag->c_handle = handle_cl_macro;
                DMC(fprintf(stderr, DHL "  kind: container macro (old style)\n"));
            }
        }
    }

    return (FALSE);
}
