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
              /* help */
              "HELP=?/S"      , &help  , "display this text",

#if 0
              "FROM", &fromfile, "input file",
              "TO", &tofile, "output file",
              "IGNORE=IGN/N/K/M", &ignore, "ignore warning number",
              "Num/N"            , &num, "number of lines to convert",
#endif
#if 0
              "HELP/S", &help, "display help",
              "ERRFILE/K", &errfile, "error file",
              "VERBOSE/S", &verb, "verbose flag",
              "StartLine=SL/R/K" , 0, 2048, &line, NULL,
              "SourceMode=SM/E/K", "ASC|BIN|HEX", &mode, NULL,
#endif
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

