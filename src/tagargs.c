/*
** tagargs.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** functions for parsing of tag arguments
**
** updated: 22-Nov-1995
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
#include "eval.h"


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


/*
** parse_strarg; TODO: remove this
*/
#if 0
STRPTR parse_strarg( INFILE *inpf )
{
    HSCVAR var;
    STRPTR strarg = NULL;

    if ( parse_eq( inpf) )
        strarg =  eval_expression( &var, inpf, NULL );

    return( strarg );
}
#endif

