/*
** msgid.h
**
** defines for message-ids (warnings, errors,..)
**
** updated: 22-Oct-1995
** created: 30-Jul-1995
*/


#ifndef HSC_MSGID_H
#define HSC_MSGID_H

#include "ugly/types.h"

/* masks for message classes */
#define MASK_MESSAGE   0x0fff
#define MASK_MSG_CLASS 0xf000

/* message  classes */
#define MSG   0x00000 /* message */
#define STYLE 0x01000 /* bad style */
#define WARN  0x02000 /* warning */
#define ERROR 0x03000 /* error */
#define FATAL 0xff000 /* fatal error */

/*
**=========================
** next message number: 62
**=========================
**
** obsolete numbers:
** - none until now
*/

/*
** fatal errors
*/
#define MSG_NO_MEM             (FATAL+  1)   /* out of mem */
#define MSG_WRITE_ERR          (FATAL+  2)   /* write error */
#define MSG_UNEX_EOF           (FATAL+  3)   /* unexpected eof */
#define MSG_NO_OUTPUT          (FATAL+  4)   /* can't open output file */
#define MSG_NO_OUTFNAME        (FATAL+  5)   /* can't evaluate outp-filenm. */
#define MSG_NO_INPUT           (FATAL+  6)   /* can't open input file */
#define MSG_NO_CONFIG          (FATAL+ 40)   /* can't open config */

/*
** information messages
*/
#define MSG_STRIPPED_TAG       (MSG  +  7)   /* stripped tag with ext. href */

/*
** bad style messages
*/
#define MSG_WSPC_AROUND_TAG    (STYLE+  8)  /* white space around tag */
#define MSG_WRONG_HEADING      (STYLE+  9)  /* wrong heading */
#define MSG_CLICK_HERE         (STYLE+ 10)  /* click-here-syndrome */


/*
** messages within tags
*/
#define MSG_UNKN_TAG           (WARN + 11)  /* unknown tag */
#define MSG_TAG_TOO_OFTEN      (ERROR+ 12)  /* tag occured too often */
#define MSG_UNMA_CTAG          (ERROR+ 13)  /* unmatched closing tag */
#define MSG_CTAG_NESTING       (WARN + 14)  /* illegal closing tag nesting */
#define MSG_MISS_REQTAG        (WARN + 15)  /* required tag missing */
#define MSG_MISS_CTAG          (WARN + 16)  /* closing tag missing */
#define MSG_UNKN_TAG_OPTION    (ERROR+ 17)  /* unknown tag option */
#define MSG_TAG_OBSOLETE       (WARN + 37)  /* obsolete tag */
#define MSG_TAG_JERK           (WARN + 38)  /* jerk tag */
#define MSG_ILLG_WHTSPC        (ERROR+ 47)  /* illegal whitespace */
#define MSG_MBI                (ERROR+ 60)  /* must be inside */
#define MSG_NAW                (ERROR+ 61)  /* not allowed with */

/*
** messages within entities
*/
#define MSG_UNKN_ENTITY        (WARN + 18)  /* unknown entity */
#define MSG_EXPT_SEMIK         (ERROR+ 19)  /* ";" expected */
#define MSG_RPLC_ENT           (MSG  + 46)  /* replaced entity */
#define MSG_RPLC_SPCCHR        (MSG  + 52)  /* replaced special char */

/*
** messages within attributes
*/
#define MSG_UNKN_SYMB          (ERROR+ 20)  /* unknown symbol */
#define MSG_NO_URIPATH         (WARN + 21)  /* path to URI not found */
#define MSG_ARG_NO_QUOTE       (WARN + 22)  /* argument without quote */
#define MSG_EMPTY_SYMB_REF     (ERROR+ 23)  /* empty symbol reference */
#define MSG_UNKN_SYMB_REF      (ERROR+ 24)  /* unknown symbol reference */

#define MSG_UNEX_ATTR_TYPE     (ERROR+ 25)  /* illegal symbol type */
#define MSG_ILLEGAL_SYMB_TYPE  (ERROR+ 25)  /* TODO: remove this */

#define MSG_SYMB_2ND_DEFAULT   (ERROR+ 26)  /* default value already set */
#define MSG_SYMB_BOOL_DEFAULT  (ERROR+ 27)  /* default vaule for bool */
#define MSG_ENUM_UNKN          (ERROR+ 35)  /* unknown enum value */
#define MSG_ENUM_SUSPICIOUS    (WARN + 36)  /* suspicios enum value */
#define MSG_MISSING_ATTR       (ERROR+ 39)  /* required attr. missing */
#define MSG_ARG_BOOL_ATTR      (ERROR+ 41)  /* argument for bool attr */
#define MSG_NOARG_ATTR         (ERROR+ 42)  /* attr requires arg */
#define MSG_UNKN_ATTR_OPTION   (ERROR+ 43)  /* unknown attr option */
#define MSG_MISS_REQ_ATTR      (ERROR+ 44)  /* required attr missing */

/*
** messages from tag handles
*/
#define MSG_EXPT_H1            (STYLE+ 28)  /* Hx: first heading H1 */
#define MSG_ANCH_NO_NMHR       (ERROR+ 29)  /* A : no NAME or HREF */
#define MSG_CL_MACR_ARG        (ERROR+ 46)  /* args for closing macro */
#define MSG_LF_IN_COMMENT      (WARN + 48)  /* LF within comment */
#define MSG_GT_IN_COMMENT      (WARN + 49)  /* GT within comment */
#define MSG_CMTEND_QUOTE       (WARN + 50)  /* SGML-comment ends in quotes */
#define MSG_ZERO_COMMENT       (WARN + 55)  /* SGML zero comment */
#define MSG_ONEW_COMMENT       (WARN + 56)  /* SGML one word comment */

/*
** messages from hsc-tag handles
*/
#define MSG_UNMA_ELSE          (ERROR+ 53)  /* unmatched $ELSE */
#define MSG_SYSTEM_RETURN      (WARN + 54)  /* external process result <> 0 */
#define MSG_DEFTAG_NO_OPEN     (FATAL+ 57)  /* no opening tag for deftag cl.*/
#define MSG_REDEFINE_TAG       (FATAL+ 58)  /* tried to redefine tag */
#define MSG_REDEFINE_MACRO     (WARN + 59)  /* redefined & replaced macro */

/*
** misc. messages
*/
#define MSG_UNMA_GT            (ERROR+ 30)  /* unmatched ">" */
#define MSG_UNEXPT_CH          (ERROR+ 31)  /* unexpected char */
#define MSG_ILLEGAL_NUM        (ERROR+ 32)  /* illegal numeric value */
#define MSG_STR_EOL            (ERROR+ 33)  /* string exeeds line */
#define MSG_UNEX_EOL           (ERROR+ 34)  /* unexpected eol */
#define MSG_UNMA_QUOTE         (WARN + 51)  /* quote inside text */

/*
** soon obsolete errors (hopefully)
*/
#define FATAL_LONG_STR      (FATAL+300) /* string too long */
#define MSG_HSC_ERR         (ERROR+301) /* error within hsc-tag */


#endif
