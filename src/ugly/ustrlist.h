/*
 *
 * ugly/ustrlist.h
 *
 * header file for string list functions
 *
 * (W) by Tommy-Saftwörx in 1996
 *
 */

#ifndef UGLY_USTRLIST_H
#define UGLY_USTRLIST_H

#include "utypes.h"
#include "ustring.h"
#include "dllist.h"

/*
 * external function prototypes
 */

#define app_strnode(list,str) app_dlnode((list),(APTR)(str))
#define find_strnode(nd,str) find_dlnode((nd),(APTR)(str), cmp_string_node)

#ifndef NOEXTERN_UGLY_USTRLIST_H

extern VOID del_string_node(APTR data);
extern STRPTR new_string_node(STRPTR data);
extern int cmp_string_node(APTR cmp_data, APTR lst_data);

extern VOID clr_strlist(DLLIST *list);
extern VOID del_strlist(DLLIST *list);
extern DLLIST *init_strlist(VOID);

#endif

#endif

