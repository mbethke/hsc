/*
** tag_macr.c
**
** tag handles for "<$>" (macro)
**
** updated:  6-Aug-1995
** created:  5-Aug-1995
*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/memory.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"

char macarg[ MAX_ARGLEN ];

/*
** parse_macstr
**
** parses string for macro-definition and argument
** - read first word, check it to be a quote ('\"' or '\'')
** - remember which type of quote the first quote was (var: quote)
** - read (char-wise) rest of string, until first quote appears again
*/
STRPTR parse_macstr( INFILE *inpf )
{
    int    nxtch = EOF;                /* next char read */
    int    quote = EOF;                /* quote char */
    size_t idx   = 0;                  /* pos. in macarg */


    if ( parse_eq( inpf ) )
        quote = parse_quote( inpf );


    if ( quote != EOF ) {

        nxtch = infgetc( inpf );
        while ( (nxtch != EOF) && (nxtch != quote) ) {

            macarg[idx] = nxtch;              /* store char */
            idx++;
            if ( idx != (MAX_ARGLEN-1) )
                nxtch = infgetc( inpf );
            else {

                nxtch = EOF;
                message( FATAL_LONG_STR, inpf );
                errstr( "Argument string too long" );

            }

        }

    }

    macarg[idx] = '\0';
    if ( nxtch != EOF )
        return ( macarg );
    else
        return ( NULL );
}

/*
**
** exported funs
**
*/


/*
** insert_macro
**
** insert already defined macro:
** - write macro text to a temp. output file
** - perform an include_hsc() on this file
*/
BOOL insert_macro( HSCMAC *macro, INFILE *inpf )
{
    BOOL    ok     = FALSE; /* result var */
    STRPTR  nambuf = NULL;  /* name of temp. file */
    STRPTR  nam    = NULL;  /* result of tmpnam() (probably not neccessary */
    FILE   *tmp_outfile = NULL; /* temp. file macro text is written to */

    /*
    ** open tmpfile
    */
    nambuf = (STRPTR) malloc( L_tmpnam );
    if ( nambuf )
        nam = tmpnam( nambuf );
    if ( nam )
        tmp_outfile = fopen( nam, "w" );


    if ( debug )
        fprintf( stderr, "** expand macro %s into file \"%s\" as \"%s\"\n",
                 macro->name, nam, macro->text );

    /* write macro text to temp. outfile */
    if ( tmp_outfile ) {

        ok = ( fputs( macro->text, tmp_outfile ) != EOF );
        fclose( tmp_outfile );

    }

    /* include temp. outfile */
    if ( ok ) {

        ok = include_hsc( nam, outfile, IH_PARSE_MACRO );

    } else {

        /* error expanding macro */
        message( ERROR_EXPAND_MACRO, inpf );
        errstr( "Can not expand macro \n" );
        /* TODO: display macro name */

    }

    /* cleanup */
    if ( tmp_outfile )
        remove( nam );
    ufree( nambuf );

    return (ok);
}


/*
** create_new_macro
**
** read a decription text for a new macro,
** alloc & init the new macro
*/
BOOL create_new_macro( STRPTR mname, DLNODE *nd, INFILE *inpf)
{
    HSCMAC *macro = NULL;
    STRPTR  mtext = parse_macstr( inpf );        /* read macro text */

    /* remove existing macro */
    if ( mtext && nd ) {

        del_dlnode( macros, nd );

        message( MSG_RPLC_MACRO, inpf );
        errstr( "Replacing existing macro " );
        errqstr( mname );
        errlf();

    }

    /* append new macro */
    if ( mtext ) {

        /* create new macro */
        macro =  new_hscmac( mname, mtext );
        if ( macro ) {

            /* add new macro to macro list */
            if ( !app_dlnode( macros, macro ) ) {
                /* out of mem */
                /* TODO: handle this case */
            }

        }
    }

    return ( (BOOL)(macro!=NULL) );
}

/*
** handle_hsc_macro
**
** define a new macro or extract it
*/
BOOL handle_hsc_macro( INFILE *inpf )
{
    DLNODE *nd;
    STRPTR mname   = NULL;
    STRPTR nxtoptn = NULL;

    nxtoptn = parse_tagoptn( inpf );
    mname = strclone( nxtoptn );

    if ( mname ) {

        nd = find_dlnode( macros->first, (APTR) mname, cmp_strmac );

        nxtoptn = parse_tagoptn( inpf );

        if ( nxtoptn ) {

            if ( !upstrcmp("SRC",nxtoptn) )
                /* create a new macro */
                create_new_macro( mname, nd, inpf );
            else {
                /* TODO: handle other options */
            }

        } else {

            if ( nd )
                /* extract a macro and write it to output */
                insert_macro( (HSCMAC*)nd->data, inpf );
            else {

                /* unknown macro: display message */
                message( MSG_UNKN_MACRO, inpf );
                errstr( "Unknown macro " );
                errqstr( mname );
                errlf();

            }

        }
        ufreestr( mname );

    } else {

        /* TODO: out of mem */

    }

    skip_until_gt( inpf );

    return TRUE;
}

