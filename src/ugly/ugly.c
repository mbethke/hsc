/*  this file links all faetures of ugly together
** (bloody silly, but i don't know how this f..king c-compiler
** should link sub-sub-modules to one sub-module together...)
*/


/*
** TODO:
** - this bullshit won't work if multiple includes are
**   not active. better implement an old_malloc() & co.
*/
#include "memory.c"

#ifdef UMEM_TRACKING_DEF
#define UMEM_TRACKING
#include "memory2.h"
#endif

#include "string.c"
#include "dllist.c"

#if 0
#include "bntree.c"
#include "umx.c"
#endif

#include "fname.c"
#include "prginfo.c"
#include "args.c"
#include "infile.c"

/* end of ugly.c */
