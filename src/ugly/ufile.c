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
 * ugly/ufile.c
 *
 * misc. filename functions
 *
 * updated: 31-May-2004
 * created: 14-Oct-1996
 *
 */

#include "sysdep.h"
#include <stdio.h>
#include <errno.h>
#include <time.h>

#ifdef AMIGA
/* SAS/C's stat handling is weird, so use dos.library :-( */
#include <dos/dos.h>
#include <proto/dos.h>
#endif

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

#define NOEXTERN_UGLY_UFILE_H
#include "ufile.h"

/* Windoze doesn't have those S_IS* macros for struct stat */
#ifdef WINNT
#define	__S_ISTYPE(mode, mask)	(((mode) & _S_IFMT) == (mask))
#define S_ISDIR(mode) __S_ISTYPE(mode, _S_IFDIR)
#define S_ISCHR(mode) __S_ISTYPE(mode, _S_IFCHR)
#define S_ISFIFO(mode) __S_ISTYPE(mode, _S_IFIFO)
#define S_ISREG(mode) __S_ISTYPE(mode, _S_IFREG)
#define S_ISLNK(mode) __S_ISTYPE(mode, _S_IFREG)  /* no symbolic links either */
#endif

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
        fclose(file);
    return ((BOOL) (file != NULL));
}

/*
 * fgetentrytype
 *
 * check whether a filesystem object is a file or a directory
 *
 * result: see fentrytype_t definition
 *
 */
fentrytype_t fgetentrytype(const STRPTR name) {
   fentrytype_t type = FE_NONE;
#ifdef AMIGA
   struct FileInfoBlock *fib;
   BPTR lock = Lock(name,ACCESS_READ);
      
   if(lock) {
      if((fib = AllocDosObjectTagList(DOS_FIB,NULL))) {
         if(Examine(lock,fib)) {
            /* this will barf on softlinks, but on AmigaOS these suck anyway */
            type = (fib->fib_DirEntryType > 0) ? FE_DIR : FE_FILE;
         }
         FreeDosObject(DOS_FIB,fib);
      }
      UnLock(lock);
   }
#else
   /* Does this work on RiscOS? Does anyone use HSC on RiscOS? Does anyone use
    * RiscOS...? */
   struct stat sbuf;
   
   if(-1 != stat(name,&sbuf)) {
      if(S_ISDIR(sbuf.st_mode))
         type = FE_DIR;
      else if(S_ISREG(sbuf.st_mode) || S_ISLNK(sbuf.st_mode))
         type = FE_FILE;
   }
#endif
   return type; 
}

/*
 * fgetmtime
 *
 * Get the date/time of last modification from a file and return it as a
 * pointer to an internal struct tm or NULL on failure.
 *
 */
const struct tm *fgetmtime(const STRPTR name) {
   struct tm *time = NULL;
#ifdef AMIGA
   struct FileInfoBlock *fib;
   BPTR lock = Lock(name,ACCESS_READ);
      
   if(lock) {
      if((fib = AllocDosObjectTagList(DOS_FIB,NULL))) {
         if(Examine(lock,fib)) {
            time_t tt;
            /* convert struct DateStamp to time_t format */
            /* TODO: test this */
            tt = 252432000 +
               fib->fib_Date.ds_Days * 86400 +
               fib->fib_Date.ds_Minute * 60 +
               fib->fib_Date.ds_Tick / 50;
            time = localtime(&tt);
         }
         FreeDosObject(DOS_FIB,fib);
      }
      UnLock(lock);
   }
#else
   /* Does this work on RiscOS? Does anyone use HSC on RiscOS? Does anyone use
    * RiscOS...? */
   struct stat sbuf;
   
   if(-1 != stat(name,&sbuf)) {
      time = localtime(&sbuf.st_mtime);
   }
#endif
   return time; 
}

/*
 * getcurrentdir
 *
 * Get the current working directory as a string 
 *
 * result: pointer to the the directory name, free this using ufree()
 *
 */
STRPTR getcurrentdir(void) {
   STRPTR s;
   int ss=32;
   errno = 0;
   while(1) {
      if(NULL == (s = umalloc(ss)))
         panic("out of memory");
      if(NULL == getcwd(s,ss)) {
         if(ERANGE != errno)
            panic(strerror(errno));
         ufree(s);
         ss *= 2;
      } else break;
   }
   return s;
}

/*
 * setcurrentdir
 *
 * Set the working directory
 *
 * result: nothing, side effect only
 *
 */
void setcurrentdir(const STRPTR dir) {
   /* this is just to avoid too much system specific stuff in other modules */
   chdir(dir);
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
