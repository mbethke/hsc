/*
** tag_misc
**
** misc tag handles
**
** updated: 12-Sep-1995
** created: 30-Jul-1995
*/

/*
** ansi includes
*/
#include <stdio.h>
#include <time.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/args.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "tagargs.h"
#include "parse.h"
#include "error.h"
#include "msgid.h"

#include "tag.h"

UBYTE prev_heading_num = 0; /* stores previous heading */

/*
**
** global funs
**
*/


/*
** handle_heading: tag handle for <H1>..<H6>
**
** compute number of haeding,
** compare it with previous heading,
** check first heading to be <H1>
**
*/
BOOL handle_heading( INFILE *inpf, HSCTAG *tag )
{
    BYTE num = (tag->name[1] - '0'); /* num of heading (1..6) */

    if ( debug )
        fprintf( stderr, "**   heading %d\n", num);

    if ( (prev_heading_num-num) < (-1) )
        if (prev_heading_num) {

            char hstr[4];

            message( MSG_WRONG_HEADING, inpf );
            errstr( "next heading should be " );
            sprintf( hstr, "H%d", prev_heading_num+1 );
            errtag( hstr );
            errlf();

        } else {

            message( MSG_EXPT_H1, inpf );
            errstr( "first heading should be " );
            errtag( "H1" );
            errlf();

        }

    prev_heading_num = num;

    return (TRUE);

}


/*
** handle_sgml_comment: tag handle for <!..>
**
**
*/
BOOL handle_sgml_comment( INFILE *inpf, HSCTAG *TAG )
{
    /*
    ** TODO: - correct abort
    **       - copy chars
    */
    skip_until_gt( inpf );

    return (TRUE);
}

