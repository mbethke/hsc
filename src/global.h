/*
** global.h
**
** global variables and functions for hsc
**
*/

#ifndef HSC_GLOBAL_H
#define HSC_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/infile.h"

/*
** defines
*/
#ifdef AMIGA
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "", ":", "s:"
#endif

#ifdef UNIX
#define CONFIG_FILE ".hscrc"
#define CONFIG_PATH "./", "~/", "/"
#endif

#ifdef MSDOS
#define CONFIG_FILE "HSC.CFG"
#define CONFIG_PATH ".\\", "\\"
#endif

#define HSC_DEFNAME "hscdef.cfg"       /* file that defines tags & entities */

#define MAXLINELEN  1023               /* input line */
#define MAX_ARGLEN   511               /* arguments */
#define MAX_TAGLEN    63               /* tag id */
#define MAX_PATHLEN  255               /* directory path & filename */
#define MAX_URILEN   255               /* URIs */

/*
** defines
*/

/*
** char constants
*/
#define CH_QUOTE '\"'
#define CH_SPACE ' '
#define CH_LF    '\n'
#define CH_CR    '\r'
#define CH_ESC   27
#define CH_NULL  '\0'

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSC_GLOBAL_H

/*
** global vars
*/
extern STRPTR inpfilename;
extern STRPTR outfilename;
extern STRPTR errfilename;
extern STRPTR destdir;

extern ULONG max_error;

extern BOOL absuri;
extern BOOL chkuri;
extern BOOL debug;
extern BOOL insanch;
extern BOOL need_help;
extern BOOL pipe_in;
extern BOOL stripuri;
extern BOOL statusmsg;
extern BOOL verbose;


extern FILE *outfile;
extern time_t now;

extern STRPTR destfname;
extern STRPTR rel_destdir;

/*
** global funcs
*/
extern int fprintf_spc( FILE *file, size_t num );


#endif /* NOEXTERN_HSC_GLOBAL_H */

#endif /* HSC_GLOBAL_H */
