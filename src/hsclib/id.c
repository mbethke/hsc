/*
** hsclib/id.c
**
** id functions for hsc
**
** Copyright (C) 1996  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 15-Apr-1996
** created: 12-Apr-1995
**
*/

#include "hsclib/inc_base.h"

#include <stdarg.h>

#define NOEXTERN_HSCLIB_ID_H
#include "hsclib/id.h"


#if DEBUG_ID
#define DIHP(x) if ( hp->debug ) x
#define DI(x)   x
#else
#define DIHP(x) /* nufin */
#define DI(x)   /* nufin */
#endif


/*
 * NOTE:
 *     hp->id_global:
 *         list of ids defined after hsc_read_id();
 *         usually taken from file "hsc.id". this
 *         is a list of IDNODEs.
 *
 *     hp->id_ref: ids of current file, that are also
 *         referenced from current file. this one is
 *         only a simple list of strings
 */

/* hsc_msg_unknown_id: warning about unknown id */
VOID hsc_msg_unknown_id( HSCPRC *hp, STRPTR filename, STRPTR id )
{
    if ( filename )
        hsc_message( hp, MSG_UNKN_ID, "unknown id `%s#%s'",
                     filename, id );
    else
        hsc_message( hp, MSG_UNKN_ID, "unknown id %q", id );
}

/*
 *
 * compare-callbacks
 *
 */
static int cmp_idname( APTR cmp_data, APTR list_data )
{
    STRPTR s1 = (STRPTR) cmp_data;
    STRPTR s2 = ((IDLOCAL*) list_data)->name;

    if ( !strcmp( s1, s2 ) )
        return (-1);
    else
        return (0);
}

static int cmp_id_ref( APTR data1, APTR data2 )
{
    return( cmp_idname( data1, data2 ) );
}

static int cmp_id_local( APTR data1, APTR data2 )
{
    return( cmp_idname( data1, data2 ) );
}

/*
 *
 * find and exists-functins
 *
 */
#if 0
static IDGLOBAL *find_idfile( DLLIST *idlist, STRPTR filename )
{
    DLNODE *nd = find_dlnode( dll_first( idlist ),
                              (APTR) filename, cmp_idfile );
    IDGLOBAL *idfile = NULL;

    if ( nd )
        idfile = dln_data( nd );

    return( idfile );
}

static BOOL idfile_exists( DLLIST *idlist, STRPTR filename )
{
    if ( find_idfile( idlist, filename ) )
        return( TRUE );
    else
        return( FALSE );
}

static BOOL idname_exists( DLLIST *idlist, STRPTR filename, STRPTR id )
{
    IDGLOBAL *idfile = find_idfile( idlist, filename );
    BOOL    found  = FALSE;

    if ( idfile )
      {
        DLNODE *nd = find_dlnode( dll_first( idfile->idlist ),
                                  (APTR) id, cmp_idname );
        if ( nd )
            found = TRUE;
      }

    return( found );
}
#endif

/*
 *
 * new- and del-methodes
 *
 */


/*
 * del/new_id_local
 */

/* del_id_local: remove local ID */
VOID del_id_local( APTR data )
{
    IDLOCAL *id_local = (IDLOCAL*) data;

    ufreestr( id_local->name );
    del_infilepos( id_local->fpos );
    ufree( id_local );
}

/* new_id_local: create new local id  */
static IDLOCAL *new_id_local( STRPTR id, INFILEPOS *fpos )
{
    IDLOCAL *newid = (IDLOCAL*) umalloc( sizeof( IDLOCAL ) );

    newid->name = strclone( id );
    newid->fpos = fpos;

    return( newid );
}

/*
 * del/new_id_ref
 */

/* del_id_ref: remove referenced id */
VOID del_id_ref( APTR data )
{
    IDREF *id_ref = (IDREF*) data;

    ufreestr( id_ref->name );
    del_infilepos( id_ref->fpos );
    ufree( id_ref );
}

/* new_id_local: create new reference to local id  */
static IDREF *new_id_ref( STRPTR id, INFILEPOS *fpos )
{
    IDREF *newid = (IDREF*) umalloc( sizeof( IDREF ) );

    newid->name = strclone( id );
    newid->fpos = fpos;

    return( newid );
}


/* del_subdoc: remove sub-document list-entry */
VOID del_subdoc( APTR data )
{
    STRPTR s = (STRPTR) data;
    ufreestr( s );
}

/*
 * debugging printf functions
 */
VOID prt_id_local( FILE *stream, APTR data )
{
    IDLOCAL *id_local = (IDLOCAL *) data;
    INFILEPOS *fpos = id_local->fpos;

    fprintf( stream, "`%s' at (%lu,%lu)\n",
             id_local->name, ifp_get_y( fpos ), ifp_get_x( fpos) );
}

VOID prt_id_ref( FILE *stream, APTR data )
{
    prt_id_local( stream, data );
}



/*
 * hsc_add_id_local
 */
BOOL hsc_add_id_local( HSCPRC *hp, STRPTR id )
{
    INFILEPOS *fpos = new_infilepos( hp->inpf );

    DIHP( fprintf( stderr, DHL "add local id `%s' at `%s' (%lu,%lu)\n",
                   id, ifp_get_fname( fpos ),
                   ifp_get_y( fpos ), ifp_get_x( fpos) ) );

    /* TODO: check for duplicate ID */
    app_dlnode( hp->id_local, (APTR) new_id_local( id, fpos ) );

    return( TRUE );
}

/*
 * hsc_add_id_ref
 */
BOOL hsc_add_id_ref( HSCPRC *hp, STRPTR id )
{
    INFILEPOS *fpos = new_infilepos( hp->inpf );
    DLNODE    *nd   = NULL;

    DIHP( fprintf( stderr, DHL "add ref to id `%s' at `%s' (%lu,%lu)\n",
                   id, ifp_get_fname( fpos ),
                   ifp_get_y( fpos ), ifp_get_x( fpos) ) );

    /* check for duplucate definition */
    nd = find_dlnode( dll_first( hp->id_local ),
                      (APTR) id, cmp_id_local );
    if ( nd )
      {
        /* duplicate definition */
        IDLOCAL   *id_local = (IDLOCAL *) nd->data;

        DIHP( fprintf( stderr, DHL "    duplicate definition\n", id ) );

        hsc_message( hp, MSG_REDEFINE_ID,
                     "redefined id %q", id_local->name );

        set_infilepos( hp->inpf, id_local->fpos );
        hsc_message( hp, MSG_REDEFINE_ID,
                     "(location of previous declaration)" );

        set_infilepos( hp->inpf, fpos );
        del_infilepos( fpos );
      }
    else
      {
        /* remember new local id */
        DIHP( fprintf( stderr, DHL "    append to id_local\n", id ) );
        app_dlnode( hp->id_local, (APTR) new_id_ref( id, fpos ) );
      }

    return( TRUE );
}


/*
 * check_id_local
 *
 * append id to id_ref-list so it as checked when
 * check_all_local_id_ref() is called
 */
VOID check_id_local( HSCPRC *hp, STRPTR id )
{
    INFILEPOS *fpos = new_infilepos( hp->inpf );

    DIHP( fprintf( stderr, DHL "  check id: `#%s' (local)\n", id ) );
    DIHP( fprintf( stderr, DHL "    append to id_ref\n", id ) );
    app_dlnode( hp->id_ref, new_id_ref( id, fpos ) );
}


/*
 * check_local_id_ref
 */
static BOOL check_local_id_ref( HSCPRC *hp, IDREF *id_ref )
{
    DLNODE *nd = find_dlnode( dll_first( hp->id_local ),
                              (APTR) id_ref->name, cmp_id_ref );
    BOOL    found = FALSE;

    if ( nd ) {

        found = TRUE;
        DIHP( fprintf( stderr, DHL " local id ok: `%s'\n", id_ref->name ) );

    } else {

        DIHP( {
            INFILEPOS *fpos = id_ref->fpos;
            fprintf( stderr, DHL " local id unknown: `%s'\n",
                     id_ref->name, ifp_get_y( fpos ), ifp_get_x( fpos) );
        } );

        set_infilepos( hp->inpf, id_ref->fpos );
        hsc_msg_unknown_id( hp, NULL, id_ref->name );

    }

    return( found );
}

/*
 * check_all_local_id_ref
 */
BOOL check_all_local_id_ref( HSCPRC *hp )
{
    BOOL    ok     = TRUE;
    DLNODE *nd     = NULL;
    INFILEPOS *infpos = new_infilepos( hp->inpf );

    DIHP( fprintf( stderr, DHL "check local IDs\n" ) );

    DIHP( {

        fprintf( stderr, DHL " local IDs defined:\n" );
        fprintf_dllist( stderr, hp->id_local, prt_id_local );
        fprintf( stderr, DHL " local IDs referenced:\n" );
        fprintf_dllist( stderr, hp->id_ref, prt_id_ref );

    } );

    nd = dll_first( hp->id_ref );
    while ( nd ) {

        check_local_id_ref( hp, ( (IDREF *) nd->data ) );
        nd = dln_next( nd );
    }

    set_infilepos( hp->inpf, infpos );
    del_infilepos( infpos );

    return( ok );
}

