/*
 * ugly/infile.c
 *
 * ugly input file functions
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
 * updated: 30-Jul-1996
 * created:  8-Jul-1995
 */

/*
 * TODO:
 *
 * - handle errors within expstr (no mem)
 * - more elegant handling of inunget( '\n' );
 */

/*
 * includes
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utypes.h"
#include "expstr.h"
#include "ustring.h"

#define NOEXTERN_UGLY_FILE_H
#include "infile.h"
#include "umemory.h"

/* buffer size for fgets() in ugly_infgetc() */
#define INF_FGETS_BUFSIZE 32

/*
 * global vars
 */
STRPTR FNAME_STDIN = "STDIN"; /* filename for stdin (CONSTANT) */

/*
 * debugging defines
 */
#define DINF "*infile* "

#ifndef D

#if DEBUG_UGLY_INFILE
#define D( x ) x
#else
#define D( x )                  /* nufin */
#endif

#endif

/*
 * local vars
 */

/* forward references */
BOOL infget_skws(INFILE * inpf);
static VOID del_infilepos_nddata(APTR data);

/*
 *
 * local functions (not exported into header)
 *
 */

/*
 * default_whtspc, default_normch
 *
 * default methods for infgetw() to determine if
 * a char is a whitespace or normal char.
 */
BOOL default_whtspc(int ch)
{
    if (strchr(" \t", ch) != NULL)
        return TRUE;
    else
        return FALSE;
}

BOOL default_normch(int ch)
{
    if (isalnum(ch) || (ch == '_'))
        return TRUE;
    else
        return FALSE;
}

/*
 *-------------------------------------
 * constructor/destructor
 *-------------------------------------
 */

/*
 * reset_inpf
 *
 * reset a INFILE struct (set all items to NULL)
 * (only called by con/destructor)
 */
static VOID reset_infile(INFILE * inpf)
{
    inpf->infile = NULL;
    inpf->filename = NULL;
    inpf->lnbuf = NULL;
    inpf->wordbuf = NULL;
    inpf->wspcbuf = NULL;
    inpf->filepos = 0;
    inpf->pos_x = 0;
    inpf->pos_y = 0;
    inpf->wpos_x = 0;
    inpf->wpos_y = 0;
    inpf->base_x = 0;
    inpf->base_y = 0;
    inpf->pos_list = NULL;
    inpf->pos_count = 0;
    inpf->eof_reached = FALSE;
    inpf->out_of_mem = FALSE;
    inpf->skipped_ws = FALSE;
    inpf->closed = FALSE;
    inpf->is_nc = NULL;
    inpf->is_ws = NULL;
}

/*
 * remove INFILE structure
 */
static VOID del_infile(INFILE * inpf)
{
    if (inpf)
    {

        D(
             {
             fprintf(stderr, DINF "close file");
             if (inpf->filename)
             fprintf(stderr, " \"%s\"", inpf->filename);
             fprintf(stderr, "\n");
             }
        );

        if (inpf->pos_count)
        {
            inpf->closed = TRUE;
            D(fprintf(stderr, DINF "  (%d fpos-req left)\n",
                      inpf->pos_count));
        }
        else
        {
            /* close file */
            if (inpf->infile)
                fclose(inpf->infile);

            /* remove pos-requests */
            del_dllist(inpf->pos_list);

            /* release mem */
            ufreestr(inpf->filename);
            del_estr(inpf->lnbuf);
            del_estr(inpf->wordbuf);
            del_estr(inpf->wspcbuf);

            /* reset all items */
            reset_infile(inpf);

            /* release whole structure */
            ufree(inpf);
        }
    }
}

/*
 * init INFILE structure
 */
INFILE *init_infile(CONSTRPTR name, size_t step_size)
{
    INFILE *inpf = (INFILE *) umalloc(sizeof(INFILE));

    if (inpf)
    {

        /* check for buffer-stepsize */
        if (!step_size)
            step_size = IF_BUFFER_VALUE;

        /* reset all items */
        reset_infile(inpf);

        /* clone filename (NULL=stdin) */
        if (name)
            inpf->filename = strclone(name);

        /* init wordbuffers */
        inpf->lnbuf = init_estr(step_size);
        inpf->wordbuf = init_estr(step_size);
        inpf->wspcbuf = init_estr(step_size);
        inpf->pos_list = init_dllist(del_infilepos_nddata);
        inpf->pos_count = 0;

        /* check if init ok */
        if (!((inpf->filename || !(name))
              && inpf->lnbuf
              && inpf->wordbuf
              && inpf->wspcbuf))
        {
            /* remove infile, */
            /* set return value to NULL */
            del_infile(inpf);
            inpf = NULL;
        }
    }

    return (inpf);
}

/*
 *=====================================
 *
 * exported functions
 *
 *=====================================
 */

/*
 *-------------------------------------
 * functions to get info about infile
 *-------------------------------------
 */

/*
 * infget_x
 *
 * get colum of current line
 */
ULONG infget_x(INFILE * inpf)
{
    return (inpf->pos_x + inpf->base_x);
}

/*
 * infget_y
 *
 * get current line
 */
ULONG infget_y(INFILE * inpf)
{
    return (inpf->pos_y + inpf->base_y);
}

/*
 * infget_wx
 *
 * get colum of current word
 */
ULONG infget_wx(INFILE * inpf)
{
    return (inpf->wpos_x + inpf->base_x);
}

/*
 * infget_y
 *
 * get line of current word
 */
ULONG infget_wy(INFILE * inpf)
{
    return (inpf->wpos_y + inpf->base_y);
}

/*
 * infget_skws
 *
 * get status of skipped_ws
 */
BOOL infget_skws(INFILE * inpf)
{
    return (inpf->skipped_ws);
}

/*
 * infget_fname
 *
 * get name of file
 */
STRPTR infget_fname(INFILE * inpf)
{
    if (inpf->filename)
        return (inpf->filename);
    else
        return (FNAME_STDIN);
}

/*
 * infeof
 *
 * check if end of input file reached
 *
 * params: inpf...input file to check
 * result: if end of input file reached, 0 is returned,
 *         else a value < 0 (compare feof() of some
 *         compilers)
 */
int infeof(INFILE * inpf)
{
    if (inpf->eof_reached == TRUE)
        return EOF;
    else
        return 0;
}

/*
 * infget_cws (get current whites spaces)
 *
 * get string that contains all white-spaces
 * skipped within the last call of infgetw()
 */
STRPTR infgetcws(INFILE * inpf)
{
    return (estr2str(inpf->wspcbuf));
}

/*
 * infget_cw (get current word)
 *
 * get string that contains all chars
 * read within the last call of infgetw()
 */
STRPTR infgetcw(INFILE * inpf)
{
    return (estr2str(inpf->wordbuf));
}

/*
 *-------------------------------------
 * functions to set methods
 *-------------------------------------
 */

/*
 * set_inf_whtspc
 *
 * set function to check if a char is a whitespace.
 * this func is called by infgetw() to determine if
 * the begining of a word is reached.
 */
void set_whtspc(INFILE * inpf, BOOL(*iswsfn) (int ch))
{
    if (inpf)
        inpf->is_ws = iswsfn;
}

/*
 * set_inf_normch
 *
 * set function to check if a char is a "normal" char.
 * this function is called by infgetw() to determine if
 * the end of a word is reached
 */
void set_normch(INFILE * inpf, BOOL(*isncfn) (int ch))
{
    if (inpf)
        inpf->is_nc = isncfn;
}

/*
 *-------------------------------------
 * functions for open & close
 *-------------------------------------
 */

/*
 * infclose1
 *
 * close INFILE entry, free all mem allocated by it
 *
 * result: always 0
 *
 */
int infclose1(INFILE * inpf)
{
    del_infile(inpf);

    return (0);
}

/*
 * infopen
 *
 * params: name.......name of input file to open
 *         step_size..portion of memory buffers should be increased with
 * result: ptr to INFILE entry or NULL if error;
 *
 * NOTE: if result = NULL, but errno=0, too, then there
 *       wasn't enough mem for the line-buffer. in this
 *       case, calling perror() will not produce a proper
 *       error message.
 *
 * NOTE: don't forget to set errno=0 before calling infopen().
 */
INFILE *infopen(CONSTRPTR name, size_t step_size)
{
    INFILE *inpf = init_infile(name, step_size);        /* return file */

    if (inpf)
        /* open input file or assign stdin to input file */
        if (name)
        {

            inpf->infile = fopen(name, "r");
            if (!(inpf->infile))
            {

                del_infile(inpf);
                inpf = NULL;

            }
        }
        else
            inpf->infile = stdin;

    /* read whole file into file lnbuf */
    if (inpf)
    {

        STRARR buf[INF_FGETS_BUFSIZE];
        STRPTR restr = buf;     /* result of fgets() (dummy init) */
        BOOL ok = TRUE;

        while (!feof(inpf->infile) && ok)
        {

            restr = fgets(buf, INF_FGETS_BUFSIZE, inpf->infile);
            if (restr)
                ok &= app_estr(inpf->lnbuf, restr);

        }
    }

    return (inpf);              /* return input file */
}

/*
 * infopen_str
 *
 * open a string as an input file
 *
 * params: fname......pseudo filename the string should have
 *         s..........string that should be handled as a file
 *         step_size..portion of memory buffers should be increased with
 * result: ptr to INFILE entry or NULL if error;
 *
 * NOTE: a copy of the passed string is created. so you can
 *       modify or release the string after calling inopen_str()
 */
INFILE *infopen_str(CONSTRPTR name, CONSTRPTR s, size_t step_size)
{
    INFILE *inpf = init_infile(name, step_size);        /* return file */

    if (inpf)
    {

        /* copy string to line buffer */
        BOOL ok = set_estr(inpf->lnbuf, s);

        if (!ok)
        {

            del_infile(inpf);
            inpf = NULL;
        }
    }

    return (inpf);              /* return input file */
}

/*
 *-------------------------------------
 * functions to get text from infile
 *-------------------------------------
 */

/*
 * ugly_infgetc
 *
 *
 */
int ugly_infgetc(INFILE * inpf)
{
    int result = EOF;

#if 0
    fprintf(stderr, "** ingetch( \"%s\" at %p\n", inpf->filename, inpf);
#endif
    if (inpf && (!inpf->eof_reached))
    {

        STRPTR lnbuf_str = estr2str(inpf->lnbuf);

        /*
         * if at end of line buffer, scan next line
         * before proceding
         */
        if (lnbuf_str[inpf->filepos] == 0)
        {

            inpf->eof_reached = TRUE;

        }

        /*
         * check wether to return EOF or char from
         * line buffer
         */
        if (inpf->eof_reached == FALSE)
        {

            lnbuf_str = estr2str(inpf->lnbuf);
            result = lnbuf_str[inpf->filepos];  /* set last char as result */
            if (result)
            {                   /* goto next char in buf */

                inpf->pos_x++;
                inpf->filepos++;

            }
        }

        /* update line number */
        if (result == '\n')
        {

            inpf->pos_y++;
            inpf->pos_x = 0;

        }
    }

    return (result);
}

/*
 * infgetc
 *
 * read next char from file, update word-position
 */
int infgetc(INFILE * inpf)
{
    /* store word position */
    inpf->wpos_x = inpf->pos_x;
    inpf->wpos_y = inpf->pos_y;
    return (ugly_infgetc(inpf));
}

/*
 *-------------------------------------
 * functions to unget text from infile
 *-------------------------------------
 */

/*
 * inungetc
 *
 * write char back to stream; comparable to ansi's ungetc()
 *
 * params: inpf...input file
 *         ch.....char to write back
 * result: ch, if sucessful, else EOF
 */
int inungetc(int ch, INFILE * inpf)
{
    int result = EOF;

    if (inpf && (inpf->filepos))
    {

        STRPTR lnbuf_str = estr2str(inpf->lnbuf);

        /* update file position */
        inpf->filepos--;

        /* write back char */
        lnbuf_str[inpf->filepos] = ch;
        result = ch;

        /* handle LF */
        if (ch == '\n')
        {

            result = ch;
            inpf->pos_y--;
            inpf->pos_x = 0;

        }
        else if (inpf->pos_x)
            inpf->pos_x--;

    }

    return (result);
}

/*
 * inungets
 * write string back to stream
 *
 * params: s......string to write back
 *         inpf...input file
 * result: num of chars written back
 */
size_t inungets(STRPTR s, INFILE * inpf)
{
    size_t ctr = 0;             /* counter, how many chars already written */
    size_t slen = strlen(s);
    STRPTR p = s + (strlen(s) - 1);     /* ptr to current char in string */
    int ch = 0;                 /* current char written, dummy init */

    if (slen > 0)
    {

        ctr = 1;                /* unget first char */
        ch = inungetc(p[0], inpf);

        while ((p != s) && (ch != EOF))
        {

            ctr++;              /* inc counter */
            p--;                /* goto next char */
            ch = inungetc(p[0], inpf);  /* unget current char */

        }

    }

    return (ctr);
}

/*
 * inungetcw
 * write current word back to stream
 *
 * params: inpf...input file
 * result: num of chars written back
 */
size_t inungetcw(INFILE * inpf)
{
    size_t ctr;                 /* counter how many chars written */

    /* unget word */
    ctr = inungets(infgetcw(inpf), inpf);

    return (ctr);
}

/*
 * inungetcwws
 * write current word & whitespaces back to stream
 *
 * params: inpf...input file
 * result: num of chars written back
 */
size_t inungetcwws(INFILE * inpf)
{
    size_t ctr;                 /* counter how many chars written */

    /* unget word & white spaces */
    ctr = inungets(infgetcw(inpf), inpf);
    ctr += inungets(infgetcws(inpf), inpf);

    return (ctr);
}

/*
 * inf_isws
 *
 * checks if a char is a white space
 *
 * params: ch...char to check for white space
 * result: TRUE if ch was white space
 */
BOOL inf_isws(char ch, INFILE * inpf)
{
    BOOL(*isws) (int ch) = inpf->is_ws;

    if (isws == NULL)           /* if no func for is_ws, */
        isws = default_whtspc;  /*   set default function */

    return ((*isws) (ch));
}

/*
 * infskip_ws
 *
 * skip white spaces; update wspbuf; clear wordbuf
 *
 * TODO: handle wspcbuf-overflow
 */
size_t infskip_ws(INFILE * inpf)
{
    char nxtch;                 /* next char to read */
    size_t ctr = 0;             /* num of ws skipped */
    BOOL ok;

    /*
     * set function to determine if a
     * specified char is a white space
     */
    inpf->skipped_ws = FALSE;   /* clear skippe-flag */

    /* clear wspcbuf */
    ok = clr_estr(inpf->wspcbuf);

    /*
     * loop:  skip white spaces
     */
    nxtch = ugly_infgetc(inpf); /* read next char */
    while ((!infeof(inpf))      /* while not at end of file.. */
           && ok
           && inf_isws(nxtch, inpf))    /* ..and current char is a whtspc */
    {
        ok &= app_estrch(inpf->wspcbuf, nxtch);
        ctr++;
        nxtch = ugly_infgetc(inpf);     /*   read next char */
    }

    if (ctr)                    /* any whtspcs skipped? */
        inpf->skipped_ws = TRUE;        /* Y-> set skippe-flag */

#if 0
    if (!ok)
        /* TODO: error */ ;
#endif

    /*
     * write back last char read
     */
    inungetc(nxtch, inpf);

    return (ctr);
}

/*
 * infgetall
 *
 * returns whole text
 */
STRPTR infgetall(INFILE * inpf)
{
    return (estr2str(inpf->lnbuf));
}

/*
 * infgetw
 *
 * read word
 */
STRPTR infgetw(INFILE * inpf)
{
    /* TODO: handle expstr errors */
    int ch = EOF;
    BOOL wordread = FALSE;
    STRPTR thisword = NULL;
    BOOL ok = TRUE;

    /* set function for normal chars */
    BOOL(*isnc) (int ch) = inpf->is_nc;
    if (isnc == NULL)
        isnc = default_normch;

    /* skip all white spaces */
    infskip_ws(inpf);

    ok = clr_estr(inpf->wordbuf);

    /*
     * read word until non-normal char is reached
     */
    if (!infeof(inpf))
    {

        ch = infgetc(inpf);

        if (((*isnc) (ch)))
            do
            {

                ok &= app_estrch(inpf->wordbuf, ch);
                ch = ugly_infgetc(inpf);
                wordread = TRUE;
                /* todo: set out-of-mem-flag */

            }
            while ((ch != EOF) && ok && ((*isnc) (ch)));
        else
            ok &= app_estrch(inpf->wordbuf, ch);

        if ((ch != EOF) && (wordread))
            inungetc(ch, inpf);

        thisword = estr2str(inpf->wordbuf);

    }
    else
        thisword = NULL;

    return (thisword);
}

/*
 * infgotoeol
 *
 * read all chars until CR or EOF
 */
int infgotoeol(INFILE * inpf)
{
    int ch = EOF;

    if (!infeof(inpf))
    {
        /*
         * read all chars until CR appears
         */
        do
        {
            ch = ugly_infgetc(inpf);
        }
        while ((ch > 0) && (ch != 0x0a));

        /*
         * read LF
         */
        if (ch == 0x0a)
        {
            ch = ugly_infgetc(inpf);
            if (ch != 0x0d)
                inungetc(ch, inpf);
            ch = 0x0a;
        }
    }

    /* return last char read */
    return (ch);
}

/*
 *-------------------------------------
 * functions for file-positions
 *-------------------------------------
 */

/*
 * del_infilepos_nddata
 */
static VOID del_infilepos_nddata(APTR data)
{
    INFILEPOS *pos = (INFILEPOS *) data;

    if (pos)
    {
        /* decrese number of pending pos-requests */
        (pos->inpf->pos_count)--;

        D(fprintf(stderr, DINF "del pos-req: \"%s\" (%d,%d); %d left\n",
                  pos->inpf->filename, pos->x, pos->y,
                  pos->inpf->pos_count));

        /* free resources alloceted by pos-request */
        ufree(pos);
    }
}

/*
 * find_posnd
 */
static int cmp_posdata(APTR data1, APTR data2)
{
    if (data1 == data2)
        return (-1);
    else
        return (0);
}

/*
 * del_infilepos
 */
VOID del_infilepos(INFILEPOS * pos)
{
    if (pos)
    {
        INFILE *inpf = pos->inpf;

        /* remove filename */
        ufreestr(pos->fname);

        /* check, if file has already been closed by user,
         * but the last pos-requests has just been deleted
         * now.
         * if so, really close the file now
         */
        if ((inpf->closed)
            && (inpf->pos_count == 1)
            )
        {
            D(fprintf(stderr, DINF "  really closing file:\n"));
            (pos->inpf->pos_count)--;
            infclose1(inpf);
        }
        else
        {
            DLNODE *nd = find_dlnode(dll_first(pos->inpf->pos_list),
                                     pos, cmp_posdata);
            del_dlnode(pos->inpf->pos_list, nd);
        }
    }
}

/*
 * del_all_infilepos
 */
VOID del_all_infilepos(INFILE * inpf)
{
    del_all_dlnodes(inpf->pos_list);
}

/*
 * new_infilepos
 */
static INFILEPOS *new_infilepos_node(INFILE * inpfile, ULONG x, ULONG y)
{
    INFILEPOS *pos = umalloc(sizeof(INFILEPOS));

    if (pos)
    {
        DLNODE *nd = app_dlnode(inpfile->pos_list, pos);

        pos->inpf = inpfile;
        pos->fname = strclone(inpfile->filename);
        pos->x = x;
        pos->y = y;
        pos->fpos = inpfile->filepos;

        if (!nd)
        {
            del_infilepos_nddata(pos);
            pos = NULL;
        }
        else
        {
            (inpfile->pos_count)++;
        }
    }

    D(
         {
         if (pos)
         {
         fprintf(stderr, DINF "new pos-req: \"%s\" (%d,%d); #%d\n",
                 inpfile->filename,
                 inpfile->pos_x, inpfile->pos_y, inpfile->pos_count);
         }
         else
         {
         fprintf(stderr, DINF "new pos-req FAILED\n");
         }
         }
    );

    return pos;
}

INFILEPOS *new_infilepos(INFILE * inpfile)
{
    INFILEPOS *pos =
    new_infilepos_node(inpfile, inpfile->pos_x, inpfile->pos_y);
    return pos;
}

INFILEPOS *new_winfilepos(INFILE * inpfile)
{
    INFILEPOS *pos =
    new_infilepos_node(inpfile, inpfile->wpos_x, inpfile->wpos_y);
    return pos;
}

/*
 * get methodes for pos-req
 */
STRPTR ifp_get_fname(INFILEPOS * pos)
{
    return (infget_fname(pos->inpf));
}

ULONG ifp_get_x(INFILEPOS * pos)
{
    return (pos->x);
}

ULONG ifp_get_y(INFILEPOS * pos)
{
    return (pos->y);
}

BOOL set_infile_base(INFILE * inpf, INFILEPOS * pos)
{
    D(fprintf(stderr, DINF "set base \"%s\" to \"%s\" (%d,%d)\n",
              inpf->filename, pos->inpf->filename, pos->x, pos->y));
    reallocstr(&(inpf->filename), pos->fname);
    inpf->base_x = pos->x;
    inpf->base_y = pos->y + 1;
    return (TRUE);              /* TODO: handle out of mem */
}

BOOL set_infilepos(INFILE * inpf, INFILEPOS * pos)
{
    D(fprintf(stderr, DINF "set pos  \"%s\" to \"%s\" (%d,%d)\n",
              inpf->filename, pos->inpf->filename, pos->x, pos->y));
    reallocstr(&(inpf->filename), pos->fname);
    inpf->pos_x = pos->x;
    inpf->pos_y = pos->y;
    inpf->wpos_x = pos->x;
    inpf->wpos_y = pos->y;
    return (TRUE);              /* TODO: handle out of mem */
}

