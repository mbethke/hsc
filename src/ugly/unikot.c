/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
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
 * ugly/unikot.c - unicode support functions
 */

#include "ugly/utypes.h"
#include "ugly/unikot.h"

/* debuggin define */
#if 0
#define DUC(x) x
#else
#define DUC(x)
#endif
#define UNIKOT "*unikot* "

/*
 * display internal error message
 *
 * c - condition which causes error
 * m - message text to discibe error
 */
#define internal_error(c,m)                           \
    if ((c))                                          \
    {                                                 \
        unikot_internal_error(__FILE__, __LINE__, m); \
    }

#if 0
static void unikot_internal_error(char *file, long int line, char *message) {
    fprintf(stderr, "*** internal error at `%s' (%ld): %s\n",
            file, line, message);
    exit(255);
}
#endif

/**** ugly/utf8_to_ucs4 *********************************************
 * NAME
 *   uft8_to_ucs4 - convert UTF-8-string to  UTF-4-code (32 bit)
 * SYNOPSIS
 *   skip = utf8_to_ucs4(ucs4    , utf8_t  , skip)
 *   int    utf8_to_ucs4(ucs4_t *, utf8_t *, size_t *)
 * FUNCTION
 *   Attempts to convert the beginning of an UTF-8-encoded string to
 *   an UCS-4-code. This will only convert the next character(s) of
 *   the string. The result tell how many characters were needed.
 *   Encoding errors can be handled.
 * INPUTS
 *   ucs4 - target variable which will hold UCS-4-code
 *   utf8 - start of UTF-8-string to be converted
 * RESULT
 *   Denotes how many chars of UTF-8-string were used for the
 *   calculation of the UCS-4-code. This can be used to increase the
 *   string pointer and call this routine again.
 *
 *   If there was an error in the UTF-8 encoding, this value will be
 *   zero or negative.
 *******************************************************************/
int utf8_to_ucs4(ucs4_t * ucs4, const utf8_t * utf8str) {
    int skip = 1;

    if (utf8str[0] >= 192) {
        /* blimy! This must be an extended char */
        char ch_to_read = 0;
        utf8_t i = utf8str[0];
        ucs4_t first_ch_mask = 0x3f;

        while (i & 0x40) {
            ch_to_read += 1;
            i = (i << 1) & 0x7f;
            first_ch_mask = first_ch_mask >> 1;
        }

        *ucs4 = utf8str[0] & first_ch_mask;

        DUC(fprintf(stderr, "%02x: %d chars, mask=0x%02x\n",
                    utf8str[0], ch_to_read, first_ch_mask));

        do {
            utf8_t ch = 0;

            /* get next char */
            ch = utf8str[skip];

            if ((ch & 0xc0) == 0x80) {
                /* add it to to current ucs4-value */
                *ucs4 = (*ucs4 << 6) + (ch & 0x3f);
                ch_to_read -= 1;
                skip += 1;
            } else {
                /* encoding error */
                DUC(fprintf(stderr, UNIKOT "*** encoding error: 0x%02x != 10xxxxxx\n", ch));
                skip = ERR_UNIKOT_LATER_CHAR;
            }
        }
        while (ch_to_read && (skip > 0));
    } else if (utf8str[0] >= 128) {
        /* encoding error */
        DUC(fprintf(stderr, UNIKOT "*** encoding error: 128<ch<192\n"));
        skip = ERR_UNIKOT_FIRST_CHAR;
    } else {
        /* it is just a simple ASCII-char */
        *ucs4 = utf8str[0];
        skip = 1;
    }
    return skip;
}

/**** ugly/ucs4_to_utf8 *********************************************
 * NAME
 *   ucs4_to_utf8 - convert a ucs4-value to a utf8-string
 * SYNOPSIS
 * FUNCTION
 *   Converts a 31-bit UCS-4 code into a UTF-8 string.
 *   The string will be stored in a buffer specified by the user
 *   No trailing zero will be stored.
 * INPUTS
 *   utf8 - target string where the UTF-8 string will be written to
 *          (without a trailing zero). Therefore, this buffer must
 *          at least be able to store six characters, or seven, if
 *          you want to be able to append a '\0' after the call.
 *   ucs4 - source UCS-4 code
 * RESULT
 *   Denotes how many chars of UTF-8-string were used to store the
 *   result. This can be used to increase the string pointer and call
 *   this routine again, or to expand or flush the buffer
 *******************************************************************/
int ucs4_to_utf8(utf8_t * utf8str, ucs4_t ucs4) {
    int skip = 0;
    if (ucs4 < 128) {
        /* it is just a simple ASCII-char */
        utf8str[0] = (utf8_t) ucs4;
        skip = 1;
    } else {
        /* blimey! This must be an extended char */
        static const utf8_t first_char_mask[] = {
            0xc0,               /* 110xxxxx */
            0xe0,               /* 1110xxxx */
            0xf0,               /* 11110xxx */
            0xf8,               /* 111110xx */
            0xfc                /* 1111110x */
        };

        int i = 0;
        utf8_t first_char = 0;
        utf8_t ch = 0;

        /*
         * Quoted from RFC 2044:
         *
         *   UCS-4 range (hex.)    UTF-8 octet sequence (binary)
         *   0000 0000-0000 007F   0xxxxxxx
         *   0000 0080-0000 07FF   110xxxxx 10xxxxxx
         *   0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx
         *   0001 0000-001F FFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
         *   0020 0000-03FF FFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
         *   0400 0000-7FFF FFFF   1111110x 10xxxxxx ... 10xxxxxx
         */
        if (ucs4 <= 0x0000ffff)
            skip = (ucs4 <= 0x007ff) ? 2 : 3;
        else if (ucs4 < 0x03ffffff)
            skip = (ucs4 <= 0x001fffff) ? 4 : 5;
        else
            skip = 6;

        first_char = first_char_mask[skip - 2];

        DUC(fprintf(stderr, UNIKOT "0x%08lx: ch=%d fmsk=%02x\n",
                    ucs4, skip, first_char));

        /* compute every but first character */
        i = skip - 1;
        while (i > 0) {
            ch = 0x80 + (ucs4 & 0x3f);
            ucs4 = ucs4 >> 6;
            utf8str[i] = ch;
            DUC(fprintf(stderr, UNIKOT "  i=%d ch=%02x\n", i, ch));
            --i;
        }

        DUC(internal_error(first_char & ucs4, "bits overlapping"));

        /* compute first character */
        first_char = first_char | ucs4;
        utf8str[0] = first_char;
    }
    return skip;
}

