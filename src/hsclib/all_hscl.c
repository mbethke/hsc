/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
/* compile while hsclib as a single file */
#define NOEXTERN_HSCLIB_ATTRIBUTE_H
#define NOEXTERN_HSCLIB_LMESSAGE_H
#define NOEXTERN_HSCLIB_URI_H
#define NOEXTERN_HSCLIB_DEFATTR
#define NOEXTERN_HSCLIB_ENTITY_H
#define NOEXTERN_HSCLIB_EVAL_H
#define NOEXTERN_HSCLIB_HSCPRC
#define NOEXTERN_HSCLIB_ID_H
#define NOEXTERN_HSCLIB_INCLUDE_H
#define NOEXTERN_HSCLIB_PARSE_H
#define NOEXTERN_HSCLIB_SKIP_H
#define NOEXTERN_HSCLIB_TAG_H

#include "hsclib/ldebug.h"
#include "hsclib/msgid.h"

#include "ugly/utypes.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/umemory.h"
#include "ugly/infile.h"
#include "ugly/ustring.h"

#include "hsclib/lmessage.c"
#include "hsclib/entity.c"
#include "hsclib/tag.c"
#include "hsclib/attrib.c"
#include "hsclib/idref.c"
#include "hsclib/hscprc.c"
#include "hsclib/lstatus.c"
#include "hsclib/input.c"
#include "hsclib/skip.c"
#include "hsclib/uri.c"
#include "hsclib/eval.c"
#include "hsclib/posteval.c"
#include "hsclib/defattr.c"
#include "hsclib/deftag.c"

BOOL hsc_include_string( HSCPRC *hp, STRPTR filename, STRPTR s, ULONG optn );

#include "hsclib/parse.c"
#include "hsclib/include.c"
#include "hsclib/linit.c"
#include "hsclib/size.c"
#include "hsclib/tag_misc.c"
#include "hsclib/tag_a.c"
#include "hsclib/tag_hsc.c"
#include "hsclib/tag_if.c"
#include "hsclib/tag_macro.c"

