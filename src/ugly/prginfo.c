/*
** ugly/prginfo.c
**
** ugly program info functions
**
** Version 1.0.0, (W) by Tommy-Saftwörx in 1994
**
** updated: 04-Jul-94
** created: 03-Jul-94
**
** $VER: prginfo.c 1.0.0 (4.7.94)
**
**=========================================================
** TODO:
** - set_prginfo()-macro: replace "name" by "( name ? name : argv[0] )"
**
*/


/*
** includes
*/
#include <stdio.h>
#include <string.h>

#include "types.h"

#define SIZE_DATESTR 12
/*
** local global vars
*/
STRPTR pi_progname = NULL;             /* exported */
STRPTR pi_authname = NULL;
int    pi_version  = 0;
int    pi_release  = 0;
int    pi_revision = 0;
char   pi_rel_date[ SIZE_DATESTR ];
STRPTR pi_rel_time = NULL;
STRPTR pi_descript = NULL;
STRPTR pi_copystat = NULL;

STRPTR pi_dt_day   = NULL;
STRPTR pi_dt_month = NULL;
STRPTR pi_dt_year  = NULL;


/*
** call_set_prginfo
**
** set pi_xxx vars; called by macro set_info()
**
*/
void call_set_prginfo( STRPTR name, STRPTR auth, int ver, int rel, int rev,
    STRPTR rel_date, STRPTR rel_time, STRPTR infostr, STRPTR copystatus)
{

    pi_progname = name;
    pi_authname = auth;
    pi_version  = ver;
    pi_release  = rel;
    pi_revision = rev;
    pi_rel_time = rel_time;
    pi_descript = infostr;
    pi_copystat = copystatus;

    strncpy( pi_rel_date, rel_date, SIZE_DATESTR );
    pi_rel_date[3] = '\0';
    pi_rel_date[6] = '\0';
    pi_dt_month = pi_rel_date;
    pi_dt_day = &(pi_rel_date[4]);
    if ( pi_dt_day[0] == ' ' )
        pi_dt_day++;
    pi_dt_year = &(pi_rel_date[7]);

}


/*
** fprintf_prginfo
**
** display program information
**
*/
int fprintf_prginfo( FILE *stream )
{
    int err = 0;

    err  = fprintf( stream, "%s - %s, v%d.%d.%d (%s-%s-%s)\n",
            pi_progname, pi_descript, 
            pi_version, pi_release, pi_revision,
            pi_dt_day, pi_dt_month, pi_dt_year );
    err += fprintf( stream, "(W) by %s. %s\n",
            pi_authname, pi_copystat );

    return err;

}

