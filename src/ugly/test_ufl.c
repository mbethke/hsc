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
 * test_file.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ufile.h"

VOID err_fcopy(CONSTRPTR from, CONSTRPTR to)
{
    fcopy_t ferr = fcopy(from, to);

    switch (ferr)
    {
    case FC_OK:
        /* ok */
        break;
    case FC_ERR_NoMemory:
        fprintf(stderr, "error copying \"%s\": out of memory\n", from);
        break;
    case FC_ERR_OpenInput:
        fprintf(stderr, "error opening \"%s\" for input: %s\n",
                from, strerror(errno));
        break;
    case FC_ERR_OpenOutput:
        fprintf(stderr, "error opening \"%s\" for output: %s\n",
                to, strerror(errno));
        break;
    case FC_ERR_Read:
        fprintf(stderr, "error reading \"%s\": %s\n",
                from, strerror(errno));
        break;
    case FC_ERR_Write:
        fprintf(stderr, "error writting \"%s\": %s\n",
                to, strerror(errno));
        break;
    default:
        fprintf(stderr, "error: class=%d, errno=%d\n", ferr, errno);
        break;
    }
}

int main(VOID)
{
#define FNAME_SOURCE "test_ufl.tmp"
#define FNAME_DEST   "tmp."

#define BUFSIZE_SOURCE ((128*1024)+1713)
    FILE *source = NULL;

    /* create source file */
    source = fopen(FNAME_SOURCE, "wb");
    if (source)
    {
        char *buf = (char *) umalloc(BUFSIZE_SOURCE);
        fwrite(buf, sizeof(char), BUFSIZE_SOURCE, source);
        ufree(buf);
        fclose(source);
    }
    else
    {
        perror("no source file");
        exit(1);
    }

    if (fexists(FNAME_SOURCE))
    {
        printf("%s: fsize=%ld\n", FNAME_SOURCE, getfsize(FNAME_SOURCE));
    }

    /* some errors */
    err_fcopy("noinput.tmp", "nodest.tmp");
    err_fcopy(FNAME_SOURCE, "%$no&de\\/?st.+^*tm;#p");
#if 0
    /* try to write to read-only device */
    err_fcopy(FNAME_SOURCE, "cd0:" FNAME_SOURCE);
#endif

    /* these should work */
    err_fcopy("test_ufl.c", FNAME_DEST "1");
    err_fcopy(FNAME_SOURCE, FNAME_DEST "2");

    /* remove source file */
    remove(FNAME_SOURCE);
    remove(FNAME_DEST);
    remove(FNAME_DEST ".1");
    remove(FNAME_DEST ".2");

    return (0);
}
