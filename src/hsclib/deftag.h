/*
** hsclib/deftag.h
*/

#ifndef HSCLIB_DEFTAG_H
#define HSCLIB_DEFTAG_H

#ifndef NOEXTERN_HSCLIB_DEFTAG_H

extern LONG get_mci( HSCPRC *hp );
extern VOID unget_mci( HSCPRC *hp );
extern LONG get_current_mci( HSCPRC *hp );

extern ULONG   set_tag_args( HSCPRC *hp, HSCTAG *tag );
extern BOOL    def_tag_args( HSCPRC *hp, HSCTAG *tag );
extern HSCTAG *def_tag_name( HSCPRC *hp, BOOL *open_tag );

#endif /* NOEXTERN_HSCLIB_DEFTAG_H */

#endif /* HSCLIB_DEFTAG_H */
