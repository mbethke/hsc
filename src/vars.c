/*
** vars.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** hsc-variable funcs for hsc
**
** updated: 10-Dec-1995
** created:  2-Sep-1995
**
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/memory.h"
#include "ugly/infile.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "tagargs.h"
#include "uri.h"

#define NOEXTERN_HSC_VARS
#include "vars.h"

#include "eval.h"

/*
** global vars
*/
DLLIST *vars = NULL;
EXPSTR *vararg = NULL;


/*
**-------------------------------------
** debugging functions
**-------------------------------------
*/

/*
** ptr_var
*/
void prt_var( FILE *stream, APTR data )
{
    HSCVAR *var = (HSCVAR*) data;

    if ( var ) {

        int varquote = var->quote;
        if ( varquote == VQ_NO_QUOTE )
            varquote = '.';

        /* name & type & macro_id */
        fprintf( stream, "%s (type:%d,mci:%d) ",
                 var->name, var->vartype, var->macro_id );
        /* text value */
        if ( var->text )
            fprintf( stream, "cur:%c%s%c",
                     var->quote, var->text, varquote );
        else
            fprintf( stream, "<NULL>" );
        fprintf( stream, " " );
        /* default text */
        if ( var->deftext )
            fprintf( stream, "def:%c%s%c",
                     var->quote, var->deftext, varquote );
        else
            fprintf( stream, "<NULL>" );
        fprintf( stream, "\n" );


    } else
        fprintf( stream, "<NULL>\n" );
}

void prt_varlist( DLLIST *varlist, STRPTR title )
{
    fprintf( stderr, "** %s\n", title );
    fprintf_dllist( stderr, varlist, prt_var );
}

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** del_var
*/
void del_var( APTR data )
{
    HSCVAR *var = (HSCVAR *)data;

    /* release mem */
    ufreestr( var->name );
    ufreestr( var->deftext );
    ufreestr( var->text );
    ufreestr( var->enumstr );

    var->macro_id = 0;
    var->varflag  = 0;
    var->vartype  = VT_NONE;
    var->quote    = EOF;

    ufree( var );

}

/*
** new_hscvar
**
** alloc & init a new variable
*/
HSCVAR *new_hscvar( STRPTR newname )
{

    HSCVAR *newvar = (HSCVAR*) umalloc( sizeof(HSCVAR) );

    if (newvar) {

        /* init new varument item */
        newvar->vartype   = VT_NONE;
        newvar->name      = strclone( newname );
        newvar->deftext   = NULL;
        newvar->text      = NULL;
        newvar->enumstr   = NULL;
        newvar->macro_id  = 0;
        newvar->varflag   = 0;
        newvar->quote     = EOF;

    }

    if ( !( newvar->name ) ) {

        del_var( (APTR) newvar );
        newvar = NULL;

    }

    return (newvar);

}



/*
** app_var
**
** append a new var to the var list AT BEGINNING
*/
HSCVAR *app_var( DLLIST *varlist, STRPTR newname )
{
    HSCVAR *var = new_hscvar( newname );
    BOOL    ok = FALSE;

    if ( var )
        ok = ( ins_dlnode( varlist, varlist->first, (APTR) var ) != NULL );

    if ( !ok ) {
        del_var( (APTR) var );
        var = NULL;
    }

    return ( var );
}


/*
**-------------------------------------
** search functions
**-------------------------------------
*/


/*
** cmp_varname
**
** compares a var-name with the name
** of a HSCVAR-entry
*/
int cmp_varname( APTR cmpstr, APTR vardata )
{
    STRPTR varstr = NULL;

    if ( vardata )
        varstr = ((HSCVAR*)vardata)->name;

    if (varstr)
        if ( !upstrcmp( cmpstr, varstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
** find_varname
*/
HSCVAR *find_varname( DLLIST *varlist, STRPTR name )
{
    DLNODE *nd = find_dlnode( varlist->first, (APTR) name, cmp_varname );
    HSCVAR *var = NULL;

    if ( nd )
        var = (HSCVAR*)nd->data;

    return ( var );
}


/*
**-------------------------------------
** misc. functions
**-------------------------------------
*/
BYTE str2vartype( STRPTR s )
{
    BYTE vartype = VT_NONE;

    if ( !upstrcmp( VT_STR_URI, s ) )
        vartype = VT_URI;
    else if ( !upstrcmp( VT_STR_STRING, s ) )
        vartype = VT_STRING;
    else if ( !upstrcmp( VT_STR_BOOL, s ) )
        vartype = VT_BOOL;
    else if ( !upstrcmp( VT_STR_NUM, s ) )
        vartype = VT_NUM;
    else if ( !upstrcmp( VT_STR_ENUM, s ) )
        vartype = VT_ENUM;
    else if ( !upstrcmp( VT_STR_ID, s ) )
        vartype = VT_ID;
    else if ( !upstrcmp( VT_STR_COLOR, s ) )
        vartype = VT_COLOR;

    return( vartype );
}

/*
** set_vartext
**
** set value of a var
**
** params: var......var to set
**         newtext..new text to set
** result: value of new text set
** errors: return NULL if out of mem, display error message
*/
STRPTR set_vartext( HSCVAR *var, STRPTR newtext )
{
    if ( newtext != var->text ) {

        ufreestr( var->text );
        var->text = NULL;
        if ( newtext )
            var->text = strclone( newtext );

    }

    return (var->text);
}


/*
** set_vartext_bool
**
** set value of a boolean attr
**
** params: var....var to set
**         value..new value to set
** result: value
** errors: if out of mem, display error message
*/
BOOL set_vartext_bool( HSCVAR *attr, BOOL value )
{
    if ( value )
        set_vartext( attr, attr->name );
    else
        set_vartext( attr, "" );

    return ( value );
}


/*
** clr_vartext
**
** clear or reset var to default text
**
** params: var..var to clear
** result: TRUE, if new value set correctly
*/
BOOL clr_vartext( HSCVAR *var )
{
    BOOL ok = TRUE;

    if ( var->deftext ) {
        if ( !(set_vartext( var, var->deftext )) )
            ok = FALSE;
    } else {

        ufreestr( var->text );
        var->text = NULL;

    }

    return( ok );
}

/*
** clr_attrdef
**
** clear attributes default text
**
** params: attr..attr to clear
*/
VOID clr_attrdef( HSCVAR *attr )
{
    ufreestr( attr->deftext );
    attr->deftext = NULL;
}

/*
** clr_varlist
**
** clear all vars to default text
**
** params: varlist..varlist to clear
*/
BOOL clr_varlist( DLLIST *varlist )
{
    DLNODE *nd = varlist->first;
    BOOL    ok = TRUE;

    while ( nd && ok ) {

        ok &= clr_vartext( (HSCVAR*) nd->data );
        nd = nd->next;
    }

    return( ok );
}

/*
** clr_varlist_bool
**
** set all "empty" (=NULL) boolean attributes of
** an attr list to FALSE
**
** params: varlist..varlist to process
*/
VOID clr_varlist_bool( DLLIST *varlist )
{
    DLNODE *nd = varlist->first;
    BOOL    ok = TRUE;

    while ( nd && ok ) {

        HSCVAR *attr = (HSCVAR*) nd->data;

        if ( ( attr->vartype == VT_BOOL ) && !( attr->text ) )
            set_vartext_bool( attr, FALSE );

        nd = nd->next;
    }
}

/*
** get_vartext_byname: get text value of a var
**
**
*/
STRPTR get_vartext_byname( DLLIST *varlist, STRPTR name )
{
    HSCVAR *var     = find_varname( varlist, name );
    STRPTR  vartext = NULL;

    if ( var )
        vartext = var->text;

    return( vartext );
}

/*
** get_vartext: get text value of a var
**
*/
STRPTR get_vartext( HSCVAR *var )
{
    STRPTR text = NULL;

    if ( var )
        text = var->text;

    return( text );
}

/*
** get_varbool: get valueof a boolean attr
*/
BOOL get_varbool( HSCVAR *attr )
{
    BOOL set = FALSE;
    if ( attr && ( attr->text[0] ) )
        set = TRUE;

    return( set );
}

/*
** get_varbool_byname: get value of a boolean var
*/
BOOL get_varbool_byname( DLLIST *varlist, STRPTR name )
{
    HSCVAR *var  = find_varname( varlist, name );

    return( get_varbool( var ) );
}


/*
** get_vardeftext: get default text value of a var
**
*/
STRPTR get_vardeftext( HSCVAR *var )
{
    STRPTR deftext = NULL;

    if ( var )
        deftext = var->deftext;

    return( deftext );
}


/*
** check_varlist: check for required attributes missing
**
** result: TRUE, if all attributes ok
*/
BOOL check_reqvar( HSCVAR *var, INFILE *inpf )
{
    BOOL ok = TRUE;

    if ( (var->varflag & VF_REQUIRED)
         && (!var->text) )
    {

        message( MSG_MISS_REQ_ATTR, inpf );
        errstr( "required" );
        errsym( var->name );
        errstr( "missing\n" );

        ok = FALSE;

    }

    return( ok );
}
BOOL check_varlist( DLLIST *varlist, INFILE *inpf )
{
    DLNODE *nd = varlist->first;
    BOOL    ok = TRUE;

    while ( nd ) {

        ok &= check_reqvar( (HSCVAR *) (nd->data), inpf );
        nd = nd->next;

    }

    return( ok );
}



/*
**-------------------------------------
** define a new var from input file
**-------------------------------------
*/

#if 0
/*
** parse_vararg: read & check a attribute value
*/
STRPTR parse_vararg( HSCVAR *var, INFILE *inpf )
{
    return( eval_expression( var, inpf, NULL ) );
}
#endif

/*
** read_enum_str
**
** sidefx: modifies tmpstr
*/
BOOL read_enum_str( HSCVAR *var, INFILE *inpf )
{
    BOOL ok;
    int  ch;

    ok = parse_wd( inpf, "(" );        /* check for "(" */
    ok &= clr_estr( tmpstr );          /* reset string */

    ch = infgetc( inpf );
    while ( ok && (ch!=')') && (ch!=EOF) && (ch!=CH_LF) ) {

        if ( (ch!=')') && (ch!=' ') )
            ok &= app_estrch( tmpstr, ch );
        ch = infgetc( inpf );

    }; 

    /* check result */
    if ( ok )
    if ( ch == EOF )
        err_eof( inpf, "reading enumerator" );
    else if ( ch ==CH_LF )
        err_eol( inpf );

    /* store enumstr in var-struct */
    if ( ok ) {

        DDA( fprintf( stderr, "**   enum: %s\n", estr2str( tmpstr ) ) );
        var->enumstr = strclone( estr2str( tmpstr ) );

    }


    return( (BOOL)(!fatal_error) );
}

/*
** check_attr_option
**
** check if a attribute-option-string is equal to an id/short id.
** if so, set the corresponding option value within the attribute.
**
** params: option..option string to check for (read from input)
**         attr....attribute to update option value for
**         id......id string of option (eg "REQUIRED")
**         sid.....short id string (eg "R")
**         value...option value to OR with old tag's option value
** result: TRUE, if tag's option value updated
*/
BOOL check_attr_option( STRPTR option, HSCVAR *attr, STRPTR id, STRPTR sid, ULONG value )
{
    BOOL found = FALSE;

    if ( !( (upstrcmp(option,id )) && (upstrcmp(option,sid )) ) ) {

        D( fprintf( stderr, "**   option %s\n", id ) );
        attr->varflag |= value;
        found = TRUE;

    }

    return( found );

}


/*
** define_var
**
** define a new var with reading its def from input file
** (starts parsing after ":", so ":" has to be read before)
**
** params: varname..name of new var
**         varlist..list new var should be inserted at the beginning
**         inpf.....input file where to read def from
**         flag.....flags: VF_ONLYONCE to avoid re-definition of a var
** result: ptr to new var
**
** definition syntax in input file:
**   <vartype>[/flag]["="<deftext value>]
**   legal vartypes: see VT_STR_xx in "vars.h"
**   legal flags   : see VF_STR_xx in "vars.h"
*/
HSCVAR *define_var( STRPTR varname, DLLIST *varlist, INFILE *inpf, UBYTE flag )
{
    HSCVAR *var        = NULL;                   /* result */
    BOOL    ok         = FALSE;
    BYTE   val_vartype = VT_NONE;                /* var-type (numeric) */
    BOOL   newattr     = FALSE;
    STRPTR nw          = infgetw( inpf );        /* next word read from input */

    /* read (optional) var type */
    if ( nw ) {
        if ( !strcmp( nw, ":" ) ) {

            nw = infgetw( inpf );
            if ( nw )
                val_vartype = str2vartype( nw );
            else
                err_eof( inpf, "defining attribute" );

        } else
            inungetcw( inpf );
    } else
        err_eof( inpf, "defining attribute" );

    /* look if attr already exist, */
    var = find_varname( varlist, varname );
    if ( !var ) {

        /* create new attr */
        DDA( fprintf( stderr, "** new attr: %s\n", varname ) );
        var = app_var( varlist, varname );

        if ( val_vartype == VT_NONE ) {

            /* TODO: error message "attr type expected" */

            /* asume generic attribute type "STRING" */
            val_vartype = VT_STRING;

        }

        /* set type */
        var->vartype = val_vartype;

        /* init enum-attribute */
        if ( var->vartype == VT_ENUM ) {

            var->varflag |= VF_NOQUOTE;
            read_enum_str( var, inpf );

        }

        newattr = TRUE;

    } else {

        /* check for illegal redefinance */
        if ( !( flag & VF_UPDATE ) ) {

            message( MSG_ATTR_REDEFINED, inpf );
            errstr( "redefinance of" );
            errsym( varname );
            errlf();

        }

        /* attr already exists: check for type consistence */
        if ( ( val_vartype != VT_NONE ) && ( val_vartype != var->vartype ) ) {

            /* TODO: error message attr type inconsistent */

        }
    }

    /* get next word */
    nw = infgetw( inpf );
    if ( !nw )
        err_eof( inpf, "defining attribute" );

    /*
    ** loop: handle flags and deftext value
    */
    while ( nw ) {

        if ( !strcmp( nw, "=" ) ) {

            /* get new deftext value */
            STRPTR new_deftext;

            if ( !(var->deftext) )
                new_deftext = eval_expression( var, inpf, NULL );
            else {

                STRPTR dummy;

                message( MSG_SYMB_2ND_DEFAULT, inpf  );
                errstr( "default value for" );
                errsym( var->name );
                errstr( "already set\n" );

                /* skip illegal default value */
                dummy = eval_expression( var, inpf, NULL );

            }

            /* store default text value */
            if ( new_deftext )
                var->deftext = strclone( new_deftext );

            /* check deftext value */
            if ( var->vartype == VT_BOOL ) {

                /* check boolean value */
                message( MSG_SYMB_BOOL_DEFAULT, inpf );
                errstr( "no default value for boolean" );
                errsym( var->name );
                errstr( "allowed\n" );

            } else if ( var->vartype == VT_NUM ) {

                /* TODO: test-set value with default value */
                /* check numeric value */
                LONG num;

                if ( sscanf( var->text, "%d", &num ) != strlen(var->text) ) {

                    ok = FALSE;
                    message( MSG_ILLEGAL_NUM, inpf );
                    errstr( "Illegal numeric value: " );
                    errstr( var->text );
                    errlf();

                }
            }

            /* clear boolean var */
            /* TODO: why set new bool to ""? */
            if ( var->vartype == VT_BOOL ) {

                var->quote = VQ_NO_QUOTE;
                var->deftext = strclone( "" );

            }

        } else if ( !strcmp( nw, "/" ) ) {

            /* set flag */
            nw = infgetw( inpf );
            if ( flag & VF_UPDATE ) {

                if ( nw ) {

                    BOOL ok = FALSE;

                    ok |= check_attr_option( nw, var,
                              VF_JERK_STR, VF_JERK_SHT, VF_JERK );
                    ok |= check_attr_option( nw, var,
                              VF_NOQUOTE_STR, VF_NOQUOTE_SHT, VF_NOQUOTE );
                    ok |= check_attr_option( nw, var,
                              VF_ONLYONCE_STR, VF_ONLYONCE_SHT, VF_ONLYONCE );
                    ok |= check_attr_option( nw, var,
                              VF_REQUIRED_STR, VF_REQUIRED_SHT, VF_REQUIRED );
                    if ( !ok ) {

                        message( MSG_UNKN_ATTR_OPTION, inpf );
                        errstr( "Unknown attribute option " );
                        errqstr( nw );
                        errlf();

                    }

                } else
                    err_eof( inpf, "defining attribute" );

            } else {

                /* TODO: error message "no attr flags when updating" */

            }
        } else {

            /* end of var definition reached */
            inungets( nw, inpf );
            nw = NULL;
            ok = TRUE;

        }

        /* get next word */
        if ( nw )
            nw = infgetw( inpf );

    } /* while(nw) */

    if ( !ok && var ) {

        del_dlnode( varlist, (APTR) var );
        var = NULL;
    }

    return (var);
}

/*
**-------------------------------------
** copy & remove local vars to/from
** global varlist
**-------------------------------------
*/

/*
** copy_local_var
**
** copies a local attribute to the global attribute list
**
** NOTE: the VF_MACRO-flag of the copy is enabled!
*/
HSCVAR *copy_local_var( DLLIST *destlist, HSCVAR *locvar, ULONG mci )
{
    HSCVAR *var = app_var( destlist, locvar->name );

    var->macro_id = mci;
    var->vartype = locvar->vartype;
    var->varflag = locvar->varflag & (~VF_MACRO) ; /* disable VF_MACRO */
    set_vartext( var, locvar->text );
    var->quote = locvar->quote;

    return( var );
}

/*
** copy_local_vars
**
** add all local attributes of a macro to the global
** attribute list.
**
*/
BOOL copy_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci )
{
    BOOL ok = TRUE;

    if ( mci == MCI_ERROR )
        panic( "mci=MCI_ERROR" );
    else {

        DLNODE *nd = varlist->first;
        HSCVAR *var;

        while ( nd && ok ) {

            var =  copy_local_var( destlist, (HSCVAR*)(nd->data), mci );
            ok  &= (BOOL)(var!=NULL);
            nd  =  nd->next;

        }
    }

    return( ok );
}

/*
** set_local_var
**
** copies a local attribute to the global attribute list
**
** NOTE: the VF_MACRO-flag of the set is enabled!
*/
HSCVAR *set_local_var( DLLIST *destlist, HSCVAR *locvar, ULONG mci )
{
    HSCVAR *var = find_varname( destlist, locvar->name );

    if ( var ) {

        var->macro_id = mci;
        var->vartype = locvar->vartype;
        set_vartext( var, locvar->text );

    } else
        panic( "set_local_var to UNKNOWN ATTR" );

    return( var );
}

/*
** set_local_vars
**
** add all local attributes of a macro to the global
** attribute list.
**
*/
BOOL set_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci )
{
    BOOL ok = TRUE;

    if ( mci == MCI_ERROR )
        panic( "mci=MCI_ERROR" );
    else {

        DLNODE *nd = varlist->first;
        HSCVAR *var;

        while ( nd && ok ) {

            var =  set_local_var( destlist, (HSCVAR*)(nd->data), mci );
            ok  &= (BOOL)(var!=NULL);
            nd  =  nd->next;

        }
    }

    return( ok );
}

/*
** remove_local_varlist
*/
void remove_local_varlist( DLLIST *varlist, ULONG mci )
{
    DLNODE *nd = varlist->first;

    while ( nd ) {

        HSCVAR *var    = (HSCVAR *) nd->data;    /* var data of node */
        DLNODE *nd_nxt = nd->next;               /* next node */

        if ( var->macro_id == mci ) {

            DDA( fprintf( stderr, "**    del %s\n", var->name ) );
            del_dlnode( varlist, nd );

        } else
            DDA( fprintf( stderr, "**    skip %s\n", var->name ) );

        nd = nd_nxt;

    }

}

