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
 * ugly/fname.c - filename processing functions
 *
 * updated: 23-Aug-1998
 * created: 24-May-1994
 *
 *-------------------------------------------------------------------
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_FNAME_H
#include "fname.h"

/*
 * get_fext
 *
 * get filename extension
 *
 * params: fn...full filename to examine
 * result: extension of _fn or NULL, if fn was also NULL
 *
 */
BOOL get_fext(EXPSTR * dest, CONSTRPTR fn)
{
    CONSTRPTR fn_ext = fn;      /* result var */

    /* search for end of string */
    while (fn_ext[0])
        fn_ext++;

    /* search string backwards for "." or PATH_SEPARATOR */
    do
    {
        /* nufin */ ;
    }
    while ((fn_ext != fn)       /* beginning of name reached? */
           && (fn_ext--)        /* process next char */
           && (fn_ext[0] != '.')
           && (strchr(PATH_SEPARATOR, fn_ext[0]) == NULL));

    /* copy extension to dest */
    if (fn_ext[0] == '.')
        set_estr(dest, ++fn_ext);
    else
        clr_estr(dest);

    return (ok_fnl_fext(dest));
}

/*
 * get_fname
 *
 *
 */
BOOL get_fname(EXPSTR * dest, CONSTRPTR fn)
{
    CONSTRPTR fn_name = fn;

    fn_name = ustrrpbrk(fn_name, PATH_SEPARATOR);

    /* copy extension to dest */
    if (fn_name)
        set_estr(dest, ++fn_name);
    else
        set_estr(dest, fn);

    return (ok_fnl_fname(dest));
}

/*
 * get_fpath
 *
 *
 */
BOOL get_fpath(EXPSTR * dest, CONSTRPTR fn)
{
    STRPTR pa_name = ustrrpbrk(fn, PATH_SEPARATOR);

    if (pa_name)
    {
        set_estrn(dest, fn, strlen(fn) - strlen(pa_name) + 1);
    }
    else
        clr_estr(dest);

    return (ok_fnl_fpath(dest));
}

/*
 * get_fsdir: get next subdirectory (including separator)
 *
 */
BOOL get_fsdir(EXPSTR * dest, CONSTRPTR fn)
{
    STRPTR fn_name = strpbrk(fn, PATH_SEPARATOR);

    if (fn_name)
    {
        set_estrn(dest, fn, strlen(fn) - strlen(fn_name) + 1);
    }
    else
        clr_estr(dest);

    return (ok_fnl_fpath(dest));
}

/*
 * get_fdrive
 *
 *
 */
BOOL get_fdrive(EXPSTR * dest, CONSTRPTR fn)
{
    set_estr(dest, fn);         /* dummy data */
    clr_estr(dest);
    return (ok_fnl_fpath(dest));
}

/*
 * fextidx: get filename extension index of string
 *
 * params: fn...filename to examine
 * result: index in _fn, where extension starts
 *
 * NOTE: if _fn has an extension at all, _fn[i] is equal to '.', else
 *       _fn has no extension (e.g. "TESTFILE") and _fn[i] is equal
 *       to '\0'.
 *
 * (internal function)
 *
 */
static size_t fextidx(CONSTRPTR fn)
{
    size_t i;                   /* string index counter */

    i = strlen(fn) - 1;         /* scan string backwards... */
    while ((i) && (fn[i] != '.')
           && (strchr(PATH_SEPARATOR, fn[i]) == NULL))
        i--;
    if (fn[i] != '.')
        i = strlen(fn);

    return i;                   /* return result */
}

/*
 * clr_fext: clear filename extension
 *
 * params: dest...string that contains filename to strip
 *
 * EXAMPLE: "testfile.txt" -> "testfile"
 *
 */
BOOL clr_fext(EXPSTR * dest)
{
    BOOL ok;
    size_t extidx = fextidx(estr2str(dest));
    /* index, where extension starts */
    ok = set_estrn(dest, estr2str(dest), extidx);
    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * set_fext: set new (last) extension for filename
 *
 * params: dest....string that contains old filename
 *         newext..new extension to set
 *
 * EXAMPLE: "testfile.txt", "lha" -> "testfile.lha"
 *          "hugo.tar.gz" , "lha" -> "hugo.tar.lha"
 */
BOOL set_fext(EXPSTR * dest, CONSTRPTR newext)
{
    BOOL ok;

    ok = clr_fext(dest);
    ok &= app_estrch(dest, '.');
    ok &= app_estr(dest, newext);

    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * app_fext: append extension to filename
 *
 * EXAMPLE: ("testfile.txt", "lha") -> "testfile.txt.lha"
 *                                  -> "testfile.lha"     (msdos)
 */
BOOL app_fext(EXPSTR * dest, CONSTRPTR newext)
{
    BOOL ok = app_estrch(dest, '.');

    ok &= app_estr(dest, newext);
    ok &= ok_fnl_fpath(dest);

    return (ok);
}

/*
 * set_fextIdx: set file name index extension
 *
 * params: dest..destination string with new extension
 *         fn....source string with extension to replace
 *         idx...extension index
 */
BOOL set_fnameIdx(EXPSTR * dest, int idx)
{
    char fn_ext[12];            /* index extension */
    sprintf(fn_ext, "%03d", idx);       /* create index string */

    return (set_fext(dest, fn_ext));
}

/*
 * link_fname: link directory and filename together
 *
 * params: dest..where to store result
 *         dir...directoryname
 *         fn....filename to append
 *
 * NOTE: a PATHSEPARATOR[0] is append to dir, if none exists
 * NOTE: dir and fn MUST NOT be part of dest,
 *       when invoking this funtion
 */
BOOL link_fname(EXPSTR * dest, STRPTR dir, STRPTR fn)
{
    BOOL anydir;                /* TRUE, if any dir passed as arg */
    BOOL ok = TRUE;

    /* is a dir passed? */
    anydir = (dir != NULL);
    if (anydir)
        anydir = (strlen(dir) != 0);

    if (anydir)
    {
        /* clone dir, if any
         *
         * NOTE: it's neccesarry to work with a copy do `dir',
         * because if `dir' is part or `dest', this could lead
         * to a mungwall hit */
        STRPTR dir_clone = strclone(dir);       /* clone of `dir' */

        set_estr(dest, dir_clone);
        /* check, if the last char of dir is a path separator */
        /* ->if not, append a direcory separator */
        if (!strchr(PATH_SEPARATOR, dir_clone[strlen(dir_clone) - 1]))
            app_estrch(dest, DIR_SEPARATOR);

        /* free cloned dir */
        ufreestr(dir_clone);
    }
    else
        clr_estr(dest);

    /* append filename */
    if (fn)
        app_estr(dest, fn);

    ok &= ok_fnl_fpath(dest);

    return ok;
}

/*
 * link_envfname: link content of an environment variable,
 *                directory and filename together
 *
 * params: dest..where to store result
 *         env...name of environment variable
 *         dir...directoryname
 *         fn....filename to append
 *
 * result: FALSE, if envvar could not be found or
 *         filename got too long
 */
BOOL link_envfname(EXPSTR * dest, STRPTR envname, STRPTR dir, STRPTR fn)
{
    BOOL ok = FALSE;
    STRPTR env = getenv(envname);

    if (env)
    {
        STRPTR env1 = strclone(env);        /* copy envvar to own memory area */
        EXPSTR *tmpstr = init_estr(32);

        /* strip linefeeds from hscenv */
        while (strlen(env1) && (env1[strlen(env1)] == '\n'))
            env1[strlen(env1)] = '\0';

        if (dir)
        {
            link_fname(tmpstr, env1, dir);
        }
        else
        {
            set_estr(tmpstr, env1);
        }

        ok = link_fname(dest, estr2str(tmpstr), fn);

        del_estr(tmpstr);
        ufreestr(env1);
    }
    else
    {
        clr_estr(dest);
    }

    return ok;
}

/*
 * tmpnamstr: alloc & create a string with a temp. filename
 *
 * result: string containig filename;
 *         MUST be release using ufreestr() by caller
 * errors: return NULL; this can be because auf no more
 *         temp. files available or out of mem
 *
 * IMPORTANT: you need to copy the filename, if you
 *           call this function frquently
 */
static size_t adjust_prefix_length(size_t prefix_length)
{
#ifdef MSDOS
    if (prefix_length > MAX_FNAME - 4)
    {
        prefix_length = MAX_FNAME - 4;
    }
#else
    if (prefix_length > MAX_FNAME - 8)
    {
        prefix_length = MAX_FNAME - 8;
    }
#endif

    return (prefix_length);
}

STRPTR tmpnamstr(STRPTR suggested_prefix)
{
    static LONG fileidx = 0;
    static STRARR buf[256 + 32];
    STRPTR s = NULL;
    STRPTR prefix = suggested_prefix;
    FILE *file = NULL;
    size_t prefix_length = 0;

    if (prefix == NULL)
    {
        prefix = "";
    }

    /* copy prefix to buffer */
    prefix_length = adjust_prefix_length(strlen(prefix));
    strncpy(buf, prefix, prefix_length);
    buf[prefix_length] = '\0';

    /* add address of buffer to prefix */
    if (MAX_FNAME > 8)
    {
        sprintf(&(buf[strlen(buf)]), "%p", tmpnamstr);
    }

    prefix_length = strlen(buf);

    /* Try to open a file for input until fopen() fails,
     * which means a file with such a name does not yet exist.
     *
     * This still sucks as in multi-tasking systems the file
     * can be created while this function returns, but already
     * reduces the likelyhood that a file is reused. */
    do
    {
        fileidx += 1;
#ifdef RISCOS
        sprintf(&(buf[prefix_length]), "%04lx", fileidx);
#else
        sprintf(&(buf[prefix_length]), "%04lx.tmp", fileidx);
#endif
        file = fopen(buf, "r");
        if (file)
        {
            fclose(file);
            if (fileidx == 0xffff)
            {
                fileidx = 0;    /* ran out of names */
            }
        }
    }
    while (fileidx && file);

    if (fileidx != 0)
    {
        s = buf;
    }

    return (s);
}

/*
 * get_relfname: get relative filename, according to given path
 *
 * params: absn..absolute filename
 *         curp..current path
 *
 * EXAMPLE:
 *  "image/back.gif" and "image/hugo/" -> "/back.gif"
 *  "image/back.gif" and ""            -> "image/back.gif"
 *  "image/back.gif" and "people/"     -> "/image/back.gif"
 */
BOOL get_relfname(EXPSTR * dest, STRPTR absn, STRPTR curp)
{
    EXPSTR *fname = init_estr(32);      /* file name only */
    EXPSTR *abspa = init_estr(32);      /* absolute path only */
    EXPSTR *tmpp1 = init_estr(32);      /* temp pointer */
    EXPSTR *tmpp2 = init_estr(32);
    STRPTR rest_absp = NULL;    /* rest of current path */
    STRPTR absp = NULL;         /* path processing */
    int cmp_result;             /* stores result returned by upstrcmp */

    /* init string array */
    clr_estr(dest);
    get_fname(fname, absn);
    get_fpath(abspa, absn);
    absp = estr2str(abspa);

    /*
     * skip all equal subdirs
     */
    do
    {
        get_fsdir(tmpp1, absp);
        get_fsdir(tmpp2, curp);
        cmp_result = upstrcmp(estr2str(tmpp1), estr2str(tmpp2));

        if (!cmp_result)
        {
            absp += estrlen(tmpp1);
            curp += estrlen(tmpp2);
        }
    }
    while (estrlen(tmpp1)
           && estrlen(tmpp2)
           && (!cmp_result));

    /* remember equal part of path */
    rest_absp = absp;

    /*
     * for every subdir in absp unequal to
     * corresponding subdir curp, insert a parent dir
     */
    if (curp[0])
        do
        {
            get_fsdir(tmpp1, absp);
            get_fsdir(tmpp2, curp);
            cmp_result = upstrcmp(estr2str(tmpp1), estr2str(tmpp2));

            if (cmp_result)
            {
                absp += estrlen(tmpp1);
                curp += estrlen(tmpp2);
                app_estr(dest, PARENT_DIR);
#if 0
                printf("absp: (tmpp1) [%d] `%s'\n", estrlen(tmpp1), absp);
                printf("curp: (tmpp2) [%d] `%s'\n", estrlen(tmpp2), curp);
                printf("dest: `%s'\n", estr2str(dest));
#endif
            }
        }
        while (strlen(curp) && cmp_result);

    /* append equal part of path */
    app_estr(dest, rest_absp);

    /* append name of file */
    app_estr(dest, estr2str(fname));

    /* relaese resources */
    del_estr(fname);
    del_estr(abspa);
    del_estr(tmpp1);
    del_estr(tmpp2);

    return (ok_fnl_fpath(dest));
}

BOOL optimize_fname(STRPTR *target_name, STRPTR source_name)
{
/* Some system dependant defines */
#if defined AMIGA
#define PARENT_DIRECTORY_BEGIN  "/"
#define PARENT_DIRECTORY_INSIDE "//"
#elif defined BEOS || defined NEXTSTEP || defined RISCOS || defined UNIX
#define PARENT_DIRECTORY_BEGIN  "../"
#define PARENT_DIRECTORY_INSIDE "/../"
#else
#error "system not supported: parent directory begin/inside"
#endif

#define mark_for_skip(what)             \
    {                                   \
        STRPTR what2 = (what);          \
        if (what2[0] != SKIP_CHARACTER) \
        {                               \
            what2[0] = SKIP_CHARACTER;  \
            filename_length -= 1;       \
        }                               \
    }

/* Set this to 1 for debugging output */
#if 0
/* Use asterix to mark characters to be skipped. This can cause problems as
 * this character might be used in filenames on some systems, but it is
 * much nicer for debugging. */
#define SKIP_CHARACTER '*'

/* Show debug output for every remarkable filename manipulation */
#define PRINT_FILENAME(heading)                          \
    {                                                    \
        STRPTR debug_scan = filename;                    \
                                                         \
        if (heading[0] != '\0')                          \
        {                                                \
            printf(":::-- %s\n", heading);               \
        }                                                \
                                                         \
        printf(":::  `%s'\n", filename);                 \
        printf(":::   ");                                \
        while (debug_scan[0] != 0)                       \
        {                                                \
            if (debug_scan == next_parent_inside)        \
            {                                            \
                printf("n");                             \
            }                                            \
            else if (debug_scan == previous_directory)   \
            {                                            \
                printf("p");                             \
            }                                            \
            else if (debug_scan == filename_fun_start)   \
            {                                            \
                printf("f");                             \
            }                                            \
            else if (debug_scan == filename_scan)        \
            {                                            \
                printf("s");                             \
            }                                            \
            else                                         \
            {                                            \
                printf(" ");                             \
            }                                            \
            debug_scan += 1;                             \
        }                                                \
        printf("\n");                                    \
    }
#else
#define SKIP_CHARACTER '\a' /* Use the invisible bell character */
#define PRINT_FILENAME(heading) /* Do nufin */
#endif

    STRPTR filename = strclone(source_name);
    STRPTR filename_fun_start = filename;
    STRPTR filename_scan = NULL;
    STRPTR next_parent_inside = NULL;
    STRPTR previous_directory = NULL;
    size_t filename_length = strlen(filename);
    size_t parent_begin_length = strlen(PARENT_DIRECTORY_BEGIN);
    size_t parent_inside_length = strlen(PARENT_DIRECTORY_BEGIN);
    size_t parent_inside_index = 0;

    /* Skip device name */
#if defined AMIGA
    STRPTR device_delimiter = strchr(filename_fun_start, ':');

    if (device_delimiter != NULL)
    {
        filename_fun_start = device_delimiter + 1;
        PRINT_FILENAME("strip device");
    }

#elif defined BEOS || defined NEXTSTEP || defined RISCOS || defined UNIX
    if (filename_fun_start[0] == '/')
    {
        STRPTR next_directory = strchr(filename_fun_start+1, '/');
        if (next_directory != NULL)
        {
            filename_fun_start = next_directory+1;
        }
        else
        {
            /* This is a plain device name, simply move to the end of it */
            filename_fun_start += strlen(filename_fun_start);
        }
        PRINT_FILENAME("strip device");
    }
#else
#error "system not supported: skip device name"
#endif

    /* Skip leading parent directories */
    while (!strncmp(filename_fun_start, PARENT_DIRECTORY_BEGIN, parent_begin_length))
    {
        filename_fun_start += parent_begin_length;
    }

    /* Now filename_scan points to the first character where "the fun
     * starts". Everything before it must not be overwritten by
     * SKIP_CHARACTER */
    filename_scan = filename_fun_start;

    do
    {
        /* Find parent directory inside filename starting from current
         * scan position */
        next_parent_inside = strstr(filename_scan, PARENT_DIRECTORY_INSIDE);
        if (next_parent_inside != NULL)
        {
            /* Now we have a situation like this:
             *
             * sepp:///hugo/resi//hinz/kunz.txt
             * ^       ^        ^
             * |       |        |
             * |       |        next_parent_inside
             * |       filename_fun_start
             * filename
             *
             * Therefor we need to scan backwards for the next "/"
             */
            char fun_start_character = filename_fun_start[0];

            previous_directory = next_parent_inside - 1;

            while ((previous_directory > filename_fun_start)
                   && (previous_directory[0] != PATH_SEPARATOR[0]))
            {
                previous_directory -= 1;
            }

            PRINT_FILENAME("");

            /* Overwrite the previous directory, including the "/" */
            do
            {
                mark_for_skip(previous_directory)
                previous_directory += 1;
            }
            while (previous_directory[0] != PATH_SEPARATOR[0]);

            PRINT_FILENAME("");


            /* Overwrite the parent directory, excluding the "/" */
            for (parent_inside_index = 0;
                 parent_inside_index < parent_inside_length;
                 parent_inside_index += 1)
            {
                mark_for_skip(&(next_parent_inside[parent_inside_index]));
            }
            filename_scan = next_parent_inside + parent_inside_length;

            PRINT_FILENAME("");

            /* If the previous step removed the first directory of the
             * whole filename, also strip the "/" */
            if (filename_fun_start[0] != fun_start_character)
            {
                mark_for_skip(filename_scan);
                filename_scan += 1;

                PRINT_FILENAME("strip parent inside at start");
            }
        }
        else
        {
            /* Juhuu, the nightmare is over! Copy all characters from
             * filename to target except those marked as SKIP_CHARACTER */
            STRPTR target = (STRPTR) umalloc(filename_length + 1);
            size_t target_index = 0;
            *target_name = target;

            PRINT_FILENAME("finished");

            filename_scan = filename;
            while (filename_scan[0] != '\0')
            {
                if (filename_scan[0] != SKIP_CHARACTER)
                {
                    target[target_index] = filename_scan[0];
                    target_index += 1;
                }
                filename_scan += 1;
            }

            /* Mark end of string */
            target[target_index] = '\0';

            if (target_index != filename_length)
            {
                panic("bad target_index");
            }

        }
    }
    while (next_parent_inside != NULL);

    /* Cleanup */
    ufree(filename);

    return TRUE;
}

