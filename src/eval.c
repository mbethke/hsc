/*
** eval.c
**
** attribute value evaluation functions
**
** updated: 18-Oct-1995
** created: 11-Oct-1995
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

#define NOEXTERN_EVAL_H
#include "eval.h"


/*
** equation operators
*/
#define OP_EQ_STR  "="
#define OP_NEQ_STR "<>"
#define OP_GT_STR  ">"
#define OP_LT_STR  "<"
#define OP_GTE_STR ">="
#define OP_LTE_STR "<="

#define OP_EQ   1
#define OP_NEQ  2
#define OP_GT   3
#define OP_LT   4
#define OP_GTE  5
#define OP_LTE  6

/*
** boolean operators
*/
#define OP_AND_STR "AND"
#define OP_NOT_STR "NOT"
#define OP_OR_STR  "OR"
#define OP_XOR_STR "XOR"

#define OP_AND 11
#define OP_NOT 12
#define OP_OR  13
#define OP_XOR 14

/*
** string operators
*/
#define OP_CAT_STR "+"
#define OP_CAT      11


/*
**
** global funcs
**
*/


/*
**-------------------------------------
** eval_attr: evaluate attributes
**-------------------------------------
*/

/*
** eval_attr
**
** eval attr name, return ptr to its structure
*/
HSCVAR *eval_attr( INFILE *inpf )
{
    STRPTR  varname = NULL;
    HSCVAR *var = NULL;

    if ( parse_wd( inpf, "<" ) ) {

        varname = infgetw( inpf );
        if ( varname )
            varname = strclone( varname );
        else
            err_eof( inpf, "read attribute" );

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
** eval_attr_type
**
** eval attr name, check for required type
*/

HSCVAR *eval_attr_type( INFILE *inpf, BYTE exptype )
{
    HSCVAR *attr = eval_attr( inpf );

    /* display error message, if attr is defined, but not of
    ** the expected type. also check, if the expected attr is
    ** of type STRING, and the found attr has a STRING-compatible
    ** type (ENUM,URI). In this case , display no message.                                    break;
    */
    if ( attr && ( exptype!=VT_NONE ) && ( exptype!=attr->vartype )
         && ( (exptype==VT_STRING)
              && ( (attr->vartype!=VT_URI) || (attr->vartype!=VT_ENUM) )
            )
       )
    {

        /* unexpected attribute type */
        message( MSG_UNEX_ATTR_TYPE, inpf );
        switch ( exptype ) {

            case VT_URI:
                errstr( "URI" );
                break;

            case VT_STRING:
                errstr( "string" );
                break;

            case VT_BOOL:
                errstr( "boolean" );
                break;

            case VT_NUM:
                errstr( "numeric" );
                break;

            case VT_ENUM:
                errstr( "enumerator" );
                break;

        }

        errstr( " attribute expected" );
        errlf();

        attr = NULL;

    }


    return( attr );
}

/*
**-------------------------------------
** eval_bool: evaluate bool-expression
**-------------------------------------
*/

/*
** eval_bool
**
** params: inpf..input file
** result: result of evaluation (IF_TRUE or FALSE)
**
*/
BOOL eval_bool( INFILE *inpf, BOOL *err, STRPTR endstr )
{
    BOOL    value = FALSE;
    STRPTR  nw    = infgetw( inpf );
    HSCVAR *var   = NULL;

    if ( !strcmp( nw, "<" ) ) {

        inungetcw( inpf );
        var = eval_attr_bool( inpf );
        if ( var && var->text )
            value = TRUE;

    } else if ( !upstrcmp( nw, "DEFINED" ) ) {

        var = eval_attr( inpf );
        *err = fatal_error;
        if ( var )
            value = TRUE;

#if 0
    } else if ( !upstrcmp( nw, "EXISTS" ) ) {

        /* TODO: exists */

    } else if ( !upstrcmp( nw, "EXTERN" ) ) {

        /* TODO: external reference */

#endif
    } else if ( !upstrcmp( nw, OP_NOT_STR ) ) {            /* NOT */

        value = !eval_bool( inpf, err, NULL );

    } else if ( !upstrcmp( nw, "SET" ) ) {

        /* get attribute, check for existence, check if value set */
        var = eval_attr( inpf );
        *err = fatal_error;
        if ( var && var->text )
            value = TRUE;


    }

    if ( !(*err) )
        if ( !parse_wd( inpf, endstr ) )
            *err = TRUE;

    return( value );
}


