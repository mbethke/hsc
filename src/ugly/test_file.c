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
#include "infile.h"

#define GETSTRLEN 10

int main(int argc, char *argv[])
{
    INFILE *inpf;
    INFILEPOS *pos1 = NULL;
    INFILEPOS *pos2 = NULL;

    char *fname;
    char ch;
    size_t i;
    char getstr[GETSTRLEN + 1];
    char *wdbuf;

#if DEBUG_UGLY_MEMORY
    atexit(atexit_uglymemory);
#endif

    if (argc == 1)
        fname = "hugo.txt";
    else
        fname = argv[1];

    errno = 0;
    inpf = infopen(fname, 0);

    if (inpf)
    {

#if 1
        /* info about first word */
        wdbuf = infgetw(inpf);
        printf("word#0: \"%s\", \"%s\"\n",
               infgetcws(inpf), infgetcw(inpf));

        /*
         * get 10 chars, print them,
         * and put them back again
         */
        ch = '*';               /* dummy */
        for (i = 0; i < GETSTRLEN; i++)
            getstr[i] = infgetc(inpf);

        getstr[i] = '\0';
        printf("got : \"%s\"\nback: \"", getstr);

        for (; i > 0; i--)
        {
            ch = getstr[i - 1];
            printf("%c", ch);
            inungetc(ch, inpf);
        }
        printf("\"\n");
#endif

        /*
         * read the first 3 words
         */
        for (i = 0; i < 3; i++)
        {
            wdbuf = infgetw(inpf);
            printf("word#%lu: [%lu] \"%s\"\n",
                   (ULONG) i, (ULONG) strlen(wdbuf), wdbuf);
        }

        /* info about current word */
        printf("current word: \"%s\", \"%s\"\n",
               infgetcws(inpf), infgetcw(inpf));

        /*
         * unget last word
         */
        inungetcw(inpf);
        wdbuf = infgetw(inpf);
        printf("reword: [%lu] \"%s\"\n", (ULONG) strlen(wdbuf), wdbuf);

        /* remember current position */
        pos1 = new_infilepos(inpf);

        /*
         * read and print rest of file
         */

        while (!infeof(inpf))
        {

            ch = infgetc(inpf);

            if (!infeof(inpf))
            {
                printf("%c", ch);
                fflush(stdout); /* flush output -> slow */
            }

        }

        pos2 = new_infilepos(inpf);

        printf("pos#1: %s (%lu,%lu)\n",
               ifp_get_fname(pos1),
               ifp_get_x(pos1),
               ifp_get_y(pos1));
        del_infilepos(pos1);

        infclose(inpf);

        printf("pos#2: %s (%lu,%lu)\n",
               ifp_get_fname(pos2),
               ifp_get_x(pos2),
               ifp_get_y(pos2));
        del_infilepos(pos2);

    }
    else
        perror("Can't open input file");

    /* test a string file */
    inpf = infopen_str("test string", "this is\n a string\n converted\n to an\n input file.", 0);
    if (inpf)
    {

        wdbuf = infgetw(inpf);
        while (wdbuf)
        {

            printf("%s%s", infgetcws(inpf), infgetcw(inpf));
            wdbuf = infgetw(inpf);
        }

        infclose(inpf);
    }
    else
        perror("Can't open input file from string");

    printf("\n");

    return (0);
}
