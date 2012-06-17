/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2003 Matthias Bethke
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
 * hsclib/tcpip.h
 *
 * Functions for TCP/IP-based protocols support
 *
 * created:  23-Dec-2003
 */

#include "ugly/utypes.h"
#include "ugly/umemory.h"
#include "ugly/expstr.h"
#include "hsclib/hscprc.h"

extern BOOL check_ext_uri(HSCPRC *hp, const char *uri);
void init_tcpip(void);
void cleanup_tcpip(void);

