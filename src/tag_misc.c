/*
** tag_misc
**
** misc tag handles
**
** updated: 30-Jul-1995
** created: 30-Jul-1995
*/

#include <stdio.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/string.h"

#include "global.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "error.h"
#include "msgid.h"

UBYTE prev_heading_num = 0;
/*
**
** global funs
**
*/

/*
** handle_heading
**
**
*/
BOOL check_heading( UBYTE num, INFILE *inpf )
{
    if ( (prev_heading_num-num) < (-1) )
        if (prev_heading_num) {

            char hstr[4];

            message( STYLE_WRONG_HEADING, inpf );
            errstr( "next heading should be " );
            sprintf( hstr, "H%d", prev_heading_num+1 );
            errtag( hstr );
            errlf();

        } else {

            message( STYLE_WRONG_HEADING, inpf );
            errstr( "first heading should be " );
            errtag( "H1" );
            errlf();

        }

    prev_heading_num = num;

    return (TRUE);
}


/*
** handle_headingX
**
**
*/
BOOL handle_heading1( INFILE *inpf )
{
    return ( check_heading(1, inpf) );
}

BOOL handle_heading2( INFILE *inpf )
{
    return ( check_heading(2, inpf) );
}

BOOL handle_heading3( INFILE *inpf )
{
    return ( check_heading(3, inpf) );
}

BOOL handle_heading4( INFILE *inpf )
{
    return ( check_heading(4, inpf) );
}

BOOL handle_heading5( INFILE *inpf )
{
    return ( check_heading(5, inpf) );
}

BOOL handle_heading6( INFILE *inpf )
{
    return ( check_heading(6, inpf) );
}


/*
** handle_comment
**
**
*/
BOOL handle_comment( INFILE *inpf )
{
    /*
    ** TODO: - correct abort
    **       - copy chars
    */
    skip_until_gt( inpf );

    return (TRUE);
}

