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
 * ugly/expstr.c
 *
 * ugly expandable-string functions
 *
 * updated: 26-Feb-1997
 * created: 12-Sep-1995
 *
 */

/****** ugly.lib/--background-- **********************************************
*
*   PURPOSE
*       It is a known fact, that the programming langage C comes with one of
*       the most useless string concept ever.
*
*       This function library implements a simple concept to use strings
*       which allocate new memory themself if data will not fit in the
*       current anymore.
*
*       All operations work with a internal structure EXPSTR, which is
*       private and must not be accessed by the programmer. For all
*       interessting information, access functions exist. (See estrlen()
*       and estr2str().)
*
*       An interface with standard strings is provided. (See estr2str() and
*       set_estr().)
*
*   EXAMPLE
*
*       A sample session of including and using some of the functions of
*       this collection can be found below:
*
*       #include "ugly/expstr.h"
*       ...
*
*       EXPSTR * sepp = init_estr( 0 );          \* an expandable string *\
*       EXPSTR * hugo = init_estr( 0 );          \* an expandable string *\
*       STRPTR resi = "this is resi";            \* a standard string *\
*       BOOL ok = ((BOOL) (sepp && hugo);        \* result variable *\
*
*       if (ok)
*       {
*           ok &= set_estr( sepp, "sepp" );      \* set data *\
*           ok &= set_estr( hugo, "hugo" );
*
*           ok &= app_estr( sepp, ". ");         \* append data *\
*           ok &= app_estr( hugo, " and ");      
*           ok &= estrcat( hugo, sepp );         \* ("hugo and sepp.") *\
*
*           ok &= clr_estr( sepp );              \* clear sepp *\
*
*           printf( "sepp=%s\nhugo=%s\n",        \* output data *\
*                   estr2str( sepp ), estr2str( hugo ));
*       }
*
*       if (!ok)
*       {
*           printf( "out of resources\n" );
*       }
*
*       del_estr( hugo );                        \* cleanup (in any case) *\
*       del_estr( sepp );
*
*   NOTES
*       Internally, expandable string still contain a zero-character ('\0')
*       at the end of data.
*
*   BUGS
*       Currently, expandable strings can not hold data which contain a
*       zero-character ('\0'). This is because at several locations I have
*       used functions like strcpy() instead of memcpy().
*
*       Currently, strings expand very stupid in a linear way and not
*       exponentially. That means, if a string runs out of memory,
*       it will only increase by a constant size, instead of eg. increase
*       to twice as large as before. Altough this also works, it is quite
*       slow for long strings.
*
******************************************************************************
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "ustring.h"
#include "umemory.h"

#define NOEXTERN_UGLY_STRING_H
#include "expstr.h"

#if DEBUG_UGLY_EXPSTR
#define D(x) x
#define DEXP "*expstr* "
#else
#define D(x)                    /* nufin */
#endif

/* some functions that perform consistency checks and panic if necessary */
static void es_null(STRPTR func, STRPTR file, ULONG line)
{
    fprintf(stderr, "\n##\n## panic: es=NULL in %s()\n##   called from %s (%lu)\n##\n",
            func, file, line);
}

static void s_null(STRPTR func, STRPTR file, ULONG line)
{
    fprintf(stderr, "\n##\n## panic: string=NULL in %s()\n##   called from %s (%lu)\n##\n",
            func, file, line);
}

/*
 *-------------------------------------
 * set/clear expstr
 *-------------------------------------
 */

/*
 * set_estr_mem
 *
 * set new mem for es_data
 *
 * params: es........expstr where to assign the new mem to
 *         new_size..new size for data part
 * result: TRUE if all ok, new mem in es->es_data;
 * errors: return FALSE, leave es->es_data untouched
 *
 */
static BOOL do_set_estr_mem(EXPSTR * es, STRPTR new_data, size_t new_size)
{
    BOOL ok = TRUE;

    if (new_data)
    {

#if DEBUG_UGLY_EXPSTR == 2
        D(fprintf(stderr, DEXP "set to %lu (%p->%p)\n",
                  new_size, es->es_data, new_data));
#endif
        es->es_size = new_size;
        es->es_data = new_data;

    }
    else
        ok = FALSE;

    return (ok);
}

BOOL ugly_set_estr_mem(EXPSTR * es, size_t new_size)
{
    return (do_set_estr_mem(es, (STRPTR) umalloc(new_size), new_size));
}

BOOL ugly_dbg_set_estr_mem(EXPSTR * es, size_t new_size, STRPTR file, ULONG line)
{
    BOOL ok = FALSE;

    if (!es)
        es_null("set_estr_mem", file, line);
    else
    {
        ok = do_set_estr_mem(es,
                             (STRPTR) ugly_malloc_tracking(new_size, file, line),
                             new_size);
    }

    return (ok);
}

/****** ugly.lib/set_estr() **************************************************
*
*   NAME
*       set_estr -- set new data for an expandable string.
*
*   SYNOPSIS
*       ok = set_estr( string, data )
*
*       BOOL set_estr
*            ( EXPSTR * string, STRPTR data )
*
*   FUNCTION
*       Reset data of an expandable string to an empty string (""). If
*       necessary, the buffer to store the string data will be expanded.
*
*   INPUTS
*       string - string to clear
*       data   - new data to store, represented as a normal zero-terminated
*                C-string
*
*   RESULT
*       ok - TRUE, if string could be updated. If there have not been enough
*            resources available to allocate a new buffer for the new data,
*            it will still contain the old data, and FALSE will be returned.
*
*   EXAMPLE
*       ok = set_estr( sepp, "data for sepp" );
*
*   SEE ALSO
*       app_estr(), estrcpy()
*
******************************************************************************
*/
BOOL ugly_set_estr(EXPSTR * es, CONSTRPTR s)
{
    BOOL ok = FALSE;
    size_t new_len = strlen(s) + 1;
    STRPTR old_data = es->es_data;

    if ((es->es_size == es->es_step)
        && (es->es_size > new_len))
    {
        strcpy(es->es_data, s); /* copy new data */
        es->es_len = new_len;   /* set new len */
        ok = TRUE;
    }
    else if (set_estr_mem(es, modadj(new_len, es->es_step)))
    {

        strcpy(es->es_data, s); /* copy new & release old data */
        ufreestr(old_data);

        es->es_len = new_len;   /* set new len */
        ok = TRUE;
    }
    return (ok);
}

BOOL ugly_dbg_set_estr(EXPSTR * es, CONSTRPTR s, STRPTR file, ULONG line)
{
    BOOL ok = FALSE;

    if (!es)
        es_null("set_estr_mem", file, line);
    else if (!s)
        s_null("set_estr_mem", file, line);
    else
    {
        size_t new_len = strlen(s) + 1;
        STRPTR old_data = es->es_data;

#if DEBUG_UGLY_EXPSTR == 2
        uglymem_wallcheck("setestr()", file, line);
#endif

        if ((es->es_size == es->es_step)
            && (es->es_size > new_len))
        {

            strcpy(es->es_data, s);     /* copy new data */
            es->es_len = new_len;       /* set new len */
            ok = TRUE;

        }
        else if (ugly_dbg_set_estr_mem(es, modadj(new_len, es->es_step), file, line))
        {

            strcpy(es->es_data, s);     /* copy new & release old data */
            ufree(old_data);

            es->es_len = new_len;       /* set new len */
            ok = TRUE;
        }
#if DEBUG_UGLY_EXPSTR == 2
        uglymem_wallcheck("setestr()", file, line);
#endif

    }

    return (ok);
}

/****** ugly.lib/clr_estr() **************************************************
*
*   NAME
*       clr_estr -- clear data of an expandable string.
*
*   SYNOPSIS
*       ok = clr_estr( string )
*
*       BOOL clr_estr
*            ( EXPSTR * string )
*
*   FUNCTION
*       Reset data of an expandable string to an empty string (""). This
*       will reset the amount of memory to be used by this string to the
*       initial size passed with init_estr().
*
*   INPUTS
*       string - string to clear
*
*   RESULT
*       ok - TRUE, if string could be cleared. If there have not been enough
*            resources available to allocate a new buffer for the empty
*            string, it will still contain the old data, and FALSE will be
*            returned.
*
*   EXAMPLE
*       ok = clr_estr( sepp );
*
*   SEE ALSO
*       init_estr(), set_estr()
*
******************************************************************************
*/
BOOL ugly_clr_estr(EXPSTR * es)
{
    return (set_estr(es, ""));
}

BOOL ugly_dbg_clr_estr(EXPSTR * es, STRPTR file, ULONG line)
{
#if DEBUG_UGLY_EXPSTR == 2
    STRPTR s = es->es_data;
    if (!s)
        s = "<null>";
    fprintf(stderr, DEXP "clr_estr(%p,`%s')\n", es, s);
    uglymem_wallcheck("clr_estr()", file, line);
#endif
    return (ugly_dbg_set_estr(es, "", file, line));
}

/*
 * set_estrn
 *
 * set expstr with first n chars of string
 */
BOOL set_estrn(EXPSTR * es, CONSTRPTR s, size_t n)
{
    BOOL ok = FALSE;
    STRPTR s1 = NULL;
    size_t len = strlen(s);

    if (n > len)
        n = len;

    s1 = (STRPTR) umalloc(n + 1);
    if (s1)
    {

        memcpy(s1, s, n);
        s1[n] = 0;
        ok = set_estr(es, s1);
        ufree(s1);

    }
    return (ok);
}

/*
 *-------------------------------------
 * constructor / destructor
 *-------------------------------------
 */

/****** ugly.lib/init_estr() *************************************************
*
*   NAME
*       init_estr -- create a new expandable string.
*
*   SYNOPSIS
*       new_string = init_estr( initial_size )
*
*       EXPSTR * init_estr
*            ( size_t initial_size )
*
*   FUNCTION
*       Creates a new expandable string. Initialises the data that it
*       represents an empty string.
*
*   INPUTS
*       initial_size -
*           Initial size to use for the string. This amount of memory
*           will immdediately be allocated and is used to store the
*           string data. If the string grows and data will not fit in
*           this memory area any more, new memory will be allocated
*
*           If a value of `0' is used, a reasonable value will be
*           used. This usualy depend on your OS, but will not be
*           smaller than 8.
*
*   RESULT
*       new_string - pointer to new string, or NULL if not enough
*                    memory available
*
*   EXAMPLE
*       EXPSTR * sepp = init_estr( 20 );
*
*   SEE ALSO
*       del_estr(), clr_estr(), set_estr()
*
******************************************************************************
*/

EXPSTR *ugly_dbg_init_estr(size_t step_size, STRPTR file, ULONG line)
{
    EXPSTR *es = (EXPSTR *) ugly_malloc_tracking(sizeof(EXPSTR), file, line);

    if (es)
    {

        if (step_size < ES_MIN_MEMSTEP)
            step_size = ES_MIN_MEMSTEP;
        es->es_data = NULL;
        es->es_size = 0;
        es->es_step = step_size;
        if (!clr_estr(es))
        {

            ufree(es);
            es = NULL;

        }
    }
    return (es);

}

EXPSTR *ugly_init_estr(size_t step_size)
{
    EXPSTR *es = (EXPSTR *) umalloc(sizeof(EXPSTR));

    if (es)
    {

        if (step_size < ES_MIN_MEMSTEP)
            step_size = ES_MIN_MEMSTEP;
        es->es_data = NULL;
        es->es_size = 0;
        es->es_step = step_size;
        if (!clr_estr(es))
        {
            ufree(es);
            es = NULL;
        }
    }
    return (es);

}

/****** ugly.lib/del_estr() **************************************************
*
*   NAME
*       del_estr -- release all resources used by an expandable string
*
*   SYNOPSIS
*       del_estr( string )
*
*       VOID * init_estr
*            ( EXPSTR * string )
*
*   FUNCTION
*       Releases all resources allocated by an expandable string. The data
*       are no more valid afterwards and must not be accessed any more.
*
*   INPUTS
*       string - expandable string to remove
*
*   EXAMPLE
*       del_estr( sepp );
*
*   SEE ALSO
*       init_estr()
*
******************************************************************************
*/
VOID del_estr(EXPSTR * es)
{
#if DEBUG_UGLY_EXPSTR
    if (es)
    {
        if (es->es_data)
        {
#if DEBUG_UGLY_EXPSTR == 2
            STRARR s[17];
            strncpy(s, es->es_data, 17);
            s[16] = 0;
            D(fprintf(stderr, DEXP "del_estr(%p,`%s')\n", es, s));
            umem_wallcheck("del_estr()");
#endif
        }
        else
        {
            D(fprintf(stderr, DEXP "attempt to free null-data-estr\n"));
        }
    }
    else
    {
#if DEBUG_UGLY_EXPSTR == 2
        D(fprintf(stderr, DEXP "attempt to free null-estr\n"));
#endif
    }
#endif

    if (es)
    {

        ufree(es->es_data);
        es->es_len = 0;
        es->es_size = 0;
        es->es_step = 0;
        ufree(es);

    }
}

/****** ugly.lib/app_estrch() **************************************************
*
*   NAME
*       app_estrch -- append single character to an expandable string.
*
*   SYNOPSIS
*       ok = app_estrch( string, ch )
*
*       BOOL app_estrch
*            ( EXPSTR * string, int ch )
*
*   FUNCTION
*       At the end of a string, a single character will be appended.
*
*   INPUTS
*       string - string to which data should be appended
*       ch     - character to append
*
*   RESULT
*       ok - TRUE, if string could be updated. If there have not been enough
*            resources available to allocate a new buffer for the new data,
*            it will still contain the old data, and FALSE will be returned.
*
*   EXAMPLE
*       ok = app_estrch( sepp, 'x' );
*
*   BUGS
*       Result is undefined, if (ch > 255) or (ch < 0).
*
*   SEE ALSO
*       app_estr()
*
******************************************************************************
*/
BOOL ugly_app_estrch(EXPSTR * es, int ch) {
    BOOL ok = TRUE;

    if (es->es_len >= es->es_size) { /* enough mem left? */

        STRPTR old_data = es->es_data;  /* N->remeber old data ptr */

        if (set_estr_mem(es, es->es_size + es->es_step)) {
           /* set new mem sucessful? */

            strcpy(es->es_data, /*    Y->copy old data */
                   old_data);
            ufree(old_data);    /*       release old data */

        } else {
            /*    N->return error */
            ok = FALSE;
        }
    }
    if (ok) {

        STRPTR s;
        s = es->es_data;
        s[es->es_len - 1] = ch; /* append new char to expstr */
        s[es->es_len] = 0;
        es->es_len++;           /* incr. expstr length */

    }
    return (ok);
}

BOOL ugly_dbg_app_estrch(EXPSTR * es, int ch, STRPTR file, ULONG line) {
    BOOL ok = TRUE;

    if (!es) {
        es_null("app_estrch", file, line);
        ok = FALSE;
    } else if (es->es_len >= es->es_size) { /* enough mem left? */

        STRPTR old_data = es->es_data;  /* N->remeber old data ptr */

        if (ugly_dbg_set_estr_mem(es, es->es_size + es->es_step, file, line)) {
            /*    set new mem sucessfully? */

            strcpy(es->es_data, /*    Y->copy old data */
                   old_data);
            ufree(old_data);    /*       release old data */
        } else {                       /*    N->return error */
            ok = FALSE;
        }
    }
    if (ok) {
        STRPTR s;
        s = es->es_data;
        s[es->es_len - 1] = ch; /* append new char to expstr */
        s[es->es_len] = 0;
        es->es_len++;           /* incr. expstr length */
    }
    return (ok);
}

/****** ugly.lib/app_estr() **************************************************
*
*   NAME
*       app_estr -- append data to an expandable string.
*
*   SYNOPSIS
*       ok = app_estr( string, data )
*
*       BOOL app_estr
*            ( EXPSTR * string, STRPTR data )
*
*   FUNCTION
*       At the end of a string, new data will be appended.
*
*   INPUTS
*       string - string to which data should be appended
*       data   - new data to append, represented as a normal zero-terminated
*                C-string
*
*   RESULT
*       ok - TRUE, if string could be updated. If there have not been enough
*            resources available to allocate a new buffer for the new data,
*            it will still contain the old data, and FALSE will be returned.
*
*   EXAMPLE
*       ok = app_estr( sepp, "data to append" );
*
*   SEE ALSO
*       estrcat(), set_estr(), estrcpy()
*
******************************************************************************
*/
BOOL ugly_app_estr(EXPSTR * es, CONSTRPTR s)
{
    BOOL ok = TRUE;
    size_t slen = strlen(s);

    ok = TRUE;
    if ((es->es_len + slen - 1) >= es->es_size)
    {                           /* enough mem left? */

        STRPTR old_data = es->es_data;  /* N->remeber old data ptr */

        if (ugly_set_estr_mem(es,
                              modadj(es->es_len + slen + 1, es->es_step)))
        {
            /*    set new mem sucessful? */
            strcpy(es->es_data, /*    Y->copy old data */
                   old_data);
            ufree(old_data);    /*       release old data */
        }
        else
        {                       /*    N->return error */
            ok = FALSE;
        }
    }
    if (ok)
    {

        STRPTR ds;
        ds = es->es_data + (es->es_len - 1);
        strcat(ds, s);
        /* append new char to expstr */
        es->es_len += slen;     /* incr. expstr length */
        es->es_data[es->es_len - 1] = 0;

    }
    return (ok);
}

BOOL ugly_dbg_app_estr(EXPSTR * es, CONSTRPTR s, STRPTR file, ULONG line)
{
    BOOL ok = FALSE;

    if (!es)
        es_null("app_estr", file, line);
    else if (!s)
        s_null("app_estr", file, line);
    else
    {
        /* faster, but maybe buggy */
        size_t slen = strlen(s);

        ok = TRUE;
        if ((es->es_len + slen - 1) >= es->es_size)
        {                       /* enough mem left? */

            STRPTR old_data = es->es_data;      /* N->remeber old data ptr */

            if (ugly_dbg_set_estr_mem(es,
                    modadj(es->es_len + slen + 1, es->es_step), file, line))
            {                   /*    set new mem sucessful? */

                strcpy(es->es_data,     /*    Y->copy old data */
                       old_data);
                ufree(old_data);        /*       release old data */
            }
            else
            {                   /*    N->return error */
                ok = FALSE;
            }
        }
        if (ok)
        {

            STRPTR ds;
            ds = es->es_data + (es->es_len - 1);
            strcat(ds, s);
            /* append new char to expstr */
            es->es_len += slen; /* incr. expstr length */
            es->es_data[es->es_len - 1] = 0;

        }
    }

    return (ok);
}

/*
 *-------------------------------------
 * get part of expstr
 *-------------------------------------
 */

/*
 * todo: handle special cases like
 * get_right_estr( .., "hugo", 99 );
 */

/*
 * get_mid_estr
 *
 * get a part from a expstr; compare BASIC's "MID$()"
 *
 * params: dest...destination expstr where to store result part
 *         src....source expstr where to get part from
 *         from...position of char where to begin part (0=first char)
 *         num....number of chars to get
 * result: TRUE and result in dest if ok; else FALSE is returned an
 *         dest is left untouched
 *
 * NOTE: it is possible to use the source-exstr as destination-expstr,
 *       because the result is copied to a temp. expstr before
 *       ( example: get_mid_estr( hugo, hugo, 3, 4 ); )
 */
BOOL get_mid_estr(EXPSTR * dest, EXPSTR * src, size_t from, size_t num)
{
    BOOL ok = FALSE;
    EXPSTR *tmp = init_estr(dest->es_step);

    if (tmp)
    {

        STRPTR old_data = tmp->es_data;

        /* check size */
        if (from >= src->es_len)
            from = src->es_len - 1;
        if (from + num >= src->es_len)
            num = src->es_len - from - 1;

        /* set new mem for tmp */
        ok = set_estr_mem(tmp, modadj(num + 1, tmp->es_step));

        if (ok)
        {

            /* copy data */
            strncpy(estr2str(tmp), estr2str(src) + from, num);
            tmp->es_data[num] = 0;
            tmp->es_len = num + 1;
            ufree(old_data);

            ok = estrcpy(dest, tmp);

        }
        del_estr(tmp);

    }
    return (ok);
}

/*
 * get_right_estr
 *
 * get right part from a expstr; compare BASIC's "RIGHT$()"
 */
BOOL get_right_estr(EXPSTR * dest, EXPSTR * src, size_t num)
{
    if (num >= src->es_len)
        num = src->es_len - 1;

    return (get_mid_estr(dest, src, (src->es_len - num - 1), num));
}

/*
 * get_left_estr
 *
 * get left part from a expstr; compare BASIC's "LEFT$()"
 */
BOOL get_left_estr(EXPSTR * dest, EXPSTR * src, size_t num)
{
    return (get_mid_estr(dest, src, 0, num));
}

/*
 *-------------------------------------
 * misc. functions
 *-------------------------------------
 */

/****** ugly.lib/estr2str() *************************************************
*
*   NAME
*       estr2str -- convert expandable string to standard string
*
*   SYNOPSIS
*       s = estr2str( string )
*
*       STRPTR estr2str
*            ( EXPSTR * string )
*
*   FUNCTION
*       Copy data from a source string (old) to a destination string (new).
*
*   INPUTS
*       string - expandable string to convert; if you pass NULL, this
*                function crashes.
*
*   RESULT
*       s - pointer to data of string, terminated with a zero-character
*           ('\0')
*
*   EXAMPLE
*       EXPSTR * hugo = init_estr(0);
*       STRPTR data;
*       ...
*       data = estr2str( hugo );
*
*   SEE ALSO
*       set_estr(), estrlen()
*
******************************************************************************
*/
STRPTR ugly_estr2str(EXPSTR * es)
{
    return (es->es_data);
}

/****** ugly.lib/estrlen() *************************************************
*
*   NAME
*       estrlen -- dtermines length of an expandable string
*
*   SYNOPSIS
*       len = estrlen( string )
*
*       size_t estrlen
*            ( EXPSTR * string )
*
*   FUNCTION
*       This functions determines the number of characters stored in an
*       expandable string.
*
*   INPUTS
*       string - expandable string to examine; if you pass NULL, this
*                function crashes.
*
*   RESULT
*       len - length of string
*
*   EXAMPLE
*       EXPSTR * hugo = init_estr(0);
*       size_t len;
*       ...
*       len = estrlen( hugo );
*
*   NOTES
*       As expandable strings always keep track of their current length,
*       this functions always executes in O(1) versus classic strlen(),
*       which always has to examine the whole string and therefor always
*       executes in O(n), with n = number of characters stored.
*
*   SEE ALSO
*       estr2str()
*
******************************************************************************
*/
size_t ugly_estrlen(EXPSTR * es)
{
    return (es->es_len - 1);
}

/****** ugly.lib/estrcpy() **************************************************
*
*   NAME
*       estrcpy -- copy data of an expandable string
*
*   SYNOPSIS
*       ok = estrcpy( new, old )
*
*       BOOL estrcpy
*            ( EXPSTR * new, EXPPTR * old )
*
*   FUNCTION
*       Copy data from a source string (old) to a destination string (new).
*
*   INPUTS
*       new - destination string
*       old - source string
*
*   RESULT
*       ok - TRUE, if string could be updated. If there have not been enough
*            resources available to allocate a new buffer for the new data,
*            it will still contain the old data, and FALSE will be returned.
*
*   EXAMPLE
*       EXPSTR * hugo = init_estr(0);
*       EXPSTR * sepp = init_estr(0);
*       BOOL ok;
*       ...
*       ok = estrcpy( sepp, hugo );
*
*   SEE ALSO
*       estrcat(), set_estr()
*
******************************************************************************
*/
BOOL estrcpy(EXPSTR * dest, EXPSTR * src)
{
    return (set_estr(dest, estr2str(src)));
}

/****** ugly.lib/estrcat() **************************************************
*
*   NAME
*       estrcat -- concatenate two expandable strings
*
*   SYNOPSIS
*       ok = estrcat( dest, app )
*
*       BOOL estrcat
*            ( EXPSTR * dest, EXPPTR * app )
*
*   FUNCTION
*       Append data of an string to another one and store the result in it.
*
*   INPUTS
*       dest -
*       app -
*
*   RESULT
*       ok - TRUE, if string could be updated. If there have not been enough
*            resources available to allocate a new buffer for the new data,
*            it will still contain the old data, and FALSE will be returned.
*
*   EXAMPLE
*       EXPSTR * hugo = init_estr(0);
*       EXPSTR * sepp = init_estr(0);
*       BOOL ok;
*       ...
*       ok = estrcat( sepp, hugo );
*
*   SEE ALSO
*       app_estr(), estrcpy()
*
******************************************************************************
*/
BOOL estrcat(EXPSTR * dest, EXPSTR * src)
{
    return (app_estr(dest, estr2str(src)));
}
