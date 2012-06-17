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
 * hsclib/idref.c
 *
 * functions for id-references
 *
 */

#include "hsclib/inc_base.h"

#include <stdarg.h>
#include "ugly/ustrlist.h"

#include "hscprj/document.h"
#define NOEXTERN_HSCLIB_ID_H
#include "hsclib/idref.h"

#if DEBUG_ID
#define DIHP(x) if ( hp->debug ) x
#define DI(x)   x
#else
#define DIHP(x)                 /* nufin */
#define DI(x)                   /* nufin */
#endif

/* hsc_msg_unknown_id: warning about unknown id-refrence */
void hsc_msg_unknown_id(HSCPRC * hp, STRPTR filename, STRPTR id)
{
    if (filename)
        hsc_message(hp, MSG_UNKN_ID, "unknown id `%s#%s'",
                    filename, id);
    else
        hsc_message(hp, MSG_UNKN_ID, "unknown id %q", id);
}

/* macros for local compare-callbacks */
#define cmp_idname cmp_string_node
#define cmp_idref cmp_string_node
#define cmp_id_local cmp_string_node

/*
 *
 * new- and del-methodes
 *
 */

/*
 * del/new_idref
 */

/* del_idref: remove referenced id */
void del_idref(APTR data)
{
    IDREF *idref = (IDREF *) data;

    ufreestr(idref->name);
    del_infilepos(idref->fpos);
    ufree(idref);
}

/* new_id_local: create new reference to local id  */
static IDREF *new_idref(STRPTR id, INFILEPOS * fpos)
{
    IDREF *newid = (IDREF *) umalloc(sizeof(IDREF));

    newid->name = strclone(id);
    newid->fpos = fpos;

    return (newid);
}

void prt_idref(FILE * stream, APTR data)
{
    IDREF *idref = (IDREF *) data;
    INFILEPOS *fpos = idref->fpos;

    fprintf(stream, "`%s' at (%lu,%lu)\n",
            idref->name, ifp_get_y(fpos), ifp_get_x(fpos));
}

/*
 * add_local_iddef
 *
 * define a new local ID that can be refered to
 */
BOOL add_local_iddef(HSCPRC * hp, STRPTR id)
{
    INFILEPOS *fpos = new_infilepos(hp->inpf);
    HSCIDD *iddef = find_iddef(hp->project->document, id);

    DIHP(fprintf(stderr, DHL "add ref to id `%s' at `%s' (%lu,%lu)\n",
                 id, ifp_get_fname(fpos),
                 ifp_get_y(fpos), ifp_get_x(fpos)));

    /* check for duplicate definition */

    if (iddef)
    {
        /* duplicate definition */
        DIHP(fprintf(stderr, DHL "    duplicate definition\n"));

        hsc_message(hp, MSG_REDEFINE_ID,
                    "local id %q already declared", id);

        set_infilepos(hp->inpf, iddef->fpos);
        hsc_message(hp, MSG_REDEFINE_ID,
                    "(location of previous declaration)");

        set_infilepos(hp->inpf, fpos);
        del_infilepos(fpos);
    }
    else
    {
        /* remember new local id */
        iddef = app_iddef(hp->project->document, id);
        iddef->caller = fpos2caller(fpos);
        iddef->fpos = fpos;

        DIHP(fprintf(stderr, DHL "    append to local iddefs\n"));
    }

    return (TRUE);
}

/*
 * check_id_local
 *
 * append id to idref-list so it as checked when
 * check_all_local_idref() is called
 */
void add_local_idref(HSCPRC * hp, STRPTR id)
{
    INFILEPOS *fpos = new_infilepos(hp->inpf);

    DIHP(fprintf(stderr, DHL "  check id: `#%s' (local)\n", id));
    DIHP(fprintf(stderr, DHL "    append to idref\n"));
    app_dlnode(hp->idrefs, new_idref(id, fpos));
}

/*
 * check_local_idref
 */
static BOOL check_local_idref(HSCPRC * hp, IDREF * idref)
{
    HSCIDD *iddef = find_iddef(hp->project->document, idref->name);
    BOOL found = FALSE;

    if (iddef)
    {
        found = TRUE;
        DIHP(fprintf(stderr, DHL " local id ok: `%s'\n", idref->name));
    }
    else
    {
        DIHP(
                {
                INFILEPOS * fpos = idref->fpos;
                fprintf(stderr, DHL " local id unknown: `%s' (%lu,%lu)\n",
                        idref->name, ifp_get_y(fpos), ifp_get_x(fpos));
                }
        );

        set_infilepos(hp->inpf, idref->fpos);
        hsc_msg_unknown_id(hp, NULL, idref->name);
    }

    return (found);
}

/*
 * check_all_local_idref
 */
BOOL check_all_local_idref(HSCPRC * hp)
{
    BOOL ok = TRUE;
    DLNODE *nd = NULL;
    INFILEPOS *infpos = new_infilepos(hp->inpf);

    DIHP(fprintf(stderr, DHL "check local IDs\n"));

    DIHP(
            {

            fprintf(stderr, DHL " local IDs defined:\n");
            fprintf_dllist(stderr, hp->project->document->iddefs, prt_iddef);
            fprintf(stderr, DHL " local IDs referenced:\n");
            fprintf_dllist(stderr, hp->idrefs, prt_idref);

            }
    );

    nd = dll_first(hp->idrefs);
    while (nd)
    {
        check_local_idref(hp, ((IDREF *) nd->data));
        nd = dln_next(nd);
    }

    set_infilepos(hp->inpf, infpos);
    del_infilepos(infpos);

    return (ok);
}

/* $Id: idref.c,v 1.3 2005/04/12 14:29:35 mb Exp $ */
/* vi: set ts=4: */

