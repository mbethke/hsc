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
BOOL hsc_init_throwback(HSCPRC * hp);
void hsc_del_throwback(HSCPRC * hp);
void hsc_throwback(HSCPRC * hp,
                   HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                   STRPTR fname, ULONG x, ULONG y,
                   STRPTR msg_text);

/* $Id$ */
/* vi: set ts=4: */

