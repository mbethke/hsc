/*
** tag_if.c
**
** tag handles for <$IF> and <$ELSE>
**
** updated:  8-Oct-1995
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
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "skip.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"

/*
** TODO:
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
#define IFST_CIF   7 /* after "<$/IF" */
#define IFST_TAG   8 /* inside a tag */
#define IFST_ERR  99 /* error occured */

/* chars that represent TRUE and FALSE on IF_stack */
#define ISTK_FALSE '0'
#define ISTK_TRUE  '1'


BOOL parse_wd( INFILE *inpf, STRPTR expstr )
{
    BOOL   value = TRUE;

    if ( expstr ) {

        STRPTR nw    = infgetw( inpf );
        if ( nw ) {

            if ( upstrcmp( nw, expstr ) ) {

                message( ERROR, inpf );  /* TODO: error message */
                errstr( "Expected " );
                errqstr( expstr );
                errstr( ", found " );
                errqstr( nw );
                errlf();
                value = FALSE;

            }
        } else {

            err_eof( inpf );
            value = FALSE;

        }
    }

    return( value );
}

/*
**-------------------------------------
** IF-stack manipulation
**-------------------------------------
*/

/*
**
*/

/* is_push: add a new value to the if-stack */
VOID is_push( BOOL value )
{
    BYTE ch;

    if ( value ) ch = ISTK_TRUE;
    else         ch = ISTK_FALSE;

    if ( !app_estrch( IF_stack, ch ) )
        err_mem( NULL );

}

/* is_get: get first value from the if-stack */
BOOL is_get( VOID )
{
    BOOL value = FALSE;

    if ( IF_stack ) {

        STRPTR stkstr = estr2str( IF_stack );

        if ( !stkstr || !(stkstr[0]) )
            D( panic( "IF_stack EMPTY" ) );
        else {

            char lastch = stkstr[strlen(stkstr)];
            if ( lastch == ISTK_TRUE )
                value = TRUE;
            else if ( lastch != ISTK_FALSE )
                D( panic( "Illegal value on IF_stack" ) );

        }

    } else
        D( panic( "IF_stack EMPTY" ) );

    return( value );

}

/* is_push: remove first value of the if-stack */
BOOL is_pop( VOID )
{
    BOOL value = is_get();

    if ( !strlen(estr2str(IF_stack)) )
        D( panic( "Popping empty IF_stack" ) );

    if ( !get_left_estr( IF_stack, IF_stack, strlen(estr2str(IF_stack)) ) )
        err_mem( NULL );

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
                        else
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
                        break;

                    case  IFST_ELSE:
                        if ( if_nest )
                            state = IFST_TAG;
                        else {

                            /* TODO: check for 2nd <$ELSE> */
                            quit = TRUE;
                        }

                        break;

                    case IFST_CIF:
                        if ( if_nest ) {

                            state = IFST_TAG;
                            if_nest--;

                        } else {

                            /* TODO: pop IF-stack */
                            quit = TRUE;

                        }

                        break;
                }
            }
        } else {

            err_eof( inpf );
            state = IFST_ERR;

        }

    } while ( !quit && nw );

    /* check for legal end state */
    if ( (state == IFST_CIF)
         || (state == IFST_ELSE) )
    {

        if ( !parse_wd( inpf, ">" ) )
            skip_until_eot( inpf );

    }

    return( state );
}

/*
**-------------------------------------
** eval_if: evaluate IF-expression
**-------------------------------------
*/

HSCVAR *getvar_if( INFILE *inpf )
{
    STRPTR  varname = NULL;
    HSCVAR *var = NULL;

    if ( parse_wd( inpf, "<" ) ) {

        varname = infgetw( inpf );
        if ( varname )
            varname = strclone( varname );
        else
            err_eof( inpf );

        if ( !varname )
            err_mem( inpf );
        else {

            if ( parse_wd( inpf, ">" ) )
                var = find_varname( vars, varname );
            else
                skip_until_eot( inpf );
            ufreestr( varname );
        }

    } else
        skip_until_eot( inpf );

    return( var );
}

/*
** eval_ifexpr
**
** params: inpf..input file
** result: result of evaluation (IF_TRUE or FALSE)
**
*/
BOOL eval_ifexpr( INFILE *inpf, BOOL *err, STRPTR endstr )
{
    BOOL    value = FALSE;
    STRPTR  nw    = infgetw( inpf );
    HSCVAR *var   = NULL;

    if ( !strcmp( nw, "<" ) ) {

         /* TODO: eval bool var */

    } else if ( !upstrcmp( nw, "DEFINED" ) ) {

        var = getvar_if( inpf );
        *err = fatal_error;
        if ( var )
            value = TRUE;

    } else if ( !upstrcmp( nw, "EXISTS" ) ) {

        /* TODO: exists */

    } else if ( !upstrcmp( nw, "EXTERN" ) ) {

        /* TODO: external reference */

    } else if ( !upstrcmp( nw, "NOT" ) ) {

        value = !eval_ifexpr( inpf, err, NULL );

    } else if ( !upstrcmp( nw, "SET" ) ) {

        /* get attribute, check for existence, check if value set */
        var = getvar_if( inpf );
        *err = fatal_error;
        if ( var && var->text )
            value = TRUE;


    }

    if ( !(*err) )
        if ( !parse_wd( inpf, endstr ) )
            *err = TRUE;

    return( value );
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
    BOOL ok    = TRUE;
    BOOL err   = FALSE; /* error flag */
    BOOL value = eval_ifexpr( inpf, &err, ">" );

    is_push( value );
    if ( !value ) {

        BYTE state;

        D( fprintf( stderr, "** IF: refused\n" ) );
        state = skip_if( inpf );
        if ( state == IFST_ELSE )
            D( fprintf( stderr, "** IF: process ELSE\n" ) );

    } else
        D( fprintf( stderr, "** IF: GRANTED\n" ) );

    return ( ok );
}

/*
**-------------------------------------
** </$IF> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_cif( INFILE *inpf, HSCTAG *tag )
{
    BOOL ok = TRUE;

    is_pop();                          /* remove if-value from stack */
    remove_ctag( tag, inpf );          /* remove closing tag from stack */
    if ( !parse_wd( inpf, ">" ) )
        skip_until_eot( inpf );

    return ( ok );
}

/*
**-------------------------------------
** <$ELSE> conditional conversion
**-------------------------------------
*/
BOOL handle_hsc_else( INFILE *inpf, HSCTAG *tag )
{
    BOOL ok = TRUE;
    if ( is_empty() ) {

        message( MSG_UNMA_ELSE, inpf );
        errtag( tag->name );
        errstr( " not assoziated with " );
        errtag( HSC_IF_STR );
        errlf();

    } else if ( !is_get() ) {

        D( fprintf( stderr, "** ELSE: GRANTED\n" ) );

    } else {

        BYTE state;

        D( fprintf( stderr, "** ELSE: refused\n" ) );
        state = skip_if( inpf );
        if ( state == IFST_ELSE )
            /* TODO: </$IF> expected */;
        remove_ctag( tag, inpf );      /* remove closing tag from stack */

    }

    return ( ok );
}



