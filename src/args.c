/*
** args.c
**
** user argument handling for hsc
**
** updated: 15-Oct-1995
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
** TODO: OutExt: add extension (eg ".html") to outfilename
** TODO: PipeIn: doesn't work cause outfilename is set before
** TODO: use stdout as default
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
    STRPTR destdir_arg = NULL; /* temp vars for set_args() */
    STRPTR projdir_arg = NULL;
    struct arglist *hsc_args;

    /* create arg-table */
    hsc_args = prepare_args( "HSC_ARGS",
              /* file args */
              "From"            , &inpfilename, "input file",
              "To"              , &outfilename, "output file (default: stdout)",
              "ErrFile=ef/T/K"  , &errfilename, "error file (default: stderr)",
              "DestDir/K"       , &destdir_arg, "destination directory",
              "Include=inc/K/M" , &incfile    , "file(s) to be included",
              "ProjDir/K"       , &projdir_arg, "project main directory (rel.path)",

              /* numeric */
              "MaxErr/N/K"      , &max_error  , "max. number of errors (default:20)",
              "Ignore=ign/N/K/M", &ignore     , "ignore message number",
              /* switches */
              "CheckUri=cu/S"   , &chkuri     , "check existence of local URIs",
#if 0
              "InsAnch=ia/S"    , &insanch    , "insert stripped URIs as text",
              "PipeIn=pi/S"     , &pipe_in    , "read input file from stdin",
#endif
              "RplcEnt=re/S"    , &rplc_ent   , "replace entities",
              "SmartEnt=sa/S"   , &smart_ent  , "replace special entities",
#if 0
              "StripCmt=sc/S"   , &stripcmt   , "strip SGML comments",
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

    ok = ( hsc_args != NULL );

    /* set & test args */
    if ( ok ) {

        EXPSTR *outfnbuf = init_estr( MAX_PATH );
                                        /* buffer to create output filename */

        ok = set_args( argc, argv, hsc_args );
        ok &= ( !need_help && ( inpfilename || pipe_in ) );

        if ( !ok ) {

            /* display help, if error in args or HELP-switch set */
            fprintf_prginfo( stderr );
            fprintf_arghelp( stderr, hsc_args );

        } else if ( !outfnbuf ) {

            err_mem( NULL );

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

            /* compute project directory */
            projdir = strclone( app_fname( projdir_arg, NULL ) );
            destdir = strclone( app_fname( destdir_arg, NULL ) );
            if ( !projdir || !destdir )
                err_mem( NULL );

            /*
            ** if no destdir or output-filename given,
            ** outfilename stays NULL. this let open_output
            ** open stdout as output-file
            */
            if ( destdir_arg || outfilename ) {

                /* copy destination directory and */
                /* add "/" if neccessary */
                /* compute destination directory */
                if ( destdir ) {

                    if ( outfilename ) {

                        UBYTE lastch = outfilename[ strlen(outfilename)-1 ];

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
                            if ( !set_estr( outfnbuf, set_fext( inpfilename, "html" ) ) )
                                err_mem( NULL );

                            outfilename = strclone( app_fname( destdir, estr2str( outfnbuf ) ) );
                            if ( !outfilename )
                                err_mem( NULL );

                        } else
                            fnsux = TRUE;

                    }

                }

                if ( fnsux ) {

                    /* no way to find out output filename */
                    message( MSG_NO_OUTFNAME, NULL );
                    errstr( "Can't evaluate output filename\n" );
                    outfilename = NULL;
                    ok = FALSE;

                }
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
        free_args( hsc_args );
        del_estr( outfnbuf );

    } else
        /* only for developer */
        printf( "ArgDef error: %2x\n", prep_error_num );

    return( (BOOL)( ok && (!fatal_error) ) );
}

