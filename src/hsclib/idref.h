/*
 * hsclib/idref.h
 *
 * functions for id-references
 *
 */

#ifndef HSCLIB_IDREF_H
#define HSCLIB_IDREF_H

typedef struct idref_node {
    STRPTR name;                /* name of id */
    INFILEPOS *fpos;            /* position where ID has been called from */
} IDREF;

#ifndef NOEXTERN_HSCLIB_IDREF_H

extern VOID del_string_entry(APTR data);
extern STRPTR new_string_entry(STRPTR data);
extern int cmp_string_entry(APTR cmp_data, APTR list_data);


extern VOID hsc_msg_unknown_id(HSCPRC * hp, STRPTR filename, STRPTR id);

extern BOOL add_local_iddef(HSCPRC * hp, STRPTR id);

extern VOID del_idref(APTR data);
extern VOID add_local_idref(HSCPRC * hp, STRPTR id);

extern BOOL check_all_local_idref(HSCPRC * hp);

#endif /* NOEXTERN_HSCLIB_IDREF_H */

#endif /* HSC_IDREF_H */

