/*
 * hsclib/include.h
 */

#ifndef HSCLIB_INCLUDE_H
#define HSCLIB_INCLUDE_H

/*
 * flags for include_hsc_xxx()
 */
#define IH_PARSE_END    (1<<0)  /* check for unclosed tags & co at end */
#define IH_PARSE_PRE    (1<<1)  /* parse with replacing "><&" */
#define IH_PARSE_MACRO  (1<<2)  /* include macro */
#define IH_PARSE_HSC    (1<<3)  /* include text of a special
                                 *   hsc-command (eg $insert time) */
#define IH_PARSE_SOURCE (1<<4)  /* include source (replace special chars) */
#define IH_NO_STATUS    (1<<5)  /* oppress status message */
#define IH_UPDATE_PRJ   (1<<6)  /* update project file */
#define IH_POS_PARENT   (1<<7)  /* use position of first file on stack
                                 *   for messages */
#define IH_IS_INCLUDE   (1<<8)  /* included vai <$include>
                                 *   (should be stored in project-file */
#define IH_IS_SOURCE    (1<<9)  /* this is the main source-file
                                 *   (should be stored in project-file */

#ifndef NOEXTERN_HSCLIB_INCLUDE_H

extern BOOL hsc_include_file(HSCPRC * hp, STRPTR filename, ULONG optn);
extern BOOL hsc_include_string(HSCPRC * hp, STRPTR filename, STRPTR s, ULONG optn);
extern BOOL hsc_base_include_file(HSCPRC * hp, STRPTR filename, ULONG optn, INFILEPOS * base_pos);
extern BOOL hsc_base_include_string(HSCPRC * hp, STRPTR filename, STRPTR s, ULONG optn, INFILEPOS * base_pos);

#endif /* NOEXTERN_HSCLIB_INCLUDE_H */

#endif /* HSCLIB_INCLUDE_H */

