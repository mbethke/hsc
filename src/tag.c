/*
** tag.c
**
** hsc-tag funcs for hsc
**
** updated:  8-Oct-1995
** created:  8-Sep-1995
**
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

#include "vars.h"

#define NOEXTERN_HSC_TAG
#include "tag.h"

/*
** global vars
*/
DLLIST *deftag  = NULL;                /* list for defined tags */
DLLIST *cltags  = NULL;                /* history for closing tags */
DLLIST *hsctags = NULL;                /* list for hsc tags ("$xx") */

STRPTR last_anchor = NULL;             /* stores prev URL of last anchor; */
                                       /* used within tag_a.c, but must be */
                                       /* released within cleanup.c */
ULONG tag_call_id = 0;

/*
**
** global funs
**
*/

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** new_hsctag
**
** alloc & init a new hsctag
*/
HSCTAG *new_hsctag( STRPTR newid )
{

    HSCTAG *newtag = (HSCTAG*) malloc( sizeof(HSCTAG) );

    if (newtag) {

        /* init new tag item */
        newtag->name        = upstr( strclone(newid) );        /* set id */
        newtag->option      = 0;
        newtag->vers        = 0;
        newtag->nest_count  = 0;
        newtag->o_handle    = NULL;       /* no handle functions */
        newtag->c_handle    = NULL;
        newtag->occured     = FALSE;
        newtag->op_text     = NULL;
        newtag->cl_text     = NULL;
        newtag->op_args     = init_dllist( del_var );
        newtag->cl_args     = init_dllist( del_var );
        newtag->only_inside = NULL;

        if ( !( newtag->name && newtag->op_args && newtag->cl_args ) ) {

            del_tag( newtag );
            newtag = NULL;
        }

    }

    return (newtag);
}

/*
** del_tag
*/
void del_tag( APTR data )
{
    HSCTAG *tag = (HSCTAG *)data;

    /* release mem */
    ufreestr( tag->name );
    ufreestr( tag->name );
    del_estr( tag->op_text  );
    del_estr( tag->cl_text  );
    ufreestr( tag->only_inside );
    del_dllist( tag->op_args );
    del_dllist( tag->cl_args );

    /* clear values */
    tag->option     = 0;
    tag->vers       = 0;
    tag->nest_count = 0;
    tag->op_text    = NULL;
    tag->cl_text    = NULL;
    tag->op_args    = NULL;
    tag->cl_args    = NULL;

    ufree( tag );
}



/*
**---------------------------
** find tag string
**---------------------------
*/

/*
** cmp_strtag
**
** compares a tag-string with the name
** of a HSCTAG-entry
*/
int cmp_strtag( APTR cmpstr, APTR tagdata )
{
    STRPTR tagstr = NULL;

    if ( tagdata )
        tagstr = ((HSCTAG*)tagdata)->name;

    if (tagstr)
        if ( !upstrcmp( cmpstr, tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
** find_strtag
*/
HSCTAG *find_strtag( DLLIST *taglist, STRPTR name )
{
    DLNODE *nd = find_dlnode( taglist->first, (APTR) name, cmp_strtag );
    HSCTAG *tag = NULL;

    if ( nd )
        tag = (HSCTAG*)nd->data;

    return ( tag );
}


/*
**---------------------------
** find closing tag string
**---------------------------
*/

/*
** cmp_strctg
**
** compares a tag-string with the name
** of closing tag (which is a STRPTR, but
** has been converted to APTR)
*/
int cmp_strctg( APTR cmpstr, APTR tagstr )
{
    if (tagstr)
        if ( !upstrcmp( (STRPTR) cmpstr, (STRPTR) tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}


/*
**---------------------------
** remove closing tag
**---------------------------
** params: tagname..tag to remove
**         check....show messages
*/
VOID remove_ctag( HSCTAG *tag, INFILE *inpf )
{
    /* search for tag on stack of occured tags */
    DLNODE *nd = find_dlnode( cltags->first, (APTR) tag->name, cmp_strctg );
    if ( nd == NULL ) {

        /* closing tag not found on stack */
        /* ->unmatched closing tag without previous opening tag */
        message( MSG_UNMA_CTAG, inpf );
        errstr( "Unmatched closing tag " );
        errctag( tag->name );
        errlf();

    } else {

        /* closing tag found on stack */
        STRPTR foundnm = (STRPTR) nd->data;
        STRPTR lastnm  = (STRPTR) cltags->last->data;

        /* check if name of closing tag is -not- equal
        /* to the name of the last tag last on stack */
        /* ->illegal tag nesting */
        if ( upstrcmp(lastnm, foundnm)
             && !(tag->option | HT_MACRO)
             && !(is_hsc_tag( tag )) )
        {

            message( MSG_CTAG_NESTING, inpf );
            errstr( "Illegal closing tag nesting (expected " );
            errctag( lastnm );
            errstr( ", found " );
            errctag( tag->name );
            errch( ')' );
            errlf();

        }

        /* remove node for closing tag from cltags-list */
        del_dlnode( cltags, nd );

    }
}

/*
**-------------------------------------
** append tag functions
**-------------------------------------
*/

/*
** app_tag
**
** create a new tag and append it to tag-list
**
** params: tagid..name of the new tag (eg "IMG")
** result: ptr to the new tag or NULL if no mem
*/
HSCTAG *app_tag( DLLIST *taglist, STRPTR tagid )
{
    HSCTAG *newtag;

    newtag = new_hsctag( tagid );
    if ( newtag ) {
        if (app_dlnode( taglist, newtag ) == NULL ) {

            del_tag( (APTR) newtag );
            newtag = NULL;

        }
    } else
        err_mem( NULL );

    return (newtag);
}


/*
**-------------------------------------
** read a tag-name from input file
**-------------------------------------
*/

/*
** hsc_normch_tagid
**
** decides if an char is an 'normal' char for tagnames
**
** params: ch...char to test
** result: TRUE, if ch is a 'normal' ch
**
** NOTE: this function is used as is_nc-methode
**       for the infile class
*/
BOOL hsc_normch_tagid( int ch )
{
    extern BOOL hsc_normch( int ch ); /* what a pervert! -> input.c */
    BOOL found = hsc_normch(ch);

    if ( !found )
        if ( strchr( HSC_TAGID, ch) )
            found = TRUE;

    return( found );
}

/*
** infget_tagid
**
** read next word from input, but with a
** different is_nc-methode that also handles
** the id for hsc-tags (usually "$")
*/
STRPTR infget_tagid( INFILE *inpf )
{
    STRPTR tagid = NULL;

    BOOL  (*old_is_nc)( int ch );

    old_is_nc = inpf->is_nc;           /* remember old is_nc-methode */
    inpf->is_nc = hsc_normch_tagid;
    tagid = infgetw( inpf );           /* read tagid */
    if ( !tagid )
        err_eof( inpf );
    inpf->is_nc = old_is_nc;           /* remember old is_nc-methode */

    return( tagid );
}

/*
**-------------------------------------
** define a new tag from input file
**-------------------------------------
*/

/*
** def_tag_name
**
*/
HSCTAG *def_tag_name( DLLIST *taglist, INFILE *inpf, BOOL *open_tag )
{
    STRPTR  nw  = NULL;
    HSCTAG *tag = NULL;

    /* get tag name */
    nw = infget_tagid( inpf );

    /* create new tag */
    if ( nw ) {

        *open_tag = (BOOL)(strcmp( nw, "/" ));
        if ( !*open_tag ) {

            /* add closing tag */
            nw = infget_tagid( inpf );
            if ( nw ) {

                tag = find_strtag( taglist, nw );
                if ( tag )
                    /* set closing flag */
                    tag->option |= HT_CLOSE;
                else {
                    /*TODO: no opening tag */
                }


            } else
                err_eof( inpf );

        } else {

            /* create a new opening tag */
            tag = app_tag( taglist, nw );
        }
    } else
        err_eof( inpf );

    /* convert tag name to upper  case */
    if ( tag )
        upstr( tag->name );

    return( tag );
}

/*
** check_tag_option
**
** check if a tag-option-string is equal to an id/short id.
** if so, set the corresponding option value within the tag.
**
** params: option..option string to check for (read from input)
**         tag.....tag to update option value for
**         id......id string of option (eg "REQUIRED")
**         sid.....short id string (eg "REQ")
**         value...option value to OR with old tag's option value
** result: TRUE, if tag's option value updated
*/
BOOL check_tag_option( STRPTR option, HSCTAG *tag, STRPTR id, STRPTR sid, ULONG value )
{
    BOOL found = FALSE;

    if ( !( (upstrcmp(option,id )) && (upstrcmp(option,sid )) ) ) {

        DDT( fprintf( stderr, "**   option %s\n", id ) );
        tag->option |= value;
        found = TRUE;

    }

    return( found );

}
/*
** parse_tag_option
*/
BOOL parse_tag_option( STRPTR option, HSCTAG *tag, INFILE *inpf )
{
    BOOL ok = FALSE;

    ok |= check_tag_option( option, tag, TO_CLOSE_STR, TO_CLOSE_SHT, HT_CLOSE );
    ok |= check_tag_option( option, tag, TO_IGNOREARGS_STR, TO_IGNOREARGS_SHT, HT_IGNOREARGS );
    ok |= check_tag_option( option, tag, TO_JERK_STR, TO_JERK_SHT, HT_JERK );
    ok |= check_tag_option( option, tag, TO_NOCOPY_STR, TO_NOCOPY_SHT, HT_NOCOPY );
    ok |= check_tag_option( option, tag, TO_NOHANDLE_STR, TO_NOHANDLE_SHT, HT_NOHANDLE );
    ok |= check_tag_option( option, tag, TO_NONESTING_STR, TO_NONESTING_SHT, HT_NONESTING );
    ok |= check_tag_option( option, tag, TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE );
    ok |= check_tag_option( option, tag, TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE );
    ok |= check_tag_option( option, tag, TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED );
    ok |= check_tag_option( option, tag, TO_SKIPLF_STR, TO_SKIPLF_SHT, HT_SKIPLF );

    if ( !ok ) {

        message( MSG_UNKN_TAG_OPTION, inpf );
        errstr( "Unknown tag option " );
        errqstr( option );
        errlf();

    }

    return( ok );
}

/*
** parse_tag_var
*/
BOOL parse_tag_var( STRPTR varname, HSCTAG *tag, INFILE *inpf, BOOL open_tag )
{
    BOOL ok = FALSE;
    DLLIST *varlist;
    HSCVAR *var;

    /* select var list to add new var to */
    if ( open_tag )
        varlist = tag->op_args;
    else
        varlist = tag->cl_args;

    var = define_var( varname, varlist, inpf, 0 );

    /* set macro attribute flag for macro tags */
    if ( var && (tag->option & HT_MACRO) )
        var->varflag |= VF_MACRO;

    return( ok );
}

/*
** def_tag_args
**
*/
BOOL def_tag_args( DLLIST *taglist, HSCTAG *tag, INFILE *inpf, BOOL *open_tag )
{
    BOOL    ok = FALSE;
    STRPTR  nw;

    if ( tag ) {

        /* read args */
        nw = infgetw( inpf );
        if ( strcmp( nw, ">" ) ) {

            while ( nw ) {

                if ( !nw )
                    err_eof( inpf );
                if ( !strcmp( nw, ">" ) ) {

                    nw = NULL;
                    ok = TRUE;

                } else  if ( strcmp( nw, "\n" ) ) {

                    STRPTR pw = strclone( nw );

                    if ( pw ) {

                        nw = infgetw( inpf );
                        if ( !strcmp( nw, ":" ) )
                            /* define new tag var */
                            parse_tag_var( pw, tag, inpf, *open_tag );
                        else {

                            /* set new tag option */
                            inungetcw( inpf );
                            parse_tag_option( pw, tag, inpf );

                        }
                        ufreestr( pw );

                    } else
                        err_mem( inpf );

                    nw = infgetw( inpf );

                } else /* skip LF */
                    nw = infgetw( inpf );

            }

            if ( !(ok || fatal_error) )
                err_eof( inpf );

            if ( !ok ) {
                del_dlnode(
                    taglist,
                    find_dlnode( taglist->first, (APTR) tag->name, cmp_strtag )
                );
                tag = NULL;
            }
        } else
            ok = TRUE;

        /* check args for closing tag */
        if ( !(*open_tag) && (tag->cl_args) && (tag->cl_args->first) ) {

            message( MSG_CL_MACR_ARG, inpf );
            errstr( "attributes for closing tag/macro" );
            errlf();
            /* TODO: remove list of closing args */

        }

    }

    return( ok );
}

/*
** set_tag_arg
**
** parse & set one single tag argument
**
*/
BOOL set_tag_arg( STRPTR varname, DLLIST *varlist, INFILE *inpf )
{
    HSCVAR *var = find_varname( varlist, varname );
    STRPTR  arg = NULL;
    BOOL    ok  = FALSE;
    STRPTR  nw;
    HSCVAR  skipvar;

    DAV( fprintf( stderr, "**    set attr %s\n", varname ) );

    if ( !var ) {                      /* attribute not found */

        /* assign to pseudo-attribute */
        var = &skipvar;
        var->name = varname;
        var->deftext = NULL;
        var->text    = NULL;
        var->enumstr = NULL;
        var->vartype = VT_STRING;
        var->varflag = VF_NOQUOTE;

        /* message: reference to unknown attribute */
        message( MSG_UNKN_SYMB, inpf );
        errstr( "Unknown" );
        errsym( varname );
        errlf();

    }

    /* get argument */
    nw  = infgetw( inpf );
    if ( !strcmp( nw, "=" ) ) {

        arg = parse_vararg( var, inpf );
        if ( arg ) {

            if ( set_vartext( var, arg ) ) {

                DAV( fprintf( stderr, "**    \"%s\"\n", arg ) );
                ok = TRUE;

            }

        }

    } else {

        arg = NULL;
        inungets( nw, inpf );
        ok = TRUE;

    }

    if ( arg ) {

        if ( var->vartype == VT_BOOL ) {

            message( MSG_ARG_BOOL_ATTR, inpf );
            errstr( "value for boolean" );
            errsym( var->name );
            errlf();

        }
    } else {

        if ( var->vartype == VT_BOOL ) {

            /* set boolean attribute */
            DAV( fprintf( stderr, " (bool)\n", var->name ) );
            set_vartext( var, var->name );
            var->quote = VQ_NO_QUOTE;

        } else {

            /* todo:non-bool var requires arg */
            message( MSG_NOARG_ATTR, inpf );
            errstr( "missing value for" );
            errsym( var->name );
            errlf();

        }
    }

    /* cleanup pseudo-attr */
    if ( var == &skipvar )
        clr_vartext( var );

    return( ok );

}


/*
** set_tag_args
**
** parse & set all arguments of a tag
*/
ULONG set_tag_args( HSCTAG *tag, INFILE *inpf, BOOL open_tag )
{
    BOOL    ok = FALSE;
    DLLIST *varlist;
    ULONG   result_tci;        /* resulting tag_call_id */
    STRPTR  nw = infgetw( inpf );

    tag_call_id++;
    result_tci = tag_call_id;

    /* evaluate which varlist to use */
    if ( open_tag )
        varlist = tag->op_args;
    else
        varlist = tag->cl_args;

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


                    set_tag_arg( nw, varlist, inpf );

                }

                nw = infgetw( inpf );

            }
        }

        /* check for required attributes */
        if ( ok )
            ok = check_varlist( varlist, inpf );

    } else
        ok = TRUE;

    if ( !ok )
        result_tci = MCI_ERROR;

    return( result_tci );
}


