/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * hsclib/msgid.h
 *
 * defines for message-ids (warnings, errors,..)
 *
 * updated: 24-Mar-2003
 * created: 30-Jul-1995
 */

#ifndef HSC_MSGID_H
#define HSC_MSGID_H

#include "ugly/utypes.h"

/* masks for message classes */
#define MASK_MESSAGE   0x0fff
#define MASK_MSG_CLASS 0xf000

/* message  classes */
#define MSG_NOTE  0x00000       /* message */
#define MSG_STYLE 0x01000       /* bad style */
#define MSG_PORT  0x02000       /* portability prolem */
#define MSG_WARN  0x03000       /* warning */
#define MSG_ERROR 0x04000       /* error */
#define MSG_FATAL 0x0f000       /* fatal error */
#define MSG_NONE  0xff000       /* out of msg-array */

/*
 *=========================
 * next message number
 *=========================
 */
#define MAX_MSGID (93-1)

/*
 * obsolete numbers:
 * - see below
 * CURRENTLY NONE
 */

/*
 * fatal errors
 */
#define MSG_NEW_PRJFILE        (MSG_WARN +  1)  /* creating new project file */
#define MSG_CORRUPT_PRJFILE    (MSG_FATAL+  2)  /* corrupt project file */
#define MSG_UNEX_EOF           (MSG_FATAL+  3)  /* unexpected eof */
#define MSG_READ_ERROR         (MSG_FATAL+  4)  /* read error */
#define MSG_TOO_MANY           (MSG_FATAL+  5)  /* too many messages/errors */
#define MSG_NO_INPUT           (MSG_FATAL+  6)  /* can't open input file */
#define MSG_ARITHMETIC_ERROR   (MSG_FATAL+  8)  /* arithmetic error */

/*
 * information messages
 */
#define MSG_TAG_STRIPPED       (MSG_NOTE +  7)  /* stripped tag with ext. href */
#define MSG_TAG_CANT_STRIP     (MSG_WARN + 70)  /* can't strp special tag */

/*
 * bad style messages
 */
#define MSG_WRONG_HEADING      (MSG_STYLE+  9)  /* wrong heading */
#define MSG_CLICK_HERE         (MSG_STYLE+ 10)  /* click-here-syndrome */

/*
 * messages within tags
 */
#define MSG_UNKN_TAG           (MSG_WARN + 11)  /* unknown tag */
#define MSG_TAG_TOO_OFTEN      (MSG_ERROR+ 12)  /* tag occured too often */
#define MSG_UNMA_CTAG          (MSG_ERROR+ 13)  /* unmatched end-tag */
#define MSG_CTAG_NESTING       (MSG_WARN + 14)  /* illegal end-tag nesting */
#define MSG_MISS_REQTAG        (MSG_ERROR+ 15)  /* required tag missing */
#define MSG_MISS_RCMDTAG       (MSG_STYLE+ 67)  /* recommended tag missing */
#define MSG_MISS_CTAG          (MSG_WARN + 16)  /* end-tag missing */
#define MSG_UNKN_TAG_OPTION    (MSG_ERROR+ 17)  /* unknown tag option */
#define MSG_TAG_OBSOLETE       (MSG_WARN + 37)  /* obsolete tag */
#define MSG_TAG_JERK           (MSG_WARN + 38)  /* jerk tag */
#define MSG_ILLG_WHTSPC        (MSG_ERROR+ 47)  /* illegal whitespace */
#define MSG_MBI                (MSG_ERROR+ 60)  /* must be inside */
#define MSG_NAW                (MSG_ERROR+ 61)  /* not allowed with */
#define MSG_ILLG_CTAG          (MSG_ERROR+ 66)  /* illegal end-tag */
#define MSG_SUCC_WHTSPC        (MSG_PORT + 78)  /* succeeding white-space */
#define MSG_PREC_WHTSPC        (MSG_PORT + 79)  /* preceeding white-space */
#define MSG_XHTML_NOTCLOSED    (MSG_WARN + 88)  /* EMPTY tag not closed in
                                                   XHTML mode */

/*
 * messages within entities
 */
#define MSG_UNKN_ENTITY        (MSG_WARN + 18)  /* unknown entity */
#define MSG_EXPT_SEMIC         (MSG_WARN + 19)  /* ";" expected */
#define MSG_RPLC_ENT           (MSG_NOTE + 46)  /* replaced entity */
#define MSG_ILLG_DEFENT        (MSG_ERROR+ 69)  /* illegal entity definition */
#define MSG_ICON_ENTITY        (MSG_PORT + 58)  /* icon-entity found */
#define MSG_RPLC_ICON          (MSG_NOTE + 77)  /* icon-entity found */
#define MSG_DEFENT_WARN        (MSG_WARN + 92)  /* suspicious entity definition */

/* 
 * messages pertaining to Cascading Style Sheets
 */
#define MSG_INVALID_STYLE      (MSG_WARN + 90)  /* sytax error in CSS definition */
#define MSG_STYLE_REDEFINED    (MSG_WARN + 91)  /* CSS property redefined */

/*
 * messages within attributes
 */
#define MSG_UNKN_ATTR_REF      (MSG_ERROR+ 20)  /* unknown attribute reference */
#define MSG_UNKN_ATTR_TAG      (MSG_WARN + 84)  /* unknown tag attribute */
#define MSG_UNKN_ATTR_MACRO    (MSG_ERROR+ 85)  /* unknown macro attribute */
#define MSG_NO_URIPATH         (MSG_WARN + 21)  /* path to URI not found */
#define MSG_SERVER_URI         (MSG_WARN + 36)  /* suspicios enum value */
#define MSG_ARG_NO_QUOTE       (MSG_PORT + 22)  /* argument without quote */
#define MSG_EMPTY_SYMB_REF     (MSG_ERROR+ 23)  /* empty symbol reference */
#define MSG_ILLG_ATTR_FLAG     (MSG_WARN + 24)  /* attribute flag not allowed
                                                   here */
#define MSG_UNEX_ATTR_TYPE     (MSG_ERROR+ 25)  /* illegal symbol type */
#define MSG_UNEX_ATTR_VALUE    (MSG_WARN + 45)  /* unexpected attribute value */
#define MSG_SYMB_2ND_DEFAULT   (MSG_ERROR+ 26)  /* default value already set */
#define MSG_ENUM_UNKN          (MSG_WARN + 35)  /* unknown enum value */
#define MSG_NOARG_ATTR         (MSG_ERROR+ 42)  /* attr requires arg */
#define MSG_UNKN_ATTR_OPTION   (MSG_ERROR+ 43)  /* unknown attr option */
#define MSG_MISS_REQ_ATTR      (MSG_ERROR+ 44)  /* required attr missing */
#define MSG_MISS_RCMD_ATTR     (MSG_STYLE+ 90)  /* recommended attr missing */
#define MSG_ATTR_REDEFINED     (MSG_WARN + 63)  /* attr already defined */
#define MSG_ILLG_ATTRNAME      (MSG_ERROR+ 64)  /* illegal attribute
                                                   identifier */
#define MSG_UNKN_BINOP         (MSG_ERROR+ 65)  /* unknown binary operator */
#define MSG_ATTR_CONST         (MSG_ERROR+ 27)  /* can't change constant attr */
#define MSG_IMG_CORRUPT        (MSG_ERROR+  8)  /* image corrupt */
#define MSG_ATTR_OBSOLETE      (MSG_WARN + 87)  /* attribute is obsolete */
#define MSG_ATTR_AFTER_SLASH   (MSG_ERROR+ 89)  /* attribute after closing slash
                                                   in XHTML mode */

/*
 * messages from tag handles
 */
#define MSG_ANCHOR_HEADING     (MSG_STYLE+ 28)  /* heading inside anchor */
#define MSG_ANCH_NO_NMHR       (MSG_ERROR+ 29)  /* A : no NAME or HREF */
#define MSG_CL_TAG_ARG         (MSG_ERROR+ 62)  /* args for end-tag */
#define MSG_LF_IN_COMMENT      (MSG_PORT + 48)  /* LF within comment */
#define MSG_GT_IN_COMMENT      (MSG_PORT + 49)  /* GT within comment */
#define MSG_ZERO_COMMENT       (MSG_PORT + 50)  /* SGML zero comment */
#define MSG_TEXT_IN_COMMENT    (MSG_WARN + 55)  /* text outside comment context */

/*
 * messages from hsc-tag handles
 */
#define MSG_SYSTEM_RETURN      (MSG_WARN + 54)  /* external process result <> 0 */
#define MSG_UNMA_ELSE          (MSG_ERROR+ 53)  /* unmatched $ELSE */
#define MSG_DEFTAG_NO_OPEN     (MSG_FATAL+ 57)  /* no opening tag for deftag cl. */
#define MSG_REDEFINE_ENDTAG    (MSG_ERROR+ 52)  /* redefined end tag */
#define MSG_REDEFINE_TAG       (MSG_WARN + 59)  /* redefined & replaced tag */
#define MSG_USER_MESSAGE       (           39)  /* user message */
#define MSG_BLINK_SUX          (MSG_STYLE+ 25)  /* blink sucks  */
#define MSG_FRAME_SUX          (MSG_STYLE+ 76)  /* frames are disgusting */
#define MSG_NO_CONTENT         (MSG_FATAL+ 28)  /* no content */
#define MSG_REDEFINE_LAZY      (MSG_ERROR+ 56)  /* redefined lazy */
#define MSG_UNKN_LAZY          (MSG_ERROR+ 32)  /* unknown lazy */

/*
 * messages within expressions
 */
#define MSG_UNKN_ENVVAR        (MSG_WARN + 41)  /* unknown environ. var */
#define MSG_ILLG_NUM           (MSG_WARN + 71)  /* illegal numeric value */
#define MSG_ILLG_COLOR         (MSG_WARN + 72)  /* illegal color value */
#define MSG_REQU_QUOTE         (MSG_WARN + 81)  /* value required quotes */
#define MSG_CHANGED_QUOTE      (MSG_NOTE + 82)  /* not assign quote requested */

/*
 * misc. messages
 */
#define MSG_UNMA_GT            (MSG_ERROR+ 30)  /* unmatched ">" */
#define MSG_UNEXPT_CH          (MSG_ERROR+ 31)  /* unexpected char */
#define MSG_STR_LF             (MSG_WARN + 33)  /* linefeed in string */
#define MSG_UNEX_EOL           (MSG_ERROR+ 34)  /* unexpected eol */
#define MSG_NO_CONFIG          (MSG_FATAL+ 40)  /* can't open config */
#define MSG_NO_DOCENTRY        (MSG_WARN + 51)  /* no document-entry for id */
#define MSG_UNKN_FILETYPE      (MSG_WARN + 68)  /* unknown file type (for size) */
#define MSG_UNKN_LOCAL_ID      (MSG_WARN + 73)  /* unknown local ID */
#define MSG_UNKN_ID            (MSG_WARN + 74)  /* unknown ID */
#define MSG_REDEFINE_ID        (MSG_WARN + 75)  /* duplicate ID */
#define MSG_NOEXEC_OUTPUT      (MSG_WARN + 80)  /* no output-file for <$exec>*/
#define MSG_IOERROR            (MSG_ERROR+ 83)  /* i/o-error */
#define MSG_REMOVE_FAILED      (MSG_WARN + 86)  /* remove file failed */


#endif

