/*
** find.c
**
** misc. find functions
**
** updated:  1-Sep-1995
** created:  1-Sep-1995
**
*/

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/prginfo.h"
#include "ugly/infile.h"
#include "ugly/dllist.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"
#include "status.h"

/* TODO: remove this */
#if 0
#define NOEXTERN_FIND_H
#include "find.h"
#endif

/*
**---------------------------
** find entity string
**---------------------------
*/

/*
** cmp_strent
**
** compares a entity-string with the name
** of a HSCENT-entry
*/
int cmp_strent( APTR cmpstr, APTR entdata )
{
    STRPTR entstr = NULL;

    if ( entdata )
        entstr = ((HSCENT*)entdata)->name;

    if (entstr)
        if ( !strcmp( cmpstr, entstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}


/*
**---------------------------
** find tag string
**---------------------------
*/

/*
** cmp_strtag
**
** compares a tag-string with the name
** of a HSCTAG-entry
*/
int cmp_strtag( APTR cmpstr, APTR tagdata )
{
    STRPTR tagstr = NULL;

    if ( tagdata )
        tagstr = ((HSCTAG*)tagdata)->name;

    if (tagstr)
        if ( !upstrcmp( cmpstr, tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
** find_strtag
*/
HSCTAG *find_strtag( STRPTR name )
{
    DLNODE *nd = find_dlnode( deftag->first, (APTR) name, cmp_strtag );
    HSCTAG *tag = NULL;

    if ( nd )
        tag = (HSCTAG*)nd->data;

    return ( tag );
}


/*
**---------------------------
** find closing tag string
**---------------------------
*/

/*
** cmp_strctg
**
** compares a tag-string with the name
** of closing tag (which is a STRPTR, but
** has been converted to APTR)
*/
int cmp_strctg( APTR cmpstr, APTR tagstr )
{
    if (tagstr)
        if ( !upstrcmp( (STRPTR) cmpstr, (STRPTR) tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}


