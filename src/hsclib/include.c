/*
 * hsclib/include.c
 *
 * hsc include functions
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
 * updated:  6-Sep-1996
 * created: 19-Feb-1996
 */

#include <time.h>

#include "hsclib/inc_base.h"

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
    {
        panic("IH_POS_PARENT set");
    }

    if (inpf)
    {                           /* file opened? */
        /* push current input file on input-file-stack */
        if (hp->inpf)
            ins_dlnode(hp->inpf_stack, hp->inpf_stack->first, (APTR) hp->inpf);

        /* set new base position for input-file */
        /* (if called from a macro or after eg. <$source>) */
        if (base_pos)
            set_infile_base(inpf, base_pos);

        /* assign new input file to hsc-process */
        hp->inpf = inpf;

        /* hide status? */
        if ((optn & IH_PARSE_MACRO) || (optn & IH_PARSE_MACRO))
            optn |= IH_NO_STATUS;

        /* set char-parse methods */
        inpf->is_nc = hsc_normch;       /* set is_nc-methode */
        inpf->is_ws = hsc_whtspc;       /* set is_ws-methode */

        /* status message: reading new file */
        if (!(optn & IH_NO_STATUS) && !infeof(inpf))
            hsc_status_file_begin(hp, infget_fname(hp->inpf));

        while (!infeof(inpf) && ok)
        {                       /* parse file */
            if (!(optn & IH_NO_STATUS) &&       /* status message */
                (hp->prev_status_line != infget_y(hp->inpf))
                )
            {
                hsc_status_line(hp);
                hp->prev_status_line = infget_y(hp->inpf);
            }
            /* parse next item */
            if (optn & IH_PARSE_SOURCE)
                ok = hsc_parse_source(hp);
            else
                ok = hsc_parse(hp);
        }

        /* parse at end: check for missing tags, .. */
        if (ok && (optn & IH_PARSE_END))
        {                       /* parse end (unclosed tags etc) */
            ok = hsc_parse_end(hp);

            if (ok && (optn & IH_UPDATE_PRJ))
                ok = hsc_parse_end_id(hp);      /* update project file */
        }

        /* end of file status */
        if (!(optn & IH_NO_STATUS))
        {
            hsc_status_file_end(hp);    /* status message: file processed */
        }
        infclose(hp->inpf);     /*    close file */

        /* pull previous input file from input-file-stack
         * or end hsc-process
         */
        if (hp->inpf_stack->first)
        {
            /* pull first item from stack */
            hp->inpf = (INFILE *) hp->inpf_stack->first->data;
            hp->inpf_stack->first->data = NULL;

            del_dlnode(hp->inpf_stack, hp->inpf_stack->first);
        }
        else
        {

            hp->inpf = NULL;
        }

    }
    else
        panic("no input file");

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

    /* status message: reading input */
    if (!(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)))
    {
        hsc_status_file_begin(hp, filename);
    }

    /* check for stdin to use as input-file */
    if ( !strcmp(filename, FILENAME_STDIN))
        filename = NULL;

    /* open & read input file */
    errno = 0;
    inpf = infopen(filename, ES_STEP_INFILE);

    if (inpf)
    {
        /* include opened file */
        ok = hsc_include(hp, inpf, optn, base_pos);

        /* check if this file is the main-source-file
         * or an include-file and update project-data
         * if neccessary
         */
        if (ok && hp->project)
        {
            if (optn & IH_IS_SOURCE)
            {
                if (!filename)
                    filename = FILENAME_STDIN;
                D(fprintf(stderr, DHL "INCLUDE source: `%s'\n", filename));
                hsc_project_set_source(hp->project, filename);
                /*reallocstr(&(hp->document->sourcename), filename); */
            }

            /* check if this file is an include-file
             * and update project-data if neccessary */
            if (filename && (optn & IH_IS_INCLUDE))
            {
                D(fprintf(stderr, DHL "INCLUDE subfile: `%s'\n", filename));
#if 1
                hsc_project_add_include(hp->project, filename);
#else /* TODO: remove */
                HSCINC *inc = app_include(hp->document, filename);
                INFILEPOS *fpos = NULL;
                if (hp->inpf)
                    fpos = new_infilepos(hp->inpf);
                inc->caller = fpos2caller(fpos);
                if (fpos)
                    del_infilepos(fpos);
#endif
            }
        }
    }
    else
    {
        hsc_msg_noinput(hp, filename);  /* couldn't open file */
    }

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

    if (optn & IH_POS_PARENT)
    {
        filename = PARENT_FILE_ID;
        optn &= ~IH_POS_PARENT;
    }
    inpf = infopen_str(filename, s, 0);         /* try to open input file */

    ok = hsc_include(hp, inpf, optn, base_pos);

    return (ok);
}

/* hsc_include_file: include file without base-position */
BOOL hsc_include_file(HSCPRC * hp, STRPTR filename, ULONG optn)
{
    return (hsc_base_include_file(hp, filename, optn, NULL));
}

/* hsc_include_string: include string without base-position */
BOOL hsc_include_string(HSCPRC * hp, STRPTR filename, STRPTR s, ULONG optn)
{
    return (hsc_base_include_string(hp, filename, s, optn, NULL));
}

