/*
** entity.h
**
** entity structure, variables and functions
**
*/

#ifndef HSC_ENTITY_H
#define HSC_ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"

/*
** defines
*/

/*
** structures & typdefs for entities, tags and macros
*/
typedef struct hscent {
    STRPTR name;
    STRPTR source;
} HSCENT;

/*
** defines
*/

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_ENTITY_H

/*
** global vars
*/
extern DLLIST *defent;


/*
** global funcs
*/
extern HSCENT *new_hscent( STRPTR newid );
extern void    del_entity( APTR data );
extern int     cmp_strent( APTR cmpstr, APTR entdata );

extern HSCENT *app_entnode( STRPTR entid );
extern BOOL    add_ent( STRPTR entid, STRPTR entsource );

#endif /* NOEXTERN_HSC_ENTITY_H */

#endif /* HSC_ENTITY_H */
