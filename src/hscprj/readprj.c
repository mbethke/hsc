/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * hscprj/readprj.c
 *
 * project managment input-routines for hsc
 *
 * updated: 17-Dec-1997
 * created: 10-Sep-1996
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "hsclib/ldebug.h"
#include "hscprj/pdebug.h"
#include "hscprj/pdefs.h"

#include "ugly/utypes.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/umemory.h"
#include "ugly/infile.h"
#include "ugly/ustring.h"

#include "hscprj/document.h"
#include "hscprj/project.h"

/*
 * hsc_msg_project_corrupt: display error message about
 * corrupt project file
 */
static VOID hsc_msg_project_corrupt(HSCPRJ * hp, STRPTR descr)
{
    hp->fatal = TRUE;
    if (hp->CB_msg_corrupt_pf)
        (*(hp->CB_msg_corrupt_pf)) (hp, descr);
}

/* convert hex-digit to int */
static int x2int(char c)
{
    if ((c >= '0') && (c <= '9'))
        return (c - '0');
    if ((c >= 'A') && (c <= 'F'))
        return (c - 'A' + 10);
    if ((c >= 'a') && (c <= 'f'))
        return (c - 'a' + 10);
    return (EOF);
}

/*
 * read_long: read (unsigned) long integer value
 */
static ULONG read_ulong(HSCPRJ * hp)
{
    INFILE *inpf = hp->inpf;
    ULONG num = 0;
    int ch;
    int digit = EOF;

    do
    {
        ch = infgetc(inpf);
        if (ch != ' ')
        {
            digit = x2int(ch);
            if (digit == EOF)
                num = 0;
            else
                num = (num << 4) + digit;
        }
        if (digit == EOF)
            hsc_msg_project_corrupt(hp, "hex digit expected");
    }
    while ((digit != EOF) && (ch != ' '));

    if (digit == EOF)
        num = 0;

    return (num);
}

/*
 * read_string:
 *
 * read string length, alloc mem and read string into it
 */
static STRPTR read_string(HSCPRJ * hp)
{
    STRPTR dest = NULL;

    ULONG len = read_ulong(hp);

    if (len)
    {
        ULONG i;
        int ch = 'x';           /* dummy init */

        /* alloc mem */
        dest = umalloc((size_t) (len + 1));
        dest[len] = '\0';

        for (i = 0; ((i < len) && (ch != EOF)); i++)
        {
            ch = infgetc(hp->inpf);
            if (ch != EOF)
                dest[i] = ch;
            else
                hsc_msg_project_corrupt(hp, "string expected");
        }
        if (ch != EOF)
            dest[len] = 0;
        else
        {
            ufree(dest);
            dest = NULL;
        }
    }
    return (dest);
}

/*
 * read_caller: read file position
 */
static CALLER *read_caller(HSCPRJ * hp)
{
    CALLER *caller = NULL;
    STRPTR callerid = infgetw(hp->inpf);

    if (callerid && !upstrcmp(callerid, ID_CALLER_STR))
    {
        int ch = infgetc(hp->inpf);     /* skip blank */

        if (ch == ' ')
        {
            STRPTR fname = read_string(hp);
            caller = new_caller(fname, read_ulong(hp), read_ulong(hp));
            ufreestr(fname);
        }
        else
        {
            hsc_msg_project_corrupt(hp, "blank expected");
        }
    }
    else if (callerid && !strcmp(callerid, "\n"))
    {
        /* skip empty caller */
        inungetcw(hp->inpf);
        D(fprintf(stderr, DHP "skip EMPTY CALLER\n"));
    }
    else
    {
        hsc_msg_project_corrupt(hp, ID_CALLER_STR " expected");
    }
    return (caller);
}

/*
 * read_lf: read linefeed
 */
static BOOL read_lf(HSCPRJ * hp)
{
    int ch = infgetc(hp->inpf);
    BOOL ok = TRUE;

    if (ch != '\n')
    {
        hsc_msg_project_corrupt(hp, "linefeed expected");
        ok = FALSE;
    }
    return (ok);
}

/*
 * read command
 *
 * returns next command or NULL if eof reached
 */
static STRPTR read_command(HSCPRJ * hp)
{
    STRPTR command;

    do
    {
        command = infgetw(hp->inpf);
    }
    while (command && !strcmp(command, "\n"));

    if (command)
    {
        /* skip blanks */
        infskip_ws(hp->inpf);
        DP(fprintf(stderr, DHP "command `%s'\n", command));
    }
    else
    {
        DP(fprintf(stderr, DHP "command EOF\n"));
    }

    return (command);
}

/*
 * read header
 */
static BOOL read_header(HSCPRJ * hp)
{
    STRARR fileid[1 + sizeof(FILEID_HSCPRJ)];
    BOOL ok = FALSE;
    STRPTR cmd = NULL;
    size_t i;

    /* read fileid */
    for (i = 0; i < strlen(FILEID_HSCPRJ); i++)
    {
        int ch = infgetc(hp->inpf);

        fileid[i] = (UBYTE) ch;
    }
    fileid[i] = '\0';

    DP(fprintf(stderr, DHP "fileid: `%s'\n", fileid));

    /* check fileid */
    if (!strcmp(fileid, FILEID_HSCPRJ))
    {
        DP(fprintf(stderr, DHP "fileid: `%s'\n", fileid));
        ok = read_lf(hp);
    }
    else
    {
        hsc_msg_project_corrupt(hp, "wrong file-id");
    }

    if (ok)
    {

        ok = FALSE;

        /* read version */
        cmd = read_command(hp);

        /* check version */
        if (cmd && !strcmp(cmd, LINE_VERSION_STR))
        {
            ULONG version = read_ulong(hp);

            DP(fprintf(stderr, DHP "version: %lu\n", version));

            if (version && (version <= 2))
            {
                ok = read_lf(hp);
            }
            else
            {
                hsc_msg_project_corrupt(hp, "wrong version");
            }

        }
        else
        {
            hsc_msg_project_corrupt(hp, "unknown version");
        }

    }
    return (ok);
}

/*
 * hsc_project_read_data
 *
 * read project file
 *
 * params: hp....HSCPRJ created using new_project()
 *         inpf..input file opened using infopen();
 *               this has to be opened and closed by
 *               you outside this function.
 */
BOOL hsc_project_read_data(HSCPRJ * hp, INFILE * inpf)
{
    BOOL ok = FALSE;

    if (inpf)
    {
        hp->inpf = inpf;

        DP(fprintf(stderr, DHP "read project-file from `%s'\n",
                     infget_fname(inpf)));

        if (read_header(hp))
        {
            HSCDOC *document = NULL;
            STRPTR cmd = NULL;

            do
            {
                cmd = read_command(hp);
                if (cmd)
                {
                    if (!strcmp(cmd, LINE_REM_STR))
                    {
                        /* skip comment */
                        int ch;
                        DP(fprintf(stderr, DHP "comment `"));
                        do
                        {
                            ch = infgetc(inpf);
                            DP(
                                    {
                                    if (ch != '\n')
                                    fprintf(stderr, "%c", ch);
                                    }
                            );
                        }
                        while ((ch != EOF) && (ch != '\n'));
                        DP(fprintf(stderr, "'\n"));
                    }
                    else if (!strcmp(cmd, LINE_DOCUMENT_STR))
                    {
                        /* begin new DOCUMENT */
                        STRPTR docname = read_string(hp);
                        if (docname)
                        {
                            document = new_document(docname);
                            app_dlnode(hp->documents, (APTR) document);
                            /* free mem allocated by read_string() */
                            ufree(docname);
                        }
                    }
                    else if (!strcmp(cmd, LINE_SOURCE_STR))
                    {
                        /* assign SOURCE */
                        if (document)
                        {
                            STRPTR sourcename = read_string(hp);
                            if (sourcename)
                            {
                                reallocstr(&(document->sourcename),
                                           sourcename);
                                /* free mem allocated by read_string() */
                                ufree(sourcename);
                            }
                        }
                        else
                            hsc_msg_project_corrupt
                                (hp, LINE_SOURCE_STR " without "
                                 LINE_DOCUMENT_STR);
                    }
                    else if (!strcmp(cmd, LINE_TITLE_STR))
                    {
                        /* assign TITLE */
                        if (document)
                        {
                            STRPTR titlename = read_string(hp);
                            if (titlename)
                            {
                                set_estr(document->title, titlename);
                                /* free mem allocated by read_string() */
                                ufree(titlename);
                            }
                        }
                        else
                            hsc_msg_project_corrupt
                                (hp, LINE_TITLE_STR " without "
                                 LINE_DOCUMENT_STR);
                    }
                    else if (!strcmp(cmd, LINE_ID_STR))
                    {
                        /* append new ID */
                        if (document)
                        {
                            STRPTR idname = read_string(hp);
                            if (idname)
                            {
                                HSCIDD *iddef =
                                app_iddef(document, idname);
                                iddef->caller = read_caller(hp);
                                /* free mem allocated by read_string() */
                                ufree(idname);
                            }
                        }
                        else
                            hsc_msg_project_corrupt
                                (hp, LINE_ID_STR " without "
                                 LINE_DOCUMENT_STR);

                    }
                    else if (!strcmp(cmd, LINE_INCLUDE_STR))
                    {
                        /* store new INCLUDE */
                        if (document)
                        {
                            STRPTR incname = read_string(hp);
                            if (incname)
                            {
                                HSCINC *inc =
                                app_include(document, incname);
                                inc->caller = read_caller(hp);
                                /* free mem allocated by read_string() */
                                ufree(incname);
                            }
                        }
                        else
                            hsc_msg_project_corrupt
                                (hp, LINE_INCLUDE_STR " without "
                                 LINE_DOCUMENT_STR);
                    }
                    else
                    {
                        /* unknown command */
                        hsc_msg_project_corrupt(hp, "unknown tag");
                    }
                }
                else
                {
                    DP(fprintf(stderr, DHP "EOF\n"));
                }
            }
            while (cmd && !hp->fatal);

            ok = !hp->fatal;
        }
        hp->inpf = NULL;
    }

    return (ok);
}

