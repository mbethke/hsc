/*
** cleanup.c
**
** cleanup function for hsc
**
** updated: 26-Sep-1995
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

#include "entity.h"
#include "tag.h"
#include "vars.h"


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
    del_dllist( vars );
    del_dllist( hsctags );

    del_dllist( ignore );

    /* release expstrings */
    del_estr( vararg );
    del_estr( tmpstr );

    /* misc. work */
    ufreestr( outfilename );
    ufreestr( destdir );
    ufreestr( projdir );
    ufreestr( rel_destdir );
    ufreestr( last_anchor );
}





