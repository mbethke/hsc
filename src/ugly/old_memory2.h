/*
** ugly/memory2.h
**
** additional memory manegment functions, prototype header file
**
** updated:  1-Aug-1995
** created:  1-Aug-1995                                      compt: ANSI
**
*/

#include "memory.h"

#ifdef UMEM_TRACKING
#define malloc(size) ugly_malloc( size, __FILE__, __LINE__ )
#define free(ptr)    ugly_free( ptr, __FILE__, __LINE__ )
#define umem_report(msg)  uglymem_report( msg, __FILE__, __LINE__, __DATE__, __TIME__ )
#endif



