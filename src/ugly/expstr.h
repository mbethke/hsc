/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
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
#ifndef UGLY_EXPSTR_H
#define UGLY_EXPSTR_H

/*
 * ugly/expstr.h
 *
 * self-expanding string functions, header
 *
 * (W) by Tommy-Saftwörx in 1995
 *
 */

#include <stddef.h>

#include "utypes.h"

#define EXPSTR_MEMSTEP 96
#define ES_MIN_MEMSTEP 8        /* min. memory step for init( step_size ) */

#ifndef modadj
#define modadj(x,by) ((by)*(((x)+(by))/(by)))
#endif

/* some inline macros used without DEBUG defined */
#define ugly_inline_estr2str(es) ((es)->es_data)
#define ugly_inline_estrlen(es)  ((es)->es_len - 1)

typedef struct
{
    STRPTR es_data;             /* ptr to string data */
    size_t es_len;              /* current len */
    size_t es_size;             /* current size of mem allocated */
    size_t es_step;             /* size of memory step */
}
EXPSTR;

/*
 * external prototypes
 */

#ifndef NOEXTERN_UGLY_EXPSTR_H

extern EXPSTR *ugly_dbg_init_estr(size_t step_size, STRPTR file, ULONG line);
extern EXPSTR *ugly_init_estr(size_t step_size);
extern void del_estr(EXPSTR * es);

extern BOOL ugly_clr_estr(EXPSTR * es);
extern BOOL ugly_dbg_clr_estr(EXPSTR * es, STRPTR file, ULONG line);
extern BOOL set_estrn(EXPSTR * es, CONSTRPTR s, size_t n);

extern BOOL ugly_set_estr(EXPSTR * es, CONSTRPTR s);
extern BOOL ugly_app_estrch(EXPSTR * es, int ch);
extern BOOL ugly_app_estr(EXPSTR * es, CONSTRPTR s);

extern BOOL ugly_dbg_set_estr(EXPSTR * es, CONSTRPTR s, STRPTR file, ULONG line);
extern BOOL ugly_dbg_app_estrch(EXPSTR * es, int ch, STRPTR file, ULONG line);
extern BOOL ugly_dbg_app_estr(EXPSTR * es, CONSTRPTR s, STRPTR file, ULONG line);
extern STRPTR ugly_estr2str(EXPSTR * es);
extern size_t ugly_estrlen(EXPSTR * es);

extern BOOL estrcpy(EXPSTR * dest, EXPSTR * src);
extern BOOL estrcat(EXPSTR * dest, EXPSTR * src);

extern BOOL get_mid_estr(EXPSTR * dest, EXPSTR * src, size_t from, size_t num);
extern BOOL get_right_estr(EXPSTR * dest, EXPSTR * src, size_t num);
extern BOOL get_left_estr(EXPSTR * dest, EXPSTR * src, size_t num);

#endif /* NOEXTERN_UGLY_EXPSTR_H */

/*
 * debugging prototypes
 */
#if DEBUG_UGLY_EXPSTR

/* full debugging */
#define set_estr_mem( es, size ) ugly_dbg_set_estr_mem( es, size, __FILE__, __LINE__ )
#define set_estr( es, s ) ugly_dbg_set_estr( es, s, __FILE__, __LINE__ )
#define app_estrch( es, ch ) ugly_dbg_app_estrch( es, ch, __FILE__, __LINE__ )
#define app_estr( es, s ) ugly_dbg_app_estr( es, s, __FILE__, __LINE__ )
#define init_estr( s ) ugly_dbg_init_estr( s, __FILE__, __LINE__ )
#define clr_estr( s ) ugly_dbg_clr_estr( s, __FILE__, __LINE__ )
#define estr2str( s ) ugly_estr2str( s )
#define estrlen( s ) ugly_estrlen( s )

#else

/* no debugging */
#define set_estr_mem( es, size ) ugly_set_estr_mem( es, size )
#define set_estr( es, s ) ugly_set_estr( es, s )
#define app_estrch( es, ch ) ugly_app_estrch( es, ch )
#define app_estr( es, s ) ugly_app_estr( es, s )
#define init_estr( s ) ugly_init_estr( s )
#define clr_estr( s ) ugly_clr_estr( s )
#define estr2str( s ) ugly_inline_estr2str( s )
#define estrlen( s ) ugly_inline_estrlen( s )

#endif

#endif /* UGLY_EXPSTR_H */

