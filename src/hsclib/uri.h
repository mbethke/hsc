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
 * uri.h
 *
 * functions for parsing URIs
 *
 */

#ifndef HSCLIB_URI_H
#define HSCLIB_URI_H

#define ABSURI_ID ":"

typedef enum
{
    URI_abs,                    /* starts with ":" */
    URI_ext,                    /* contains ":" before first "/" */
    URI_rel,                    /* no ":" */
    URI_relserv                 /* starts with "/"; server relative */
}
URIKIND;                        /* uri kinds */

/*
 * global funcs
 */
#ifndef NOEXTERN_HSCLIB_URI_H

extern VOID conv_path2uri(EXPSTR * dest, STRPTR path);
extern VOID conv_uri2path(EXPSTR * dest, STRPTR uri, BOOL weenix);

extern VOID conv_hscuri2file(HSCPRC * hp, EXPSTR * dest_fname, STRPTR uri);
extern VOID conv_hscuri2fileNuri(HSCPRC * hp, EXPSTR * dest_uri, EXPSTR * dest_fname, STRPTR uri);

extern VOID parse_uri(HSCPRC * hp, EXPSTR * dest_uri, STRPTR uri);
extern URIKIND uri_kind(STRPTR uri);

#endif /* NOEXTERN_HSCLIB_URI_H */

#endif /* HSCLIB_URI_H */

