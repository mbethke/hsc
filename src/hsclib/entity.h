/*
** entity.h
**
** entity structure, variables and functions
**
*/

#ifndef HSCLIB_ENTITY_H
#define HSCLIB_ENTITY_H

/*
** defines
*/

/*
** structures & typdefs for entities
*/
typedef struct hscent {
    STRPTR name;    /* name/id (eg &"uuml"; ) */
    STRPTR replace; /* replace by (eg "ü" ) */
    LONG   numeric; /* numeric code of entity ( eg &#"123"; ) */
} HSCENT;

/* value for hscent.numeric to mark icon entities */
#define ICON_ENTITY (-1)

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSCLIB_ENTITY_H

extern HSCENT *new_hscent( STRPTR newid );
extern VOID    del_entity( APTR data );
extern HSCENT *cpy_hscent( HSCENT *oldent );

extern int     cmp_strent( APTR cmpstr, APTR entdata );
extern int     cmp_nument( APTR cmpstr, APTR entdata );
extern int     cmp_rplcent( APTR cmpstr, APTR entdata );

extern HSCENT *app_entnode( DLLIST *entlist, STRPTR entid );
extern BOOL    add_ent( DLLIST *entlist, STRPTR entid, STRPTR entreplace, LONG num );

#endif /* NOEXTERN_HSCLIB_ENTITY_H */

#endif /* HSCLIB_ENTITY_H */
