/*
** ugly/infile.c
**
** ugly input file functions
**
** Version 1.2.1, (W) by Tommy-Saftwörx in 1995
**
** updated:  5-Aug-1995
** created:  8-Jul-1995
**
** $VER: infile.c 1.2.1 (5.8.1995)
**
*/

/*
** TODO:
**
** - handle whtspc correctly when inungetw()
** - own wordbuf & wspcbuf for every infile
** - dynamically allocated wordbuf (autoexpandable)
*/

/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "string.h"

#define NOEXTERN_UGLY_FILE_H
#include "infile.h"
#include "memory.h"

#define MAX_IFLINELEN 511

/*
** global vars
*/
STRPTR FNAME_STDIN="<stdin>";          /* filename for stdin (CONSTANT) */


/*
** local vars
*/
char wordbuf[MAX_IFLINELEN]; /* buffers written by ingetw */
char wspcbuf[MAX_IFLINELEN];

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
    if ( !infget_skws( inpf ) ) {

        wspcbuf[0] = '\0';

    }

    return ( wspcbuf );
}

/*
** infget_cw (get current word)
**
** get string that contains all chars
** read within the last call of infgetw()
*/
STRPTR infgetcw( INFILE *inpf )
{
    return ( wordbuf );
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
        ufree( inpf->lnbuf );          /* free line buffer */
        inpf->lnbufsz     = 0;         /* reset vars */
        inpf->lnctr       = 0;
        inpf->flnctr      = 0;
        inpf->eof_reached = FALSE;
        inpf->skipped_ws  = FALSE;
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
        uinpf->lnbuf       = (char *) malloc( bufsize+1 );
        uinpf->lnbufsz     = bufsize;
        uinpf->lnctr       = 0;
        uinpf->flnctr      = 0;
        uinpf->eof_reached = FALSE;
        uinpf->skipped_ws  = FALSE;
        uinpf->is_nc       = NULL;
        uinpf->is_ws       = NULL;

        memset( uinpf->lnbuf, 0, bufsize+1 );    /* clear buffer */

        wordbuf[0] = '\0';             /* reset wordbuffers */
        wspcbuf[0] = '\0';

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
         || (!uinpf->lnbuf) )
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

    if ( inpf && (inpf->eof_reached == FALSE) ) {

        /*
        ** if at end of line buffer, scan next line
        ** before proceding
        */
        if ( inpf->lnbuf[inpf->lnctr] == 0) {

            STRPTR restr;               /* result of fgets() */

            /*
            ** read next input line into buffer,
            ** reset line counter
            */
            restr = fgets( inpf->lnbuf, inpf->lnbufsz, inpf->infile );
            inpf->lnctr = 0;
            inpf->flnctr++;

            /*
            ** check for end of file or error,
            ** set eof-flag if neccessary
            */
            if ( restr != inpf->lnbuf )
                inpf->eof_reached = TRUE;

        }

        /*
        ** check wether to return EOF or char from
        ** line buffer
        */
        if ( inpf->eof_reached == FALSE ) {

            result = inpf->lnbuf[inpf->lnctr];   /* set last char as result */
            if ( result ) inpf->lnctr++;         /* goto next char in buf */

        }
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

        inpf->lnctr--;
        inpf->lnbuf[inpf->lnctr] = ch;
        result = ch;

    }

    return result;
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
** inungetw
** write word back to stream
**
** params: s......word to write back
**         ws.....char to be used as white space
**         inpf...input file
** result: num of chars written back
*/
size_t inungetw( STRPTR s, char ws, INFILE *inpf )
{
    size_t ctr;                        /* counter how many chars written */

    /* unget word */
    ctr = inungets( s, inpf );

    /* unget white space */
    if ( (ctr==strlen(s)) && infget_skws(inpf) )
        if ( inungetc(ws,inpf) != EOF )
            ctr++;

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
** skip white spaces; update wspbuf
**
** TODO: handle wspcbuf-overflow
*/
size_t infskip_ws( INFILE *inpf )
{
    char   nxtch;                      /* next char to read */
    size_t ctr = 0;                    /* num of ws skipped */
    /*
    ** set function to determine if a
    ** specified char is a white space
    */
    inpf->skipped_ws = FALSE;          /* clear skippe-flag */

    /*
    ** loop:  skip white spaces
    */
    nxtch = infgetc(inpf);             /* read next char */
    while ( (!infeof(inpf))            /* while not at end of file.. */
            && inf_isws(nxtch,inpf) )  /* ..and current char is a whtspc */
    {
        wspcbuf[ctr] = nxtch;          /*   update wspc-buffer */
        nxtch = infgetc(inpf);         /*   read next char */
        ctr++;                         /*   inc counter */
    }

    if (ctr) {                         /* any whtspcs skipped? */

        inpf->skipped_ws = TRUE;       /* Y-> set skippe-flag */
        wspcbuf[ctr] = '\0';         /*     mark end of wspc-buffer */

    } else
        wspcbuf[ctr] = '\0';           /*     mark end of wspc-buffer */

    /*
    ** write back last char read
    */
    inungetc( nxtch, inpf );

    return (ctr);
}




/*
** infgetw
**
** read word
*/
STRPTR infgetw( INFILE *inpf )
{
    int ch = EOF;
    size_t wordctr = 0;

    /* set function for normal chars */
    BOOL (*isnc)(char ch) = inpf->is_nc;
    if (isnc==NULL) isnc = default_normch;

    /* skip all white spaces */
    infskip_ws( inpf );

    /*
    ** read word until non-normal char is reached
    */
    if ( !infeof(inpf) ) {

        ch = infgetc( inpf );
        if ( !((*isnc)(ch)) ) {

            wordbuf[0] = ch;
            wordbuf[1] = '\0';

        } else do  {

            wordbuf[wordctr] = ch;
            wordctr++;
            wordbuf[wordctr] = '\0';
            ch = infgetc( inpf );

        } while ( (ch!=EOF) && ((*isnc)(ch)) );

        if ( (ch!=EOF) && (wordctr) )
            inungetc( ch, inpf );
        else
            wordbuf[wordctr-1] = '\0';

    }

    return ( wordbuf );

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


