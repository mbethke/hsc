/*
** parse.c
**
** parse file: handle for entities & tags
**
** updated:  4-Sep-1995
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
#include "find.h"
#include "msgid.h"
#include "status.h"

#define NOEXTERN_PARSE_H
#include "parse.h"

INFILE *parse_end_file; /* file for do_list-methods called in parse_end() */


/*
**---------------------------
** parse functions
**---------------------------
*/

/*
** parse_tag
**
** parse tag (after "<")
*/
BOOL parse_tag( INFILE *inpf)
{
    if ( !fatal_error ) {

        STRPTR  nxtwd;
        DLNODE *nd;

        /* write white space (if any) */
        outstr( infgetcws( inpf ) );

        strcpy( this_tag, infgetcw( inpf ) );

        /* get tag id */
        nxtwd = infgetw( inpf );                 /* get tag id */

        /* remember tag id (used by some handlers, which have */
        /* HT_NOCOPY set, but need to write tagid (eg <A>) */
        strcat( this_tag, infgetcws( inpf ) );
        strcat( this_tag, infgetcw( inpf ) );
        this_tag_data = NULL;

        if ( debug )                         /* debug msg */
            fprintf( stderr, "** tag ", this_tag );

        if ( strcmp( "/", nxtwd ) ) {            /* is it a closing tag? */

            /*
            **
            ** process non-closing tag
            **
            */

            if ( debug )                         /* debug msg */
                fprintf( stderr, "%s>\n", this_tag );

            /* search for tag in list */
            nd = find_dlnode( deftag->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                message( WARN_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown tag " );
                errtag( nxtwd );
                errlf();

                outstr( this_tag );                  /* copy whole tag */
                copy_until_gt( inpf );

            } else {

                HSCTAG *tag = (HSCTAG*)nd->data;
                BOOL   (*hnd)(INFILE *inpf) = tag->o_handle;

                /* set global pointer to tag */
                this_tag_data = tag;

                /*
                ** handle options
                */

                /* only-once-tag occured twice? */
                if ( (tag->option & HT_ONLYONCE ) && (tag->occured) ) {

                    message( ERROR_TOO_OFTEN, inpf );
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

                /* write out tag? */
                if ( !(tag->option & HT_NOCOPY) )
                    outstr( this_tag );                        /* write out tag */

                /*
                ** call handle if available
                */
                if ( hnd && !fatal_error)
                    (*hnd)( inpf );                            /* call handle */

                /*
                ** copy rest of tag if HT_COPY-bit not set
                */
                if ( !(tag->option & HT_NOCOPY) )              /* write out '>' */
                    copy_until_gt( inpf );

            }

        } else {

            /*
            **
            ** process closing tag
            **
            */

            /* TODO: handle </HR> */
            /* TODO: check why ^ this TODO is here?! */

            /* get tag id */
            nxtwd = infgetw( inpf );           /* get tag id */

            /* remember tag id */
            strcat( this_tag, infgetcws( inpf ) );
            strcat( this_tag, infgetcw( inpf ) );

            if ( debug )                         /* debug msg */
                fprintf( stderr, "%s>\n", this_tag );

            /* search for tag in deftag list */
            /* (see if it exists at all) */
            nd = find_dlnode( deftag->first, (APTR) nxtwd, cmp_strtag );
            if ( nd == NULL ) {

                /* closing tag is absolutely unknown */
                message( WARN_UNKN_TAG, inpf );      /* tag not found */
                errstr( "Unknown closing tag " );
                errctag( nxtwd );
                errlf();

                outstr( this_tag );                  /* copy whole tag */
                copy_until_gt( inpf );

            } else {

                HSCTAG *tag = (HSCTAG*)nd->data; /* fitting tag in deftag-list */
                BOOL   (*hnd)(INFILE *inpf) = tag->c_handle; /* closing handle */

                /* set global pointer to tag */
                this_tag_data = tag;

                /* search for tag on stack of occured tags */
                nd = find_dlnode( cltags->first, (APTR) nxtwd, cmp_strctg );
                if ( nd == NULL ) {

                    /* closing tag not found on stack */
                    /* ->unmatched closing tag without previous opening tag */
                    message( WARN_UNMA_CTAG, inpf );
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

                        message( WARN_NEST_TAG, inpf );
                        errstr( "Illegal closing tag nesting (expected " );
                        errctag( lastnm );
                        errstr( ", found " );
                        errctag( nxtwd );
                        errch( ')' );
                        errlf();

                    }

                    /* call closing handle if available */
                    if ( hnd )
                        (*hnd)( inpf );
                }

                /* write out whole closing tag */
                if ( !(tag->option & HT_NOCOPY) ) {

                    outstr( this_tag );
                    copy_until_gt( inpf );

                }

                /* remove node for closing tag from cltags-list */
                del_dlnode( cltags, nd );
            }
        }
    }

    return (BOOL)( !fatal_error );
}


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

        /* write blank if necessary */
        outstr( infgetcws( inpf ) );

        /* get entity id */
        nxtwd = infgetw( inpf );

        /* search for entity in list */
        nd = find_dlnode( defent->first, (APTR) nxtwd, cmp_strent );
        if ( nd == NULL ) {

            message( WARN_UNKN_ENTITY, inpf );
            errstr( "Unknown entity " );
            errqstr( nxtwd );
            errlf();

        }

        /* check for closing ';' */
        nxtch = infgetc( inpf );
        if ( nxtch != ';' ) {

            message( WARN_EXPT_SEMIK, inpf );
            errqstr( ";" );
            errstr( " expected (found: " );
            errqstr( ch2str( nxtch ) );
            errstr( ")\n" );

        }

        /* output whole entity */
        outch( '&' );
        outstr( nxtwd );
        outch( nxtch );
    }

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

        if ( nxtwd ) {

            if ( !strcmp(nxtwd, "<") )                     /* parse tag */
                parse_tag( inpf );
            else if ( !strcmp(nxtwd, "&") )                /* parse entity */
                parse_amp( inpf );

            else {                                         /* handle text */

                if ( !strcmp(nxtwd, ">") ) {               /* unmatched ">"? */

                    message( ERROR_UNMA_GT, inpf );        /* Y->error message */
                    errstr( "unmatched \">\"\n" );

                }

                outstr( infgetcws( inpf ) );               /* output word */
                if (!infeof(inpf))
                    outstr( infgetcw( inpf ) );


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
        message( WARN_MISS_RTAG, parse_end_file );
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

    message( WARN_MISS_CTAG, parse_end_file );
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
BOOL include_hsc( STRPTR inpfnm, FILE *outf, ULONG optn )
{

    INFILE *inpf  = NULL;  /* input file */
    BOOL    ok;            /* result */

    inpf = infopen( inpfnm, MAXLINELEN );        /* open input file */
    ok   = (inpf!=NULL);

    if ( inpf ) {                                /* file opened? */

        while ( !infeof(inpf) && ok ) {          /*    parse file */

            if ( !(optn & IH_PARSE_MACRO) )
                status_infile( inpf, FALSE );    /*    status message */
            ok = parse_hsc( inpf );
        }

        if ( ok && (optn & IH_PARSE_END) ) {     /*    parse end */
                                                 /*    (unclosed tags etc) */
            ok = parse_end( inpf );

        }

        /* end of file status */
        if ( !(optn & IH_PARSE_MACRO) ) {

            status_infile( inpf, TRUE );         /*    status message */
            status_lf();                         /*    (display num of line) */

        }

        infclose( inpf );                        /*    close file */
                                                 
    } else {                                     /* N-> error message */

        message( ERROR_FILE_OPEN, NULL );
        errstr( "can not open " );
        errqstr( inpfnm );
        errstr( " for input: " );
        errstr( strerror( errno ) );
        errlf();
        ok = FALSE;
    }

    return ( ok );
}


