/*
** hsclib/tag.h
**
** tag structure and functions
**
*/

#ifndef HSCLIB_TAG_H
#define HSCLIB_TAG_H

/*
** defines
*/
#define HSC_TAGID        "$"
#define HSC_COMMENT_STR  "*"
#define HSC_ONLYCOPY_STR "|"
#define HSC_INSEXPR_STR  "("
#define HSC_DEFENT_STR   HSC_TAGID "DEFENT"
#define HSC_DEFINE_STR   HSC_TAGID "DEFINE"
#define HSC_DEFTAG_STR   HSC_TAGID "DEFTAG"
#define HSC_ELSE_STR     HSC_TAGID "ELSE"
#define HSC_EXEC_STR     HSC_TAGID "EXEC"
#define HSC_IF_STR       HSC_TAGID "IF"
#define HSC_INCLUDE_STR  HSC_TAGID "INCLUDE"
#define HSC_INSERT_STR   HSC_TAGID "INSERT"
#define HSC_LET_STR      HSC_TAGID "LET"
#define HSC_MACRO_STR    HSC_TAGID "MACRO"
#define HSC_MESSAGE_STR  HSC_TAGID "MESSAGE"
#define HSC_SOURCE_STR   HSC_TAGID "SOURCE"

#define HSC_TEXT_STR     "TEXT"
#define HSC_TIME_STR     "TIME"

struct hscprocess; /* forward reference */

/*
** structure & typdef for tag
*/
typedef struct hsctag {
    STRPTR name;                       /* tag name, eg "TITLE" */
    ULONG  option;                     /* tag options, eg HT_CLOSE|HT_REQUIRED */
    LONG   vers;                       /* tag version, eg 10 for 1.0 */
    BOOL   (*o_handle)(struct hscprocess *hp, struct hsctag *tag );
                                       /* callback for start-tag */
    BOOL   (*c_handle)(struct hscprocess *hp, struct hsctag *tag );
                                       /* callback for end-tag */
    DLLIST *attr;                      /* list of attributes */
    EXPSTR *op_text;                   /* macro text (open/close) */
    EXPSTR *cl_text;
    STRPTR mbi;                        /* string that tells inside which */
                                       /*   tag this tag has to be */
                                       /*   e.g. for <LI>: "ul|ol|dir|menu" */
    STRPTR naw;                        /* "not allowed within */
    HSCVAR *uri_stripext;              /* if this uri attribute's value is */
                                       /*   an external uri, tag is stripped */
    HSCVAR *uri_size;                  /* with this uri, values for WIDTH and */
                                       /*   HEIGHT and can be evaluated */
    INFILEPOS *start_fpos;             /* for macros: location of def. */
    INFILEPOS *end_fpos;               /* for endtag: location of start tag */
    BOOL   occured;                    /* TRUE, if tag already occured */
    /* NOTE: the occured-flag is also set by def_tagname(),
    **   if a new macro already exists. the warning message
    **   is displayed later within def_tag_args(), where
    **   also the occured-flag is reset to FALSE. see "deftag.c"
    */
} HSCTAG;

/*
** defines
*/
#define HT_NOCOPY      (1<<0)  /* avoid copying of tag text */
#define HT_CLOSE       (1<<1)  /* closing tag required */
#define HT_REQUIRED    (1<<2)  /* tag required at least once in file */
#define HT_ONLYONCE    (1<<3)  /* tag required at most once in file */
#define HT_SPECIAL     (1<<4)  /* do not evaluate attributes, call handler */
#define HT_OBSOLETE    (1<<5)  /* tag is already obsolete */
#define HT_JERK        (1<<6)  /* netscape externsion & co. */
#define HT_AUTOCLOSE   (1<<7)  /* ignore closing tags (<P> and <LI>) */
#define HT_NOBP        (1<<8)  /* TODO: warning if <P> before tag */
#define HT_NOAP        (1<<9)  /* TODO: -"- after tag */
#define HT_MACRO       (1<<10) /* macro tag */
#define HT_NOHANDLE    (1<<11) /* don't call tag handles */
#define HT_NONESTING   (1<<12) /* TODO: remove this */
#define HT_SKIPLF      (1<<13) /* skip possible LF after tag */

/* tag options that can be set via DEFTAG */
#define TO_CLOSE_STR       "CLOSE"
#define TO_CLOSE_SHT       "C"
#define TO_SPECIAL_STR     "SPECIAL"
#define TO_SPECIAL_SHT     "SPC"
#define TO_JERK_STR        "JERK"
#define TO_JERK_SHT        "J"
#define TO_MBI_STR         "MUST_BE_INSIDE"
#define TO_MBI_SHT         "MBI"
#define TO_NAW_STR         "NOT_ALLOWED_WITHIN"
#define TO_NAW_SHT         "NAW"
#define TO_AUTOCLOSE_STR   "AUTOCLOSE"
#define TO_AUTOCLOSE_SHT   "AC"
#define TO_ONLYONCE_STR    "ONLYONCE"
#define TO_ONLYONCE_SHT    "1"
#define TO_OBSOLETE_STR    "OBSOLETE"
#define TO_OBSOLETE_SHT    "O"
#define TO_REQUIRED_STR    "REQUIRED"
#define TO_REQUIRED_SHT    "R"
#define TO_SKIPLF_STR      "SKIPLF"
#define TO_SKIPLF_SHT      "S"

/* TODO: think about this tag-options */
#define TO_VERS_STR        "VERS"
#define TO_VERS_SHT        "V"

/* decides if a tag is a hsc-tag */
#define is_hsc_tag( tag ) (!upstrncmp(((tag)->name),HSC_TAGID,strlen(HSC_TAGID)))

/* decides if a tag is a macro-tag */
#define is_macro_tag( tag ) ((tag)->option & HT_MACRO )

/* find closing tag on container stack */
#define find_ctag( name ) find_strtag( cltags, name )

/*
**
** extern references
**
*/
#ifndef NOEXTERN_HSCLIB_TAG_H

extern HSCTAG *new_hsctag( STRPTR newid );
extern VOID    del_hsctag( APTR data );
extern HSCTAG *cpy_hsctag( HSCTAG *oldtag );

extern int     cmp_strtag( APTR cmpstr, APTR tagdata );
extern HSCTAG *find_strtag( DLLIST *taglist, STRPTR name );
extern int     cmp_strctg( APTR cmpstr, APTR tagstr );

extern HSCTAG *app_tag( DLLIST *taglist, STRPTR tagid );

#endif /* NOEXTERN_HSCLIB_TAG_H */

#endif /* HSCLIB_TAG_H */
