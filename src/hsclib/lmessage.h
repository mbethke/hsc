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
 * hsclib/message.h
 *
 * message funcs for hsc
 *
 */

#ifndef HSCLIB_MESSAGE_H
#define HSCLIB_MESSAGE_H
#include "hsclib/hscprc.h"

#ifndef NOEXTERN_HSCLIB_MESSAGE_H

extern VOID hsc_message(HSCPRC * hp, HSCMSG_ID msg_id,
                        const char *format,...);

extern VOID hsc_msg_eof(HSCPRC * hp, STRPTR descr);
extern VOID hsc_msg_illg_whtspc(HSCPRC * hp);
extern VOID hsc_msg_stripped_tag(HSCPRC * hp, HSCTAG * tag, STRPTR why);
extern VOID hsc_msg_unkn_attr_ref(HSCPRC * hp, STRPTR attr);
extern VOID hsc_msg_unkn_attr_tag(HSCPRC * hp, STRPTR attr, STRPTR tag);
extern VOID hsc_msg_unkn_attr_macro(HSCPRC * hp, STRPTR attr, STRPTR macro);
extern VOID hsc_msg_read_error(HSCPRC * hp, STRPTR filename);

#if 1 /* TODO: get rid of this */
extern VOID hsc_msg_eol(HSCPRC * hp);
#endif

extern VOID hsc_msg_noinput(HSCPRC * hp, STRPTR filename);
extern VOID hsc_msg_nouri(HSCPRC * hp, STRPTR filename, STRPTR uriname, STRPTR note);

#endif /* NOEXTERN_HSCLIB_MESSAGE */
#endif /* HSCLIB_MESSAGE_H */

/* $Id$ */
/* vi: set ts=4: */

