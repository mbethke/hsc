/*
** args.c
**
** argument handling for hsc
**
** updated:  7-Aug-1995
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
    char outfnbuf[ MAX_PATH ];

    struct arglist *my_args;

    /* create arg-table */
    my_args = prepare_args( "HSC_ARGS",
              /* file args */
              "From"          , &inpfilename, "input file",
              "To"            , &outfilename, "output file (default: stdout)",
              "ErrFile=ef/T/K", &errfilename, "error file (default: stderr)",
              "DestDir/K"     , &destdir    , "destination directory",
              /* numeric */
              "MaxErr/N/K"    , &max_error  , "max. number of errors (default:20)",
              /* switches */
              "AbsUrl=au/S"   , &absurl     , "use absolute URLs",
              "CheckUrl=cu/S" , &chkurl     , "check existence of local URLs",
              "InsAnch=ia/S"  , &insanch    , "insert stripped URLs as text",
              "PipeIn=pi/S"   , &pipe_in    , "read input file from stdin",
              "StripUrl=su/S" , &stripurl   , "strip external urls",
              "Status=st/S"   , &statusmsg  , "enable status message",
              "Verbose=v/S"   , &verbose    , "enable verbose output",
              "-Debug/S"      , &debug      , "enable debugging output",
              /* help */
              "HELP=?/S"      , &need_help  , "display this text",
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
        if ( !ok )
            pargerr();
        ok &= ( !need_help && ( inpfilename || pipe_in ) );

        if ( !ok ) {

            /* display help, if error in args or HELP-switch set */
            fprintf_prginfo( stderr );
            fprintf_arghelp( stderr, my_args );

        } else {

            /* TODO: check conflicting options */
            /*       - check urls & pipein */

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

            if ( destdir )
                if ( outfilename )
                    outfilename = strclone( app_fname( destdir, outfilename ) );
                else {

                    if ( !pipe_in ) {



                        /* only destdir, but no outfilename given */
                        /* ->outfilename = destdir + inpfilename + ".html" */

                        /* link destdir & input filename */
                        strcpy( outfnbuf, set_fext( inpfilename, "html" ) );

                        outfilename = strclone( app_fname( destdir, outfnbuf ) );

                    } else {

                        /* TODO: better warning */
                        message( ERROR, NULL );
                        errstr( "Can't evaluate output filename\n" );
                        outfilename = NULL;
                        ok = FALSE;

                    }

            } else if ( outfilename )
                outfilename = strclone( outfilename );

        }

        /* release mem used by args */
        free_args( my_args );

    } else
        /* only for developer */
        printf( "ArgDef error: %2x\n", prep_error_num );

    return ( ok );
}

