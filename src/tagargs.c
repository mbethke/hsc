/*
**
** tagargs.c
**
** functions for parsing of tag arguments
**
** updated: 30-Aug-1995
** created: 29-Jul-1995
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/fname.h"
#include "ugly/infile.h"
#include "ugly/string.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"

#define PARENT_URL "../" /* string for parent dir within URLs */

char argbuf[ MAX_URLLEN ];             /* buffer for url names */
char relnam[ MAX_PATHLEN ];            /* result for get_relfname() */
char relurl[ MAX_PATHLEN ];            /* result for conv_path2url() */

/*
**-------------------------------------
** parse simple chars
**-------------------------------------
*/

/*
** parse_ch
**
** check if a expected char really occured and
** display error message if neccessary
**
** params: inpf.....input file to read char from
**         exptch...expected char
** result: TRUE if successful, FALSE if wrong char found
*/
BOOL parse_ch( INFILE *inpf, int exptch )
{
    /*
    ** TODO: read word, check last char, test whtspc
    */
    int ch = EOF;

    if ( !fatal_error ) {

        ch = infgetc( inpf );          /* read next char */

        if ( ch != exptch ) {              /* equal to expected char? */

            message( ERROR_UNEXPT_CH, inpf );        /* N->error message */
            errqstr( ch2str(exptch) );
            errstr( " expected, found " );
            errqstr( ch2str(ch) );
            errlf();

        }

    }

    return ( (BOOL)( (ch == exptch) && !fatal_error ) );
}


/*
** parse_eq
**
** check for '='
**
** params: inpf...input file to read char from
** result: -1 if successful, 0 if wrong char found
*/
BOOL parse_eq ( INFILE *inpf )
{
    return ( parse_ch(inpf,'=') );
}


/*
**-------------------------------------
** parse more complex strings
**-------------------------------------
*/

/*
** parse_tagoptn
*/
STRPTR parse_tagoptn( INFILE *inpf )
{
    STRPTR optn  = NULL;

    if ( !fatal_error ) {

        STRPTR nxtwd = infgetw( inpf );

        if ( nxtwd ) {

            if ( !strcmp( nxtwd, ">" ) ) {           /* end of tag reached? */

                /* TODO: unget wspc & word */
                inungetw( nxtwd, ' ', inpf );        /* Y->unget word */

            } else {

                optn = nxtwd;                        /* N->return word */

            }

        }

    }

    return (optn);
}

/*
** parse_quote
**
** read first word, check it to be a quote
**
** params: inpf...file to read word from
** result: quote char ('\'' or '\"') or EOF
**
*/
char parse_quote( INFILE *inpf )
{
    char   quote = EOF;
    STRPTR nxtwd = infgetw( inpf );

    if ( nxtwd ) {

        size_t wdlen  = strlen( nxtwd );  /* length of word */
        char   lastch = nxtwd[ wdlen-1 ]; /* last char of word */
        char   prevch = nxtwd[ wdlen-2 ]; /* 2nd last ch of word */

        if ( ( inf_isws( prevch, inpf )
             && ( lastch == '\'') || (lastch=='\"') ) )
        {
            quote = lastch;
        }

    }

    return ( quote );

}

/*
** parse_strarg
*/
STRPTR parse_strarg( INFILE *inpf )
{
    STRPTR strarg = NULL;

    if ( parse_ch(inpf, '=')
         && parse_ch(inpf, CH_QUOTE) )
    {

        size_t idx = 0;                /* index in strarg string */
        int    ch;                     /* next char to be read */

        /* clear buffer */
        memset( argbuf, 0, MAX_URLLEN );

        ch = infgetc( inpf );

        while ( !infeof(inpf) && (ch!=CH_QUOTE) ) {

            argbuf[idx] = ch;
            idx++;
            ch = infgetc( inpf );
        }

        if ( ch==CH_QUOTE ) {

            inungetc( ch, inpf );
            strarg = argbuf;

        }

    }

    if ( strarg && (!parse_ch(inpf,CH_QUOTE)) )
        strarg = NULL;

    return ( strarg );

}


/*
** parse mutual exclusion emum string
**
** this is eg used with the ALIGN option of <IMG>:
** the mutex-string for ALIGN is "TOP:MIDDLE:BOTTOM" (html1.0)
**
** params: line....description of the mutex-string (example above)
**         inpf....input file to read arg from
**         match...*ptr* to a bool var; TRUE, if arg read from
**                 input file matched any mutex-var
**         igcs....flag: if TRUE, upper/lower case will be ignored
** result: ptr to arg read; NULL if input error
**         if the arg matched any mutex-val, match is TRUE
*/
STRPTR parse_mutex( STRPTR line, INFILE *inpf, BOOL *match, BOOL igcs )
{
    STRPTR nxtwd = infgetw( inpf );    /* next word from input */
    STRPTR cmpwd = strtok( line, ":" );/* first word from mutex string */

    *match = FALSE;                    /* TRUE, if nxtwd matched cmpwd */

    while ( nxtwd && cmpwd ) {

        if ( igcs ) {
            if ( !upstrcmp( nxtwd, cmpwd ) )
                *match = TRUE;
        } else if ( !strcmp( nxtwd, cmpwd ) )
            *match = TRUE;

        cmpwd = strtok( NULL, ":" );

    }

    return ( nxtwd );

}

/*
** get_relfname
**
** convert absolut filename passed to <IMG> or <A HREF>
** to a relative filename (used with absolute URLs)
**
** params: absn...absolute filename passed to tag
**         curp...current path currently processed
**                hsc-file is in. (must end with "/")
** result: ptr to a buffer that contains the relative
**         path. this buffer is overwritten with the next call
*/
STRPTR get_relfname( STRPTR absnam, STRPTR curdir )
{
    char tmpb[ MAX_PATH ];   /* temp buffer */
    char fname[ MAX_FNAME ]; /* file name only */
    char abspa[ MAX_PATH ];  /* absolute path only */
    STRPTR tmpp1, tmpp2;     /* temp pointer */
    STRPTR rest_absp;        /* rest of current path */
    STRPTR absp;

    /*
    ** TODO: - handle out of mem
    **       - handle too long paths
    **       - optimise this routine; it contains
    **         some lines that are not really required
    **
    ** (frankly speaking, i don't really understand why
    ** this routine works, but it seems that is does)
    */

    /* init string array */
    relnam[0] = 0;
    strcpy( fname, get_fname( absnam ) );
    strcpy( abspa, get_fpath( absnam ) );
    absp = abspa;

    /*
    ** strip all equal subdirs
    */
    do {
        tmpp1 = get_nsdir( absp );
        strcpy( tmpb, tmpp1 );
        tmpp2 = get_nsdir( curdir );
        if (!upstrcmp( tmpb, tmpp2 )) {

            absp += strlen( tmpb );
            curdir += strlen( tmpp2 );

        }

    } while ( tmpb[0] && tmpp2[0] && (!upstrcmp( tmpb, tmpp2 )) );

    rest_absp = absp;


    /*
    ** for every subdir in absp unequal to
    ** corresponding subdir curdir, insert a parent dir
    */
    if ( curdir[0] )
        do {

            tmpp1 = get_nsdir( absp );
            strcpy( tmpb, tmpp1 );
            tmpp2 = get_nsdir( curdir );
            if ( upstrcmp( tmpb, tmpp2 )) {

                absp += strlen( tmpb );
                curdir += strlen( tmpp2 );
                strcat( relnam, PARENT_DIR );

            }


        } while ( tmpp2[0] && (upstrcmp( tmpb, tmpp2 )));

    strcat( relnam, rest_absp );
    strcat( relnam, fname );

    return ( relnam );
}

/*
** conv_path2url
**
** convert a path for local (system-dependant)
** file system to url
*/
STRPTR conv_path2url( STRPTR path )
{
    relurl[0] = '\0';               /* result for conv_path2url() */

    while ( !strncmp( path, PARENT_DIR, strlen(PARENT_DIR) ) ) {

        strcat( relurl, PARENT_URL );
        path += strlen( PARENT_DIR );

    }

    strcat( relurl, path );

    return ( relurl );
}


/*
** conv_url2path
**
** convert a url to a path for local (system-dependant)
** file system
*/
STRPTR conv_url2path( STRPTR url )
{
    relurl[0] = '\0';               /* result for conv_path2url() */

    while ( !strncmp( url, PARENT_URL, strlen( PARENT_URL ) ) ) {

        strcat( relurl, PARENT_DIR );
        url += strlen( PARENT_URL );

    }

    /*
    ** TODO: for MS-Dos, replace "/" by "\"
    */
    strcat( relurl, url );

    return ( relurl );
}




/*
** parse_url
**
** read an url-string, check it for syntatic correctnes.
** if the url refers to an local file, convert its absolute
** path to a relative path and check its existence.
**
** url = "rsrc_type://host.domain:port/pathname#name"
*/
STRPTR parse_url( INFILE *inpf )
{

    STRPTR rsrc = NULL;
    STRPTR host = NULL;
    STRPTR port = NULL;
    STRPTR path = NULL;
    STRPTR name = NULL;
    char dest_fname[MAX_PATHLEN]; /* destination file name that's existence */
                                  /* is checked if chkurl is enabled */
    char dest_url[MAX_URLLEN];    /* destination url name that is written to */
                                  /* the output file */
    STRPTR url = parse_strarg( inpf );

    if (url) {

        /* check for valid url */
        rsrc = strchr( url, ':' );
        if ( rsrc ) {

            /*
            ** check global url
            */
            if (!host) host = "";
            if (!port) port = "";
            if (!host) host = "";

            /*
            ** TODO:
            ** - parse global urls
            */


        } else {

            /*
            ** check local url
            */

            /* extract path and #name */
            if ( url[0] == '#' ) {
                path = "";
                name = url+1; /* skip '#' */
            } else {
                path = strtok( url, "#" );
                name = strtok( NULL, "" );
            }

            if ( path ) {

                FILE *exist;

                if ( absurl ) {

                    /*
                    **
                    ** parse absolute url
                    **
                    */
                    /* debug */
                    if (debug)
                        fprintf( stderr, "** exists %s\n", path );

                    /* check if local url exists */
                    strcpy( dest_fname, destdir );
                    strcat( dest_fname, path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   ->file %s\n", dest_fname );

                    /* create path of destination file */
                    strcpy( dest_url, rel_destdir );
                    strcat( dest_url, path );


                    path = get_relfname( path, rel_destdir );
                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   -> real path %s\n", path );

                    /* convert (filesystem depending) path to url */
                    url = conv_path2url( path );

                    /* debug */
                    if (debug)
                        fprintf( stderr, "**   -> real url  %s\n", url );

                } else { /* if (absurl) */

                    /*
                    ** parse relative url
                    */


                    /* debug */
                    if (debug)
                        fprintf( stderr, "** exists %s\n", path );

                    /* check if local url exists */
                    strcpy( dest_fname, destdir );
                    strcat( dest_fname, rel_destdir );
                    strcat( dest_fname, conv_url2path(path) );

                    /* create url (only copy path) */
                    strcpy( dest_url, path );
                    url = dest_url;

                    /* debug */
                    if (debug) {
                        fprintf( stderr, "**   -> real path %s\n", dest_fname );
                        fprintf( stderr, "**   -> real url  %s\n", dest_url );
                    }
                }

                /*
                **check existence of local url
                */
                if ( chkurl ) {

                    exist = fopen( dest_fname, "r" );
                    if ( !exist ) {

                        message( ERROR_NO_URLPATH, inpf );
                        errstr( "path to URL not found:" );
                        errqstr( dest_fname );
                        errlf();

                    } else
                        fclose( exist );
                }

            } else { /* if (path) */

               path = "";

            }

            /* add #name part */
            if ( name ) {
                strcat( url, "#" );
                strcat( url, name );
            }

        } /* else (rsrc) */

    } /* if (url) */

    return ( url );
}





