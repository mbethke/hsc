/*
 * tag_if.h
 *
 * tag handles for <$IF> and <$ELSE>
 *
 */

#ifndef HSCLIB_TAG_IF_H
#define HSCLIB_TAG_IF_H

/*
 *
 * extern references
 *
 */
extern BOOL handle_hsc_if(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_cif(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_else(HSCPRC * hp, HSCTAG * tag);
extern BOOL handle_hsc_elseif(HSCPRC * hp, HSCTAG * tag);

extern VOID del_select_stack_node(APTR data);

#endif /* HSCLIB_TAG_IF_H */

