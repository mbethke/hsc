/*
 * hsclib/defattr.c
 *
 * functions to define new attribute
 * and manipulate attribute lists
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
 * updated:  4-Aug-1996
 * created:  6-Jan-1995
 */

#define NOEXTERN_HSCLIB_DEFATTR

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/eval.h"
#include "hsclib/input.h"

/*
 *-------------------------------------
 * misc. functions
 *-------------------------------------
 */
static BYTE str2vartype(STRPTR s)
{
    BYTE vartype = VT_NONE;

    if (!upstrcmp(VT_STR_URI, s))
        vartype = VT_URI;
    else if (!upstrcmp(VT_STR_STRING, s))
        vartype = VT_STRING;
    else if (!upstrcmp(VT_STR_BOOL, s))
        vartype = VT_BOOL;
    else if (!upstrcmp(VT_STR_NUM, s))
        vartype = VT_NUM;
    else if (!upstrcmp(VT_STR_ENUM, s))
        vartype = VT_ENUM;
    else if (!upstrcmp(VT_STR_ID, s))
        vartype = VT_ID;
    else if (!upstrcmp(VT_STR_COLOR, s))
        vartype = VT_COLOR;

    return (vartype);
}

/*
 * check_varlist: check for required attributes missing
 *
 * result: TRUE, if all attributes ok
 */
static BOOL check_reqvar(HSCPRC * hp, HSCATTR * var)
{
    BOOL ok = TRUE;

    if ((var->varflag & VF_REQUIRED)
        && (!var->text))
    {
        hsc_message(hp, MSG_MISS_REQ_ATTR,
                    "required %A missing", var);
        ok = FALSE;
    }

    return (ok);
}

BOOL check_varlist(HSCPRC * hp, DLLIST * varlist)
{
    DLNODE *nd = varlist->first;
    BOOL ok = TRUE;

    while (nd)
    {
        ok &= check_reqvar(hp, (HSCATTR *) (nd->data));
        nd = nd->next;
    }

    return (ok);
}

/*
 *-------------------------------------
 * define a new var from input file
 *-------------------------------------
 */

/*
 * read_enum_str
 *
 * sidefx: modifies tmpstr
 */
static BOOL read_enum_str(HSCPRC * hp, HSCATTR * var)
{
    HSCATTR *attr = new_hscattr(PREFIX_TMPATTR "enumerator");
    attr->vartype = VT_STRING;

    /* store enumstr in var-struct */
    if (eval_expression(hp, attr, NULL))
    {
        DDA(fprintf(stderr, DHL "  enum: %s\n", estr2str(hp->tmpstr)));
        var->enumstr = strclone(get_vartext(attr));
    }

    del_hscattr(attr);

    return ((BOOL) (!hp->fatal));
}

/*
 * check_attr_option
 *
 * check if a attribute-option-string is equal to an id/short id.
 * if so, set the corresponding option value within the attribute.
 *
 * params: option..option string to check for (read from input)
 *         attr....attribute to update option value for
 *         id......id string of option (eg "REQUIRED")
 *         sid.....short id string (eg "R")
 *         value...option value to OR with old tag's option value
 * result: TRUE, if tag's option value updated
 */
static BOOL check_attr_option(HSCPRC * hp, STRPTR option, HSCATTR * attr, STRPTR id, STRPTR sid, ULONG value, ULONG unmasked_flags)
{
    BOOL found = FALSE;

    if (!((upstrcmp(option, id)) && (upstrcmp(option, sid))))
    {

        DDA(fprintf(stderr, DHL "  option %s\n", id));

        if (value & unmasked_flags)
        {

            hsc_message(hp, MSG_ILLG_ATTR_FLAG,
                        "attribute flag %q not allowed in this context",
                        id);

        }
        else
            attr->varflag |= value;

        found = TRUE;

    }

    return (found);

}

/*
 * define_var
 *
 * define a new var with reading its def from input file
 * (starts parsing after ":", so ":" has to be read before)
 *
 * params: varname..name of new var
 *         varlist..list new var should be inserted at the beginning
 *         inpf.....input file where to read def from
 *         flag.....flags: VF_ONLYONCE to avoid re-definition of a var
 * result: ptr to new var
 *
 * definition syntax in input file:
 *   <vartype>[/flag]["="<deftext value>]
 *   legal vartypes: see VT_STR_xx in "vars.h"
 *   legal flags   : see VF_STR_xx in "vars.h"
 */
HSCATTR *define_var(HSCPRC * hp, DLLIST * varlist, ULONG unmasked_flags)
{
    HSCATTR *var = NULL;        /* result */
    BOOL ok = FALSE;
    BYTE val_vartype = VT_NONE; /* var-type (numeric) */
    BOOL newattr = FALSE;       /* next word read from input */
    STRPTR nw = NULL;
    STRPTR varname = NULL;
    BOOL eof_called = FALSE;    /* used at end-of-func, if nw==NULL */
    INFILE *inpf = hp->inpf;

    /* read attribute name */
    nw = infget_attrid(hp);
    if (nw)
    {
        varname = strclone(nw); /* remember attribute name */
    }
    else
        eof_called = TRUE;      /* err_eof() called already */

    /* read attribute type */
    if (nw)
        if (parse_wd(hp, ":"))
        {
            nw = infgetw(inpf);
            if (nw)
                val_vartype = str2vartype(nw);
        }
        else
            inungetcw(inpf);

    if (nw)
    {
        /*
         * look if attr already exist;
         * if yes, clear old attribute
         * to redefine the new one
         */
        var = find_varname(varlist, varname);
        if (var)
        {
            DLNODE *nd = find_attrnode(varlist, varname);

            /* remove old attribute */
            if (nd)
                del_dlnode(varlist, nd);
            else
                panic("no node for redefined attribute");

            hsc_message(hp, MSG_ATTR_REDEFINED,
                        "redefined %a", varname);
        }

        /*
         * create new attribute
         */
        DDA(fprintf(stderr, DHL "new attr: %s\n", varname));
        var = app_var(varlist, varname);

        /* set type */
        var->vartype = val_vartype;
        if (var->vartype == VT_ENUM)
        {
            /* init enum-attribute */
            read_enum_str(hp, var);
        }
        else if (var->vartype == VT_BOOL)
        {
            /* init boolean attr with FALSE */
            set_varbool(var, FALSE);
        }

        newattr = TRUE;

    }

    /* disable "/STRIPEXT" and "/GETSIZE" for non-URI-attributes */
    if (nw)
    {
        if (var->vartype != VT_URI)
            unmasked_flags |= VF_GETSIZE | VF_STRIPEXT;

        nw = infgetw(inpf);     /* get net word */
    }

    /*
     * handle attribute flags
     */
    while (nw && !strcmp(nw, "/"))
    {
        nw = infgetw(inpf);     /* read flag identifier */
        if (nw)
        {
            BOOL ok = FALSE;

            ok |= check_attr_option(hp, nw, var,
                                    VF_CONST_STR, VF_CONST_SHT,
                                    VF_CONST, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_GLOBAL_STR, VF_GLOBAL_SHT,
                                    VF_GLOBAL, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_JERK_STR, VF_JERK_SHT,
                                    VF_JERK, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_ONLYONCE_STR, VF_ONLYONCE_SHT,
                                    VF_ONLYONCE, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_REQUIRED_STR, VF_REQUIRED_SHT,
                                    VF_REQUIRED, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_GETSIZE_STR, VF_GETSIZE_SHT,
                                    VF_GETSIZE, unmasked_flags);
            ok |= check_attr_option(hp, nw, var,
                                    VF_STRIPEXT_STR, VF_STRIPEXT_SHT,
                                    VF_STRIPEXT, unmasked_flags);
            if (!ok)
            {
                hsc_message(hp, MSG_UNKN_ATTR_OPTION,
                            "unknown attribute flag %q", nw);
            }

            /* read next word (should be "/", "=" or next attr / ">") */
            nw = infgetw(inpf);
        }
        else
            hsc_msg_eof(hp, "defining attribute");

    }

    /*
     * handle default value
     */
    if (nw && !strcmp(nw, "="))
    {
        /* get new deftext value */
        STRPTR new_deftext = NULL;
        LONG old_attrflag = var->varflag;

        /* disable quotemode-checking */
        var->varflag |= VF_KEEP_QUOTES;

        if (!(var->deftext))
            new_deftext = eval_expression(hp, var, NULL);
        else
        {
            STRPTR dummy;

            hsc_message(hp, MSG_SYMB_2ND_DEFAULT,
                        "default value for %A already set", var);

            /* skip illegal default value */
            dummy = eval_expression(hp, var, NULL);
        }

        /* restore quotemode-checking */
        var->varflag = old_attrflag;

        /* store default text value */
        if (new_deftext)
            var->deftext = strclone(new_deftext);

        /* read next word, only to be ungotten below */
        nw = infgetw(inpf);
    }

    /* check for unexpected end of file */
    if (!nw)
    {
        if (!eof_called)
            hsc_msg_eof(hp, "defining attribute");
    }
    else
    {
        /* end of var definition reached */
        inungetcw(inpf);
        ok = TRUE;
    }

    /* cleanup */
    if (!ok && var)
    {
        DLNODE *nd = find_attrnode(varlist,varname);
        if (nd)
            del_dlnode(varlist, (APTR) nd);
        else
            del_hscattr(var);
        var = NULL;
    }
    ufreestr(varname);

    return (var);
}

/*
 *-------------------------------------
 * copy & remove local vars to/from
 * global varlist
 *-------------------------------------
 */

/*
 * copy_local_var
 *
 * copies a local attribute to the global attribute list
 *
 * NOTE: the VF_MACRO-flag of the copy is disabled!
 */
static HSCATTR *copy_local_var(DLLIST * destlist, HSCATTR * locvar, ULONG mci)
{
    HSCATTR *var = app_var(destlist, locvar->name);

    var->macro_id = mci;
    var->vartype = locvar->vartype;
    var->varflag = locvar->varflag & (~VF_MACRO);       /* disable VF_MACRO */
    set_vartext(var, locvar->text);
    var->quote = locvar->quote;

    return (var);
}

/*
 * copy_local_vars
 *
 * add all local attributes of a macro to the global
 * attribute list.
 *
 */
BOOL copy_local_varlist(DLLIST * destlist, DLLIST * varlist, ULONG mci)
{
    BOOL ok = TRUE;

    if (mci == MCI_ERROR)
        panic("mci=MCI_ERROR");
    else
    {
        DLNODE *nd = varlist->first;
        HSCATTR *var;

        while (nd && ok)
        {
            var = copy_local_var(destlist, (HSCATTR *) (nd->data), mci);
            ok &= (BOOL) (var != NULL);
            nd = nd->next;
        }
    }

    return (ok);
}

/*
 * set_local_var
 *
 * copies a local attribute to the global attribute list
 *
 * NOTE: the VF_MACRO-flag of the set is enabled!
 */
static HSCATTR *set_local_var(DLLIST * destlist, HSCATTR * locvar, ULONG mci)
{
    HSCATTR *var = find_varname(destlist, locvar->name);

    if (var)
    {
        var->macro_id = mci;
        var->vartype = locvar->vartype;
        set_vartext(var, locvar->text);
    }
    else
        panic("set_local_var to UNKNOWN ATTR");

    return (var);
}

/*
 * set_local_vars
 *
 * add all local attributes of a macro to the global
 * attribute list.
 *
 */
BOOL set_local_varlist(DLLIST * destlist, DLLIST * varlist, ULONG mci)
{
    BOOL ok = TRUE;

    if (mci == MCI_ERROR)
        panic("mci=MCI_ERROR");
    else
    {
        DLNODE *nd = varlist->first;
        HSCATTR *var;

        while (nd && ok)
        {
            var = set_local_var(destlist, (HSCATTR *) (nd->data), mci);
            ok &= (BOOL) (var != NULL);
            nd = nd->next;
        }
    }

    return (ok);
}

/*
 * remove_local_varlist
 */
VOID remove_local_varlist(DLLIST * varlist, ULONG mci)
{
    DLNODE *nd = varlist->first;

    while (nd)
    {
        HSCATTR *var = (HSCATTR *) nd->data;    /* var data of node */
        DLNODE *nd_nxt = nd->next;      /* next node */

        if (var->macro_id == mci)
            del_dlnode(varlist, nd);

        nd = nd_nxt;
    }
}

