/*
** ugly/infile.c
**
** ugly input file functions
**
** Version 1.3.0, (W) by Tommy-Saftwörx in 1995
**
** updated:  8-Sep-1995
** created:  8-Jul-1995
**
** $VER: infile.c 1.3.0 (7.9.1995)
**
*/

/*
** TODO:
**
** - handle errors within expstr (no mem)
*/

/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "expstr.h"
#include "string.h"

#define NOEXTERN_UGLY_FILE_H
#include "infile.h"
#include "memory.h"

#define MAX_IFLINELEN 511

/* buffer size for fgets() in infgetc() */
#define INF_FGETS_BUFSIZE 32

/*
** global vars
*/
STRPTR FNAME_STDIN="<stdin>";          /* filename for stdin (CONSTANT) */


/*
** local vars
*/

/* forward references */
BOOL infget_skws( INFILE *inpf );

/*
**
** local functions (not exported into header)
**
*/

/*
** default_whtspc, default_normch
**
** default methods for infgetw() to determine if
** a char is a whitespace or normal char.
*/
BOOL default_whtspc( char ch )
{
    if ( strchr(" \t",ch) != NULL )
        return TRUE;
    else return FALSE;
}

BOOL default_normch( char ch )
{
    if ( isalnum(ch) || (ch=='_') )
        return TRUE;
    else return FALSE;
}


/*
**
** exported functions
**
*/


/*
**-------------------------------------
** functions to get info about infile
**-------------------------------------
*/

/*
** infget_x
**
** get colum of current line
*/
ULONG infget_x( INFILE *inpf )
{
    return (inpf->lnctr);
}


/*
** infget_y
**
** get colum of current line
*/
ULONG infget_y( INFILE *inpf )
{
    return (inpf->flnctr);
}


/*
** infget_skws
**
** get status of skipped_ws
*/
BOOL infget_skws( INFILE *inpf )
{
    return (inpf->skipped_ws);
}


/*
** infget_fname
**
** get name of file
*/
STRPTR infget_fname( INFILE *inpf )
{
    return (inpf->filename);
}


/*
** infeof
**
** check if end of input file reached
**
** params: inpf...input file to check
** result: if end of input file reached, 0 is returned,
**         else a value < 0 (compare feof() of some
**         compilers)
*/
int infeof( INFILE *inpf )
{
    if ( inpf->eof_reached == TRUE )
        return EOF;
    else
        return 0;
}

/*
** infget_cws (get current whites spaces)
**
** get string that contains all white-spaces
** skipped within the last call of infgetw()
*/
STRPTR infgetcws( INFILE *inpf )
{
    return ( estr2str( inpf->wspcbuf ) );
}

/*
** infget_cw (get current word)
**
** get string that contains all chars
** read within the last call of infgetw()
*/
STRPTR infgetcw( INFILE *inpf )
{
    return ( estr2str( inpf->wordbuf ) );
}


/*
**-------------------------------------
** functions to set methods
**-------------------------------------
*/

/*
** set_inf_whtspc
**
** set function to check if a char is a whitespace.
** this func is called by infgetw() to determine if
** the begining of a word is reached.
*/
void set_whtspc( INFILE *inpf, BOOL (*iswsfn)( char ch ) )
{
    if ( inpf ) inpf->is_ws = iswsfn;
}


/*
** set_inf_normch
**
** set function to check if a char is a "normal" char.
** this function is called by infgetw() to determine if
** the end of a word is reached
*/
void set_normch( INFILE *inpf, BOOL (*isncfn)( char ch ) )
{
    if ( inpf ) inpf->is_nc = isncfn;
}


/*
**-------------------------------------
** functions for open & close
**-------------------------------------
*/

/*
** infclose1
**
** close INFILE entry, free all mem allocated by it
**
** result: normaly, the result of fclose(); if there
**         was no file open, 0 is returned.
**
** NOTE: if inpf was assigned to stdin, no file is
**       closed.
**
** NOTE: this function is usually called by the macro
**       ufclose() defined in <ugly/file.h>
*/
int infclose1( INFILE *inpf )
{
  int result = 0;

    if ( inpf ) {

        if ( inpf->infile )            /* close input file, free filename */
            if ( inpf->infile != stdin ) {

                result = fclose( inpf->infile );

            } else {

                inpf->infile = NULL;
                inpf->filename = NULL;

            }

        if ( inpf->infile != stdin )   /* free filename */
            ufreestr( inpf->filename );
        del_estr( inpf->lnbuf );       /* free line buffer */
        del_estr( inpf->wordbuf );     /* free word buffers */
        del_estr( inpf->wspcbuf );
        del_estr( inpf->logstr );
        inpf->lnctr       = 0;         /* reset vars */
        inpf->flnctr      = 0;
        inpf->eof_reached = FALSE;
        inpf->skipped_ws  = FALSE;
        inpf->log_enable  = FALSE;
        inpf->is_nc       = NULL;
        inpf->is_ws       = NULL;
        ufree( inpf );                 /* free whole struct */

    }

    return ( result );
}


/*
** infopen
**
** result: ptr to INFILE entry or NULL if error;
**
** NOTE: if result = NULL, but errno=0, too, then there
**       wasn't enough mem for the line-buffer. in this
**       case, calling perror() will not produce a proper
**       error message.
**
** NOTE: don't forget to set errno=0 before calling infopen().
*/
INFILE *infopen( CONSTRPTR fname, size_t bufsize )
{
    INFILE *uinpf = NULL;              /* return file */
    FILE   *inpf;                      /* normal input file */
    STRPTR  fnm;


    /*
    ** init INFILE structure
    */
    uinpf = (INFILE *) malloc( sizeof(INFILE) );
    if ( uinpf ) {

        uinpf->infile      = NULL;
        uinpf->filename    = NULL;
        uinpf->lnbuf       = init_estr();
        uinpf->wordbuf     = init_estr();        /* reset wordbuffers */
        uinpf->wspcbuf     = init_estr();
        uinpf->logstr      = init_estr();
        uinpf->lnctr       = 0;
        uinpf->flnctr      = 0;
        uinpf->eof_reached = FALSE;
        uinpf->skipped_ws  = FALSE;
        uinpf->log_enable  = FALSE;
        uinpf->is_nc       = NULL;
        uinpf->is_ws       = NULL;

    }


    /*
    ** open input file or assign stdin to input file,
    ** clone filename
    */
    if ( fname ) {

        inpf = fopen( fname, "r" );
        fnm  = strclone( fname );

    } else {

        inpf = stdin;
        fnm  = FNAME_STDIN;

    }


    /*
    ** build result or handle errors
    */
    if ( uinpf ) {
        uinpf->infile   = inpf;                  /* Y-> build result */
        uinpf->filename = fnm;
    }

    if ( (!inpf)                                 /* error? */
         || (!fnm)
         || (!uinpf)
         || (!uinpf->lnbuf)
         || (!uinpf->wordbuf )
         || (!uinpf->wspcbuf )
         || (!uinpf->logstr ) )
    {                                            /* Y-> clean up */

        if ( uinpf ) infclose ( uinpf )
        else if ( inpf ) fclose( inpf );
        uinpf = NULL;

    }

    return uinpf;                      /* return result */
}

/*
**-------------------------------------
** functions to get text from infile
**-------------------------------------
*/

/*
** infgetc
**
**
*/
int infgetc( INFILE *inpf )
{
    int result = EOF;

    if ( inpf && (!inpf->eof_reached) ) {

        STRPTR lnbuf_str = estr2str( inpf->lnbuf );
        BOOL   ok;          /* result of expstr-funcs */

        /*
        ** if at end of line buffer, scan next line
        ** before proceding
        */
        if ( lnbuf_str[inpf->lnctr] == 0 ) {

            STRARR buf[ INF_FGETS_BUFSIZE ];
            STRPTR restr = buf; /* result of fgets() (dummy init)*/
            BOOL   eol = FALSE; /* flag: end of line reached */

            /* read next input line into buffer (expstr) */
            ok = clr_estr( inpf->lnbuf );
            while (ok && restr && (!eol) ) {

                restr = fgets( buf, INF_FGETS_BUFSIZE, inpf->infile );
                if ( restr ) {

                    ok &= app_estr( inpf->lnbuf, restr );
                    eol = (restr[strlen(restr)] != '\n' );

                } else ok = (!feof( inpf->infile ) );

            }

            /* reset line counter if LF found at end of line */
            if ( eol ) {

                inpf->lnctr = 0;
                inpf->flnctr++;

            }

            /*
            ** check for end of file or error,
            ** set eof-flag if neccessary
            */
            if ( !ok )
                inpf->eof_reached = TRUE;

        }

        /*
        ** check wether to return EOF or char from
        ** line buffer
        */
        if ( inpf->eof_reached == FALSE ) {

            lnbuf_str = estr2str( inpf->lnbuf );
            result = lnbuf_str[inpf->lnctr];     /* set last char as result */
            if ( result ) inpf->lnctr++;         /* goto next char in buf */

        }

        /* update log-string if neccessary */
        if ( (result != EOF) && (inpf->log_enable) )
            ok &= app_estrch( inpf->logstr, result );


        if ( !ok )
            /* TODO: handle out of mem */;
    }

    return result;
}

/*
**-------------------------------------
** functions to unget text from infile
**-------------------------------------
*/

/*
** inungetc
**
** write char back to stream; comparable to ansi's ungetc()
**
** params: inpf...input file
**         ch.....char to write back
** result: ch, if sucessful, else EOF
*/
int inungetc( int ch, INFILE *inpf )
{
    int result = EOF;

    if ( inpf && ( inpf->lnctr ) ) {

        STRPTR lnbuf_str = estr2str( inpf->lnbuf );

        inpf->lnctr--;
        lnbuf_str[inpf->lnctr] = ch;
        result = ch;

        /* unget in logstr */
        if ( (inpf->log_enable) && ( estrlen( inpf->logstr ) ) )
            if ( !get_left_estr( inpf->logstr, inpf->logstr, estrlen(inpf->logstr)-1 ) )
                /* TODO: handle out of mem */;
    }

    return( result );
}

/*
** inungets
** write string back to stream
**
** params: s......string to write back
**         inpf...input file
** result: num of chars written back
*/
size_t inungets( STRPTR s, INFILE *inpf )
{
    size_t ctr = 0;               /* counter, how many chars already written */
    size_t slen = strlen(s);
    STRPTR p=s+(strlen(s)-1);     /* ptr to current char in string */
    int    ch = 0;                /* current char written, dummy init */

    if ( slen > 0 ) {

        ctr = 1;                            /* unget first char */
        ch = inungetc( p[0], inpf );

        while ( (p!=s) && (ch!=EOF) ) {

            ctr++;                          /* inc counter */
            p--;                            /* goto next char */
            ch = inungetc( p[0], inpf );    /* unget current char */

        } 

    }

    return (ctr);
}


/*
** inungetcw
** write current word back to stream
**
** params: inpf...input file
** result: num of chars written back
*/
size_t inungetcw( INFILE *inpf )
{
    size_t ctr; /* counter how many chars written */

    /* unget word */
    ctr = inungets( infgetcw( inpf ), inpf );

    return (ctr);
}

/*
** inungetcwws
** write current word & whitespaces back to stream
**
** params: inpf...input file
** result: num of chars written back
*/
size_t inungetcwws( INFILE *inpf )
{
    size_t ctr; /* counter how many chars written */

    /* unget word & white spaces */
    ctr = inungets( infgetcw( inpf ), inpf );
    ctr += inungets( infgetcws( inpf ), inpf );

    return (ctr);
}

/*
** inf_isws
**
** checks if a char is a white space
**
** params: ch...char to check for white space
** result: TRUE if ch was white space
*/
BOOL inf_isws( char ch, INFILE *inpf )
{
    BOOL   (*isws)(char ch) = inpf->is_ws;

    if (isws==NULL)                    /* if no func for is_ws, */
        isws = default_whtspc;         /*   set default function */

    return ( (*isws)(ch) );
}

/*
** infskip_ws
**
** skip white spaces; update wspbuf; clear wordbuf
**
** TODO: handle wspcbuf-overflow
*/
size_t infskip_ws( INFILE *inpf )
{
    char   nxtch;                      /* next char to read */
    size_t ctr = 0;                    /* num of ws skipped */
    BOOL ok;

    /*
    ** set function to determine if a
    ** specified char is a white space
    */
    inpf->skipped_ws = FALSE;          /* clear skippe-flag */

    /* clear wspcbuf */
    ok = clr_estr( inpf->wspcbuf );

    /*
    ** loop:  skip white spaces
    */
    nxtch = infgetc(inpf);             /* read next char */
    while ( (!infeof(inpf))            /* while not at end of file.. */
            && ok
            && inf_isws(nxtch,inpf) )  /* ..and current char is a whtspc */
    {
        ok &=app_estrch( inpf->wspcbuf, nxtch );
        ctr++;
        nxtch = infgetc(inpf);         /*   read next char */
    }

    if ( ctr )                         /* any whtspcs skipped? */
        inpf->skipped_ws = TRUE;       /* Y-> set skippe-flag */

    if ( !ok )
        /* TODO: error */;

    /*
    ** write back last char read
    */
    inungetc( nxtch, inpf );

    return( ctr );
}




/*
** infgetw
**
** read word
*/
STRPTR infgetw( INFILE *inpf )
{
    /* TODO: handle expstr errors */
    int ch = EOF;
    BOOL   wordread = FALSE;
    STRPTR thisword = NULL;
    BOOL   ok = TRUE;

    /* set function for normal chars */
    BOOL (*isnc)(char ch) = inpf->is_nc;
    if (isnc==NULL) isnc = default_normch;

    /* skip all white spaces */
    infskip_ws( inpf );

    ok = clr_estr( inpf->wordbuf );

    /*
    ** read word until non-normal char is reached
    */
    if ( !infeof(inpf) ) {

        ch = infgetc( inpf );

        if ( ((*isnc)(ch)) )
            do  {

                ok &= app_estrch( inpf->wordbuf, ch );
                ch = infgetc( inpf );
                wordread = TRUE;
                /* todo: set out-of-mem-flag */

            } while ( (ch!=EOF) && ok && ((*isnc)(ch)) );
        else
            ok &= app_estrch( inpf->wordbuf, ch );

        if ( (ch!=EOF) && (wordread) )
            inungetc( ch, inpf );

        if ( ch != EOF );
        thisword = estr2str( inpf->wordbuf );

    } else
        thisword = NULL;

    return ( thisword );
}


/*
** infgotoeol
**
** read all chars until CR or EOF
*/
int infgotoeol( INFILE *inpf )
{
    int ch = EOF;

    if ( !infeof( inpf ) ) {

        /*
        ** read all chars until CR appears
        */
        do {

            ch = infgetc( inpf );

        } while ( (ch > 0) && (ch!=0x0a) );

        /*
        ** read LF
        */
        if ( ch == 0x0a ) {

            ch = infgetc( inpf );
            if ( ch != 0x0d )
                inungetc( ch, inpf );
            ch = 0x0a;
        }

    }

    /*
    ** return last char read
    */
    return (ch);
}

/*
**-------------------------------------
** functions for logstr
**-------------------------------------
*/

/* enable log */
void inflog_enable( INFILE *inpf )
{
    inpf->log_enable = TRUE;
}

/* disable log */
void inflog_disable( INFILE *inpf )
{
    inpf->log_enable = FALSE;
}

/* clear log-string */
BOOL inflog_clear( INFILE *inpf )
{
    /* TODO: handle out of mem */
    return ( clr_estr( inpf->logstr ) );
}

/* get log-string */
STRPTR infget_log( INFILE *inpf )
{
    return ( estr2str( inpf->logstr ) );
}


