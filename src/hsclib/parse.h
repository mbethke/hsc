/*
** hsclib/parse.h
**
** functions for parsing of hsc files
**
*/

#ifndef HSC_PARSE_H
#define HSC_PARSE_H

/*
** global funcs
*/
#ifndef NOEXTERN_HSCLIB_PARSE_H

extern BOOL hsc_parse( HSCPRC *hp );
extern BOOL hsc_parse_tag( HSCPRC *hp );
extern BOOL hsc_parse_amp( HSCPRC *hp );
extern BOOL hsc_parse_text( HSCPRC *hp );
extern BOOL hsc_parse_source( HSCPRC *hp );
extern BOOL hsc_parse_end( HSCPRC *hp );
extern BOOL hsc_parse_end_id( HSCPRC *hp );

extern VOID remove_ctag( HSCPRC *hp, HSCTAG *tag );

#endif /* NOEXTERN_PARSE_H */
#endif /* HSC_PARSE_H */

