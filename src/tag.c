/*
** tag.c
**
** Copyright (C) 1995  Thomas Aglassinger <agi@sbox.tu-graz.ac.at>
**
** hsc-tag funcs for hsc
**
** updated: 20-Nov-1995
** created:  8-Sep-1995
**
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "ugly/types.h"
#include "ugly/dllist.h"
#include "ugly/memory.h"
#include "ugly/infile.h"
#include "ugly/string.h"

#include "global.h"
#include "error.h"
#include "msgid.h"
#include "tagargs.h"

#include "vars.h"

#define NOEXTERN_HSC_TAG
#include "tag.h"

/*
** global vars
*/
DLLIST *deftag  = NULL;                /* list for defined tags */
DLLIST *cltags  = NULL;                /* history for closing tags */

STRPTR last_anchor = NULL;             /* stores prev URL of last anchor; */
                                       /* used within tag_a.c, but must be */
                                       /* released within cleanup.c */

/*
**
** global funs
**
*/

/*
**-------------------------------------
** constructor/destructor
**-------------------------------------
*/

/*
** new_hsctag
**
** alloc & init a new hsctag
*/
HSCTAG *new_hsctag( STRPTR newid )
{

    HSCTAG *newtag = (HSCTAG*) umalloc( sizeof(HSCTAG) );

    if (newtag) {

        /* init new tag item */
        newtag->name        = upstr( strclone(newid) );        /* set id */
        newtag->option      = 0;
        newtag->vers        = 0;
        newtag->o_handle    = NULL;       /* no handle functions */
        newtag->c_handle    = NULL;
        newtag->occured     = FALSE;
        newtag->op_text     = NULL;
        newtag->cl_text     = NULL;
        newtag->attr        = init_dllist( del_var );
        newtag->mbi         = NULL;
        newtag->naw         = NULL;

        if ( !( newtag->name && newtag->attr ) ) {

            del_tag( newtag );
            newtag = NULL;
        }

    }

    return (newtag);
}

/*
** del_tag
*/
void del_tag( APTR data )
{
    HSCTAG *tag = (HSCTAG *)data;

    /* release mem */
    ufreestr( tag->name );
    ufreestr( tag->name );
    del_estr( tag->op_text  );
    del_estr( tag->cl_text  );
    ufreestr( tag->mbi );
    ufreestr( tag->naw );
    del_dllist( tag->attr );

    /* clear values */
    tag->option     = 0;
    tag->vers       = 0;
    tag->op_text    = NULL;
    tag->cl_text    = NULL;
    tag->attr       = NULL;

    ufree( tag );
}



/*
**---------------------------
** find tag string
**---------------------------
*/

/*
** cmp_strtag
**
** compares a tag-string with the name
** of a HSCTAG-entry
*/
int cmp_strtag( APTR cmpstr, APTR tagdata )
{
    STRPTR tagstr = NULL;

    if ( tagdata )
        tagstr = ((HSCTAG*)tagdata)->name;

    if (tagstr)
        if ( !upstrcmp( cmpstr, tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}

/*
** find_strtag
*/
HSCTAG *find_strtag( DLLIST *taglist, STRPTR name )
{
    DLNODE *nd = find_dlnode( taglist->first, (APTR) name, cmp_strtag );
    HSCTAG *tag = NULL;

    if ( nd )
        tag = (HSCTAG*)nd->data;

    return ( tag );
}

#if 0
/*
**---------------------------
** find closing tag string
**---------------------------
*/

/*
** cmp_strctg
**
** compares a tag-string with the name
** of closing tag (which is a STRPTR, but
** has been converted to APTR)
*/
int cmp_strctg( APTR cmpstr, APTR tagstr )
{
    if (tagstr)
        if ( !upstrcmp( (STRPTR) cmpstr, (STRPTR) tagstr ) )
            return (-1);
        else
            return (0);
    else
        return (0);
}
#endif

/*
**---------------------------
** remove closing tag
**---------------------------
** params: tagname..tag to remove
**         check....show messages
*/
VOID remove_ctag( HSCTAG *tag, INFILE *inpf )
{
    /* search for tag on stack of occured tags */
    DLNODE *nd = find_dlnode( cltags->first, (APTR) tag->name, cmp_strtag );
    if ( nd == NULL ) {

        /* closing tag not found on stack */
        /* ->unmatched closing tag without previous opening tag */
        if ( !( tag->option & HT_SMARTCLOSE ) ) {

            message( MSG_UNMA_CTAG, inpf );
            errstr( "unmatched closing tag " );
            errctag( tag->name );
            errlf();

        }

    } else {

        /* closing tag found on stack */
        HSCTAG *ctag    = (HSCTAG*) nd->data;
        STRPTR  foundnm = (STRPTR)  ctag->name;
        STRPTR  lastnm  = (STRPTR)  cltags->last->data;

        /* check if name of closing tag is -not- equal
        /* to the name of the last tag last on stack */
        /* ->illegal tag nesting */
        if ( upstrcmp(lastnm, foundnm)
             && !(tag->option | HT_MACRO)
             && !(is_hsc_tag( tag )) )
        {

            message( MSG_CTAG_NESTING, inpf );
            errstr( "illegal closing tag nesting (expected " );
            errctag( lastnm );
            errstr( ", found " );
            errctag( tag->name );
            errch( ')' );
            errlf();

        }

        /* if closing tag has any attributes defined,
        ** it must be a closing macto tag. so copy
        ** the attributes of the closing tag to the
        ** attributes of the macro tag. therefor,
        ** the closing macro tag inherits the
        ** attributes of his opening macro
        */
        if ( ctag->attr )
            set_local_varlist( tag->attr, ctag->attr, MCI_APPCTAG );

        /* remove node for closing tag from cltags-list */
        del_dlnode( cltags, nd );

    }
}

/*
**-------------------------------------
** append tag functions
**-------------------------------------
*/

/*
** app_tag
**
** create a new tag and append it to tag-list
**
** params: tagid..name of the new tag (eg "IMG")
** result: ptr to the new tag or NULL if no mem
*/
HSCTAG *app_tag( DLLIST *taglist, STRPTR tagid )
{
    HSCTAG *newtag;

    newtag = new_hsctag( tagid );
    if (app_dlnode( taglist, newtag ) == NULL ) {

        del_tag( (APTR) newtag );
        newtag = NULL;

    }

    return (newtag);
}

/*
** app_ctag
**
** create closing tag and append it to tag-list;
** also clone options & attribute list of parent
** tag, if tag is a macro and has a closing tag.
**
** params: tagid..name of the new tag (eg "IMG")
** result: ptr to the new tag or NULL if no mem
*/
HSCTAG *app_ctag( DLLIST *taglist, HSCTAG *tag )
{
    HSCTAG *ctag;

    ctag = new_hsctag( tag->name );
    if ( ctag ) {

        BOOL    ok = TRUE;
        DLNODE *nd = NULL;

        /* copy important data of tag */
        ctag->option = tag->option;

        /* clone attributes, if tag is a
        ** macro tag and has a closing tag
        */
        if ( (tag->option & HT_MACRO)
             && (tag->option & HT_CLOSE) )
        {

            ok = copy_local_varlist(
                     ctag->attr, tag->attr, MCI_APPCTAG );

        }

        /* insert tag in list */
        if ( ok ) {

            nd = app_dlnode( taglist, ctag );
            if ( !nd ) {

                del_tag( (APTR) ctag );
                ctag = NULL;

            }
        }
    }

    return (ctag);
}


