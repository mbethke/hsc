/*
** eval.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** attribute value evaluation functions
**
** updated: 10-Dec-1995
** created: 11-Oct-1995
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
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
#include "uri.h"

#define NOEXTERN_EVAL_H
#include "eval.h"

/* maximul length of an operator identifer */
#define MAX_OP_LEN 8

/* unknown operator */
#define OP_NONE 0

/*
** equation operators
*/
#define OP_EQ_STR  "="
#define OP_NEQ_STR "<>"
#define OP_GT_STR  ">"
#define OP_LT_STR  "<"
#define OP_GTE_STR ">="
#define OP_LTE_STR "<="
#define OP_CEQ_STR "==" /*  case sensitive string comparison */
#define OP_CL_BRAKET_STR ")"  /* closing braket */

#define OP_EQ   1
#define OP_NEQ  2
#define OP_GT   3
#define OP_LT   4
#define OP_GTE  5
#define OP_LTE  6
#define OP_CEQ  7
#define OP_CL_BRAKET  8

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
#define OP_CAT      15

typedef BYTE op_t;

/*
** forward references
*/
STRPTR eval_expression( HSCVAR *dest, INFILE *inpf, STRPTR endstr );

/*
**
** global funcs
**
*/

/*
** err_op: unknown binary operator
*/
VOID err_op( INFILE *inpf, STRPTR opstr )
{
    message( MSG_UNKN_BINOP, inpf );
    errstr( "unknown binary operator " );
    errqstr( opstr );
    errlf();
}

/*
** err_unkn_attr: reference to unknown attribute
*/
VOID err_unkn_attr( INFILE *inpf, STRPTR attrname )
{
    message( MSG_UNKN_SYMB_REF, inpf );
    errstr( "reference to unknown" );
    errsym( attrname );
    errlf();
}

/*
** eval_boolstr
*/
BOOL eval_boolstr( STRPTR s )
{
    if  ( s[0] )
        return( TRUE );
    else
        return( FALSE );
}

/*
** check_attrname
**
** check string for legal attribute name
*/
BOOL check_attrname ( STRPTR name, INFILE *inpf )
{
    BOOL ok = FALSE;

    if ( hsc_normch( name[0] ) )
        ok = TRUE;
    else {

        message( MSG_ILLG_ATTRNAME, inpf );
        errstr( "illegal attribute identifier " );
        errqstr( name );
        errlf();

    }

    return( ok );
}

/*
** eval_attrname
**
** read next word and check it for a legal
** attribute identifier
*/
STRPTR eval_attrname( INFILE *inpf )
{
    STRPTR result = NULL;
    STRPTR nw     = infgetw( inpf );

    if ( nw ) {

        if ( check_attrname( nw, inpf ) )
            result = nw;

    } else
        err_eof( inpf, "attribute identifier expected" );

    return( result );
}


/*
** try_eval_unary_op
**
** reads next word and tries to interpret it as an unary
** operator; if no fitting operator exists, return NULL,
** else immediatly process the operator and return its
** result
*/
STRPTR try_eval_unary_op( HSCVAR *dest, INFILE *inpf, BOOL *err )
{
    STRPTR eval_result = NULL;
    STRPTR nw = eval_attrname( inpf );


    *err = FALSE;
    if ( nw ) {

        if ( !upstrcmp( nw, "NOT" ) ) {

            /* TODO: this part looks a bit supid... */
            STRPTR nw = infgetw( inpf );

            if ( nw ) {

                BOOL   err_rec = FALSE; /* error var for recursive call */
                STRPTR endstr = NULL;

                if ( strcmp( nw, "(" ) ) {

                    /* try to process another unary operator */
                    inungetcw( inpf );
                    eval_result = try_eval_unary_op( dest, inpf, &err_rec );

                } else
                    endstr = ")";

                /* if not, process another expression */
                if ( !eval_result && !err_rec )
                    eval_result = eval_expression( dest, inpf, endstr );

            } else
                err_eof( inpf, "after NOT" );

            /* set result or return error */
            if ( eval_result ) {

                set_vartext_bool( dest, !get_varbool( dest ) );
                eval_result = get_vartext( dest );

            } else
                *err = TRUE;

        } else if ( !upstrcmp( nw, "DEFINED" ) ) {

            nw = eval_attrname( inpf );
            if ( nw ) {

                HSCVAR *attr = find_varname( vars, nw );

                if ( attr )
                    set_vartext_bool( dest, TRUE );
                else
                    set_vartext_bool( dest, FALSE );
                eval_result = get_vartext( dest );

            }
        } else if ( !upstrcmp( nw, "SET" ) ) {

            nw = eval_attrname( inpf );
            if ( nw ) {

                HSCVAR *attr = find_varname( vars, nw );

                if ( attr ) {
                    if ( attr->vartype == VT_BOOL ) {

                        set_vartext_bool( dest, get_varbool( attr ) );

                    } else if ( get_vartext( attr ) )
                        set_vartext_bool( dest, TRUE );
                    else
                        set_vartext_bool( dest, FALSE );
                    eval_result = get_vartext( dest );

                } else {

                    err_unkn_attr( inpf, nw );
                    *err = TRUE;

                }
            }
        } else
            inungetcw( inpf );

    }

    if ( !nw )
        *err = TRUE;

    return( eval_result );
}


/*
** eval_op
**
** evaluate binary operator string
*/
BYTE eval_op( INFILE *inpf )
{
    BYTE   op = OP_NONE;
    BOOL   op_eof = FALSE; /* flag: end of file reached */
    STRPTR nw = infgetw( inpf );

    D( fprintf( stderr, "**   operator \"%s", nw ) );

    if ( nw ) {

        /* boolean operators */
        if ( !upstrcmp( nw, OP_AND_STR ) )
            op = OP_AND;
        else if ( !upstrcmp( nw, OP_OR_STR ) )
            op = OP_OR;
        else if ( !upstrcmp( nw, OP_XOR_STR ) )
            op = OP_XOR;

        /* concatenation operator */
        else if ( !strcmp( nw, OP_CAT_STR ) )
            op = OP_CAT;

        /* closing braket */
        else if ( !strcmp( nw, OP_CL_BRAKET_STR ) )
            op = OP_CL_BRAKET;

        /* comparison operators */
        else if ( strenum( nw, "<|=|>", '|', STEN_CASE ) ) {

            STRARR opstr[3];
            int ch;

            /* determine whole comparison operator:
            ** take first word, and check for next
            ** single character, if it is one of
            ** "<", "=" or ">", too. if so, append
            ** it to the string that tells the
            ** operator.
            */
            strcpy( opstr, nw );
            ch = infgetc( inpf );
            if ( ch != EOF ) {

                D( fprintf( stderr, "%c", (char) ch ) );

                if ( strchr( "<=>", ch ) ) {

                    opstr[1] = ch;
                    opstr[2] = 0;

                } else
                    inungetc( ch, inpf );
            } else
                op_eof = TRUE;

            /* find out comparison operator */
            if ( !strcmp( nw, OP_EQ_STR ) )
                op = OP_EQ;
            else if ( !strcmp( nw, OP_NEQ_STR ) )
                op = OP_EQ;
            else if ( !strcmp( nw, OP_GT_STR ) )
                op = OP_GT;
            else if ( !strcmp( nw, OP_LT_STR ) )
                op = OP_LT;
            else if ( !strcmp( nw, OP_LTE_STR ) )
                op = OP_LTE;
            else if ( !strcmp( nw, OP_GTE_STR ) )
                op = OP_GTE;
            else if ( !strcmp( nw, OP_CEQ_STR ) )
                op = OP_CEQ;
            else
                err_op( inpf, opstr );

        } else 
            err_op( inpf, nw );

    } else
        op_eof = TRUE;

    DMSG( "\"\n" );


    if ( op_eof )
        err_eof( inpf, "operator expected" );

    return( op );
}

/*
** process_op
*/
VOID process_op( HSCVAR *dest, BYTE op, STRPTR str1, STRPTR str2 )
{
    EXPSTR *result = init_estr( 40 ); /* TODO: own size */

    D( fprintf( stderr, "**   \"%s\", \"%s\"\n", str1, str2 ) );
    if ( str2 && ( op != OP_NONE ) ) {

        BOOL bool_val1 = eval_boolstr( str1 );
        BOOL bool_val2 = eval_boolstr( str2 );

        switch ( op ) {

            case OP_AND:
                if ( bool_val1 && bool_val2 )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case OP_OR:
                if ( bool_val1 || bool_val2 )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case OP_XOR:
                if ( (bool_val1 || bool_val2)
                    && !( bool_val1 && bool_val2 )
                )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;



            case  OP_EQ:

                /* string comparison, ignore case */
                if ( !upstrcmp( str1, str2 ) )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_NEQ:

                /* string comparison "<>" */
                if ( upstrcmp( str1, str2 ) )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_GT:

                /* string comparison ">" */
                if ( upstrcmp( str1, str2 ) > 0 )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_LT:

                /* string comparison "<" */
                if ( upstrcmp( str1, str2 ) < 0)
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_GTE:

                /* string comparison ">=" */
                if ( upstrcmp( str1, str2 ) >= 0 )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_LTE:

                /* string comparison "<=" */
                if ( upstrcmp( str1, str2 ) <= 0)
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case  OP_CEQ:

                /* string comparison, case sensitive */
                if ( !strcmp( str1, str2 ) )
                    set_estr( result, VC_TRUE );
                else
                    set_estr( result, VC_FALSE );
                break;

            case OP_CAT:

                /* concat two expressions */
                set_estr( result, str1 );
                app_estr( result, str2 );

                break;

            default:
                panic( "empty operator" );
                break;

        }
    } 

    /* store result in destination attribute */
    set_vartext( dest, estr2str( result ) );

    /* remove temp. string for result */
    del_estr( result );
}


/*
**-------------------------------------
** eval_expression: evaluate expression
**-------------------------------------
*/

/*
** eval_string_expr
**
** evaluate string expression WITH enclosing quotes
*/
STRPTR eval_string_expr( HSCVAR *dest, INFILE *inpf )
{
    STRPTR  eval_result = NULL;
    EXPSTR *tmpstr      = init_estr( 80 ); /* TODO: size constant */
    int     quote;


    /* get quote char */
    quote = infgetc( inpf );
    if ( quote != EOF ) {

        BOOL end = FALSE;

        while ( !end ) {

            int ch = infgetc( inpf );
            if ( ch == EOF )
                err_eof( inpf, "reading string constant" );
            else if ( ch == CH_LF ) {

                message( MSG_STR_EOL, inpf );
                errstr( "string constant exeeds line" );
                errlf();

                eval_result = NULL;
                end = TRUE;

            } else if ( ch != quote ) {

                /* append next char to string */
                app_estrch( tmpstr, ch );

            } else {

                 /* closing quote reached */
                 eval_result = estr2str( tmpstr );
                 end = TRUE;

            }
        }

    } else
        err_eof( inpf, "reading string constant" );

    /* set new attribute value */
    if ( eval_result ) {

        /* set new quotes */
        dest->quote = quote;
        /* set new value */
        set_vartext( dest, eval_result );
        eval_result = get_vartext( dest );

    }

    /* remove temp. string */
    del_estr( tmpstr );

    return( eval_result );
}


/*
** eval_string_expr_noquote
**
** evaluate string expression WITHOUT enclosing quotes
*/
STRPTR eval_string_expr_noquote( HSCVAR *dest, INFILE *inpf )
{
    /* TODO: check for ch==">": attrval missing */
    STRPTR  eval_result = NULL;
    EXPSTR *tmpstr      = init_estr( 80 ); /* TODO: size constant */
    BOOL    end         = FALSE;

    /* TODO: check for empty expression */

    /*
    ** read next char from input file until a
    ** closing quote if found.
    ** if the arg had no quote, a white space
    ** or a '>' is used to detect end of arg.
    ** if a LF is found, view error message
    */
    while ( !end ) {

        int ch = infgetc( inpf );
        if ( ch == EOF ) {

            err_eof( inpf, "reading attribute" );

            end = TRUE;

        } else if ( ( ch==CH_LF )
                  || ( inf_isws(ch,inpf) || ( ch=='>' ) ) )
        {

            /* end of arg reached */
            inungetc( ch, inpf );
            eval_result = estr2str( tmpstr );
            end = TRUE;

        } else {

            /* append next char to tmpstr */
            app_estrch( tmpstr, ch );

        }
    }

    /* set new attribute value */
    if ( eval_result ) {

        set_vartext( dest, eval_result );
        eval_result = get_vartext( dest );

    }

    /* remove temp. string */
    del_estr( tmpstr );

    return( eval_result );
}

/*
** eval_attrref
**
** evaluate reference to attribute
**
*/
STRPTR eval_attrref( HSCVAR *destattr, INFILE *inpf )
{
    STRPTR eval_result = NULL;
    STRPTR nw = eval_attrname( inpf );

    if ( nw ) {

        HSCVAR *refvar = find_varname( vars, nw );

        if ( refvar ) {

            /* TODO: type checking */
            destattr->quote = refvar->quote;
            eval_result     = refvar->text;

            /* check empty/circular reference */
            if ( !eval_result ) {

                message( MSG_EMPTY_SYMB_REF, inpf );
                errstr( "empty reference to" );
                errsym( destattr->name );
                errlf();

            }

            /* debugging message */
            DDA( fprintf( stderr, "**    %s refers to (%s)\n",
                          destattr->name, refvar->name ) );

        } else {

            /* reference to unknown destattr */
            err_unkn_attr( inpf, nw );

        }

        /* set empty value for reference to NULL */
        if ( (!refvar) || (!eval_result ) ) {

            /* return empty destattr */
            destattr->quote = '"';
            eval_result     = "";

        }

    }

    /* set value of destination attribute */
    if ( eval_result )
        set_vartext( destattr, eval_result );

    return( eval_result );
}

/*
** eval_expression
**
** params: dest....attribute where to store result in
**         inpf....input file
**         err.....flag that is set to TRUE if an error occured
**         endstr..word that is expected at end of expession;
**                 NULL for no special ending word
** result: result of evaluation (IF_TRUE or FALSE)
**
** NOTE: if endstr==NULL, that means that eval_expression() is called
**   immediatly after the "=" of an attribute. In this case, if no
**   quotes are found as next char, all chars are read until the next
**   white-space or LF occures.
**   If no special char was found until now (only "A".."Z", "_", "-" and
**   digits occured), we first interpret the string as name for an
**   attribute reference; if such an attribute does not exist, it is
**   asumed that the value passed is a string constant (same as it would
**   have been enclosed in quotes).
**
*/
STRPTR eval_expression( HSCVAR *dest, INFILE *inpf, STRPTR endstr )
{
    /* TODO: endstr needs to be boolean flag only */

    EXPSTR *vararg = init_estr( 80 ); /* TODO: size constant */
        /* used as destination by eval_string_exprXX() */

    STRPTR exprstr = NULL;          /* return value */
    int    ch;                         /* char read from input */

    /* skip white spaces */
    infskip_ws( inpf );

#if 0
    /* disable log */
    inflog_disable( inpf );
#endif

    /* read dest->quote char */
    ch = infgetc( inpf );
    if ( !strchr( VQ_STR_QUOTE, ch ) )
        if ( ch != EOF )
            dest->quote = VQ_NO_QUOTE;
        else
            err_eof( inpf, "reading attribute" );
    else
        dest->quote = ch;

    if ( ch == '(' ) {

        /* process braket */
        exprstr = eval_expression( dest, inpf, ")" );

        /* set generic double quote */
        if ( !endstr )
            dest->quote = '\"';

    } else if ( ch != EOF ) {

        /* write current char read back to input buffer */
        inungetc( ch, inpf );

        if ( dest->quote != VQ_NO_QUOTE )

            /* process string expression with quotes */
            exprstr = eval_string_expr( dest, inpf );

        else if ( endstr  ) {

            BOOL err = FALSE;

            /* try to process unary operator */
            exprstr = try_eval_unary_op( dest, inpf, &err );

            /* process attribute reference */
            if ( !exprstr && !err )
                exprstr = eval_attrref( dest, inpf );

        } else {

            /* process string expression without quotes */
            exprstr = eval_string_expr_noquote( dest, inpf );

        }

    }

    if ( exprstr && endstr ) {

        BYTE op = eval_op( inpf );

        if ( op == OP_CL_BRAKET )
            DMSG( "  END mark operator reached" );
        else if ( op != OP_NONE ) {

            /* no endmark reached */
            STRPTR  str1  = exprstr;

            /* read second operator */
            if ( op != OP_NONE ) {

                STRPTR  str2 = NULL;
                HSCVAR *dest1 = new_hscvar( "dummy");

                /* init dummy attribute */
                dest1->vartype = dest->vartype;
                dest1->quote   = dest->quote;

                /* compute second value */
                str2 = eval_expression( dest1, inpf, endstr );

                if ( str2 )
                    process_op( dest, op, str1, str2 );
                else
                    exprstr = NULL;

                /* remove result of second value */
                del_var( (APTR) dest1 );

            } else {

                /* TODO: skip expression until ">"*/
                exprstr = NULL;
            }

            if ( exprstr ) {

                /* store result */
                exprstr = get_vartext( dest );

            }

        } else
            DMSG( "  NO operator" );

    }

    if ( !endstr )
        if ( exprstr && !endstr) {

            /*
            ** check enum type
            */
            if (dest->vartype == VT_ENUM) {

                if ( !strenum( exprstr, dest->enumstr, '|', STEN_NOCASE ) ) {

                    /* unknown enum value */
                    message( MSG_ENUM_UNKN, inpf );
                    errstr( "unknown" );
                    errstr( " value " );
                    errqstr( exprstr );
                    errstr( " for enumerator" );
                    errsym( dest->name );
                    errlf();

                }
            }
            /*
            ** parse uri (only if no macro-attr)
            ** (convert abs.uris, check existence)
            */
            else if (dest->vartype == VT_URI )

                if ( !(dest->varflag & VF_MACRO) )
                    set_vartext( dest, parse_uri( exprstr, inpf ) );
                else {

                    DDA( fprintf( stderr, "**    didn't parse uri \"%s\"\n",
                                  exprstr ) );

                }
            /*
            ** for boolean attributes, set the name
            ** of the attribute if TRUE, or set an
            ** empty string, if FALSE
            */
            else if ( dest->vartype == VT_BOOL )

                if ( eval_boolstr( exprstr ) )
                    set_vartext( dest, dest->name );
                else
                    set_vartext( dest, "" );

#if 0
            /* update and enable log */
            if ( !fatal_error && !endstr ) {

                if ( dest->quote != VQ_NO_QUOTE )
                    inflog_app( inpf, ch2str( dest->quote ) );     /* append quote */
                inflog_app( inpf, exprstr );                       /* append arg */
                if ( dest->quote != VQ_NO_QUOTE )
                    inflog_app( inpf, ch2str( dest->quote ) );     /* append quote */
                inflog_enable( inpf );                             /* enable log */

            }
#endif

            exprstr = get_vartext( dest );

        } else {

            /* if error occured,
            ** skip until ">", unread ">"
            */
            skip_until_eot( inpf, NULL );
            if ( !fatal_error )
                inungetcw( inpf );

        }

    del_estr( vararg );

    return ( exprstr );
}

