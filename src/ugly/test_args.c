/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * test args
 */

#include <stdio.h>

#include "utypes.h"
#include "uargs.h"
#include "dllist.h"
#include "prginfo.h"
#include "umemory.h"

void prt_ign(FILE * stream, APTR data)
{
    fprintf(stream, "%ld\n", (LONG) data);
}

STRPTR set_ignore(STRPTR arg)
{
    printf("set ignore: \"%s\"\n", arg);

    return (NULL);
}

/*
 * arg_ignore
 *
 * argument handler for special values that are passed
 * to "IGNORE=.." several messages are set to be ignored
 * with the old messages left active
 */
STRPTR arg_ignore(STRPTR arg)
{
    STRPTR errmsg = NULL;

    printf("ign : %s\n", arg);

    return (errmsg);
}

/*
 * arg_mode
 *
 * argument handler for values that are passed
 * to "MODE=..". this one resets all ignored
 * messages.
 */
STRPTR arg_mode(STRPTR arg)
{
    STRPTR errmsg = NULL;

    printf("mode: %s\n", arg);

    return (errmsg);
}

/*
 * main (test)
 */
int main(int argc, char *argv[])
{
    DLLIST *fromfiles = NULL;
    STRPTR errfile = NULL;
    STRPTR tofile = NULL;
    BOOL help = FALSE;
    BOOL flag = FALSE;
    STRPTR destdir = NULL;
    LONG mode = 0;
    LONG num = 0;
    LONG line = 0;
    BOOL verb = 0;
    DLLIST *ignore = NULL;
#if 0
    DLLIST *ignore_list = NULL; /* dummy */
#endif

    struct arglist *my_args;

#if DEBUG_UGLY_MEMORY
    atexit(atexit_uglymemory);
#endif
    /*
     * set program information
     */
    set_prginfo("test_args", "Tommy-Saftwörx", 1, 0, 0,
                "a short test program", "This is FreeWare!");

    my_args = prepare_args("MY_ARGS",
#if 1
                           "From/M", &fromfiles, "input file",
                           "To/K", &tofile, "output file (default: stdout)",
                 "ErrFile=ef/T/K", &errfile, "error file (default: stderr)",
                           "DestDir/K", &destdir, "destination directory",
                   "MaxErr/N/K", &num, "max. number of errors (default:20)",
    /* switches */
                           "AbsUrl=au/S", &flag, "use absolute URLs",
                    "CheckUrl=cu/S", &flag, "check existence of local URLs",
                      "InsAnch=ia/S", &flag, "insert stripped URLs as text",
                         "PipeIn=pi/S", &flag, "read input file from stdin",
                           "StripUrl=su/S", &flag, "strip external urls",
                           "Status=st/S", &flag, "enable status message",
                           "Verbose=v/S", &verb, "enable verbose output",
                           "-Debug/S", &flag, "enable debugging output",
#endif
    /* help */

#if 0
                           "FROM", &fromfile, "input file",
                           "TO", &tofile, "output file",
                           "Num/N", &num, "number of lines to convert",
#endif
#if 0
    "Ignore=ign/N/K/M/$", arg_ignore, &ignore_list, "ignore message number",
    "Mode/E/K/$", arg_mode, "hugo|sepp|resi", &mode, "set the fucking mode",
                           "ERRFILE/K", &errfile, "error file",
                           "VERBOSE/S", &verb, "verbose flag",
                           "StartLine=SL/R/K", 0, 2048, &line, NULL,
#endif
                           "HELP=?/S", &help, "display this text",
                           NULL);

#if DEBUG_UGLY_MEMORY__0
    fprintf(stderr, "<mem report>\n");
    umem_report("testargs - end");
#endif
    if (my_args)
    {
        ARGFILE *argf = NULL;

        fprintf_prginfo(stdout);
        printf("**short help**\n");
        fprintf_arghelp_short(stdout, my_args);
#if 0
        if (help)
        {
            printf("**help**\n");
            fprintf_arghelp(stdout, my_args);
        }
#endif
        /* read argfile */
        argf = new_argfile("test_args.options");
        if (argf)
        {
            int i=1;
            printf( "argfile `%s', %d args\n", argf->argv[0], argf->argc);
            while( argf->argv[i])
            {
                printf( "  %2d: `%s'\n", i, argf->argv[i]);
                i++;
            }
        }

        if (set_args_file(argf, my_args)
            && set_args(argc, argv, my_args)
            && check_args( my_args))
        {

            DLNODE *nd = NULL;
            if (fromfiles)
                nd = dll_first(fromfiles);

            printf("args ok:\n");

            while (nd)
            {
                printf("  from: \"%s\"\n", (STRPTR) nd->data);
                nd = nd->next;
            }

            if (tofile)
                printf("  to  : \"%s\"\n", tofile);
            if (errfile)
                printf("  err : \"%s\"\n", errfile);
            if (help)
                printf("  help: TRUE\n");
            if (verb)
                printf("  verb: TRUE\n");
            if (num)
                printf("  num : %ld\n", num);
            if (ignore)
            {
                printf("  ign :\n");
                fprintf_dllist(stdout, ignore, prt_ign);
            }
            if (line)
                printf("  line: %ld\n", line);
            if (mode)
                printf("  mode: %ld\n", mode);

        }
        else
            pargerr();

        /* cleanup args */
        del_argfile(argf);
        free_args(my_args);
        del_dllist(ignore);
        del_dllist(fromfiles);

    }
    else
        printf("ArgDef error: %2lu\n", prep_error_num);
    return 0;
}

