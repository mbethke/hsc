/*
 * hsc/callback.h
 *
 * callbacks of hsc for hsclib
 *
 */

#ifndef HSC_CALLBACK_H
#define HSC_CALLBACK_H

/*
 * global funcs
 */
#ifndef NOEXTERN_HSC_CALLBACK_H

extern BOOL hsc_nomem_handler(size_t size);

extern BOOL init_callback(HSCPRC * hp);

extern BOOL init_msgfile(HSCPRC * hp, STRPTR fname);
extern VOID cleanup_msgfile(VOID);

#endif /* NOEXTERN_HSC_CALLBACK_H */
#endif /* HSC_CALLBACK_H */

