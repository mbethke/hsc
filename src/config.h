/*
** config.h
**
** updated: 31-Aug-1995
** created: 12-Jul-1995
*/
#ifndef HSC_CONFIG_H                   /* avoid include twice */
#define HSC_CONFIG_H

#define HSC_CONFIG_FILENAME "hscdef.cfg"


#ifndef NOEXTERN_HSC_CONFIG_H
extern BOOL config_ok( void );
extern HSCTAG *add_tag(
  STRPTR tagid,
  ULONG  optn,
  ULONG  version,
  BOOL   (*op_handle)(INFILE *inpf),
  BOOL   (*cl_handle)(INFILE *inpf));
#endif

#endif                                 /* HSC_CONFIG_H */
