/*
** ugly/outfile.c
**
** ugly input file functions
**
** Version 1.0.1, (W) by Tommy-Saftwörx in 1995
**
** updated: 18-Nov-1995
** created: 26-Oct-1995
**
** $VER: outfile.c 1.0.0 (18.11.1995)
**
*/

/*
** TODO: improve error handling
**
*/

/*
** includes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "expstr.h"
#include "string.h"


#define NOEXTERN_UGLY_FILE_H
#include "outfile.h"
#include "memory.h"

/*
** global vars
*/
STRPTR FNAME_STDOUT="<stdout>";          /* filename for stdout (CONSTANT) */


/*
** local vars
*/


/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** remove OUTFILE structure
*/
void del_outfile( OUTFILE *outf )
{
    if ( outf ) {

        /* close file */
        if ( outf->outfile )
            fclose( outf->outfile );

        /* release mem */
        ufreestr( outf->filename );
        del_estr( outf->buf );

        /* release whole structure */
        ufree( outf );
    }

}

/*
** init OUTFILE structure
*/
OUTFILE *init_outfile( CONSTRPTR name, size_t step_size )
{
    OUTFILE *outf = (OUTFILE *) umalloc( sizeof(OUTFILE) );

    if ( outf ) {

        /* check for buffer-stepsize */
        if ( !step_size )
            step_size = OF_BUFFER_VALUE;

        /* clone filename (NULL=stdin) */
        if ( name )
            outf->filename = strclone( name );
        else
            outf->filename = NULL;

        /* init file */
        outf->outfile = NULL;

        /* init wordbuffers */
        outf->buf = init_estr( step_size );

        /* init rest of struct */
        outf->out_of_mem = TRUE;
        outf->fp.fpos_x   = 0;
        outf->fp.fpos_y   = 0;
        outf->fp.fpos_idx = 0;



        /* check if init ok */
        if ( !( (outf->filename || !(name))
                && outf->buf ) )
        {
            /* remove outfile, */
            /* set return value to NULL */
            del_outfile( outf );
            outf = NULL;
        }
    }

    return( outf );
}


/*
**=====================================
**
** exported functions
**
**=====================================
*/


/*
**-------------------------------------
** functions to get info about outfile
**-------------------------------------
*/

/*
** outfget_x
**
** get colum of current line
*/
ULONG outfget_x( OUTFILE *outf )
{
    return (outf->fp.fpos_x+1);
}


/*
** outfget_y
**
** get colum of current line
*/
ULONG outfget_y( OUTFILE *outf )
{
    return (outf->fp.fpos_y+1);
}


/*
** outfget_fname
**
** get name of file
*/
STRPTR outfget_fname( OUTFILE *outf )
{
    if ( outf->filename )
        return (outf->filename);
    else
        return( FNAME_STDOUT );
}


/*
**-------------------------------------
** functions for open & close
**-------------------------------------
*/

/*
** outfclose1
**
** close OUTFILE entry, free all mem allocated by it
**
** result: always 0
**
*/
int outfclose1( OUTFILE *outf, BOOL write )
{
    int result = 0;

    if ( write ) {

        if ( outf->filename ) {

            outf->outfile = fopen( outf->filename, "w" );
            if ( !(outf->outfile) )
                result = EOF;

        } else
            outf->outfile = stdout;

        if ( result != EOF ) {

            result = fputs( estr2str(outf->buf), outf->outfile );
            if ( result != EOF )
                result = 0; /* all ok */

        }
    }

    del_outfile( outf );

    return( result );
}


/*
** outfopen
**
** params: name.......name of input file to open
**         step_size..portion of memory buffers should be increased with
** result: ptr to OUTFILE entry or NULL if error;
**
** NOTE: if result = NULL, but errno=0, too, then there
**       wasn't enough mem for the line-buffer. in this
**       case, calling perror() will not produce a proper
**       error message.
**
** NOTE: don't forget to set errno=0 before calling outfopen().
*/
OUTFILE *outfopen( CONSTRPTR name, size_t step_size )
{
    OUTFILE *outf = init_outfile( name, step_size ); /* return file */

    return( outf );                    /* return input file */
}


/*
**-------------------------------------
** functions to put text to outputfile
**-------------------------------------
*/

/*
** outfputc
**
**
*/
int outfputc( int c, OUTFILE *outf )
{
    int result = EOF;

#if 0
    fprintf( stderr, "** outputc( \"%s\": '%c' (%d) at %p\n",
             outf->filename, c, c, outf );
#endif
    if ( outf ) {

        if ( outf->outfile == stdout )
            result = putchar( c );
        else {

            if ( app_estrch( outf->buf, c ) )
                result = c;
            else
                outf->out_of_mem = TRUE;

        }
    }

    return( result );
}

/*
** outfputs
**
*/
int outfputs( CONSTRPTR s, OUTFILE *outf )
{
    int result = 0;

    while( s[0] && ( result != EOF ) ) {

        int rc = outfputc( s[0], outf );

        s++;

        if ( rc == EOF )
            result = EOF;
        else
            result++;

    }

    return( result );

}

