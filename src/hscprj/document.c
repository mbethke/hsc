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
 * hsclib/document.c
 *
 * document routines for hsc
 *
 * updated: 20-Feb-1997
 * created:  7-Jul-1996
 */

#include <stdio.h>
#include <string.h>

#include "hscprj/document.h"

/* debuggin define */
#ifdef D
#undef D
#endif
#if defined DEBUG
#if DEBUG
#define D(x) x
#else
#define D(x)                    /* nufin */
#endif
#else
#define D(x)                    /* nufin */
#endif

/*
 * del/new_document
 */

/* del_document: remove document */
VOID del_document(APTR data)
{
    if (data)
    {
        HSCDOC *document = (HSCDOC *) data;

        ufreestr(document->docname);
        ufreestr(document->sourcename);
        del_estr(document->title);
        del_dllist(document->iddefs);
        del_dllist(document->includes);
        del_dllist(document->references);
        ufree(document);
    }
}

/* new_document: create new document */
HSCDOC *new_document(STRPTR docname)
{
    HSCDOC *newdoc = (HSCDOC *) umalloc(sizeof(HSCDOC));

#if 0
    DP(fprintf(stderr, DHP "new document `%s'\n", docname));
#endif
    newdoc->docname = strclone(docname);
    newdoc->sourcename = NULL;
    newdoc->title = init_estr(0);
    newdoc->iddefs = init_dllist(del_iddef);
    newdoc->includes = init_dllist(del_include);
    newdoc->references = init_dllist(del_reference);

    return (newdoc);
}

/*
 * cmp_document: compare document
 */
int cmp_document(APTR cmp_data, APTR list_data)
{
    HSCDOC *document = (HSCDOC *) list_data;
    STRPTR fname1 = NULL;
    STRPTR fname2 = (STRPTR) cmp_data;

#if DEBUG
    if (!document)
    {
        panic("documant = NULL");
    }
    else if (!document->docname)
    {
        panic("fname1 = NULL");
    }
    if (!fname2)
    {
        panic("fname2 = NULL");
    }
#endif
    fname1 = document->docname;

    if (!strcmp(fname1, fname2))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

/* find_document_node: scans document list for a specific document */
DLNODE *find_document_node(DLLIST * list, STRPTR name)
{
    return (find_dlnode(dll_first(list), (APTR) name, cmp_document));
}

/* find_document: scans document list for a specific document */
HSCDOC *find_document(DLLIST * list, STRPTR name)
{
    DLNODE *nd = find_document_node(list, name);
    HSCDOC *document = NULL;

    if (nd)
        document = (HSCDOC *) dln_data(nd);

    return (document);
}

/*
 * functions for caller
 */
CALLER *new_caller(STRPTR name, ULONG posx, ULONG posy)
{
    CALLER *caller = (CALLER *) umalloc(sizeof(CALLER));

    caller->name = strclone(name);
    caller->posx = posx;
    caller->posy = posy;

    return (caller);
}

VOID del_caller(APTR data)
{
    if (data)
    {
        CALLER *caller = (CALLER *) data;
        ufreestr(caller->name);
        ufree(caller);
    }
}

CALLER *fpos2caller(INFILEPOS * fpos)
{
    CALLER *cal = NULL;

    if (fpos)
    {
        cal = new_caller(ifp_get_fname(fpos),
                         ifp_get_x(fpos), ifp_get_y(fpos));
    }
    else
        cal = new_caller("", 0, 0);

    return (cal);
}

/*
 * functions for references
 */

/* del_reference: remove a reference */
VOID del_reference(APTR data)
{
    HSCREF *ref = (HSCREF *) data;

    ufreestr(ref->name);
    del_caller(ref->caller);
    ufree(ref);
    /* NOTE: ref->fpos is removed whith the
     * corresponding file automatically */
}

/* new_reference: create a new reference */
HSCREF *new_reference(STRPTR newname)
{
    HSCREF *ref = (HSCREF *) umalloc(sizeof(HSCREF));

    if (ref)
    {
        ref->name = strclone(newname);
        ref->caller = NULL;
    }
    return (ref);
}

/*
 * cmp_reference: compare reference
 */
int cmp_reference(APTR cmp_data, APTR list_data)
{
    HSCREF *reference = (HSCREF *) list_data;
    STRPTR fname1 = NULL;
    STRPTR fname2 = (STRPTR) cmp_data;

#if DEBUG
    if (!reference)
    {
        panic("reference = NULL");
    }
    else if (!reference->name)
    {
        panic("fname1 = NULL");
    }
    if (!fname2)
    {
        panic("fname2 = NULL");
    }
#endif
    fname1 = reference->name;

    if (!strcmp(fname1, fname2))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

/*
 * app_reference: append new reference to reflist of a document;
 *   if reference already exists, no new ref is added
 */
HSCREF *app_reference(HSCDOC * document, STRPTR ref_name)
{
    HSCREF *ref = NULL;

    /* append new reference */
    ref = new_reference(ref_name);
    app_dlnode(document->references, (APTR) ref);
#if 0
    DP(fprintf(stderr, DHP "new reference `%s'\n", ref_name));
#endif

    return (ref);
}

/*
 * functions for includes
 */

/* del_include: remove a include */
VOID del_include(APTR data)
{
    HSCINC *inc = (HSCINC *) data;

    ufreestr(inc->name);
    del_caller(inc->caller);
    ufree(inc);
}

/* new_include: create a new include */
HSCINC *new_include(STRPTR newname)
{
    HSCINC *inc = (HSCINC *) umalloc(sizeof(HSCINC));

    if (inc)
    {
        inc->name = strclone(newname);
        inc->caller = NULL;
    }
    return (inc);
}

/*
 * cmp_include: compare include
 */
int cmp_include(APTR cmp_data, APTR list_data)
{
    HSCINC *include = (HSCINC *) list_data;
    STRPTR fname1 = NULL;
    STRPTR fname2 = (STRPTR) cmp_data;

#if DEBUG
    if (!include)
    {
        panic("include = NULL");
    }
    else if (!include->name)
    {
        panic("fname1 = NULL");
    }
    if (!fname2)
    {
        panic("fname2 = NULL");
    }
#endif
    fname1 = include->name;

    if (!strcmp(fname1, fname2))
        return (-1);
    else
        return (0);
}

/*
 * app_include: append new include to inclist of a document;
 *   even append multiple instancies of the same include
 */
HSCINC *app_include(HSCDOC * document, STRPTR inc_name)
{
    DLNODE *nd = NULL;
    /* append new include */
    HSCINC *inc = new_include(inc_name);
    nd = app_dlnode(document->includes, (APTR) inc);

    return (inc);
}

/*
 * functions for id-definitions
 */

/* del_iddef: remove an id-definition */
VOID del_iddef(APTR data)
{
    HSCIDD *iddef = (HSCIDD *) data;

    ufreestr(iddef->name);
    del_caller(iddef->caller);
    if (iddef->fpos)
        del_infilepos(iddef->fpos);
    ufree(iddef);
}

/* new_iddef: create a new id-definition */
HSCIDD *new_iddef(STRPTR newname)
{
    HSCIDD *iddef = (HSCIDD *) umalloc(sizeof(HSCIDD));

    if (iddef)
    {
        iddef->name = strclone(newname);
        iddef->caller = NULL;
        iddef->fpos = NULL;
    }
    return (iddef);
}

/*
 * debugging printf functions
 */
VOID prt_iddef(FILE * stream, APTR data)
{
    HSCIDD *iddef = (HSCIDD *) data;
    INFILEPOS *fpos = iddef->fpos;

    fprintf(stream, "`%s' at (%lu,%lu)\n",
            iddef->name, ifp_get_y(fpos), ifp_get_x(fpos));
}

/*
 * cmp_iddef: compare id-definition
 */
int cmp_iddef(APTR cmp_data, APTR list_data)
{
    HSCIDD *iddef = (HSCIDD *) list_data;
    STRPTR fname1 = NULL;
    STRPTR fname2 = (STRPTR) cmp_data;

#if DEBUG
    if (!iddef)
    {
        panic("id-definition = NULL");
    }
    else if (!iddef->name)
    {
        panic("fname1 = NULL");
    }
    if (!fname2)
    {
        panic("fname2 = NULL");
    }
#endif
    fname1 = iddef->name;

    if (!strcmp(fname1, fname2))
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}

/* find_iddef: scans document list for a specific id */
HSCIDD *find_iddef(HSCDOC * document, STRPTR name)
{
    DLNODE *nd = find_dlnode(dll_first(document->iddefs),
                             (APTR) name, cmp_iddef);
    HSCIDD *iddef = NULL;

    if (nd)
    {
        iddef = (HSCIDD *) dln_data(nd);
    }

    return (iddef);
}

/*
 * app_iddef: append new id-definition to iddef-list of a document
 */
HSCIDD *app_iddef(HSCDOC * document, STRPTR iddef_name)
{
    HSCIDD *iddef = NULL;

    /* append new id-definition */
    iddef = new_iddef(iddef_name);
    app_dlnode(document->iddefs, (APTR) iddef);
#if 0
    DP(fprintf(stderr, DHP "new id-definition `%s'\n", iddef_name));
#endif

    return (iddef);
}
