/*
** hsc/args.c
**
** user argument handling for hsc
**
** Copyright (C) 1995,96  Thomas Aglassinger
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
** updated: 15-May-1996
** created:  1-Jul-1995
*/

#include "hsc/global.h"
#include "hsc/status.h"

/*
** ugly includes
*/
#include "ugly/args.h"
#include "ugly/fname.h"
#include "ugly/prginfo.h"
#include "ugly/returncode.h"

/*
** TODO: NOEXT: do not set extension ".html" to outfilename
** TODO: MSGFILE instead of ERRFILE
** TODO: MSGFORMAT
** TODO: HTML3 switch to disable some warning
*/

static STRPTR arg_inpfname = NULL; /* temp vars for set_args() */
static STRPTR arg_outfname = NULL;
static LONG   arg_quotemode = QMODE_KEEP;
static BOOL   arg_mode      = FALSE;
static BOOL   arg_compact   = FALSE;
static BOOL   arg_getsize   = FALSE;
static BOOL   arg_rplc_ent  = FALSE;
static BOOL   arg_smart_ent = FALSE;
static BOOL   arg_jens      = FALSE;
static BOOL   arg_strip_cmt = FALSE;
static BOOL   arg_strip_ext = FALSE;
static BOOL   arg_pipe_in   = FALSE;
static BOOL   arg_license   = FALSE;
static BOOL   arg_help      = FALSE;
static BOOL   arg_debug     = FALSE;
static STRPTR arg_prjfile   = NULL;
static STRPTR arg_striptags = NULL;
#if 0
static BOOL   arg_chkuri    = FALSE;
static STRPTR arg_destdir   = NULL;
static LONG   arg_entmode   = EMODE_KEEP;
#endif

static HSCPRC *arg_hp = NULL;

/*
** arg_ignore
**
** argument handler for special values that are passed
** to "IGNORE=.." several messages are set to be ignored
** with the old messages left active
*/
static STRPTR arg_ignore_CB( STRPTR arg )
{
    STRPTR  errmsg = NULL;
    HSCPRC *hp = arg_hp;

    if ( !upstrcmp( arg, IGNORE_ALL_STR ) ) {              /* all */

        /* ignore all non-error messages */
        HSCMSG_ID i;

        for ( i=0; i<MAX_MSGID; i++ )
            hsc_set_msg_ignore( hp, i, TRUE );


    } else if ( !upstrcmp( arg, IGNORE_BADSTYLE_STR ) ) {  /* bad style */

        /* ignore bad style messages */
        hsc_set_msg_ignore( hp, MSG_WSPC_AROUND_TAG, TRUE );
        hsc_set_msg_ignore( hp, MSG_WRONG_HEADING, TRUE );
        hsc_set_msg_ignore( hp, MSG_CLICK_HERE, TRUE );
        hsc_set_msg_ignore( hp, MSG_EXPT_H1, TRUE );

    } else if ( !upstrcmp( arg, IGNORE_JERKS_STR ) ) {     /* jerks */

        /* ignore jerk messages */
        hsc_set_msg_ignore( hp, MSG_TAG_JERK, TRUE );
        hsc_set_msg_ignore( hp, MSG_RPLC_SPCCHR, TRUE );

    } else if ( !upstrcmp( arg, IGNORE_NOTES_STR ) ) {     /* notes */

        /* ignore note messages */
        hsc_set_msg_ignore( hp, MSG_TAG_STRIPPED, TRUE );
        hsc_set_msg_ignore( hp, MSG_RPLC_ENT, TRUE );
        hsc_set_msg_ignore( hp, MSG_RPLC_SPCCHR, TRUE );

    } else {

        /* ignore message # */
        LONG ignnum;

        if ( !str2long( arg, &ignnum ) )
            errmsg = "illegal ignore";
        else
            hsc_set_msg_ignore( hp, ignnum, TRUE );

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
static STRPTR arg_mode_CB( STRPTR arg )
{
    STRPTR errmsg = NULL;
    size_t mode = strenum( arg, MODE_ENUMSTR, '|', STEN_NOCASE );
    HSCPRC *hp = arg_hp;

    D( fprintf( stderr, DHP "args: mode=%s\n", arg ) );

    if ( !mode )
        errmsg = "unknown mode";
    else if ( mode == MODE_PEDANTIC ) {          /* pedantic */

        /* enable all messages */
        HSCMSG_ID i;

        for ( i=0; i<MAX_MSGID; i++ )
            hsc_set_msg_ignore( hp, i, FALSE );

    } else if ( mode == MODE_NORMAL ) {          /* normal */

        /* ignore note messages */
        arg_mode_CB( MODE_PEDANTIC_STR );
        arg_ignore_CB( IGNORE_NOTES_STR );
        hsc_set_msg_ignore( hp, MSG_MISS_REQTAG, TRUE );
        hsc_set_msg_ignore( hp, MSG_UNMA_QUOTE, TRUE );
        hsc_set_msg_ignore( hp, MSG_WRONG_HEADING, TRUE );
        hsc_set_msg_ignore( hp, MSG_EXPT_H1, TRUE );
        hsc_set_msg_ignore( hp, MSG_LF_IN_COMMENT, TRUE );

    } else if ( mode == MODE_RELAXED ) {         /* relaxed */

        arg_mode_CB( MODE_NORMAL_STR );
        arg_ignore_CB( IGNORE_BADSTYLE_STR );
        arg_ignore_CB( IGNORE_JERKS_STR );
        hsc_set_msg_ignore( hp, MSG_TAG_OBSOLETE, TRUE );
        hsc_set_msg_ignore( hp, MSG_TAG_TOO_OFTEN, TRUE );
        hsc_set_msg_ignore( hp, MSG_CTAG_NESTING, TRUE );
        hsc_set_msg_ignore( hp, MSG_EXPT_SEMIK, TRUE );

    } else {

        /* ignore message # */
        LONG ignnum;

        if ( !str2long( arg, &ignnum ) )
            errmsg = "illegal argument";
        else
            hsc_set_msg_ignore( hp, ignnum, TRUE );

    }

    return( errmsg );
}

/*
** arg_status
**
** argument handler for values that are passed
** to "STATUS=..".
*/
static STRPTR arg_status_CB( STRPTR arg )
{
    STRPTR errmsg = NULL;
    STRPTR argstr = strclone( arg );
    STRPTR argold = argstr;            /* used to call ufree() */
#if DEBUG
    HSCPRC *hp = arg_hp;
#endif
    D( fprintf( stderr, DHP "args: status=%s\n", arg ) );

    arg = strtok( argstr, "|" );
    while( arg ) {

        if ( !upstrcmp( arg, STATUS_QUIET_STR ) )
            disp_status = STATUS_QUIET;
        else if ( !upstrcmp( arg, STATUS_LINE_STR ) )
            disp_status |= STATUS_LINE;
        else if ( !upstrcmp( arg, STATUS_VERSION_STR ) )
            disp_status |= STATUS_VERSION;
        else if ( !upstrcmp( arg, STATUS_VERBOSE_STR ) )
            disp_status |= STATUS_VERBOSE;
        else
            errmsg = "illegal argument";

        arg = strtok( NULL, "|" );

    }

    ufreestr( argold );

    return( errmsg );
}


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
** user_defines_ok
**
** process all defines passed via user args
**
** result: always TRUE
*/
BOOL user_defines_ok( HSCPRC *hp )
{
    if ( define_list && dll_first( define_list ) )
    {
        DLNODE *nd = dll_first( define_list );
        EXPSTR *defbuf = init_estr( 64 );
#if 0
        BOOL   old_ignore_quotemsg =
                   hsc_get_msg_ignore( hp, MSG_ARG_NO_QUOTE );
#endif

        while ( nd )
        {
            STRPTR defarg = (STRPTR) dln_data( nd );

            set_estr( defbuf, "<$define " );

            /* append attribute name & type */
            do {
                app_estrch( defbuf, defarg[0] );
                defarg++;
            } while ( defarg[0] && (defarg[0]!='=') );

            /* append quotes and value */
            if ( defarg[0] )
            {
                BOOL quote_needed = FALSE; /* flag: user did not use quotes */

                /* append "=" */
                app_estrch( defbuf, defarg[0] );
                defarg++;

                /* append quote (if not already done by user) */
                if ( defarg[0] != '\"' )
                {
                    quote_needed = TRUE;
                    app_estrch( defbuf, '\"' );
                }

                /* append value */
                while ( defarg[0] )
                {
                    app_estrch( defbuf, defarg[0] );
                    defarg++;
                }

                /* append quote (if not already done by user) */
                if ( quote_needed )
                    app_estrch( defbuf, '\"' );
            }

            /* append end ">" */
            app_estrch( defbuf, '>' );

            D( fprintf( stderr, DHP "define: `%s'\n",  estr2str(defbuf) ) );

            hsc_include_string( hp, "[arg define]",
                                estr2str(defbuf), IH_PARSE_HSC | IH_NO_STATUS );
            nd = dln_next( nd );
        }

        del_estr( defbuf );
#if 0
        hsc_set_msg_ignore( hp, MSG_ARG_NO_QUOTE, old_ignore_quotemsg );
#endif
    }
    else
    {
        D( fprintf( stderr, DHP "(no defines)\n" ) );
    }

    return( (BOOL) (return_code < RC_ERROR) );
}


/*
** args_ok
**
** prepare args, check & parse user args, display error and
** help message if neccessary
**
** result: TRUE, if all args ok
*/
BOOL args_ok( HSCPRC *hp, int argc, char *argv[] )
{
    BOOL   ok; /* return value */
    DLLIST *ignore_list = NULL; /* dummy */
    EXPSTR *destdir     = init_estr( 32 );  /* destination dir */
    EXPSTR *rel_destdir = init_estr( 32 );  /* relative destination dir */
    struct arglist *hsc_args;   /* argument structure */

    arg_hp = hp;

    arg_mode_CB( DEFAULT_MODE_STR );

    /* create arg-table */
    hsc_args = prepare_args( "HSC_ARGS",
              /* file args */
              "FROM/M"            , &incfile        , "input and include file(s)",
              "TO/K"              , &arg_outfname   , "output file (default: stdout)",
              "MSGFILE=MF/T/K"    , &msgfilename    , "message file (default: stderr)",
              "MSGFORMAT/T/K"     , &msg_format     , "how to display message",
              "PRJFILE/T/K"       , &arg_prjfile    , "project file",
#if 0
              "DESTDIR/K"         , &arg_destdir    , "destination directory",
              "ProjDir/K"         , &projdir_arg    , "project main directory (rel.path)",
#endif
              /* numeric */
              "MAXERR/N/K"        , &max_error      , "max. number of errors (default:20)",
#if 0
              "DEFINE=DEF/T/K/M"  , &define_list    , "define global attribute",
#endif
              "IGNORE=IGN/N/K/M/$", arg_ignore_CB, &ignore_list, "ignore message number",
              "MODE/E/K/$"        , arg_mode_CB, MODE_ENUMSTR, &arg_mode,
                                    "mode for syntax check (" MODE_ENUMSTR ")",
#if 0
              "QUOTEMODE=QM/E/K"  , QMODE_ENUMSTR, &arg_quotemode,
                                    "defines how quotes appear (" QMODE_ENUMSTR ")",
              "ENTITYMODE=EM/E/K" , EMODE_ENUMSTR, &entmode,
                                    "defines how special chars. appear (" EMODE_ENUMSTR ")",
              /* switches */
              "CHECKURI=CU/S"     , &arg_chkuri     , "check existence of local URIs",
#endif
              "COMPACT=CO/S"      , &arg_compact    , "stripe useless LFs and white-spaces",
              "GETSIZE/S"         , &arg_getsize    , "get width and height of images",
              "PIPEIN=PI/S"       , &arg_pipe_in    , "read input file from stdin",
              "RPLCENT=RE/S"      , &arg_rplc_ent   , "replace special characters",
              "SMARTENT=SA/S"     , &arg_smart_ent  , "replace special entities (`&<>\"')",
              "JENS/S"            , &arg_jens       , "don't try this at home",
              "STRIPCOMMENT=SC/S" , &arg_strip_cmt  , "strip SGML comments",
              "STRIPEXTERNAL=SX/S", &arg_strip_ext  , "strip tags with external URIs",
              "STRIPTAGS=ST/K"    , &arg_striptags  , "tags to be stripped",
              "STATUS/E/K/$"      , arg_status_CB, STATUS_ENUM_STR,
                                    &disp_status    , "status message (" STATUS_ENUM_STR ")",
              "-DEBUG/S"          , &arg_debug      , "enable debugging output",
              /* help */
              "HELP=?/S"          , &arg_help       , "display this text",
              "LICENSE/S"         , &arg_license    , "display license",

              NULL );

    /* remove dummy list TODO: this sucks */
    del_dllist( ignore_list );

    ok = ( hsc_args != NULL );

    /* set & test args */
    if ( ok )
    {
        BOOL use_stdout = FALSE; /* flag: use stdout as output-file */

        ok = set_args( argc, argv, hsc_args );

        /* display help, if requested vie HELP switch, or no
         * input to pipe or read is passed */
        ok &= ( !arg_help &&
                ( arg_pipe_in || (incfile && dll_first(incfile)) ) );

        if ( arg_license ) {

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

            /* set debugging switch */
            hsc_set_debug( hp, arg_debug );

            /* autoset depending options */
            if ( hsc_get_debug( hp ) )
                disp_status = STATUS_VERBOSE;

            /* compute name of input file */
            arg_inpfname = NULL;
            if ( dll_first( incfile ) && !arg_pipe_in ) {

                /* use first FROM as input file */
                arg_inpfname = dln_data( dll_first( incfile ) );

                set_estr( inpfilename, arg_inpfname );

                /* get path part of inputfilename as relative
                ** destination directory */
                get_fpath( rel_destdir, arg_inpfname ); /* TODO: set reldir when including first file */

                /* remove input filename from incfile */
                del_dlnode( incfile, dll_first( incfile ) );

                D( fprintf( stderr, DHP "input : use `%s'\n"
                                    DHP "reldir: use `%s'\n",
                            estr2str( inpfilename ), estr2str( rel_destdir ) ) );

            }

            /* display include files */
            D( {

                DLNODE *nd = dll_first( incfile );

                while ( nd )
                {
                    fprintf( stderr, DHP "includ: use `%s'\n", (
                                     STRPTR) dln_data( nd ) );
                    nd = dln_next( nd );
                }

            } );

            /*
            ** if no output-filename given,
            ** outfilename stays NULL. this let open_output
            ** open stdout as output-file
            */
            if ( arg_outfname ) {

                /* check, if last char of outputfilename is a
                ** directory separator; if so, use the filename
                ** as destination directory
                */
                if ( arg_outfname ) {

                    UBYTE lastch = arg_outfname[ strlen(arg_outfname)-1 ];

                    if ( strchr( PATH_SEPARATOR, lastch ) ) {

                        /* use outfilename as destdir */
                        set_estr( destdir, arg_outfname );
                        arg_outfname = NULL;
                        D( fprintf( stderr, DHP "output: use `%s' as destdir\n",
                                    estr2str( destdir ) ) );

                    }
                }

                if ( arg_outfname ) {

                    /* set outputfilename with value passed iwithin args */
                    outfilename = init_estr( 32 );
                    set_estr( outfilename, arg_outfname );
                    D( fprintf( stderr, DHP "output: set to `%s'\n",
                                estr2str( outfilename ) ) );

                } else {

                    if ( !arg_pipe_in ) {

                        /* no outfilename given */
                        /* ->outfilename = destdir + inpfilename + ".html" */

                        /* link destdir & input filename */
                        outfilename = init_estr( 32 );
                        link_fname( outfilename, estr2str( destdir ), arg_inpfname );
                        set_fext( outfilename, "html" );
                        D( fprintf( stderr,
                                    DHP "output: concat destdir+inpfile+`.html'\n"
                                    DHP "output: set to `%s'\n",
                                    estr2str( outfilename ) ) );


                    } else
                        fnsux = TRUE;

                }

                if ( fnsux ) {

                    /* no way to find out output filename */
                    status_error( "unable to evaluate output filename\n" );
                    arg_outfname = NULL;
                    ok = FALSE;

                }
            } else {
                D( fprintf( stderr, DHP "output: use stdout\n" ) );
                use_stdout = TRUE;
            }
        }

        if (ok ) {

            if ( arg_prjfile )
                hsc_set_filename_project( hp, arg_prjfile );
            if ( !use_stdout )
                hsc_set_filename_process( hp, estr2str( outfilename ) );
        }

        /* display argument error message */
        if ( !ok ) {

            /* NOTE: no strclone() is used on outfilename, if an
            ** error already occured within set_args(). therefore,
            ** you must not call ufreestr( outfilename ) */
            pargerr();
            arg_outfname = NULL;

        } else { D( {
            HSCMSG_ID i;

            fprintf( stderr, "\n"
                             DHP "input : `%s'\n", estr2str( inpfilename ) );
            fprintf( stderr, DHP "output: `%s'\n", get_outfilename() );
            fprintf( stderr, DHP "destdr: `%s'\n", estr2str( destdir ) );
            fprintf( stderr, DHP "reldst: `%s'\n", estr2str( rel_destdir ) );
            if ( arg_prjfile )
                fprintf( stderr, DHP "projct: `%s'\n", arg_prjfile );
            if ( !use_stdout )
                fprintf( stderr, DHP "procss: `%s'\n", estr2str( outfilename ) );
            fprintf( stderr, DHP "ignore:" );
            for ( i=0; i<MAX_MSGID; i++ )
                if ( hsc_get_msg_ignore(hp, i) )
                    fprintf( stderr, " %lu", i );
            fprintf( stderr, "\n" );

        } ); }

        /*
        ** set flags of hsc-process
        */
        if ( ok )
        {
            hsc_set_chkid( hp, TRUE );
            hsc_set_chkuri( hp, TRUE );
            hsc_set_compact( hp, arg_compact );
            hsc_set_debug( hp, arg_debug );
            hsc_set_getsize( hp, arg_getsize );
            hsc_set_jens( hp, arg_jens );
            hsc_set_rplc_ent( hp, arg_rplc_ent );
            hsc_set_smart_ent( hp, arg_smart_ent );
            hsc_set_strip_cmt( hp, arg_strip_cmt );
            hsc_set_strip_ext( hp, arg_strip_ext );

            hsc_set_quote_mode( hp, arg_quotemode );

            /* set diretories */
            hsc_set_destdir( hp, estr2str( destdir ) );
            hsc_set_reldir( hp, estr2str( rel_destdir ) );
        }

        /* release mem used by args */
        free_args( hsc_args );

    }
    else
    {
        D(
            /* only for developer */
            fprintf( stderr, "ArgDef error: %lu\n", prep_error_num );
        );
    }

    del_estr( destdir );
    del_estr( rel_destdir );

#if 1
    return( ok );
#else
    return( FALSE ); /* for arg-debugging */
#endif
}

