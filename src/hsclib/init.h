/*
** config.h
**
** configuration & initialisation functions
*/
#ifndef HSCLIB_INIT_H                   /* avoid include twice */
#define HSCLIB_INIT_H

#ifndef NOEXTERN_HSCLIB_INIT_H

extern HSCPRC *hsc_read_base_info( VOID );
extern BOOL hsc_copy_base_info( HSCPRC *dest_hp, HSCPRC *dummy_hp );

extern BOOL hsc_read_prefs( HSCPRC *hp );

extern BOOL hsc_init_tagsNattr( HSCPRC *hp );
extern BOOL hsc_init_basicEntities( HSCPRC *hp );
extern BOOL hsc_assign_tagCBs( HSCPRC *hp );

extern BOOL hsc_init_hscprc( HSCPRC *hp );

#endif                                 /* NOEXTERN_HSCLIB_INIT_H */
#endif                                 /* HSCLIB_INIT_H */
