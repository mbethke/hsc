/*
** var args test
*/

#include <stdio.h>

#include "types.h"
#include "args.h"
#include "dllist.h"
#include "prginfo.h"
#include "memory.h"

void prt_ign( FILE *stream, APTR data )
{
    fprintf( stream, "%d\n", (LONG) data );
}

STRPTR set_ignore( STRPTR arg )
{
    printf( "set ignore: \"%s\"\n", arg );

    return ( NULL );
}

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

    printf( "ign : %s\n", arg );

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

    printf( "mode: %s\n", arg );

    return( errmsg );
}

/*
** main (test)
*/
int main( int argc, char *argv[] )
{
    STRPTR fromfile = NULL;
    STRPTR errfile = NULL;
    STRPTR tofile = NULL;
    STRPTR destdir = NULL;
    BOOL   help = FALSE;
    LONG   mode = 0;
    LONG   num  = 0;
    LONG   line = 0;
    BOOL   verb = 0;
    BOOL   flag = FALSE;
    DLLIST *ignore = NULL;
    DLLIST *ignore_list = NULL; /* dummy */

    struct arglist *my_args;

#ifdef UMEM_TRACKING
    atexit( atexit_uglymemory );
#endif
    /*
    ** set program information
    */
    set_prginfo( "test_args", "Tommy-Saftwörx", 1, 0, 0,
        "a short test program", "This is FreeWare!" );

    my_args = prepare_args( "MY_ARGS",
#if 0
              "From"          , &fromfile, "input file",
              "To"            , &tofile, "output file (default: stdout)",
              "ErrFile=ef/T/K", &errfile, "error file (default: stderr)",
              "DestDir/K"     , &destdir    , "destination directory",
              "MaxErr/N/K"    , &num  , "max. number of errors (default:20)",
              /* switches */
              "AbsUrl=au/S"   , &flag     , "use absolute URLs",
              "CheckUrl=cu/S" , &flag , "check existence of local URLs",
              "InsAnch=ia/S"  , &flag       , "insert stripped URLs as text",
              "PipeIn=pi/S"   , &flag       , "read input file from stdin",
              "StripUrl=su/S" , &flag       , "strip external urls",
              "Status=st/S"   , &flag       , "enable status message",
              "Verbose=v/S"   , &verb       , "enable verbose output",
              "-Debug/S"      , &flag       , "enable debugging output",
#endif
              /* help */

#if 0
              "FROM", &fromfile, "input file",
              "TO", &tofile, "output file",
              "Num/N"            , &num, "number of lines to convert",
#endif
#if 1
              "Ignore=ign/N/K/M/$", arg_ignore, &ignore_list, "ignore message number",
              "Mode/E/K/$"        , arg_mode, "hugo|sepp|resi", &mode,
              "ERRFILE/K", &errfile, "error file",
              "VERBOSE/S", &verb, "verbose flag",
              "StartLine=SL/R/K" , 0, 2048, &line, NULL,
#endif
              "HELP=?/S"      , &help  , "display this text",
              NULL );


#ifdef UMEM_TRACKING__0
    fprintf( stderr, "<mem report>\n" );
    umem_report( "testargs - end" );
#endif
    if ( my_args ) {

        fprintf_prginfo      ( stdout );
        printf( "**short help**\n" );
        fprintf_arghelp_short( stdout, my_args );
#if 0
        if (help ) {
            printf( "**help**\n" );
            fprintf_arghelp( stdout, my_args );
        }
#endif
        if ( set_args( argc, argv, my_args ) ) {

                printf( "args ok:\n" );

                if (fromfile) printf( "  from: \"%s\"\n", fromfile );
                if (tofile)   printf( "  to  : \"%s\"\n", tofile );
                if (errfile)  printf( "  err : \"%s\"\n", errfile );
                if ( help )   printf( "  help: TRUE\n" );
                if ( verb )   printf( "  verb: TRUE\n" );
                if ( num )    printf( "  num : %d\n", num );
                if ( ignore)  {
                    printf( " ign :\n" );
                    fprintf_dllist( stdout, ignore, prt_ign );
                }
                if ( line )   printf( "  line: %d\n", line );
                if ( mode )   printf( "  mode: %d\n", mode );

        } else

            pargerr();

        /* cleanup args */
        free_args( my_args );
        del_dllist( ignore );

    } else
        
        printf( "ArgDef error: %2x\n", prep_error_num );
    return 0;
}

