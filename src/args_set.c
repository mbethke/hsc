/*
** ugly/args_set.c
**
** ugly set arguments handling functions
**
** updated:  3-Nov-1995
** created:  3-Jul-1994
**
**===================================================================
** TODO:
** - support ARG_LONG_RANGE
** - support ARG_INV_SWITCH (set switch value to FALSE if NOxxx found)
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
** global export vars
*/
int    arg_error_num;                  /* error number */
STRPTR arg_error_arg;                  /* argument causing error */
STRPTR arg_error_hfs = NULL;           /* error string returned by handle function */


/*
** result string for _strargerr()
*/
#define SIZE_ARGERRSTR 60
char argerrstr[ SIZE_ARGERRSTR ];

/*
** local global vars
*/
BOOL no_argerr,  any_argerr;           /* error flags for _set_args() */
int  argidx = -1;                      /* index in _argv[] currently examing */

/*
** compare_arginfo
**   ( called by _set_args() )
**
*/
int compare_arginfo( APTR cmp_data, APTR list_data )
{
    struct arginfo *ai      = (struct arginfo *) list_data;
    STRPTR          lst_arg = strclone( ai->ai_id );                /* TODO: possible enforcer hit */
    STRPTR          cmp_arg = ((struct arginfo *) cmp_data)->ai_id; /* TODO: possible enforcer hit */
    BOOL            eq      = FALSE;
    char            lastch;

    if ( lst_arg && cmp_arg ) {

        STRPTR nxt_arg = strtok( lst_arg, "=" );

        while ( nxt_arg && ( eq == FALSE ) ) {

            if ( ai->ai_flags & ARG_CASESENS )
                
                eq = ( strncmp( nxt_arg, cmp_arg, strlen( nxt_arg ) ) == 0 );

            else

                eq = ( upstrncmp( nxt_arg, cmp_arg, strlen( nxt_arg ) ) == 0 );

#if 0
            if ( eq ) {

                if ( ai->ai_type != ARG_SWITCH )
                    eq = ( cmp_arg[ strlen( nxt_arg ) ] == '=' );

            }
#endif
            lastch = cmp_arg[ strlen( nxt_arg ) ];
            eq &= ( lastch=='\0' ) || ( lastch=='=' );

            nxt_arg = strtok( NULL, "=" );

        } /* while */

        ufree( lst_arg );

    } else
        eq = FALSE;

    return eq;
}


/*
** clr_ai_set
**
** clear all ai_set bits in all _arginfo entries of an _arglist
**
*/
void clr_ai_set( struct arglist *al )
{
    struct dlnode  *nd;
    struct arginfo *ai;

    if ( al )
        if ( al->al_list )
            if ( nd = al->al_list->first )
                while ( nd ) {

                    if ( ai = ( struct arginfo * ) nd->data )
                        ai->ai_set = FALSE;
                    nd = nd->next;

                }
}


/*
** reset_nokeywd
**
** set al_nokeywd to first entry w/o ARG_KEYWORD flag
**
*/
void reset_nokeywd( struct arglist *al )
{
    /*
    ** NOTE: _al must be fully initialised _arglist,
    **       otherwise _set_args() would have rejeceted
    **       it. therefor no "al == NULL" check is done
    **       when setting _nd and _ai.
    */
    struct dlnode  *nd  = al->al_list->first;
    struct arginfo *ai = ( struct arginfo * ) nd->data;

    if ( ai->ai_flags & ARG_KEYWORD || (ai->ai_type & ARG_SWITCH) ) {

        al->al_nokeywd = NULL;

        do {

            if ( (ai->ai_flags & ARG_KEYWORD)
                 || (ai->ai_type & ARG_SWITCH) )
            {

                nd = nd->next;
                if ( nd )
                    ai = ( struct arginfo * ) nd->data;

            } else
                al->al_nokeywd = ai;

        } while ( nd && !( al->al_nokeywd ) );

    } else
        al->al_nokeywd = ai;

}

/*
** find_nxt_nokeywd
**
** find next entry w/o ARG_KEYWORD flag set
**
*/
void find_nxt_nokeywd( struct arglist *al )
{
    if ( al->al_nokeywd ) {

        /*
        ** find entry in al_list pointing to al_nokeywd 
        */
        struct dlnode  *nd = al->al_list->first;
        struct arginfo *ai;

        while ( al->al_nokeywd != ( struct arginfo * ) nd->data )
            nd = nd->next;

        /*
        ** find next entry in al_list w/o ARG_KEYWORD
        */
        nd = nd->next;
        al->al_nokeywd = NULL;
        while ( nd && ( al->al_nokeywd == NULL ) ) {

            ai = ( struct arginfo * ) nd->data;

            if ( (ai->ai_flags & ARG_KEYWORD)
                 || (ai->ai_type & ARG_SWITCH)
            )
                nd = nd->next;
            else
                al->al_nokeywd = ai;


        };

    }

}


/*
**
** error functions for _set_args()
**
*/
void set_argerr( int num, STRPTR arg )
{
    if ( no_argerr ) {

        no_argerr = FALSE;
        any_argerr = TRUE;

        arg_error_num = num;
        arg_error_arg = arg;

    }

}

void clr_argerr( void )
{
    no_argerr  = TRUE;
    any_argerr = FALSE;
    arg_error_num = 0;
    arg_error_arg = NULL;
}


/*
** strargerr
**
** returns string containing detailed error message of
** error occured in last call of _set_args()
**
*/
STRPTR strargerr( void )
{
    STRPTR str = "unknown error";

    switch ( arg_error_num ) {

        case ASE_NO_MEM:
            str = "out of memory"; break;

        case ASE_OCCURED_TWICE:
            str = "argument occured twice"; break;

        case ASE_UNKNOWN_KEYWORD:
            str = "unknown keyword"; break;

        case ASE_INVALID_NUM:
            str = "invalid numeric argument"; break;

        case ASE_INVALID_ENUM:
            str = "invalid enumerator value"; break;

        case ASE_REQUIRED_MISS:
            str = "required argument missing"; break;

        case ASE_EMPTY_TEMPLATE:
            str = "empty template"; break;

        case ASE_OUT_OF_RANGE:
            str = "numeric argument out of range"; break;
        case ASE_NO_VAL_AFTER_KW:
            str = "value after keyword missing"; break;

        case ASE_HANDLE_FUNC:          /* error in handle function */
            str = arg_error_hfs; break;

    } /* switch */

    strcpy( argerrstr, str );
    if ( arg_error_arg ) {

        strncat( argerrstr, ": ", SIZE_ARGERRSTR-strlen( str )-1 );
        strncat( argerrstr, arg_error_arg, SIZE_ARGERRSTR-strlen( str )-3 );

    }

    if ( arg_error_num )
        return argerrstr;
    else
        return NULL;
}

/*
** pargerr
**
** printf error message from last call of _set_args()
**
*/
void pargerr( void )
{
    if ( arg_error_num )
        fprintf( stderr, "%s\n", strargerr() );
}


/*
** set_arg_value
**
** sets argument _ai->ai_dest with value specified in _arg
**
*/
UBYTE set_arg_value( struct arginfo *ai, STRPTR arg, STRPTR arg2, BOOL keywd )
{
    APTR   dest     =   ai->ai_dest;
    STRPTR param;
    UBYTE  arg_incr =   0;             /* set to 1 if arg2 is used */
    BOOL   arg2used =   FALSE;

    /* evaluate parameter:                                 */
    /*   if arg is equal to arg-id of ai (no other chars   */
    /*   following), the param is taken from the next arg. */
    /*   otherwise, the arg is scanned for '=' and the     */
    /*   rest of the arg is taken as param                 */
    if ( keywd && !( ai->ai_type == ARG_SWITCH ) ) {

        param = arg;
        while ( param[0] && ( param[0] != '=' ) )
            param++;
        if ( param[0] )
            param++;
        else {
            param    = arg2;
            arg2used = TRUE;
            if ( !param )
                set_argerr( ASE_REQUIRED_MISS, arg );
        }

    } else
        param = arg;


    /*
    ** set switch/arg-value
    */
    if ( no_argerr ) {

        if ( ai->ai_func ) {

            /* call handle function with arg value */
            arg_error_hfs  =  (*(ai->ai_func))( param );
            if ( arg_error_hfs )
                set_argerr( ASE_HANDLE_FUNC, param );

        } else if ( ai->ai_type == ARG_SWITCH )         /* switch */

            *( (BOOL *) dest )  = TRUE;

        else {

            /*
            ** check if argument already set
            */
            if ( ai->ai_set && !( (ai->ai_flags & ARG_OVERWRITE)
                                  || (ai->ai_flags & ARG_MULTIPLE) )
               )
                set_argerr( ASE_OCCURED_TWICE, arg );
            else 
                ai->ai_set = TRUE;

            if ( no_argerr ) {

                APTR aparam = NULL;
                DLLIST **dest_list = (DLLIST**)dest;
                LONG along;

                /*
                ** get new value and  store it in aparam
                */
                if ( !param )                              /* missing param */
                    set_argerr( ASE_NO_VAL_AFTER_KW, arg );
                if ( ai->ai_type == ARG_TEXT )             /* text */
                    aparam = (APTR) param;
                else if ( ai->ai_type == ARG_LONG ) {      /* long */

                    if ( !str2long( param, &along ) )
                        set_argerr( ASE_INVALID_NUM, arg );
                    else
                        aparam = (APTR) along; /* what a pervert! */

                } else if ( ai->ai_type == ARG_ENUM ) {

                    LONG aenum = strenum( param, ai->ai_misc1.ai_enum,
                                          '|', STEN_NOCASE );

                    if ( !aenum )
                        set_argerr( ASE_INVALID_ENUM, arg );
                    else
                        aparam = (APTR) aenum; /* what a pervert! */

                }
#if 0
                if ( !param )                              /* missing param */
                    set_argerr( ASE_NO_VAL_AFTER_KW, arg );
                if ( ai->ai_type == ARG_TEXT )             /* text */
                    *( ( STRPTR *) dest ) = param;
                else if ( ai->ai_type == ARG_LONG ) {      /* long */

                    if ( !str2long( param, ( LONG * ) dest ) )
                        set_argerr( ASE_INVALID_NUM, arg );
                }
#endif

                /*
                ** set new value
                */
                if ( no_argerr )
                    if ( ai->ai_flags & ARG_MULTIPLE ) {

                        if ( !(*dest_list) )
                            *dest_list = init_dllist( NULL );
                        if ( *dest_list ) {
                            if ( !app_dlnode( *dest_list, aparam ) )
                                set_argerr( APE_NO_MEM, arg );
                        } else
                            set_argerr( APE_NO_MEM, arg );

                    } else {

                        if ( ai->ai_type == ARG_LONG )
                            *((LONG*) dest) = (LONG) aparam;
                        else if ( ai->ai_type == ARG_ENUM )
                            *((LONG*) dest) = (LONG) aparam;
                        else if ( ai->ai_type == ARG_TEXT )
                            *((STRPTR*) dest) = (STRPTR) aparam;

                    }

            }
        
        }

       if ( arg2used )        /* set return value that arg2 */
           arg_incr = 1;      /* is skipped outside this func */

    }


    return ( arg_incr );
}



/*
** check_required_set
**
** check, if all required arguments are set
**
*/
void check_required_set( struct arglist *al )
{
    struct dlnode  *nd = al->al_list->first;
    struct arginfo *ai;

    do {

        ai = ( struct arginfo *) nd->data;

        if ( ( ai->ai_flags & ARG_REQUIRED ) && !(ai->ai_set) )
            set_argerr( ASE_REQUIRED_MISS, ai->ai_id );
        else
            nd = nd->next;

    } while ( nd && no_argerr );

}


/*
** set_args
**
** check and set arguments according to template specified in _al
**
** params: argc, argv[]...arguments from _main
**         al.............argument template (result from _prepare_args())
** result: TRUE, if no error occured
** errors: return FALSE, error information in _arg_error_???
**
*/
BOOL set_args( int argc, char *argv[], struct arglist *al )
{

    clr_argerr();
    clr_ai_set( al );
    reset_nokeywd( al );

    argidx = 1;

    if ( al == NULL )
        set_argerr( ASE_EMPTY_TEMPLATE, argv[ argidx ] );
    else if ( al->al_list == NULL )
        set_argerr( ASE_EMPTY_TEMPLATE, argv[ argidx ] );

    while ( ( argidx < argc ) && no_argerr ) {

        struct arginfo  search_ai;
        struct arginfo *found_ai;
        struct dlnode  *ainode;
        STRPTR          arg2 = NULL;   /* passed to set_arg_value */

        /*
        ** search for entry in arginfo-list
        */
        search_ai.ai_id = argv[ argidx ];
        ainode = find_dlnode( al->al_list->first, &search_ai, compare_arginfo );

        /* evaluate next argument (passed to set_arg_value) */
        /* TODO: check if "<" or "<=" */
        if ( argidx < argc-1 )
            arg2 = argv[ argidx+1 ];

        if ( ainode ) {

            found_ai = (struct arginfo *) ainode->data;

            argidx += set_arg_value( found_ai, argv[ argidx ], arg2, TRUE );

        } else {

            if ( al->al_nokeywd ) {
                
                argidx += set_arg_value( al->al_nokeywd, argv[ argidx ], arg2, FALSE );
                find_nxt_nokeywd( al );

            } else
                set_argerr( ASE_UNKNOWN_KEYWORD, argv[ argidx ] );

        }

        argidx++;

    } /* while*/

    /* 
    ** check, if all required arguments were set 
    */
    if ( no_argerr )
        check_required_set( al );

    return no_argerr;

}

