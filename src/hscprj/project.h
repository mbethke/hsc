/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * hscprj/project.h
 *
 * project managment routines for hsc
 *
 */

#ifndef HSCPRJ_PROJECT_H
#define HSCPRJ_PROJECT_H
#include "hscprj/document.h"

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

extern BOOL hsc_project_read_data(HSCPRJ * hp, INFILE * inpf);
extern BOOL hsc_project_write_data(HSCPRJ * hp, STRPTR project_fname, BOOL force);

extern BOOL hsc_project_add_document(HSCPRJ *hp);
extern BOOL hsc_project_del_document(HSCPRJ * hp, STRPTR docname);

extern BOOL hsc_project_set_filename( HSCPRJ *hp, STRPTR new_prjname);
extern BOOL hsc_project_set_document( HSCPRJ *hp, STRPTR new_docname);
extern BOOL hsc_project_set_source( HSCPRJ *hp, STRPTR new_sourcename);
extern BOOL hsc_project_add_include( HSCPRJ *hp, STRPTR new_includename);

#endif /* HSCPRJ_PROJECT_H */

