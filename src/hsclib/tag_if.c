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
 * hsclib/tag_if.c
 *
 * tag callbacks for <$if>,<$else> and <$elseif>
 *
 * updated: 24-Feb-1997
 * created:  7-Oct-1995
 */

#include "hsclib/inc_tagcb.h"

#include "hsclib/eval.h"
#include "hsclib/parse.h"
#include "hsclib/skip.h"

/* chars that represent TRUE, FALSE and
 * UNDEF (error in if-expression) on if_stack
 */
#define ISTK_FALSE '0'          /* condition false */
#define ISTK_TRUE  '1'          /* condition true */
#define ISTK_UNDEF 'x'          /* condition unknown (after error, but still parsing input) */
#define ISTK_ELSE  'e'          /* after processing alternative condition
                                 * * read: <$else> or <$elseif> */

typedef BYTE if_t;

/* error message */
static VOID message_unma_else(HSCPRC * hp, HSCTAG * tag)
{
    hsc_message(hp, MSG_UNMA_ELSE,
                "unmatched conditional %T", tag);
}

/* forward references */
BOOL handle_hsc_else(HSCPRC * hp, HSCTAG * tag);
BOOL handle_hsc_elseif(HSCPRC * hp, HSCTAG * tag);
BOOL handle_hsc_cif(HSCPRC * hp, HSCTAG * tag);

/* convert boolean value to value for if-stack */
static if_t bool2ift(BOOL bval)
{
    if (bval)
        return (ISTK_TRUE);
    else
        return (ISTK_FALSE);
}

/*
 *-------------------------------------
 * IF-stack manipulation
 *-------------------------------------
 */

/* is_push: add a new value to the if-stack */
static VOID is_push(HSCPRC * hp, if_t value)
{
    app_estrch(hp->if_stack, (char) value);

    DIF(fprintf(stderr, DHL "push IF-stack: \"%s\"\n", estr2str(hp->if_stack)));
}

/* is_get: get first value from the if-stack */
static if_t is_get(HSCPRC * hp)
{
    if_t value = ISTK_FALSE;

    if (hp->if_stack)
    {
        STRPTR stkstr = estr2str(hp->if_stack);

        DIF(fprintf(stderr, DHL "get  IF-stack: \"%s\"\n", stkstr));

        if (stkstr && (stkstr[0]))
        {
            char lastch = stkstr[strlen(stkstr) - 1];
            if ((lastch != ISTK_TRUE)
                && (lastch != ISTK_FALSE)
                && (lastch != ISTK_ELSE))
            {
                DIF(panic("Illegal value on if_stack"));
            }
            else
                value = (if_t) lastch;
        }
        else
        {
            DIF(panic("if_stack EMPTY"));
        }
    }
    else
    {
        DIF(panic("if_stack UNDEFINED"));
    }

    DIF(fprintf(stderr, DHL "get  IF-stack: value=`%c'\n", (char) value));

    return (value);

}

/* is_pop: remove first value of the if-stack */
static if_t is_pop(HSCPRC * hp)
{
    if_t value = is_get(hp);

    if (!strlen(estr2str(hp->if_stack)))
    {
        DIF(panic("Popping empty if_stack"));
    }

    get_left_estr(hp->if_stack, hp->if_stack, strlen(estr2str(hp->if_stack)) - 1);

    DIF(fprintf(stderr, DHL "pop  IF-stack: \"%s\"\n", estr2str(hp->if_stack)));

    return (value);
}

/* is_empty: check if if-stack is empty */
static BOOL is_empty(HSCPRC * hp)
{
    BOOL result = FALSE;

    if (!strlen(estr2str(hp->if_stack)))
        result = TRUE;

    return (result);
}

/*
 *-------------------------------------
 * SELECT-stack manipulation
 *-------------------------------------
 */

/*
 * del_select_stack_node
 */
VOID del_select_stack_node(APTR data)
{
    STRPTR s = (STRPTR) data;
    ufreestr(s);
}

/*
 * new_select_stack_node
 */
STRPTR new_select_stack_node(STRPTR data)
{
    return (strclone(data));
}

/*
 * cmp_select_stack_node
 */
int cmp_select_stack_node(APTR cmp_data, APTR lst_data)
{
    STRPTR s1 = (STRPTR) cmp_data;
    STRPTR s2 = (STRPTR) lst_data;

#if DEBUG
    if (!cmp_data)
        panic("cmp_data = NULL");
    if (!lst_data)
        panic("lst_data = NULL");
#endif

    if (!strcmp(s1, s2))
        return (-1);
    else
        return (0);
}

/* ss_push: add a new value to the if-stack */
#if 0
static VOID ss_push(HSCPRC * hp, STRPTR value)
{
    app_dlnode(hp->select_stack, new_select_stack_node(value));
    DIF(fprintf(stderr, DHL "push select_stack: \"%s\"\n", value));
}

/* ss_get: get first value from the select-stack */
static STRPTR ss_get(HSCPRC * hp, EXPSTR * dest)
{
    STRPTR value = NULL;

    if (hp->select_stack)
    {
        DLNODE *nd = dll_first(hp->select_stack);

        if (nd)
        {
            value = dln_data(nd);
        }
        else
        {
            DIF(panic("select_stack EMPTY"));
        }
    }
    else
    {
        DIF(panic("select_stack UNDEFINED"));
    }

    DIF(
           if (value)
           fprintf(stderr, DHL "get  select_stack: value=`%s'\n", value)
        );

    return (value);
}

/* ss_pop: remove first value of select-stack */
static STRPTR ss_pop(HSCPRC * hp, EXPSTR * dest)
{
    STRPTR value = ss_get(hp, dest);

    if (!value)
    {
        DIF(panic("popping empty select_stack"));
        value = "";
    }

    DIF(fprintf(stderr, DHL "pop  select_stack: \"%s\"\n", value));

    return (value);
}

/* ss_empty: check if if-stack is empty */
static BOOL ss_empty(HSCPRC * hp)
{
    BOOL result = FALSE;

    if (dll_first(hp->select_stack))
        result = TRUE;

    return (result);
}

/*
 *-------------------------------------
 * misc. functions
 *-------------------------------------
 */

/*
 * remove_cif_tag
 *
 * remove </$IF> from closing tag stack
 */
static VOID remove_cif_tag(HSCPRC * hp)
{
    HSCTAG endiftag;            /* artificial if-tag to remove */

    endiftag.name = HSC_IF_STR;
    remove_end_tag(hp, &endiftag);      /* remove end tag from stack */
    DIF(fprintf(stderr, DHL "</$IF> removed\n"));
}

/*
 * get_condition
 *
 * evaluate if-condition
 */
static if_t get_condition(HSCPRC * hp)
{
    if_t cond = ISTK_UNDEF;     /* boolean result of expression */
    STRPTR condstr = NULL;
    STRPTR nw = infgetw(hp->inpf);

    if (nw)
    {
        /* create temp. attr */
        HSCVAR *condattr = new_hscattr(PREFIX_TMPATTR "if.condition");

        /* skip `COND=' if there (new syntax) */
        if (!upstrcmp(nw, CONDITION_ATTR))
            parse_eq(hp);
        else
            inungetcw(hp->inpf);

        condattr->vartype = VT_BOOL;

        condstr = eval_expression(hp, condattr, NULL);

        /* check for closing ">" */
        parse_gt(hp);

        if (condstr)
            if (get_varbool(condattr))
                cond = ISTK_TRUE;
            else
                cond = ISTK_FALSE;
        else
            cond = ISTK_UNDEF;

        /* remove temp. attribute */
        del_hscattr(condattr);
    }
    return (cond);
}
#endif

/*
 * skip_until_conditional
 *
 * skip text, until <$/IF>, <$ELSEIF> or <$ELSE> is found
 * also handle recursive IFs
 *
 */
static VOID skip_until_conditional(HSCPRC * hp)
{
    EXPSTR *s = init_estr(32);
    /* skip until next conditional tag is found; this specific tag
     * will not be skipped, and be parsed afterwards */
    skip_until_tag(hp, NULL, s,
                   HSC_ELSE_STR "|" HSC_ELSEIF_STR, HSC_IF_STR,
                   SKUT_NO_SKIP_TAGFOUND);
    del_estr(s);
}

/*
 *
 * exported funcs
 *
 */

/*
 *-------------------------------------
 * <$IF> conditional conversion
 *-------------------------------------
 */
BOOL handle_hsc_if(HSCPRC * hp, HSCTAG * tag)
{
    BOOL new_condbool = get_varbool_byname(tag->attr, CONDITION_ATTR);
    if_t new_cond = bool2ift(new_condbool);

    /* store new_cond on stack */
    is_push(hp, new_cond);
    DIF(fprintf(stderr, DHL "  new_cond=`%c'\n", new_cond));

    if (new_cond != ISTK_TRUE)
    {
        DIF(DMSG("IF: refused"));
        skip_until_conditional(hp);
    }
    else
    {
        DIF(DMSG(" IF: GRANTED"));
    }

    return (FALSE);
}

/*
 *-------------------------------------
 * </$IF> conditional conversion
 *-------------------------------------
 */
BOOL handle_hsc_cif(HSCPRC * hp, HSCTAG * tag)
{
    DIF(fprintf(stderr, DHL "IF: standard closing handler\n"));

    if (is_empty(hp))
    {
        /* this can happen, if <$if> had errors in args */
        DIF(fprintf(stderr, DHL "%s: unhandled handler conditional\n", tag->name));
    }
    else
    {
        /* remove if-value from stack */
        is_pop(hp);
    }

    return (FALSE);
}

/*
 *-------------------------------------
 * <$ELSE> conditional conversion
 *-------------------------------------
 */
BOOL handle_hsc_else(HSCPRC * hp, HSCTAG * tag)
{
    if (is_empty(hp))
    {
        /* this can happen, if <$if> had errors in args */
        DIF(fprintf(stderr, DHL "%s: unhandled handler conditional\n", tag->name));
    }
    else
    {
        if_t value = is_pop(hp);

        if (value == ISTK_ELSE)
        {
            message_unma_else(hp, tag);
        }
        else if (value == ISTK_FALSE)
        {
            DIF(fprintf(stderr, DHL "ELSE: GRANTED\n"));
        }
        else
        {
            DIF(fprintf(stderr, DHL "ELSE: refused\n"));
            skip_until_conditional(hp);
        }

        /* mark <$else>-occured on if-stack */
        is_push(hp, ISTK_ELSE);
    }

    return (FALSE);
}

/*
 *-------------------------------------
 * <$ELSEIF> conditional conversion
 *-------------------------------------
 */
BOOL handle_hsc_elseif(HSCPRC * hp, HSCTAG * tag)
{
    BOOL new_condbool = get_varbool_byname(tag->attr, CONDITION_ATTR);
    if_t new_cond = bool2ift(new_condbool);

    if (is_empty(hp))
    {
        /* this can happen, if <$if> had errors in args */
        DIF(fprintf(stderr, DHL "%s: unhandled handler conditional\n", tag->name));
    }
    else
    {
        if_t old_cond = is_pop(hp);     /* condition of previous if/elseif */
        if_t push_cond = old_cond;      /* value to be pushed on stack */

        if (old_cond == ISTK_ELSE)
        {
            message_unma_else(hp, tag);
        }
        else if (old_cond == ISTK_TRUE)
        {
            DIF(fprintf(stderr, DHL "ELSEIF: refused (old_cond was true)\n"));
            skip_until_conditional(hp);
        }
        else if (new_cond == ISTK_TRUE)
        {
            DIF(fprintf(stderr, DHL "ELSEIF: GRANTED\n"));
            push_cond = new_cond;
        }
        else
        {
            DIF(fprintf(stderr, DHL "ELSEIF: refused (new_cond is false)\n"));
            skip_until_conditional(hp);
        }

        /* push new condition to if-stack */
        is_push(hp, push_cond);
    }

    return (FALSE);
}
