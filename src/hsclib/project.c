/*
** hsclib/project.c
**
** project managment routines for hsc
**
** Copyright (C) 1995,96  Thomas Aglassinger
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
** updated: 12-May-1996
** created: 13-Apr-1996
*/

#include "hsclib/inc_base.h"

#include "hsclib/id.h"

/*
 * project_file:
 *
 * line 1  : "HSC_PROJECT"             to identify prokect file
 * line 2  : "VERSION x"               file format version
 * comments: "# xxx"                   some comment lines
 *
 * fileinfo: "DOCUMENT" len filename       start fileinfo block
 *           "ID"       len idname         ID defined in this file
 */

/* debugging define */
#define DHLP DHL "[proj] "
#if DEBUG_ID
#define DIHP(x) if ( hp->debug ) x
#define DI(x)   x
#else
#define DIHP(x) /* nufin */
#define DI(x)   /* nufin */
#endif

/*
 * IDs in project file
 */
#define FILEID_HSCPRJ    "HSC_PROJECT"
#define VERSION_HSCPRJ   1

#define LINE_DOCUMENT_STR "DOCUMENT"
#define LINE_ID_STR       "ID"
#define LINE_REM_STR      "#"
#define LINE_VERSION_STR  "VERSION"

#define TIMEBUFSIZE 40
static STRARR timebuf[TIMEBUFSIZE];

typedef struct idglobal_node
{
    STRPTR  filename; /* filename */
    STRPTR  title;    /* title specified with <TITLE> */
    DLLIST *idlist;   /* list of IDs defined in this file */
} IDGLOBAL;

/*
 * del/new_idlist_entry
 */

/* del_idlist_entry */
VOID del_idlist_entry( APTR data )
{
    STRPTR s = (STRPTR) data;
    ufreestr( s );
}

STRPTR new_idlist_entry( STRPTR idname )
{
#if 0
    DI( fprintf( stderr, DHL "new id-entry `%s'\n", idname ) );
#endif
    return( strclone( idname ) );
}

/*
 * del/new_id_global
 */

/* del_id_global: remove global ID */
VOID del_id_global( APTR data )
{
    IDGLOBAL *id_global = (IDGLOBAL*) data;

    ufreestr( id_global->filename );
    ufreestr( id_global->title );
    del_dllist( id_global->idlist );
    ufree( id_global );
}

/* new_id_global: create new file and its included IDs */
IDGLOBAL *new_id_global( STRPTR filename )
{
    IDGLOBAL *newid = (IDGLOBAL*) umalloc( sizeof( IDGLOBAL ) );

#if 0
    DI( fprintf( stderr, DHL "new id-file `%s'\n", filename ) );
#endif
    newid->filename = strclone( filename );
    newid->title    = NULL;
    newid->idlist   = init_dllist( del_idlist_entry );

    return( newid );
}

/*
 * cmp_idfile: compare callback
 */
static int cmp_idfile( APTR cmp_data, APTR list_data )
{
    IDGLOBAL *idfile = (IDGLOBAL *) list_data;
    STRPTR    fname1 = NULL;
    STRPTR    fname2 = (STRPTR)   cmp_data;

#if DEBUG
    if ( !idfile )
        panic( "idfile = NULL" );
    else if ( !idfile->filename )
        panic( "fname1 = NULL" );
    if ( !fname2 )
        panic( "fname2 = NULL" );
#endif
    fname1 = idfile->filename;

    if ( !strcmp( fname1, fname2 ) )
        return (-1);
    else
        return (0);
}

static int cmp_idlist_entry( APTR cmp_data, APTR list_data )
{
    STRPTR    fname1 = (STRPTR) list_data;
    STRPTR    fname2 = (STRPTR) cmp_data;

#if DEBUG
    if ( !fname1 )
        panic( "fname1 = NULL" );
    if ( !fname2 )
        panic( "fname2 = NULL" );
#endif

    if ( !strcmp( fname1, fname2 ) )
        return (-1);
    else
        return (0);
}


/*
 * check_id_global
 *
 * append id to id_ref-list so it as checked when
 * check_all_local_id_ref() is called
 */
VOID check_id_global( HSCPRC *hp, STRPTR filename, STRPTR id )
{
    DLNODE *nd = NULL;

    DIHP( fprintf( stderr, DHL "  check id: `%s#%s'\n", filename, id ) );

    nd = find_dlnode( dll_first( hp->id_global ),
                      (APTR) filename, cmp_idfile );
    if ( nd ) {

        IDGLOBAL *id_global = (IDGLOBAL *) dln_data( nd );

        nd = find_dlnode( dll_first( id_global->idlist ),
                          (APTR) id, cmp_idlist_entry );
        if ( !nd ) {

            DIHP( fprintf( stderr, DHL "    id unknown\n" ) );
            hsc_msg_unknown_id( hp, filename, id );

        } else {

            DIHP( fprintf( stderr, DHL "    id ok\n" ) );

        }

    } else {

        DIHP( fprintf( stderr, DHL "    no file-entry in project\n",
                       filename, id ) );

    }
}


/*
 *-------------------------------------
 * write project file
 *-------------------------------------
 */

/*
 * hsc_write_project_file
 *
 * write ids to file
 */
static VOID append_ulong( EXPSTR *dest, ULONG num )
{
    STRARR lenbuf[20];

    sprintf( lenbuf, "%lx ", num );
    app_estr( dest, lenbuf );
}

static VOID append_string( EXPSTR *dest, STRPTR s )
{
    append_ulong( dest, (ULONG) strlen( s ) );
    app_estr( dest, s );
}

static VOID append_filename( EXPSTR *prjstr, STRPTR filename )
{
    app_estr( prjstr, LINE_DOCUMENT_STR );
    app_estr( prjstr, " " );
    append_string( prjstr, filename );
    app_estr( prjstr, "\n" );
}

static VOID append_id( EXPSTR *prjstr, STRPTR id )
{
    app_estr( prjstr, LINE_ID_STR );
    app_estr( prjstr, " " );
    append_string( prjstr, id );
    app_estr( prjstr, "\n" );
}


static VOID append_header( EXPSTR *prjstr )
{
    time_t  now = time( NULL ); /* get current time */

    /* create string for current time */
    strftime( timebuf, TIMEBUFSIZE,
              "%d-%b-%Y %H:%M:%S", localtime( &now ) );

    /* append key-sequence, file-format-version and comment */
    app_estr( prjstr,
              FILEID_HSCPRJ "\n" LINE_VERSION_STR " " );
    append_ulong( prjstr, VERSION_HSCPRJ );
    app_estr( prjstr, "\n" );
    app_estr( prjstr,
              LINE_REM_STR " contains all project relevant data\n"
              LINE_REM_STR " maintained by hsc, DO NOT MODIFY!\n" );
    app_estr( prjstr,
              LINE_REM_STR " updated: " );
    app_estr( prjstr, timebuf );
    app_estrch( prjstr, '\n' );
}

static VOID append_id_global( EXPSTR *prjstr, IDGLOBAL *id_global )
{
    DLNODE *idnd   = NULL;
    STRPTR  filename = id_global->filename;
    DLLIST *idlist   = id_global->idlist;

    append_filename( prjstr, filename );

    idnd = dll_first( idlist );
    while ( idnd )
      {
        append_id( prjstr, (STRPTR) idnd->data );
        idnd = dln_next( idnd );
      }
}

static VOID append_id_local( EXPSTR *prjstr, STRPTR filename,
                             DLLIST *id_local_list )
{
    DLNODE *nd = dll_first( id_local_list );

    append_filename( prjstr, filename );

    while ( nd ) {

        IDLOCAL *id_local = (IDLOCAL *) dln_data( nd );
        append_id( prjstr, id_local->name );
        nd = dln_next( nd );

    }
}

/*
 * hsc_write_project_file
 *
 * store all project-data in a string and write it to
 * the file specified with hsc_set_project_filename()
 */
BOOL hsc_write_project_file( HSCPRC *hp )
{
    BOOL    written  = FALSE;

    if ( !hp->fatal && hp->filename_process && hp->filename_project ) {

        EXPSTR *prjstr   = init_estr( 256 );
        DLNODE *nd   = NULL;
        FILE   *outfile  = NULL;

        DIHP( fprintf( stderr, DHLP "update project file `%s'\n",
                       hp->filename_project ) );

        /* remove old info of current file */
        nd = find_dlnode( dll_first( hp->id_global ),
                          (APTR) hp->filename_process, cmp_idfile );
        if (nd ) {

            del_dlnode( hp->id_global, nd );

        } else {

            DIHP( fprintf( stderr, DHL "new file in project: `%s'\n",
                           hp->filename_process ) );
            /* TODO: show message "creating new entry in project file" */
        }


        /* append header information */
        append_header( prjstr );

        /* update project info of current file */
        append_id_local( prjstr, hp->filename_process, hp->id_local );

        /* append all old project info of other files */
        nd = dll_first( hp->id_global );
        while ( nd )
          {
            IDGLOBAL *id_global = (IDGLOBAL*) nd->data;

            append_id_global( prjstr, id_global );

            nd = dln_next( nd );
          }

        DIHP( fprintf( stderr, DHLP "project file contains:\n%s", estr2str( prjstr ) ) );

        /* write new project file */
        errno = 0;
        outfile = fopen( hp->filename_project, "w" );
        if ( outfile ) {

            errno = 0;
            fwrite( estr2str( prjstr ), sizeof( char ),
                    estrlen( prjstr ), outfile );

            if ( errno ) {

                DIHP( fprintf( stderr, DHLP "can't write project file\n" ) );
            } else
                written = TRUE;

        } else {

          DIHP( fprintf( stderr, DHLP "can't open project file for output\n" ) );
          /* TODO: message "cant open project file" */

        }

        del_estr( prjstr );

    } else {

        D( fprintf( stderr, DHLP "no update project\n" ) );

    }

    return( written );
}

/*
 *-------------------------------------
 * read project file
 *-------------------------------------
 */

/*
 * hsc_msg_project_corrupt: display warning about
 * corrupt project file
 */
static VOID hsc_msg_project_corrupt( HSCPRC *hp, STRPTR descr )
{
    STRPTR prjtxt = "project file corrupt";

    if ( descr )
        hsc_message( hp, MSG_CORRUPT_PRJFILE, "%s (%s)", prjtxt, descr );
    else
        hsc_message( hp, MSG_CORRUPT_PRJFILE, "%s", prjtxt );
}

static int x2int( char c )
{
    if ( (c>='0') && (c<='9') ) return( c-'0' );
    if ( (c>='A') && (c<='F') ) return( c-'A'+10 );
    if ( (c>='a') && (c<='f') ) return( c-'a'+10 );
    return(EOF);
}

/*
 * read_long
 */
static ULONG read_ulong( HSCPRC *hp )
{
    INFILE *inpf = hp->inpf;
    ULONG   num  = 0;
    int ch;
    int digit = EOF;

    do {
        ch = infgetc( inpf );
        if ( ch != ' ' ) {

            digit = x2int( ch );
            if ( digit == EOF ) {

                num = 0;

            } else
                num = (num<<4) + digit;

        }

        if ( digit == EOF )
            hsc_msg_project_corrupt( hp, "hex digit expected" );

    } while ( (digit != EOF) && (ch!=' ') );

    if ( digit == EOF )
        num = 0;

    return( num );
}

/*
 * read_string:
 *
 * read string length, alloc mem and read string into it
 */
static STRPTR read_string( HSCPRC *hp )
{
    STRPTR dest = NULL;

    ULONG len = read_ulong( hp );

    if ( len ) {

        ULONG i;
        int   ch = 'x'; /* dummy init */

        /* alloc mem */
        dest = umalloc( (size_t) (len+1) );
        dest[len] = '\0';


        for ( i=0; ( (i<len) && (ch!=EOF) ); i++ ) {

            ch = infgetc( hp->inpf );
            if ( ch != EOF )
                dest[i] = ch;
            else
                hsc_msg_project_corrupt( hp, "string expected" );

        }

        if ( ch != EOF )
            dest[len] = 0;
        else {
            ufree( dest );
            dest = NULL;
        }
    }

    return( dest );
}

/*
 * read_lf: read linefeed
 */
static BOOL read_lf( HSCPRC *hp )
{
    int ch = infgetc( hp->inpf );
    BOOL ok = TRUE;

    if ( ch != '\n' ) {
        hsc_msg_project_corrupt( hp, "linefeed expected" );
        ok = FALSE;
    }

    return( ok );
}

/*
 * read command
 *
 * returns next command or NULL if eof reached
 */
static STRPTR read_command( HSCPRC *hp )
{
    STRPTR command;

    do {
        command = infgetw( hp->inpf );
    } while ( command && !strcmp( command, "\n" ) );

    if ( command ) {
        /* skip blanks */
        infskip_ws( hp->inpf );
        DIHP( fprintf( stderr, DHLP "command `%s'\n", command ) );
    } else {
        DIHP( fprintf( stderr, DHLP "command EOF\n" ) );
    }

    return( command );
}


/*
 * read header
 */
static BOOL read_header( HSCPRC *hp )
{
    STRARR fileid[ 1+sizeof( FILEID_HSCPRJ ) ];
    BOOL   ok = FALSE;
    STRPTR cmd = NULL;
    size_t i;

    /* read fileid */
    for ( i=0; i<strlen( FILEID_HSCPRJ ); i++ )
    {
        int ch = infgetc( hp->inpf );

        fileid[i] = (UBYTE) ch;
    }
    fileid[i] = '\0';

    DIHP( fprintf( stderr, DHLP "fileid: `%s'\n", fileid ) );

    /* check fileid */
    if ( !strcmp( fileid, FILEID_HSCPRJ ) )
    {
        DIHP( fprintf( stderr, DHLP "fileid: `%s'\n", fileid ) );
        ok = read_lf( hp );
    } else {
        hsc_msg_project_corrupt( hp, "wrong file-id" );
    }

    if ( ok ) {

        ok = FALSE;

        /* read version */
        cmd = read_command( hp );

        /* check version */
        if ( cmd && !strcmp( cmd, LINE_VERSION_STR ) ) {

            ULONG version = read_ulong( hp );

            DIHP( fprintf( stderr, DHLP "version: %lu\n", version ) );

            if ( version && (version <= 1) )
            {
                ok = read_lf( hp );
            } else {

                hsc_msg_project_corrupt( hp, "wrong version" );

            }

        } else {

            hsc_msg_project_corrupt( hp, "unknown version" );

        }

    }

    return( ok );
}

/*
 * hsc_read_project_file
 *
 * read project file
 */
BOOL hsc_read_project_file( HSCPRC *hp )
{
    BOOL ok = FALSE;

    if ( hp->filename_project ) {

        INFILE *inpf = NULL;

        DIHP( fprintf( stderr, DHLP "read project file from `%s'\n",
                       hp->filename_project ) );

        hp->inpf = infopen( hp->filename_project, 0 );
        inpf = hp->inpf;

        if ( inpf ) {

            if ( read_header( hp ) ) {

                IDGLOBAL *id_global = NULL;
                STRPTR    cmd = NULL;


                do {
                    cmd = read_command( hp );
                    if ( cmd ) {

                        if ( !strcmp( cmd, LINE_DOCUMENT_STR ) ) {

                            /* begin new DOCUMENT */
                            STRPTR filename = read_string( hp );
                            if ( filename ) {
                                id_global = new_id_global( filename );
                                app_dlnode( hp->id_global, (APTR) id_global );
                                ufree( filename ); /* free mem allocated by read_string() */
                            }

                        } else if ( !strcmp( cmd, LINE_ID_STR ) ) {

                            /* append new ID */
                            if ( id_global ) {
                                STRPTR idname = read_string( hp );
                                if ( idname ) {
                                    app_dlnode( id_global->idlist, (APTR) new_idlist_entry( idname ) );
                                    ufree( idname ); /* free mem allocated by read_string() */
                                }
                            } else
                                hsc_msg_project_corrupt( hp, LINE_ID_STR " without " LINE_DOCUMENT_STR );

                        } else if ( !strcmp( cmd, LINE_REM_STR ) ) {

                            /* skip comment */
                            int ch;
                            DIHP( fprintf( stderr, DHLP "comment `" ) );
                            do {
                                ch = infgetc( inpf );
                                DIHP( {
                                    if ( ch != '\n' ) {
                                        fprintf( stderr, "%c", ch );
                                    }
                                } );

                            } while ( (ch != EOF) && (ch!='\n') );
                            DIHP( fprintf( stderr, "'\n" ) );

                        } else {

                            /* unknown command */
                            hsc_msg_project_corrupt( hp, "unknown tag" );

                        }
                    } else {
                        DIHP( fprintf( stderr, DHLP "EOF\n" ) );
                    }
                } while (cmd && !hp->fatal);
            }

            infclose( inpf );
            hp->inpf = NULL;

            if ( !hp->fatal )
                ok = TRUE;

        } else {

            DIHP( fprintf( stderr, DHLP "can't open project file from `%s'\n",
                           hp->filename_project ) );
            /* TODO: message "no project file" */

            ok = TRUE;

          }


    } else {

        DIHP( fprintf( stderr, DHL "no project file specified'\n" ) );
        ok = TRUE;

    }
    return( ok );
}

