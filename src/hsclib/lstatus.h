/*
 * hsclib/lstatus.h
 *
 * status messages
 *
 */

#ifndef HSCLIB_LSTATUS_H
#define HSCLIB_LSTATUS_H

extern VOID hsc_status_misc(HSCPRC * hp, STRPTR s);
extern VOID hsc_status_file_begin(HSCPRC * hp, STRPTR filename);
extern VOID hsc_status_file_end(HSCPRC * hp);
extern VOID hsc_status_line(HSCPRC * hp);

#endif /* HSCLIB_LSTATUS_H */

