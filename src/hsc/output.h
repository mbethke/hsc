/*
** hsc/output.h
**
** output functions for hsc
*/

#ifndef HSC_OUTPUT_H
#define HSC_OUTPUT_H

extern BOOL init_output( HSCPRC *hp );
extern VOID cleanup_output( VOID );

extern BOOL write_output( HSCPRC *hp );
extern VOID append_output( STRPTR text );

#endif /* HSC_OUTPUT_H */
