/*
 * hscprj/project.h
 *
 * project managment routines for hsc
 *
 */

#ifndef HSCPRJ_PROJECT_H
#define HSCPRJ_PROJECT_H

/*
 * project-structure
 */
typedef struct hscproject
{
    HSCDOC *document;           /* current document */
    DLLIST *documents;          /* other documents */
    INFILE *inpf;
    BOOL debug;
    BOOL fatal;
    APTR user_data;             /* user data; hsclib assigns the current
                                 * hsc-process to it */

    /* callbacks */
      VOID(*CB_msg_unknown_id) (struct hscproject * hp,
                                STRPTR document, STRPTR id);
      VOID(*CB_msg_corrupt_pf) (struct hscproject * hp, STRPTR reason);
}
HSCPRJ;

/* return-codes for check_document_id */
#define ERR_CDI_OK              0
#define ERR_CDI_NoID            1
#define ERR_CDI_NoDocumentEntry 2

extern int check_document_id(HSCPRJ * hp, STRPTR docname, STRPTR id);

extern HSCPRJ *new_project( VOID );
extern VOID del_project(HSCPRJ *hp);

extern BOOL hsc_project_read_file(HSCPRJ * hp, INFILE * inpf);
extern BOOL hsc_project_write_file(HSCPRJ * hp, STRPTR project_fname);

extern BOOL hsc_project_add_document(HSCPRJ *hp);
extern BOOL hsc_project_del_document(HSCPRJ * hp, STRPTR docname);

extern BOOL hsc_project_set_filename( HSCPRJ *hp, STRPTR new_prjname);
extern BOOL hsc_project_set_document( HSCPRJ *hp, STRPTR new_docname);
extern BOOL hsc_project_set_source( HSCPRJ *hp, STRPTR new_sourcename);
extern BOOL hsc_project_add_include( HSCPRJ *hp, STRPTR new_includename);

#endif /* HSCPRJ_PROJECT_H */

