/*
** hsclib/tag_if.c
**
** tag callbacks for <$IF> and <$ELSE>
**
** Copyright (C) 1996  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 12-Apr-1996
** created:  7-Oct-1995
*/

#include "hsclib/inc_tagcb.h"

#include "hsclib/eval.h"
#include "hsclib/parse.h"


/* states for skip_if */
#define IFST_TEXT  0 /* inside text */
#define IFST_LT    1 /* after "<" */
#define IFST_HSC   2 /* after "<$" */
#define IFST_SLASH 3 /* after "</" */
#define IFST_SLHSC 4 /* after "</$" */
#define IFST_IF    5 /* after "<$IF" */
#define IFST_ELSE  6 /* after "<$ELSE" */
#define IFST_CIF   7 /* after "</$IF" */
#define IFST_TAG   8 /* inside a tag */
#define IFST_ERR  99 /* error occured */

/* chars that represent TRUE, FALSE and
** UNDEF (error in if-expression) on IF_stack
*/
#define ISTK_FALSE '0'
#define ISTK_TRUE  '1'
#define ISTK_UNDEF 'x'

typedef BYTE if_t;

/* error message */
static VOID message_unma_else( HSCPRC *hp, HSCTAG *tag )
{
    hsc_message( hp, MSG_UNMA_ELSE,
                 "%T not associated with %t", tag, HSC_IF_STR );
}

/* forward references */
BOOL handle_hsc_cif_else( HSCPRC *hp, HSCTAG *tag );
BOOL handle_hsc_cif( HSCPRC *hp, HSCTAG *tag );

/*
**-------------------------------------
** IF-stack manipulation
**-------------------------------------
*/

/* is_push: add a new value to the if-stack */
static VOID is_push( HSCPRC *hp, if_t value )
{
    BYTE ch;

    if ( value ) ch = ISTK_TRUE;
    else         ch = ISTK_FALSE;

    app_estrch( hp->IF_stack, ch );

    DIF( fprintf( stderr, DHL "push IF-stack: \"%s\"\n", estr2str( hp->IF_stack ) ) );


}

/* is_get: get first value from the if-stack */
static if_t is_get( HSCPRC *hp )
{
    if_t value = ISTK_FALSE;

    if ( hp->IF_stack ) {

        STRPTR stkstr = estr2str( hp->IF_stack );

        DIF( fprintf( stderr, DHL "get  IF-stack: \"%s\"\n", stkstr ) );

        if ( stkstr && (stkstr[0]) ) {

            char lastch = stkstr[strlen(stkstr)-1];
            if ( lastch == ISTK_TRUE )
                value = TRUE;
            else if ( lastch != ISTK_FALSE )
                DIF( panic( "Illegal value on IF_stack" ) );

        } else
            DIF( panic( "IF_stack EMPTY" ) );


    } else
        DIF( panic( "IF_stack UNDEFINED" ) );

    DIF( fprintf( stderr, DHL "get  IF-stack: value=%d\n", value ) );

    return( value );

}

/* is_pop: remove first value of the if-stack */
static if_t is_pop( HSCPRC *hp )
{
    if_t value = is_get( hp );

    if ( !strlen(estr2str(hp->IF_stack)) )
        DIF( panic( "Popping empty IF_stack" ) );

    get_left_estr( hp->IF_stack, hp->IF_stack, strlen(estr2str(hp->IF_stack))-1 );

    DIF( fprintf( stderr, DHL "pop  IF-stack: \"%s\"\n", estr2str( hp->IF_stack ) ) );

    return( value );
}

/* is_empty: check if if-stack is empty */
static BOOL is_empty( HSCPRC *hp )
{
    BOOL result = FALSE;

    if ( !strlen( estr2str( hp->IF_stack ) ) )
        result = TRUE;

    return( result );
}

/*
**-------------------------------------
** misc. functions
**-------------------------------------
*/

/*
** remove_cif_tag
**
** remove </$IF> from closing tag stack
*/
static VOID remove_cif_tag( HSCPRC *hp )
{
    HSCTAG ciftag; /* artificial if-tag to remove */

    ciftag.name = HSC_IF_STR;
    remove_ctag( hp, &ciftag );  /* remove closing tag from stack */
    DIF( fprintf( stderr, DHL "</$IF> removed\n" ) );
}

/*
**-------------------------------------
** skip_if: the ultimate if-handler
**-------------------------------------
*/

/*
** skip_if
**
** skip text, until <$/IF> or <$ELSE> is found
** also handle recursive IFs
**
** params: inpf..input file
** result: IFST_CIF, if exited with </$IF>,
**         IFST_ELSE, if exited with </$ELSE>
** errors: call err_eof(), if end-of-file,
**         return IFST_ERR
*/
static BYTE skip_if( HSCPRC *hp )
{
    INFILE *inpf = hp->inpf;
    BOOL   quit    = FALSE;     /* TRUE, if end-of-if found */
    STRPTR nw      = NULL;      /* word read from input */
    BYTE   state   = IFST_TEXT; /* current state */
    LONG   if_nest = 0;         /* counter for $IF nesting */

    do {

        if ( state != IFST_TAG )
            nw = infgetw( inpf );
        if ( nw ) {

            if ( state == IFST_TAG ) {

                /*
                ** skip inside tags
                */
                BYTE   tag_state = TGST_TAG; /* state var passe to */
                                             /*     eot_reached() */

                do {

                    if ( eot_reached( hp, &tag_state ) ) /* CHECK: empty body? */
                        state = IFST_TEXT;

                } while ( (tag_state!=TGST_END) && !(hp->fatal) );

            } else {

                /*
                ** NOTE: I know that this section could be
                ** shorter, but it would also make the
                ** source less readable
                */

                /*
                ** evaluate next state depending on
                ** previous state
                */
                switch ( state ) {

                    case IFST_TEXT:
                        if ( !strcmp( nw, "<" ) )
                            state = IFST_LT;
                        break;

                    case IFST_LT:
                        if ( !strcmp( nw, "$" ) )
                            state = IFST_HSC;
                        else if ( !strcmp( nw, "/" ) )
                            state = IFST_SLASH;
                        else if ( !upstrcmp( nw, HSC_COMMENT_STR ) ) {

                            skip_hsc_comment( hp );
                            state = IFST_TEXT;
                        } else
                            state = IFST_TAG;

                        break;

                    case IFST_HSC:
                        if ( !upstrcmp( nw, "ELSE" ) )
                            state = IFST_ELSE;
                        else if ( !upstrcmp( nw, "IF" ) )
                            state = IFST_IF;
                        else
                            state = IFST_TAG;
                        break;

                    case IFST_SLASH:
                        if ( !strcmp( nw, "$" ) )
                            state = IFST_SLHSC;
                        else
                            state = IFST_TAG;

                        break;

                    case IFST_SLHSC:
                        if ( !upstrcmp( nw, "IF" ) )
                            state = IFST_CIF;
                        else
                            state = IFST_TAG;

                        break;

                }

                /*
                ** handle special states
                */
                switch ( state ) {

                    case  IFST_IF:
                        state = IFST_TAG;
                        if_nest++;
                        DIF( fprintf( stderr, DHL "skip <$IF>   (%ld)\n", if_nest ) );
                        break;

                    case  IFST_ELSE:
                        if ( if_nest ) {
                            state = IFST_TAG;
                            DIF( fprintf( stderr, DHL "skip <$ELSE> (%ld)\n", if_nest ) );
                        } else {

                            /* TODO: check for 2nd <$ELSE> */
                            quit = TRUE;
                        }

                        break;

                    case IFST_CIF:
                        if ( if_nest ) {

                            state = IFST_TAG;
                            if_nest--;
                            DIF( fprintf( stderr, DHL "skip </$IF>  (%ld)\n", if_nest+1 ) );

                        } else
                            quit = TRUE;

                        break;
                }
            }
        } else {

            hsc_msg_eof( hp, "missing </" HSC_IF_STR ">" );
            state = IFST_ERR;

        }

    } while ( !quit && nw );

    /* check for legal end state */
    if ( (state == IFST_CIF)
         || (state == IFST_ELSE) )
    {

#if 0
        /* remove closing if-tag from stack */
        if ( state == IFST_CIF ) {

            remove_cif_tag( inpf );    
            is_pop( hp );

        }
#endif
        if ( !parse_wd( hp, ">" ) )
            skip_until_eot( hp, NULL );
        DIF(  {
            if ( state==IFST_CIF )
                fprintf( stderr, DHL "</$IF> reached\n" );
            else
                fprintf( stderr, DHL "<$ELSE> reached\n" );
            } );

    }

    return( state );
}


/*
**
** exported funcs
**
*/

/*
**-------------------------------------
** <$IF> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_if( HSCPRC *hp, HSCTAG *tag )
{
    if_t value;         /* boolean result of expression */
    HSCTAG *if_tag   = find_strtag( hp->deftag, HSC_IF_STR );
    HSCVAR *ifresult = new_hscattr( "__IF_RESULT__" );
    STRPTR ifstr     = NULL;

    ifresult->vartype = VT_BOOL;

    ifstr    = eval_expression( hp, ifresult, NULL );

    /* check for closing ">" */
    parse_gt( hp );

    if ( ifstr )
        if ( get_varbool( ifresult ) )
            value = ISTK_TRUE;
        else
            value = ISTK_FALSE;
    else
        value = ISTK_UNDEF;

    if ( value != ISTK_TRUE ) {

        BYTE state;

        DIF( DMSG( "IF: refused" ) );
        state = skip_if( hp );
        if ( state == IFST_ELSE ) {

            if ( value != ISTK_UNDEF ) {

                DIF( DMSG( "IF: process ELSE" ) );

            } else {

                DIF( DMSG( "ELSE: refused (undef)" ) );
                state = skip_if( hp );
                if ( state == IFST_ELSE )
                    message_unma_else( hp, tag );
                remove_cif_tag( hp );

            }

        } else
            remove_cif_tag( hp );

        /* assign standard handler for </$IF> */
        if_tag->c_handle = handle_hsc_cif_else;

    } else {

        /* store value on stack */
        is_push( hp, value );

        /* assign standard handler for </$IF> */
        if_tag->c_handle = handle_hsc_cif;

        DIF( DMSG( " IF: GRANTED" ) );

    }

    /* remove temp attribute */
    del_hscattr( (APTR) ifresult );

    return ( FALSE );
}

/*
**-------------------------------------
** </$IF> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_cif( HSCPRC *hp, HSCTAG *tag )
{
    DIF( fprintf( stderr, DHL "IF: standard closing handler\n" ) );

    is_pop( hp );                      /* remove if-value from stack */

    return ( FALSE );
}

/*
**-------------------------------------
** </$IF> after <$ELSE>
**-------------------------------------
*/
BOOL handle_hsc_cif_else( HSCPRC *hp, HSCTAG *tag )
{
    DIF( fprintf( stderr, DHL "IF: closing handler after ELSE\n" ) );
    /* DON'T call is_pop(), because <$ELSE> has done this before */

    return ( FALSE );
}

/*
**-------------------------------------
** <$ELSE> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_else( HSCPRC *hp, HSCTAG *tag )
{
    if ( is_empty( hp ) ) {

        message_unma_else( hp, tag );

    } else {

        HSCTAG *if_tag = find_strtag( hp->deftag, HSC_IF_STR );
        if_t    value  = is_pop( hp );

        /* assign handler for </$IF> after <$ELSE> (see above) */
        if_tag->c_handle = handle_hsc_cif_else;

        if ( value == ISTK_FALSE ) {

            DIF( fprintf( stderr, DHL "ELSE: GRANTED\n" ) );

        } else {

            BYTE state;

            DIF( fprintf( stderr, DHL "ELSE: refused\n" ) );
            state = skip_if( hp );
            if ( state == IFST_ELSE )
                message_unma_else( hp, tag );

            remove_cif_tag( hp );

        }
    }

    return ( FALSE );
}

