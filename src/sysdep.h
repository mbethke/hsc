#include "config.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if STDC_HEADERS || (!defined (isascii) && !HAVE_ISASCII)
#define IN_CTYPE_DOMAIN(c) 1
#else
#define IN_CTYPE_DOMAIN(c) isascii(c)
#endif

#ifdef isblank
#define ISBLANK(c) (IN_CTYPE_DOMAIN (c) && isblank (c))
#else
#define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#endif
