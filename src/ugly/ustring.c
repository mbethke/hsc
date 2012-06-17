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
 * ugly/ustring.c - ugly string functions
 *
 * updated: 18-May-1997
 * created: 31-Jul-1993
 */

#include "sysdep.h"
#include <ctype.h>
#include <stdio.h>
#include "utypes.h"
#include "umemory.h"

#define NOEXTERN_UGLY_USTRING_H
#include "ustring.h"

/*
 * last_ch
 *
 * return last char of a string, or 0 on empty string
 */
int last_ch(CONSTRPTR s)
{
    size_t len = strlen(s);

    if (len)
        return (int)(s[len - 1]);
    else
        return 0;
}

/*
 * strclone
 *
 * create a clone of a string
 *     (copy of string using its own memory area)
 *
 * params: *oldstr...ptr to string to clone
 * result: ptr to copy of string;
 * errors: out of memory: returns NULL
 *
 */
STRPTR ugly_strclone(CONSTRPTR oldstr, STRPTR file, ULONG line) {
    STRPTR newstr = NULL;

    if (oldstr) {
        /* alloc mem for clone */
#if DEBUG_UGLY_MEMORY
        newstr = (STRPTR) ugly_malloc_tracking(strlen(oldstr) + 1, file, line);
#else
        newstr = (STRPTR) umalloc(strlen(oldstr) + 1);
#endif

        if (newstr)             /* alloc sucessful? */
           strcpy(newstr, oldstr);     /* Y-> copy data */
        else
           panic("out of memory while trying to clone string");
    }
    return (newstr);            /* return result */
}

/*
 * lowstr: convert a string to lowercase
 *
 * params: *s...string to convert
 *
 */
STRPTR lowstr(STRPTR s) {
    STRPTR s_old = s;

    if (s)
        for (; *s != '\0'; s++)
            *s = tolower(*s);

    return s_old;
}
/*
 * upstr: convert a string to uppercase
 *
 * params: *s...string to convert
 *
 */
STRPTR upstr(STRPTR s) {
    STRPTR s_old = s;

    if (s)
        for (; *s != '\0'; s++)
            *s = toupper(*s);

    return s_old;
}

/*
 * upstrcmp
 *
 * string compare, ignore case
 *
 * params: *s1, *s2...ptr to strings to compare
 * result: <0: s1 < s2
 *          0: s1 = s2
 *         >0: s1 > s2
 *
 */
int upstrcmp(CONSTRPTR s1, CONSTRPTR s2) {
    unsigned char c1, c2;       /* chars currently comparing */
    size_t i = 0;               /* string index counter */

    do {
        c1 = toupper(s1[i]);
        c2 = toupper(s2[i]);
        ++i;
    } while (c1 && c2 && (c1 == c2));
    return (c2 - c1);
}

/*
 * upstrncmp
 *
 * string compare, ignore case, max. n chars
 *
 * params: *s1, *s2...ptr to strings to compare
 *         n..........max. nr. of chars to compare
 * result: -1: s1 < s2
 *          0: s1 = s2
 *          1: s1 > s2
 *
 */
int upstrncmp(CONSTRPTR s1, CONSTRPTR s2, size_t n) {
   unsigned char c1, c2;       /* char of string currently comparing */
   size_t i = 0;               /* string index counter */

   do {
      c1 = toupper(s1[i]);
      c2 = toupper(s2[i]);
      ++i;
   } while (c1 && c2 && (c1 == c2) && (i < n));
   return c2-c1;
}

/*
 * upstrstr
 *
 * find a sub-string (case insensitive)
 *
 */
STRPTR upstrstr(CONSTRPTR s1, CONSTRPTR s2) {
    const char *c1;
    const char *c2;

    do {
        c1 = s1;
        c2 = s2;

        while (*c1 != '\0' && (toupper(c1[0]) == toupper(c2[0]))) {
            ++c1;
            ++c2;
        }

        if (*c2 == '\0')
            return (char *) s1;
    } while (*s1++ != '\0');
    return NULL;
}

/*
 * freestr
 *
 * free memory used by a string
 *
 * params: s..string to free
 *
 * NOTE: strings created with strclone() or reallocstr
 *       should be freed with this function instead of
 *       ufree(). this will avoid problems when using
 *       the memory-tracking feature of ugly.o
 */
void ugly_freestr(STRPTR s, STRPTR file, ULONG line) {
#if DEBUG_UGLY_MEMORY
    ugly_free(s, file, line);
#else
    ufree(s);
#endif
}

/*
 * reallocstr
 *
 * free memory used by string, clone new data
 *
 * params:oldstr...prt to ptr of old string
 *          *newstr...ptr to new string data
 * errors: out of mem: returns NULL (old data are lost)
 *
 * IMPORTANT: old string has to be initialies like "oldstr = NULL;" before !!
 *            first call like "reallocstr( &oldstr, "<new data>" );"
 */
void ugly_reallocstr(STRPTR * oldstr, CONSTRPTR newstr, STRPTR file, ULONG line) {
#if DEBUG_UGLY_MEMORY
    ugly_freestr(*oldstr, file, line);  /* free old string */
    *oldstr = ugly_strclone(newstr, file, line);        /* clone new string */
#else
    ufree(*oldstr);             /* free old string */
    *oldstr = strclone(newstr); /* clone new string */
#endif
}

/*
 * ch2str
 *
 * convert single char to zero terminated string
 *
 * params: ch...char to convert
 * result: ptr to internal buffer, where a string containning
 *         "<ch>\0" is located.
 *
 * USAGE : buffer is rewritten at next call of _chstr(), so
 *         be sure to clone string if you need it for a longer
 *         time.
 *         _ch2str() is mostly designed for parameter conversion
 *         on string funtions, e.g s = strclone( ch2str( 'x' ) );
 *
 */
STRPTR ch2str(const char ch) {
    static char ch2str_buffer[2];       /* internal buffer */

    ch2str_buffer[0] = ch;
    ch2str_buffer[1] = '\0';

    return ch2str_buffer;
}

/*
 * ustrrpbrk
 *
 * searches for last occurence of a char of _set in _str
 *
 * params: str...string to examine
 *         set...chars to search for
 * result: ptr where char of _set occured in _str
 *         or NULL if none occurence found
 *
 * NOTE:   this is a clone of none-ansi-c function _strrpbrk()
 *
 */
STRPTR ustrrpbrk(CONSTRPTR str, CONSTRPTR set) {
    size_t i;
    STRPTR result = NULL;

    if (str && ('\0' != str[0])) {

        i = strlen(str) - 1;

        while ((i) && (strchr(set, str[i]) == NULL))
            i--;

        if (strchr(set, str[i]))
            result = (STRPTR) & (str[i]);
    }
    return result;
}

/*
 * str2long
 *
 * convert string expression to long value
 *
 * params: s....string to convert
 *         num..ptr to destination long var
 * result: TRUE, if sucessful
 * errors: returen FALSE
 *
 */
BOOL str2long(CONSTRPTR s, LONG * num) {
    BOOL conv_ok = FALSE;

    if (sscanf(s, "%d", (int *) num))
        conv_ok = TRUE;
    return conv_ok;
}

/*
 * long2str
 *
 * convert long value to string expresion
 *
 * params: num...value to convert
 * result: ptr to converted string
 * errors: returns NULL
 *
 */
STRPTR long2str(LONG num) {
    static char num2str_buffer[10];     /* internal buffer */

    STRPTR result_str = NULL;

    if (sprintf(num2str_buffer, "%d", (int) num))
        result_str = num2str_buffer;
    return result_str;
}

/*
 * strenum
 *
 * find a substring in an enumerator string
 *
 * params: str...substring to search for
 *         set...set of legal substrings
 *         sep...substring separator (eg "|")
 * result:  0..not found
 *         >0..index of substring found
 *         <0..out of mem
 * errors: out of mem: returns -1
 *
 * example: enumstr( "sepp", "hugo|sepp|resi", '|', STEN_CASE ) -> 2
 *
 * NOTE: calls strtok()
 */
LONG strenum(CONSTRPTR str, CONSTRPTR set, char sep, BYTE options)
{
    STRPTR s = strclone(set);
    LONG found = 0;

    if (s) {

        STRPTR nxtstr = strtok(s, ch2str(sep));
        LONG count = 1;

        while (!found && nxtstr) {

            if (options & STEN_NOCASE) {
                if (!upstrcmp(str, nxtstr))
                    found = count;
            } else if (!strcmp(str, nxtstr))
                found = count;

            count++;
            nxtstr = strtok(NULL, ch2str(sep));
        }
        ufreestr(s);
    } else
        found = -1;
    return (found);
}
