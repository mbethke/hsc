/*
** vars.c
**
** hsc-variable funcs for hsc
**
** updated:  5-Sep-1995
** created:  2-Sep-1995
**
** TODO:
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/memory.h"
#include "ugly/infile.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "tagargs.h"

#define NOEXTERN_HSC_VARS
#include "vars.h"

/*
** TODO:
**
*/


/*
** global vars
*/
DLLIST *vars = NULL;
char vararg[ MAX_ARGLEN+2 ];

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
        fprintf( stream, "%s (%d,%d) ",
                 var->name, var->vartype, var->macro_id );
        /* text value */
        if ( var->text )
            fprintf( stream, "%c%s%c",
                     var->quote, var->text, varquote );
        else
            fprintf( stream, "<NULL>" );
        fprintf( stream, " " );
        /* default text */
        if ( var->deftext )
            fprintf( stream, "%c%s%c",
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

    HSCVAR *newvar = (HSCVAR*) malloc( sizeof(HSCVAR) );

    if (newvar) {

        /* init new varument item */
        newvar->vartype   = VT_NONE;
        newvar->name      = strclone( newname );
        newvar->deftext   = NULL;
        newvar->text      = NULL;
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
    else if ( !upstrcmp( VT_STR_TEXT, s ) )
        vartype = VT_TEXT;

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
    ufreestr( var->text );
    if ( newtext ) {

        var->text = strclone( newtext );
        if ( !var->text )
            err_mem( NULL );

    }

    return (var->text);
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

    if ( var->deftext )
        ok = (BOOL)( set_vartext( var, var->deftext ) );
    return( ok );
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
**-------------------------------------
** define a new var from input file
**-------------------------------------
*/
STRPTR parse_vararg( HSCVAR *var, INFILE *inpf )
{
    STRPTR str_vararg = NULL;

    infskip_ws( inpf );

    /* TODO: handle "<>" (reset var->text to NULL) */

    /* read var->quote char */
    var->quote = infgetc( inpf );
    if ( !strchr( VQ_STR_QUOTE, var->quote ) )
        if ( var->quote != EOF )
            var->quote = VQ_NO_QUOTE;
        else
            err_eof( inpf );

    /* read arg string */
    if ( var->quote == '<' ) {

        /*
        ** get arg from other var
        */
        STRPTR nw = infgetw( inpf );

        if ( nw ) {

            HSCVAR *refvar = find_varname( vars, nw );

            if ( refvar ) {

                /* todo: type checking */
                var->quote = refvar->quote;
                str_vararg = refvar->text;

                /* check empty/circular reference */
                if ( !str_vararg ) {

                    message( VAR_CIRCULAR, inpf );
                    errstr( "Empty reference to" );
                    errsym( var->name );
                    errlf();

                }

                /* debugging message */
                if ( debug )
                    fprintf( stderr, "**   <%s> refers to <%s>\n",
                             var->name, refvar->name );
            } else {

                /* reference to unknown var */
                message( VAR_UNKN, inpf );
                errstr( "Unknown" );
                errsym( nw );
                errlf();

            }

            if ( (!refvar) || (!str_vararg ) ) {

                /* return empty var */
                var->quote = '"';
                str_vararg = "";
            }

            parse_gt( inpf );

        } else
            err_eof( inpf );

    } else if ( var->quote != EOF ) {

        /*
        ** get arg from input file
        */
        size_t ctr = 0;
        BOOL   end = FALSE;

        while ( !end && (ctr<MAX_ARGLEN) ) {

            int ch = infgetc( inpf );

            end = TRUE;

            if ( ch == EOF )
                err_eof( inpf );
            else if ( (ch==var->quote)
                      || ( ch==CH_LF )
                      || ( inf_isws(ch,inpf) && (ch==VQ_NO_QUOTE) )
                    )
            {

                str_vararg = vararg;
                if ( (ch==CH_LF) && (var->quote!=VQ_NO_QUOTE) )
                    err_streol( inpf );

            } else {

                /* remember char, inc counter */
                vararg[ctr]   = (char) ch;
                vararg[ctr+1] = 0;
                ctr++;
                end = FALSE; /* continue loop */

            }

        }

        if ( !end && (ctr==MAX_ARGLEN) )
            err_longstr( inpf );

    }

    return ( str_vararg );
}

/*
** define_var
**
** define a new var with reading its def from input file
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
    STRPTR str_vartype = infgetw( inpf );        /* var-type (string) */
    BYTE   val_vartype = VT_NONE;                /* var-type (numeric) */

    /* evaluate var type */
    if ( str_vartype )
        val_vartype = str2vartype( str_vartype );
    else
        err_eof( inpf );

    if ( val_vartype == VT_NONE ) {

        /* illegal var type */
        message( VAR_ILLEGAL_TYPE, inpf );
        errstr( "Illegal variable type \"" );
        errstr( str_vartype );
        errstr( "\"\n" );

    } else {

        /* look if var already exist, */
        /* create new var if neccessary */
        var = find_varname( varlist, varname );
        if ( !var ) {

            if ( debug )
                fprintf( stderr, "** new var: %s\n", varname );
            var = app_var( varlist, varname );

        }

        if ( !var )
            err_mem( inpf );
        else {

            STRPTR nw;                 /* next word from input file */

            /* set vartype */
            var->vartype = val_vartype;

            /* get next word */
            nw = infgetw( inpf );
            if ( !nw )
                err_eof( inpf );


            /*
            ** loop: handle flags and deftext value
            */
            while ( nw ) {

                if ( !strcmp( nw, "=" ) ) {

                    /* get new deftext value */
                    STRPTR new_deftext;

                    if ( !(var->deftext) )
                        new_deftext = parse_vararg( var, inpf );
                    else {

                        message( VAR_2ND_DEFAULT, inpf  );
                        errstr( "Default value for" );
                        errsym( var->name );
                        errstr( "already set\n" );

                    }

                    /* store default text value */
                    if ( new_deftext )
                        var->deftext = strclone( new_deftext );

                    /* check deftext value */
                    if ( var->deftext && clr_vartext( var ) ) {

                        if ( var->vartype == VT_BOOL ) {

                            /* check boolean value */
                            message( VAR_BOOL_DEFAULT, inpf );
                            errstr( "No default value for boolean" );
                            errsym( var->name );
                            errstr( "allowed\n" );

                        } else if ( var->vartype == VT_NUM ) {

                            /* check numeric value */
                            LONG num;

                            if ( sscanf( var->text, "%d", &num ) != strlen(var->text) ) {

                                ok = FALSE;
                                message( VAR_ILLEGAL_NUM, inpf );
                                errstr( "Illegal numeric value: " );
                                errstr( var->text );
                                errlf();

                            }
                        }
                    } else
                        err_mem( inpf );

                    /* clear boolean var */
                    if ( var->vartype == VT_BOOL ) {

                        var->quote = VQ_NO_QUOTE;
                        var->deftext = strclone( "" );
                        if ( !var->deftext )
                            err_mem( inpf );

                    }

                } else if ( !strcmp( nw, "/" ) ) {

                    /* set flag */
                    nw = infgetw( inpf );
                    if ( nw ) {

                        /* todo: set flag */

                    } else
                        err_eof( inpf );

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
        } /* if(!var) */
    } /* if(val_vartype..) */

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

/* copy_local_var */
HSCVAR *copy_local_var( HSCVAR *locvar, ULONG mci )
{
    HSCVAR *var = app_var( vars, locvar->name );

    if ( var ) {

        var->macro_id = mci;
        set_vartext( var, locvar->text );
        var->quote = locvar->quote;

    } else
        err_mem( NULL );

    return( var );
}

/*
** copy_local_vars
*/
BOOL copy_local_varlist( DLLIST *varlist, ULONG mci )
{
    BOOL ok = TRUE;

    if ( mci == MCI_ERROR )
        panic( "mci=MCI_ERROR" );
    else {

        DLNODE *nd = varlist->first;
        HSCVAR *var;

        while ( nd && ok ) {

            var =  copy_local_var( (HSCVAR*)(nd->data), mci );
            ok  &= (BOOL)(var!=NULL);
            nd  =  nd->next;

        }
    }

    return( ok );
}

/*
** remove_local_varlist
*/
void remove_local_varlist( ULONG mci )
{
    DLNODE *nd = vars->first;

    while ( nd ) {

        HSCVAR *var    = (HSCVAR *) nd->data;    /* var data of node */
        DLNODE *nd_nxt = nd->next;               /* next node */

        if ( var->macro_id == mci ) {

            if ( debug )
                fprintf( stderr, "**   del %s\n", var->name );
            del_dlnode( vars, nd );

        } else if ( debug )
            fprintf( stderr, "**   skip %s\n", var->name );

        nd = nd_nxt;

    }

}
