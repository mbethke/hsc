/*
** ugly/umx.c
**
** ugly macro expansion function
**
** Version 1.1.2, (W) by Tommy-Saftwörx
**
** updated: 22-Jun-94
** created: 30-Mar-94                                      compt: ANSI
**
*/

/*
**
** BUG : UMX_FLOAT doesn't work (wrong result)
**
*/


#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "string.h"

#define NOEXTERN_UGLY_UMX_H
#include "umx.h"

/*
** local global vars
*/

STRPTR macrobuf;                       /* buffer to converted macro */
size_t macromax;                       /* size of macrobuffer */
size_t macroidx;                       /* idx if char currently processing */

LONG   adj;                            /* adjust value */



/*
**
** appchar
**
*/
void appchar( char ch )
{
    if ( macroidx < UMX_MACRO_MAX ) {  /* macro buffer overflow? */
        macrobuf[macroidx] = ch;       /* N-> append new char */
        macroidx++;                    /*     process next char */
    }
}


/*
**
** appstr                              <-- speedup!
**
*/
void appstr( STRPTR str )
{
    while ( str[0] ) {                 /* end of string readched? */
        appchar( str[0] );             /* N-> append current char */
        str++;                         /*     process next char */
    }
}


#if 0
void appfloat( struct umacro mac )
{
    char numstr[40];

    /* printf( "\"%s\"-> %f\nsc ", mac.umx_fmt, *(mac.umx_exp) ); */

    if ( sprintf( numstr, mac.umx_fmt, *((double *) (mac.umx_exp) )) )
        appstr( numstr );
    else {
        appchar( '%' );
        appchar( mac.umx_id );
        appstr( "<ferr>" );
    }
}
#endif

/*
**
** appstr_adj                          <-- speedup!
**
** NOTE:  I had problems declacing _strl as size_t (which is
**        the type a string length it should be in a ansi-c 
**        source). _adj is declared as LONG, but to calculate
**        "(adj+strl)", _adj is also converted to size_t, which
**        of course supressed negativ values for _adj (as right
**        indent). Therefor, _strl is also declared as long.
**
*/
void appstr_adj( STRPTR str )
{
    LONG strl = strlen( str );         /* length of string */

    while ( (adj+strl) < 0 ) {         /* append blanks, right */
        appchar( ' ' );
        adj++;
    }

    appstr( str );                     /* append string */

    while ( (adj-strl) > 0 ) {         /* append blanks, left */
        appchar( ' ' );
        adj--;
    }
}


/*
**
** appnum_adj
**
*/
void appnum_adj( LONG num )
{
    char numstr[10];

    if ( sprintf( numstr, "%d", num ) )
        appstr_adj( numstr );
}


/*
**
** apptime
**
*/
void apptime( struct umacro mac )
{
    char timebuf[ UMX_TIMEBUF_MAX ];

    if ( strftime ( timebuf, UMX_TIMEBUF_MAX, mac.umx_fmt, 
                    (struct tm *) mac.umx_exp ) 
       )
        appstr_adj( timebuf );
    else {
        appchar( '%' );
        appchar( mac.umx_id );
    }
}

/*
** setumxexp
**
** set new exp
**
*/
BOOL setumx_exp( struct umacro mac[], char id, UPTR exp )
{
    int  i      = 0;                   /* loop var */
    BOOL result = FALSE;               /* TRUE, if exp set */

    while ( (mac[i].umx_type != UMX_END ) && (mac[i].umx_id != id ) )
        i++;                           /* search for index of id in mac[] */

    if ( mac[i].umx_id == id ) {       /* found? */
        mac[i].umx_exp = exp;          /* Y-> set exp */
        result = TRUE;                 /*     set result */
    }

    return result;                     /* return result */
}


/*
** setumx_fmt
**
** set new fmt
**
*/
BOOL setumx_fmt( struct umacro mac[], char id, STRPTR fmt )
{
    int  i      = 0;                   /* loop var */
    BOOL result = FALSE;               /* TRUE, if fmt set */

    while ( (mac[i].umx_type != UMX_END ) && (mac[i].umx_id != id ) )
        i++;                           /* search for index of id in mac[] */

    if ( mac[i].umx_id == id ) {       /* found? */
        mac[i].umx_fmt = fmt;          /* Y-> set fmt */
        result = TRUE;                 /*     set result */
    }

    return result;                     /* return result */
}


/*
**
** strfumx
**
** convert umx to string with given format specifier
**
** params: s......... destination string
**         maxsize... max. length of destination string
**         fmt....... format specifier
**         mac....... macro declaration
** result: number of chars converted (= new length of _s)
** errors: return 0
**
*/
size_t strfumx( STRPTR s, size_t maxsize, STRPTR fmt, struct umacro mac[] )
{
    size_t i;                          /* loop var */
    BYTE   adj_sgn;                    /* signum of adjust value */

    macrobuf = s;                      /* set global local vars */
    macromax = maxsize;
    macroidx = 0;

    while ( fmt[0] ) {                 /* start loop until end of _fmt */

        if ( fmt[0] == '%' ) {         /* macro coming? */

            
            fmt++;                     /* Y->process next ch */

            /* scan signum of adjust ('-') */
            if ( fmt[0] == '-' ) {
                adj_sgn = -1;
                fmt++;
            } else
                adj_sgn = +1;

            /*
            ** scan adjust value
            */
            adj = 0;
            while( (fmt[0] >= '0') && (fmt[0] <= '9') ) {
                adj = 10*adj + (fmt[0] - '0');
                fmt++;
            }
            adj *= adj_sgn;


            /*
            ** find matching macro id
            */
            i = 0;
            while ( ( mac[i].umx_type != UMX_END ) && 
                    ( mac[i].umx_id != fmt[0] ) ) i++;

            /*
            ** expand macro
            */
            switch ( mac[i].umx_type ) {         /* check macro type... */
                case UMX_INT:                    /*   integer */
                    appnum_adj( (LONG) ((int) mac[i].umx_exp ));
                    break;
                case UMX_LONG:                   /*   long integer */
                    appnum_adj( (LONG) mac[i].umx_exp );
                    break;
                case UMX_CHAR:                   /*   character */
                    appstr_adj( ch2str( ((STRPTR) (mac[i].umx_exp))[0] ) );
                    break;
                case UMX_STR:                    /*   string */
                    appstr_adj( (STRPTR) mac[i].umx_exp );
                    break;
                /*
                ** skip UMX_FLOAT
                */
#if 0
                case UMX_FLOAT:                  /*   float */
                    appfloat( mac[i] );
                    break;
#endif
                case UMX_TM:                     /*   time */
                    apptime( mac[i] );
                    break;
                default:
                    if ( fmt[0] != '%' )         /* unknown or "%%" */
                        appchar( '%' );
                    appchar( fmt[0] );
                    break;
            }
        } else
            appchar( fmt[0] );         /* N-> append normal character */
    
        fmt++;
    }

    macrobuf[macroidx] = '\0';         /* mark end of macro */

    return macroidx;                   /* return number of chars converted */
}

