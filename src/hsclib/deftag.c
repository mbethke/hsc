/*
** hsclib/deftag.c
**
** define new tag from input file
**
** Copyright (C) 1995,96  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 20-Apr-1996
** created: 13-Oct-1995
*/

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/eval.h"
#include "hsclib/input.h"
#include "hsclib/skip.h"

/* tag options that can be set via DEFTAG */
#define TO_CLOSE_STR       "CLOSE"
#define TO_CLOSE_SHT       "C"
#define TO_SPECIAL_STR     "SPECIAL"
#define TO_SPECIAL_SHT     "SPC"
#define TO_JERK_STR        "JERK"
#define TO_JERK_SHT        "J"
#define TO_LAZY_STR        "LAZY"
#define TO_LAZY_SHT        "L"
#define TO_MBI_STR         "MUST_BE_INSIDE"
#define TO_MBI_SHT         "MBI"
#define TO_NAW_STR         "NOT_ALLOWED_WITHIN"
#define TO_NAW_SHT         "NAW"
#define TO_AUTOCLOSE_STR   "AUTOCLOSE"
#define TO_AUTOCLOSE_SHT   "AC"
#define TO_ONLYONCE_STR    "ONLYONCE"
#define TO_ONLYONCE_SHT    "1"
#define TO_OBSOLETE_STR    "OBSOLETE"
#define TO_OBSOLETE_SHT    "O"
#define TO_REQUIRED_STR    "REQUIRED"
#define TO_REQUIRED_SHT    "R"
#define TO_SKIPLF_STR      "SKIPLF"
#define TO_SKIPLF_SHT      "S"

#define TO_VERS_STR        "VERS" /* TODO: think about this tag-options */
#define TO_VERS_SHT        "V"

/*
** get_mci
**
*/
LONG get_mci( HSCPRC *hp )
{
    hp->tag_call_id++;

    return( hp->tag_call_id );
}

/*
** unget_mci
**
*/
VOID unget_mci( HSCPRC *hp )
{
    hp->tag_call_id--;
}

/*
** get_current_mci
**
*/
LONG get_current_mci( HSCPRC *hp )
{
    return( hp->tag_call_id );
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
HSCTAG *def_tag_name( HSCPRC *hp, BOOL *open_tag )
{
    STRPTR  nw  = NULL;
    HSCTAG *tag = NULL;
    DLLIST *taglist = hp->deftag;

    /* get tag name */
    nw = infget_tagid( hp );

    /* create new tag */
    if ( nw ) {

        *open_tag = (BOOL)(strcmp( nw, "/" ));
        if ( !(*open_tag) ) {

            /* add closing tag */
            nw = infget_tagid( hp );
            if ( nw ) {

                tag = find_strtag( taglist, nw );
                if ( tag )
                    /* set closing flag */
                    tag->option |= HT_CLOSE;
                else {

                    hsc_message( hp, MSG_DEFTAG_NO_OPEN,
                                 "no start tag for %c", nw );

                }


            } /* err_eof already called in infget_tagid() */

        } else {

            tag = find_strtag( taglist, nw );
            if ( tag ) {

                /* find tag-node in list */
                /* NOTE: this is rather stupid, 'cause the list */
                /* has to be searched twice this way; but who cares? */
                DLNODE *nd = find_dlnode( hp->deftag->first,
                                          (APTR) nw, cmp_strtag );

                /* new tag/macro replaces old tag/macro */
                if ( (tag->option) & HT_MACRO ) {

                    tag->occured = FALSE;

                    hsc_message( hp, MSG_REDEFINE_MACRO,
                                 "redefined %T", tag );

                } else {

                    hsc_message( hp, MSG_REDEFINE_TAG,
                                 "can not redefine %T", tag );

                }

                del_dlnode( hp->deftag, nd );

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
static BOOL check_tag_option( HSCPRC *hp, STRPTR option, HSCTAG *tag,
                              STRPTR id, STRPTR sid, ULONG value )
{
    BOOL found = FALSE;

    if ( !( (upstrcmp(option,id )) && (upstrcmp(option,sid )) ) ) {

        DDT( fprintf( stderr, DHL "  option %s\n", id ) );
        tag->option |= value;
        found = TRUE;

    }

    return( found );

}

/*
** parse_lazy_option
**
** allowed abbrevations:
**
** c  CLASS:string
** h  HREF:uri
** i  ID:id
** k  CLEAR:enum("left|right|all|*")
** l  LANG:string
** m  MD:string
** s  SRC:URI
** w  NOWRAP:bool
*/
static HSCATTR *def_lazy_attr( HSCPRC *hp, HSCTAG *tag,
                           STRPTR attrname, BYTE attrtype )
{
    HSCATTR *newattr = app_var( tag->attr, attrname );
    DDA( fprintf( stderr, DHL "new attr: `%s'\n", attrname ) );
    newattr->vartype = attrtype;
    return( newattr );
}

static BOOL parse_lazy_option( HSCPRC *hp, HSCTAG *tag, STRPTR lazy )
{
    BOOL   ok      = TRUE;

    while( lazy[0] ) {

        switch ( lazy[0] ) {
            case 'c':
                def_lazy_attr( hp, tag, "CLASS", VT_STRING );
                break;
            case 'h':
                def_lazy_attr( hp, tag, "HREF", VT_URI );
                break;
            case 'i':
                def_lazy_attr( hp, tag, "ID", VT_ID );
                break;
            case 'k': {
                HSCATTR *clear_attr =
                         def_lazy_attr( hp, tag, "CLEAR", VT_ENUM );
                clear_attr->enumstr = strclone( "left|right|all|*" );
                }
                break;
            case 'l':
                def_lazy_attr( hp, tag, "LANG", VT_STRING );
                break;
            case 'm':
                def_lazy_attr( hp, tag, "MD", VT_STRING );
                break;
            case 's':
                def_lazy_attr( hp, tag, "SRC", VT_URI );
                break;
            case 'w':
                def_lazy_attr( hp, tag, "NOWRAP", VT_BOOL );
                break;
            default:
                hsc_message( hp, MSG_UNKN_TAG_OPTION,
                             "unknown tag option %q (arg %q)",
                             "LAZY", ch2str( lazy[0] ) );
                break;
        }
        lazy++;
    }

    return( ok );
}

/*
** parse_tag_option
*/
static BOOL parse_tag_option( HSCPRC *hp, STRPTR option, HSCTAG *tag )
{
    BOOL    ok = FALSE;
    HSCATTR *attr = new_hscattr( SPECIAL_FILE_ID "MBI+NAW" );

    attr->vartype = VT_STRING;

    if ( !(upstrcmp(option,TO_MBI_STR)
           && upstrcmp(option,TO_MBI_SHT) ) )
    {                                            /* must be inside */

        if ( parse_eq( hp ) ) {

            STRPTR strmbi = eval_expression( hp, attr, NULL );

            if ( strmbi ) {

                tag->mbi = strclone( strmbi );
                DDT( fprintf( stderr, DHL "  mbi = `%s'\n", tag->mbi ) );
                ok = TRUE;

            }
        }
    } else if ( !( upstrcmp( option, TO_NAW_STR )
                   && upstrcmp(option,TO_NAW_SHT) ) )
    {                                            /* not allowed with */

        if ( parse_eq( hp ) ) {


            STRPTR strnaw = eval_expression( hp, attr, NULL );

            if ( strnaw ) {

                tag->naw = strclone( strnaw );
                DDT( fprintf( stderr, DHL "  mbi = `%s'\n", tag->naw ) );
                ok = TRUE;

            }
        }
    } else if ( !( upstrcmp( option, TO_LAZY_STR )
                   && upstrcmp(option,TO_LAZY_SHT) ) )
    {                                            /* lazy standard attribs */

        if ( parse_eq( hp ) ) {


            STRPTR strlazy = eval_expression( hp, attr, NULL );

            if ( strlazy ) {

                DDT( fprintf( stderr, DHL "  lazy= `%s'\n", strlazy ) );
                ok = parse_lazy_option( hp, tag, strlazy );

            }
        }
    } else {

        ok |= check_tag_option( hp, option, tag, TO_CLOSE_STR, TO_CLOSE_SHT, HT_CLOSE );
        ok |= check_tag_option( hp, option, tag, TO_SPECIAL_STR, TO_SPECIAL_SHT, HT_SPECIAL );
        ok |= check_tag_option( hp, option, tag, TO_JERK_STR, TO_JERK_SHT, HT_JERK );
        ok |= check_tag_option( hp, option, tag, TO_AUTOCLOSE_STR, TO_AUTOCLOSE_SHT, HT_AUTOCLOSE );
        ok |= check_tag_option( hp, option, tag, TO_OBSOLETE_STR, TO_OBSOLETE_SHT, HT_OBSOLETE );
        ok |= check_tag_option( hp, option, tag, TO_ONLYONCE_STR, TO_ONLYONCE_SHT, HT_ONLYONCE );
        ok |= check_tag_option( hp, option, tag, TO_REQUIRED_STR, TO_REQUIRED_SHT, HT_REQUIRED );
        ok |= check_tag_option( hp, option, tag, TO_SKIPLF_STR, TO_SKIPLF_SHT, HT_SKIPLF );

        if ( !ok ) {

            hsc_message( hp, MSG_UNKN_TAG_OPTION,
                         "unknown tag option %q", option );

        }
    }

    /* remove temp. attribute */
    del_hscattr( attr );

    return( ok );
}

/*
** parse_tag_var
*/
static BOOL parse_tag_var( HSCPRC *hp, HSCTAG *tag )
{
    BOOL ok = FALSE;
    HSCATTR *var     = NULL;

    /* define new attribute */
    var = define_var( hp, tag->attr, VF_CONST | VF_GLOBAL );

    /* set several values of tag structure, if attribute has
    ** some special flags set
    */
    if ( var ) {

        /* attribute is uri that tells the size */
        if ( var->varflag & VF_GETSIZE )
            tag->uri_size = var;

        /* attribute is uri that tells if the tag should be stripped */
        if ( var->varflag & VF_STRIPEXT )
            tag->uri_stripext = var;

        /* set macro attribute flag for macro tags */
        if ( tag->option & HT_MACRO )
            var->varflag |= VF_MACRO;

        ok = TRUE;
    }

    return( ok );
}

/*
** def_tag_args
**
*/
BOOL def_tag_args( HSCPRC *hp, HSCTAG *tag )
{
    BOOL    ok = FALSE;
    STRPTR  nw;
    INFILE *inpf = hp->inpf;

    if ( tag ) {

        ok = TRUE;

        /* read args */
        nw = infgetw( inpf );

        /*
        ** set tag options
        */
        while ( nw && ( ( !strcmp( nw, "/" ) ) || !strcmp( nw, "\n" ) ) ) {

            nw = infgetw( inpf );
            if ( nw ) {

                if ( strcmp( nw, "\n" ) )
                    ok &= parse_tag_option( hp, nw, tag );
                nw = infgetw( inpf );

            }

        }

        /*
        ** set tag attributes
        */
        while ( nw && ( ( strcmp( nw, ">" ) || !strcmp( nw, "\n" ) ) ) ) {

            if ( strcmp( nw, "\n" ) ) {

                inungetcw( inpf );
                ok &= parse_tag_var( hp, tag );

            }

            nw = infgetw( inpf );

        }

        /* skip linefeeds */
        while ( nw && !strcmp( nw, "\n" ) )
            nw = infgetw( inpf );

        /* check for ">" at end */
        if ( nw ) {

            inungetcw( inpf );
            ok = parse_gt( hp );

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
static BOOL set_tag_arg( HSCPRC *hp, DLLIST *varlist, STRPTR varname )
{
    HSCATTR *var = find_varname( varlist, varname );
    INFILE *inpf = hp->inpf;
    STRPTR  arg = NULL;
    BOOL    ok  = FALSE;
    STRPTR  nw;
    HSCATTR  skipvar;                   /* dummy attribute to skip unkown */
    EXPSTR *attr_str = init_estr( 40 ); /* string for attribute name */
    EXPSTR *val_str  = init_estr( 40 ); /* string for "=" and value */

    DAV( fprintf( stderr, DHL "   set attr %s\n", varname ) );

    /* append attribute name to attr_str */
    app_estr( attr_str, infgetcws( inpf ) );
    app_estr( attr_str, infgetcw( inpf ) );

    if ( !var ) {                      /* attribute not found */

        /* assign to pseudo-attribute */
        var = &skipvar;
        var->name = varname;
        var->deftext = NULL;
        var->text    = NULL;
        var->enumstr = NULL;
        var->vartype = VT_STRING;
        var->varflag = 0;

        /* message: unknown attribute */
        hsc_msg_unkn_attr( hp, varname );

    }

    /* get argument */
    nw  = infgetw( inpf );
    if ( nw )
        if ( !strcmp( nw, "=" ) ) {

            /* append "=" to log */
            app_estr( val_str, infgetcws( inpf ) );
            app_estr( val_str, infgetcw( inpf ) );

            /* parse expression */
            arg = eval_expression( hp, var, NULL );

            /* append value to log */
            if ( var->quote != VQ_NO_QUOTE )
                app_estrch( val_str, var->quote );
            if ( get_vartext( var ) )
                app_estr( val_str, get_vartext( var ) );
            if ( var->quote != VQ_NO_QUOTE )
                app_estrch( val_str, var->quote );

            if ( arg ) {

                DAV( fprintf( stderr, DHL "  `%s'\n", arg ) );
                ok = TRUE;

            }

        } else {

            arg = NULL;
            inungetcwws( inpf );
            ok = TRUE;

        }
    else
        hsc_msg_eof( hp, "read attribute value" );

    if ( ok )
        if ( arg ) {

            if ( var->vartype == VT_BOOL ) {

                /* set boolean attribute depending on expression */
                set_vartext_bool( var, get_varbool( var ) );

                /* if the expression returned FALSE, remove
                ** the boolean  switch from the call
                */
                if ( !get_varbool( var ) )
                    clr_estr( attr_str );

            } else
                /* append value to attribute string */
                app_estr( attr_str, estr2str( val_str ) );

        } else {

            /* no value has been passed to the attribute */
            if ( var->vartype == VT_BOOL ) {

                /* for boolean attributes, this is legal,
                ** and enables the attribute
                */
                set_vartext_bool( var, TRUE );

            } else {

                /* for non-boolean attributes, display
                ** error message
                */
                hsc_message( hp, MSG_NOARG_ATTR,
                             "missing value for %A", var );

            }
        }

#if 0
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


        }
    }
#endif

    /* cleanup pseudo-attr */
    if ( var == &skipvar )
        clr_vartext( var );

    /* append & cleanup attribute and value string */
    app_estr( hp->tag_attr_str, estr2str( attr_str ) );
    del_estr( attr_str );
    del_estr( val_str );

    return( ok );
}



/*
** set_tag_args
**
** parse & set all arguments of a tag
*/
ULONG set_tag_args( HSCPRC *hp, HSCTAG *tag )
{
    INFILE *inpf = hp->inpf;
    BOOL    ok  = FALSE;
    DLLIST *varlist;
    ULONG   result_tci = get_mci(hp);    /* resulting tag_call_id */
    STRPTR  nw = infgetw( inpf );

    /* evaluate which varlist to use */
    varlist = tag->attr;

    /* clear string that logs all attributes passed to tag */
    clr_estr( hp->tag_attr_str );

    /* read args */
    do {

        if ( !nw )
            hsc_msg_eof( hp, "read attributes" );
        else {

            /*
            ** process next attribute
            */

            if ( !strcmp( nw, ">" ) ) {

                nw = NULL;
                ok = TRUE;

            } else  {

                if ( strcmp( nw, "\n" ) ) {

                    /* process attribute */
                    if ( check_attrname( hp, nw ) )
                        set_tag_arg( hp, varlist, nw );
                    else {

                        /* append empty value */
                        app_estr( hp->tag_attr_str, "\"\"" );
                        skip_until_eot( hp, NULL );
                        nw = NULL;

                    }
                } else {

                    /* skip blank line */
                    app_estr( hp->tag_attr_str, infgetcw( inpf ) );

                }

                /* read next attribute */
                nw = infgetw( inpf );

            }
        }

    } while ( nw );

    /* set all undefined bool. attr to FALSE */
    clr_varlist_bool( varlist );

    /* check for required attributes */
    if ( ok )
        ok = check_varlist( hp, varlist );

    if ( !ok )
        result_tci = MCI_ERROR;

    return( result_tci );
}

