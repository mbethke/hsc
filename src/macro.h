/*
** macro.h
**
** hsc-macro funcs for hsc
**
** updated:  4-Sep-1995
** created:  3-Sep-1995
*/

#ifndef HSC_MACRO_H
#define HSC_MACRO_H

#include "ugly/types.h"

#include "global.h"


typedef struct hscmac {
    STRPTR name;                       /* macro id */
    STRPTR op_fname;                   /* name of temp file that holds text */
    STRPTR cl_fname;                   /*     for opening/closing macro */
    DLLIST *op_args;                   /* list of arguments */
    DLLIST *cl_args;
} HSCMAC;

/*
** global funcs
*/
#ifndef NOEXTERN_HSC_MACRO

extern DLLIST *macros;
extern DLLIST *macarg;

extern HSCMAC *new_hscmac( STRPTR newname );
extern HSCMAC *app_macro( STRPTR newname );
extern void del_mac( APTR data );

extern int cmp_strmac( APTR cmpstr, APTR macdata );
extern HSCMAC *find_strmac( STRPTR name );

extern HSCMAC *def_macro_name( INFILE *inpf, BOOL *open_mac );
extern BOOL    def_macro_args( HSCMAC *macro, INFILE *inpf, BOOL *open_mac );

extern BOOL set_macro_args( HSCMAC *macro, INFILE *inpf, BOOL open_mac );

#endif /* NOEXTERN_HSC_MACRO */
#endif /* HSC_MACRO_H */
