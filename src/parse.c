/*
** parse.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** parse file: handle for entities & tags
**
** updated:  8-Dec-1995
** created:  1-Jul-1995
**
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/prginfo.h"
#include "ugly/infile.h"
#include "ugly/dllist.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"
#include "skip.h"
#include "status.h"
#include "tagargs.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"

#include "tag_a.h"

#include "deftag.h"

#define NOEXTERN_PARSE_H
#include "parse.h"

INFILE *parse_end_file; /* file for do_list-methods called in parse_end() */

/*
**---------------------------
** overloaded methods for
** INFILE class
**---------------------------
*/

/*
** hsc_normch
**
** decides if an char is an 'normal' char
**
** params: ch...char to test
** result: TRUE, if ch is a 'normal' ch
**
** NOTE: this function is used as is_nc-methode
**       for the infile class
*/
BOOL hsc_normch( int ch )
{
    if ( isalnum(ch) || (ch=='_') || (ch=='-') )
        return TRUE;
    else return FALSE;
}

/*
** hsc_whtspc
**
** decides if an char is a white-space
**
** params: ch...char to test
** result: TRUE, if ch is a 'normal' ch
**
** NOTE: this function is used as is_ws-methode
**       for the infile class
*/
BOOL hsc_whtspc( int ch )
{
    if ( strchr(" \t",ch) != NULL )
        return TRUE;
    else return FALSE;
}

/*
**---------------------------
** misc. functions
**---------------------------
*/

/*
** message_rplc
**
** message that tells user that a special char
** was replaced by its entity
*/

VOID message_rplc( STRPTR what, STRPTR by, INFILE *inpf )
{
    message( MSG_RPLC_ENT, inpf );
    errstr( "replaced " );
    errqstr( what );
    errstr( " by " );
    errqstr( by );
    errlf();
}

BOOL check_mbinaw( HSCTAG *tag, INFILE *inpf )
{
    BOOL ok = TRUE;

    /* check for tags that must be called before */
    if ( tag->mbi ) {

        DLNODE *nd    = cltags->first;
        LONG    found = 0;

        while ( nd && !found ) {

            HSCTAG *ctag = (HSCTAG*) nd->data;

            found = strenum( ctag->name, tag->mbi, '|', STEN_NOCASE );
            nd = nd->next;

        }

        if ( !found ) {

            message( MSG_MBI, inpf );
            errtag( tag->name );
            errstr( " must be inside " );
            errtag( tag->mbi );
            errlf();

            ok = FALSE;

        }

    }

    /* check for tags that are not to be called before */
    if ( tag->naw ) {

        DLNODE *nd    = cltags->first;
        LONG    found = 0;

        while ( nd ) {

            HSCTAG *ctag = (HSCTAG*) nd->data;

            found = strenum( ctag->name, tag->naw, '|', STEN_NOCASE );
            if ( found ) {

                message( MSG_NAW, inpf );
                errtag( tag->name );
                errstr( " not allowed within " );
                errtag( ctag->name );
                errlf();

                ok = FALSE;

            }
            nd = nd->next;

        }
    }

    return( ok );
}

/*
**---------------------------
** parse tag functions
**---------------------------
*/

/*
** parse_tag
**
** parse tag (after "<")
*/
BOOL parse_tag( INFILE *inpf)
{

    STRPTR  nxtwd;
    DLNODE *nd    = NULL;
    HSCTAG *tag   = NULL;
    ULONG   tci   = 0;   /* tag_call_id returned by set_tag_args() */
    BOOL   (*hnd)(INFILE *inpf, HSCTAG *tag) = NULL;
    BOOL    open_tag;
    DLLIST *taglist = deftag;
    BOOL    rplc_lt = FALSE;  /* TRUE, if replace spc. char "<" */
    BOOL    hnd_result = TRUE; /* result returned by handle */

    /* init strings used inside tag-handles */
    set_estr( tag_name_str, infgetcw( inpf ) );
    clr_estr( tag_attr_str );
    clr_estr( tag_close_str );

    if ( smart_ent && strlen( infgetcws( inpf ) ) ) {

        /*
        ** check for special char "<"
        */
        int ch = infgetc( inpf );

        if ( hsc_whtspc( ch ) ) {

            rplc_lt = TRUE;

            /* write "&lt;" and white spaces */
            message_rplc( "<", "&lt;", inpf );
            outstr( infgetcws( inpf ) );
            outstr( "&lt;" );
            outch( ch );

        } else
            inungetc( ch, inpf );

    }

    if ( !rplc_lt ) {

        /* get tag id */
        nxtwd = infget_tagid( inpf );

        if ( !fatal_error ) {

            /* append tag-name to tag_name_str */
            app_estr( tag_name_str, infgetcws( inpf ) );
            app_estr( tag_name_str, infgetcw( inpf ) );

            /* check for hsctag; if not, enable output */
            if ( suppress_output
                 && upstrncmp( nxtwd, HSC_TAGID, strlen(HSC_TAGID) )
                 && strcmp( nxtwd, HSC_COMMENT_STR )
                 && strcmp( nxtwd, HSC_ONLYCOPY_STR )
               )
                suppress_output = FALSE;         /* enable output */
            if ( !suppress_output )
                D( fprintf( stderr, "** tag <" ) );

        }
    }

    if ( !fatal_error && !rplc_lt ) {

        BOOL write_tag = FALSE; /* flag: write tag text & attrs to output? */

        if ( strcmp( "/", nxtwd ) ) {            /* is it a closing tag? */

            /*
            **
            ** process non-closing tag
            **
            */

            open_tag = TRUE;
            if ( !suppress_output )
                D( fprintf( stderr, "%s>\n", nxtwd ) );

            /* search for tag in list */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "unknown tag " );
                errtag( nxtwd );
                errlf();

                /* NOTE: This one's a bit perverted, because
                ** the closing ">" is appended to the
                ** attribute string, and the closing string
                ** is left empty; as there is nearly no code
                ** between setting and writing the strings,
                ** I think this is more reasonable than doing
                ** some tricky string-manipulation...
                */
                skip_until_eot( inpf, tag_attr_str );
                clr_estr( tag_close_str );

            } else {

                tag = (HSCTAG*)nd->data;

                /* set handle */
                hnd = tag->o_handle;

                /*
                ** handle options
                */

                /* check for obsolete tag */
                if ( tag->option & HT_OBSOLETE ) {

                    message( MSG_TAG_OBSOLETE, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( " is obsolete\n" );

                }

                /* check for jerk-tag */
                if ( tag->option & HT_JERK ) {

                    message( MSG_TAG_JERK, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( " is only used by jerks\n" );

                }

                /* only-once-tag occured twice? */
                if ( (tag->option & HT_ONLYONCE ) && (tag->occured) ) {

                    message( MSG_TAG_TOO_OFTEN, inpf );
                    errstr( "tag " );
                    errtag( nxtwd );
                    errstr( "occured too often\n" );

                }

                /* set occured-flag */
                if ( tag->option & (HT_ONLYONCE | HT_REQUIRED) )
                    tag->occured = TRUE;

                /* check for must be inside/not allowed within */
                if ( !check_mbinaw( tag, inpf ) )
                    hnd = NULL;

                /* clear (reset to default) attribute values of tag */
                clr_varlist( tag->attr );

                /* set attributes or check for ">" */
                if ( !(tag->option & HT_IGNOREARGS) ) {

                    tci = set_tag_args( tag, inpf, open_tag );
                    if ( tci == MCI_ERROR ) {

                        skip_until_eot( inpf, NULL );
                        hnd = NULL;

                    }

                    /* set ">" in string that contains closing text */
                    if ( !fatal_error ) {

                        set_estr( tag_close_str, infgetcws( inpf ) );
                        set_estr( tag_close_str, infgetcw( inpf ) );

                    }
                }

                /* closing tag required? */
                if ( tag->option & HT_CLOSE )
                    app_ctag( cltags, tag );

            }

        } else {

            /*
            **
            ** process closing tag
            **
            */

            /* get tag id */
            nxtwd = infget_tagid( inpf );           /* get tag id */
            open_tag = FALSE;

            /* append tag-name to tag_name_str */
            app_estr( tag_name_str, infgetcws( inpf ) );
            app_estr( tag_name_str, infgetcw( inpf ) );

            if ( !suppress_output )
                D( fprintf( stderr, "/%s>\n", nxtwd ) );

            /* search for tag in taglist */
            /* (see if it exists at all) */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                /* closing tag is absolutely unknown */
                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "unknown closing tag " );
                errctag( nxtwd );
                errlf();

                skip_until_eot( inpf, tag_attr_str );

            } else {

                tag = (HSCTAG*)nd->data; /* fitting tag in taglist */

                if ( tag->option & (HT_CLOSE | HT_SMARTCLOSE) ) {

                    /* set closing handle */
                    hnd = tag->c_handle;

                    /* check for no args */
                    if ( !parse_wd( inpf, ">" ) ) {

                        message( MSG_CL_TAG_ARG, inpf );
                        errstr( "no attributes allowed for closing tags" );
                        errlf();

                        skip_until_eot( inpf, NULL );

                    } else {

                        /* set ">" in string that contains closing text */
                        set_estr( tag_close_str, infgetcws( inpf ) );
                        set_estr( tag_close_str, infgetcw( inpf ) );

                    }

                    /* set values of attributes stored
                    ** in closing tag,
                    ** remove closing tag from stack
                    */
                    remove_ctag( tag, inpf );

                } else {

                    /* illegal closing tag */
                    message( MSG_ILLG_CTAG, inpf );      /* tag not found */
                    errstr( "illegal closing tag " );
                    errctag( nxtwd );
                    errlf();

                    skip_until_eot( inpf, tag_attr_str );

                }
            }
        }

        /*
        ** processed for opening AND closing tag
        */
        write_tag = ( !(tag) || !(tag->option & HT_NOCOPY) );

        /* call handle if available */
        if ( hnd && !fatal_error )
            hnd_result = (*hnd)( inpf, tag );

        /* write whole tag out */
        if ( write_tag && hnd_result ) {

            outstr( estr2str( tag_name_str ) );  /* output  "<tagname " */
            outstr( estr2str( tag_attr_str ) );  /* output attributes */
            outstr( estr2str( tag_close_str ) ); /* output ">" */

        }

        /* skip LF if requested */
        if ( tag && (tag->option & HT_SKIPLF) )
            skip_lf( inpf );
    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** other parse functions
**---------------------------
*/

/*
** parse_amp
**
** parse ampersand ("&")
*/
BOOL parse_amp( INFILE *inpf )
{
    EXPSTR *amp_str = init_estr( 0 );

    if ( !fatal_error ) {

        BOOL   rplc = smart_ent;       /* TRUE, if "&" should be replaced */

        suppress_output = FALSE;

        if ( rplc ) {

            /*
            ** test if char before and
            ** after ">" is white-space
            */
            int ch = infgetc( inpf );

            inungetc( ch, inpf );

            if ( !( hsc_whtspc( ch ) && strlen( infgetcws( inpf ) ) ) )
                rplc = FALSE;

        }

        if ( rplc ) {

            /* replace ampersand */
            message_rplc( "&", "&amp;", inpf );
            set_estr( amp_str, "&amp;" );

        } else {

            /*
            ** get entity-id, test for unknwon entity
            */
            char *nxtwd;
            DLNODE *nd;

            /* remember "&" */
            set_estr( amp_str, infgetcw( inpf ) );

            /* get entity id */
            nxtwd = infgetw( inpf );

            /* TODO: check for white-space */

            if ( !strcmp( nxtwd, "#" ) ) {

                /*
                ** process numeric entity
                */

                /* append "#" */
                app_estr( amp_str, infgetcw( inpf ) );

                nxtwd = infgetw( inpf );
                errno = 0;
                strtoul( nxtwd, NULL, 0 );
                if ( errno || strlen( infgetcws( inpf ) ) ) {

                    message( MSG_ILLEGAL_NUM, inpf );
                    errstr( "illegal numeric entity " );
                    errqstr( nxtwd );
                    errlf();

                }

                /* append entity specifier */
                app_estr( amp_str, nxtwd );

            } else {

                /*
                ** process text entity
                */

                /* search for entity in list */
                nd = find_dlnode( defent->first, (APTR) nxtwd, cmp_strent );
                if ( nd == NULL ) {

                    message( MSG_UNKN_ENTITY, inpf );
                    errstr( "unknown entity " );
                    errqstr( nxtwd );
                    errlf();

                }

                /* append entity specifier */
                app_estr( amp_str, nxtwd );

            }

            /* TODO: check for whitespace before ";" */

            /* check for closing ';' */
            parse_wd( inpf, ";" );

            /* append ";" */
            app_estr( amp_str, infgetcw( inpf ) );

        }

        /* output whole entity */
        outstr( estr2str( amp_str ) );

        del_estr( amp_str );
    }

    return (BOOL)( !fatal_error );
}

/*
** parse_text
*/
BOOL parse_text( INFILE *inpf )
{
    STRPTR  nw = infgetcw( inpf );

    if ( suppress_output && (strcmp( nw, "\n" )) )
        suppress_output = FALSE;

    /*
    ** check unmatched ">"
    */
    if ( !strcmp(nw, ">") ) {          

        BOOL rplc = smart_ent; /* TRUE, if ">" should be replaced */

        if ( rplc ) {

            /*
            ** test if char before and
            ** after ">" is white-space
            */
            int ch = infgetc( inpf );

            inungetc( ch, inpf );

            if ( !( hsc_whtspc( ch ) && strlen( infgetcws( inpf ) ) ) )
                rplc = FALSE;

        }

        if ( rplc ) {

            /* replace gt */
            message_rplc( nw, "&gt;", inpf );
            nw = "&gt;";

        } else {

            message( MSG_UNMA_GT, inpf );
            errstr( "unmatched \">\"\n" );

        }

    /*
    ** check for quote
    */
    } else if ( !strcmp( nw, "\"" ) ) {

        if ( smart_ent ) {

            /* replace quote */
            message_rplc( nw, "&quot;", inpf );
            nw = "&quot;";

        } else {

            message( MSG_UNMA_QUOTE, inpf );
            errstr( "quote found inside text" );
            errlf();

        }


    /*
    ** check for entities to replace
    */
    } else {

        DLNODE *nd = NULL;

        if ( rplc_ent ) {

            nd = find_dlnode( defent->first, (APTR) nw, cmp_rplcent );

            if ( nd ) {

                BOOL ok = TRUE;

                /* copy replaced entity to buffer */
                ok &= set_estr( tmpstr, "&" );
                ok &= app_estr( tmpstr, ((HSCENT*)nd->data)->name );
                ok &= app_estr( tmpstr, ";" );

                if ( ok ) {

                    /* replace-message */
                    message_rplc( nw, estr2str( tmpstr ), inpf );
                    nw = estr2str( tmpstr );

                }
            }
        }

        if ( !nd && inside_anchor && click_here_str ) {

            /*
            ** check for "click here" syndrome
            */
            ULONG found = strenum( nw, click_here_str, '|', STEN_NOCASE );
            if ( found ) {

                message( MSG_CLICK_HERE, inpf );
                errstr( "\"click here\" syndrome detected" );
                errlf();

            }
        }
    }

    if ( nw )
        outstr( nw );                  /* output word */

    return (BOOL)( !fatal_error );
}

/*
** parse_hsc
**
** parse input chars with full hsc support
**
** params: inpf...input file
**
** result: TRUE, if no error
*/
BOOL parse_hsc( INFILE *inpf )
{
    if ( !fatal_error ) {
        char *nxtwd;

        nxtwd = infgetw( inpf );

        /* write white spaces */
        if ( infgetcws( inpf ) )
            outstr( infgetcws( inpf ) );

        if ( nxtwd ) {

#if 0
            /*
            ** clear and enable log,
            ** append current word WITHOUT white-spaces to log
            */
            inflog_clear( inpf );
            inflog_app( inpf, nxtwd );
            inflog_enable( inpf );
#endif

            if ( !strcmp(nxtwd, "<") )                     /* parse tag */
                parse_tag( inpf );
            else if ( !strcmp(nxtwd, "&") )                /* parse entity */
                parse_amp( inpf );

            else {                                         /* handle text */
                parse_text( inpf );
            }
        }
#if 0
        inflog_disable( inpf );
#endif
    }

    return (BOOL)( !fatal_error );
}

/*
** parse_source
**
** parse input chars with full hsc support
**
** params: inpf...input file
**
** result: TRUE, if no error
*/
BOOL parse_source( INFILE *inpf )
{
    if ( !fatal_error ) {

        char *nxtwd = infgetw( inpf );

        if ( nxtwd ) {

            /* write white-spaces */
            outstr( infgetcws( inpf ) );

            /*
            ** process next word
            */
            if ( !strcmp(nxtwd, "<") )
                outstr( "&lt;" );
            else if ( !strcmp(nxtwd, ">") )
                outstr( "&gt;" );
            else if ( !strcmp(nxtwd, "&") )
                outstr( "&amp;" );
            else {

                parse_text( inpf );

            }
        }
    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** parse end functions
**---------------------------
*/

/*
** check_tag_missing
**
** checks if a required tag has not occured
** (methode for do_dllist())
*/
void check_tag_missing( APTR data )
{
    HSCTAG *tag = (HSCTAG *)data;

    if ( (tag->option & HT_REQUIRED
         && (tag->occured == FALSE) ) )
    {
        message( MSG_MISS_REQTAG, parse_end_file );
        errstr( "required tag missing: " );
        errtag( tag->name );
        errlf();

    }
}

/*
** check_not_closed
**
** check if a closing tag is missing
** (methode for do_dllist())
*/
void check_not_closed( APTR data )
{
    STRPTR name = ((HSCTAG*)data)->name;

    message( MSG_MISS_CTAG, parse_end_file );
    errstr( "closing tag missing: " );
    errctag( name );
    errlf();

}

/*
** parse_end
**
** check for all tags closed and required
** tags occured
*/
BOOL parse_end( INFILE *inpf )
{
    if ( !fatal_error ) {

        parse_end_file = inpf;

        /* check for tags not closed */
        do_dllist( cltags, check_not_closed );

        /* check for required tags missing */
        do_dllist( deftag, check_tag_missing );

    }

    return (BOOL)( !fatal_error );
}

/*
**---------------------------
** main include function
**---------------------------
*/


/*
** include_hsc
**
** read from inpf, parse for hsc-commands and execute them,
** check for html-error,
** write all out to outf and close input file.
**
** params: inpfnm...input file name
**         outf.....output file structure, already opended
**
** result: TRUE, if all worked well, else FALSE
*/
BOOL include_hsc( STRPTR filename, INFILE *inpf, OUTFILE *outf, ULONG optn )
{

    BOOL    ok;            /* result */
    ok   = (inpf!=NULL);

    if ( inpf ) {                                /* file opened? */

        /* hide status? */
        if ( (optn & IH_PARSE_MACRO) || (optn & IH_PARSE_MACRO) )
            optn |= IH_NO_STATUS;

        /* set char-parse methods */
        inpf->is_nc = hsc_normch;                /*    set is_nc-methode */
        inpf->is_ws = hsc_whtspc;                /*    set is_ws-methode */

        while ( !infeof(inpf) && ok ) {          /*    parse file */

            if ( !(optn & IH_NO_STATUS ) )
                status_infile( inpf, FALSE );    /*    status message */

            if ( optn & IH_PARSE_SOURCE )
                ok = parse_source( inpf );
            else
                ok = parse_hsc( inpf );
        }

        /* parse at end: check for missing tags, .. */
        if ( ok && (optn & IH_PARSE_END) ) {     /*    parse end */
                                                 /*    (unclosed tags etc) */
            ok = parse_end( inpf );

        }

        /* end of file status */
        if ( !(optn & IH_NO_STATUS) ) {

            status_infile( inpf, TRUE );         /*    status message */
            status_lf();                         /*    (display num of line) */

        }

        infclose( inpf );                        /*    close file */
                                                 
    } else {                                     /* N-> error message */

        message( MSG_NO_INPUT, NULL );
        errstr( "can not open " );
        errqstr( filename );
        errstr( " for input: " );
        errstr( strerror( errno ) );
        errlf();
        ok = FALSE;
    }

    return ( ok );
}

/*
** include_hsc_file
**
** open input file and include it
*/
BOOL include_hsc_file( STRPTR filename, OUTFILE *outf, ULONG optn )
{
    INFILE *inpf;
    BOOL    ok;
    STRARR msg[ MAX_PATHLEN ];

    /* status message: reading input */
   if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)) ) {

        sprintf( msg, "Reading \"%s\"", filename );
        status_msg( msg );

    }

    /* open & read input file */
    inpf = infopen( filename, ES_STEP_INFILE );

    /* include opened file */
    ok = include_hsc( filename, inpf, outf, optn );

    return( ok );
}

/*
** include_hsc_str
**
** open input file and include it
*/
BOOL include_hsc_string( STRPTR filename, STRPTR s, OUTFILE *outf, ULONG optn )
{
    INFILE *inpf;
    BOOL    ok;

    inpf = infopen_str( filename, s, 0 );        /* try to open input file */
    ok = include_hsc( filename, inpf, outf, optn );

    return( ok );
}

/*
** includes_ok
**
** include all files passed with "INCLUDE=file"
*/
BOOL include_ok( VOID )
{
    if ( incfile ) {

        DLNODE *nd = incfile->first;

        while ( nd && !fatal_error ) {

            STRPTR filename = (STRPTR) nd->data;
            include_hsc_file( filename, outfile, 0 );
            nd = nd->next;

        }
    }

    return( (BOOL) !fatal_error );
}


