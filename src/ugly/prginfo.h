/*
** ugly/prginfo.h
**
** ugly program info functions, header file
**
** updated: 03-Jul-94
** created: 03-Jul-94
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
#define  set_prginfo( name, auth, ver, rev, rel, info, copy ) \
    call_set_prginfo( name, auth, ver, rev, rel,              \
    __DATE__, __TIME__, info, copy )


/*
** extern vars & functions
*/

extern STRPTR pi_progname;

extern void call_set_prginfo( STRPTR name, STRPTR auth, int ver, int rel, int rev,
    STRPTR rel_date, STRPTR rel_time, STRPTR infostr, STRPTR copystatus );
extern int fprintf_prginfo( FILE *stream );


#endif /* UGLY_PRGINFO_H */

