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
 * hsclib/tag.h
 *
 * tag structure and functions
 *
 */

#ifndef HSCLIB_TAG_H
#define HSCLIB_TAG_H
#include "ugly/dllist.h"
#include "hsclib/attrib.h"

/*
 * defines
 */
#define HSC_TAGID        "$"
#define HSC_COMMENT_STR  "*"
#define HSC_VERBATIM_STR "|"
#define HSC_INSEXPR_STR  "("
#define HSC_CONTENT_STR  HSC_TAGID "content"
#define HSC_DEFENT_STR   HSC_TAGID "defent"
#define HSC_DEFSTYLE_STR HSC_TAGID "defstyle"
#define HSC_DEFICON_STR  HSC_TAGID "deficon"
#define HSC_DEFINE_STR   HSC_TAGID "define"
#define HSC_DEFTAG_STR   HSC_TAGID "deftag"
#define HSC_DEPEND_STR   HSC_TAGID "depend"
#define HSC_ELSE_STR     HSC_TAGID "else"
#define HSC_ELSEIF_STR   HSC_TAGID "elseif"
#define HSC_EXEC_STR     HSC_TAGID "exec"
#define HSC_EXPORT_STR   HSC_TAGID "export"
#define HSC_IF_STR       HSC_TAGID "if"
#define HSC_INCLUDE_STR  HSC_TAGID "include"
#define HSC_INSERT_STR   HSC_TAGID "insert"
#define HSC_LET_STR      HSC_TAGID "let"
#define HSC_LAZY_STR     HSC_TAGID "varlist"
#define HSC_MACRO_STR    HSC_TAGID "macro"
#define HSC_MESSAGE_STR  HSC_TAGID "message"
#define HSC_SOURCE_STR   HSC_TAGID "source"
#define HSC_STRIPWS_STR  HSC_TAGID "StripWS"

#define HSC_TEXT_STR     "TEXT"
#define HSC_TIME_STR     "TIME"

#define STRIPWS_BOTH     "both"
#define STRIPWS_PREV     "prev"
#define STRIPWS_SUCC     "succ"
#define STRIPWS_NONE     "none"

#define STRIPWS_ENUM     \
        STRIPWS_BOTH "|" STRIPWS_PREV "|" STRIPWS_SUCC "|" STRIPWS_NONE

struct hsc_process;             /* forward reference */

/*
 * structure & typdef for tag
 */
typedef struct hsctag
{
    STRPTR name;                /* tag name, eg "TITLE" */
    ULONG option;               /* tag options, eg HT_CLOSE|HT_REQUIRED */
      BOOL(*o_handle) (struct hsc_process * hp, struct hsctag * tag);
    /* callback for start-tag */
      BOOL(*c_handle) (struct hsc_process * hp, struct hsctag * tag);
    /* callback for end-tag */
    DLLIST *attr;               /* list of attributes */
    EXPSTR *op_text;            /* macro text (open/close) */
    EXPSTR *cl_text;
    STRPTR mbi;                 /* string that tells inside which
                                 * tag this tag has to be
                                 * e.g. for <LI>: "ul|ol|dir|menu" */
    STRPTR naw;                 /* "not allowed within */
    HSCVAR *uri_stripext;       /* if this uri attribute's value is
                                 * an external uri, tag is stripped */
    HSCVAR *uri_size;           /* with this uri, values for WIDTH and
                                 * HEIGHT can be evaluated */
    INFILEPOS *start_fpos;      /* for macros: location of def. */
    INFILEPOS *end_fpos;        /* for endtag: location of start tag */
    /* NOTE: end_fpos is also used to store the start-position
     *   of the content text for container macros */
    BOOL occured;               /* TRUE, if tag already occured */
    /* NOTE: the occured-flag is also set by def_tagname(),
     *   if a new macro already exists. the warning message
     *   is displayed later within def_tag_args(), where
     *   also the occured-flag is reset to FALSE. see "deftag.c"
     */
}
HSCTAG;

/*
 * defines for tag options
 */
#define HT_NOCOPY       (1<<0)  /* avoid copying of tag text */
#define HT_CLOSE        (1<<1)  /* closing tag required */
#define HT_REQUIRED     (1<<2)  /* tag required at least once in document */
#define HT_ONLYONCE     (1<<3)  /* tag required at most once in document */
#define HT_SPECIAL      (1<<4)  /* do not evaluate attributes, call handler */
#define HT_OBSOLETE     (1<<5)  /* tag is already obsolete */
#define HT_JERK         (1<<6)  /* netscape extension & co. */
#define HT_AUTOCLOSE    (1<<7)  /* ignore closing tags (e.g. <p>, <li>) */
#define HT_NOBP         (1<<8)  /* TODO: warning if <P> before tag */
#define HT_NOAP         (1<<9)  /* TODO: -"- after tag */
#define HT_MACRO        (1<<10) /* macro tag */
#define HT_NOHANDLE     (1<<11) /* don't call tag handles */
#define HT_WHTSPC       (1<<12) /* warn about pre/succ-ceeding white-spaces */
#define HT_SKIPLF       (1<<13) /* skip possible LF after tag */
#define HT_UNKNOWN      (1<<14) /* unknown tag (temporary created) */
#define HT_RECOMMENDED  (1<<15) /* tag recommended to appear in document */
#define HT_CONTENT      (1<<16) /* auto-enabled for content macros */
#define HT_EMPTY        (1<<17) /* EMPTY SGML content model (must be <foo/> or
                                   <foo /> in XHTML) e.g. <br>, <hr> */

#define HT_KEEP_QUOTES (1<<30)  /* keep quotes for all attributes;
                                 * * auto-enable for all macro tags  */

/* tag options that can be set via DEFTAG */
#define TO_CLOSE_STR       "CLOSE"
#define TO_CLOSE_SHT       "C"
#define TO_SPECIAL_STR     "SPECIAL"
#define TO_SPECIAL_SHT     "SPC"
#define TO_JERK_STR        "JERK"
#define TO_JERK_SHT        "J"
#define TO_LAZY_STR        "LAZY"
#define TO_LAZY_SHT        "L"
#define TO_MBI_STR         "MUST_BE_INSIDE"
#define TO_MBI_SHT         "MBI"
#define TO_NAW_STR         "NOT_ALLOWED_WITHIN"
#define TO_NAW_SHT         "NAW"
#define TO_EMPTY_STR       "EMPTY"
#define TO_EMPTY_SHT       "E"
#define TO_AUTOCLOSE_STR   "AUTOCLOSE"
#define TO_AUTOCLOSE_SHT   "AC"
#define TO_ONLYONCE_STR    "ONLYONCE"
#define TO_ONLYONCE_SHT    "1"
#define TO_OBSOLETE_STR    "OBSOLETE"
#define TO_OBSOLETE_SHT    "O"
#define TO_REQUIRED_STR    "REQUIRED"
#define TO_REQUIRED_SHT    "R"
#define TO_RECOMMENDED_STR "RECOMMENDED"
#define TO_RECOMMENDED_SHT "RCMD"
#define TO_SKIPLF_STR      "SKIPLF"
#define TO_SKIPLF_SHT      "S"
#define TO_WHTSPC_STR      "WHTSPC"
#define TO_WHTSPC_SHT      "W"

/* TODO: think about this tag-option */
#define TO_VERS_STR        "VERS"
#define TO_VERS_SHT        "V"

/*
 *
 * extern references
 *
 */
#ifndef NOEXTERN_HSCLIB_TAG_H

extern HSCTAG *new_hsctag(STRPTR newid);
extern VOID del_hsctag(APTR data);
extern HSCTAG *cpy_hsctag(HSCTAG * oldtag);

extern int cmp_strtag(APTR cmpstr, APTR tagdata);
extern HSCTAG *find_strtag(DLLIST * taglist, STRPTR name);
extern int cmp_strctg(APTR cmpstr, APTR tagstr);

extern HSCTAG *app_tag(DLLIST * taglist, STRPTR tagid);

extern BOOL is_hsc_tag(HSCTAG * tag);
extern BOOL is_macro_tag(HSCTAG * tag);

#endif /* NOEXTERN_HSCLIB_TAG_H */
#endif /* HSCLIB_TAG_H */

/* $Id$ */
/* vi: set ts=4: */
