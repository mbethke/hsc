/*
** deftag.c
**
** define new tag from iput file
**
** updated: 27-Oct-1995
** created: 13-Oct-1995
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
#include "tag.h"

/*
** global vars
*/
LONG tag_call_id = 0;

/*
**
** global funs
**
*/

/*
** get_mci
**
*/
LONG get_mci( VOID )
{
    tag_call_id++;

    return( tag_call_id );
}

/*
** get_current_mci
**
*/
LONG get_current_mci( VOID )
{
    return( tag_call_id );
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
        err_eof( inpf, "read tag-id" );
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

                    message( MSG_DEFTAG_NO_OPEN, inpf );
                    errstr( "no opening tag for " );
                    errctag( nw );
                    errlf();

                }


            } /* err_eof already called in infget_tagid() */

        } else {

            tag = find_strtag( taglist, nw );
            if ( tag ) {

                /* find tag-node in list */
                /* NOTE: this is rather stupid, 'cause the list */
                /* has to be searched twice this way; but who cares? */
                DLNODE *nd = find_dlnode( deftag->first,
                                          (APTR) nw, cmp_strtag );

                /* new tag/macro replaces old tag/macro */
                if ( (tag->option) & HT_MACRO ) {

                    tag->occured = FALSE;

                    message( MSG_REDEFINE_MACRO, inpf );
                    errstr( "redefined macro " );
                    errtag( tag->name );
                    errlf();

                } else {

                    message( MSG_REDEFINE_TAG, inpf );
                    errstr( "can not redefine tag " );
                    errtag( tag->name );
                    errlf();

                }

                del_dlnode( deftag, nd );

            }

            /* create a new opening tag */
            tag = app_tag( taglist, nw );

        }
    } /* err_eof already called in infget_tagid() */

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
    BOOL   ok = FALSE;
    HSCVAR attr;

    /* init attr */
    attr.name    = "[MBI/NAW]";
    attr.deftext = NULL;
    attr.text    = NULL;
    attr.enumstr = NULL;
    attr.vartype = VT_STRING;
    attr.varflag = VF_NOQUOTE;

    if ( !(upstrcmp(option,TO_MBI_STR)
           && upstrcmp(option,TO_MBI_SHT) ) )
    {                                            /* must be inside */

        if ( parse_eq( inpf ) ) {

            STRPTR strmbi = parse_vararg( &attr, inpf );

            if ( strmbi ) {

                tag->mbi = strclone( strmbi );
                if ( tag->mbi ) {

                    DDT( fprintf( stderr, "**   mbi = \"%s\"\n", tag->mbi ) );
                    ok = TRUE;

                } else
                    err_mem( inpf );

            }
        }
    } else if ( !( upstrcmp( option, TO_NAW_STR )
                   && upstrcmp(option,TO_NAW_SHT) ) )
    {                                            /* not allowed with */

        if ( parse_eq( inpf ) ) {


            STRPTR strnaw = parse_vararg( &attr, inpf );

            if ( strnaw ) {

                tag->naw = strclone( strnaw );
                if ( tag->naw ) {

                    DDT( fprintf( stderr, "**   naw = \"%s\"\n", tag->naw ) );
                    ok = TRUE;

                } else
                    err_mem( inpf );

            }
        }
    } else {

        ok |= check_tag_option( option, tag, TO_CLOSE_STR, TO_CLOSE_SHT, HT_CLOSE );
        ok |= check_tag_option( option, tag, TO_IGNOREARGS_STR, TO_IGNOREARGS_SHT, HT_IGNOREARGS );
        ok |= check_tag_option( option, tag, TO_JERK_STR, TO_JERK_SHT, HT_JERK );
        ok |= check_tag_option( option, tag, TO_NOCOPY_STR, TO_NOCOPY_SHT, HT_NOCOPY );
        ok |= check_tag_option( option, tag, TO_NOHANDLE_STR, TO_NOHANDLE_SHT, HT_NOHANDLE );
        ok |= check_tag_option( option, tag, TO_SMARTCLOSE_STR, TO_SMARTCLOSE_SHT, HT_SMARTCLOSE );
        ok |= check_tag_option( option, tag, TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE );
        ok |= check_tag_option( option, tag, TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE );
        ok |= check_tag_option( option, tag, TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED );
        ok |= check_tag_option( option, tag, TO_SKIPLF_STR, TO_SKIPLF_SHT, HT_SKIPLF );

    }

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
    varlist = tag->attr;

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
                    err_eof( inpf, "define attributes" );
                else if ( !strcmp( nw, ">" ) ) {

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
                err_eof( inpf, "define attributes" );

            if ( !ok ) {
                del_dlnode(
                    taglist,
                    find_dlnode( taglist->first, (APTR) tag->name, cmp_strtag )
                );
                tag = NULL;
            }
        } else
            ok = TRUE;

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
    ULONG   result_tci = get_mci();    /* resulting tag_call_id */
    STRPTR  nw = infgetw( inpf );

    /* evaluate which varlist to use */
    varlist = tag->attr;

    /* read args */
    if ( strcmp( nw, ">" ) ) {

        while ( nw ) {

            if ( !nw )
                err_eof( inpf, "read attributes" );
            else if ( !strcmp( nw, ">" ) ) {

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


