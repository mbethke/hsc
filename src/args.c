/*
** args.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** user argument handling for hsc
**
** updated:  4-Dec-1995
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
** TODO: NoExt: do not set extension ".html" to outfilename
*/

/*
** arg_ignore
**
** argument handler for special values that are passed
** to "IGNORE=.." several messages are set to be ignored
** with the old messages left active
*/
STRPTR arg_ignore( STRPTR arg )
{
    STRPTR errmsg = NULL;

    if ( !upstrcmp( arg, IGNORE_ALL_STR ) ) {              /* all */

        /* ignore all non-error messages */
        int i;

        for ( i=0; i<MAX_MSGID; i++ )
            enable_ignore( i );


    } else if ( !upstrcmp( arg, IGNORE_BADSTYLE_STR ) ) {  /* bad style */

        /* ignore bad style messages */
        enable_ignore( MSG_WSPC_AROUND_TAG );
        enable_ignore( MSG_WRONG_HEADING );
        enable_ignore( MSG_CLICK_HERE );
        enable_ignore( MSG_EXPT_H1 );

    } else if ( !upstrcmp( arg, IGNORE_JERKS_STR ) ) {     /* jerks */

        /* ignore jerk messages */
        enable_ignore( MSG_TAG_JERK );
        enable_ignore( MSG_RPLC_SPCCHR );

    } else if ( !upstrcmp( arg, IGNORE_NOTES_STR ) ) {     /* notes */

        /* ignore note messages */
        enable_ignore( MSG_STRIPPED_TAG );
        enable_ignore( MSG_RPLC_ENT );
        enable_ignore( MSG_RPLC_SPCCHR );

    } else {

        /* ignore message # */
        LONG ignnum;

        if ( !str2long( arg, &ignnum ) )
            errmsg = "illegal ignore";
        else
            enable_ignore( ignnum );

    }

    return( errmsg );
}


/*
** arg_mode
**
** argument handler for values that are passed
** to "MODE=..". this one resets all ignored
** messages.
*/
STRPTR arg_mode( STRPTR arg )
{
    STRPTR errmsg = NULL;
    size_t mode = strenum( arg, MODE_ENUMSTR, '|', STEN_NOCASE );

    D( fprintf( stderr, "** args: mode=%s\n", arg ) );

    if ( !mode )
        errmsg = "unknown mode";
    else if ( mode == MODE_PEDANTIC ) {          /* pedantic */

        /* enable all messages */
        int i;

        for ( i=0; i<MAX_MSGID; i++ )
            disable_ignore( i );

    } else if ( mode == MODE_NORMAL ) {          /* normal */

        /* ignore note messages */
        clr_ignore_msg();
        arg_ignore( IGNORE_NOTES_STR );
        enable_ignore( MSG_MISS_REQTAG );
        enable_ignore( MSG_UNMA_QUOTE );
        enable_ignore( MSG_WRONG_HEADING );
        enable_ignore( MSG_EXPT_H1 );
        enable_ignore( MSG_LF_IN_COMMENT );

    } else if ( mode == MODE_RELAXED ) {         /* relaxed */

        arg_mode( MODE_NORMAL_STR );
        arg_ignore( IGNORE_BADSTYLE_STR );
        arg_ignore( IGNORE_JERKS_STR );
        enable_ignore( MSG_TAG_OBSOLETE );
        enable_ignore( MSG_TAG_TOO_OFTEN );
        enable_ignore( MSG_CTAG_NESTING );
        enable_ignore( MSG_EXPT_SEMIK );

    } else {

        /* ignore message # */
        LONG ignnum;

        if ( !str2long( arg, &ignnum ) )
            errmsg = "illegal argument";
        else
            enable_ignore( ignnum );

    }

    return( errmsg );
}

/*
** show_license
**
** display short description of GPL
*/
VOID show_license( VOID )
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
    STRPTR destdir_arg  = NULL; /* temp vars for set_args() */
    STRPTR projdir_arg  = NULL;
    BOOL   license      = FALSE;/* flag: display license message */
    DLLIST *ignore_list = NULL; /* dummy */
    struct arglist *hsc_args;   /* argument structure */

    /* clear ignore/error message list */
    clr_ignore_msg();
    clr_error_msg();

    arg_mode( DEFAULT_MODE_STR );

    /* create arg-table */
    hsc_args = prepare_args( "HSC_ARGS",
              /* file args */
              "From"            , &inpfilename, "input file",
              "To"              , &outfilename, "output file (default: stdout)",
              "ErrFile=ef/T/K"  , &errfilename, "error file (default: stderr)",
              "DestDir/K"       , &destdir_arg, "destination directory",
              "Include=inc/K/M" , &incfile    , "file(s) to be included",
#if 0
              "ProjDir/K"       , &projdir_arg, "project main directory (rel.path)",
#endif
              /* numeric */
              "MaxErr/N/K"      , &max_error  , "max. number of errors (default:20)",
              "Ignore=ign/N/K/M/$", arg_ignore, &ignore_list, "ignore message number",
              "Mode/E/K/$"        , arg_mode, MODE_ENUMSTR, &mode,
                                    "mode for syntax check (" MODE_ENUMSTR ")",
              /* switches */
              "CheckUri=cu/S"   , &chkuri     , "check existence of local URIs",
#if 0
              "InsAnch=ia/S"    , &insanch    , "insert stripped URIs as text",
#endif
              "PipeIn=pi/S"     , &pipe_in    , "read input file from stdin",
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
              "LICENSE/S"       , &license    , "display license",
#if 0
              /* ..I never can remember that f**king syntax
              ** of my own functions..
              */
              "SourceMode=SM/E/K", "ASC|BIN|HEX", &sourcemd, NULL,
              "StartLine=SL/R/K" , 0, 2048, &startline, NULL,
              "NLQ/S"            , &nlq, "enable fast conversion",
              "FormFeed=FF/S"    , &ff, "perform formfeed",
#endif
              NULL );

    /* remove dummy list */
    del_dllist( ignore_list );

    ok = ( hsc_args != NULL );

    /* set & test args */
    if ( ok ) {

        EXPSTR *outfnbuf = init_estr( MAX_PATH );
                                        /* buffer to create output filename */

        ok = set_args( argc, argv, hsc_args );
        ok &= ( !need_help && ( inpfilename || pipe_in ) );

        if ( license ) {

            /* display license text */
            fprintf_prginfo( stderr );
            show_license();

        } else if ( !ok ) {

            /* display help, if error in args or HELP-switch set */
            fprintf_prginfo( stderr );
            fprintf_arghelp( stderr, hsc_args );

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

                    } else {

                        if ( !pipe_in ) {

                            /* only destdir, but no outfilename given */
                            /* ->outfilename = destdir + inpfilename + ".html" */

                            /* link destdir & input filename */
                            set_estr( outfnbuf, set_fext( inpfilename, "html" ) );

                            outfilename = strclone( app_fname( destdir, estr2str( outfnbuf ) ) );

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

        } else D( {

            int i;

            fprintf( stderr, "IGNORE:" );
            for ( i=0; i<MAX_MSGID; i++ )
                if ( ignore(i) )
                    fprintf( stderr, " %d", i );
            fprintf( stderr, "\n" );

        } );


        /* release mem used by args */
        free_args( hsc_args );
        del_estr( outfnbuf );

    } else
        /* only for developer */
        printf( "ArgDef error: %2x\n", prep_error_num );

    return( (BOOL)( ok && (!fatal_error) ) );
}

