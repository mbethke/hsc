/*
 * hsclib/idref.c
 *
 * functions for id-references
 *
 * Copyright (C) 1996  Thomas Aglassinger
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
 * updated:  5-Jul-1996
 * created: 12-Apr-1995
 *
 */

#include "hsclib/inc_base.h"

#include <stdarg.h>

#include "hsclib/document.h"
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
VOID hsc_msg_unknown_id(HSCPRC * hp, STRPTR filename, STRPTR id)
{
    if (filename)
        hsc_message(hp, MSG_UNKN_ID, "unknown id `%s#%s'",
                    filename, id);
    else
        hsc_message(hp, MSG_UNKN_ID, "unknown id %q", id);
}

/*
 *
 * string-callbacks
 *
 */

/*
 * del_string_entry
 */
VOID del_string_entry(APTR data)
{
    STRPTR s = (STRPTR) data;
    ufreestr(s);
}

/*
 * new_string_entry
 */
STRPTR new_string_entry(STRPTR data)
{
#if 0
    DI(fprintf(stderr, DHL "new string `%s'\n", data));
#endif
    return (strclone(data));
}

/*
 * cmp_string_entry
 */
int cmp_string_entry(APTR cmp_data, APTR lst_data)
{
    STRPTR s1 = (STRPTR) cmp_data;
    STRPTR s2 = (STRPTR) lst_data;

#if DEBUG
    if (!cmp_data)
        panic("cmp_data = NULL");
    if (!lst_data)
        panic("lst_data = NULL");
#endif

    if (!strcmp(s1, s2))
        return (-1);
    else
        return (0);
}

/* macros for local compare-callbacks */
#define cmp_idname cmp_string_entry
#define cmp_idref cmp_string_entry
#define cmp_id_local cmp_string_entry

/*
 *
 * new- and del-methodes
 *
 */

#if 0
/*
 * del/new_id_local
 */

/* del_id_local: remove local ID */
VOID del_id_local(APTR data)
{
    IDLOCAL *id_local = (IDLOCAL *) data;

    ufreestr(id_local->name);
    del_infilepos(id_local->fpos);
    ufree(id_local);
}

/* new_id_local: create new local id  */
static IDLOCAL *new_id_local(STRPTR id, INFILEPOS * fpos)
{
    IDLOCAL *newid = (IDLOCAL *) umalloc(sizeof(IDLOCAL));

    newid->name = strclone(id);
    newid->fpos = fpos;

    return (newid);
}

/*
 * hsc_add_id_local
 */
BOOL hsc_add_id_local(HSCPRC * hp, STRPTR id)
{
    INFILEPOS *fpos = new_infilepos(hp->inpf);

    DIHP(fprintf(stderr, DHL "add local id `%s' at `%s' (%lu,%lu)\n",
                 id, ifp_get_fname(fpos),
                 ifp_get_y(fpos), ifp_get_x(fpos)));

    /* TODO: check for duplicate ID */
    app_dlnode(hp->id_local, (APTR) new_id_local(id, fpos));

    return (TRUE);
}

#endif

/*
 * del/new_idref
 */

/* del_idref: remove referenced id */
VOID del_idref(APTR data)
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

VOID prt_idref(FILE * stream, APTR data)
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
    HSCIDD *iddef = find_iddef(hp->document, id);

    DIHP(fprintf(stderr, DHL "add ref to id `%s' at `%s' (%lu,%lu)\n",
                 id, ifp_get_fname(fpos),
                 ifp_get_y(fpos), ifp_get_x(fpos)));

    /* check for duplicate definition */

    if (iddef) {
        /* duplicate definition */
        DIHP(fprintf(stderr, DHL "    duplicate definition\n", id));

        hsc_message(hp, MSG_REDEFINE_ID,
                    "local id %q already declared", id);

        set_infilepos(hp->inpf, iddef->fpos);
        hsc_message(hp, MSG_REDEFINE_ID,
                    "(location of previous declaration)");

        set_infilepos(hp->inpf, fpos);
        del_infilepos(fpos);
    } else {
        /* remember new local id */
        iddef = app_iddef(hp->document, id);
        iddef->caller = fpos2caller(fpos);
        iddef->fpos = fpos;

        DIHP(fprintf(stderr, DHL "    append to local iddefs\n", id));
    }

    return (TRUE);
}

/*
 * check_id_local
 *
 * append id to idref-list so it as checked when
 * check_all_local_idref() is called
 */
VOID add_local_idref(HSCPRC * hp, STRPTR id)
{
    INFILEPOS *fpos = new_infilepos(hp->inpf);

    DIHP(fprintf(stderr, DHL "  check id: `#%s' (local)\n", id));
    DIHP(fprintf(stderr, DHL "    append to idref\n", id));
    app_dlnode(hp->idrefs, new_idref(id, fpos));
}

/*
 * check_local_idref
 */
static BOOL check_local_idref(HSCPRC * hp, IDREF * idref)
{
    HSCIDD *iddef = find_iddef(hp->document, idref->name);
    BOOL found = FALSE;

    if (iddef) {

        found = TRUE;
        DIHP(fprintf(stderr, DHL " local id ok: `%s'\n", idref->name));

    } else {

        DIHP( {
             INFILEPOS * fpos = idref->fpos;
             fprintf(stderr, DHL " local id unknown: `%s'\n",
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

    DIHP( {

         fprintf(stderr, DHL " local IDs defined:\n");
         fprintf_dllist(stderr, hp->document->iddefs, prt_iddef);
         fprintf(stderr, DHL " local IDs referenced:\n");
         fprintf_dllist(stderr, hp->idrefs, prt_idref);

         }
    );

    nd = dll_first(hp->idrefs);
    while (nd) {

        check_local_idref(hp, ((IDREF *) nd->data));
        nd = dln_next(nd);
    }

    set_infilepos(hp->inpf, infpos);
    del_infilepos(infpos);

    return (ok);
}

