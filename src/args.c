/*
** args.c
**
** user argument handling for hsc
**
** updated:  3-Oct-1995
** created:  1-Jul-1995
*/

/*
** ansi includes
*/
#include <stdio.h>
#include <string.h>

/*
** ugly includes
*/
#include "ugly/types.h"
#include "ugly/args.h"
#include "ugly/fname.h"
#include "ugly/memory.h"
#include "ugly/prginfo.h"
#include "ugly/string.h"

/*
** local includes
*/
#include "global.h"
#include "output.h"
#include "error.h"
#include "msgid.h"

/*
** TODO:
** - AppExt: add ".html" to outfilename
*/

/*
** args_ok
**
** prepare args, check & parse user args, display error and
** help message if neccessary
**
** result: TRUE, if all args ok
*/
BOOL args_ok( int argc, char *argv[] )
{
    BOOL   ok; /* return value */
    char outfnbuf[ MAX_PATH ]; /* buffer to create output filename *//* TODO: use expstr */
    STRPTR destdir_arg = NULL; /* temp vars for set_args() */
    STRPTR projdir_arg = NULL;
    struct arglist *my_args;

    /* create arg-table */
    my_args = prepare_args( "HSC_ARGS",
              /* file args */
              "From"            , &inpfilename, "input file",
              "To"              , &outfilename, "output file (default: stdout)",
#if 0
              "ErrFile=ef/T/K"  , &errfilename, "error file (default: stderr)",
#endif
              "DestDir/K"       , &destdir_arg, "destination directory",
#if 0
              "ProjDir/K"       , &projdir_arg, "project main directory (rel.path)",
#endif
              /* numeric */
              "MaxErr/N/K"      , &max_error  , "max. number of errors (default:20)",
              "Ignore=ign/N/K/M", &ignore     , "ignore message number",
              /* switches */
              "AbsUri=au/S"     , &absuri     , "use absolute URIs",
              "CheckUri=cu/S"   , &chkuri     , "check existence of local URIs",
              "InsAnch=ia/S"    , &insanch    , "insert stripped URIs as text",
#if 0
              "PipeIn=pi/S"     , &pipe_in    , "read input file from stdin",
#endif
              "RplcEnt=re/S"    , &rplc_ent   , "replace entities",
#if 0
              "StripUri=su/S"   , &stripuri   , "strip external URIs",
#endif
              "Status=st/S"     , &statusmsg  , "enable status message",
              "Verbose=v/S"     , &verbose    , "enable verbose output",
#if DEBUG
              "-Debug/S"        , &debug      , "enable debugging output",
#endif
              /* help */
              "HELP=?/S"        , &need_help  , "display this text",
#if 0
              "SourceMode=SM/E/K", "ASC|BIN|HEX", &sourcemd, NULL,
              "StartLine=SL/R/K" , 0, 2048, &startline, NULL,
              "NLQ/S"            , &nlq, "enable fast conversion",
              "FormFeed=FF/S"    , &ff, "perform formfeed",
#endif
              NULL );

    ok = ( my_args != NULL );

    /* set & test args */
    if ( ok ) {

        ok = set_args( argc, argv, my_args );
        ok &= ( !need_help && ( inpfilename || pipe_in ) );

        if ( !ok ) {

            /* display help, if error in args or HELP-switch set */
            fprintf_prginfo( stderr );
            fprintf_arghelp( stderr, my_args );

        } else {

            /* TODO: check conflicting options */
            /*       - check URIs & pipein */
            BOOL fnsux = FALSE; /* flag: TRUE = can't evaluate out-filename */

            /* autoset depending options */
            if ( debug )   verbose   = TRUE;
            if ( verbose ) statusmsg = TRUE;

            /* handle pipe_in */
            if ( pipe_in ) {

                if ( inpfilename )
                    if ( !outfilename ) {
                        outfilename = inpfilename;
                        inpfilename = NULL;
                    }
            }

            if ( inpfilename )
                rel_destdir = strclone( get_fpath( inpfilename ) );

            /* copy destination directory and */
            /* add "/" if neccessary */
            destdir = strclone( app_fname( destdir_arg, NULL ) );
            projdir = strclone( app_fname( projdir_arg, NULL ) );

            if ( destdir ) {

                if ( outfilename ) {

                    UBYTE lastch = outfilename[ strlen(outfilename)-1 ];

                    D( fprintf( stderr, "** lastch(%s): \"%c\"\n",
                       outfilename, lastch ) );

                    if ( strchr( PATH_SEPARATOR, lastch ) ) {

                        /* use outfilename as destdir */
                        if ( destdir_arg )
                            fnsux = TRUE;
                        else {

                            ufreestr( destdir );
                            destdir = strclone( outfilename );
                            outfilename = NULL;

                        }
                    }
                }

                if ( outfilename ) {

                    outfilename = strclone( app_fname( destdir, outfilename ) );
                    if ( !outfilename )
                        err_mem( NULL );


                } else {

                    if ( !pipe_in ) {



                        /* only destdir, but no outfilename given */
                        /* ->outfilename = destdir + inpfilename + ".html" */

                        /* link destdir & input filename */
                        strcpy( outfnbuf, set_fext( inpfilename, "html" ) );

                        outfilename = strclone( app_fname( destdir, outfnbuf ) );
                        if ( !outfilename )
                            err_mem( NULL );

                    } else
                        fnsux = TRUE;

                }

            } else
                err_mem( NULL );

            if ( !projdir )
                err_mem( NULL );

            if ( fnsux ) {

                /* no way to find out output filename */
                message( MSG_NO_OUTFNAME, NULL );
                errstr( "Can't evaluate output filename\n" );
                outfilename = NULL;
                ok = FALSE;

            }

        }

        /* display argument error message */
        if ( !ok ) {

            /* NOTE: no strclone() is used on outfilename, if an
            ** error already occured within set_args(). therefore,
            ** you must not call ufreestr( outfilename ) */
            pargerr();
            outfilename = NULL;

        }

        /* release mem used by args */
        free_args( my_args );

    } else
        /* only for developer */
        printf( "ArgDef error: %2x\n", prep_error_num );

    return( (BOOL)( ok && (!fatal_error) ) );
}

