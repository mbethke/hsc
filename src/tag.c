/*
** tag.c
**
** hsc-tag funcs for hsc
**
** updated: 10-Sep-1995
** created:  8-Sep-1995
**
** TODO: a lot
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
DLLIST *deftag = NULL;                 /* list for defined tags */
DLLIST *cltags = NULL;                 /* history for closing tags */

char    this_tag[MAX_TAGLEN];          /* id of tag currently processed */
HSCTAG *this_tag_data = NULL;          /* ptr to tag currently processed */

STRPTR last_anchor = NULL;             /* stores prev URL of last anchor; */
                                       /* used within tag_a.c, but must be */
                                       /* released within cleanup.c */
ULONG tag_call_id = 0;

/* TODO: remove nesting vars */
ULONG anchor_nesting = 0;
ULONG body_nesting   = 0;
ULONG head_nesting   = 0;
ULONG list_nesting   = 0;
ULONG title_nesting  = 0;

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
        newtag->name     = upstr( strclone(newid) );        /* set id */
        newtag->option   = 0;
        newtag->vers     = 0;
        newtag->o_handle = NULL;       /* no handle functions */
        newtag->c_handle = NULL;
        newtag->occured  = FALSE;
        newtag->op_fname = NULL;
        newtag->cl_fname = NULL;
        newtag->op_args  = init_dllist( del_var );
        newtag->cl_args  = init_dllist( del_var );

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

    /* remove temp. files */
    if ( tag->op_fname && tag->op_fname[0] && (!debug))
        remove( tag->op_fname );
    if ( tag->cl_fname && tag->cl_fname[0] && (!debug))
        remove( tag->cl_fname );

    /* release mem */
    ufreestr( tag->name );
    ufreestr( tag->name );
    ufreestr( tag->op_fname );
    ufreestr( tag->cl_fname );
    del_dllist( tag->op_args );
    del_dllist( tag->cl_args );
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
HSCTAG *find_strtag( STRPTR name )
{
    DLNODE *nd = find_dlnode( deftag->first, (APTR) name, cmp_strtag );
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
**-------------------------------------
** append tag functions
**-------------------------------------
*/

/*
** app_tag
**
** create a new tag and append it to the deftag-list
**
** params: tagid..name of the new tag (eg "IMG")
** result: ptr to the new tag or NULL if no mem
*/
HSCTAG *app_tag( STRPTR tagid )
{
    HSCTAG *newtag;

    newtag = new_hsctag( tagid );
    if ( newtag ) {
        if (app_dlnode( deftag, newtag ) == NULL ) {

            del_tag( (APTR) newtag );
            newtag = NULL;

        }
    } else
        err_mem( NULL );

    return (newtag);
}

#if 0
/*
** add_tag
*/
HSCTAG *add_tag(
  STRPTR tagid,
  ULONG  optn,
  ULONG  version,
  BOOL   (*op_handle)(INFILE *inpf),
  BOOL   (*cl_handle)(INFILE *inpf)
)
{
    HSCTAG *newtag = app_tag( tagid );

    if ( newtag ) {

        newtag->option   = optn;
        newtag->vers     = version;
        newtag->o_handle = op_handle;
        newtag->c_handle = cl_handle;

    } else
        err_mem( NULL );

    return (newtag);

}

/*
** add_stag: add simple tag (no version & handles)
*/
HSCTAG *add_stag( STRPTR tagid, ULONG optn )
{
    return ( add_tag( tagid, optn, 0, NULL, NULL) );
}
#endif

/*
**-------------------------------------
** define a new tag from input file
**-------------------------------------
*/

/*
** get_tag_name
**
*/
HSCTAG *def_tag_name( INFILE *inpf, BOOL *open_tag )
{
    STRPTR  nw = infgetw( inpf );
    HSCTAG *tag = NULL;

    /* create new tag */
    if ( nw ) {

        *open_tag = (BOOL)(strcmp( nw, "/" ));
        if ( !*open_tag ) {

            /* add closing tag */
            nw = infgetw( inpf );
            if ( nw ) {

                tag = find_strtag( nw );
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
            tag = app_tag( nw );
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

        if ( debug )
            fprintf( stderr, "**   option %s\n", id );
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

    ok |= check_tag_option( option, tag, TO_NOHANDLE_STR, TO_NOHANDLE_SHT, HT_NOHANDLE );
    ok |= check_tag_option( option, tag, TO_JERK_STR, TO_JERK_SHT, HT_JERK );
    ok |= check_tag_option( option, tag, TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE );
    ok |= check_tag_option( option, tag, TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE );
    ok |= check_tag_option( option, tag, TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED );

    if ( !ok ) {

        message( UNKN_TAG_OPTION, inpf );
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

    return( ok );
}

/*
** def_tag_args
**
*/
BOOL def_tag_args( HSCTAG *tag, INFILE *inpf, BOOL *open_tag )
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

                } else  {

                    STRPTR pw = strclone( nw );

                    if ( pw ) {

                        nw = infgetw( inpf );
                        if ( !strcmp( nw, "=" ) )
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

                }
            }

            if ( !(ok || fatal_error) )
                err_eof( inpf );

            if ( !ok ) {
                del_dlnode(
                    deftag,
                    find_dlnode( deftag->first, (APTR) tag->name, cmp_strtag )
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
** set_tag_args
**
** parse & set all arguments of a tag
*/
ULONG set_tag_args( HSCTAG *tag, INFILE *inpf, BOOL open_tag )
{
    BOOL    ok = FALSE;
    STRPTR  nw = infgetw( inpf );

    tag_call_id++;

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

                    if ( open_tag )
                        varlist = tag->op_args;
                    else
                        varlist = tag->cl_args;
                    set_tag_arg( nw, varlist, inpf );

                }

                nw = infgetw( inpf );

            }
        }

        /* todo: check for required args */

    } else
        ok = TRUE;

    if ( !ok )
        tag_call_id = MCI_ERROR;

    return( tag_call_id );
}


