/*
** cleanup.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** cleanup function for hsc
**
** updated: 27-Nov-1995
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
#include "output.h"

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
    close_output();
    close_error();

    /* release lists */
    del_dllist( defent );
    del_dllist( deftag );
    del_dllist( cltags );
    del_dllist( vars );

    del_dllist( incfile );

    /* release expstrings */
    del_estr( IF_stack );
    del_estr( tmpstr );
    del_estr( tag_name_str );
    del_estr( tag_attr_str );
    del_estr( tag_close_str );
    del_estr( vararg );

    /* misc. work */
    ufreestr( outfilename );
    ufreestr( destdir );
    ufreestr( projdir );
    ufreestr( rel_destdir );
    ufreestr( last_anchor );
}





