#ifndef UGLY_USTRING_H
#define UGLY_USTRING_H

/*
 * ugly/string.h
 *
 * additional string functions, header
 *
 * (W) by Tommy-Saftwörx in 1993,94,95
 *
 */

#include <stddef.h>

#include "utypes.h"

#define freestr( s ) ugly_freestr(s,__FILE__,__LINE__)
#define ufreestr( s ) freestr(s),s=NULL
#define strclone( s ) ugly_strclone(s,__FILE__,__LINE__)
#define reallocstr( o,n ) ugly_reallocstr(o,n,__FILE__,__LINE__)

/* options for strenum */
#define STEN_CASE   0           /* case sensitive search */
#define STEN_NOCASE 1           /* case insensitive search */

/*
 * external prototypes
 */

#ifndef NOEXTERN_UGLY_USTRING_H

extern STRPTR upstr(STRPTR s);
extern int upstrcmp(CONSTRPTR s1, CONSTRPTR s2);
extern int upstrncmp(CONSTRPTR s1, CONSTRPTR s2, size_t n);
extern void ugly_freestr(STRPTR s, STRPTR file, ULONG line);
extern STRPTR ugly_strclone(CONSTRPTR oldstr, STRPTR file, ULONG line);
extern void ugly_reallocstr(STRPTR * oldstr, CONSTRPTR newstr, STRPTR file, ULONG line);

extern STRPTR ustrrpbrk(CONSTRPTR str, CONSTRPTR set);
extern LONG strenum(STRPTR str, STRPTR set, char sep, BYTE options);

extern STRPTR ch2str(const char ch);
extern BOOL str2long(STRPTR s, LONG * num);
extern STRPTR long2str(LONG num);

#endif /* NOEXTERN_UGLY_USTRING_H */

#endif /* UGLY_USTRING_H */

