/*
** entity.c
**
** entity structure, variables and functions ( "&xx;")
**
** updated: 26-Sep-1995
** created:  8-Sep-1995
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
#include "ugly/memory.h"
#include "ugly/string.h"
#include "ugly/dllist.h"

/*
** local includes
*/
#include "global.h"
#include "error.h"
#include "msgid.h"
#include "tagargs.h"
#include "output.h"

#define NOEXTERN_HSC_ENTITY_H
#include "entity.h"

DLLIST *defent = NULL;

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** new_hscent
**
** alloc & init a new hscentity
*/
HSCENT *new_hscent( STRPTR newid )
{

    HSCENT *newent = (HSCENT*) malloc( sizeof(HSCENT) );

    if (newent) {

        /* init new entity item */
        newent->name    = strclone(newid);       /* set id */
        newent->replace = NULL;
    }

    return (newent);

}

/*
** del_entity
*/
void del_entity( APTR data )
{
    HSCENT *ent = (HSCENT *)data;

    ufreestr( ent->name );
    ufreestr( ent->replace );
    ent->numeric = 0;
    ufree( ent );

}


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
** cmp_rplcent
**
** compares a entity-string with the replace-item
** of a HSCENT-entry
*/
int cmp_rplcent( APTR cmpstr, APTR entdata )
{
    STRPTR entstr = NULL;

    if ( entdata )
        entstr = ((HSCENT*)entdata)->replace;

    if (entstr)
        if ( !strcmp( cmpstr, entstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
**-------------------------------------
** append entity functions
**-------------------------------------
*/

/*
** app_entnode
**
** create a new entity and append it to the defent-list
**
** params: entid..name of the new entity (eg "uuml")
** result: ptr to the new entity or NULL if no mem
*/
HSCENT *app_entnode( STRPTR entid )
{
    HSCENT *newent;

    newent = new_hscent( entid );
    if ( newent ) {

        if (app_dlnode( defent, newent ) == NULL ) {

            del_entity( (APTR) newent );
            newent = NULL;

        }
    } else
        err_mem( NULL );

    return (newent);
}


/*
** add_ent
*/
BOOL add_ent( STRPTR entid, STRPTR entreplace, LONG num )
{
    BOOL    ok = FALSE;
    HSCENT *newent = app_entnode( entid );

    if ( newent ) {

        if ( entreplace ) {

            newent->replace = strclone( entreplace );
            if ( !entreplace )
                err_mem( NULL );
        }
        newent->numeric = num;
        ok = TRUE;

    } else
        err_mem( NULL );

    DDE( fprintf( stderr, "** defent: %s\n", entid ) );

    return (ok);

}

