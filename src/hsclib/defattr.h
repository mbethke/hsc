/*
** defattr.h
**
** functions to define new attribute
** and manipulate attribute lists
**
*/

#ifndef HSCLIB_DEFATTR_H
#define HSCLIB_DEFATTR_H

/*
** global funcs
*/
#ifndef NOEXTERN_HSCLIB_DEFATTR

/* attribute list manipulation */
extern BOOL copy_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci );
extern BOOL set_local_varlist( DLLIST *destlist, DLLIST *varlist, ULONG mci );
extern VOID remove_local_varlist( DLLIST *varlist, ULONG mci );
extern BOOL check_varlist( HSCPRC *hp, DLLIST *varlist );

extern HSCVAR *define_var( HSCPRC *hp, DLLIST *varlist, ULONG unmasked_flags );

#endif /* NOEXTERN_HSCLIB_DEFATTR */
#endif /* HSCLIB_DEFATTR_H */

