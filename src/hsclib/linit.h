/*
 * hsclib/linit.h
 *
 * configuration & initialisation functions
 */
#ifndef HSCLIB_LINIT_H          /* avoid include twice */
#define HSCLIB_LINIT_H

#ifndef NOEXTERN_HSCLIB_LINIT_H

extern HSCPRC *hsc_read_base_info(VOID);
extern BOOL hsc_copy_base_info(HSCPRC * dest_hp, HSCPRC * dummy_hp);

extern BOOL hsc_init_hscprc(HSCPRC * hp, STRPTR prefs_fname);
extern BOOL hsc_init_project(HSCPRC * hp, STRPTR project_fname);

extern BOOL hsc_init_tagsNattr(HSCPRC * hp);
extern BOOL hsc_init_basicEntities(HSCPRC * hp);
extern BOOL hsc_assign_tagCBs(HSCPRC * hp);

#endif /* NOEXTERN_HSCLIB_LINIT_H */
#endif /* HSCLIB_LINIT_H */

