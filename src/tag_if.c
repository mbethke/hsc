/*
** tag_if.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** tag handles for <$IF> and <$ELSE>
**
** updated: 27-Nov-1995
** created:  7-Oct-1995
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/memory.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "eval.h"
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "skip.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"
#include "eval.h"

/*
*/

/*
**
** global funcs
**
*/

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
void message_unma_else( INFILE *inpf, HSCTAG *tag )
{
    message( MSG_UNMA_ELSE, inpf );
    errtag( tag->name );
    errstr( " not associated with " );
    errtag( HSC_IF_STR );
    errlf();
}

/* forward references */
BOOL handle_hsc_cif_else( INFILE *inpf, HSCTAG *tag );
BOOL handle_hsc_cif( INFILE *inpf, HSCTAG *tag );

/*
**-------------------------------------
** IF-stack manipulation
**-------------------------------------
*/

/* is_push: add a new value to the if-stack */
VOID is_push( if_t value )
{
    BYTE ch;

    if ( value ) ch = ISTK_TRUE;
    else         ch = ISTK_FALSE;

    app_estrch( IF_stack, ch );

    DIF( fprintf( stderr, "** push IF-stack: \"%s\"\n", estr2str( IF_stack ) ) );


}

/* is_get: get first value from the if-stack */
if_t is_get( VOID )
{
    if_t value = ISTK_FALSE;

    if ( IF_stack ) {

        STRPTR stkstr = estr2str( IF_stack );

        DIF( fprintf( stderr, "** get  IF-stack: \"%s\"\n", stkstr ) );

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

    DIF( fprintf( stderr, "** get  IF-stack: value=%d\n", value ) );

    return( value );

}

/* is_pop: remove first value of the if-stack */
if_t is_pop( VOID )
{
    if_t value = is_get();

    if ( !strlen(estr2str(IF_stack)) )
        DIF( panic( "Popping empty IF_stack" ) );

    get_left_estr( IF_stack, IF_stack, strlen(estr2str(IF_stack))-1 );

    DIF( fprintf( stderr, "** pop  IF-stack: \"%s\"\n", estr2str( IF_stack ) ) );

    return( value );
}

/* is_empty: check if if-stack is empty */
BOOL is_empty( VOID )
{
    BOOL result = FALSE;

    if ( !strlen( estr2str( IF_stack ) ) )
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
VOID remove_cif_tag( INFILE *inpf )
{
    HSCTAG ciftag; /* artificial if-tag to remove */
    ciftag.name = HSC_IF_STR;
    remove_ctag( &ciftag, inpf );  /* remove closing tag from stack */
    DIF( fprintf( stderr, "** </$IF> removed\n" ) );
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
BYTE skip_if( INFILE *inpf )
{
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

                    if ( eot_reached( inpf, &tag_state ) );
                        state = IFST_TEXT;

                } while ( (tag_state!=TGST_END) && !fatal_error );

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

                            skip_hsc_comment( inpf );
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
                        DIF( fprintf( stderr, "** skip <$IF>   (%d)\n", if_nest ) );
                        break;

                    case  IFST_ELSE:
                        if ( if_nest ) {
                            state = IFST_TAG;
                            DIF( fprintf( stderr, "** skip <$ELSE> (%d)\n", if_nest ) );
                        } else {

                            /* TODO: check for 2nd <$ELSE> */
                            quit = TRUE;
                        }

                        break;

                    case IFST_CIF:
                        if ( if_nest ) {

                            state = IFST_TAG;
                            if_nest--;
                            DIF( fprintf( stderr, "** skip </$IF>  (%d)\n", if_nest+1 ) );

                        } else
                            quit = TRUE;

                        break;
                }
            }
        } else {

            err_eof( inpf, "missing </" HSC_IF_STR ">" );
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
            is_pop();

        }
#endif
        if ( !parse_wd( inpf, ">" ) )
            skip_until_eot( inpf, NULL );
        DIF(  {
            if ( state==IFST_CIF )
                fprintf( stderr, "** </$IF> reached\n" );
            else
                fprintf( stderr, "** <$ELSE> reached\n" );
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
BOOL handle_hsc_if( INFILE *inpf, HSCTAG *tag )
{
    if_t value;         /* boolean result of expression */
    HSCTAG *if_tag   = find_strtag( deftag, HSC_IF_STR );
    HSCVAR *ifresult = new_hscvar( "__ifresult__" );
    STRPTR ifstr     = NULL;

    ifresult->vartype = VT_BOOL;

    ifstr    = eval_expression( ifresult, inpf, NULL );

    /* check for closing ">" */
    parse_gt( inpf );

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
        state = skip_if( inpf );
        if ( state == IFST_ELSE ) {

            if ( value != ISTK_UNDEF ) {

                DIF( DMSG( "IF: process ELSE" ) );

            } else {

                DIF( DMSG( "ELSE: refused (undef)" ) );
                state = skip_if( inpf );
                if ( state == IFST_ELSE )
                    message_unma_else( inpf, tag );
                remove_cif_tag( inpf );

            }

        } else
            remove_cif_tag( inpf );

        /* assign standard handler for </$IF> */
        if_tag->c_handle = handle_hsc_cif_else;

    } else {

        /* store value on stack */
        is_push( value );

        /* assign standard handler for </$IF> */
        if_tag->c_handle = handle_hsc_cif;

        DIF( DMSG( " IF: GRANTED" ) );

    }

    /* remove temp attribute */
    del_var( (APTR) ifresult );

    return ( FALSE );
}

/*
**-------------------------------------
** </$IF> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_cif( INFILE *inpf, HSCTAG *tag )
{
    DIF( fprintf( stderr, "** IF: standard closing handler\n" ) );

    is_pop();                          /* remove if-value from stack */
#if 0
    remove_ctag( tag, inpf );          /* remove closing tag from stack */
    if ( !parse_wd( inpf, ">" ) )
        skip_until_eot( inpf );
#endif

    return ( FALSE );
}

/*
**-------------------------------------
** </$IF> after <$ELSE>
**-------------------------------------
*/
BOOL handle_hsc_cif_else( INFILE *inpf, HSCTAG *tag )
{
    DIF( fprintf( stderr, "** IF: closing handler after ELSE\n" ) );
    /* DON'T call is_pop(), because <$ELSE> has done this before */

    return ( FALSE );
}

/*
**-------------------------------------
** <$ELSE> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_else( INFILE *inpf, HSCTAG *tag )
{
    if ( is_empty() ) {

        message_unma_else( inpf, tag );

    } else {

        HSCTAG *if_tag = find_strtag( deftag, HSC_IF_STR );
        if_t    value  = is_pop();

        /* assign handler for </$IF> after <$ELSE> (see above) */
        if_tag->c_handle = handle_hsc_cif_else;

        if ( value == ISTK_FALSE ) {

            DIF( fprintf( stderr, "** ELSE: GRANTED\n" ) );

        } else {

            BYTE state;

            DIF( fprintf( stderr, "** ELSE: refused\n" ) );
            state = skip_if( inpf );
            if ( state == IFST_ELSE )
                message_unma_else( inpf, tag );

            remove_cif_tag( inpf );

        }
    }

    return ( FALSE );
}

