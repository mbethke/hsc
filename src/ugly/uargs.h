/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * ugly/uargs.h
 *
 * ugly argument handling functions, header file
 *
 */

#ifndef UGLY_UARGS_H
#define UGLY_UARGS_H

/*
 * includes
 */
#include <stdio.h>
#include <stdarg.h>

#include "utypes.h"
#include "umemory.h"

/*
 * argument errors (returned by _set_args())
 */
#define ASE_NO_MEM                0x01  /* out of memory */
#define ASE_INVALID_NUM           0x02  /* invalid number format */
#define ASE_REQUIRED_MISS         0x03  /* required argument missing */
#define ASE_OUT_OF_RANGE          0x04  /* out of range (LONG, ULONG) */
#define ASE_INVALID_ENUM          0x05  /* invalid enum identifier (ENUM) */
#define ASE_EMPTY_TEMPLATE        0x06  /* empty template (arglist==NULL) */
#define ASE_UNKNOWN_KEYWORD       0x07  /* unknown keyword */
#define ASE_OCCURED_TWICE         0x08  /* arg w/o /O or /M occured twice */
#define ASE_NO_VAL_AFTER_KW       0x09  /* value after keyword missing */

#define ASE_HANDLE_FUNC           0x7f  /* error reported by handle func */

/*
 * argument errors (returned by _prep_args())
 */
#define APE_NO_MEM                0x01  /* out of memory */
#define APE_INVALID_TEMPLATE      0x02  /* invalid template */
#define APE_DOUBLE_TEMPLATE       0x03  /* doubel definition of template */
#define APE_EMPTY_TEMPLATE        0x04  /* empty template */
#define APE_ILLEGAL_TYPE          0x05  /* unknown type */
#define APE_ILLEGAL_FLAG          0x06  /* unknown flag */
#define APE_DESTVAR_IS_NULL       0x07  /* destination var is NULL */
#define APE_DOUBLE_MULTIPLE       0x08  /* /M used twice */
#define APE_CONFLICTING_M_O       0x09  /* /M and /O both occured */

/*
 * argument types
 */
#define ARG_SWITCH     0x01     /* S */
#define ARG_TEXT       0x02     /* T */
#define ARG_LONG       0x03     /* L */
#define ARG_LONG_RANGE 0x04     /* R */
#define ARG_ENUM       0x05     /* E */
#define ARG_ULONG      0x06     /* U */

/*
 * argument flags
 */
#define ARG_KEYWORD         ( 1 << 0 )  /* K */
#define ARG_REQUIRED        ( 1 << 1 )  /* R */
#define ARG_MULTIPLE        ( 1 << 2 )  /* M */
#define ARG_CASESENS        ( 1 << 3 )  /* C - case sensitive */
#define ARG_OVERWRITE       ( 1 << 4 )  /* O - overwrite old value when */
                                       /*     occurend more then once */
#define ARG_HANDLEFUNC      ( 1 << 5 )  /* $ */

/* values for arginfo.ai_set */
#define AIS_UNSET        0 /* not set at all */
#define AIS_SET_PREVIOUS 1 /* set by previous set_args() */
#define AIS_SET_LOCAL    2 /* set by current set_args() */

/*
 * struct arginfo (PRIVATE,DON NOT USE)
 */
typedef struct arginfo
{
    STRPTR ai_id;               /* arg id string */
    LONG ai_type;               /* arg type */
    LONG ai_flags;              /* arg flags */
    union {
        STRPTR ai_enum;         /*   enumerator string */
        LONG ai_lolim;          /*   lower limit for range */
    } ai_misc1;                 /* type depending information */
    union {
        LONG ai_uplim;          /*   upper limit for range */
    } ai_misc2;                 /* type depending information */
    APTR ai_dest;               /* ptr to destination var */
      STRPTR(*ai_func) (STRPTR);        /* additional arg handling function */
    STRPTR ai_help;             /* help text */
    BOOL ai_set;                /* handled by _set_args() */
} ARGINFO;

typedef struct argfile
{
    int argc;
    char **argv;
} ARGFILE;

/*
 * commments about ARGINFO
 *
 * handle function:
 *   the handle function is called by set_arg_value() AFTER the
 *   destination var is updated. the old value is passed as an
 *   APTR to the handle. the handle should return a ptr to a
 *   string that contains an error message or NULL if the new
 *   arg is ok.
 *
 *   example for a handle function for numeric args
 *   STRPTR handle_func( STRPTR newval )
 *   {
 *       STRPTR errmsg = NULL;
 *
 *       printf( "handle_func: set val to %d", newval );
 *       if ( newval > 3 )
 *          errmsg = "val to high!\n";
 *       else if ( newval < 1 )
 *          errmsg = "val to low!\n";
 *
 *       return ( errmsg );
 *   }
 */

/*
 * struct arglist (PRIVATE)
 */
typedef struct arglist
{
    STRPTR al_name;             /* name (only for debugging) */
    struct dllist *al_list;     /* argument template */
    struct arginfo *al_multiple;        /* entry with /M flag set */
    struct arginfo *al_nokeywd; /* entry w/o /K flag; TODO: remove */
} ARGLIST;

/* debuggin define */
#if DEBUG_UGLY_ARG
#define DA(x) x
#define DUA "*args  * "
#else
#define DA(x)                   /* nufin */
#define DUA                     /* nufin */
#endif

/*
 * extern functions & global vars
 */

#ifndef NOEXTERN_UGLY_UARGS_H

extern LONG prep_error_num;
extern int prep_error_idx;
extern size_t prep_error_pos;

extern ARGLIST *prepare_args(STRPTR arglist_name,...);
extern BOOL set_args(int argc, char *argv[], ARGLIST *al);
extern BOOL set_args_argv(int argc, char *argv[], ARGLIST * al);
extern BOOL set_args_file(ARGFILE *argf, ARGLIST *argl);
extern VOID free_args(ARGLIST *al);
extern BOOL check_args(ARGLIST *al);

extern ARGFILE *new_argfile(char *argfname);
extern ARGFILE *new_argfilev(STRPTR fname[]);
extern VOID del_argfile(ARGFILE *argf);

/* display help */
extern int fprintf_arghelp(FILE * stream, ARGLIST *al);
extern int fprintf_arghelp_short(FILE * stream, ARGLIST *al);

/* error handling */
extern STRPTR strargerr(VOID);
extern void pargerr(VOID);

#endif /* NOEXTERN_UGLY_UARGS_H */

#endif /* UGLY_UARGS_H */

