/*
** vars.c
**
** hsc-variable funcs for hsc
**
** updated: 27-Sep-1995
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

/*
** TODO:
**
*/


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

    HSCVAR *newvar = (HSCVAR*) malloc( sizeof(HSCVAR) );

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
** get_vartext: get text value of a var
**
**
*/
STRPTR get_vartext( DLLIST *varlist, STRPTR name )
{
    HSCVAR *var     = find_varname( varlist, name );
    STRPTR  vartext = NULL;

    if ( var )
        vartext = var->text;

    return( vartext );
}

/*
** get_varbool: get value of a boolean var
*/
BOOL get_varbool( DLLIST *varlist, STRPTR name )
{
    BOOL set = FALSE;
    HSCVAR *var  = find_varname( varlist, name );

    if ( var && var->text )
        set = TRUE;

    return( set );
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

/*
** check_enumstr: check if a given value is a legal enum value
*/
BOOL check_enumstr( HSCVAR *var, STRPTR value, INFILE *inpf )
{
    STRPTR enumcp = strclone( var->enumstr ); /* clone of enumstr */
    BOOL   found  = FALSE; /* flag: TRUE, if value found within enumstr */
    BOOL   any    = FALSE; /* flag: TRUE, if "*" was within enumstr */

    if ( enumcp ) {

        STRPTR word = strtok( enumcp, "|" );

        /* search for value in enumcp */
        while ( word && !found ) {

            if ( !strcmp( word, "*" ) )
                any = TRUE;
            if ( !upstrcmp( word, value ) )
                found = TRUE;
            word = strtok( NULL, "|" );

        }
        ufreestr( enumcp );

        /* check result, display messages if neccessary */
        if ( !found ) {

            if ( !any ) {

                /* unknown enum value */
                message( MSG_ENUM_UNKN, inpf );
                errstr( "unknown" );

            } else {

                /* suspicious enum value */
                message( MSG_ENUM_SUSPICIOUS, inpf );
                errstr( "suspicious" );

            }
            errstr( " value " );
            errqstr( value );
            errstr( " for" );
            errsym( var->name );
            errlf();

        } else DDA( fprintf( stderr, "**   enum \"%s\" ok for %s\n",
                             value, var->name ) );
    } else
        err_mem( inpf );

    return( found );
}


/*
** parse_vararg: read & check a attribute value
*/
STRPTR parse_vararg( HSCVAR *var, INFILE *inpf )
{
    STRPTR str_vararg = NULL;          /* return value */
    int    ch;                         /* char read from input */

    /* TODO: handle "<>" (reset var->text to NULL) */

    infskip_ws( inpf );

    /* disable log */
    inflog_disable( inpf );

    /* read var->quote char */
    ch = infgetc( inpf );
    if ( !strchr( VQ_STR_QUOTE, ch ) )
        if ( ch != EOF )
            var->quote = VQ_NO_QUOTE;
        else
            err_eof( inpf );
    else
        var->quote = ch;

    /* warning if no quote */
    if ( ( var->quote == VQ_NO_QUOTE )
         && !( var->varflag & VF_NOQUOTE ) )
    {

        message( MSG_ARG_NO_QUOTE, inpf );
        errstr( "Argument without quote\n" );

    }

    /* read arg string */
    if ( var->quote == '<' ) {

        /*
        ** get arg from other var
        */
        STRPTR nw = infgetw( inpf );

        if ( nw ) {

            HSCVAR *refvar = find_varname( vars, nw );

            if ( refvar ) {

                /* TODO: type checking */
                var->quote = refvar->quote;
                str_vararg = refvar->text;

                /* check empty/circular reference */
                if ( !str_vararg ) {

                    message( MSG_EMPTY_SYMB_REF, inpf );
                    errstr( "Empty reference to" );
                    errsym( var->name );
                    errlf();

                }

                /* debugging message */
                DDA( fprintf( stderr, "**    %s refers to <%s>\n",
                              var->name, refvar->name ) );

            } else {

                /* reference to unknown var */
                message( MSG_UNKN_SYMB_REF, inpf );
                errstr( "reference to unknown" );
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
        BOOL   end = FALSE;

        /* clear vararg or set with first char read */
        if ( var->quote == VQ_NO_QUOTE )
            end = !set_estr( vararg, ch2str( ch ) );
        else
            end = !clr_estr( vararg );
        if ( end )
            err_mem( inpf );

        /*
        ** read next char from input file until a
        ** closing quote if found.
        ** if the arg had no quote, a white space
        ** or a '>' is used to detect end of arg.
        ** if a LF is found, view error message
        */
        while ( !end ) {

            ch = infgetc( inpf );

            end = TRUE;

            if ( ch == EOF )
                err_eof( inpf );
            else if ( (ch==var->quote)
                      || ( ch==CH_LF )
                      || ( (var->quote==VQ_NO_QUOTE)
                           && ( inf_isws(ch,inpf) || ( ch=='>' ) ) )
                    )
            {

                /* end of arg reached */
                str_vararg = estr2str( vararg );
                if ( var->quote == VQ_NO_QUOTE ) {

                    if ( ch==CH_LF )
                        err_streol( inpf );
                    inungetc( ch, inpf );

                }

            } else {

                /* append next char to vararg */
                if ( !app_estrch( vararg, ch ) )
                    err_mem( inpf );
                else
                    end = FALSE; /* continue loop */

            }
        }
    }

    if ( str_vararg && var )
        /*
        ** check enum type
        */
        if (var->vartype == VT_ENUM)
            check_enumstr( var, str_vararg, inpf );
        /*
        ** parse uri (only if no macro-attr)
        ** (convert abs.uris, check existence)
        */
        else if (var->vartype == VT_URI )

            if ( !(var->varflag & VF_MACRO) )
                str_vararg = parse_uri( str_vararg, inpf );
            else {

                DDA( fprintf( stderr, "**   didn't parse uri \"%s\"\n",
                              str_vararg ) );

            }

    /* update and enable log */
    if ( !fatal_error ) {

        BOOL ok = TRUE;

        if ( var->quote != VQ_NO_QUOTE )                   
            ok &= inflog_app( inpf, ch2str( var->quote ) );/* append quote */
        inflog_app( inpf, str_vararg );                    /* append arg */
        if ( var->quote != VQ_NO_QUOTE )
            ok &= inflog_app( inpf, ch2str( var->quote ) );/* append quote */
        inflog_enable( inpf );                             /* enable log */

        if ( !ok )
            err_mem( NULL );
    }

    return ( str_vararg );
}

/*
** read_enum_str
**
** sidefx: modifies tmpstr
*/
BOOL read_enum_str( HSCVAR *var, INFILE *inpf )
{
    BOOL ok;
    int  ch;

    ok = parse_ch( inpf, '(' );        /* check for ")" */
    ok &= clr_estr( tmpstr );          /* reset string */

    ch = infgetc( inpf );
    while ( ok && (ch!=')') && (ch!=EOF) && (ch!=CH_LF) ) {

        if ( (ch!=')') && (ch!=' ') )
            ok &= app_estrch( tmpstr, ch );
        ch = infgetc( inpf );

    }; 

    /* check result */
    if ( !ok )
        err_mem( inpf );
    else if ( ch == EOF )
        err_eof( inpf );
    else if ( ch ==CH_LF )
        err_eol( inpf );

    /* store enumstr in var-struct */
    if ( ok ) {

        DDA( fprintf( stderr, "**   enum: %s\n", estr2str( tmpstr ) ) );
        var->enumstr = strclone( estr2str( tmpstr ) );
        if ( !(var->enumstr) )
            err_mem( inpf );
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
    STRPTR str_vartype = infgetw( inpf );        /* var-type (string) */
    BYTE   val_vartype = VT_NONE;                /* var-type (numeric) */

    /* evaluate var type */
    if ( str_vartype )
        val_vartype = str2vartype( str_vartype );
    else
        err_eof( inpf );

    if ( val_vartype == VT_NONE ) {

        /* illegal var type */
        message( MSG_ILLEGAL_SYMB_TYPE, inpf );
        errstr( "Illegal variable type \"" );
        errstr( str_vartype );
        errstr( "\"\n" );

    } else {

        /* look if var already exist, */
        /* create new var if neccessary */
        var = find_varname( varlist, varname );
        if ( !var ) {

            DDA( fprintf( stderr, "** new var: %s\n", varname ) );
            var = app_var( varlist, varname );

        }

        if ( !var )
            err_mem( inpf );
        else {

            STRPTR nw;                 /* next word from input file */

            /* set vartype */
            var->vartype = val_vartype;

            /* init enum-attribute */
            if ( var->vartype == VT_ENUM ) {

                var->varflag |= VF_NOQUOTE;
                read_enum_str( var, inpf );

            }

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

                        message( MSG_SYMB_2ND_DEFAULT, inpf  );
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
                            message( MSG_SYMB_BOOL_DEFAULT, inpf );
                            errstr( "No default value for boolean" );
                            errsym( var->name );
                            errstr( "allowed\n" );

                        } else if ( var->vartype == VT_NUM ) {

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

                        BOOL ok = FALSE;

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

/*
** copy_local_var
**
** copies a local attribute to the global attribute list
**
** NOTE: the VF_MACRO-flag of the copy is enabled!
*/
HSCVAR *copy_local_var( HSCVAR *locvar, ULONG mci )
{
    HSCVAR *var = app_var( vars, locvar->name );

    if ( var ) {

        var->macro_id = mci;
        var->vartype = locvar->vartype;
        var->varflag = locvar->varflag & (~VF_MACRO) ; /* disable VF_MACRO */
        set_vartext( var, locvar->text );
        var->quote = locvar->quote;

    } else
        err_mem( NULL );

    return( var );
}

/*
** copy_local_vars
**
** add all local attributes of a macro to the global
** attribute list.
**
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

        /* TODO: out of mem message? */
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

            DDA( fprintf( stderr, "**   del %s\n", var->name ) );
            del_dlnode( vars, nd );

        } else
            DDA( fprintf( stderr, "**   skip %s\n", var->name ) );

        nd = nd_nxt;

    }

}

