/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001 Matthias Bethke
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
 * hscprj/writeprj.c
 *
 * project-managment output-routines for hsc
 *
 * updated: 28-Mar-1997
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

#define TIMEBUFSIZE 40
static STRARR timebuf[TIMEBUFSIZE];

/*
 *-------------------------------------
 * write project file
 *-------------------------------------
 */

/*
 * hsc_project_write_data
 *
 * write ids to file
 */
static VOID append_ulong(EXPSTR * dest, ULONG num)
{
    STRARR lenbuf[20];

    sprintf(lenbuf, "%lx ", num);
    app_estr(dest, lenbuf);
}

static VOID append_string(EXPSTR * dest, STRPTR s)
{
/*    append_ulong(dest, (ULONG) strlen(s)); */
    app_estr(dest, s);
}

static VOID append_caller(EXPSTR * dest, CALLER * caller)
{
#if 0
    if (caller)
    {
        app_estr(dest, " " ID_CALLER_STR " ");
        append_string(dest, caller->name);
        append_ulong(dest, caller->posx);
        append_ulong(dest, caller->posy);
    }
#endif
}

static VOID append_docname(EXPSTR * prjstr, STRPTR docname)
{
    app_estr(prjstr, LINE_DOCUMENT_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, docname);
    app_estr(prjstr, "\n");
}

static VOID append_sourcename(EXPSTR * prjstr, STRPTR sourcename)
{
    if (sourcename)
    {
        app_estr(prjstr, LINE_SOURCE_STR);
        app_estr(prjstr, " ");
        append_string(prjstr, sourcename);
        app_estr(prjstr, "\n");
    }
}

static VOID append_title(EXPSTR * prjstr, STRPTR title)
{
    if (title)
    {
        app_estr(prjstr, LINE_TITLE_STR);
        app_estr(prjstr, " ");
        append_string(prjstr, title);
        app_estr(prjstr, "\n");
    }
}

#if 0
static VOID append_id(EXPSTR * prjstr, STRPTR id)
{
    app_estr(prjstr, LINE_ID_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, id);
    app_estr(prjstr, "\n");
}
#endif

static VOID append_include(EXPSTR * prjstr, HSCINC * include)
{
    app_estr(prjstr, LINE_INCLUDE_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, include->name);
    append_caller(prjstr, include->caller);
    app_estr(prjstr, "\n");
#if 0
    DP(fprintf(stderr, DHP "  include `%s'\n", include->name));
#endif
}

#if 0
static VOID append_reference(EXPSTR * prjstr, HSCREF * reference)
{
#if 1
    app_estr(prjstr, LINE_REFERENCE_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, reference->name);
    append_caller(prjstr, reference->caller);
    app_estr(prjstr, "\n");
#else
    DP(fprintf(stderr, DHP "  refers `%s'\n", reference->name));
#endif
}
#endif

static VOID append_iddef(EXPSTR * prjstr, HSCIDD * iddef)
{
    app_estr(prjstr, LINE_ID_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, iddef->name);
    append_caller(prjstr, iddef->caller);
    app_estr(prjstr, "\n");
}

/*
 * append groups of data
 */
static VOID append_header(EXPSTR * prjstr)
{
    time_t now = time(NULL);    /* get current time */

    /* create string for current time */
    strftime(timebuf, TIMEBUFSIZE,
             "%d-%b-%Y %H:%M:%S", localtime(&now));

    /* append key-sequence, file-format-version and comment */
    app_estr(prjstr,
             FILEID_HSCPRJ "\n" LINE_VERSION_STR " ");
    append_ulong(prjstr, VERSION_HSCPRJ);
    app_estr(prjstr, "\n");
    app_estr(prjstr,
             LINE_REM_STR " Contains all data relevant for project.\n"
             LINE_REM_STR " Maintained by hsc, DO NOT MODIFY!\n");
    app_estr(prjstr,
             LINE_REM_STR " updated: ");
    app_estr(prjstr, timebuf);
    app_estrch(prjstr, '\n');
}

/* append included files */
static VOID append_doc_includes(EXPSTR * prjstr, DLLIST * inclist)
{
    DLNODE *nd = dll_first(inclist);
    while (nd)
    {
        append_include(prjstr, (HSCINC *) dln_data(nd));
        nd = dln_next(nd);
    }
}

/* append other documents referered */
#if 0
static VOID append_doc_references(EXPSTR * prjstr, DLLIST * reflist)
{
    DLNODE *nd = dll_first(reflist);
    while (nd)
    {
        append_reference(prjstr, (HSCREF *) dln_data(nd));
        nd = dln_next(nd);
    }
}
#endif

/* append ids defined within documents */
static VOID append_doc_iddefs(EXPSTR * prjstr, DLLIST * iddefs)
{
    DLNODE *nd = dll_first(iddefs);
    while (nd)
    {
        append_iddef(prjstr, (HSCIDD *) dln_data(nd));
        nd = dln_next(nd);
    }
}

/* append all document-related data */
static VOID append_document(EXPSTR * prjstr, HSCDOC * document)
{
    STRPTR docname = document->docname;

    append_docname(prjstr, docname);
    append_sourcename(prjstr, document->sourcename);
    append_title(prjstr, estr2str(document->title));

    append_doc_iddefs(prjstr, document->iddefs);
    append_doc_includes(prjstr, document->includes);
#if 0
    append_doc_references(prjstr, document->references);
#endif
}

/****** hscprj.lib/hsc_project_write_data ************************************
*
*   NAME
*       hsc_project_write_data -- store project data in a file
*
*   SYNOPSIS
*       ok = hsc_project_write_data( project, filename, force )
*
*       BOOL hsc_project_write_data
*            ( HSCPRJ * project, STRPTR filename, BOOL force )
*
*   FUNCTION
*       Store all project data in a file. If the file already exists, it
*       will be overwritten without asking back.
*
*   INPUTS
*       project  - project data
*       filename - filename of project file
*       force    - write data even if no new documents have been atteched;
*                  enable this, if e.g. you only have removed some documents
*
*   RESULT
*       ok - TRUE, if file could have been written successfully
*
*   EXAMPLE
*       ok = hsc_project_write_data( project, "sepp.project", FALSE );
*
*   SEE ALSO
*
*
******************************************************************************
*/
BOOL hsc_project_write_data(HSCPRJ * hp, STRPTR project_fname, BOOL force)
{
    BOOL written = FALSE;

    if (hp && !hp->fatal && project_fname)
    {
        EXPSTR *prjstr = init_estr(256);
        DLNODE *nd = NULL;
        FILE *outfile = NULL;
        BOOL write_it = force; /* really write it? */

        DP(fprintf(stderr, DHP "update project file `%s'\n",
                   project_fname));

        /* append header information */
        append_header(prjstr);

        if ((hp->document) && (hp->document->docname))
        {
            /* append current document to project */
            hsc_project_add_document(hp);

            /* now it needs an update.. */
            write_it = TRUE;
        }

        if (write_it)
        {
            /*
             * append all old project info of other files
             */
            nd = dll_first(hp->documents);
            while (nd)
            {
                HSCDOC *document = (HSCDOC *) nd->data;

                append_document(prjstr, document);

                nd = dln_next(nd);
            }

            DP(fprintf(stderr, DHP "project file contains:\n%s", estr2str(prjstr)));

            /* write new project file */
            errno = 0;
            outfile = fopen(project_fname, "w");
            if (outfile)
            {
                errno = 0;
                fwrite(estr2str(prjstr), sizeof(char),
                       estrlen(prjstr), outfile);

                if (errno)
                {
                    DP(fprintf(stderr, DHP "can't write project file\n"));
                    /* TODO: show message "can't write project file" */
                }
                else
                    written = TRUE;
            }
            else
            {
                DP(fprintf(stderr, DHP "can't open project file for output\n"));
                /* TODO: show message "can't open project file" */
            }
        }
        else
        {
            DP(fprintf(stderr, DHP "  no document or docname to add\n"));
            del_document(hp->document);
            hp->document = NULL;
        }

        del_estr(prjstr);
    }
    else
    {
        D(fprintf(stderr, DHP "no update project\n"));
    }

    return (written);
}
