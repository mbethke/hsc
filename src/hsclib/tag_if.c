/*
 * hsclib/tag_if.c
 *
 * tag callbacks for <$if>,<$else> and <$elseif>
 *
 * Copyright (C) 1996  Thomas Aglassinger
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
 * updated: 18-Aug-1996
 * created:  7-Oct-1995
 */

#include "hsclib/inc_tagcb.h"

#include "hsclib/eval.h"
#include "hsclib/parse.h"
#include "hsclib/skip.h"

/* states for skip_if */
#define IFST_TEXT    0          /* inside text */
#define IFST_LT      1          /* after "<" */
#define IFST_HSC     2          /* after "<$" */
#define IFST_SLASH   3          /* after "</" */
#define IFST_SLHSC   4          /* after "</$" */
#define IFST_IF      5          /* after "<$IF" */
#define IFST_ELSE    6          /* after "<$ELSE" */
#define IFST_CIF     7          /* after "</$IF" */
#define IFST_ELSEIF  8          /* after "<$ELSEIF" */
#define IFST_TAG     9          /* inside a tag */
#define IFST_ERR    99          /* error occured */

/* chars that represent TRUE, FALSE and
 * UNDEF (error in if-expression) on if_stack
 */
#define ISTK_FALSE '0'          /* condition false */
#define ISTK_TRUE  '1'          /* condition true */
#define ISTK_UNDEF 'x'          /* condition unknown */
#define ISTK_ELSE  'e'          /* after else */

typedef BYTE if_t;

/* error message */
static VOID message_unma_else(HSCPRC * hp, HSCTAG * tag)
{
    hsc_message(hp, MSG_UNMA_ELSE,
                "%T not associated with %t", tag, HSC_IF_STR);
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
    HSCTAG ciftag;              /* artificial if-tag to remove */

    ciftag.name = HSC_IF_STR;
    remove_ctag(hp, &ciftag);   /* remove closing tag from stack */
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

/*
 *-------------------------------------
 * skip_if: the ultimate if-handler
 *-------------------------------------
 */

/*
 * skip_if
 *
 * skip text, until <$/IF> or <$ELSE> is found
 * also handle recursive IFs
 *
 * params: inpf..input file
 * result: IFST_CIF, if exited with </$IF>,
 *         IFST_ELSE, if exited with </$ELSE>
 * errors: call err_eof(), if end-of-file,
 *         return IFST_ERR
 */

static BYTE skip_if(HSCPRC * hp)
{
    INFILE *inpf = hp->inpf;
    BOOL quit = FALSE;          /* TRUE, if end-of-if found */
    STRPTR nw = NULL;           /* word read from input */
    BYTE state = IFST_TEXT;     /* current state */
    LONG if_nest = 0;           /* counter for $IF nesting */

    do
    {
        if (state != IFST_TAG)
            nw = infgetw(inpf);
        if (nw)
        {
            if (state == IFST_TAG)
            {
                /*
                 * skip inside tags
                 */
                BYTE tag_state = TGST_TAG;      /* state var passe to */
                /*     eot_reached() */

                do
                {
                    if (eot_reached(hp, &tag_state))    /* CHECK: empty body? */
                        state = IFST_TEXT;
                }
                while ((tag_state != TGST_END) && !(hp->fatal));
            }
            else
            {
                /*
                 * NOTE: I know that this section could be
                 * shorter, but it would also make the
                 * source less readable
                 */

                /*
                 * evaluate next state depending on
                 * previous state
                 */
                switch (state)
                {

                case IFST_TEXT:
                    if (!strcmp(nw, "<"))
                        state = IFST_LT;
                    break;

                case IFST_LT:
                    if (!strcmp(nw, "$"))
                        state = IFST_HSC;
                    else if (!strcmp(nw, "/"))
                        state = IFST_SLASH;
                    else if (!upstrcmp(nw, HSC_COMMENT_STR))
                    {

                        skip_hsc_comment(hp);
                        state = IFST_TEXT;
                    }
                    else
                        state = IFST_TAG;

                    break;

                case IFST_HSC:
                    if (!upstrcmp(nw, "ELSE"))
                        state = IFST_ELSE;
                    else if (!upstrcmp(nw, "IF"))
                        state = IFST_IF;
                    else if (!upstrcmp(nw, "ELSEIF"))
                        state = IFST_IF;
                    else
                        state = IFST_TAG;
                    break;

                case IFST_SLASH:
                    if (!strcmp(nw, "$"))
                        state = IFST_SLHSC;
                    else
                        state = IFST_TAG;

                    break;

                case IFST_SLHSC:
                    if (!upstrcmp(nw, "IF"))
                        state = IFST_CIF;
                    else
                        state = IFST_TAG;

                    break;
                }

                /*
                 * handle special states
                 */
                switch (state)
                {

                case IFST_IF:
                    state = IFST_TAG;
                    if_nest++;
                    DIF(fprintf(stderr, DHL "skip <$IF>   (%ld)\n", if_nest));
                    break;

                case IFST_ELSE:
                    if (if_nest)
                    {
                        state = IFST_TAG;
                        DIF(fprintf(stderr, DHL "skip <$ELSE> (%ld)\n", if_nest));
                    }
                    else
                    {
                        /* TODO: check for 2nd <$ELSE> */
                        quit = TRUE;
                    }

                    break;

                case IFST_ELSEIF:
                    if (if_nest)
                    {
                        state = IFST_TAG;
                        DIF(fprintf(stderr, DHL "skip <$ELSEIF> (%ld)\n", if_nest));
                    }
                    else
                    {
                        /* TODO: check for 2nd <$ELSE> */
                        quit = TRUE;
                    }

                    break;

                case IFST_CIF:
                    if (if_nest)
                    {

                        state = IFST_TAG;
                        if_nest--;
                        DIF(fprintf(stderr, DHL "skip </$IF>  (%ld)\n", if_nest + 1));

                    }
                    else
                        quit = TRUE;

                    break;
                }
            }
        }
        else
        {
            hsc_msg_eof(hp, "missing </" HSC_IF_STR ">");
            state = IFST_ERR;
        }
    }
    while (!quit && nw);

    /* check for legal end state */
    if ((state == IFST_CIF)
        || (state == IFST_ELSE))
    {
        parse_wd(hp, ">");

        if (state == IFST_CIF)
        {
            DIF(fprintf(stderr, DHL "</$IF> reached\n"));
        }
        else
        {
            DIF(fprintf(stderr, DHL "<$ELSE> reached\n"));
        }
    }
    else if (state == IFST_ELSEIF)
    {
        DIF(fprintf(stderr, DHL "<$ELSEIF> reached\n"));
    }

    return (state);
}

static VOID skip_until_conditional(HSCPRC * hp)
{
    EXPSTR *s = init_estr(32);
    skip_until_tag(hp, s, HSC_ELSE_STR "|" HSC_ELSEIF_STR, HSC_IF_STR);
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
#if 0
    if_t new_cond = get_condition(hp);

    /* store new_cond on stack */
    is_push(hp, new_cond);

    if (new_cond != ISTK_TRUE)
    {
        BYTE state;

        DIF(DMSG("IF: refused"));
        state = skip_if(hp);
        if ((state == IFST_ELSE) || (state == IFST_ELSEIF))
        {
            if (new_cond != ISTK_UNDEF)
            {
                if (state == IFST_ELSE)
                {
                    DIF(DMSG("IF: process ELSE"));
                    handle_hsc_else(hp, find_strtag(hp->deftag, HSC_ELSE_STR));
                }
                else if (state == IFST_ELSEIF)
                {
                    DIF(DMSG("IF: process ELSEIF"));
                    handle_hsc_elseif(hp, find_strtag(hp->deftag, HSC_ELSE_STR));
                }
                else
                {
                    DIF(panic("unexpected if-state"));
                }
            }
            else
            {
                DIF(DMSG("ELSE/ELSEIF: refused (undef)"));
                state = skip_if(hp);
                if (state == IFST_ELSE)
                    message_unma_else(hp, tag);
                remove_cif_tag(hp);
            }
        }
        else if (state == IFST_CIF)
            remove_cif_tag(hp);
        else if (state != IFST_ERR)
        {
            D(panic("strange if-state"));
        }
    }
    else
    {
        DIF(DMSG(" IF: GRANTED"));
    }
#else
#ifdef OLDIFCOND
    if_t new_cond = get_condition(hp);
#else
    BOOL new_condbool = get_varbool_byname(tag->attr, CONDITION_ATTR);
    if_t new_cond = bool2ift(new_condbool);
#endif

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
#endif

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

    is_pop(hp);                 /* remove if-value from stack */

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
        message_unma_else(hp, tag);
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
        message_unma_else(hp, tag);
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

