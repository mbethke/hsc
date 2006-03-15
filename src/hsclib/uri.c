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
 * uri.c
 *
 * functions for uri parsing of tag arguments
 */

#define NOEXTERN_HSCLIB_URI_H

#include "ugly/fname.h"
#include "ugly/ufile.h"

#include "hsclib/inc_base.h"
#include "hsclib/idref.h"
#include "hsclib/regmatch.h"
#include "hscprj/project.h"
#include "hsclib/uri.h"
#ifdef UNIX
#include "hsclib/tcpip.h"
#endif

#define PARENT_URI "../"        /* string for parent dir within URIs */

/*
 * conv_path2uri
 *
 * convert a path for local (system-dependant)
 * file system to URI
 */
void conv_path2uri(EXPSTR * dest, STRPTR path)
{
    clr_estr(dest);

#if defined(AMIGA) || defined(AROS)
    /* replace leading parent directories by "../" */
    while (!strncmp(path, PARENT_DIR, strlen(PARENT_DIR))) {
        app_estr(dest, PARENT_URI);
        path += strlen(PARENT_DIR);
    }

    while (path[0]) {
        /* replace all "//" by "../" */
        if ((path[0] == '/') && (path[1] == '/')) {
            app_estr(dest, PARENT_URI);
            path += 2;
        } else {
            app_estrch(dest, path[0]);
            path++;
        }
    }

#elif (defined NEXTSTEP) || (defined BEOS) || (defined UNIX) || (defined WINNT) || (defined RISCOS)
    /* simply copy path */
    set_estr(dest, path);
#else
#error "system not supported: conv_path2uri"
#endif
}

/*
 * conv_uri2path
 *
 * convert a uri to a path for local (system-dependant)
 * file system
 */
void conv_uri2path(EXPSTR * dest, STRPTR uri, BOOL weenix)
{
    clr_estr(dest);

#if defined(AMIGA) || defined(AROS)
    if (weenix) {
        /* convert leading "/" to ":" */
        /* convert leading "~" to ":" */
        if (!strncmp(uri, "/", 1)
            || !strncmp(uri, "~/", 2)
            || !strncmp(uri, "~", 1)) {
            app_estr(dest, ":");
            uri++;
        }
    }

    /* convert leading "../" to "/" */
    while (!strncmp(uri, PARENT_URI, strlen(PARENT_URI))) {
        app_estr(dest, PARENT_DIR);
        uri += strlen(PARENT_URI);
    }

    /* convert inside "../" to "//" */
    while (uri[0]) {
        if (!strncmp(uri, PARENT_URI, strlen(PARENT_URI))) {
            app_estrch(dest, '/');
            uri += strlen(PARENT_URI);
        } else {
            app_estrch(dest, uri[0]);
            uri++;
        }
    }
#elif (defined NEXTSTEP) || (defined BEOS) || (defined UNIX) || (defined WINNT) || defined(RISCOS)
    set_estr(dest, uri);
#else
#error "system not supported: conv_uri2path"
#endif
}

/*
 * uri_kind
 *
 * evaluate kind of uri
 */
URIKIND uri_kind(CONSTRPTR uri)
{
    URIKIND kind = URI_abs;

    if (upstrncmp(uri, ABSURI_ID, strlen(ABSURI_ID))) {
        if (uri[0] == '/')
            kind = URI_relserv;
        else {
            STRPTR colon_pos = strchr(uri, ':');
            STRPTR slash_pos = strchr(uri, '/');

            if (colon_pos) {
                if (slash_pos) {
                    if (colon_pos < slash_pos)
                        kind = URI_ext;
                    else
                        kind = URI_rel;
                } else kind = URI_ext;
            } else kind = URI_rel;
        }
    }
    return (kind);
}

/*
 * convert uri to filename in destination-dir
 *
 * TODO: optimize or rewrite this mess
 */
void conv_hscuri2fileNuri(HSCPRC * hp, EXPSTR * dest_uri, EXPSTR * dest_fname, STRPTR uri)
{
   EXPSTR *rel_path = init_estr(32);   /* relative path */
   EXPSTR *turi = init_estr(32);   /* modifiable URI */
   URIKIND kind = uri_kind(uri);
   
   clr_estr(dest_uri);
   clr_estr(dest_fname);

   D(fprintf(stderr, DHL "conv_hscuri2fileNuri('%s')\n",uri));
   /* check for empty URI first */
   if(0 == strlen(uri)) {
      set_estr(dest_uri,uri);
   } else {
      if (kind == URI_relserv) {
         /* skip "/" in URI */
         STRPTR uri2 = uri + 1;

         /* debug */
         D(fprintf(stderr, DHL "exists `%s' [relserv]\n", uri));

         /* convert server relative URI to local filename
          * by preceding server_dir */
         conv_uri2path(rel_path, uri2, hp->weenix);
         estrcpy(dest_fname, hp->server_dir);
         estrcat(dest_fname, rel_path);

         /* debug */
         D(fprintf(stderr, DHL "  server-dir=`%s'\n", estr2str(hp->server_dir)));
         D(fprintf(stderr, DHL "  rel. path =`%s'\n", estr2str(rel_path)));

         /* keep URI untouched */
         set_estr(dest_uri, uri);
      } else {
         /* convert relative/project uris */

         /* if a <BASE HREF="..."> was found before,
          * treat all relative URIs as absolute
          */
         if (hp->docbase_set)
            kind = URI_ext;

         if (kind == URI_abs) {
            uri += strlen(ABSURI_ID);  /* skip ":" */
            /*
             * parse absolute uri
             */
            D(fprintf(stderr, DHL "exists `%s' [abs]\n", uri));

            /* check if local uri exists */
            estrcpy(dest_fname, hp->destdir);
            if('\0' != uri[0]) {
               EXPSTR *dest_relfname = init_estr(32);

               conv_uri2path(dest_relfname, uri, hp->weenix);
               estrcat(dest_fname, dest_relfname);
               del_estr(dest_relfname);
            }

            D(fprintf(stderr, DHL "  -> file `%s'\n",
                     estr2str(dest_fname)));

            /* create path of destination file */
            estrcpy(dest_uri, hp->reldir);
            app_estr(dest_uri, uri);

            /* dir correction, so path collapsing will always work */
            set_estr(turi,uri);
            if(FE_DIR == fgetentrytype(uri)) {
               /* make sure there's a training slash */
               if('/' != uri[strlen(uri) - 1])
                  app_estr(turi,"/");
            }
            get_relfname(rel_path, estr2str(turi), estr2str(hp->reldir));
            D(fprintf(stderr, DHL "  -> rel. path `%s' (`%s')\n",
                     estr2str(rel_path),
                     estr2str(hp->reldir)));

            /* handle special case of paths that reference the directory
             * they are relative to */
            if('\0' == estr2str(rel_path)[0]) {
               set_estr(rel_path,".");
               set_estr(dest_fname,".");
            }

            /* debug */
            D(fprintf(stderr, DHL "  -> real path `%s'\n", uri));

            /* convert (filesystem depending) path to uri */
            conv_path2uri(dest_uri, estr2str(rel_path));
         } else if (kind == URI_rel) {
            /*
             * parse relative uri
             */
            EXPSTR *uri_path = init_estr(32);
            EXPSTR *docdir = init_estr(32);
            STRPTR optimized_name = NULL;

            /* debug */
            D(fprintf(stderr, DHL "exists `%s' [rel]\n", uri));

            /* create local filename */
            conv_uri2path(uri_path, uri, hp->weenix);
            estrcat(dest_fname, hp->destdir);
            estrcat(dest_fname, hp->reldir);
            estrcat(dest_fname, uri_path);

            /* dir correction, so path collapsing will always work */
            if(FE_DIR == fgetentrytype(estr2str(dest_fname))) {
               /* make sure there's a training slash */
               if(0 != strcmp(PATH_SEPARATOR,
                               estr2str(dest_fname) + strlen(estr2str(dest_fname)) -
                                 strlen(PATH_SEPARATOR)))
                  app_estr(dest_fname, PATH_SEPARATOR);
            }
            /* make document directory name */
            estrcpy(docdir, hp->destdir);
            estrcat(docdir, hp->reldir);

            /* optimize complete target URI */
            optimize_fname(&optimized_name, estr2str(dest_fname));
            if(optimized_name) {
               /* dest_uri is relative to target dir */
               get_relfname(dest_uri, optimized_name, estr2str(docdir));
               if(0 == estrlen(dest_uri))
                  set_estr(dest_uri, ".");
               ufree(optimized_name);
            } else {
               /* something wrong with the optimization - just jopy URI */
               set_estr(dest_uri,uri);
            }

            del_estr(docdir);
            del_estr(uri_path);
         } else {
            set_estr(dest_uri, uri);
            set_estr(dest_fname, "");
         }
      }

      /* If there is a filename, optimize it */
      if (estrlen(dest_fname) > 0) {
         STRPTR optimized_name = NULL;
         optimize_fname(&optimized_name, estr2str(dest_fname));
         set_estr(dest_fname, optimized_name);
         if(optimized_name)
            ufree(optimized_name);
         /* dir correction for the final URI */
         if(FE_DIR == fgetentrytype(estr2str(dest_fname))) {
            /* make sure there's a training slash */
            if('/' != estr2str(dest_uri)[strlen(estr2str(dest_uri)) - 1])
               app_estr(dest_uri,"/");
         }
      }

      /* debug */
      D(
            {
            fprintf(stderr, DHL "  -> real file `%s'\n",
               estr2str(dest_fname));
            fprintf(stderr, DHL "  -> real uri  `%s'\n",
               estr2str(dest_uri));
            }
       );
   } 
   /* free resources */
   del_estr(turi);
   del_estr(rel_path);
}

void conv_hscuri2file(HSCPRC * hp, EXPSTR * dest_fname, STRPTR uri)
{
   EXPSTR *dest_uri = init_estr(64);
   conv_hscuri2fileNuri(hp, dest_uri, dest_fname, uri);
   del_estr(dest_uri);
}

/*
 * parse_uri
 *
 * check URI-string for syntatic correctness;
 * If the URI refers to an local file, convert its absolute
 * path to a relative path and check its existence.
 *
 * uri = "rsrc_type://host.domain:port/pathname#id"
 */
void parse_uri(HSCPRC * hp, EXPSTR *dest_uri, STRPTR uri)
{
   /* sample URI could be:
    * http://www.host.at:80/file.html#name
    */
   STRPTR protocol = NULL;     /* "http://" */
   STRPTR host = NULL;         /* "www.sepp.at" */
   STRPTR port = NULL;         /* ":80" */
   STRPTR path = NULL;         /* "file.html" */
   STRPTR name = NULL;         /* stuff after "#" */
   STRPTR cgiargs = NULL;      /* stuff after "?"  */
   /* STRPTR p;
    EXPSTR *tmp_uri = init_estr(32); */

   clr_estr(dest_uri);

   if (uri) {
      /* check for valid uri */
      URIKIND kind = uri_kind(uri);
      if(!hscregsearch_pc(uri,NULL,hp->re_uri,NULL)) {
         hsc_message(hp,MSG_INVALID_URI,"URI %q is syntactically invalid",uri);
      }
      if ((kind == URI_ext) ||
            ((kind == URI_relserv) && !(estrlen(hp->server_dir)))) {
         if (kind == URI_ext) {
            /*
             * check global uri
             */
            if (!protocol) protocol = "";
            if (!host) host = "";
            if (!port) port = "";

            /*
            p = uri;
            while(*p && (':' != *p)) ++p;
            if(':' == *++p)
              if('/' == *++p)
                if('/' == *++p) {
                   while(*p && (':' != *p) && ('/' != *p)) ++p;
                }
              */

            /*
             * TODO: parse global uris
             */
#if defined(UNIX) || defined(AMIGA)
            if(hp->checkext) check_ext_uri(hp,uri);
#endif
         } else if (kind == URI_relserv) {
            hsc_message(hp, MSG_SERVER_URI, "server relative URI to %q", uri);
         } else {
            panic("what kind of uri now?");
         }
         set_estr(dest_uri, uri);
      } else {
         /*
          * check local uri
          */

         /* destination file name that's existence is checked if
          * chkuri is enabled */
         EXPSTR *dest_fname = init_estr(32);
         STRPTR noabsuri = uri;

         /* evaluate kind of URI */
         if (kind == URI_abs)
            noabsuri += strlen(ABSURI_ID);     /* skip ":" */

         /* extract path and #name or ?args */
         if (noabsuri[0] == '#') {
            path = NULL;
            name = noabsuri + 1;    /* skip '#' for "#id" */
         } else {
            path = uri;
            name = strchr(uri, '#');
            if (name) {
               /* blank out '#' */
               name[0] = '\0';
               name++;
            } else {
#if 1
               cgiargs = NULL;
#else
               cgiargs = strchr(uri, '?');     /* TODO: conformant? */
#endif
               if (cgiargs) {
                  /* blank out '?' */
                  cgiargs[0] = '\0';
                  cgiargs += 1;
               }
            }
         }

         if (path) {
            FILE *exist = NULL;

            /*
             * check existence of local uri
             */
            conv_hscuri2fileNuri(hp, dest_uri, dest_fname, path);

            if (hp->chkuri && !(hsc_get_msg_ignore(hp, MSG_NO_URIPATH)))
            {
               exist = fopen(estr2str(dest_fname), "r");
               if (!exist) {
                  /* URIs ":" and "" won't open correctly */
                  if(('\0' != uri[0]) && (0 != strcmp(":",uri)))
                     hsc_msg_nouri(hp, estr2str(dest_fname), uri, NULL);
               } else {
                  fclose(exist);

                  /* check id */
                  if (hp->chkid && name) {
                     STRPTR doc_fname = estr2str(dest_fname);

                     if (!fnamecmp(hp->project->document->docname, doc_fname)) {
                        /* filename references current document */
                        add_local_idref(hp, name);
                     } else {
                        /* filename reference other document;
                         * lookup project-data */
                        switch (check_document_id(hp->project,
                                 doc_fname, name))
                        {
                           case ERR_CDI_OK:
                              D(fprintf(stderr, DHL "  id ok\n"));
                              break;
                           case ERR_CDI_NoID:
                              hsc_msg_unknown_id(hp, NULL, name);
                              break;
                           case ERR_CDI_NoDocumentEntry:
                              hsc_message(hp, MSG_NO_DOCENTRY,
                                    "no entry for document %q "
                                    "in project data to check %i",
                                    estr2str(dest_fname),
                                    name);
                              break;
                           default:
                              panic("unknown returncode");
                              break;
                        }
                     }   /* if fnamecmp */
                  }       /* if hp->chkid */
               }           /* if exists */
            }               /* if hp->chkuri */
         } else {
            /* check existence ID referencing into current file */
            if (hp->chkid && name)
               add_local_idref(hp, name);
         }

         /* add #name or ?args part */
         if (name) {
            app_estrch(dest_uri, '#');
            app_estr(dest_uri, name);
         } else if (cgiargs) {
            app_estrch(dest_uri, '?');
            app_estr(dest_uri, cgiargs);
         }

         /* free resources */
         del_estr(dest_fname);
      }                       /* else (rsrc) */
   }                           /* if (uri) */
}

/* $Id$ */
/* vi: set ts=4: */
