/*
** ugly/args_prp.c
**
** ugly argument preparation functions
**
** updated:  9-Aug-1995
** created:  3-Jul-1994
**
**===================================================================
** TODO:
** - free all memory in error case
**
*/

/*
** includes
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "memory.h"
#include "string.h"
#include "dllist.h"

#define NOEXTERN_UGLY_ARGS_H
#include "args.h"


/*
** error vars
*/
LONG   prep_error_num;                 /* error number */
int    prep_error_idx;                 /* # of template causing error */
size_t prep_error_pos;                 /* pos in arg */
BOOL   no_preperr, any_preperr;        /* error flags for _prepare_args() */


/*
** error functions for _prepare_arg()
**
*/
void set_preperr( LONG num, size_t pos )
{
    if ( no_preperr ) {

        no_preperr  = FALSE;
        any_preperr = TRUE;
        prep_error_num = num;
        prep_error_pos = pos;

    }
}

void clr_preperr( void )
{
    no_preperr  = TRUE;
    any_preperr = FALSE;
    prep_error_num = 0;
    prep_error_pos = 0;
}

/*
** fprintf_arginfo (debugging aid)
*/
void fprintf_arginfo( FILE *stream, APTR data )
{
    struct arginfo *arg = (struct arginfo *) data;

    fprintf( stream, "  %-15s %2x %2x", 
             arg->ai_id, arg->ai_type, arg->ai_flags );
    if ( arg->ai_help )
        printf("  \"%s\"", arg->ai_help );
    fprintf( stream, "\n" );
}


/*
** prepare_args
**
** convert argument definitions to dllist of struct arginfo
**
*/
struct arglist *prepare_args( STRPTR arglist_name, ... )
{
    va_list         ap;
    struct dllist  *newlist;
    struct arginfo *newarg;
    struct arglist *new_arglist = NULL;

    clr_preperr();                     /* clear error vars */

    /*
    ** alloc & init _new_arglist
    */
    new_arglist = ( struct arglist *)
        malloc( sizeof( struct arglist ) );

    if ( new_arglist ) {

        new_arglist->al_name  = arglist_name;
        new_arglist->al_list  = NULL;
        new_arglist->al_multiple = NULL;
        new_arglist->al_nokeywd  = NULL;


    } else
        set_preperr( APE_NO_MEM, 0);

    /*
    ** TODO: don't go into if(), when new_arglist failed
    */

    /*
    ** convert template to double linked list
    */
    newlist = init_dllist( del_arginfo );

    if ( newlist ) {
        
        STRPTR nxtdef = NULL;          /* next template definition */

        va_start( ap, arglist_name );

        do {
            
            ufree( nxtdef );
            nxtdef = va_arg( ap, STRPTR );       /* get next definition */

            /* clone nxtdef: this is necessary because on unix-systems, */
            /* you can not let strtok() run on strings received with    */
            /* va_arg() -> bus error                                    */
            nxtdef = strclone( nxtdef );                
            prep_error_idx++;

            if ( nxtdef ) {

                newarg = malloc( sizeof( struct arginfo) );

                if ( newarg ) {

                    STRPTR new_id;
                    STRPTR typestr;
                    STRPTR flagstr;
                    STRPTR enumstr;    /* enum string for type 'E' */
                    LONG   new_type  = 0;
                    LONG   new_flags = 0;
                    LONG   rnlolim   = 0;
                    LONG   rnhilim   = 0;

                    /* init _newarg */
                    newarg->ai_id    = NULL;
                    newarg->ai_type  = 0;
                    newarg->ai_flags = 0;
                    newarg->ai_misc1 = NULL;
                    newarg->ai_misc2 = NULL;
                    newarg->ai_dest  = NULL;
                    newarg->ai_func  = NULL;
                    newarg->ai_help  = NULL;


                    new_id = strtok( nxtdef, "/" );

                    typestr = strtok( NULL, "/" );

                    /*
                    ** get argument type
                    */
                    if ( typestr ) {

                        if ( strlen( typestr ) == 1 ) {

                            switch ( toupper( typestr[0] ) ) {

                                case 'T':
                                    new_type = ARG_TEXT; break;

                                case 'S':
                                    new_type = ARG_SWITCH; break;

                                case 'N':
                                    new_type = ARG_LONG; break;

                                case 'R':
                                    new_type = ARG_LONG_RANGE; break;

                                case 'E':
                                    new_type = ARG_ENUM; break;

                            }

                            if ( new_type )      /* arg-type specified? */
                                flagstr =        /* Y-> get next flag */
                                    strtok( NULL, "/" );
                            else {

                                flagstr = typestr;   /* N-> must be flag */
                                new_type = ARG_TEXT; /*     set type to text */

                            }

                        } else
                            set_preperr( APE_INVALID_TEMPLATE, 0 );

                    } else {                     /* no type at all */

                        flagstr = NULL;          /* -> no flags also */
                        new_type = ARG_TEXT;     /*    set type to text */

                    } /* if typestr */

                    /*
                    ** get argument flags
                    */
                        while ( flagstr && flagstr[0] && no_preperr ) {

                            switch ( toupper( flagstr[0] ) ) {

                                case 'M':
                                    new_flags |= ARG_MULTIPLE;
                                    break;

                                case 'A':
                                    new_flags |= ARG_REQUIRED;
                                    break;

                                case 'K':
                                    new_flags |= ARG_KEYWORD;
                                    break;

                                case 'O':
                                    new_flags |= ARG_OVERWRITE;
                                    break;

                                case '$':
                                    new_flags |= ARG_HANDLEFUNC;
                                    break;

                                default:
                                    set_preperr( APE_ILLEGAL_FLAG, 0);
                                    break;

                            } /* switch */

                            flagstr = strtok( NULL, "/" );

                        } /* while */

                    if ( no_preperr ) {

                        /*
                        ** get handle function
                        */
#if 0
                        if ( new_flags || ARG_HANDLEFUNC ) {

                            newarg->ai_func = va_arg( ap, (*STRPTR)(STRPTR) );

                        }
#endif
                        /*
                        ** get range limits
                        */
                        if ( new_type == ARG_LONG_RANGE ) {

                            rnlolim = va_arg( ap, LONG );
                            rnhilim = va_arg( ap, LONG );

                        }

                        /*
                        ** get enum string
                        */
                        if ( new_type == ARG_ENUM ) {

                            enumstr = va_arg( ap, STRPTR );

                        }

                        if ( no_preperr ) {

                            newarg->ai_id   = strclone( new_id ); /* sux */
                            newarg->ai_type = new_type;
                            newarg->ai_flags = new_flags;
                            newarg->ai_dest = va_arg( ap, APTR );
                            newarg->ai_help = 
                                strclone( va_arg( ap, STRPTR ) );

                            /*
                            ** set additional argument information
                            ** (->misc1, ->misc2)
                            */
                            switch ( new_type ) {

                                case ARG_ENUM:
                                    newarg->ai_misc1 = ( APTR )
                                        strclone( enumstr );
                                    if ( !(newarg->ai_misc1) )
                                        set_preperr( APE_NO_MEM, 0 );
                                    break;

                                case ARG_LONG_RANGE:
                                    newarg->ai_misc1 = ( APTR ) rnlolim;
                                    newarg->ai_misc2 = ( APTR ) rnhilim;
                                    break;

                            }

                            /*
                            ** check for NULL as destination var
                            */
                            if ( newarg->ai_dest == NULL )
                                set_preperr( APE_DESTVAR_IS_NULL, 0 );

                        }

                        /*
                        ** check, if arg multiple arg without keyword
                        */
                        if ( new_flags & ARG_MULTIPLE )
                            if ( !(new_flags & ARG_KEYWORD) )
                                if ( new_arglist->al_multiple )
                                    set_preperr( APE_DOUBLE_MULTIPLE, 0 );
                                else
                                    new_arglist->al_multiple = newarg;

                        /*
                        ** append new argument entry to list
                        */
                        if ( no_preperr )
                            if ( app_dlnode( newlist, (APTR) newarg )
                                 == NULL )
                                set_preperr( APE_NO_MEM, 0 );

                        /* free _newarg if any error occured */
                        if ( any_preperr )
                            del_arginfo( (APTR) newarg );


                    }

                } else
                    set_preperr( APE_NO_MEM, 0 );
                
            }
            
        } while ( nxtdef && no_preperr );
        
        /* free last value of nxtdef */
        ufree( nxtdef );
        
        while ( nxtdef )                         /* flush arguments */
            nxtdef = va_arg( ap, STRPTR );       /* (in error case) */

        va_end( ap );


    } else
        set_preperr( APE_NO_MEM, 0 );

    /*
    ** check for empty template
    */
    if ( no_preperr )
        if ( newlist->first == NULL )
            set_preperr( APE_EMPTY_TEMPLATE, 0 );

    /*
    ** free newlist, if any error occured
    */
    if ( any_preperr ) {

        /* free newlist */
        del_dllist( newlist );
        newlist = NULL;
        new_arglist = NULL;

    } else
        new_arglist->al_list = newlist;

    return new_arglist;

}

