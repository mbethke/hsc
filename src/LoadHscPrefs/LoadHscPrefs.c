/*
** LoadHscPrefs/LoadHSCPrefs.c
**
** Copyright (C) 1996  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**-------------------------------------------------------------------
**
** Author : Thomas Aglassinger (Tommy Saftwörx)
** Email  : agi@giga.or.at
** Address: Lissagasse 12/II/9
**          8020 Graz
**          AUSTRIA
**
**-------------------------------------------------------------------
**
** updated: 13-Apr-1996
** created: 19-Feb-1996
*/

/* disable source-level memory-tracking */
#ifdef UMEM_TRACKING
#undef UMEM_TRACKING
#endif

/*
** ansi includes
*/
#include <stdlib.h>
#include <stdio.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/string.h"
#include "ugly/args.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"


/*
** local includes
*/
#include "hsclib/hsclib.h"

/*
** check if LoadHscPrefs would work under this system
*/
#ifndef HSCBASE_FILE
#error "LoadHscPrefs won't work under this system"
#endif

#include "LoadHscPrefs/LoadHscPrefs_rev.h"


#ifdef AMIGA
/* AmigaOS version string
** (imported from "LoadHscPrefs_rev.h")
*/
static STRPTR AmigaOS_version = VERSTAG;
#endif

#define RC_OK     0
#define RC_WARN   5
#define RC_ERROR 10
#define RC_FAIL  20

/* debugging defines */
#define DLP "*lpre* "
#ifdef DEBUG
#define DDL(x) if ( arg_debug ) x
#else
#define DDL(x) /* do nufin */
#endif

static HSCPRC *hp          = NULL;
static int     return_code = RC_FAIL;

static STRPTR fname   = NULL;  /* filename for prefs */
static BOOL   debug   = FALSE; /* enable debugging output */
static BOOL   force   = FALSE; /* force re-reading */
static BOOL   quiet   = FALSE; /* oppress status messages */
static BOOL   unload  = FALSE; /* unload current prefs */

static DLLIST *ddeftag = NULL;

/*
** show_license
**
** display short description of GPL
*/
static VOID show_license( VOID )
{
    fprintf( stderr,
        "\nThis program is free software; you can redistribute it and/or modify\n"
        "it under the terms of the GNU General Public License as published by\n"
        "the Free Software Foundation; either version 2 of the License, or\n"
        "(at your option) any later version.\n\n"

        "This program is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU General Public License for more details.\n\n"

        "You should have received a copy of the GNU General Public License\n"
        "along with this program; if not, write to the Free Software\n"
        "Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n\n" );

}

/*
** lhp_args_ok
**
** prepare args, check & parse user args, display error and
** help message if neccessary
**
** result: TRUE, if all args ok
*/
BOOL lhp_args_ok( int argc, char *argv[] )
{
    STRPTR arg_fname   = NULL;
    BOOL   arg_help    = FALSE;
    BOOL   arg_license = FALSE;
    BOOL   ok = FALSE;

    /* create arg-table */
    struct arglist *lhp_args = prepare_args( "LHP_ARGS",
              /* file args */
              "FROM"              , &arg_fname  , "prefs file",
              "HELP=?/S"          , &arg_help   , "display this text",
              "LICENSE/S"         , &arg_license, "display license",
              "UNLOAD/S"          , &unload     , "unload old prefs",
              "-DEBUG/S"          , &debug      , "display debugging info",
              NULL );

    ok = ( lhp_args != NULL );

    /* set & test args */
    if ( ok ) {

        ok = set_args( argc, argv, lhp_args );
        ok &= ( !arg_help && !arg_license );

        return_code = RC_WARN;

        if ( arg_license ) {

            /* display license text */
            fprintf_prginfo( stderr );
            show_license();

        } else if ( !ok ) {

            /* display help, if error in args or HELP-switch set */
            fprintf_prginfo( stderr );
            fprintf_arghelp( stderr, lhp_args );

        } else {

            if ( arg_fname )
                fname = strclone( arg_fname );

        }
    } else
        return_code = RC_ERROR;

    /* remove arg-table */
    free_args( lhp_args );

    return( ok );
}

/*
** cleanup - free all resources
*/
VOID cleanup( VOID )
{
    if ( ddeftag ) printf( "first tag: %s\n", ((HSCTAG*)(ddeftag->first->data))->name );
    ufreestr( fname );
    del_hscprc( hp );
    if ( ddeftag ) printf( "first tag: %s\n", ((HSCTAG*)(ddeftag->first->data))->name );
}

/*
** status_file
**
** callback for new file loaded
*/
VOID status_file( HSCPRC *hp )
{
    if ( !quiet ) {

        printf( "reading from `%s'\n", hp->inpf->filename );

    }
}

/*
** status_line
*/
VOID status_line( HSCPRC *hp )
{
    if ( !quiet ) {

        printf( "reading from `%s'\n", hp->inpf->filename );

    }
}

/*
** status_misc
*/
VOID status_misc( HSCPRC *hp, STRPTR s )
{
    if ( !quiet ) {

        printf( "hscl: %s\n", s );

    }
}

/*
** store_prefs_data
**
** write out base address in memory of several
** important data of hsc-process
*/
BOOL store_prefs_data( HSCPRC *hp )
{
    FILE *outf = NULL;
    BOOL  ok   = FALSE;

    errno = 0;

    outf = fopen( HSCBASE_FILE, "w" );
    if ( outf ) {

        fprintf( outf,
                 "DEFTAG %08p\nDEFATTR %08p\nDEFENT %08p\n",
                 hp->deftag, hp->defent, hp->defattr );
        if ( errno )
            perror( "error writing `" HSCBASE_FILE "'" );
        else {

            if ( !quiet )
                printf( "base information written to `"
                        HSCBASE_FILE "'\n" );

            ok = TRUE;

        }
    } else
        perror( "can't open `" HSCBASE_FILE "' for output" );

    fclose( outf );

    return( ok );
}


/*
**
** main function
**
*/
int main( int argc, char *argv[] )
{
    BOOL    debug = FALSE;

    /* set program information */
    set_prginfo( "LoadHscPrefs", "Tommy-Saftwörx", 0,9,0,
        "Permanently load prefs for hsc",
        "Freeware, type \"LoadHscPrefs LICENSE\" for details." );

#ifdef UMEM_TRACKING____not_allowed_here

    /* display a memory tracking report */
    /* at end of execution */
    atexit( atexit_uglymemory );

#endif

    /* install nomem-handler */
    ugly_nomem_handler = hsc_nomem_handler;

    atexit( cleanup );

    /*
    ** main procedure
    */
    hp = new_hscprc();
    if ( hp ) {
        if ( lhp_args_ok( argc, argv ) ) {

            HSCPRC *dummy_hp = NULL;

            /* enable debugging output if neccessary */
            hsc_set_debug( hp, debug );
            printf( "DEBUG = %d\n", debug );

            /* assign callbacks for status-messages */
            hsc_set_status_file_end( hp, status_file );
            hsc_set_status_line( hp, status_line );
            hsc_set_status_misc( hp, status_misc );

            /*
            ** try to read old base information
            ** and free memory allocated by it
            */
            dummy_hp = hsc_read_base_info();
            if ( dummy_hp )
                printf( "a:%08p\ne:%08p\nt:%08p\n",
                dummy_hp->defattr, dummy_hp->defent, dummy_hp->deftag );
            if ( dummy_hp )
                del_hscprc( dummy_hp );

            if ( unload ) {

                /* unload old preferences */
                /* remove base-file */
                remove( HSCBASE_FILE );


            } else {

                /* init process & read preferences */
                hsc_init_hscprc( hp );

                if ( store_prefs_data( hp ) ) {

                    ddeftag = hp->deftag;

                    /* detach required data from process so they won't
                    ** be released by cleanup();
                    **
                    ** NOTE: This is the most perverted source-part of
                    **  the whole hsc-package and works only for
                    **  operating systems that do not perform any
                    **  memory tracking (eg. AmigaOS). This is also
                    **  the only part out of hsclib, that accesses the
                    **  hsc-process NOT via get/set-methodes.
                    */
                    hp->defattr = NULL;
                    hp->defent  = NULL;
                    hp->deftag  = NULL;

                    system( "type " HSCBASE_FILE );

                }
            }

            return_code = RC_OK;
        }
    } else {

        fprintf( stderr, "fatal error: out of memory\n" );

    }

    exit( return_code );
}

