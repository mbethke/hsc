/*
** hsclib/tag_hsc.c
**
** tag callbacks for "<$xx>" and related
** (for macro callbacks, see "tag_macr.c")
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
** updated: 24-Mar-1996
** created: 23-Jul-1995
*/

#include "hsclib/inc_tagcb.h"

#include "hsclib/defattr.h"
#include "hsclib/deftag.h"
#include "hsclib/eval.h"
#include "hsclib/include.h"
#include "hsclib/parse.h"

#include "hsclib/tag_macr.h"
#include "hsclib/tag_if.h"

#define TIMEBUF_INC    20
#define ES_STEP_SOURCE 40

/* states for handle_hsc_source */
#define SRST_TEXT    0 /* inside text      */
#define SRST_LT      1 /* after "<"        */
#define SRST_SLASH   2 /* after "</"       */
#define SRST_CSOURCE 3 /* after "</$SOURCE" */
#define SRST_TAG     8 /* inside a tag     */
#define SRST_COMT    9 /* inside comment   */
#define SRST_ERR    99 /* error occured    */

/*
**-------------------------------------
** comment & skip handle (<* *>, <| |>)
**-------------------------------------
*/

/*
** handle_hsc_comment
**
** skip text until '*>' occures;
** nested commets are supported
**
*/
BOOL handle_hsc_comment( HSCPRC *hp, HSCTAG *tag )
{
    BYTE cstate = CMST_TEXT; /* vars for eoc_reached() */
    LONG cnest  = 0;         /* nesting: already read one "<*" */
    BOOL end    = FALSE;     /* end of comment reached? */

    while ( !end && !(hp->fatal) ) {

        end = eoc_reached( hp, &cstate, &cnest );

    }

    return( FALSE );
}

/*
** handle_hsc_onlycopy
**
** copy text until '|>' occures;
** no syntax check or whatsoever is performed
**
** TODO: find more reasonable way to send output to client.
*/
BOOL handle_hsc_onlycopy( HSCPRC *hp, HSCTAG *tag )
{
    INFILE *inpf = hp->inpf;
    int  ch      = EOF;                /* current char */
    int  prev_ch = EOF;                /* prev char read */
    BOOL abort   = FALSE;

    do {
        STRARR buf[3] = { 0,0,0 };

        ch = infgetc( inpf );

        if ( (prev_ch=='|') && (ch=='>') ) {

            abort = TRUE;

        } else if ( (prev_ch=='|') && (ch!='|') ) {

            buf[0] = prev_ch;
            buf[1] = ch;
            hsc_output_text( hp, "", buf );

        }
        else if ( ch != '|' )
        {

            buf[0] = ch;
            buf[1] = 0;
            hsc_output_text( hp, "", buf );
        }

        abort |= infeof(inpf);
        prev_ch = ch;

    } while ( !abort );

    if ( prev_ch == EOF )
        hsc_msg_eof(hp, "skipping source");

    return ( FALSE );
}

/*
** handle_hsc_insert_expression
**
** insert value of a hsc-expression
**
** TODO: what to use as error-location, when inserting expression?
*/
BOOL handle_hsc_insert_expression( HSCPRC *hp, HSCTAG *tag )
{
    HSCATTR   *dest  = new_hscattr( "_dummy_insexpr" );
    STRPTR     value = NULL;
    INFILEPOS *base = new_infilepos( hp->inpf );

    /* prepare dummy attribute */
    dest->vartype = VT_STRING;

    /* compute expression */
    value = eval_expression( hp, dest, ")" );
    if ( value )
    {
        parse_gt( hp );
        hsc_base_include_string( hp, SPECIAL_FILE_ID "insert expression",
                                 value,
                                 IH_PARSE_HSC | IH_NO_STATUS, base );
    }

    del_infilepos( base );
    del_hscattr( dest );

    return( FALSE );
}



/*
**-------------------------------------
** $INCLUDE handle
**-------------------------------------
*/

/*
** handle_hsc_include
**
** include a sub file
*/
BOOL handle_hsc_include( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR  fname   = get_vartext_byname( tag->attr, "FILE" );
    BOOL    source  = get_varbool_byname( tag->attr, "SOURCE" );
    BOOL    pre     = get_varbool_byname( tag->attr, "PRE" );
    INFILEPOS *base = NULL;

    ULONG   optn   = 0;

    /* avoid nesting of <PRE> */
    if ( hp->inside_pre )
        pre = FALSE; /* TODO: lauch warning */

    if ( source )
        optn |= IH_PARSE_SOURCE;

    /* insert leading <PRE> */
    if ( pre )
    {
        base = new_infilepos( hp->inpf );
        hsc_base_include_string( hp, SPECIAL_FILE_ID "include <PRE>", "<PRE>\n",
                                 IH_PARSE_HSC | IH_NO_STATUS, base );
        del_infilepos( base );
    }

    /* include main file */
    if ( fname )
        hsc_include_file( hp, fname, optn );

    /* insert tailing </PRE> */
    if ( pre )
    {
        base = new_infilepos( hp->inpf );
        hsc_base_include_string( hp, SPECIAL_FILE_ID "include </PRE>",
                                 "</PRE>\n",
                                 IH_PARSE_HSC | IH_NO_STATUS, base );
        del_infilepos( base );
    }

    return ( FALSE );
}


/*
**-------------------------------------
** $MESSAGE handle
**-------------------------------------
*/

/*
** handle_hsc_error
**
** user error message
*/
BOOL handle_hsc_message( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR msg_text  = get_vartext_byname( tag->attr, "TEXT" );
    STRPTR msg_class = get_vartext_byname( tag->attr, "CLASS" );

    if ( msg_text ) {

        ULONG msgid = ( MSG_USER_MESSAGE & MASK_MESSAGE );

        /* compute message id */
        if ( !upstrcmp( msg_class, "WARNING" ) )
            msgid |= MSG_WARN;
        else if ( !upstrcmp( msg_class, "ERROR" ) )
            msgid |= MSG_ERROR;
        else if ( !upstrcmp( msg_class, "FATAL" ) )
            msgid |= MSG_FATAL;
        else D( if ( upstrcmp( msg_class, "NOTE" ) )
            panic( "illegal user message class" ) );

        /* display message */
        hsc_message( hp, msgid, "user message: %s", msg_text );

    }

    return ( FALSE );
}


/*
**-------------------------------------
** $EXEC handle
**-------------------------------------
*/

/*
** handle_hsc_exec
**
** exec a sub file
*/
BOOL handle_hsc_exec( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR cmd = get_vartext_byname( tag->attr, "COMMAND" );

    if ( cmd ) {

        int     result;
        EXPSTR *msg = init_estr( 0 );

        app_estr( msg, "execute: " );
        app_estr( msg, cmd );

        /* status message */
        hsc_status_misc( hp, estr2str( msg ) );

        /* call command */
        result = system( cmd );

        /* check for non-zero-result */
        if ( result )
             hsc_message( hp, MSG_SYSTEM_RETURN,
                          "calling external command returned %d",
                          long2str( (LONG) result ) );

        del_estr( msg );

    }

    return ( FALSE );
}

/*
**-------------------------------------
** $INSERT handle
**-------------------------------------
*/


/*
** handle_hsc_time
**
** insert current time
*/
BOOL handle_hsc_time( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR  timefmt = get_vartext_byname( tag->attr, "FORMAT" );
    EXPSTR *timebuf = init_estr( TIMEBUF_INC );
    BOOL    strftrc = 0; /* result of strftime() */
    size_t  i; /* loop var */

    /* set default time format */
    if ( !timefmt )
        timefmt = "%d-%b-%Y, %H:%M";

    while ( !(hp->fatal) && !strftrc ) {

        /* expand timebuffer */
        for ( i=0; i<TIMEBUF_INC; i++ )
            app_estrch( timebuf, '.' );

        D( fprintf( stderr, DHL "  timebuf: inc+%d\n", TIMEBUF_INC ) );

        /* output time */
        strftrc = strftime( estr2str( timebuf ), estrlen( timebuf ),
                            timefmt, localtime(&(hp->start_time)) );

    }

    if ( strftrc )
    {
        INFILEPOS *base = new_infilepos( hp->inpf );
        hsc_base_include_string( hp, SPECIAL_FILE_ID "insert TIME",
                            estr2str( timebuf ),
                            IH_PARSE_HSC | IH_NO_STATUS, base );
        del_infilepos( base );
    }

    del_estr( timebuf );

    return ( FALSE );
}

/*
** handle_hsc_text
**
** insert text
*/
BOOL handle_hsc_text( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR text = get_vartext_byname( tag->attr, "TEXT" );

    /* include text */
    INFILEPOS *base = new_infilepos( hp->inpf );
    hsc_base_include_string( hp, SPECIAL_FILE_ID "insert TEXT", text,
                             IH_PARSE_HSC | IH_NO_STATUS, base );
    del_infilepos( base );

    return ( FALSE );
}


/*
** hsc_insert
**
** main insert handle
*/
BOOL handle_hsc_insert( HSCPRC *hp, HSCTAG *tag )
{
    BOOL insert_text = FALSE;
    BOOL insert_time = get_varbool_byname( tag->attr, HSC_TIME_STR );

    if ( get_vartext_byname( tag->attr, HSC_TEXT_STR ) )
        insert_text = TRUE;

    if ( insert_text )
        handle_hsc_text( hp, tag );
    else if ( insert_time )
        handle_hsc_time( hp, tag );
    else {

        /* unknown option for $insert */
        hsc_message( hp, MSG_MISS_REQ_ATTR,
                     "required attribute for %t missing", HSC_INSERT_STR );
    }

    /* clear attributes */
    clr_varlist( tag->attr );

    return( FALSE );
}

/*
**-------------------------------------
** <$DEFTAG> define a new tag
**-------------------------------------
*/
BOOL handle_hsc_deftag( HSCPRC *hp, HSCTAG *tag )
{
    BOOL   ok = FALSE;
    BOOL   open_tag = FALSE;

    tag = def_tag_name( hp, &open_tag );
    ok = ( tag && def_tag_args( hp, tag ) );

    return ( FALSE );
}

/*
**-------------------------------------
** <$DEFENT> define a new entity
**-------------------------------------
*/
static VOID msg_illg_spch( HSCPRC *hp, STRPTR msg )
{
    hsc_message( hp, MSG_ILLG_SPCH,
                 "illegal special chararcter (%s)", msg );
}

BOOL handle_hsc_defent( HSCPRC *hp, HSCTAG *tag )
{
    STRPTR name = get_vartext_byname( tag->attr, "NAME" );
    STRPTR rplc = get_vartext_byname( tag->attr, "RPLC" );
    STRPTR nums = get_vartext_byname( tag->attr, "NUM" );
    LONG   num  = 0;

    if ( nums ) {

        if ( str2long( nums, &num ) )
            if ( strlen( rplc ) == 1 )
                if ( (num >= 160) && (num <= 255) ) {

                    DLNODE *nd = NULL;

                    nd = find_dlnode(
                        hp->defent->first, (APTR) name, cmp_strent );
                    if ( nd )
                        msg_illg_spch( hp, "duplicate NAME" );
                    else {

                        nd = find_dlnode(
                            hp->defent->first, (APTR) num, cmp_nument );
                        if ( nd )
                            msg_illg_spch( hp, "duplicate NUM" );

                    }

                    if ( !nd )
                        add_ent( hp->defent, name, rplc, num );

                } else
                    msg_illg_spch( hp, "illegal range for NUM" );
            else
                msg_illg_spch( hp, "RPLC not a single character" );
        else
            panic( "illegal NUM for <$defent>" );

    } else
        panic( "NUM missing for <$defent>" );

    return ( FALSE );
}

/*
**-------------------------------------
** <$DEFINE> create a new (global) attribute
**-------------------------------------
*/
BOOL handle_hsc_define( HSCPRC *hp, HSCTAG *tag )
{

    HSCVAR *attr =  define_var( hp, hp->defattr, 0 );

    if ( attr ) {

        /* set mci for local attribute */
        if ( attr->varflag & VF_GLOBAL )
            attr->macro_id = get_mci( hp );
        else
            attr->macro_id = MCI_GLOBAL;

        /* set new value (copy from default) if passed */
        if ( get_vardeftext( attr ) )
            clr_vartext( attr );

        /* remove default value */
        clr_attrdef( attr );

        DDA( prt_varlist( hp->defattr, "attributes after $DEFINE" ) );

        /* check for closing ">" */
        parse_gt( hp );

    }

    return ( FALSE );
}

/*
**-------------------------------------
** <$LET> set/reset/clear attribute value
**-------------------------------------
*/
BOOL handle_hsc_let( HSCPRC *hp, HSCTAG *tag )
{
    INFILE *inpf    = hp->inpf;
    STRPTR  varname = infgetw( inpf );
    HSCVAR *attr    = NULL;
    BOOL    ok      = FALSE;

    if ( varname ) {

        /* create temporary dummy attribute that is
        ** used to store the value, if attribute
        ** passed is a constant
        */
        HSCVAR *dummy = new_hscattr( "__dummy__" );

        /* find attribute */
        attr = find_varname( hp->defattr, varname );
        if ( attr ) {

            STRPTR eq_sign = infgetw( inpf );

            if ( attr->varflag & VF_CONST ) {

                /* tried to set constant */
                hsc_message( hp, MSG_ATTR_CONST,
                             "attempt to modify constant %A", attr );

                /* assign destination to dummy attribute */
                attr = dummy;
                dummy->vartype = attr->vartype;

            }


            /* check if a "=" comes next */
            if ( eq_sign && !strcmp( eq_sign, "=" ) )

                /* Y->get new value */
                eval_expression( hp, attr, NULL );

            else {

                /* N->clear or reset attribute to default value */
                clr_vartext( attr );
                inungetcw( inpf );     /* write previous word back */
                                       /*   (should be ">") */
            }

            DDA( prt_varlist( hp->defattr, "attributes after $LET" ) );

            /* check for closing ">" */
            ok = parse_gt( hp );

            /* remove dummy attribute */
            del_hscattr( dummy );

        } else
            hsc_msg_unkn_attr( hp, varname );

    }

    return ( FALSE );
}

/*
**-------------------------------------
** <$SOURCE> include a source part
**-------------------------------------
*/
BOOL handle_hsc_source( HSCPRC *hp, HSCTAG *tag )
{
    INFILE *inpf = hp->inpf;
    BOOL    pre     = get_varbool_byname( tag->attr, "PRE" );
    BOOL    ok      = TRUE;
    EXPSTR *bufstr = NULL;
    EXPSTR *srcstr  = NULL;
    LONG    nesting = 0;
    BYTE    state   = SRST_TEXT;
    STRPTR  nw      = NULL;
    INFILEPOS *base = new_infilepos( hp->inpf );

    /*
    ** read until </$SOURCE> found
    */

    /* init bufstr */
    bufstr = init_estr( ES_STEP_SOURCE );
    srcstr  = init_estr( ES_STEP_SOURCE );

    /* avoid nesting of <PRE> */
    if ( hp->inside_pre )
        pre = FALSE; /* TODO: lauch warning */

    /* insert leading <PRE> */
    if ( pre )
    {
        hsc_base_include_string( hp, SPECIAL_FILE_ID "include <PRE>", "<PRE>\n",
                                 IH_PARSE_HSC | IH_NO_STATUS, base );
    }


    while ( !((hp->fatal) || (state==SRST_CSOURCE) ) ) {

        /* read next word */
        if ( state == SRST_SLASH )
            nw = infget_tagid( hp );
        else if ( state != SRST_TAG )
            nw = infgetw( inpf  );

        if ( nw ) {

            if ( state == SRST_TAG ) {

                /*
                ** skip inside tags
                */
                BYTE   tag_state = TGST_TAG; /* state var passe to */
                                             /*     eot_reached() */

                do {

                    if ( eot_reached( hp, &tag_state ) )
                        state = SRST_TEXT;

                    app_estr( srcstr, infgetcws( inpf ) );
		    app_estr( srcstr, infgetcw( inpf ) );
		    
                } while ( (tag_state!=TGST_END) && !(hp->fatal) );

            } else {

                switch ( state ) {

                    case SRST_TEXT:
                        if ( !strcmp( nw, "<" ) )
                            state = SRST_LT;
                        break;

                    case SRST_LT:
                        if ( !strcmp( nw, "/" ) )
                            state = SRST_SLASH;
                        else if ( !upstrcmp( nw, HSC_COMMENT_STR ) ) {

                            state = SRST_COMT;

                        } else {

                            /* handle "<$SOURCE" (open source) */
                            if ( !upstrcmp( nw, HSC_SOURCE_STR ) )
                                nesting++; /* incr. source nesting */
                            state = SRST_TAG;

                        }
                        break;

                    case SRST_SLASH:
                        if ( !upstrcmp( nw, HSC_SOURCE_STR ) )

                            /* handle "</$SOURCE" (close source) */
                            if ( nesting )
                                nesting--;   /* decr. source nesting */
                            else
                                state = SRST_CSOURCE; /* end of source */
                        else
                            state = SRST_TAG;
                        break;

                }

                if ( state == SRST_TEXT ) {

                    /* append current white spaces & word to srcstr */
                    app_estr( srcstr, infgetcws( inpf ) );
                    app_estr( srcstr, infgetcw( inpf ) );

                } else if ( ( state == SRST_COMT )
                            || ( state == SRST_TAG ) )
                {

                    /* append bufstr to srcstr, clear bufstr,
                    ** append current word to srcstr
                    */
                    app_estr( srcstr, estr2str(bufstr) );
                    set_estr( bufstr, "" );
                    app_estr( srcstr, infgetcws( inpf ) );
                    app_estr( srcstr, infgetcw( inpf ) );

                }
                else
                {

                    /* append current white spaces & word to srcstr */
                    app_estr( bufstr, infgetcws( inpf ) );
                    app_estr( bufstr, infgetcw( inpf ) );
                }

                /*
                ** skip hsc comment
                */
                if ( state == SRST_COMT ) {

                    BYTE cstate = CMST_TEXT; /* vars for eoc_reached() */
                    LONG cnest  = 0;
                    BOOL end    = FALSE;     /* end of comment reached? */

                    while ( !end && !(hp->fatal) )
                    {
                        end = eoc_reached( hp, &cstate, &cnest );
                        app_estr( srcstr, infgetcws( inpf ) );
                        app_estr( srcstr, infgetcw( inpf ) );
                    }

                    state = SRST_TEXT; /* reset state after comment */
                }
            }
        } else {

            hsc_msg_eof( hp, "missing </" HSC_SOURCE_STR ">" );
            state = SRST_ERR;

        }

    } /* while */

    /* check for legal end state */
    if ( state == SRST_CSOURCE ) {

        ok = parse_wd( hp, ">" );

    }

    /* include source */
    if ( ok ) {

#if 0
        /* include pseudo-file */
        hsc_include_string( hp, SPECIAL_FILE_ID "source",
                            estr2str( srcstr ),
                            IH_PARSE_SOURCE | IH_NO_STATUS );

        /* append closing <PRE> */
        if ( pre )
            hsc_include_string( hp, SPECIAL_FILE_ID "source </PRE>",
                                "</PRE>",
                                IH_PARSE_HSC | IH_NO_STATUS );
#endif
        /* include pseudo-file */
        hsc_base_include_string( hp, SPECIAL_FILE_ID "source",
                            estr2str( srcstr ),
                            IH_PARSE_SOURCE | IH_NO_STATUS, base );

        /* insert tailing </PRE> */
        if ( pre )
        {
            del_infilepos( base );
            base = new_infilepos( hp->inpf );
            hsc_base_include_string( hp, SPECIAL_FILE_ID "include </PRE>", "</PRE>\n",
                                IH_PARSE_HSC | IH_NO_STATUS, base );
        }

    }

    del_infilepos( base );
    del_estr( bufstr );
    del_estr( srcstr );

    return ( FALSE );
}

