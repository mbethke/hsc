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
 * ugly/prginfo.h
 *
 * ugly program info functions, header file
 *
 */

#ifndef UGLY_PRGINFO_H          /* avoid include twice */
#define UGLY_PRGINFO_H

#include <stdio.h>

/*
 * Example @ _set_prginfo()
 *
 * set_prginfo( "test_prg", "Tommy-Saftwörx", 1, 0, 0,
 *              "a short test program", "This is FreeWare!" );
 *
 * (hope that helps)
 *
 *
 * IMPORTANT: Always use _set_prginfo(),
 *            never use _call_set_prginfo()!
 *
 */
/*
#if defined (AMIGA) && ( !defined( VERSTAG ) )
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
{                                                             \
    static char amiga_version[50];                            \
    call_set_prginfo( name, auth, ver, rev, rel,              \
                      __DATE__, __TIME__, info, copy );       \
    strcpy(amiga_version, "$VER:");                           \
    strcat(amiga_version, name);                              \
    strcat(amiga_version, " ");                               \
    strcat(amiga_version, long2str(ver));                     \
    strcat(amiga_version, ".");                               \
    strcat(amiga_version, long2str(rev));                     \
    strcat(amiga_version, ".");                               \
    if (rel)                                                  \
    {                                                         \
        strcat(amiga_version, long2str(rel));                 \
        strcat(amiga_version, ".");                           \
    }                                                         \
}
#else
*/
#if (defined DATE && defined TIME)
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
    call_set_prginfo2( name, auth, ver, rev, rel,              \
    DATE, TIME, info, copy )
#else
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
    call_set_prginfo( name, auth, ver, rev, rel,              \
    __DATE__, __TIME__, info, copy )
#endif

#define UGLY_AUTHOR "Matthias Bethke, T. Aglassinger"

/*
 * extern vars & functions
 */

extern STRPTR pi_progname;

extern void call_set_prginfo(STRPTR name, STRPTR auth,
       int ver, int rel, int rev,
       STRPTR rel_date, STRPTR rel_time,
       STRPTR infostr, STRPTR copystatus);
extern void call_set_prginfo2(STRPTR name, STRPTR auth,
       int ver, int rel, int rev,
       STRPTR rel_date, STRPTR rel_time,
       STRPTR infostr, STRPTR copystatus);
extern int fprintf_prginfo(FILE * stream);

#ifdef AMIGA
/* system version string on amiga systems */
extern STRPTR amiga_version;
#endif

#endif /* UGLY_PRGINFO_H */

