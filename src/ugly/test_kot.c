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
 * ugly/test_kot.c - test unicode funtions
 */

#include <stdio.h>

#include "ugly/unikot.c"

/*
 * printf_ch
 *
 * print single char on stdout, if it can be displayed;
 * otherwise, print a dot (.)
 */
static void printf_ch(utf8_t ch)
{
    if ((ch >= 32) && !((ch >= 128) && (ch <= 159)))
    {
        printf("%c", ch);
    }
    else
    {
        printf(".");
    }
}

static void printf_unikot(const char *title, const ucs4_t ucs4,
                          const utf8_t * utf8str, const int skip)
{
    int i = 0;

    /* display title and UCS4 code */
    printf("%s: %08x --> `", title, ucs4);

    /* display UTF8-string */
    for (i = 0; i < skip; i += 1)
    {
        utf8_t ch = utf8str[i];
        printf_ch(ch);
    }
    printf("' -->");

    /* display UTF-8 string (hex) */
    for (i = 0; i < skip; i++)
    {
        printf(" %02x", utf8str[i]);
    }
    printf("\n");

}

/*
 * do_utf8_example
 */
static void do_utf8_example(char *name, const utf8_t * utf8str)
{
    size_t skip = 0;
    ucs4_t ucs4;

    printf("\nutf8_to_ucs4: process example `%s'\n", name);

    do
    {
        skip = utf8_to_ucs4(&ucs4, utf8str);
        if (skip > 0)
        {
            printf_unikot("utf8_to_ucs4", ucs4, utf8str, skip);
            utf8str += skip;
        }
        else
        {
            printf("utf8_to_ucs4: endcoding error\n");
        }
    }
    while ((skip > 0) && utf8str[0]);
}

/*
 * do_ucs4_example
 */
static void do_ucs4_example(char *name, const ucs4_t * ucs4str)
{
    int i = 0;
    utf8_t buffer[7];

    printf("\nucs4_to_utf8: process example `%s'\n", name);

    while (ucs4str[i])
    {
        int skip = 0;
        ucs4_t ucs4 = ucs4str[i];

        skip = ucs4_to_utf8(buffer, ucs4);
        buffer[skip] = 0;       /* terminate string */

        printf_unikot("ucs4_to_utf8", ucs4, buffer, skip);

        i += 1;
    }
}

#define TEST_utf8_to_ucs4 1
#define TEST_ucs4_to_utf8 1

/*
 * main
 */
int main(int argc, char *argv[])
{
#if TEST_utf8_to_ucs4
    {
        /* some example texts in UTF-8 */
        const utf8_t utf8_hi_mom[] =
        {
            0x48, 0x69, 0x20, 0x4d, 0x6f, 0x6d,
            0x20, 0xe2, 0x98, 0xba, 0x21, 0x0
        };
        const utf8_t utf8_nihongo[] =
        {
            0xe6, 0x97, 0xa5, 0xe6, 0x9c,
            0xac, 0xe8, 0xaa, 0x9e, 0x0
        };

        /* test examples for utf8-to-ucs4 */
        do_utf8_example("hi mom :-) !", utf8_hi_mom);
        do_utf8_example("nihongo", utf8_nihongo);
    }
#endif

#if TEST_ucs4_to_utf8
    {
        /* some example texts in UCS4 */
        const ucs4_t ucs4_hi_mom[] =
        {
            0x0048, 0x0069, 0x0020, 0x004d, 0x006f,
            0x006d, 0x0020, 0x263a, 0x0021, 0x0000
        };
        const ucs4_t ucs4_nihongo[] =
        {
            0x65e5, 0x672c, 0x8a9e, 0x0000
        };

        const ucs4_t ucs4_a_neq_alpha[] =
        {
            0x0041, 0x2262, 0x0391, 0x002e, 0x0000
        };

        /* test examples for ucs4-to-ucs4 */
        do_ucs4_example("hi mom :-) !", ucs4_hi_mom);
        do_ucs4_example("nihongo", ucs4_nihongo);
        do_ucs4_example("a<>alpha", ucs4_a_neq_alpha);
    }
#endif
    return 0;
}
