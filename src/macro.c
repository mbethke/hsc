/*
** macro.c
**
** hsc-variable funcs for hsc
**
** updated:  5-Sep-1995
** created:  3-Sep-1995
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
#include "ugly/fname.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "tagargs.h"

#include "vars.h"
#include "find.h"

#define NOEXTERN_HSC_MACRO
#include "macro.h"

/*
** TODO:
**
*/

/*
** global vars
*/
DLLIST *macros = NULL;                 /* list for macros */
DLLIST *macarg = NULL;                 /* list for macro arguments */

ULONG macro_call_id = 0;

/* local var */

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** del_mac
*/
void del_mac( APTR data )
{
    HSCMAC *mac = (HSCMAC *)data;

    /* remove temp. files */
    if ( mac->op_fname && mac->op_fname[0] && (!debug))
        remove( mac->op_fname );
    if ( mac->cl_fname && mac->cl_fname[0] && (!debug))
        remove( mac->cl_fname );

    /* release mem */
    ufreestr( mac->name );
    ufreestr( mac->op_fname );
    ufreestr( mac->cl_fname );
    del_dllist( mac->op_args );
    del_dllist( mac->cl_args );

    ufree( mac );

}

/*
** new_hscmac
**
** alloc & init a new hsc-macro
*/
HSCMAC *new_hscmac( STRPTR newname )
{

    HSCMAC *newmac = (HSCMAC*) malloc( sizeof(HSCMAC) );

    if (newmac) {

        /* init new macity item */
        newmac->name     = strclone(newname);
        newmac->op_fname = tmpnamstr();
        newmac->cl_fname = tmpnamstr();
        newmac->op_args  = init_dllist( del_var );
        newmac->cl_args  = init_dllist( del_var );

        if ( !( newmac->name && newmac->op_fname && newmac->cl_fname
                && newmac->op_args && newmac->cl_args ) )
        {
            del_mac( newmac );
            newmac = NULL;
        }
    }

    return(newmac);

}

/*
** app_macro
**
** append a new macro to the macro list
*/
HSCMAC *app_macro( STRPTR newname )
{
    HSCMAC *macro = new_hscmac( newname );
    BOOL    ok = FALSE;

    if ( macro )
        ok = ( app_dlnode( macros, (APTR) macro ) != NULL );

    if ( !ok ) {
        del_mac( (APTR) macro );
        macro = NULL;
    }

    return( macro );
}



/*
**-------------------------------------
** search functions
**-------------------------------------
*/

/*
** cmp_strmac
**
** compares a macro-string with the name
** of a HSCTAG-entry
*/
int cmp_strmac( APTR cmpstr, APTR macdata )
{
    STRPTR macstr = NULL;

    if ( macdata )
        macstr = ((HSCMAC*)macdata)->name;

    if (macstr)
        if ( !upstrcmp( cmpstr, macstr ) )
            return(-1);
        else
            return(0);
    else
        return(0);
}

/*
** find_strmac
*/
HSCMAC *find_strmac( STRPTR name )
{
    DLNODE *nd = find_dlnode( macros->first, (APTR) name, cmp_strmac );
    HSCMAC *macro = NULL;

    if ( nd )
        macro = (HSCMAC*)nd->data;

    return( macro );
}


/*
**-------------------------------------
** misc. functions
**-------------------------------------
*/


/*
**-------------------------------------
** define a new macro from input file
**-------------------------------------
*/

/*
** get_macro_name
**
*/
HSCMAC *def_macro_name( INFILE *inpf, BOOL *open_mac )
{
    STRPTR  nw = infgetw( inpf );
    HSCMAC *macro = NULL;
    HSCTAG *tag;

    /* create new macro */
    if ( nw ) {

        *open_mac = (BOOL)(strcmp( nw, "/" ));
        if ( !*open_mac ) {

            /* add closing macro */
            nw = infgetw( inpf );
            if ( nw ) {

                macro = find_strmac( nw );
                if ( macro ) {

                    tag = find_strtag( nw );
                    if ( tag )
                        tag->option |= HT_CLOSE;
                    else
                        panic( "no tag for macro" );

                } else {

                }


            } else
                err_eof( inpf );

        } else {

            /* create a new opening macro */
            macro = app_macro( nw );
        }
    } else
        err_eof( inpf );

    /* convert macro name to upper  case */
    if ( macro )
        upstr( macro->name );

    return( macro );
}

/*
** parse_macro_option
*/
BOOL parse_macro_option( STRPTR option, HSCMAC *macro, INFILE *inpf )
{
    BOOL ok = FALSE;

    return( ok );
}

/*
** parse_macro_var
*/
BOOL parse_macro_var( STRPTR varname, HSCMAC *macro, INFILE *inpf, BOOL open_mac )
{
    BOOL ok = FALSE;
    DLLIST *varlist;
    HSCVAR *var;

    /* select var list to add new var to */
    if ( open_mac )
        varlist = macro->op_args;
    else
        varlist = macro->cl_args;

    var = define_var( varname, varlist, inpf, 0 );

    return( ok );
}

/*
** def_macro_args
**
*/
BOOL def_macro_args( HSCMAC *macro, INFILE *inpf, BOOL *open_mac )
{
    BOOL    ok = FALSE;
    STRPTR  nw;

    if ( macro ) {

        /* read args */
        nw = infgetw( inpf );
        if ( strcmp( nw, ">" ) ) {

            while ( nw ) {

                if ( !nw )
                    err_eof( inpf );
                if ( !strcmp( nw, ">" ) ) {

                    nw = NULL;
                    ok = TRUE;

                } else  {

                    STRPTR pw = strclone( nw );

                    if ( pw ) {

                        nw = infgetw( inpf );
                        if ( !strcmp( nw, "=" ) )
                            /* define new macro var */
                            parse_macro_var( pw, macro, inpf, *open_mac );
                        else
                            /* set new macro option */
                            parse_macro_option( pw, macro, inpf );
                        ufreestr( pw );

                    } else
                        err_mem( inpf );

                    nw = infgetw( inpf );

                }
            }

            if ( !(ok || fatal_error) )
                err_eof( inpf );

            if ( !ok ) {
                del_dlnode(
                    macros,
                    find_dlnode( macros->first, (APTR) macro->name, cmp_strmac )
                );
                macro = NULL;
            }
        } else
            ok = TRUE;
    }

    return( ok );
}

/*
** set_macro_arg
**
** parse & set one single macro argument
**
*/
BOOL set_macro_arg( STRPTR varname, DLLIST *varlist, INFILE *inpf )
{
    HSCVAR *var = find_varname( varlist, varname );
    STRPTR  arg = NULL;
    BOOL    ok  = FALSE;
    STRPTR  nw;

    if ( var ) {

        var = find_varname( varlist, varname );
        if ( debug )
            fprintf( stderr, "**    set var %s", varname );

        /* get argument */
        nw  = infgetw( inpf );
        if ( !strcmp( nw, "=" ) ) {

            arg = parse_vararg( var, inpf );
            if ( arg ) {

                if ( set_vartext( var, arg ) ) {

                    if ( debug )
                        fprintf( stderr, "=\"%s\"\n", arg );
                    ok = TRUE;

                }

            }

        } else {

            arg = NULL;
            inungets( nw, inpf );
            ok = TRUE;

        }

        if ( arg ) {

            /* todo: bool var doesn't require arg */

        } else {

            if ( var->vartype == VT_BOOL ) {

                /* set boolean var */
                if ( debug )
                    fprintf( stderr, " (bool)\n", var->name );
                set_vartext( var, var->name );
                var->quote = VQ_NO_QUOTE;

            } else {

                /* todo:non-bool var requires arg */

            }
        }


    } else {

        /* reference to unknown var */
        message( VAR_UNKN, inpf );
        errstr( "Unknown" );
        errsym( varname );
        errlf();

    }
    return( ok );

}


/*
** set_macro_args
**
** parse & set all arguments of a macro
*/
ULONG set_macro_args( HSCMAC *macro, INFILE *inpf, BOOL open_mac )
{
    BOOL    ok = FALSE;
    STRPTR  nw = infgetw( inpf );

    macro_call_id++;

    /* read args */
    if ( strcmp( nw, ">" ) ) {

        while ( nw ) {

            if ( !nw )
                err_eof( inpf );
            if ( !strcmp( nw, ">" ) ) {

                nw = NULL;
                ok = TRUE;

            } else  {

                if ( strcmp( nw, "\n" ) ) {

                    DLLIST *varlist;

                    if ( open_mac )
                        varlist = macro->op_args;
                    else
                        varlist = macro->cl_args;
                    set_macro_arg( nw, varlist, inpf );

                }

                nw = infgetw( inpf );

            }
        }

        /* todo: check for required args */

    } else
        ok = TRUE;

    if ( !ok )
        macro_call_id = MCI_ERROR;

    return( macro_call_id );
}

