/*
 * hsclib/document.h
 *
 * document managment routines for hsc
 *
 */

#ifndef HSCLIB_DOCUMENT_H
#define HSCLIB_DOCUMENT_H

/* document structure */
typedef struct document_node {
    STRPTR docname;             /* document name */
    STRPTR sourcename;          /* main sourcefile name */
    EXPSTR *title;              /* title specified with <TITLE> */
    DLLIST *iddefs;             /* list of IDs defined in this file */
    DLLIST *includes;           /* list of included files */
    DLLIST *references;         /* list of docs/images/.. references
                                 *   within this document */
    ULONG  flags;               /* document flags */
} HSCDOC;

/* document flags */
#define DF_INFO_IDDEF     (1<<0)
#define DF_CALL_IDDEF     (1<<1)
#define DF_INFO_REFERENCE (1<<2)
#define DF_CALL_REFERENCE (1<<3)
#define DF_INFO_INCLUDE   (1<<4)
#define DF_CALL_INCLUDE   (1<<5)

/* caller structure */
typedef struct caller_node {
    STRPTR name;
    ULONG posx;
    ULONG posy;
} CALLER;

/* reference structure */
typedef struct reference_node {
    STRPTR name;
    CALLER *caller;
} HSCREF;

/* include structure */
typedef struct include_node {
    STRPTR name;
    CALLER *caller;
} HSCINC;

/* id-definition structure */
typedef struct iddef_node {
    STRPTR name;
    CALLER *caller;
    INFILEPOS *fpos; /* only used by local IDs for error-position */
} HSCIDD;

extern CALLER *new_caller(STRPTR fname, ULONG posx, ULONG posy);
extern VOID del_caller(APTR data);
extern CALLER *fpos2caller(INFILEPOS * fpos);

extern HSCDOC *new_document(STRPTR docname);
extern VOID del_document(APTR data);
extern int cmp_document(APTR cmp_data, APTR list_data);
extern HSCDOC *find_document(DLLIST * list, STRPTR name);

extern VOID del_reference(APTR data);
extern HSCREF *new_reference(STRPTR newname);
extern int cmp_reference(APTR cmp_data, APTR list_data);
extern HSCREF *app_reference(HSCDOC * document, STRPTR ref_name);

extern VOID del_include(APTR data);
extern HSCINC *new_include(STRPTR newname);
extern int cmp_include(APTR cmp_data, APTR list_data);
extern HSCINC *app_include(HSCDOC * document, STRPTR inc_name);

extern VOID del_iddef(APTR data);
extern HSCIDD *new_iddef(STRPTR newname);
extern VOID prt_iddef(FILE * stream, APTR data);
extern int cmp_iddef(APTR cmp_data, APTR list_data);
extern HSCIDD *app_iddef(HSCDOC * document, STRPTR iddef_name);
extern HSCIDD *find_iddef(HSCDOC * document, STRPTR name);

#endif /* HSCLIB_DOCUMENT_H */

