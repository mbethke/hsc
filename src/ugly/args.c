/*
** ugly/args.c
**
** ugly argument handling functions
**
** Version 1.2.2, (W) by Tommy-Saftwörx in 1994,95
**
** updated:  8-Aug-1995
** created:  3-Jul-1994
**
** $VER: args.c 1.2.2 (30.7.95)
**
**===================================================================
** TODO:
** - improve error handling ( _prepare_args() )
** - support /M
**
*/

/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "memory.h"
#include "string.h"
#include "dllist.h"

#define NOEXTERN_UGLY_ARGS_H
#include "args.h"

/*
**
** include sub-modules
**
*/
#include "args_fre.c"
#include "args_set.c"
#include "args_prp.c"
#include "args_hlp.c"


