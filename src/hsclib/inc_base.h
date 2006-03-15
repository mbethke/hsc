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
/*
 * hsclib/inc_base.h
 *
 * include basic structures and prototypes that are
 * required by most other modules
 *
 */

#ifndef HSCLIB_INC_BASE_H
#define HSCLIB_INC_BASE_H

#include "sysdep.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

#include "hsclib/ldebug.h"

#include "ugly/utypes.h"
#include "ugly/dllist.h"
#include "ugly/hsctrees.h"
#include "ugly/expstr.h"
#include "ugly/umemory.h"
#include "ugly/infile.h"
#include "ugly/ustring.h"
#include "ugly/ufile.h"

#include "hsclib/msgid.h"

#include "hscprj/document.h"

#include "hsclib/attrib.h"
#include "hsclib/entity.h"
#include "hsclib/tag.h"
#include "hsclib/hscprc.h"
#include "hsclib/lmessage.h"
#include "hsclib/lstatus.h"

#define NMEMBERS(s) (sizeof(s)/sizeof(s[0]))

#endif /* HSCLIB_INC_BASE_H */

/* $Id$ */
/* vi: set ts=4: */

