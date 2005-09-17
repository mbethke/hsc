/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * hscprc.c
 *
 * hsc process functions
 *
 */

#define NOEXTERN_HSCLIB_HSCPRC

#include "hsclib/inc_base.h"

#include "ugly/fname.h"
#include "ugly/ustrlist.h"
#include "ugly/returncd.h"

#include "hscprj/project.h"

#include "hsclib/idref.h"
#include "hsclib/tag_if.h"
#include "hsclib/css.h"
#include "hsclib/regmatch.h"

/*
 * del_inpf_stack_node
 *
 * remove node from input file stack
 */
static void del_inpf_stack_node(APTR data)
{
    /* do nufin */
}

/*
 * del_hscprc
 *
 * Remove HSC process and all its resources
 */
void del_hscprc(HSCPRC *hp)
{
    if (hp) {
        /* remove list */
        del_hsctree(hp->defstyle);
        del_dllist(hp->defent);
        del_hsctree(hp->deftag);
        del_dllist(hp->defattr);
        del_hsctree(hp->deflazy);
        del_dllist(hp->container_stack);
        del_dllist(hp->content_stack);
        del_dllist(hp->inpf_stack);
        del_dllist(hp->idrefs);
        del_dllist(hp->select_stack);
        del_dllist(hp->tag_styles);
        del_strlist(hp->include_dirs);

        /* remove precompiled regular expressions */
        hscregfreeall(hp->re_uri);

        /* remove strings */
        del_estr(hp->destdir);
        del_estr(hp->reldir);
        del_estr(hp->iconbase);
        del_estr(hp->server_dir);
        del_estr(hp->if_stack);
        del_estr(hp->tag_name_str);
        del_estr(hp->tag_attr_str);
        del_estr(hp->tag_close_str);
        del_estr(hp->tmpstr);
        del_estr(hp->curr_msg);
        del_estr(hp->curr_ref);
        del_estr(hp->whtspc);

        ufreestr(hp->filename_document);
        ufreestr(hp->strip_tags);

        ufree(hp->image_buffer);

        /* remove project-data */
        del_project(hp->project);

        /* close input files */
        infclose(hp->inpf);

        /* remove message arrays */
        ufree(hp->msg_ignore);
        ufree(hp->msg_class);

        ufree(hp);
    }
}

/*
 * reset_hscprc
 *
 * reset all items of a hsc process
 */
void reset_hscprc(HSCPRC * hp)
{
    int i;

    /* get current time */
    hp->start_time = time(NULL);

    /* reset strings */
    clr_estr(hp->destdir);
    clr_estr(hp->reldir);
    clr_estr(hp->if_stack);
    clr_estr(hp->whtspc);

    hp->suppress_output = TRUE;
    hp->fatal = FALSE;
    hp->prev_heading_num = 0;
    hp->prev_status_line = (ULONG) - 1;
    hp->msg_count = 0;

    hp->inside_pre = FALSE;
    hp->inside_anchor = FALSE;
    hp->inside_title = FALSE;

    hp->tag_next_whtspc = NULL;
    hp->strip_badws = FALSE;
    hp->strip_next_whtspc = FALSE;
    hp->strip_next2_whtspc = FALSE;

    ufreestr(hp->strip_tags);

    /* check for prostitute */
    hp->prostitute = (getenv(ENV_HSCSALARY) != NULL);

    hp->nested_errors = TRUE;
    hp->lctags = FALSE;
    hp->xhtml = FALSE;

    /* allow infinite number messages */
    hp->max_messages = MAXIMUM_MESSAGE_INFINITE;
    hp->max_errors = MAXIMUM_MESSAGE_INFINITE;

    /* enable all messages */
    hp->msg_ignore_notes = FALSE;      
    hp->msg_ignore_style = FALSE;
    hp->msg_ignore_port = FALSE;
    for (i = 0; i <= MAX_MSGID; i++)
    {
        hp->msg_ignore[i] = FALSE;
        hp->msg_class[i] = MSG_NOTE;
    }
}

#define REGEXP_URI "^(([a-zA-Z][-0-9a-zA-Z+.]*:)?/{0,2}[-0-9a-zA-Z;/?:@&=+$._!~*'()%]+)?" \
                   "(#[-0-9a-zA-Z;/?:@&=+$._!~*'()%]+)?$"

/*
 * new_hscprc
 *
 * create and init a new hsc process
 */
HSCPRC *new_hscprc(void)
{
    HSCPRC *hp = NULL;

    hp = (HSCPRC *) umalloc(sizeof(HSCPRC));
    if (hp)
    {
        memset(hp, 0, sizeof(HSCPRC));

        /* init lists */
        hp->defent = init_dllist(del_entity);
        hp->deftag = init_hsctree(free_tag_node, cmp_tag_node, ubi_trOVERWRITE);
        hp->defattr = init_dllist(del_hscattr);
        hp->deflazy = init_hsctree(free_tag_node, cmp_tag_node, ubi_trOVERWRITE);
        hp->defstyle = init_hsctree(free_style_node, cmp_style_node, ubi_trOVERWRITE);
        hp->container_stack = init_dllist(del_hsctag);
        hp->content_stack = init_dllist(del_string_node);
        hp->inpf_stack = init_dllist(del_inpf_stack_node);
        hp->project = NULL;
        hp->idrefs = init_dllist(del_idref);
        hp->select_stack = init_dllist(del_select_stack_node);
        hp->tag_styles = init_dllist(&del_styleattr);
        hp->include_dirs = init_strlist();

        /* precompile some regular expressions */
        hp->re_uri = hscregcomp(hp, REGEXP_URI, TRUE, TRUE);

        /* init strings */
        hp->destdir = init_estr(0);
        hp->reldir = init_estr(0);
        hp->iconbase = init_estr(0);
        hp->server_dir = init_estr(0);
        hp->if_stack = init_estr(0);
        hp->tag_name_str = init_estr(128);
        hp->tag_attr_str = init_estr(128);
        hp->tag_close_str = init_estr(0);
        hp->tmpstr = init_estr(0);
        hp->curr_msg = init_estr(64);
        hp->curr_ref = init_estr(64);
        hp->whtspc = init_estr(0);

        /* allocate message arrays */
        hp->msg_ignore = (HSCIGN *)
            umalloc((MAX_MSGID + 1) * sizeof(HSCIGN));
        hp->msg_class = (HSCMSG_CLASS *)
            umalloc((MAX_MSGID + 1) * sizeof(HSCMSG_CLASS));

        /* allocate image buffer */
        hp->image_buffer = (unsigned char *) umalloc(IMAGE_BUFFER_SIZE);

        reset_hscprc(hp);
    }
    return (hp);
}

/*
 *
 * GET-methods for public item of HSCPRC
 *
 */

/*
 * get flags
 */
BOOL hsc_get_chkid(HSCPRC * hp) {
    return (hp->chkid);
}

BOOL hsc_get_chkuri(HSCPRC * hp) {
    return (hp->chkuri);
}

BOOL hsc_get_compact(HSCPRC * hp) {
    return (hp->compact);
}

BOOL hsc_get_debug(HSCPRC * hp) {
    return (hp->debug);
}

BOOL hsc_get_getsize(HSCPRC * hp) {
    return (hp->getsize);
}

BOOL hsc_get_htmlonly(HSCPRC * hp) {
    return (hp->htmlonly);
}

BOOL hsc_get_jerkvalues(HSCPRC * hp) {
    return (hp->jerkvalues);
}

BOOL hsc_get_rplc_ent(HSCPRC * hp) {
    return (hp->rplc_ent);
}

BOOL hsc_get_rplc_quote(HSCPRC * hp) {
    return (hp->rplc_quote);
}

BOOL hsc_get_smart_ent(HSCPRC * hp) {
    return (hp->smart_ent);
}

BOOL hsc_get_strip_badws(HSCPRC * hp) {
    return (hp->strip_badws);
}

BOOL hsc_get_strip_cmt(HSCPRC * hp) {
    return (hp->strip_cmt);
}

BOOL hsc_get_strip_ext(HSCPRC * hp) {
    return (hp->strip_ext);
}

BOOL hsc_get_nested_errors(HSCPRC * hp) {
    return (hp->nested_errors);
}

BOOL hsc_get_lctags(HSCPRC * hp) {
    return (hp->lctags);
}

BOOL hsc_get_xhtml(HSCPRC * hp) {
    return (hp->xhtml);
}

/*
 * get internal flags
 */
BOOL hsc_get_suppress_output(HSCPRC * hp) {
    return (hp->suppress_output);
}

BOOL hsc_get_inside_pre(HSCPRC * hp) {
    return (hp->inside_pre);
}

BOOL hsc_get_inside_anchor(HSCPRC * hp) {
    return (hp->inside_anchor);
}

BOOL hsc_get_fatal(HSCPRC * hp) {
    return (hp->fatal);
}

/*
 * get values
 */
STRPTR hsc_get_destdir(HSCPRC * hp) {
    return (estr2str(hp->destdir));
}

STRPTR hsc_get_reldir(HSCPRC * hp) {
    return (estr2str(hp->reldir));
}

STRPTR hsc_get_iconbase(HSCPRC * hp) {
    return (estr2str(hp->iconbase));
}

STRPTR hsc_get_server_dir(HSCPRC * hp) {
    return (estr2str(hp->server_dir));
}


/*
 * get internal values
 */
STRPTR hsc_get_click_here_str(HSCPRC * hp) {
    return (hp->click_here_str);
}

STRPTR hsc_get_file_name(HSCPRC * hp) {
    if (hp->inpf)
        return (infget_fname(hp->inpf));
    else
        return (NULL);
}

ULONG hsc_get_file_line(HSCPRC * hp) {
    if (hp->inpf)
        return (infget_y(hp->inpf));
    else
        return (0);
}

ULONG hsc_get_file_column(HSCPRC * hp) {
    if (hp->inpf)
        return (infget_x(hp->inpf));
    else
        return (0);
}

ULONG hsc_get_msg_count(HSCPRC * hp) {
    return (hp->msg_count);
}

/*
 *
 * SET-methodes for public item of HSCPRC
 *
 */

/*
 * set values
 */
BOOL hsc_set_destdir(HSCPRC * hp, STRPTR dir) {
    set_estr(hp->destdir, dir);
    /* append "/" if neccessary */
    link_fname(hp->destdir, estr2str(hp->destdir), "");
    D(fprintf(stderr, DHL "destdir=`%s'\n", estr2str(hp->destdir)));

    return (TRUE);
}

BOOL hsc_set_reldir(HSCPRC * hp, STRPTR dir) {
    /* append "/" if neccessary */
    link_fname(hp->reldir, dir, "");
    D(fprintf(stderr, DHL "reldir =`%s'\n", estr2str(hp->reldir)));
    return (TRUE);
}

BOOL hsc_set_iconbase(HSCPRC * hp, STRPTR uri) {
    set_estr(hp->iconbase, uri);
    D(fprintf(stderr, DHL "iconbase=`%s'\n", estr2str(hp->iconbase)));
    return (TRUE);
}

BOOL hsc_set_server_dir(HSCPRC * hp, STRPTR dir) {
    set_estr(hp->server_dir, dir);

    /* if dir does not already end with a directory separator, append it now */
    if (!strchr(PATH_SEPARATOR, last_ch(dir))) {
        app_estrch(hp->server_dir, PATH_SEPARATOR[0]);
    }
    D(fprintf(stderr, DHL "serverdir=`%s'\n", estr2str(hp->server_dir)));
    return (TRUE);
}

BOOL hsc_set_strip_tags(HSCPRC * hp, STRPTR taglist) {
    reallocstr(&(hp->strip_tags), taglist);
    return (TRUE);
}

BOOL hsc_set_filename_document(HSCPRC * hp, STRPTR filename) {
    BOOL ok = FALSE;

    D(fprintf(stderr, DHL "document=`%s'\n", filename));
    hp->filename_document = strclone(filename);
    return (ok);
}

/*
 * set flags
 */
void hsc_set_chkid(HSCPRC * hp, BOOL new_chkid) {
    hp->chkid = new_chkid;
    D(fprintf(stderr, DHL "flag: chkid    =%d\n", new_chkid));
}

void hsc_set_chkuri(HSCPRC * hp, BOOL new_chkuri) {
    hp->chkuri = new_chkuri;
    D(fprintf(stderr, DHL "flag: chkuri   =%d\n", new_chkuri));
}

void hsc_set_compact(HSCPRC * hp, BOOL new_compact) {
    hp->compact = new_compact;
    D(fprintf(stderr, DHL "flag: compact  =%d\n", new_compact));
}

void hsc_set_debug(HSCPRC * hp, BOOL new_debug) {
    hp->debug = new_debug;
    D(fprintf(stderr, DHL "flag: debug    =%d\n", new_debug));
}

void hsc_set_getsize(HSCPRC * hp, BOOL new_getsize) {
    hp->getsize = new_getsize;
    D(fprintf(stderr, DHL "flag: getsize  =%d\n", new_getsize));
}

void hsc_set_jerkvalues(HSCPRC * hp, BOOL new_jerkvalues) {
    hp->jerkvalues = new_jerkvalues;
    D(fprintf(stderr, DHL "flag: jerkval  =%d\n", new_jerkvalues));
}

void hsc_set_rplc_ent(HSCPRC * hp, BOOL new_rplc_ent) {
    hp->rplc_ent = new_rplc_ent;
    D(fprintf(stderr, DHL "flag: rplc_ent =%d\n", new_rplc_ent));
}

void hsc_set_rplc_quote(HSCPRC * hp, BOOL new_rplc_quote) {
    hp->rplc_quote = new_rplc_quote;
    D(fprintf(stderr, DHL "flag: rplc_quote =%d\n", new_rplc_quote));
}

void hsc_set_smart_ent(HSCPRC * hp, BOOL new_smart_ent) {
    hp->smart_ent = new_smart_ent;
    D(fprintf(stderr, DHL "flag: smart_ent=%d\n", new_smart_ent));
}

void hsc_set_strip_badws(HSCPRC * hp, BOOL new_strip_badws) {
    hp->strip_badws = new_strip_badws;
    D(fprintf(stderr, DHL "flag: strip_bws=%d\n", new_strip_badws));
}

void hsc_set_strip_cmt(HSCPRC * hp, BOOL new_strip_cmt) {
    hp->strip_cmt = new_strip_cmt;
    D(fprintf(stderr, DHL "flag: strip_cmt=%d\n", new_strip_cmt));
}

void hsc_set_strip_ext(HSCPRC * hp, BOOL new_strip_ext) {
    hp->strip_ext = new_strip_ext;
    D(fprintf(stderr, DHL "flag: strip_ext=%d\n", new_strip_ext));
}

void hsc_set_no_nested_errors(HSCPRC * hp, BOOL new_nested_errors) {
    hp->nested_errors = !new_nested_errors;
    D(fprintf(stderr, DHL "flag: nested_errors=%d\n", new_nested_errors));
}

void hsc_set_lctags(HSCPRC * hp, BOOL new_lctags) {
   hp->lctags = new_lctags;
   D(fprintf(stderr, DHL "flag: lctags=%d\n", new_lctags));
}

void hsc_set_checkext(HSCPRC * hp, BOOL new_checkext) {
   hp->checkext = new_checkext;
   D(fprintf(stderr, DHL "flag: checkext=%d\n", new_checkext));
}

void hsc_set_xhtml(HSCPRC * hp, BOOL new_xhtml) {
    if((hp->xhtml = new_xhtml)) {
       hp->lctags = TRUE;            /* XHTML implies LOWERCASETAGS */
       hp->quotemode = QMODE_DOUBLE; /* use double quotes */
       hp->validate_css = TRUE;      /* CSS should be validated */
       hp->entitymode = EMODE_UTF8;  /* output UTF-8 instead of ASCII entities */
    }
    D(fprintf(stderr, DHL "flag: xhtml=%d\n", new_xhtml));
}

void hsc_set_novcss(HSCPRC * hp, BOOL new_vcss) {
   hp->validate_css = !new_vcss;
   D(fprintf(stderr, DHL "flag: validatecss=%d\n", new_vcss));
}

/*
 * set values
 */
void hsc_set_quote_mode(HSCPRC * hp, LONG new_mode) {
   hp->quotemode = new_mode;
   D(fprintf(stderr, DHL "quote_mode=%ld\n", new_mode));
}

void hsc_set_entity_mode(HSCPRC * hp, LONG new_mode) {
   if(EMODE_INVALID == new_mode)
      new_mode = EMODE_KEEP;    /* default if unspecified */
   hp->entitymode = new_mode;
   D(fprintf(stderr, DHL "entity_mode=%ld\n", new_mode));
}

void hsc_set_maximum_messages(HSCPRC * hp, LONG messages) {
   hp->max_messages = messages;
}

void hsc_set_maximum_errors(HSCPRC * hp, LONG errors) {
   hp->max_errors = errors;
}

/*
 * set call-backs
 */
void hsc_set_status_file_begin(HSCPRC * hp, void(*status_file) (HSCPRC * hp, STRPTR filename)) {
    hp->CB_status_file_begin = status_file;
}

void hsc_set_status_file_end(HSCPRC * hp, void(*status_file) (HSCPRC * hp)) {
    hp->CB_status_file_end = status_file;
}

void hsc_set_status_line(HSCPRC * hp, void(*status_line) (HSCPRC * hp)) {
    hp->CB_status_line = status_line;
}

void hsc_set_status_misc(HSCPRC * hp, void(*status_misc) (HSCPRC * hp, STRPTR s)) {
    hp->CB_status_misc = status_misc;
}

void hsc_set_message(
                        HSCPRC * hp,
                        void(*message) (struct hsc_process * hp,
                                        HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                                        STRPTR fname, ULONG x, ULONG y,
                                        STRPTR msg_text))
{
    hp->CB_message = message;
}

void hsc_set_message_ref(
                            HSCPRC * hp,
                            void(*message_ref) (struct hsc_process * hp,
                                   HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                                             STRPTR fname, ULONG x, ULONG y,
                                                STRPTR msg_text))
{
    hp->CB_message_ref = message_ref;
}

void hsc_set_start_tag(HSCPRC * hp,
                       void(*start_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close))
{
    hp->CB_start_tag = start_tag;
}

void hsc_set_end_tag(HSCPRC * hp,
                     void(*end_tag) (struct hsc_process * hp,
          HSCTAG * tag, STRPTR tag_name, STRPTR tag_attr, STRPTR tag_close))
{
    hp->CB_end_tag = end_tag;
}

void hsc_set_text(HSCPRC * hp,
                  void(*text) (struct hsc_process * hp,
                               STRPTR white_spaces, STRPTR text))
{
    hp->CB_text = text;
}

void hsc_set_id(HSCPRC * hp,
                void(*id) (struct hsc_process * hp,
                           HSCATTR * attr, STRPTR id))
{
    hp->CB_id = id;
}

/*
 * message methodes
 */
BOOL hsc_set_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id, HSCIGN value) {
    BOOL set = FALSE;

    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
        hp->msg_ignore[msg_id & MASK_MESSAGE] = value;
    return (set);
}

HSCIGN hsc_get_msg_ignore(HSCPRC * hp, HSCMSG_ID msg_id) {
    /* HSCMSG_ID max_msgid = MAX_MSGID; */
    return ((msg_id & MASK_MESSAGE) <= MAX_MSGID) ?
        hp->msg_ignore[msg_id & MASK_MESSAGE] : FALSE;
}

BOOL hsc_set_msg_class(HSCPRC * hp, HSCMSG_ID msg_id, HSCMSG_CLASS msg_class) {
    BOOL set = FALSE;

    if ((msg_id & MASK_MESSAGE) <= MAX_MSGID)
        hp->msg_class[msg_id & MASK_MESSAGE] = msg_class;
    return (set);
}

HSCMSG_CLASS hsc_get_msg_class(HSCPRC * hp, HSCMSG_ID msg_id) {
    HSCMSG_ID msg_id_unmasked = msg_id & MASK_MESSAGE;
    if (msg_id_unmasked <= MAX_MSGID) {
        HSCMSG_CLASS mchp = hp->msg_class[msg_id_unmasked];
        HSCMSG_CLASS mcid = msg_id & MASK_MSG_CLASS;
        return (mcid > mchp) ? mcid : mchp;
    }
    return (MSG_NONE);
}

/*
 * set message classes to be ignored
 */
BOOL hsc_get_msg_ignore_notes(HSCPRC * hp) {
    return hp->msg_ignore_notes;
}

BOOL hsc_get_msg_ignore_style(HSCPRC * hp) {
    return hp->msg_ignore_style;
}

BOOL hsc_get_msg_ignore_port(HSCPRC * hp) {
    return hp->msg_ignore_port;
}

BOOL hsc_set_msg_ignore_notes(HSCPRC * hp, BOOL value) {
    hp->msg_ignore_notes = value;
    return (value);
}

BOOL hsc_set_msg_ignore_style(HSCPRC * hp, BOOL value) {
    hp->msg_ignore_style = value;
    return (value);
}

BOOL hsc_set_msg_ignore_port(HSCPRC * hp, BOOL value) {
    hp->msg_ignore_port = value;
    return (value);
}


/*
 * reset whole arrays for message-class/ignores
 */
void hsc_clear_msg_ignore(HSCPRC * hp) {
    size_t i;
    for (i = 0; i <= MAX_MSGID; i++)
        hp->msg_ignore[i] = FALSE;
}

void hsc_reset_msg_class(HSCPRC * hp) {
    size_t i;
    for (i = 0; i <= MAX_MSGID; i++)
        hp->msg_class[i] = MSG_NONE;
}

/*
 * methods for include-directories
 */
BOOL hsc_add_include_directory(HSCPRC * hp, STRPTR dir) {
   if(strlen(dir) > MAX_FPATH-2)
      return FALSE;
   return ((BOOL) (app_strnode(hp->include_dirs, dir) != NULL));
}

void hsc_clr_include_directory(HSCPRC * hp) {
   clr_strlist(hp->include_dirs);
}

/*
 *-------------------------------------
 * misc. funtions for white spaces
 *-------------------------------------
 */

/*
 * compactWs - reduce white space string to blank
 *             or linefeed
 */
STRPTR compactWs(HSCPRC * hp, STRPTR ws) {
    STRPTR newWs = "";

    if (ws[0]) {
        /* any white spaces at all */
        STRPTR containsLF = strchr(ws, '\n');
        STRPTR containsCR = strchr(ws, '\r');

        if (containsLF) {
            if (containsCR)
                newWs = "\r\n";
            else
                newWs = "\n";
        } else {
           if (containsCR)
              newWs = "\r";
           else
              newWs = " ";
        }
    }
    return (newWs);
}

/*
 * output text function
 *
 * output text to host process
 *
 * params: hp....hsc process to perform ouput with
 *         wspc..white spaces
 *         text..other text
 * result: true, if text has been outputted
 */
BOOL hsc_output_text(HSCPRC * hp, STRPTR wspc, STRPTR text) {
    BOOL written = FALSE;
    if ((hp)->CB_text && !((hp)->suppress_output)) {
        /* add current white spaces to white space
         * buffer; if hp->compact is enabled, reduce
         * white spaces */
        if (wspc)
            app_estr(hp->whtspc, wspc);
        if (hp->compact && (!hp->inside_pre))
            /* reduce white spaces */
            wspc = compactWs(hp, estr2str(hp->whtspc));
        else
            wspc = estr2str(hp->whtspc);

        /* strip white spaces if requested */
        if (hp->strip_next_whtspc) {
            D(fprintf(stderr, DHL "bad white spaces stripped\n"));
            hp->strip_next_whtspc = FALSE;
            wspc = "";
        } else if (hp->strip_next2_whtspc) {
            hp->strip_next2_whtspc = FALSE;
            hp->strip_next_whtspc = TRUE;
        } else if ((hp->tag_next_whtspc) && strlen(wspc)) {
            hsc_message(hp, MSG_SUCC_WHTSPC,
                        "succeeding white-space for %T",
                        hp->tag_next_whtspc);
        }
        hp->tag_next_whtspc = NULL;

#if DEBUG_HSCLIB_OUTPUT
        if (hp->debug)
            if (text)
                if (strcmp(text, "\n"))
                    fprintf(stderr, DHL "ouput: `%s', `%s'\n", wspc, text);
                else
                    fprintf(stderr, DHL "ouput: `%s', `\\n'\n", wspc);
#endif
        if ((wspc && wspc[0]) || (text && text[0])) {
            /* convert NULL values to empty strings */
            if (!wspc)
                wspc = "";
            if (!text)
                text = "";

            /* output text */
            (*((hp)->CB_text)) ((hp), wspc, text);
            written = TRUE;
        }
        /* reset white space buffer */
        clr_estr(hp->whtspc);
    }
    return (written);
}

/*
 * nomem-handler
 */
BOOL hsc_standard_nomem_handler(size_t size) {
    fputs("\n*** out of memory\n\n", stderr);
    exit(RC_FAIL);
    return (FALSE);             /* abort immediately */
}

/* $Id$ */
/* vi: set ts=3 sw=3: */
