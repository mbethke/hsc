/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
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
 * ugly/ufile.c
 *
 * misc. filename functions
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
 * updated:  6-Jan-1997
 * created: 14-Oct-1996
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_UFILE_H
#include "ufile.h"

/*
 * fexists
 *
 * check if file exists ( = could have been opened for input)
 *
 * result: TRUE, if file exists, else FALSE
 *
 */
BOOL fexists(STRPTR filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
    }
    return ((BOOL) (file != NULL));
}

/*
 * getfsize
 *
 * get size of file
 *
 * params: filename...path and name of file to examine
 * result: size of file; if file does not exist, result
 *         is 0, and ``errno'' is set
 */
LONG getfsize(CONSTRPTR filename)
{
    FILE *file = fopen(filename, "rb");
    LONG filesize = 0;
    if (file)
    {
        /* retrieve size */
        fseek(file, 0L, SEEK_END);
        filesize = ftell(file);
        fclose(file);
    }
    return (filesize);
}

/*
 * fcopy
 *
 * copy file (binary)
 *
 * params: oldname,newname...filename of input/output files
 * result: value of type ``fcopy_t'', see "ugly/ufile.h" for details
 *
 * NOTE: you can use ``errno'' to obtain details about the error
 */
fcopy_t fcopy(CONSTRPTR oldname, CONSTRPTR newname)
{
#define BUFSIZE_FCOPY (16*1024)
#define SETERR(code) {err = (code); nowErrno = errno; }
    fcopy_t err = FC_OK;        /* function result */
    FILE *inpf = fopen(oldname, "rb");  /* input file */
    FILE *outf = fopen(newname, "wb");  /* output file */
    char *buf = (char *) umalloc(sizeof(char) * BUFSIZE_FCOPY);         /* copy buffer */
    int nowErrno = 0;

    /* check for errors */
    if (!buf)
    {
        SETERR(FC_ERR_NoMemory);
    }
    else if (!inpf)
    {
        SETERR(FC_ERR_OpenInput);
    }
    else if (!outf)
    {
        SETERR(FC_ERR_OpenOutput);
    }
    else
    {
        /* copy data */
        BOOL quit = FALSE;
        do
        {
            size_t byteRead = fread(buf, sizeof(char), BUFSIZE_FCOPY, inpf);

            if (ferror(inpf))
            {
                SETERR(FC_ERR_Read);
            }
            else if (byteRead == 0)
            {
                quit = TRUE;
            }
            else
            {
                fwrite(buf, sizeof(char), byteRead, outf);
                if (ferror(outf))
                {
                    SETERR(FC_ERR_Write);
                }
            }
        }
        while (!quit && (err == FC_OK));
    }

    /* cleanup */
    if (inpf)
        fclose(outf);
    if (inpf)
        fclose(inpf);
    if (buf)
        ufree(buf);

    /* set errno to value where error occured first */
    if (nowErrno)
        errno = nowErrno;

    return err;
}

/*
 * fmove
 *
 * rename or move file; act's same as rename(), but is guaranteed
 * to work accross devices
 *
 * params: oldname...source filename
 *         newname...destination filename
 * result: same as fcopy()
 */
fcopy_t fmove(CONSTRPTR oldname, CONSTRPTR newname)
{
    fcopy_t ferr = FC_OK;

    if (rename(oldname, newname))
    {
        int nowErrno = 0;

        /* rename failed; perform a copy-and-delete */
        ferr = fcopy(oldname, newname);
        nowErrno = errno;       /* remember errno */

        switch (ferr)
        {
        case FC_OK:
            remove(oldname);    /* remove old file */
            break;

        case FC_ERR_Write:
            remove(newname);    /* remove (incomplete) new file */
            errno = nowErrno;
            break;

        case FC_ERR_NoMemory:
        case FC_ERR_OpenInput:
        case FC_ERR_OpenOutput:
        case FC_ERR_Read:
            /* do nufin */
            break;

        default:
            panic("unhandled return value");
            break;
        }
    }

    return ferr;
}
