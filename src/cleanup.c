/*
** cleanup.c
**
** cleanup function for hsc
**
** updated:  5-Aug-1995
** created:  1-Jul-1995
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "ugly/types.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"

/*
** del_entity
*/
void del_entity( APTR data )
{
    HSCENT *ent = (HSCENT *)data;

    ufreestr( ent->name );
    ufreestr( ent->source );
    ufree( ent );

}

/*
** del_tag
*/
void del_tag( APTR data )
{
    HSCTAG *tag = (HSCTAG *)data;

    ufreestr( tag->name );
    ufree( tag );

}

/*
** del_mac
*/
void del_mac( APTR data )
{
    HSCMAC *mac = (HSCMAC *)data;

    ufreestr( mac->name );
    ufreestr( mac->text );
    ufree( mac );

}

/*
** cleanup:
**
** free all memory, close all files, etc.
*/
void cleanup( void )
{
    /* close file */
    if ( outfile && (outfile!=stdout) ) fclose( outfile );
    close_error();

    /* release lists */
    del_dllist( defent );
    del_dllist( deftag );
    del_dllist( cltags );
    del_dllist( macros );

    /* misc. work */
    ufreestr( outfilename );
    ufreestr( rel_destdir );
    ufreestr( last_anchor );
}





