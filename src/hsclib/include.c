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
 * hsclib/include.c
 *
 * hsc include functions
 */

#include <time.h>

#include "hsclib/inc_base.h"

#include "ugly/ufile.h"
#include "ugly/fname.h"

#include "hsclib/input.h"
#include "hsclib/parse.h"

#define NOEXTERN_HSCLIB_INCLUDE_H
#include "hsclib/include.h"

/*
 * hsc_include
 *
 * read from inpf, parse for hsc-commands and execute them,
 * check for html-error,
 * write all out to outf and close input file.
 *
 * params: inpfnm...input file name
 *         outf.....output file structure, already opended
 *
 * result: TRUE, if all worked well, else FALSE
 */
static BOOL hsc_include(HSCPRC * hp, INFILE * inpf, ULONG optn, INFILEPOS * base_pos)
{
    BOOL ok;                    /* result */
    ok = (inpf != NULL);

    if (optn & IH_POS_PARENT)
        panic("IH_POS_PARENT set");

    if (inpf) {                  /* file opened? */
        /* push current input file on input-file-stack */
        if (hp->inpf)
            add_dlnode(hp->inpf_stack, (APTR) hp->inpf);

        /* set new base position for input-file */
        /* (if called from a macro or after eg. <$source>) */
        if (base_pos)
            set_infile_base(inpf, base_pos);

        /* assign new input file to hsc-process */
        hp->inpf = inpf;

        /* hide status? */
        if (optn & IH_PARSE_MACRO)
            optn |= IH_NO_STATUS;

        /* set char-parse methods */
        inpf->is_nc = hsc_normch;       /* set is_nc-methode */
        inpf->is_ws = hsc_whtspc;       /* set is_ws-methode */

        /* status message: reading new file */
        if (!(optn & IH_NO_STATUS) && !infeof(inpf))
            hsc_status_file_begin(hp, infget_fname(hp->inpf));

        /* parse file */
        while (!infeof(inpf) && ok) {
            if (!(optn & IH_NO_STATUS) &&
                (hp->prev_status_line != infget_y(hp->inpf)))
            {
                /* status message */
                hsc_status_line(hp);
                hp->prev_status_line = infget_y(hp->inpf);
            }

            /* parse next item */
            ok = (optn & IH_PARSE_SOURCE) ? hsc_parse_source(hp) : hsc_parse(hp);
        }

        /* parse at end: check for missing tags, .. */
        if (ok && (optn & IH_PARSE_END)) {
           /* parse end (unclosed tags etc) */
            ok = hsc_parse_end(hp);

            if (ok && (optn & IH_UPDATE_PRJ)) {
                /* update project file */
                ok = hsc_parse_end_id(hp);
            }
        }

        /* end of file status */
        if (!(optn & IH_NO_STATUS)) {
            /* status message: file processed */
            hsc_status_file_end(hp);
        }

        /* close file */
        infclose(hp->inpf);

        /* pull previous input file from input-file-stack
         * or end hsc-process */
        if (hp->inpf_stack->first) {
            /* pull first item from stack */
            hp->inpf = (INFILE *) hp->inpf_stack->first->data;
            hp->inpf_stack->first->data = NULL;

            del_dlnode(hp->inpf_stack, hp->inpf_stack->first);
        } else {
            hp->inpf = NULL;
        }
    } else {
        panic("no input file");
    }

    return (ok);
}

/*
 * hsc_include_file
 *
 * open input file and include it
 */
BOOL hsc_base_include_file(HSCPRC * hp, STRPTR filename, ULONG optn, INFILEPOS * base_pos)
{
    BOOL ok = FALSE;
    INFILE *inpf = NULL;
    EXPSTR *fpath = init_estr(32);

    /* status message: reading input */
    if (!(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)))
        hsc_status_file_begin(hp, filename);

    /* check for stdin to use as input-file */
    if (!(strcmp(filename, FILENAME_STDIN1) &&
          strcmp(filename, FILENAME_STDIN2))) {
        filename = NULL;
    } else {
       /* this is a real file -- try to get path */
       
       get_fpath(fpath,filename);
       /* if there is a path, add it to the list of include directories */
       if(0 != estrlen(fpath))
          hsc_add_include_directory(hp,estr2str(fpath));
    }

    /* open & read input file */
    errno = 0;
    inpf = infopen(filename, ES_STEP_INFILE);

    if (inpf) {
        /* include opened file */
        ok = hsc_include(hp, inpf, optn, base_pos);

        /* check if this file is the main-source-file
         * or an include-file and update project-data
         * if neccessary
         */
        if (ok && hp->project) {
            if (optn & IH_IS_SOURCE) {
                if (!filename)
                    filename = FILENAME_STDIN1;
                D(fprintf(stderr, DHL "INCLUDE source: `%s'\n", filename));
                hsc_project_set_source(hp->project, filename);
                /*reallocstr(&(hp->document->sourcename), filename); */
            }

            /* check if this file is an include-file
             * and update project-data if neccessary */
            if (filename && (optn & IH_IS_INCLUDE)) {
                D(fprintf(stderr, DHL "INCLUDE subfile: `%s'\n", filename));
                hsc_project_add_include(hp->project, filename);
            }
        }
    } else
        hsc_msg_noinput(hp, filename);  /* couldn't open file */

    /* if we got a path part earlier, remove the corresponding incdir node */
    if(0 != estrlen(fpath)) {
       del_dlnode(hp->include_dirs,dll_last(hp->include_dirs));
    }
    del_estr(fpath);
    return (ok);
}

/*
 * hsc_include_string
 *
 * open string as input file and include it
 */
BOOL hsc_base_include_string(HSCPRC * hp, STRPTR filename, STRPTR s, ULONG optn, INFILEPOS * base_pos)
{
    BOOL ok;
    INFILE *inpf = NULL;

    if (optn & IH_POS_PARENT) {
        filename = PARENT_FILE_ID;
        optn &= ~IH_POS_PARENT;
    }
    inpf = infopen_str(filename, s, 0);         /* try to open input file */

    ok = hsc_include(hp, inpf, optn, base_pos);

    return (ok);
}

/*
 * find_includefile
 *
 * scan all include-directories for file to be opened;
 * if the file can't be found, the filename in the current
 * directory will be returned (which should therefor result
 * in an error while processing hsc_include())
 *
 * NOTE: this function considers the file to be found if it could have
 *       been opened for input. afterwards, the file is immediatly
 *       closed. This isn't really a multitasking-conform behavior
 *       because the file will have to be reopend again by the
 *       hsc_include() function later and meanwhile could have been
 *       removed by another task.
 */
static BOOL find_includefile(HSCPRC *hp, EXPSTR * dest, STRPTR filename)
{
    BOOL found = FALSE;

    /* reset filename */
    set_estr(dest, filename);

    if (!fexists(filename)) {
        DLNODE *nd = dll_first(hp->include_dirs);

        /* process all include-directories.. */
        while (nd && !found) {
            /* concat incdir+filename, check if it exists,
             * process next or abort loop */
            link_fname(dest, (STRPTR) dln_data(nd), filename);
            D(fprintf(stderr, DHL "  try `%s'\n", estr2str(dest)));
            if (fexists(estr2str(dest)))
                found = TRUE;
            else
                nd = dln_next(nd);
        }
    } else {
        /* found in current directory */
        found = TRUE;
    }

    if (found)
    {
        D(fprintf(stderr, DHL "  found at `%s'\n", estr2str(dest)));
    }

    return (found);
}

/* hsc_include_file: include file without base-position */
BOOL hsc_include_file(HSCPRC * hp, STRPTR filename, ULONG optn)
{
    BOOL ok = FALSE;
    EXPSTR *real_filename = init_estr(64);

    if(NULL == filename) {
       fprintf(stderr,"hsc_include_file(): NULL/empty filename!\n");
       return ok;
    }
    
    /* scan include directories */
    find_includefile(hp, real_filename, filename);

    /* now include file */
    ok = hsc_base_include_file(hp, estr2str(real_filename), optn, NULL);

    /* cleanup */
    del_estr(real_filename);

    return ok;
}

/* hsc_include_string: include string without base-position */
BOOL hsc_include_string(HSCPRC * hp, STRPTR filename, STRPTR s, ULONG optn)
{
    return (hsc_base_include_string(hp, filename, s, optn, NULL));
}

/* $Id$ */
/* vi: set ts=4: */
