#ifndef UGLY_DEBUG_H
#define UGLY_DEBUG_H
/*
 * ugly/debug.h
 *
 * ugly debugging defines.
 *
 * (C) by Tommy-Saftwörx 1996
 *
 */

#ifdef DEBUG_UGLY

/* NOTE: for some defines, a value of "2" enables
 * extended debuggin information:
 *
 * DEBUG_UGLY_EXPSTR=2 display every call to set_estr()
 * DEBUG_UGLY_MEMORY=2 display every call to umalloc/ufree
 */
#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 1
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 1
#define DEBUG_UGLY_TIME   1

#else

#define DEBUG_UGLY_ARG    0
#define DEBUG_UGLY_EXPSTR 0
#define DEBUG_UGLY_INFILE 0
#define DEBUG_UGLY_MEMORY 0
#define DEBUG_UGLY_TIME   0

#endif /* DEBUG_UGLY */

#define panic(msg)                                              \
    fprintf(stderr, "\n##\n## PANIC at \"%s\" (%u): %s\n##\n", \
            __FILE__, __LINE__, (msg))                          \

#endif /* UGLY_DEBUG_H */

