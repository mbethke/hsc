/*
** tag_hsc.c
**
** tag handles for <UL>, <DL>, <LI>
**
** updated:  8-Sep-1995
** created: 29-Jul-1995
*/

#include <stdio.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"

#include "tag.h"
/*
** counter for nesting of list; it is increased by <UL> and <DL>,
** and decreased by the matching closing tags. so is possible for
** <LI> to determine if it is in a list
*/
LONG list_nest = 0;

/*
**
** global funs
**
*/

/*
** handle_ul <UL>
**
**
*/
BOOL handle_ul( INFILE *inpf )
{
    list_nest++;

    return (TRUE);
}

/*
** handle_dl <DL>
**
**
*/
BOOL handle_dl( INFILE *inpf )
{
    list_nest++;

    return (TRUE);
}

/*
** handle_cul </UL>
**
**
*/
BOOL handle_cul( INFILE *inpf )
{
    list_nest--;

    return (TRUE);
}

/*
** handle_cdl </DL>
**
**
*/
BOOL handle_cdl( INFILE *inpf )
{
    list_nest--;

    return (TRUE);
}

/*
** handle_li <LI>
**
**
*/
BOOL handle_li( INFILE *inpf )
{
    if ( !list_nest ) {

        message( ERROR_UNEX_LI, inpf );
        errstr( "<LI> outside list\n" );

    }


    return (TRUE);
}
