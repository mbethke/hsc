/*
 * hsclib/project.c
 *
 * project managment routines for hsc
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
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
 * updated:  9-Jul-1996
 * created: 13-Apr-1996
 */

#include "hsclib/inc_base.h"

#include "hsclib/idref.h"
#include "hsclib/document.h"
#include "hsclib/project.h"

/*
 * project_file:
 *
 * line 1  : "HSC_PROJECT"             to identify prokect file
 * line 2  : "VERSION x"               file format version
 * comments: "# xxx"                   some comment lines
 *
 * fileinfo: "DOCUMENT"   len uri            document
 *                                           start document-info block
 *           "SOURCE"     len file           hsc-source used to create
 *                                           this document
 *           "TITLE"      len title          document title
 *           "ID"         len idname         ID defined in this doc.
 *           "INCLUDE     len file"          file included during
 *                                           processing this document
 *           "REFERENCE   len uri            document/object refered
 *                                           from this document
 */

/* debugging define */
#define DHLP DHL "[proj] "
#if DEBUG_ID
#define DIHP(x) if ( hp->debug ) x
#define DI(x)   x
#else
#define DIHP(x)                 /* nufin */
#define DI(x)                   /* nufin */
#endif

/*
 * IDs in project file
 */
#define FILEID_HSCPRJ    "HSC_PROJECT"
#define VERSION_HSCPRJ   2

#define LINE_VERSION_STR  "VERSION"
#define LINE_REM_STR      "#"

#define LINE_DOCUMENT_STR  "DOCUMENT"
#define LINE_SOURCE_STR    "SOURCE"
#define LINE_TITLE_STR     "TITLE"
#define LINE_ID_STR        "ID"
#define LINE_INCLUDE_STR   "INCLUDE"
#define LINE_REFERENCE_STR "REFERENCE"

#define ID_CALLER_STR "FROM"

#define TIMEBUFSIZE 40
static STRARR timebuf[TIMEBUFSIZE];

/*
 * check_document_id
 *
 * append id to id_ref-list so it as checked when
 * check_all_local_id_ref() is called
 */
VOID check_document_id(HSCPRC * hp, STRPTR docname, STRPTR id)
{
    HSCDOC *document = find_document(hp->documents, docname);

    DIHP(fprintf(stderr, DHL "  check id: `%s#%s'\n", docname, id));

    if (document) {

        HSCIDD *iddef = NULL;

        iddef = find_iddef(document, id);
        if (!iddef) {

            DIHP(fprintf(stderr, DHL "    id unknown\n"));
            hsc_msg_unknown_id(hp, docname, id);

        } else {
            DIHP(fprintf(stderr, DHL "    id ok\n"));
        }
    } else {

        DIHP(fprintf(stderr, DHL "    no file-entry in project\n",
                     docname, id));

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
static VOID append_ulong(EXPSTR * dest, ULONG num)
{
    STRARR lenbuf[20];

    sprintf(lenbuf, "%lx ", num);
    app_estr(dest, lenbuf);
}

static VOID append_string(EXPSTR * dest, STRPTR s)
{
    append_ulong(dest, (ULONG) strlen(s));
    app_estr(dest, s);
}

static VOID append_caller(EXPSTR * dest, CALLER * caller)
{
#if 0
    if (caller) {
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
    if (sourcename) {
        app_estr(prjstr, LINE_SOURCE_STR);
        app_estr(prjstr, " ");
        append_string(prjstr, sourcename);
        app_estr(prjstr, "\n");
    }
}

static VOID append_title(EXPSTR * prjstr, STRPTR title)
{
    if (title) {
        app_estr(prjstr, LINE_TITLE_STR);
        app_estr(prjstr, " ");
        append_string(prjstr, title);
        app_estr(prjstr, "\n");
    }
}

static VOID append_id(EXPSTR * prjstr, STRPTR id)
{
    app_estr(prjstr, LINE_ID_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, id);
    app_estr(prjstr, "\n");
}

static VOID append_include(EXPSTR * prjstr, HSCINC * include)
{
    app_estr(prjstr, LINE_INCLUDE_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, include->name);
    append_caller(prjstr, include->caller);
    app_estr(prjstr, "\n");
#if 0
    DI(fprintf(stderr, DHL "  include `%s'\n", include->name));
#endif
}

static VOID append_reference(EXPSTR * prjstr, HSCREF * reference)
{
#if 1
    app_estr(prjstr, LINE_REFERENCE_STR);
    app_estr(prjstr, " ");
    append_string(prjstr, reference->name);
    append_caller(prjstr, reference->caller);
    app_estr(prjstr, "\n");
#else
    DI(fprintf(stderr, DHL "  refers `%s'\n", reference->name));
#endif
}

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
             LINE_REM_STR " contains all project relevant data\n"
             LINE_REM_STR " maintained by hsc, DO NOT MODIFY!\n");
    app_estr(prjstr,
             LINE_REM_STR " updated: ");
    app_estr(prjstr, timebuf);
    app_estrch(prjstr, '\n');
}

/* append included files */
static VOID append_doc_includes(EXPSTR * prjstr, DLLIST * inclist)
{
    DLNODE *nd = dll_first(inclist);
    while (nd) {
        append_include(prjstr, (HSCINC *) dln_data(nd));
        nd = dln_next(nd);
    }
}

/* append other documents referered */
static VOID append_doc_references(EXPSTR * prjstr, DLLIST * reflist)
{
    DLNODE *nd = dll_first(reflist);
    while (nd) {
        append_reference(prjstr, (HSCREF *) dln_data(nd));
        nd = dln_next(nd);
    }
}

/* append ids defined within documents */
static VOID append_doc_iddefs(EXPSTR * prjstr, DLLIST * iddefs)
{
    DLNODE *nd = dll_first(iddefs);
    while (nd) {
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

/*
 * hsc_write_project_file
 *
 * store all project-data in a string and write it to
 * the file specified with hsc_set_project_filename()
 */
BOOL hsc_write_project_file(HSCPRC * hp)
{
    BOOL written = FALSE;

    if (!hp->fatal && hp->filename_process && hp->filename_project) {

        EXPSTR *prjstr = init_estr(256);
        DLNODE *nd = NULL;
        FILE *outfile = NULL;

        DIHP(fprintf(stderr, DHLP "update project file `%s'\n",
                     hp->filename_project));

        /* remove old info of current file */
        nd = find_dlnode(dll_first(hp->documents),
                         (APTR) hp->filename_process, cmp_document);
        if (nd) {
            del_dlnode(hp->documents, nd);
        } else {

            DIHP(fprintf(stderr, DHL "new file in project: `%s'\n",
                         hp->filename_process));
            /* TODO: show message "creating new entry in project file" */
        }

        /* append header information */
        append_header(prjstr);

        /* update project info of current file */
        append_document( prjstr, hp->document);

        /*
         * append all old project info of other files
         */
        nd = dll_first(hp->documents);
        while (nd) {
            HSCDOC *document = (HSCDOC *) nd->data;

            append_document(prjstr, document);

            nd = dln_next(nd);
        }

        DIHP(fprintf(stderr, DHLP "project file contains:\n%s", estr2str(prjstr)));

        /* write new project file */
        errno = 0;
        outfile = fopen(hp->filename_project, "w");
        if (outfile) {

            errno = 0;
            fwrite(estr2str(prjstr), sizeof(char),
                   estrlen(prjstr), outfile);

            if (errno) {
                DIHP(fprintf(stderr, DHLP "can't write project file\n"));
                /* TODO: show message "can't open project file" */
            } else
                written = TRUE;

        } else {
            DIHP(fprintf(stderr, DHLP "can't open project file for output\n"));
            /* TODO: show message "can't open project file" */
        }

        del_estr(prjstr);

    } else {

        D(fprintf(stderr, DHLP "no update project\n"));

    }

    return (written);
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
static VOID hsc_msg_project_corrupt(HSCPRC * hp, STRPTR descr)
{
    STRPTR prjtxt = "project-file corrupt";

    if (descr)
        hsc_message(hp, MSG_CORRUPT_PRJFILE, "%s (%s)", prjtxt, descr);
    else
        hsc_message(hp, MSG_CORRUPT_PRJFILE, "%s", prjtxt);
}
static int x2int(char c)
{
    if ((c >= '0') && (c <= '9'))
        return (c - '0');
    if ((c >= 'A') && (c <= 'F'))
        return (c - 'A' + 10);
    if ((c >= 'a') && (c <= 'f'))
        return (c - 'a' + 10);
    return (EOF);
}                               /*
                                 * read_long
                                 */
static ULONG read_ulong(HSCPRC * hp)
{
    INFILE *inpf = hp->inpf;
    ULONG num = 0;
    int ch;
    int digit = EOF;

    do {
        ch = infgetc(inpf);
        if (ch != ' ') {

            digit = x2int(ch);
            if (digit == EOF) {

                num = 0;

            } else
                num = (num << 4) + digit;

        }
        if (digit == EOF)
            hsc_msg_project_corrupt(hp, "hex digit expected");

    } while ((digit != EOF) && (ch != ' '));

    if (digit == EOF)
        num = 0;

    return (num);
}

/*
 * read_string:
 *
 * read string length, alloc mem and read string into it
 */
static STRPTR read_string(HSCPRC * hp)
{
    STRPTR dest = NULL;

    ULONG len = read_ulong(hp);

    if (len) {

        ULONG i;
        int ch = 'x';           /* dummy init */

        /* alloc mem */
        dest = umalloc((size_t) (len + 1));
        dest[len] = '\0';

        for (i = 0; ((i < len) && (ch != EOF)); i++) {

            ch = infgetc(hp->inpf);
            if (ch != EOF)
                dest[i] = ch;
            else
                hsc_msg_project_corrupt(hp, "string expected");

        }
        if (ch != EOF)
            dest[len] = 0;
        else {
            ufree(dest);
            dest = NULL;
        }
    }
    return (dest);
}

/*
 * read_caller: read file position
 */
static CALLER *read_caller(HSCPRC * hp)
{
    CALLER *caller = NULL;
    STRPTR callerid = infgetw(hp->inpf);

    if (callerid && !upstrcmp(callerid, ID_CALLER_STR)) {
        int ch = infgetc(hp->inpf);     /* skip blank */

        if (ch == ' ') {
            STRPTR fname = read_string(hp);
            caller = new_caller(fname, read_ulong(hp), read_ulong(hp));
            ufreestr(fname);
        } else {
            hsc_msg_project_corrupt(hp, "blank expected");
        }
    } else if (callerid && !strcmp(callerid, "\n")) {
        /* skip empty caller */
        inungetcw(hp->inpf);
        D(fprintf(stderr, DHL "skip EMPTY CALLER\n"));
    } else {
        hsc_msg_project_corrupt(hp, ID_CALLER_STR " expected");
    }
    return (caller);
}

/*
 * read_lf: read linefeed
 */
static BOOL read_lf(HSCPRC * hp)
{
    int ch = infgetc(hp->inpf);
    BOOL ok = TRUE;

    if (ch != '\n') {
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
static STRPTR read_command(HSCPRC * hp)
{
    STRPTR command;

    do {
        command = infgetw(hp->inpf);
    } while (command && !strcmp(command, "\n"));

    if (command) {
        /* skip blanks */
        infskip_ws(hp->inpf);
        DIHP(fprintf(stderr, DHLP "command `%s'\n", command));
    } else {
        DIHP(fprintf(stderr, DHLP "command EOF\n"));
    }

    return (command);
}

/*
 * read header
 */
static BOOL read_header(HSCPRC * hp)
{
    STRARR fileid[1 + sizeof(FILEID_HSCPRJ)];
    BOOL ok = FALSE;
    STRPTR cmd = NULL;
    size_t i;

    /* read fileid */
    for (i = 0; i < strlen(FILEID_HSCPRJ); i++) {
        int ch = infgetc(hp->inpf);

        fileid[i] = (UBYTE) ch;
    } fileid[i] = '\0';

    DIHP(fprintf(stderr, DHLP "fileid: `%s'\n", fileid));

    /* check fileid */
    if (!strcmp(fileid, FILEID_HSCPRJ)) {
        DIHP(fprintf(stderr, DHLP "fileid: `%s'\n", fileid));
        ok = read_lf(hp);
    } else {
        hsc_msg_project_corrupt(hp, "wrong file-id");
    }

    if (ok) {

        ok = FALSE;

        /* read version */
        cmd = read_command(hp);

        /* check version */
        if (cmd && !strcmp(cmd, LINE_VERSION_STR)) {

            ULONG version = read_ulong(hp);

            DIHP(fprintf(stderr, DHLP "version: %lu\n", version));

            if (version && (version <= 2)) {
                ok = read_lf(hp);
            } else {

                hsc_msg_project_corrupt(hp, "wrong version");

            }

        } else {

            hsc_msg_project_corrupt(hp, "unknown version");

        }

    }
    return (ok);
}

/*
 * hsc_read_project_file
 *
 * read project file
 */
BOOL hsc_read_project_file(HSCPRC * hp)
{
    BOOL ok = FALSE;

    if (hp->filename_project) {

        INFILE *inpf = NULL;

        DIHP(fprintf(stderr, DHLP "read project-file from `%s'\n",
                     hp->filename_project));

        hsc_status_file_begin(hp, hp->filename_project);

        hp->inpf = infopen(hp->filename_project, 0);
        inpf = hp->inpf;

        if (inpf) {

            if (read_header(hp)) {

                HSCDOC *document = NULL;
                STRPTR cmd = NULL;

                do {
                    cmd = read_command(hp);
                    if (cmd) {

                        if (!strcmp(cmd, LINE_REM_STR)) {

                            /* skip comment */
                            int ch;
                            DIHP(fprintf(stderr, DHLP "comment `"));
                            do {
                                ch = infgetc(inpf);
                                DIHP( {
                                     if (ch != '\n') {
                                     fprintf(stderr, "%c", ch);
                                     }
                                     }
                                );

                            } while ((ch != EOF) && (ch != '\n'));
                            DIHP(fprintf(stderr, "'\n"));

                        } else if (!strcmp(cmd, LINE_DOCUMENT_STR)) {

                            /* begin new DOCUMENT */
                            STRPTR docname = read_string(hp);
                            if (docname) {
                                document = new_document(docname);
                                app_dlnode(hp->documents, (APTR) document);
                                /* free mem allocated by read_string() */
                                ufree(docname);
                            }
                        } else if (!strcmp(cmd, LINE_SOURCE_STR)) {

                            /* assign SOURCE */
                            if (document) {
                                STRPTR sourcename = read_string(hp);
                                if (sourcename) {
                                    reallocstr(&(document->sourcename),
                                               sourcename);
                                    /* free mem allocated by read_string() */
                                    ufree(sourcename);
                                }
                            } else
                                hsc_msg_project_corrupt
                                    (hp, LINE_SOURCE_STR " without "
                                     LINE_DOCUMENT_STR);
                        } else if (!strcmp(cmd, LINE_TITLE_STR)) {

                            /* assign TITLE */
                            if (document) {
                                STRPTR titlename = read_string(hp);
                                if (titlename) {
                                    set_estr(document->title, titlename);
                                    /* free mem allocated by read_string() */
                                    ufree(titlename);
                                }
                            } else
                                hsc_msg_project_corrupt
                                    (hp, LINE_TITLE_STR " without "
                                     LINE_DOCUMENT_STR);
                        } else if (!strcmp(cmd, LINE_ID_STR)) {

                            /* append new ID */
                            if (document) {
                                STRPTR idname = read_string(hp);
                                if (idname) {
                                    HSCIDD *iddef =
                                    app_iddef(document, idname);
                                    iddef->caller = read_caller(hp);
                                    /* free mem allocated by read_string() */
                                    ufree(idname);
                                }
                            } else
                                hsc_msg_project_corrupt
                                    (hp, LINE_ID_STR " without "
                                     LINE_DOCUMENT_STR);

                        } else if (!strcmp(cmd, LINE_INCLUDE_STR)) {

                            /* store new INCLUDE */
                            if (document) {
                                STRPTR incname = read_string(hp);
                                if (incname) {
                                    HSCINC *inc =
                                    app_include(document, incname);
                                    inc->caller = read_caller(hp);
                                    /* free mem allocated by read_string() */
                                    ufree(incname);
                                }
                            } else
                                hsc_msg_project_corrupt
                                    (hp, LINE_INCLUDE_STR " without "
                                     LINE_DOCUMENT_STR);

                        } else if (!strcmp(cmd, LINE_REFERENCE_STR)) {

                            /* store new REFERENCE */
                            if (document) {
                                STRPTR refname = read_string(hp);
                                if (refname) {
                                    HSCREF *ref =
                                    app_reference(document, refname);
                                    /* read caller-info */
                                    ref->caller = read_caller(hp);
                                    /* free mem allocated by read_string() */
                                    ufree(refname);
                                }
                            } else
                                hsc_msg_project_corrupt
                                    (hp, LINE_REFERENCE_STR " without "
                                     LINE_DOCUMENT_STR);

                        } else {

                            /* unknown command */
                            hsc_msg_project_corrupt(hp, "unknown tag");

                        }
                    } else {
                        DIHP(fprintf(stderr, DHLP "EOF\n"));
                    }
                }
                while (cmd && !hp->fatal);

            }
            infclose(inpf);
            hp->inpf = NULL;

            if (!hp->fatal) {
                EXPSTR *msg = init_estr(32);
                set_estr(msg, hp->filename_project);
                app_estr(msg, ": project-file read");
                hsc_status_misc(hp, estr2str(msg));
                del_estr(msg);
                ok = TRUE;
            }
        } else {

            DIHP(fprintf(stderr, DHLP "can't open project-file from `%s'\n",
                         hp->filename_project));
            hsc_message(hp, MSG_NEW_PRJFILE,
                        "unable to open project-file from %q",
                        hp->filename_project);
            ok = TRUE;

        }

    } else {

        DIHP(fprintf(stderr, DHL "no project-file specified'\n"));
        ok = TRUE;

    }
    return (ok);
}

