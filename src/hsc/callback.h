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
 * hsc/callback.h
 *
 * callbacks of hsc for hsclib
 *
 */

#ifndef HSC_CALLBACK_H
#define HSC_CALLBACK_H

/*
 * global funcs
 */
#ifndef NOEXTERN_HSC_CALLBACK_H

extern BOOL hsc_nomem_handler(size_t size);

extern BOOL init_callback(HSCPRC * hp);

extern BOOL init_msgfile(HSCPRC * hp, STRPTR fname);
extern VOID cleanup_msgfile(VOID);

#endif /* NOEXTERN_HSC_CALLBACK_H */
#endif /* HSC_CALLBACK_H */

