/*
** ugly/prginfo.h
**
** ugly program info functions, header file
**
*/

#ifndef UGLY_PRGINFO_H                     /* avoid include twice */
#define UGLY_PRGINFO_H

#include <stdio.h>

/*
** Example @ _set_prginfo()
**
** set_prginfo( "test_prg", "Tommy-Saftwörx", 1, 0, 0,
**              "a short test program", "This is FreeWare!" );
**
** (hope that helps)
**
**
** IMPORTANT: Always use _set_prginfo(), 
**            never use _call_set_prginfo()!
**
*/
#if defined (AMIGA) && ( !defined( VERSTAG ) )
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
    { call_set_prginfo( name, auth, ver, rev, rel,            \
      __DATE__, __TIME__, info, copy );                       \
      amiga_version = "$VER:" name " " #ver "." #rev "." #rel; }
#else
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
    call_set_prginfo( name, auth, ver, rev, rel,              \
    __DATE__, __TIME__, info, copy )
#endif


/*
** extern vars & functions
*/

extern STRPTR pi_progname;

extern void call_set_prginfo( STRPTR name, STRPTR auth, int ver, int rel, int rev,
    STRPTR rel_date, STRPTR rel_time, STRPTR infostr, STRPTR copystatus );
extern int fprintf_prginfo( FILE *stream );

#ifdef AMIGA
/* system version string on amiga systems */
extern STRPTR amiga_version;
#endif

#endif /* UGLY_PRGINFO_H */

