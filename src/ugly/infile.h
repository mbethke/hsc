/*
** ugly/infile.h
**
** ugly input file handling functions, header file
**
*/

#ifndef UGLY_INFILE_H                  /* avoid include twice */
#define UGLY_INFILE_H

/*
** includes
*/
#include <ctype.h>
#include <stdio.h>

#include "types.h"
#include "expstr.h"

/*
** ugly input file structure (PRIVATE)
*/
typedef struct infile {
    FILE  *infile;           /* file opened if fopen() */
    STRPTR filename;         /* file name */
    EXPSTR *lnbuf;           /* buffer for inputline */
    EXPSTR *wordbuf;         /* word buffer */
    EXPSTR *wspcbuf;         /* word buffer (white spaces) */
    EXPSTR *logstr;          /* log string */
    STRPTR eow;              /* string of chars used as end of word */

    ULONG  flnctr;           /* line number in file */
    size_t lnctr;            /* pos. in readbuffer */

    BOOL  (*is_ws)( char ch );     /* ptr to func that checks if a char */
                                   /* is a white-space */
    BOOL  (*is_nc)( char ch );     /* deto, but for "normal char" */

    BOOL   log_enable;       /* flag: TRUE means to log all chars */
    BOOL   eof_reached;      /* flag: TRUE, if end of file */
    BOOL   skipped_ws;       /* flag: TRUE, if last infgetw */
                             /*       skipped a white-space */

} INFILE;


/*
** global macros
*/
#define infclose( f ) { if ( f ) { infclose1( f ); f = NULL; } }

/*
** global vars
*/
extern STRPTR FNAME_STDIN;

/*
**
** extern function prototypes & global vars
**
*/
#ifndef NOEXTERN_UGLY_INFILE_H

extern INFILE *infopen( CONSTRPTR filename, size_t bufsize );
extern int     infclose1( INFILE *inpf );  /* PRIVATE; use infclose() */

extern int    infgetc( INFILE *inpf );
extern STRPTR infgetw( INFILE *inpf );
extern STRPTR infgetcws( INFILE *inpf );
extern STRPTR infgetcw( INFILE *inpf );

extern int    inungetc( int ch, INFILE *inpf );
extern size_t inungets( STRPTR s, INFILE *inpf );
extern size_t inungetcwws( INFILE *inpf );
extern size_t inungetcw( INFILE *inpf );

extern ULONG  infget_x( INFILE *inpf );
extern ULONG  infget_y( INFILE *inpf );
extern STRPTR infget_fname( INFILE *inpf );
extern BOOL   infget_skws( INFILE *inpf );

extern BOOL   inf_isws( char ch, INFILE *inpf );
extern size_t infskip_ws( INFILE *inpf );

extern int    infeof( INFILE *inpf );
extern int    infgotoeol( INFILE *inpf );

extern void inflog_enable( INFILE *inpf );
extern void inflog_disable( INFILE *inpf );
extern BOOL inflog_clear( INFILE *inpf );
extern STRPTR infget_log( INFILE *inpf );


#endif /* NOEXTERN_UGLY_INFILE_H */

#endif /* UGLY_INFILE_H */

