/*
** hsc/status.h
**
** status messages
**
*/

#ifndef HSC_STATUS_H
#define HSC_STATUS_H

#include <stdio.h>

#include "ugly/types.h"
#include "ugly/infile.h"

#define STATUS_QUIET            0 /* no status */
#define STATUS_LINE        (1<<0) /* line which parser currently processes */
#define STATUS_VERSION     (1<<1) /* display program version  */
#define STATUS_VERBOSE     (1<<2) /* more verbose output */

#define STATUS_QUIET_STR   "quiet"
#define STATUS_LINE_STR    "line"
#define STATUS_VERSION_STR "version"
#define STATUS_VERBOSE_STR "verbose"

#define STATUS_ENUM_STR STATUS_QUIET_STR    "|" \
                        STATUS_LINE_STR     "|" \
                        STATUS_VERSION_STR  "|" \
                        STATUS_VERBOSE_STR

#define disp_status_quiet   (!disp_status)
#define disp_status_line    (disp_status & STATUS_LINE)
#define disp_status_version (disp_status & (STATUS_VERSION | STATUS_VERBOSE))
#define disp_status_verbose (disp_status & STATUS_VERBOSE)

#ifndef NOEXTERN_HSC_STATUS

/*
** global funcs & vars
*/
extern LONG disp_status;

extern VOID set_return_code( int newrc );

extern VOID status_lf( VOID );
extern VOID status_file_end( HSCPRC *hp );
extern VOID status_line( HSCPRC *hp );
extern VOID status_misc( HSCPRC *hp, STRPTR s );
extern VOID status_error( STRPTR s );
extern VOID status_clear( VOID );
extern VOID status_msg( STRPTR s );

#endif /* NOEXTERN_HSC_STATUS */

#endif /* HSC_STATUS_H */

