/*
** ugly/file.h
**
** ugly file handling functions, header file
**
*/

#ifndef UGLY_FILE_H                  /* avoid include twice */
#define UGLY_FILE_H

typedef struct filepos {

    size_t fpos_idx;         /* file position (index in string) */
    ULONG  fpos_y;           /* line number in file */
    size_t fpos_x;           /* pos. in current line */

} FILEPOS;

#include "infile.h"
#include "outfile.h"

#endif /* UGLY_FILE_H */
