/*
** hsclib/status.h
**
** status messages
**
*/

#ifndef HSCLIB_STATUS_H
#define HSCLIB_STATUS_H

extern VOID hsc_status_misc( HSCPRC *hp, STRPTR s );
extern VOID hsc_status_file_begin( HSCPRC *hp );
extern VOID hsc_status_file_end( HSCPRC *hp );
extern VOID hsc_status_line( HSCPRC *hp );

#endif /* HSC_STATUS_H */

