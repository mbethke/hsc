/*
** parse.c
**
** parse file: handle for entities & tags
**
** updated:  3-Oct-1995
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
#include "status.h"
#include "tagargs.h"

#include "entity.h"
#include "tag.h"
#include "vars.h"

#define NOEXTERN_PARSE_H
#include "parse.h"

/* TODO: remove this */
/*#include "tag_a.h" */

INFILE *parse_end_file; /* file for do_list-methods called in parse_end() */

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

    /* get tag id */
    nxtwd = infgetw( inpf );                 /* get tag id */
    if ( !nxtwd )
        err_eof( inpf );

    if ( !fatal_error ) {

        if ( debug )                             /* debug msg */
            fprintf( stderr, "** tag <" );

        /* check for hsctag */
        if ( !upstrcmp( nxtwd, HSC_TAGID ) ) {

            if ( debug )                         /* debug msg */
                fprintf( stderr, "%s", nxtwd );
            nxtwd = infgetw( inpf );             /* get hsc-tag id */
            taglist = hsctags;

        }

        if ( !nxtwd )
            err_eof( inpf );

    }

    if ( !fatal_error ) {

        if ( strcmp( "/", nxtwd ) ) {            /* is it a closing tag? */

            /*
            **
            ** process non-closing tag
            **
            */

            open_tag = TRUE;
            if ( debug )                         /* debug msg */
                fprintf( stderr, "%s>\n", nxtwd );

            /* search for tag in list */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown tag " );
                errtag( nxtwd );
                errlf();

                skip_until_gt( inpf );

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

                /* closing tag required? */
                if ( tag->option & HT_CLOSE ) {
                    /* TODO: handle out of mem */
                    if ( !app_dlnode( cltags, (APTR) tag->name ) )
                        err_mem( inpf );
                }
            }

        } else {

            /*
            **
            ** process closing tag
            **
            */

            /* get tag id */
            nxtwd = infgetw( inpf );           /* get tag id */
            open_tag = FALSE;

            if ( debug )                         /* debug msg */
                fprintf( stderr, "/%s>\n", nxtwd );

            /* search for tag in taglist */
            /* (see if it exists at all) */
            nd = find_dlnode( taglist->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                /* closing tag is absolutely unknown */
                message( MSG_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown closing tag " );
                errctag( nxtwd );
                errlf();

                skip_until_gt( inpf );

            } else {

                tag = (HSCTAG*)nd->data; /* fitting tag in taglist */

                /* set closing handle */
                hnd = tag->c_handle;

                /* search for tag on stack of occured tags */
                nd = find_dlnode( cltags->first, (APTR) nxtwd, cmp_strctg );
                if ( nd == NULL ) {

                    /* closing tag not found on stack */
                    /* ->unmatched closing tag without previous opening tag */
                    message( MSG_UNMA_CTAG, inpf );
                    errstr( "Unmatched closing tag " );
                    errctag( nxtwd );
                    errlf();

                } else {

                    /* closing tag found on stack */
                    STRPTR foundnm = (STRPTR) nd->data;
                    STRPTR lastnm  = (STRPTR) cltags->last->data;

                    /* check if name of closing tag is -not- equal
                    /* to the name of the last tag last on stack */
                    /* ->illegal tag nesting */
                    if ( upstrcmp(lastnm, foundnm)
                         && !(tag->option | HT_MACRO) )
                    {

                        message( MSG_CTAG_NESTING, inpf );
                        errstr( "Illegal closing tag nesting (expected " );
                        errctag( lastnm );
                        errstr( ", found " );
                        errctag( nxtwd );
                        errch( ')' );
                        errlf();

                    }

                }

                /* remove node for closing tag from cltags-list */
                del_dlnode( cltags, nd );

            }
        }

        /*
        ** processed for opening AND closing tag
        */

        /* set attributes */
        if ( tag && !(tag->option & HT_IGNOREARGS) )
            tci = set_tag_args( tag, inpf, open_tag );
        else if ( !hnd )
            skip_until_gt( inpf );

        /* call closing handle if available */
        if ( hnd && !fatal_error )
            (*hnd)( inpf, tag );

        /* write whole tag out */
        if ( !(tag) || !(tag->option & HT_NOCOPY) )
            outstr( infget_log( inpf ) );

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
    if ( !fatal_error ) {
        char *nxtwd;
        char nxtch;
        DLNODE *nd;

        /* get entity id */
        nxtwd = infgetw( inpf );

        /* search for entity in list */
        nd = find_dlnode( defent->first, (APTR) nxtwd, cmp_strent );
        if ( nd == NULL ) {

            message( MSG_UNKN_ENTITY, inpf );
            errstr( "Unknown entity " );
            errqstr( nxtwd );
            errlf();

        }

        /* check for closing ';' */
        nxtch = infgetc( inpf );
        if ( nxtch != ';' ) {

            message( MSG_EXPT_SEMIK, inpf );
            errqstr( ";" );
            errstr( " expected (found: " );
            errqstr( ch2str( nxtch ) );
            errstr( ")\n" );

        }

        /* output whole entity */
        outstr( infget_log( inpf ) );
    }

    return (BOOL)( !fatal_error );
}

/*
** parse_text
*/
BOOL parse_text( INFILE *inpf )
{
    STRPTR  nw = infgetcw( inpf );

    /* output whtspc */
    outstr( infgetcws( inpf ) );                 

    if ( !strcmp(nw, ">") ) {          /* unmatched ">"? */

        message( MSG_UNMA_GT, inpf );            /* Y->error message */
        errstr( "unmatched \">\"\n" );

    } else if ( rplc_ent ) {

        DLNODE *nd = find_dlnode( defent->first, (APTR) nw, cmp_rplcent );

        if ( nd ) {

            STRARR  entbuf[ 30 ]; /* TODO: replace with expstr */

            /* copy replaced entity to buffer */
            strcpy( entbuf, "&" );
            strncat( entbuf, ((HSCENT*)nd->data)->name, 30 );
            strncat( entbuf, ";", 30 );

            /* message */
            message( MSG_RPLC_ENT, inpf );
            errstr( "replaced " );
            errqstr( nw );
            errstr( " by " );
            errqstr( entbuf );
            errlf();

            nw = entbuf;

        }
    }


    outstr( nw );                /* output word */

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
    /* clear and enable log */
    if ( !inflog_clear( inpf ) )
        err_mem( inpf );
    else
        inflog_enable( inpf );

    if ( !fatal_error ) {
        char *nxtwd;

        nxtwd = infgetw( inpf );

        if ( nxtwd ) {

            if ( !strcmp(nxtwd, "<") )                     /* parse tag */
                parse_tag( inpf );
            else if ( !strcmp(nxtwd, "&") )                /* parse entity */
                parse_amp( inpf );

            else {                                         /* handle text */
                parse_text( inpf );
            }
        }
        inflog_disable( inpf );

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
    STRPTR name = (STRPTR)data;

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
BOOL include_hsc( STRPTR filename, INFILE *inpf, FILE *outf, ULONG optn )
{

    BOOL    ok;            /* result */
    ok   = (inpf!=NULL);

    if ( inpf ) {                                /* file opened? */

        inpf->is_nc = hsc_normch;                /*    set is_nc-methode */

        while ( !infeof(inpf) && ok ) {          /*    parse file */

            if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC) ) )
                status_infile( inpf, FALSE );    /*    status message */
            ok = parse_hsc( inpf );
        }

        if ( ok && (optn & IH_PARSE_END) ) {     /*    parse end */
                                                 /*    (unclosed tags etc) */
            ok = parse_end( inpf );

        }

        /* end of file status */
        if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)) ) {

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
BOOL include_hsc_file( STRPTR filename, FILE *outf, ULONG optn )
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
BOOL include_hsc_string( STRPTR filename, STRPTR s, FILE *outf, ULONG optn )
{
    INFILE *inpf;
    BOOL    ok;

    inpf = infopen_str( filename, s, 0 );        /* try to open input file */
    ok = include_hsc( filename, inpf, outf, optn );

    return( ok );
}

