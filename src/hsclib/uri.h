/*
 * uri.h
 *
 * functions for parsing URIs
 *
 */

#ifndef HSCLIB_URI_H
#define HSCLIB_URI_H

#define ABSURI_ID ":"

typedef enum
{
    URI_abs,                    /* starts with ":" */
    URI_ext,                    /* contains ":" before first "/" */
    URI_rel,                    /* no ":" */
    URI_relserv                 /* starts with "/"; server relative */
}
URIKIND;                        /* uri kinds */

/*
 * global funcs
 */
#ifndef NOEXTERN_HSCLIB_URI_H

extern VOID conv_path2uri(EXPSTR * dest, STRPTR path);
extern VOID conv_uri2path(EXPSTR * dest, STRPTR uri);

extern VOID conv_hscuri2file(HSCPRC * hp, EXPSTR * dest_fname, STRPTR uri);

extern VOID parse_uri(HSCPRC * hp, EXPSTR * dest_uri, STRPTR uri);
extern URIKIND uri_kind(STRPTR uri);

#endif /* NOEXTERN_HSCLIB_URI_H */

#endif /* HSCLIB_URI_H */

