/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hscpitt
 *
 * hsc project interfering'n'trashing tool
 *
 *-------------------------------------------------------------------
 *
 * hsctools/hscpitt.c
 *
 * updated: 14-Oct-1997
 * created: 15-Oct-1996
 */

/* ANSI includes */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/* include revision data */
#include "hsctools/pitt_rev.h"

/* ugly includes */
#include "ugly/ustring.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/infile.h"
#include "ugly/ufile.h"
#include "ugly/uargs.h"
#include "ugly/ustrlist.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"

/* hsclib includes */
#include "hscprj/document.h"
#include "hscprj/project.h"
#include "hscprj/license.h"

#ifdef AMIGA
/* AmigaOS version string
 * (imported from "pitt_rev.h")
 */
static const STRPTR AmigaOS_version = VERSTAG;
#endif

/* prefix for messages */
#define HP ""
#define DHP "*hscpit* "
#define SHIT "*** "             /* prefix for total failure */

/* debugging define */
#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#endif

#ifdef D
#undef D
#endif

#if DEBUG
#define D(x) if (debug) {x;}
#else
#define D(x) {/*nufin*/}
#endif

/* default parameters */
#define DEFAULT_PROJECT "hsc.project"   /* project-filename */

/* commands supported */
#define CMD_COUNT_STR   "count"
#define CMD_LIST_STR    "list"
#define CMD_EXTRACT_STR "extract"
#define CMD_DELETE_STR  "delete"
#define CMD_ERASE_STR   "erase"
#define CMD_ADD_STR     "add"
#define CMD_NEW_STR     "new"
#define CMD_COPY_STR    "copy"
#define CMD_MOVE_STR    "move"

#define COMMAND_NONE    0
#define COMMAND_COUNT   1
#define COMMAND_LIST    2
#define COMMAND_EXTRACT 3
#define COMMAND_DELETE  4
#define COMMAND_ERASE   5
#define COMMAND_ADD     6
#define COMMAND_NEW     7
#define COMMAND_COPY    8
#define COMMAND_MOVE    9

#define COMMAND_ENUMSTR     \
        CMD_COUNT_STR   "|" \
        CMD_LIST_STR    "|" \
        CMD_EXTRACT_STR "|" \
        CMD_DELETE_STR  "|" \
        CMD_ERASE_STR   "|" \
        CMD_ADD_STR     "|" \
        CMD_NEW_STR

/*
 * global vars
 */

/* command strings */
static STRPTR command_name[] =
{
    "***NONE***",
    CMD_COUNT_STR,
    CMD_LIST_STR,
    CMD_EXTRACT_STR,
    CMD_DELETE_STR,
    CMD_ERASE_STR,
    CMD_ADD_STR,
    CMD_NEW_STR,
    NULL
};

static int return_code = RC_FAIL;       /* exit code of program */

static STRPTR prjfile = NULL;
static LONG command = COMMAND_LIST;
static LONG cmdArgNum = 0;      /* number of args for command (ARG/M) */
static DLLIST *command_arglist = NULL;
static BOOL force = FALSE;
static BOOL quiet = FALSE;
static BOOL debug = FALSE;
static BOOL arg_help = FALSE;
static BOOL arg_license = FALSE;

static HSCPRJ *project = NULL;

/*
 * cleanup: free all resources
 * (called in any case)
 */
static VOID cleanup(VOID)
{
    D(fprintf(stderr, "(cleanup)\r"));
    del_dllist(command_arglist);
    del_project(project);
    D(fprintf(stderr, "         \r"));
}

static VOID set_return_code(int new_code)
{
    if (new_code > return_code)
        return_code = new_code;
}

/*
 * hsc_nomem_handler
 *
 * called from ugly/umalloc, if malloc() did return NULL
 */
static BOOL hscpitt_nomem_handler(size_t size)
{
    fputs(SHIT "out of memory\n", stderr);

    return_code = RC_FAIL;
    exit(return_code);          /* abort immediatly */

    return (FALSE);
}

VOID msg_corrupt_pf(HSCPRJ * hp, STRPTR reason)
{
    fprintf(stderr, "project-file corrupt: %s\n", reason);
    set_return_code(RC_ERROR);
}

/*
 * update_project_file
 *
 * write updated project file, handle errors
 */
BOOL update_project_file(HSCPRJ * project)
{
    BOOL ok = FALSE;

    D(fprintf(stderr, DHP "update_project_file()\n"));
    /* TODO: create backup */

    errno = 0;
    ok = hsc_project_write_data(project, prjfile, TRUE);

    if (!ok)
    {
        fprintf(stderr, HP "error writing `%s'", prjfile);
        if (errno)
            fprintf(stderr, HP ": %s", strerror(errno));
        fprintf(stderr, "\n");
        set_return_code(RC_ERROR);
    }

    return (ok);
}

/*-------------------------------------------------------------------
 *
 * functions for arguments
 *
 *-------------------------------------------------------------------*/

/*
 * check_min_cmd_args
 *
 * check, if at least a specific num of args has been specified
 * for the command
 */
BOOL check_min_cmd_args(LONG minArgNum, DLLIST * list)
{
    BOOL ok = TRUE;
    DLNODE *nd = dll_first(list);
    LONG argNum = 0;

    /* count arguments */
    while (nd)
    {
        argNum++;
        nd = dln_next(nd);
    }

    if (argNum < minArgNum)
    {
        fprintf(stderr, "command %s requires at least %ld arguments\n",
                command_name[command], minArgNum);
        ok = FALSE;
    }

    return (ok);
}

/*
 * args_ok
 *
 * prepare args, check & parse user args, display error and
 * help message if neccessary
 *
 * result: TRUE, if all args ok and no request for HELP or
 *         LICENSE has been detected
 */
static BOOL args_ok(int argc, char *argv[])
{
    struct arglist *hscpitt_args;       /* argument structure */
    BOOL arg_help = FALSE;
    BOOL arg_license = FALSE;
    BOOL ok = FALSE;

    /* create arg-table */
    hscpitt_args = prepare_args
        ("HSCPITT_ARGS",
         "COMMAND/E", COMMAND_ENUMSTR, &command,
         "command to perform (" COMMAND_ENUMSTR ")",
         "ARG/T/M", &command_arglist, "command argument(s)",
         "PRJFILE/T/K", &prjfile, "project file",
         "FORCE/S", &force, "disable certain checks",
         "QUIET/S", &quiet, "act quietly",
         "-DEBUG/S", &debug, "enable debugging output",
         "HELP=?=-h=--help/S", &arg_help, "display this text",
         "LICENSE/S", &arg_license, "display license",
         NULL);

    ok = (hscpitt_args != NULL);

    /* set & test args */
    if (ok)
    {
        ok = set_args(argc, argv, hscpitt_args);

        /* display argument error message */
        if (!ok)
        {
            pargerr();
            set_return_code(RC_ERROR);
        }
        else if (arg_help || arg_license)
        {
            /*
             * display help or license text
             */
            fprintf_prginfo(stderr);
            if (arg_help)
                fprintf_arghelp(stderr, hscpitt_args);
            else
                show_license();
            set_return_code(RC_WARN);
            ok = FALSE;
        }
        else
        {
            /* display copyright in verbose-mode */
            if (!quiet)
                fprintf_prginfo(stderr);

            /* set default-parameters if neccessary */
            if (!prjfile)
            {
                prjfile = DEFAULT_PROJECT;
                if (!quiet)
                {
                    fprintf(stderr, HP "%s: using default project-file\n",
                            prjfile);
                }
            }

            if (command)
            {
                /* debugging control output */
                D(
                     {
                     fprintf(stderr, DHP "prjfile =`%s'\n", prjfile);
                     fprintf(stderr, DHP "command =`%ld'\n", command);
                     }
                );
            }
            else
            {
                fprintf(stderr, "no command specified\n");
                ok = FALSE;
            }
        }

        /* set error return code if neccessary */
        if (!ok)
        {
            set_return_code(RC_ERROR);
        }

        /* release mem used by args */
        free_args(hscpitt_args);
    }
    else
    {
        /* only for developer */
        D(fprintf(stderr, SHIT "ArgDef error: %lu\n", prep_error_num));
    }

#if 1
    return (ok);
#else
    return (FALSE);             /* for arg-debugging */
#endif
}

/*
 * read_project
 *
 * read data from project file
 */
static BOOL read_project(VOID)
{
    BOOL ok = FALSE;
    INFILE *inpf = NULL;

    if (!quiet)
    {
        fprintf(stderr, HP "%s: reading..\r", prjfile);
        fflush(stderr);
    }

    /* assign message-callback for corrupt project-file */
    project->CB_msg_corrupt_pf = msg_corrupt_pf;

    /* read project-file */
    errno = 0;
    inpf = infopen(prjfile, 1024);
    if (inpf)
    {
        if (hsc_project_read_data(project, inpf))
        {
            if (!quiet)
            {
                fprintf(stderr, HP "%s: project-file read\n", prjfile);
            }
            ok = TRUE;
        }
        infclose(inpf);
    }

    if (!ok)
    {
        fprintf(stderr, HP "error reading `%s'", prjfile);
        if (errno)
            fprintf(stderr, HP ": %s", strerror(errno));
        fprintf(stderr, "\n");
    }

    /* set error return code if neccessary */
    if (!ok)
    {
        set_return_code(RC_ERROR);
    }

    return (ok);
}

/*-------------------------------------------------------------------
 *
 * functions for commands arguments
 *
 *-------------------------------------------------------------------*/

/* check for no args */
static BOOL chkArg0(STRPTR command)
{
    BOOL ok = TRUE;
    if (cmdArgNum)
    {
        fprintf(stderr, "no arguments allowed for command `%s'\n", command);
        set_return_code(RC_ERROR);
        ok = FALSE;
    }
    return (ok);
}

/* check for two args */
static BOOL chkArg2(STRPTR command, STRPTR arg1, STRPTR arg2)
{
    BOOL ok = TRUE;
    if (cmdArgNum != 2)
    {
        fprintf(stderr, "two arguments required for command `%s': "
                "%s and %s\n", command, arg1, arg2);
        set_return_code(RC_ERROR);
        ok = FALSE;
    }
    return (ok);
}

/* check for at least one arg (or more) */
static BOOL chkArgAny(STRPTR command)
{
    BOOL ok = TRUE;
    if (!cmdArgNum)
    {
        fprintf(stderr, "arguments required for command `%s'\n", command);
        set_return_code(RC_ERROR);
        ok = FALSE;
    }
    return (ok);
}

/*-------------------------------------------------------------------
 *
 * functions for commands
 *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/

/*
 * command_new
 *
 * write an empty project file
 */
BOOL command_new(HSCPRJ * project, DLLIST * arglist)
{
    BOOL ok = TRUE;

    if (chkArg0(CMD_NEW_STR))
    {
        if (fexists(prjfile) && !force)
        {
            printf("project file `%s' already exists\n", prjfile);
            set_return_code(RC_ERROR);
            ok = FALSE;
        }
        else
        {
            /* write new project file */
            if (update_project_file(project))
            {
                if (!quiet)
                {
                    printf("Created new project file `%s'\n", prjfile);
                }
            }
        }
    }

    return (ok);
}

/*-------------------------------------------------------------------*/

/*
 * command_add
 *
 * add new document and source to project
 */
VOID command_add(HSCPRJ * project, DLLIST * arglist)
{
    if (chkArg2(CMD_ADD_STR, "DOCUMENT", "SOURCE"))
    {
        BOOL deleted = FALSE;
        DLNODE *docNameNd = dll_first(arglist);
        DLNODE *docSourceNd = dln_next(docNameNd);
        STRPTR docName = (STRPTR) dln_data(docNameNd);
        STRPTR docSource = (STRPTR) dln_data(docSourceNd);

        /* try to remove document */
        deleted = hsc_project_del_document(project, docName);

        if (!deleted || force)
        {
            D(fprintf(stderr, DHP " add doc=`%s'\n    src=`%s'\n",
                      docName, docSource));

            /* set document as current document */
            hsc_project_set_document(project, docName);
            hsc_project_set_source(project, docSource);
            hsc_project_add_document(project);

            /* update project file */
            if (update_project_file(project))
            {
                if (!quiet)
                {
                    STRPTR operation = "Added";
                    if (deleted)
                    {
                         operation = "Replaced";
                    }
                    printf("%s document `%s'\n", operation, docName);
                }
            }
        }
        else
        {
            printf("Document `%s' already in project\n", docName);
            set_return_code(RC_WARN);
        }
    }
}

/*-------------------------------------------------------------------*/

/*
 * command_delete
 *
 * remove document(s) from project
 */
VOID command_delete(HSCPRJ * project, DLLIST * arglist)
{
    if (chkArgAny(CMD_DELETE_STR))
    {
        DLNODE *docNode = dll_first(arglist);

        while (docNode)
        {
            STRPTR docName = (STRPTR) dln_data(docNode);
            BOOL deleted = FALSE;

            /* remove document */
            deleted = hsc_project_del_document(project, docName);

            /* output message */
            if (deleted)
            {
                if (!quiet)
                {
                    printf("Delete `%s'\n", docName);
                }
            }
            else
            {
                printf("Delete `%s': no such document\n", docName);
                set_return_code(RC_WARN);
            }
            docNode = dln_next(docNode);
        }

        /* update project file */
        update_project_file(project);
    }
}

/*-------------------------------------------------------------------*/

/*
 * command_erase
 *
 * remove document(s) and corresponding files from project
 */
static BOOL uremove(STRPTR fname)
{
    BOOL erased = FALSE;

    /* TODO: convert URI to filename */
    errno = 0;
    erased = !remove(fname);
    if (erased)
    {
        if (!quiet)
        {
            printf("Delete `%s'\n", fname);
        }
    }
    else
    {
        printf("Error deleting `%s': %s\n", fname, strerror(errno));
    }

    return (erased);
}

VOID command_erase(HSCPRJ * project, DLLIST * arglist)
{
    if (chkArgAny(CMD_ERASE_STR))
    {
        DLNODE *argNode = dll_first(arglist);
        BOOL erasedAny = FALSE;

        while (argNode)
        {
            STRPTR docName = (STRPTR) dln_data(argNode);
            HSCDOC *document = find_document(project->documents, docName);
            BOOL erased = FALSE;

            /* remove document and source file */
            if (document)
            {
                uremove(document->docname);
                uremove(document->sourcename);
                erased = hsc_project_del_document(project, docName);
                if (!erased)
                    panic("no document entry");
            }

            /* output message */
            if (!erased)
            {
                printf("Delete `%s': no such document\n", docName);
                set_return_code(RC_WARN);
            }
            else
            {
                /* project file needs to be updated */
                erasedAny = TRUE;
            }

            argNode = dln_next(argNode);
        }

        /* update project file */
        if (erasedAny)
        {
            update_project_file(project);
        }
    }
}

/*-------------------------------------------------------------------*/

/*
 * command_extract
 *
 * display detailed information about documents found in project
 */
static VOID printfVar(STRPTR var, STRPTR value)
{
    if (!value)
        value = "";
    printf("%s=\"%s\"\n", var, value);
}

static VOID extract_document(HSCPRJ * project, HSCDOC * document, BOOL emptyLine)
{
    /* show empty line; not for first document */
    if (emptyLine)
    {
        puts("");
    }

    /* display document name and source used */
    printfVar("DOCUMENT", document->docname);
    printfVar("SOURCE", document->sourcename);

#if 1
    /* display includes */
    if (document->includes)
    {
        DLNODE *nd = dll_first(document->includes);
        while (nd)
        {
            HSCINC *include = (HSCINC *) dln_data(nd);
            printfVar("INCLUDE", include->name);
            nd = dln_next(nd);
        }
    }
#endif
}

VOID command_extract(HSCPRJ * project, DLLIST * arglist)
{
    if (cmdArgNum)
    {
        /* extract document(s) specified in args */
        DLNODE *argNode = dll_first(arglist);
        BOOL emptyLine = FALSE;

        while (argNode)
        {
            STRPTR docName = (STRPTR) dln_data(argNode);
            HSCDOC *document = find_document(project->documents, docName);

            /* remove document and source file */
            if (document)
            {
                extract_document(project, document, emptyLine);
                emptyLine = TRUE;
            }
            else
            {
                fprintf(stderr, "Extract `%s': no such document\n", docName);
                set_return_code(RC_WARN);
            }

            argNode = dln_next(argNode);
        }
    }
    else
    {
        /* extract all documents */
        DLNODE *docNode = dll_first(project->documents);
        BOOL emptyLine = FALSE;

        while (docNode)
        {
            HSCDOC *document = (HSCDOC *) dln_data(docNode);

            extract_document(project, document, emptyLine);
            emptyLine = TRUE;

            /* process next node, show empty line if
             * further data exist */
            docNode = dln_next(docNode);
        }
    }
}

/*-------------------------------------------------------------------*/

/*
 * command_count
 *
 * display number of documents currently stored in project file
 */
VOID command_count(HSCPRJ * project, DLLIST * arglist)
{
    DLNODE *docNode = dll_first(project->documents);
    ULONG docNum = 0;

    if (chkArg0(CMD_COUNT_STR))
    {
        while (docNode)
        {
            docNum++;
            docNode = dln_next(docNode);
        }

        /* display number of documents */
        printf("%lu\n", docNum);
    }
}

/*-------------------------------------------------------------------*/

/*
 * command_list
 *
 * display list of documents found in project
 */
VOID command_list(HSCPRJ * project, DLLIST * arglist)
{
    if (chkArg0(CMD_COUNT_STR))
    {
        DLNODE *docNode = dll_first(project->documents);

        while (docNode)
        {
            HSCDOC *document = (HSCDOC *) dln_data(docNode);

            printf("%s\n", document->docname);
            docNode = dln_next(docNode);
        }
    }
}

/*-------------------------------------------------------------------*/

/*
 * process_command
 *
 */
BOOL process_command(HSCPRJ * project, LONG command, DLLIST * arglist)
{
    BOOL ok = TRUE;
    DLNODE *nd = NULL;

    D(fprintf(stderr, DHP "process_command()\n"));
    D(fprintf(stderr, DHP "command: %s\n", command_name[command]));

    /* count number of args */
    if (arglist)
        nd = dll_first(arglist);
    if (nd)
    {
        D(fprintf(stderr, DHP "command args:\n"));
        while (nd)
        {
            D(fprintf(stderr, DHP "  `%s'\n", (STRPTR) dln_data(nd)));
            cmdArgNum++;
            nd = dln_next(nd);
        }
    }
    else
    {
        D(fprintf(stderr, DHP "command args: NONE\n"));
    }
    D(fprintf(stderr, DHP "cmdarg#: %ld\n", cmdArgNum));

    switch (command)
    {
    case COMMAND_COUNT:
        command_count(project, arglist);
        break;
    case COMMAND_LIST:
        command_list(project, arglist);
        break;
    case COMMAND_EXTRACT:
        command_extract(project, arglist);
        break;
    case COMMAND_DELETE:
        command_delete(project, arglist);
        break;
    case COMMAND_ERASE:
        command_erase(project, arglist);
        break;
    case COMMAND_ADD:
        command_add(project, arglist);
        break;
    case COMMAND_NEW:
        ok = command_new(project, arglist);
        break;
    default:
        {
            D(fprintf(stderr, DHP "unknown command\n"));
            ok = FALSE;
            break;
        }
    }

    return (ok);
}

/*-------------------------------------------------------------------
 *
 * main function
 *
 *-------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
#ifndef BETA
#define BETA 0
#endif
    /* set program information */
    set_prginfo("hscpitt", UGLY_AUTHOR, VERSION, REVISION, BETA,
                "hsc project interfering'n'trashing tool",
                "Freeware, type `hscpitt LICENSE' for details.");

#if DEBUG
    /* display a memory tracking report */
    /* at end of execution */
    atexit(atexit_uglymemory);
#endif

    /* install nomem-handler */
    ugly_nomem_handler = hscpitt_nomem_handler;

    /* use cleanup() as additional exit func */
    if (!atexit(cleanup))
    {
        /*
         * main procedure
         */
        return_code = RC_OK;
        project = new_project();
        if (project && args_ok(argc, argv))
        {
            BOOL ok = TRUE;

            if (command != COMMAND_NEW)
            {
                ok = read_project();
            }

            if (ok && process_command(project, command, command_arglist))
            {
                return_code = RC_OK;
            }
        }
    }
    else
    {
        fputs(SHIT "atexit() failed ", stderr);
    }
    return (return_code);
}

