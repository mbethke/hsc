/*
** hsclib/include.c
**
** hsc include functions
**
** Copyright (C) 1995,96  Thomas Aglassinger
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** updated: 14-Apr-1996
** created: 19-Feb-1996
*/

#include <time.h>

#include "hsclib/inc_base.h"

#include "hsclib/input.h"
#include "hsclib/parse.h"

#define NOEXTERN_HSCLIB_INCLUDE_H
#include "hsclib/include.h"

/*
** hsc_include
**
** read from inpf, parse for hsc-commands and execute them,
** check for html-error,
** write all out to outf and close input file.
**
** params: inpfnm...input file name
**         outf.....output file structure, already opended
**
** result: TRUE, if all worked well, else FALSE
*/
static BOOL hsc_include( HSCPRC *hp, INFILE *inpf, ULONG optn, INFILEPOS *base_pos )
{
    BOOL    ok;            /* result */
    ok   = (inpf!=NULL);

    if ( inpf ) {                                /* file opened? */

        /* push current input file on input-file-stack */
        if ( hp->inpf )
            ins_dlnode( hp->inpf_stack, hp->inpf_stack->first, (APTR) hp->inpf );

        /* set new base position for input-file */
        /* (if called from a macro or after eg. <$source>) */
        if ( base_pos )
            set_infile_base( inpf, base_pos );

        /* assign new input file to hsc-process */
        hp->inpf = inpf;

        /* hide status? */
        if ( (optn & IH_PARSE_MACRO) || (optn & IH_PARSE_MACRO) )
            optn |= IH_NO_STATUS;

        /* set char-parse methods */
        inpf->is_nc = hsc_normch;                /*    set is_nc-methode */
        inpf->is_ws = hsc_whtspc;                /*    set is_ws-methode */

        if ( !(optn & IH_NO_STATUS ) && !infeof(inpf) )
            hsc_status_file_begin( hp );         /*    status message: */
                                                 /*      new file */

        while ( !infeof(inpf) && ok ) {          /*    parse file */


            if ( !(optn & IH_NO_STATUS ) &&      /* status message */
                 ( hp->prev_status_line != infget_y( hp->inpf ) )
               )
            {
                hsc_status_line( hp );
                hp->prev_status_line = infget_y( hp->inpf );
            }

            /* parse next item */
            if ( optn & IH_PARSE_SOURCE )
                ok = hsc_parse_source( hp );
            else
                ok = hsc_parse( hp );
        }

        /* parse at end: check for missing tags, .. */
        if ( ok && (optn & IH_PARSE_END) ) {     /*    parse end */
                                                 /*    (unclosed tags etc) */
            ok = hsc_parse_end( hp );

            if ( ok && (optn & IH_UPDATE_PRJ ) )
                ok = hsc_parse_end_id( hp );     /* update project file */

        }

        /* end of file status */
        if ( !(optn & IH_NO_STATUS) ) {

            hsc_status_file_end( hp );           /*    status message: */
                                                 /*      file processed */
        }

        infclose( hp->inpf );                    /*    close file */

        /* pull previous input file from input-file-stack
        ** or end hsc-process
        */
        if ( hp->inpf_stack->first ) {

            /* pull first item from stack */
            hp->inpf = (INFILE *) hp->inpf_stack->first->data;
            hp->inpf_stack->first->data = NULL;

            del_dlnode( hp->inpf_stack, hp->inpf_stack->first );

        } else {

            hp->inpf = NULL;
        }

    } else
        panic( "no input file" );

    return ( ok );
}

/*
** hsc_include_file
**
** open input file and include it
*/
BOOL hsc_base_include_file( HSCPRC *hp, STRPTR filename, ULONG optn, INFILEPOS *base_pos  )
{
    BOOL    ok = FALSE;
    STRARR msg[ 200 ]; /* TODO: use expstr */
    INFILE *inpf = NULL;

    /* status message: reading input */
    if ( !(optn & (IH_PARSE_MACRO | IH_PARSE_HSC)) ) {

        sprintf( msg, "reading `%s'", filename );
        hsc_status_misc( hp, msg );

    }

    /* open & read input file */
    inpf = infopen( filename, ES_STEP_INFILE );

    if ( inpf )
        /* include opened file */
        ok = hsc_include( hp, inpf, optn, base_pos );
    else
    {
        /* couldn't open file */
        hsc_message( hp, MSG_NO_INPUT,
                     "can not open %q for input: %s",
                     filename, strerror( errno ) );
    }

    return( ok );
}

/*
** hsc_include_string
**
** open string as input file and include it
*/
BOOL hsc_base_include_string( HSCPRC *hp, STRPTR filename, STRPTR s, ULONG optn, INFILEPOS *base_pos  )
{
    BOOL    ok;
    INFILE *inpf = infopen_str( filename, s, 0 );/* try to open input file */

    ok = hsc_include( hp, inpf, optn, base_pos );

    return( ok );
}


BOOL hsc_include_file( HSCPRC *hp, STRPTR filename, ULONG optn )
{
    return( hsc_base_include_file( hp, filename, optn, NULL ) );
}

BOOL hsc_include_string( HSCPRC *hp, STRPTR filename, STRPTR s, ULONG optn )
{
    return( hsc_base_include_string( hp, filename, s, optn, NULL ) );
}



