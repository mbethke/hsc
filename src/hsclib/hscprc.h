/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2004  Matthias Bethke
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
 * hsclib/hscprc.h
 *
 * hsc process structure
 *
 */

#ifndef HSC_HSCPRC_H
#define HSC_HSCPRC_H

#include <time.h>
#include "ugly/hsctrees.h"
#include "hsclib/ldebug.h"
#include "hsclib/tag.h"
#include "regex/regex.h"

#include "hscprj/project.h"

/*
 * system-dependant defines
 */
#if defined(AMIGA) || defined(AROS)
#define CONFIG_FILE "hsc.prefs"
#define OPTION_FILE "hsc.options", "env:hsc.options"
#define CONFIG_PATH "PROGDIR:"

/* TODO: find out what the hell this did here in the first place
#define UNIX 1       */           /* utilize POSIX-layer of BeOS */

#elif defined RISCOS
#define CONFIG_FILE "hsc:hsc.prefs"
#define CONFIG_PATH "hsc:"
#define OPTION_FILE "hsc.options"

#elif (defined NEXTSTEP) || (defined UNIX) || (defined BEOS) || (defined WINNT)
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "/usr/local/lib/hsc/", "/usr/lib/hsc/"
#define OPTION_FILE "hsc.options"

/* [3] */
#else
#error "Operating system not supported: config-file/path"
#endif

/* utilize POSIX-layer of BEOS */
#if defined BeOS
#define UNIX 1
#endif

/* step sizes for expstr's */
#define ES_STEP_MACRO   1024    /* tag_macro.c */
#define ES_STEP_INFILE  4096    /* input file buffer */

/*
 * modes for syntax check
 */
#define MODE_PEDANTIC 1
#define MODE_NORMAL   2
#define MODE_RELAXED  3

/*
 * modes for attribute quotes
 */
#define QMODE_KEEP   1          /* keep quotes from input */
#define QMODE_DOUBLE 2          /* always use double quotes (compatible) */
#define QMODE_SINGLE 3          /* always use single quotes */
#define QMODE_NONE   4          /* never use any quotes (compact) */

/*
 * modes for special characters/entity extraction
 */
#define EMODE_KEEP     (1)        /* do not replace */
#define EMODE_REPLACE  (2)        /* replace by preferred value (numeric/symbolic) */
#define EMODE_NUMERIC  (3)        /* always replace with numeric entity (&#123;) */
#define EMODE_SYMBOLIC (4)        /* always replace with symbolic entity (&uuml;) */
#define EMODE_UTF8     (5)        /* always replace with UTF-8 representation */
#define EMODE_INVALID  (-1)       /* option has not been set */

/*
 * misc. defines for files & envvars
 */
#define ENV_HSCPATH    "HSCPATH"        /* envvar that contains path for prefs */
#define ENV_HSCSALARY  "HSCSALARY"      /* contains salary of user */
#define ENV_HSCUSER    "HSCUSER_PATH"   /* to substitute "~/" */
#define ENV_HSCUSERS   "HSCUSERS_PATH"  /* to substitute "~" */
#define ENV_HSCROOT    "HSCROOT_PATH"   /* to substitute "/" */
#define ENV_HSCINCLUDE "HSCINCLUDE"     /* path to search for include files */

/* used as prefix in filename for
 * internal (pseudo-)files (macros,init) */
#define SPECIAL_FILE_ID "::s::"

/* prefix to be used by filenames if parent file on input-stack
 * should be used for message-pos.
 *
 * NOTE: this is set if IH_POS_PARENT is passed on hsc_include() */
#define PARENT_FILE_ID  "::p::"

/* pseudo-filenames for stdin */
#define FILENAME_STDIN1 "STDIN"
#define FILENAME_STDIN2 "-"

/* size of input buffer for getting dimension of image files */
#define IMAGE_BUFFER_SIZE  2048

#define MAXIMUM_MESSAGE_INFINITE ((ULONG) -1)

/* names for special attributes */
#define CLICK_HERE_ATTR     "HSC.CLICK-HERE"    /* attribute that holds "click here" words */
#define COLOR_NAMES_ATTR    "HSC.COLOR-NAMES"
#define CONTENT_ATTR        "HSC.CONTENT"       /* keep macro content for content macros */
#define ANCHOR_ATTR         "HSC.ANCHOR"
#define RESULT_ATTR         "HSC.EXEC.RESULT"
#define FILESIZEFORMAT_ATTR "HSC.FORMAT.FILESIZE"
#define TIMEFORMAT_ATTR     "HSC.FORMAT.TIME"
#define LINEFEED_ATTR       "HSC.LF"
#define HSCVERSION_ATTR     "HSC.VERSION"
#define HSCREVISION_ATTR    "HSC.REVISION"


/* attribute that holds condition on <$if/$elseif> */
#define CONDITION_ATTR "COND"

/* attribute that tells operating system */
#define SYSTEM_ATTR     "hsc.System"
#ifdef AMIGA
#define SYSTEM_ATTR_ID "Amiga"

#elif defined AROS
#define SYSTEM_ATTR_ID "AROS"

#elif defined BEOS
#define SYSTEM_ATTR_ID "BeOS"

#elif defined RISCOS
#define SYSTEM_ATTR_ID "RiscOS"

#elif defined NEXTSTEP
#define SYSTEM_ATTR_ID "NeXTStep"

#elif defined UNIX
#define SYSTEM_ATTR_ID "Unix"

#elif defined WINNT
#define SYSTEM_ATTR_ID "Win32"

/* dos4 */
#else
#error "system not supported: SYSTEM_ATTR_ID"
#endif

/*
 * some typedefs used inside the hsc_process
 */
typedef LONG HSCMSG_ID;         /* hsc message id */
typedef LONG HSCMSG_CLASS;      /* hsc message class */

/* enumerator values for hsc_process->msg_ignore */
enum hsc_ignore_status
{
    make_my_day,                /* ignore only, if whole class is ignored */
    ignore,                     /* always ignore */
    enable                      /* always show up (even if class ignored) */
};

typedef enum hsc_ignore_status HSCIGN;

/*
 * hsc process structure
 */
struct hsc_process
{
    INFILE *inpf;               /* current input file */
    DLLIST *inpf_stack;         /* stack of nested input files */
    hsctree *deftag;            /* defined tags and macros */
    DLLIST *defattr;            /* defined attributes */
    DLLIST *defent;             /* defined special charcters & entities */
    hsctree *deflazy;           /* defined lazy attribute lists */
    hsctree *defstyle;          /* defined CSS styles */
    DLLIST *container_stack;    /* stack of container-tags currently open */
    DLLIST *content_stack;      /* stack of contents of container macros */
    DLLIST *include_dirs;       /* include directories */
    DLLIST *tag_styles;         /* CSS styles used by current tag */
    HSCPRJ *project;            /* project data */
    DLLIST *idrefs;             /* list of references to local IDs */
    EXPSTR *destdir;            /* destination root directory */
    EXPSTR *reldir;             /* relative destination directory */
    EXPSTR *tmpstr;             /* temp. string used by several functions */
    EXPSTR *curr_msg;           /* current message */
    EXPSTR *curr_ref;           /* current reference message */
    EXPSTR *iconbase;           /* base URI for icons */
    EXPSTR *server_dir;         /* root dir for server-relative URIs */
    time_t start_time;          /* time process has been started */

    DLLIST *select_stack;       /* stack for results of <$select> */
    EXPSTR *if_stack;           /* stack for results of <$if> */

    EXPSTR *tag_name_str;       /* strings for communication with tag-handlers */
    EXPSTR *tag_attr_str;
    EXPSTR *tag_close_str;

    STRPTR filename_document;   /* document-name to be stored in project */

    HSCIGN *msg_ignore;         /* messages to be ignored */
    BOOL msg_ignore_notes;      /* message-classes to be ignored */
    BOOL msg_ignore_style;
    BOOL msg_ignore_port;
    HSCMSG_CLASS *msg_class;    /* messages with remaped classes */
    ULONG msg_count;            /* number of messages/errors occured until now */
    ULONG max_messages;          /* maximum number of messages/errors allowed */
    ULONG max_errors;
    unsigned char *image_buffer;  /* buffer to get image dimension from file */

    regex_t *re_uri;            /* precompiled RE to match valid URIs */

    BOOL chkid;                 /* flag: check existence of URIs/IDs */
    BOOL chkuri;
    BOOL compact;               /* flag: create compact output */
    BOOL debug;                 /* flag: display debuging info */
    BOOL getsize;               /* flag: get size of images/embedded docs */
    BOOL htmlonly;              /* flag: disable hsc-extensions */
    BOOL jerkvalues;            /* flag: interpret jerk values */
    BOOL rplc_ent;              /* flag: replace special chars */
    BOOL rplc_quote;            /* flag: replace quotes in text by "&quot;" */
    BOOL smart_ent;             /* flag: replace special entities "<>&" */
    BOOL strip_cmt;             /* flag: strip SGML-comments */
    BOOL strip_ext;             /* flag: strip external references */
    BOOL strip_badws;           /* flag: strip bad white spaces */
    BOOL weenix;                /* flag: unix compatibilty mode */
    BOOL xhtml_emptytag;        /* flag: processing empty XHTML tag */
    BOOL suppress_output;       /* flag: TRUE, until outputable data occure */
    BOOL docbase_set;           /* <BASE HREF=".."> occured */
    BOOL inside_pre;            /* inside preformatted tag <PRE> & Co. */
    BOOL inside_anchor;         /* inside anchor-tag <A> */
    BOOL inside_title;          /* inside title-tag <TITLE> */
    BOOL prostitute;            /* use "prostitute" or "jerk"? */
    BOOL nested_errors;         /* show "location of previous call" msgs */
    BOOL lctags;                /* force all tags to lowercase */
    BOOL checkext;              /* check external links */
    BOOL xhtml;                 /* try to be XHTML compatible */
    BOOL validate_css;          /* validate contents of STYLE attributes */

    BOOL fatal;                 /* fatal error occured; abort process */

    LONG tag_call_id;
    ULONG prev_status_line;
    LONG prev_heading_num;      /* number of previous heading */
    LONG entitymode;            /* entity replace mode */
    LONG quotemode;             /* quotes to use as output quotes */

    STRPTR click_here_str;      /* keywords for click-here syndrome */
    STRPTR color_names;         /* predefined names for colors */
    STRPTR strip_tags;          /* tags that should be stripped */
    EXPSTR *whtspc;             /* white spaces buffered */
    HSCTAG *tag_next_whtspc;    /* set, if a tag did not allow succ.whtspc */
    BOOL strip_next_whtspc;     /* flag: strip next whtite space
                                 * set by parse_tag(), if strip_badws = TRUE */
    BOOL strip_next2_whtspc;    /* flag: strip next but one white space */
    /* status callbacks */
    void(*CB_status_misc) (struct hsc_process * hp, STRPTR s);
    /* called for verbose messages */
    void(*CB_status_line) (struct hsc_process * hp);
    /* called after new line */
    void(*CB_status_file_begin) (struct hsc_process * hp, STRPTR filename);
    /* called when new file is going to be loaded */
    void(*CB_status_file_end) (struct hsc_process * hp);
    /* called after file has fully been processed */

    /* message callbacks */
    void(*CB_message) (struct hsc_process * hp,
                       HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                       STRPTR fname, ULONG x, ULONG y,
                       STRPTR msg_text);
    void(*CB_message_ref) (struct hsc_process * hp,
                           HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                           STRPTR fname, ULONG x, ULONG y,
                           STRPTR msg_text);

    /* syntax elements callbacks */
    void(*CB_start_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close);
    void(*CB_end_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close);
    void(*CB_text) (struct hsc_process * hp,
                    STRPTR white_spaces, STRPTR text);
    void(*CB_id) (struct hsc_process * hp,
                  HSCATTR * attr, STRPTR id);

};

typedef struct hsc_process HSCPRC;

/*
 * global funcs
 */
#ifndef NOEXTERN_HSC_HSCPRC

#define anyWhtspc(hp) (estrlen(hp->whtspc))

extern void del_hscprc(HSCPRC * hp);
extern HSCPRC *new_hscprc(void);
extern void reset_hscprc(HSCPRC * hp);

/* set-methodes for callbacks */
extern void hsc_set_status_file_begin(HSCPRC * hp, void(*status_file) (HSCPRC * hp, STRPTR filename));
extern void hsc_set_status_file_end(HSCPRC * hp, void(*status_file) (HSCPRC * hp));
extern void hsc_set_status_line(HSCPRC * hp, void(*status_line) (HSCPRC * hp));
extern void hsc_set_status_misc(HSCPRC * hp, void(*status_misc) (HSCPRC * hp, STRPTR s));
extern void hsc_set_message(HSCPRC * hp,
                            void(*message) (struct hsc_process * hp,
                                            HSCMSG_CLASS msg_class,
                                            HSCMSG_ID msg_id,
                                            STRPTR fname, ULONG x, ULONG y,
                                            STRPTR msg_text));
extern void hsc_set_message_ref(HSCPRC * hp,
                                void(*message_ref) (struct hsc_process * hp,
                                                    HSCMSG_CLASS msg_class,
                                                    HSCMSG_ID msg_id,
                                                    STRPTR fname,
                                                    ULONG x, ULONG y,
                                                    STRPTR msg_text));
extern void hsc_set_start_tag(HSCPRC * hp,
                              void(*CB_start_tag) (struct hsc_process * hp,
                                                   HSCTAG * tag,
                                                   STRPTR tag_name,
                                                   STRPTR tag_attr,
                                                   STRPTR tag_close));
extern void hsc_set_end_tag(HSCPRC * hp,
                            void(*CB_end_tag) (struct hsc_process * hp,
                                               HSCTAG * tag,
                                               STRPTR tag_name,
                                               STRPTR tag_attr,
                                               STRPTR tag_close));
extern void hsc_set_text(HSCPRC * hp,
                         void(*CB_text) (struct hsc_process * hp,
                                         STRPTR white_spaces, STRPTR text));
extern void hsc_set_id(HSCPRC * hp,
                       void(*id) (struct hsc_process * hp,
                                  HSCATTR * attr, STRPTR id));

/* set-methodes for flags */
extern void hsc_set_chkid(HSCPRC * hp, BOOL new_chkid);
extern void hsc_set_chkuri(HSCPRC * hp, BOOL new_chkuri);
extern void hsc_set_compact(HSCPRC * hp, BOOL new_compact);
extern void hsc_set_debug(HSCPRC * hp, BOOL new_debug);
extern void hsc_set_getsize(HSCPRC * hp, BOOL new_getsize);
extern void hsc_set_jerkvalues(HSCPRC * hp, BOOL new_jerkvalues);
extern void hsc_set_rplc_ent(HSCPRC * hp, BOOL new_rplc_ent);
extern void hsc_set_rplc_quote(HSCPRC * hp, BOOL new_rplc_quote);
extern BOOL hsc_set_server_dir(HSCPRC * hp, STRPTR dir);
extern void hsc_set_smart_ent(HSCPRC * hp, BOOL new_smart_ent);
extern void hsc_set_strip_badws(HSCPRC * hp, BOOL new_strip_badws);
extern void hsc_set_strip_cmt(HSCPRC * hp, BOOL new_strip_cmt);
extern void hsc_set_strip_ext(HSCPRC * hp, BOOL new_strip_ext);
extern void hsc_set_no_nested_errors(HSCPRC * hp, BOOL new_nested_errors);
extern void hsc_set_lctags(HSCPRC * hp, BOOL new_lctags);
extern void hsc_set_checkext(HSCPRC * hp, BOOL new_checkext);
extern void hsc_set_xhtml(HSCPRC * hp, BOOL new_xhtml);
extern void hsc_set_novcss(HSCPRC * hp, BOOL new_vcss);

/* set-methodes for values */
extern BOOL hsc_set_destdir(HSCPRC * hp, STRPTR dir);
extern BOOL hsc_set_reldir(HSCPRC * hp, STRPTR fname);
extern BOOL hsc_set_iconbase(HSCPRC * hp, STRPTR uri);
extern BOOL hsc_set_strip_tags(HSCPRC * hp, STRPTR taglist);
extern BOOL hsc_set_filename_document(HSCPRC * hp, STRPTR filename);
extern void hsc_set_quote_mode(HSCPRC * hp, LONG new_mode);
extern void hsc_set_entity_mode(HSCPRC * hp, LONG new_mode);
extern void hsc_set_maximum_messages(HSCPRC * hp, LONG messages);
extern void hsc_set_maximum_errors(HSCPRC * hp, LONG errors);

/* methodes for include-directories */
extern BOOL hsc_add_include_directory(HSCPRC * hp, STRPTR dir);
extern void hsc_clr_include_directory(HSCPRC * hp);

/* get-methodes for flags */
extern BOOL hsc_get_chkid(HSCPRC * hp);
extern BOOL hsc_get_chkuri(HSCPRC * hp);
extern BOOL hsc_get_compact(HSCPRC * hp);
extern BOOL hsc_get_debug(HSCPRC * hp);
extern BOOL hsc_get_getsize(HSCPRC * hp);
extern BOOL hsc_get_htmlonly(HSCPRC * hp);
extern BOOL hsc_get_jerkvalues(HSCPRC * hp);
extern BOOL hsc_get_rplc_ent(HSCPRC * hp);
extern BOOL hsc_get_rplc_quote(HSCPRC * hp);
extern BOOL hsc_get_smart_ent(HSCPRC * hp);
extern BOOL hsc_get_strip_badws(HSCPRC * hp);
extern BOOL hsc_get_strip_cmt(HSCPRC * hp);
extern BOOL hsc_get_strip_ext(HSCPRC * hp);

/* get-methodes for internal flags */
extern BOOL hsc_get_fatal(HSCPRC * hp);
extern BOOL hsc_get_inside_anchor(HSCPRC * hp);
extern BOOL hsc_get_inside_pre(HSCPRC * hp);
extern BOOL hsc_get_suppress_output(HSCPRC * hp);

/* get-methodes for values */
extern STRPTR hsc_get_destdir(HSCPRC * hp);
extern STRPTR hsc_get_reldir(HSCPRC * hp);
extern STRPTR hsc_get_iconbase(HSCPRC * hp);
extern STRPTR hsc_get_server_dir(HSCPRC * hp);

/* get-methodes for internal values */
extern STRPTR hsc_get_click_here_str(HSCPRC * hp);
extern STRPTR hsc_get_file_name(HSCPRC * hp);
extern ULONG hsc_get_file_line(HSCPRC * hp);
extern ULONG hsc_get_file_column(HSCPRC * hp);
extern ULONG hsc_get_msg_count(HSCPRC * hp);

/* methodes for messages */
extern BOOL hsc_get_msg_ignore_notes(HSCPRC * hp);
extern BOOL hsc_get_msg_ignore_style(HSCPRC * hp);
extern BOOL hsc_get_msg_ignore_port(HSCPRC * hp);
extern BOOL hsc_set_msg_ignore_notes(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore_style(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore_port(HSCPRC * hp, BOOL value);
extern BOOL hsc_set_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id, HSCIGN value);
extern HSCIGN hsc_get_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id);
extern BOOL hsc_set_msg_class(HSCPRC * hp, HSCMSG_ID msg_id, HSCMSG_CLASS msg_class);
extern HSCMSG_CLASS hsc_get_msg_class(HSCPRC * hp, HSCMSG_ID msg_id);
extern void hsc_clear_msg_ignore(HSCPRC * hp);
extern void hsc_reset_msg_class(HSCPRC * hp);

/* output function */
extern STRPTR compactWs(HSCPRC * hp, STRPTR ws);
extern BOOL hsc_output_text(HSCPRC * hp, STRPTR wspc, STRPTR text);

/* misc. functions */
extern BOOL hsc_standard_nomem_handler(size_t size);

#endif /* NOEXTERN_HSC_HSCPRC */
#endif /* HSC_HSCPRC_H */

/* $Id$ */
/* vi: set ts=4: */

