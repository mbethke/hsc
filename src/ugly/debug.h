#ifndef UGLY_DEBUG_H
#define UGLY_DEBUG_H
/*
** ugly/debug.h
**
** ugly debugging defines.
**
** (C) by Tommy-Saftwörx 1996
**
*/

#ifdef DEBUG_UGLY

#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 1
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 1

#else

#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 0
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 0

#endif /* DEBUG_UGLY */

#endif  /* UGLY_DEBUG_H */

