/*
** hsclib/hscprc.h
**
** hsc process structure
**
*/

#ifndef HSC_HSCPRC_H
#define HSC_HSCPRC_H

#include <time.h>

#include "hsclib/tag.h"

typedef LONG HSCMSG_ID;    /* hsc message id */
typedef LONG HSCMSG_CLASS; /* hsc message class */

/*
** hsc process structure
*/
typedef struct hscprocess {
    INFILE *inpf;            /* current input file */
    DLLIST *inpf_stack;      /* stack of nested input files */
    DLLIST *deftag;          /* defined tags */
    DLLIST *defattr;         /* defined attributes */
    DLLIST *defent;          /* defined special charcters & entities */
    DLLIST *container_stack; /* stack of containers curently open */
    DLLIST *id_global;       /* list of global IDs (read from project) */
    DLLIST *id_local;        /* list of local IDs (in current file) */
    DLLIST *id_ref;          /* list of referencing local IDs */
    DLLIST *subdoc;          /* list of subdocuments refered to */
    EXPSTR *destdir;         /* destination root directory */
    EXPSTR *reldir;          /* relative destination directory */
    EXPSTR *tmpstr;          /* temp. string used by several functions */
    EXPSTR *rmt_str;         /* temp. string used for macro text */
    EXPSTR *curr_msg;        /* current message */
    EXPSTR *curr_ref;        /* current reference message */
    time_t start_time;       /* time process has been started */

    EXPSTR *IF_stack;        /* stack for results of nested <$if>s */

    EXPSTR *tag_name_str;    /* strings for communication with tag-handlers */
    EXPSTR *tag_attr_str;
    EXPSTR *tag_close_str;

    STRPTR filename_project; /* where to read/write project data */
    STRPTR filename_process; /* process-name to be stored in project */

    BOOL         *msg_ignore;/* messages to be ignored */
    HSCMSG_CLASS *msg_class; /* messages with remaped classes */

    BOOL chkid;              /* flag: check existence of URIs/IDs */
    BOOL chkuri;
    BOOL compact;            /* flag: create compact output */
    BOOL debug;              /* flag: display debuging info */
    BOOL getsize;            /* flag: get size of images/embedded docs */

    BOOL htmlonly;           /* flag: disable hsc-extensions */
    BOOL jens;               /* flag: enable some experimental features */
    BOOL jerkvalues;         /* flag: interpret jerk values */
    BOOL rplc_ent;           /* flag: replace special chars */
    BOOL smart_ent;          /* flag: replace special entities "<>&" */
    BOOL strip_cmt;          /* flag: strip SGML-comments */
    BOOL strip_ext;          /* flag: strip external references */

    BOOL suppress_output;
    BOOL docbase_set;        /* <BASE HREF=".."> occured */
    BOOL inside_pre;         /* inside preformatted tag <PRE> & Co. */
    BOOL inside_anchor;      /* inside anchor tag <A> */

    BOOL fatal;              /* fatal error occured; abort process */

    LONG  tag_call_id;
    ULONG prev_status_line;
    LONG  prev_heading_num;  /* number of previous heading */
    LONG  entmode;           /* entity replace mode */
    LONG  quotemode;         /* quotes to use as output quotes */

    STRPTR click_here_str;   /* keywords for click-here syndrome */
    STRPTR color_names;      /* predifined names for colors */
    STRPTR strip_tags;       /* tags that should be stripped */
    STRPTR last_anchor;      /* uri-text for last anchor defined */

    /* status callbacks */
    VOID (*CB_status_misc)(struct hscprocess *hp, STRPTR s);
                             /* called for verbose messages */
    VOID (*CB_status_line)(struct hscprocess *hp);
                             /* called after new line */
    VOID (*CB_status_file_begin)(struct hscprocess *hp); /* TODO: add filename */
                             /* called when new file is going to be loaded */
    VOID (*CB_status_file_end)(struct hscprocess *hp);
                             /* called after file has fully been processed */

    /* message callbacks */
    VOID (*CB_message)(struct hscprocess *hp,
         HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
         STRPTR fname, ULONG x, ULONG y,
         STRPTR msg_text);
    VOID (*CB_message_ref)(struct hscprocess *hp,
         STRPTR fname, ULONG x, ULONG y,
         STRPTR msg_text);

    /* syntax elements callbacks */
    VOID (*CB_start_tag)(struct hscprocess *hp,
         HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close );
    VOID (*CB_end_tag)(struct hscprocess *hp,
         HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close );
    VOID (*CB_text)(struct hscprocess *hp,
         STRPTR white_spaces, STRPTR text );
    VOID (*CB_id)(struct hscprocess *hp,
         HSCATTR *attr, STRPTR id );
} HSCPRC;

#define HSCPREFS_ENVVAR  "HSCPREFS" /* envvar that contains path for prefs */
#define SPECIAL_FILE_ID  "::"       /* used as prefix in filename for */
                                    /* internal (pseudo-)files (macros,init)*/
#define ID_FILE "hsc.id"            /* where to store/read from ids */

#ifdef AMIGA
/* file that contains base-data from `LoadHscPrefs' */
#define HSCBASE_FILE "env:hscBase"
#endif

#ifdef AMIGA
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "", ":", "env:", "s:"
#endif

#ifdef UNIX
#define CONFIG_FILE "hsc.prefs"
#define CONFIG_PATH "./", "~/", "/"
#endif

#ifdef MSDOS
#define CONFIG_FILE "HSC.PRE"
#define CONFIG_PATH ".\\", "\\"
#endif

/* step sizes for expstr's */
#define ES_STEP_RMTSTR    32 /* tag_macr.c */
#define ES_STEP_MACRO   1024 /* tag_macr.c */
#define ES_STEP_INFILE  4096 /* input file buffer */

/*
** modes for syntax check
*/
#define MODE_PEDANTIC 1
#define MODE_NORMAL   2
#define MODE_RELAXED  3

/*
** modes for attribute quotes
*/
#define QMODE_KEEP   1                 /* keep quotes from input */
#define QMODE_DOUBLE 2                 /* always use double quotes (compatible) */
#define QMODE_SINGLE 3                 /* always use single quotes */
#define QMODE_NONE   4                 /* never use any quotes (compact) */

/*
** modes for special characters/entity extraction
*/
#define EMODE_KEEP     1               /* do not replace */
#define EMODE_REPLACE  2               /* replace by prefered value */
                   /* (depends wheter if a PREFNUM was used within $DEFENT) */
#define EMODE_NUMERIC  3               /* always replace by numeric (&#123;)*/
#define EMODE_SYMBOLIC 4               /* always replace by symbolic (&uuml;)*/

/* prefix char used for special hsc-tags and -attributes */
#define HSC_SPECIAL_PREFIX "$"

/* attribute that holds "click here" words */
#define CLICK_HERE_ATTR  "__CLICK_HERE__"
#define COLOR_NAMES_ATTR "__COLOR_NAMES__"

/* attribute that tells operating system */
#define SYSTEM_ATTR     "__SYSTEM__"
#ifdef AMIGA
#define SYSTEM_ATTR_ID "AMIGA"
#elif defined UNIX
#define SYSTEM_ATTR_ID "UNIX"
#else
#error "system not supported: SYSTEM_ATTR_ID"
#endif

/*
** global funcs
*/
#ifndef NOEXTERN_HSC_HSCPRC

extern VOID    del_hscprc( HSCPRC *hp );
extern HSCPRC *new_hscprc( void );

/* set-methodes for callbacks */
extern VOID hsc_set_status_file_begin( HSCPRC *hp, VOID (*status_file)(HSCPRC *hp) );
extern VOID hsc_set_status_file_end( HSCPRC *hp, VOID (*status_file)(HSCPRC *hp) );
extern VOID hsc_set_status_line( HSCPRC *hp, VOID (*status_line)(HSCPRC *hp) );
extern VOID hsc_set_status_misc( HSCPRC *hp, VOID (*status_misc)(HSCPRC *hp, STRPTR s) );
extern VOID hsc_set_message(
    HSCPRC *hp,
    VOID (*message)(struct hscprocess *hp,
         HSCMSG_CLASS msg_class, HSCMSG_ID,
         STRPTR fname, ULONG x, ULONG y,
         STRPTR msg_text) );
extern VOID hsc_set_message_ref(
    HSCPRC *hp,
    VOID (*message_ref)(struct hscprocess *hp,
         STRPTR fname, ULONG x, ULONG y,
         STRPTR msg_text) );
extern VOID hsc_set_start_tag( HSCPRC *hp,
    VOID (*CB_start_tag)(struct hscprocess *hp,
         HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close ) );
extern VOID hsc_set_end_tag( HSCPRC *hp,
    VOID (*CB_end_tag)(struct hscprocess *hp,
         HSCTAG *tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close ) );
extern VOID hsc_set_text( HSCPRC *hp,
    VOID (*CB_text)(struct hscprocess *hp,
         STRPTR white_spaces, STRPTR text ) );
extern VOID hsc_set_id( HSCPRC *hp,
    VOID (*id)(struct hscprocess *hp,
         HSCATTR *attr, STRPTR id ) );

/* set-methodes for flags */
extern VOID hsc_set_chkid( HSCPRC *hp, BOOL new_chkid );
extern VOID hsc_set_chkuri( HSCPRC *hp, BOOL new_chkuri );
extern VOID hsc_set_compact( HSCPRC *hp, BOOL new_compact );
extern VOID hsc_set_debug( HSCPRC *hp, BOOL new_debug );
extern VOID hsc_set_getsize( HSCPRC *hp, BOOL new_getsize );
extern VOID hsc_set_htmlonly( HSCPRC *hp, BOOL new_jens );
extern VOID hsc_set_jens( HSCPRC *hp, BOOL new_jens );
extern VOID hsc_set_jerkvalues( HSCPRC *hp, BOOL new_jens );
extern VOID hsc_set_rplc_ent( HSCPRC *hp, BOOL new_rplc_ent );
extern VOID hsc_set_smart_ent( HSCPRC *hp, BOOL new_smart_ent );
extern VOID hsc_set_strip_cmt( HSCPRC *hp, BOOL new_strip_cmt );
extern VOID hsc_set_strip_ext( HSCPRC *hp, BOOL new_strip_ext );

/* set-methodes for values */
extern BOOL hsc_set_destdir( HSCPRC *hp, STRPTR dir );
extern BOOL hsc_set_reldir( HSCPRC *hp, STRPTR fname );
extern BOOL hsc_set_filename_project( HSCPRC *hp, STRPTR filename );
extern BOOL hsc_set_filename_process( HSCPRC *hp, STRPTR filename );
extern VOID hsc_set_quote_mode( HSCPRC *hp, LONG new_mode );
extern VOID hsc_set_entity_mode( HSCPRC *hp, LONG new_mode );

/* get-methodes for flags */
extern BOOL hsc_get_chkid( HSCPRC *hp );
extern BOOL hsc_get_chkuri( HSCPRC *hp );
extern BOOL hsc_get_compact( HSCPRC *hp );
extern BOOL hsc_get_debug( HSCPRC *hp );
extern BOOL hsc_get_getsize( HSCPRC *hp );
extern BOOL hsc_get_htmlonly( HSCPRC *hp );
extern BOOL hsc_get_jerkvalues( HSCPRC *hp );
extern BOOL hsc_get_jens( HSCPRC *hp );
extern BOOL hsc_get_rplc_ent( HSCPRC *hp );
extern BOOL hsc_get_smart_ent( HSCPRC *hp );
extern BOOL hsc_get_strip_cmt( HSCPRC *hp );
extern BOOL hsc_get_strip_ext( HSCPRC *hp );

/* get-methodes for internal flags */
extern BOOL hsc_get_fatal( HSCPRC *hp );
extern BOOL hsc_get_inside_anchor( HSCPRC *hp );
extern BOOL hsc_get_inside_pre( HSCPRC *hp );
extern BOOL hsc_get_suppress_output( HSCPRC *hp );

/* get-methodes for values */
extern STRPTR hsc_get_destdir( HSCPRC *hp );
extern STRPTR hsc_get_reldir( HSCPRC *hp );

/* get-methodes for internal values */
extern STRPTR hsc_get_click_here_str( HSCPRC *hp );
extern STRPTR hsc_get_last_anchor( HSCPRC *hp );
extern STRPTR hsc_get_file_name( HSCPRC *hp );
extern ULONG  hsc_get_file_line( HSCPRC *hp );
extern ULONG  hsc_get_file_column( HSCPRC *hp );

/* methodes for messages */
extern BOOL hsc_set_msg_ignore( HSCPRC *hp, HSCMSG_ID msg_id, BOOL value );
extern BOOL hsc_get_msg_ignore( HSCPRC *hp, HSCMSG_ID msg_id );
extern BOOL hsc_set_msg_class( HSCPRC *hp, HSCMSG_ID msg_id, HSCMSG_CLASS msg_class );
extern HSCMSG_CLASS hsc_get_msg_class( HSCPRC *hp, HSCMSG_ID msg_id );
extern VOID hsc_clear_msg_ignore( HSCPRC *hp );
extern VOID hsc_reset_msg_class( HSCPRC *hp );

/* output function */
extern BOOL hsc_output_text( HSCPRC *hp, STRPTR wspc, STRPTR text );

/* misc. functions */
extern VOID call_panic( STRPTR text, STRPTR file, ULONG line );
extern BOOL hsc_standard_nomem_handler( size_t size );

/* function called if invalid state dedected */
#define panic(text) call_panic(text,__FILE__,__LINE__)

#endif /* NOEXTERN_HSC_HSCPRC */
#endif /* HSC_HSCPRC_H */
