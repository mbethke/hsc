/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hsclib/skip.h
 *
 * functions for skipping several things
 *
 */

#ifndef HSCLIB_SKIP_H
#define HSCLIB_SKIP_H

#include "ugly/utypes.h"
#include "ugly/infile.h"

#include "hsclib/hscprc.h"

/* options for skip_until_tag(); see "hsclib/skip.c" for explanation */
#define SKUT_NO_SKIP_TAGFOUND    (1<<0)
#define SKUT_NO_CONTENT_TAGFOUND (1<<1)
#define SKUT_CLEAR_CONTENT       (1<<2)
#define SKUT_NO_ANALYSE_TAGS     (1<<3)

/* states for eot_reached() */
#define TGST_TAG     0          /* in tag */
#define TGST_REF     1          /* in attribute reference */
#define TGST_QUOTE   2          /* inside single quote */
#define TGST_DQUOTE  3          /* inside double quote */
#define TGST_END    10          /* end reached */
#define TGST_ERR    99          /* error occured */

/* states for eoc_reached() TODO: remove this */
#define CMST_TEXT     0         /* after normal text (or inside tag) */
#define CMST_STAR     1         /* after "*" */
#define CMST_TAG      2         /* after "<" */
#define CMST_END     10         /* after "*>" */
#define CMST_ERR     99         /* error occured */

#ifndef NOEXTERN_SKIP_H

/*
 * global funcs
 */
extern BOOL skip_next_lf(HSCPRC * hp);
extern BOOL skip_hsc_comment(HSCPRC * hp, EXPSTR * content);
extern BOOL skip_hsc_verbatim(HSCPRC * hp, EXPSTR * content);
extern BOOL skip_sgml_special(HSCPRC * hp, EXPSTR * content);

extern BOOL skip_until_eot(HSCPRC * hp, EXPSTR * logstr);

extern BOOL skip_until_tag(HSCPRC * hp, EXPSTR * content, EXPSTR * tagfound, STRPTR tagstoplist, STRPTR tagnest, ULONG option);

#endif /* NOEXTERN_HSCLIB_SKIP_H */

#endif /* HSCLIB_SKIP_H */

