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
 * hsclib/deftag.c
 *
 * define new tag from input file
 *
 * updated: 25-Feb-1997
 * created: 13-Oct-1995
 */

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/eval.h"
#include "hsclib/input.h"
#include "hsclib/skip.h"

/*
 *-------------------------------------
 * define a new tag from input file
 *-------------------------------------
 */

/*
 * def_tag_name
 *
 */
HSCTAG *def_tag_name(HSCPRC * hp, BOOL * start_tag)
{
    STRPTR nw = NULL;
    HSCTAG *tag = NULL;
    DLLIST *taglist = hp->deftag;

    /* get tag name */
    nw = infget_tagid(hp);

    /* create new tag */
    if (nw)
    {
        *start_tag = (BOOL) (strcmp(nw, "/"));
        if (!(*start_tag))
        {
            /* add closing tag */
            nw = infget_tagid(hp);
            if (nw)
            {
                tag = find_strtag(taglist, nw);
                if (tag)
                {
                    if ((tag->option & HT_CLOSE)
                        ||
                        (tag->option & HT_CONTENT))
                    {
                        /* tried to redefine end tag */
                        tag = NULL;
                        hsc_message(hp, MSG_REDEFINE_ENDTAG,
                                    "redefined %c", nw);
                    }
                    else
                    {
                        /* mark macro as a container */
                        tag->option |= HT_CLOSE;
                    }
                }
                else
                {
                    /* tried to define end tag without previous start tag */
                    tag = NULL;
                    hsc_message(hp, MSG_DEFTAG_NO_OPEN,
                                "no start tag for %c", nw);
                }
            }                   /* err_eof already called in infget_tagid() */
        }
        else
        {
            tag = find_strtag(taglist, nw);
            if (tag)
            {
                /* find tag-node in list to delete it
                 * NOTE: this is rather stupid, 'cause the list
                 * has to be searched twice this way; but who cares? */
                DLNODE *nd = find_dlnode(hp->deftag->first,
                                         (APTR) nw, cmp_strtag);

                /* new tag/macro replaces old tag/macro */
                tag->occured = FALSE;

                hsc_message(hp, MSG_REDEFINE_TAG, "redefined %T", tag);

                del_dlnode(hp->deftag, nd);

            }

            /* create a new opening tag */
            tag = app_tag(taglist, nw);
        }
    }                           /* err_eof already called in infget_tagid() */

#if 0                           /* TODO: remove this */
    /* convert tag name to upper  case */
    if (tag)
        upstr(tag->name);
#endif

    return (tag);
}

/*
 * check_tag_option
 *
 * check if a tag-option-string is equal to an id/short id.
 * if so, set the corresponding option value within the tag.
 *
 * params: option..option string to check for (read from input)
 *         tag.....tag to update option value for
 *         id......id string of option (eg "REQUIRED")
 *         sid.....short id string (eg "REQ")
 *         value...option value to OR with old tag's option value
 * result: TRUE, if tag's option value updated
 */
static BOOL check_tag_option(HSCPRC * hp, STRPTR option, HSCTAG * tag,
                             STRPTR id, STRPTR sid, ULONG value)
{
    BOOL found = FALSE;

    if (!((upstrcmp(option, id)) && (upstrcmp(option, sid))))
    {
        DDT(fprintf(stderr, DHL "  option %s\n", id));
        tag->option |= value;
        found = TRUE;
    }

    return (found);

}

/*
 * parse_lazy_option
 *
 * allowed abbrevations:
 *
 * c  CLASS:string
 * h  HREF:uri
 * i  ID:id
 * k  CLEAR:enum("left|right|all|*")
 * l  LANG:string
 * m  MD:string
 * s  SRC:URI
 * w  NOWRAP:bool
 */
static HSCATTR *def_lazy_attr(HSCPRC * hp, HSCTAG * tag,
                              STRPTR attrname, BYTE attrtype)
{
    HSCATTR *newattr = app_var(tag->attr, attrname);
    DDA(fprintf(stderr, DHL "new attr: `%s'\n", attrname));
    newattr->vartype = attrtype;
    return (newattr);
}

static BOOL parse_lazy_option(HSCPRC * hp, HSCTAG * tag, STRPTR lazy)
{
    BOOL ok = TRUE;

    while (lazy[0])
    {
        switch (lazy[0])
        {
        case 'c':
            def_lazy_attr(hp, tag, "CLASS", VT_STRING);
            break;
        case 'h':
            def_lazy_attr(hp, tag, "HREF", VT_URI);
            break;
        case 'i':
            def_lazy_attr(hp, tag, "ID", VT_ID);
            break;
        case 'k':
            {
                HSCATTR *clear_attr =
                def_lazy_attr(hp, tag, "CLEAR", VT_ENUM);
                clear_attr->enumstr = strclone("left|right|all|*");
            }
            break;
        case 'l':
            def_lazy_attr(hp, tag, "LANG", VT_STRING);
            break;
        case 'm':
            def_lazy_attr(hp, tag, "MD", VT_STRING);
            break;
        case 's':
            def_lazy_attr(hp, tag, "SRC", VT_URI);
            break;
        case 'w':
            def_lazy_attr(hp, tag, "NOWRAP", VT_BOOL);
            break;
        default:
            hsc_message(hp, MSG_UNKN_TAG_OPTION,
                        "unknown tag modifier %q (arg %q)",
                        "LAZY", ch2str(lazy[0]));
            break;
        }
        lazy++;
    }

    return (ok);
}

/*
 * parse_tag_option
 *
 * check a tag-modifier, read additional values (if necessary), update
 * tag structure
 *
 * params: hp......hsc process
 *         option..string that contains modifier (eg. "REQUIRED")
 *         tag.....tag to modify
 * result: TRUE, if modifier could be handled
 * errors: return FALSE, output message
 */
static BOOL parse_tag_option(HSCPRC * hp, STRPTR option, HSCTAG * tag)
{
    BOOL ok = FALSE;
    HSCATTR *attr = new_hscattr(PREFIX_TMPATTR "mbi.naw");

    attr->vartype = VT_STRING;

    if (!(upstrcmp(option, TO_MBI_STR)
          && upstrcmp(option, TO_MBI_SHT)))
    {                           /* must be inside */
        if (parse_eq(hp))
        {
            STRPTR strmbi = eval_expression(hp, attr, NULL);

            if (strmbi)
            {
                tag->mbi = strclone(strmbi);
                DDT(fprintf(stderr, DHL "  mbi = `%s'\n", tag->mbi));
                ok = TRUE;
            }
        }
    }
    else if (!(upstrcmp(option, TO_NAW_STR)
               && upstrcmp(option, TO_NAW_SHT)))
    {                           /* not allowed with */
        if (parse_eq(hp))
        {
            STRPTR strnaw = eval_expression(hp, attr, NULL);

            if (strnaw)
            {
                tag->naw = strclone(strnaw);
                DDT(fprintf(stderr, DHL "  mbi = `%s'\n", tag->naw));
                ok = TRUE;
            }
        }
    }
    else if (!(upstrcmp(option, TO_LAZY_STR)
               && upstrcmp(option, TO_LAZY_SHT)))
    {                           /* lazy standard attribs */
        if (parse_eq(hp))
        {
            STRPTR strlazy = eval_expression(hp, attr, NULL);

            if (strlazy)
            {
                DDT(fprintf(stderr, DHL "  lazy= `%s'\n", strlazy));
                ok = parse_lazy_option(hp, tag, strlazy);
            }
        }
    }
    else
    {
        ok |= check_tag_option(hp, option, tag, TO_CLOSE_STR, TO_CLOSE_SHT, HT_CLOSE);

        /* now check for all the other stuff */
        ok |= check_tag_option(hp, option, tag, TO_SPECIAL_STR, TO_SPECIAL_SHT, HT_SPECIAL);
        ok |= check_tag_option(hp, option, tag, TO_JERK_STR, TO_JERK_SHT, HT_JERK);
        ok |= check_tag_option(hp, option, tag, TO_AUTOCLOSE_STR, TO_AUTOCLOSE_SHT, HT_AUTOCLOSE);
        ok |= check_tag_option(hp, option, tag, TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE);
        ok |= check_tag_option(hp, option, tag, TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE);
        ok |= check_tag_option(hp, option, tag, TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED);
        ok |= check_tag_option(hp, option, tag, TO_RECOMMENDED_STR, TO_RECOMMENDED_SHT, HT_RECOMMENDED);
        ok |= check_tag_option(hp, option, tag, TO_SKIPLF_STR, TO_SKIPLF_SHT, HT_SKIPLF);
        ok |= check_tag_option(hp, option, tag, TO_WHTSPC_STR, TO_WHTSPC_SHT, HT_WHTSPC);

        if (!ok)
        {
            hsc_message(hp, MSG_UNKN_TAG_OPTION,
                        "unknown tag modifer %q", option);
        }
    }

    /* remove temp. attribute */
    del_hscattr(attr);

    return (ok);
}

/*
 * parse_tag_var
 */
static BOOL parse_tag_var(HSCPRC * hp, HSCTAG * tag)
{
    BOOL ok = FALSE;
    HSCATTR *var = NULL;

    /* define new attribute */
    var = define_var(hp, tag->attr, VF_CONST | VF_GLOBAL);

    /* set several values of tag structure, if attribute has
     * some special flags set
     */
    if (var)
    {
        /* attribute is uri that tells the size */
        if (var->varflag & VF_GETSIZE)
            tag->uri_size = var;

        /* attribute is uri that tells if the tag should be stripped */
        if (var->varflag & VF_STRIPEXT)
            tag->uri_stripext = var;

        /* set attribute flag to keep quotes */
        if (tag->option & HT_KEEP_QUOTES)
            var->varflag |= VF_KEEP_QUOTES;

        /* set macro attribute flag for macro tags */
        if (tag->option & HT_MACRO)
            var->varflag |= VF_MACRO;

        ok = TRUE;
    }

    return (ok);
}

/*
 * def_tag_args
 *
 */
BOOL def_tag_args(HSCPRC * hp, HSCTAG * tag)
{
    /* TODO: remove the bullshit with "\n"-checking;
     * meanwhile "\n" is treated as white space */

    BOOL ok = FALSE;
    STRPTR nw;
    INFILE *inpf = hp->inpf;

    if (tag)
    {
        ok = TRUE;

        /* read args */
        nw = infgetw(inpf);

        /*
         * set tag options
         */
        while (nw && ((!strcmp(nw, "/")) || !strcmp(nw, "\n")))
        {
            nw = infgetw(inpf);
            if (nw)
            {
                if (strcmp(nw, "\n"))
                    ok &= parse_tag_option(hp, nw, tag);
                nw = infgetw(inpf);
            }
        }

        /* auto-set HT_KEEP_QUOTES */
        if (!strncmp(tag->name, HSC_TAGID, strlen(HSC_TAGID)))
            tag->option |= HT_KEEP_QUOTES;

        /*
         * set tag attributes
         */
        while (nw && ((strcmp(nw, ">") || !strcmp(nw, "\n"))))
        {
            if (strcmp(nw, "\n"))
            {
                inungetcw(inpf);
                ok &= parse_tag_var(hp, tag);
            }

            nw = infgetw(inpf);
        }

        /* skip linefeeds */
        while (nw && !strcmp(nw, "\n"))
            nw = infgetw(inpf);

        /* check for ">" at end */
        if (nw)
        {
            inungetcw(inpf);
            ok = parse_gt(hp);
        }
    }

    return (ok);
}

/*
 * set_tag_arg
 *
 * parse & set one single tag argument
 *
 */
static BOOL set_tag_arg(HSCPRC * hp, DLLIST * varlist, STRPTR varname, BOOL unknown)
{
    HSCATTR *var = find_varname(varlist, varname);
    INFILE *inpf = hp->inpf;
    STRPTR arg = NULL;
    BOOL ok = FALSE;
    BOOL inheritage_failed = FALSE;     /* flag: set, if "?=" failed */
    STRPTR nw;
    HSCATTR skipvar;            /* dummy-attribute to skip unknown */
    EXPSTR *attr_str = init_estr(40);   /* string for attribute name */
    EXPSTR *val_str = init_estr(40);    /* string for "=" and value */

    DAV(fprintf(stderr, DHL "   set attr %s\n", varname));

    /* append attribute name to attr_str */
    if (hp->compact)
    {
        app_estr(attr_str, compactWs(hp, infgetcws(inpf)));
    }
    else
    {
        app_estr(attr_str, infgetcws(inpf));
    }
    app_estr(attr_str, infgetcw(inpf));

    if (!var)
    {
        /* attribute not found: assign to dummy-attribute */
        var = &skipvar;
        var->name = varname;
        var->deftext = NULL;
        var->text = NULL;
        var->enumstr = NULL;
        var->vartype = VT_STRING;
        var->varflag = 0;

        /* message: unknown attribute */
        if (!unknown)
            hsc_msg_unkn_attr(hp, varname);
    }

    /* get argument */
    nw = infgetw(inpf);
    if (nw)
        if (!strcmp(nw, "="))
        {
            /* append "=" to log */
            if (!hp->compact)
            {
                app_estr(val_str, infgetcws(inpf));
            }
            app_estr(val_str, infgetcw(inpf));

            /* parse expression */
            arg = eval_expression(hp, var, NULL);

            /* append value to log */
            if (var->quote != VQ_NO_QUOTE)
                app_estrch(val_str, var->quote);
            if (get_vartext(var))
                app_estr(val_str, get_vartext(var));
            if (var->quote != VQ_NO_QUOTE)
                app_estrch(val_str, var->quote);

            if (arg)
            {
                DAV(fprintf(stderr, DHL "  `%s'\n", arg));
                ok = TRUE;
            }
        }
        else if (!strcmp(nw, "?"))
        {
            /* process "?="-assignment */
            if (!hp->compact)
                app_estr(val_str, infgetcws(inpf));
            if (parse_eq(hp))
            {
                app_estr(val_str, "=");

                arg = eval_cloneattr(hp, var);

                /* append value to log */
                if (var->quote != VQ_NO_QUOTE)
                    app_estrch(val_str, var->quote);
                if (get_vartext(var))
                    app_estr(val_str, get_vartext(var));
                if (var->quote != VQ_NO_QUOTE)
                    app_estrch(val_str, var->quote);

                if (arg)
                {
                    DAV(fprintf(stderr, DHL "  inherited `%s'\n", arg));
                }
                else
                {
                    DAV(fprintf(stderr, DHL "  inheritage failed\n"));
                    inheritage_failed = TRUE;
                }
                ok = TRUE;
            }
        }
        else
        {
            /* handle boolean attribute */
            arg = NULL;
            inungetcwws(inpf);
            if (var == &skipvar)
                var->vartype = VT_BOOL;
            ok = TRUE;
        }
    else
        hsc_msg_eof(hp, "read attribute value");

    if (ok)
        if (arg)
        {
            if (var->vartype == VT_BOOL)
            {
                /* set boolean attribute depending on expression */
                set_varbool(var, get_varbool(var));

                /* if the expression returned FALSE, remove
                 * the boolean  switch from tag-call
                 */
                if (!get_varbool(var))
                    clr_estr(attr_str);
            }
            else if (!inheritage_failed)
                /* append value to attribute string */
                estrcat(attr_str, val_str);
        }
        else if (inheritage_failed)
        {
            /* if attribute to inherit from was empty,
             * remove the attribute from tag-call
             */
            clr_estr(attr_str);
        }
        else
        {
            /* no value has been passed to the attribute */
            if (var->vartype == VT_BOOL)
            {
                /* for boolean attributes, this is legal,
                 * and enables the attribute
                 */
                set_varbool(var, TRUE);
            }
            else if (!unknown)
            {
                /* for non-boolean attributes, display
                 * error message
                 */
                hsc_message(hp, MSG_NOARG_ATTR,
                            "missing value for %A", var);
            }
        }

    /* cleanup pseudo-attr */
    if (var == &skipvar)
        clr_vartext(var);

    /* append & cleanup attribute and value string */
    app_estr(hp->tag_attr_str, estr2str(attr_str));
    del_estr(attr_str);
    del_estr(val_str);

    return (ok);
}

/*
 * set_tag_args
 *
 * parse & set all arguments of a tag
 */
ULONG set_tag_args(HSCPRC * hp, HSCTAG * tag)
{
    INFILE *inpf = hp->inpf;
    BOOL ok = FALSE;
    DLLIST *varlist;
    ULONG result_tci = get_mci(hp);     /* resulting tag_call_id */
    STRPTR nw = infgetw(inpf);

    /* evaluate which varlist to use */
    varlist = tag->attr;

    /* clear string that logs all attributes passed to tag */
    clr_estr(hp->tag_attr_str);

    /* read args */
    do
    {
        if (!nw)
            hsc_msg_eof(hp, "read attributes");
        else
        {
            /*
             * process next attribute
             */
            if (!strcmp(nw, ">"))
            {
                nw = NULL;
                ok = TRUE;
            }
            else
            {
                /* process attribute */
                if (check_attrname(hp, nw))
                {
                    BOOL unknown = tag->option & HT_UNKNOWN;
                    set_tag_arg(hp, varlist, nw, unknown);
                }
                else
                {
                    /* append empty value */
#if 0
                    app_estr(hp->tag_attr_str, "\"\"");
                    skip_until_eot(hp, NULL);
#endif
                    nw = NULL;
                }

                /* read next attribute */
                if (nw)
                    nw = infgetw(inpf);
            }
        }
    }
    while (nw);

    /* unset scope */
    unget_mci(hp);

    /* set all undefined bool. attr to FALSE */
    clr_varlist_bool(varlist);

    /* check for required attributes */
    if (ok)
    {
        ok = check_varlist(hp, varlist);
        if (!ok)
            inungetcw(inpf);
    }

    if (!ok)
        result_tci = MCI_ERROR;

    return (result_tci);
}
