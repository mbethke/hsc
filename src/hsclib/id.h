/*
** hsclib/id.h
**
** id related functions
**
*/

#ifndef HSCLIB_ID_H
#define HSCLIB_ID_H

typedef struct idlocal_node
{
    STRPTR     name;  /* name of id */
    INFILEPOS *fpos;  /* position where ID has been defined */
} IDLOCAL;

typedef struct idref_node
{
    STRPTR     name;  /* name of id */
    INFILEPOS *fpos;  /* position where ID has been called from */
} IDREF;

#ifndef NOEXTERN_HSCLIB_ID_H

extern VOID del_id_local( APTR data );
extern VOID del_id_ref( APTR data );
extern VOID del_subdoc( APTR data );

extern VOID hsc_msg_unknown_id( HSCPRC *hp, STRPTR filename, STRPTR id );

extern BOOL check_all_local_id_ref( HSCPRC *hp );
extern VOID check_id_local( HSCPRC *hp, STRPTR id );
extern BOOL hsc_add_id_ref( HSCPRC *hp, STRPTR id );

#endif /* NOEXTERN_HSCLIB_ID_H */

#endif /* HSC_ID_H */

