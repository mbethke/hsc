/*
**
** tagargs.c
**
** functions for parsing of tag arguments
**
** updated: 30-Oct-1995
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

#include "vars.h"


/*
**-------------------------------------
** parse simple chars/words
**-------------------------------------
*/

/*
** parse_wd
**
** check if a expected word really occured and
** display error message if neccessary
**
** params: inpf.....input file to read char from
**         expstr...expected word
** result: TRUE if successful, FALSE if wrong char found
*/
BOOL parse_wd( INFILE *inpf, STRPTR expstr )
{
    BOOL   value = TRUE;

    if ( expstr ) {

        STRPTR nw;

        /* skip LFs */
        do
          nw = infgetw( inpf );
        while ( nw && !strcmp( nw, "\n" ) );

        /* check for expeted word */
        if ( !nw || upstrcmp( nw, expstr ) ) {

            message( MSG_UNEXPT_CH, inpf );
            errstr( "expected " );
            errqstr( expstr );

            if ( nw ) {

                errstr( ", found " );
                errqstr( nw );

            }

            errlf();
            value = FALSE;
        }
    }

    return( value );
}


/*
** parse_eq
**
** check for '='
**
** params: inpf...input file to read char from
** result: -1 if successful, 0 if wrong char found
*/
BOOL parse_eq( INFILE *inpf )
{
    return ( parse_wd(inpf,"=") );
}

/*
** parse_gt
**
** check for '>'
**
** params: inpf...input file to read char from
** result: -1 if successful, 0 if wrong char found
*/
BOOL parse_gt( INFILE *inpf )
{
    return ( parse_wd(inpf,">") );
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

            if ( !strcmp( nxtwd, ">" ) ) {       /* end of tag reached? */

                inungetcwws( inpf );             /* Y->unget word */

            } else {

                optn = nxtwd;                    /* N->return word */

            }

        }

    }

    return (optn);
}

#if 0
/*
** parse_quote
**
** read first word, check it to be a quote
**
** params: inpf...file to read word from
** result: quote char ('\'' or '\"') or EOF
**
*/
int parse_quote( INFILE *inpf )
{
    int    quote = EOF;
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
#endif

/*
** parse_strarg
*/
STRPTR parse_strarg( INFILE *inpf )
{
    HSCVAR var;
    STRPTR strarg = NULL;

    if ( parse_eq( inpf) )
        strarg =  parse_vararg( &var, inpf );

    return( strarg );
}


#if 0
/*
** parse mutual exclusion emum string
**
** this is eg used with the ALIGN option of <IMG>:
** the mutex-string for ALIGN is "TOP|MIDDLE|BOTTOM" (html1.0)
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
    STRPTR cmpwd = strtok( line, "|" );/* first word from mutex string */

    *match = FALSE;                    /* TRUE, if nxtwd matched cmpwd */

    while ( nxtwd && cmpwd ) {

        if ( igcs ) {
            if ( !upstrcmp( nxtwd, cmpwd ) )
                *match = TRUE;
        } else if ( !strcmp( nxtwd, cmpwd ) )
            *match = TRUE;

        cmpwd = strtok( NULL, "|" );

    }

    return ( nxtwd );

}
#endif
