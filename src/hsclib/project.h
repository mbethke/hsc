/*
 * hsclib/project.h
 *
 * project managment routines for hsc
 *
 */

#ifndef HSCLIB_PROJECT_H
#define HSCLIB_PROJECT_H

extern VOID check_document_id(HSCPRC * hp, STRPTR docname, STRPTR id);

extern BOOL hsc_read_project_file(HSCPRC * hp);
extern BOOL hsc_write_project_file(HSCPRC * hp);

#endif /* HSCLIB_PROJECT_H */

