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
 * hscprj/project.c
 *
 * project managment routines for hsc
 *
 * updated: 12-Jan-1997
 * created: 13-Apr-1996
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "hsclib/ldebug.h"
#include "hscprj/pdebug.h"

#include "ugly/utypes.h"
#include "ugly/dllist.h"
#include "ugly/expstr.h"
#include "ugly/umemory.h"
#include "ugly/infile.h"
#include "ugly/ustring.h"

#include "hscprj/document.h"
#include "hscprj/project.h"

/*
 * new_project
 */
HSCPRJ *new_project(VOID)
{
    HSCPRJ *hp = (HSCPRJ *) umalloc(sizeof(HSCPRJ));

    if (hp)
    {
        memset(hp, 0, sizeof(HSCPRJ));
        hp->documents = init_dllist(del_document);
    }

    return (hp);
}


/*
 * del_project
 */
VOID del_project(HSCPRJ * hp)
{
    if (hp)
    {
        del_dllist(hp->documents);
        del_document(hp->document);
        ufree(hp);
    }
}


/*
 * check_document_id
 *
 * append id to id_ref-list so it as checked when
 * check_all_local_id_ref() is called
 */
int check_document_id(HSCPRJ * hp, STRPTR docname, STRPTR id)
{
    int err = ERR_CDI_OK;
    HSCDOC *document = find_document(hp->documents, docname);

    DP(fprintf(stderr, DHP "  check id: `%s#%s'\n", docname, id));

    if (document)
    {
        HSCIDD *iddef = NULL;

        iddef = find_iddef(document, id);
        if (!iddef)
        {
            DP(fprintf(stderr, DHP "    id unknown\n"));
            err = ERR_CDI_NoID;
        }
        else
        {
            DP(fprintf(stderr, DHP "    id ok\n"));
            err = ERR_CDI_OK;
        }
    }
    else
    {
        DP(fprintf(stderr, DHP "    no file-entry in project\n"));
        err = ERR_CDI_NoDocumentEntry;
    }

    return (err);
}

/*
 * hsc_project_set_filename
 *
 * set project-filename (used by hsc_project_write_file())
 */
BOOL hsc_project_set_filename(HSCPRJ * hp, STRPTR new_prjname)
{
    BOOL ok = TRUE;
    DP(fprintf(stderr, DHP "set project `%s'\n", new_prjname));

    if (hp->document->sourcename)
    {
        panic("project-name already set");
    }

    hp->document->sourcename = strclone(new_prjname);

    return (ok);
}

/*
 * hsc_project_add_document
 *
 * add current document to project
 */
BOOL hsc_project_add_document(HSCPRJ * hp)
{
    BOOL ok = TRUE;

    if (hp->document)
    {
        if (hp->document->docname)
        {
            DP(fprintf(stderr, DHP "  add document `%s'\n",
                       hp->document->docname));
            app_dlnode(hp->documents, hp->document);
        }
        else
        {
            DP(fprintf(stderr, DHP "  add document <stdout>: skipped\n"));
            del_document(hp->document);
        }

        hp->document = NULL;
    }
    else
    {
        panic("no document to replace");
    }

    return (ok);
}

/*
 * hsc_project_del_document
 *
 * remove document from project
 */
BOOL hsc_project_del_document(HSCPRJ * hp, STRPTR docname)
{
    BOOL deleted = FALSE;
    DLNODE *docnd = find_document_node(hp->documents, docname);

    if (docnd)
    {
        DP(fprintf(stderr, DHP "  remove document `%s'\n", docname));
        del_dlnode(hp->documents, docnd);
        deleted = TRUE;
    }
    else
    {
        DP(fprintf(stderr, DHP "  remove document `%s': FAILED\n", docname));
    }

    return (deleted);
}

/*
 * hsc_project_set_document
 *
 * Remove document from project's document list and
 * create a new empty document with this name and
 * make it the current document of the project.
 * The new document is added to the project when the
 * project-file is updated.
 */
BOOL hsc_project_set_document(HSCPRJ * hp, STRPTR new_docname)
{
    BOOL ok = TRUE;
    DLNODE *nd = NULL;
    if (new_docname)
    {
        DP(fprintf(stderr, DHP "set document `%s'\n", new_docname));

        /* dettach document from project */
        nd = find_document_node(hp->documents, new_docname);
        if (nd)
        {
            DP(fprintf(stderr, DHP "  remove old document `%s'\n",
                       ((HSCDOC *) dln_data(nd))->docname));
            del_dlnode(hp->documents, nd);
        }
        else
        {
            DP(fprintf(stderr, DHP "  new document `%s'\n", new_docname));
        }
    }
    else
    {
        DP(fprintf(stderr, DHP "set document <stdout>\n"));
    }

    /* remove current document */
    if (hp->document)
    {
        DP(fprintf(stderr, DHP "  remove current document `%s'\n",
                   hp->document->docname));
        del_document(hp->document);
    }

    hp->document = new_document(new_docname);

    return (ok);
}

/*
 * hsc_project_set_source
 *
 * set main source for project's current document
 */
BOOL hsc_project_set_source(HSCPRJ * hp, STRPTR new_sourcename)
{
    BOOL ok = TRUE;
    DP(fprintf(stderr, DHP "set source `%s'\n", new_sourcename));

    if (!hp->document->sourcename)
        hp->document->sourcename = strclone(new_sourcename);
    else
    {
        panic("sourcename already set");
        ok = FALSE;
    }

    return (ok);
}

/*
 * hsc_project_add_include
 *
 * add include-file to project's current document
 */
BOOL hsc_project_add_include(HSCPRJ * hp, STRPTR new_includename)
{
    BOOL ok = TRUE;
    DP(fprintf(stderr, DHP "add include `%s'\n", new_includename));
    app_include(hp->document, new_includename);
    return (ok);
}
