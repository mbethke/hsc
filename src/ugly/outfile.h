/*
** ugly/outfile.h
**
** ugly output file handling functions, header file
**
*/

#ifndef UGLY_OUTFILE_H                  /* avoid include twice */
#define UGLY_OUTFILE_H

/*
** includes
*/
#include <ctype.h>
#include <stdio.h>

#include "types.h"
#include "expstr.h"

#include "file.h" /* TODO: remove this */

/* reasonable value for buffer-step-size */
#define OF_BUFFER_VALUE 128

/*
** ugly output file structure (PRIVATE)
*/
typedef struct outfile {
    STRPTR  filename;         /* file name */
    FILE   *outfile;          /* file opened if fopen() */
    EXPSTR *buf;              /* file data */
    FILEPOS fp;               /* file position */

    BOOL   out_of_mem;        /* flag: TRUE, if ran out of memory */

} OUTFILE;


/*
** global macros
*/
#define outfclose( f ) ((f) ? (outfclose1( f,TRUE ), f=NULL) : (NULL))
#define outfclear( f ) ((f) ? (outfclose1( f,FALSE ), f=NULL) : (NULL))

/*
** global vars
*/
extern STRPTR FNAME_STDOUT;

/*
**
** extern function prototypes & global vars
**
*/
#ifndef NOEXTERN_UGLY_OUTFILE_H

extern OUTFILE *outfopen( CONSTRPTR name, size_t step_size );
extern OUTFILE *outfopen_str( CONSTRPTR fname, CONSTRPTR s, size_t step_size );
extern int     outfclose1( OUTFILE *outf, BOOL write );  /* PRIVATE; use outfclose() */

extern int    outfputc( int c, OUTFILE *outf );
extern int    outfputs( CONSTRPTR s, OUTFILE *outf );

extern ULONG outfget_x( OUTFILE *outf );
extern ULONG outfget_y( OUTFILE *outf );
extern STRPTR outfget_fname( OUTFILE *outf );

#endif /* NOEXTERN_UGLY_OUTFILE_H */

#endif /* UGLY_OUTFILE_H */

