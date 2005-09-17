/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2005 Matthias Bethke
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
 * hsc/args_gnu.c
 *
 * user argument handling, glibc style
 *
 * created: 12-Apr-2005
 */

#include "hsc/global.h"
#include "hsc/status.h"
#include "hsc/callback.h"
#include "hsc/args.h"
#include "hsc/args_shared.h"
#include "ugly/fname.h"
#include "ugly/prginfo.h"
#include "ugly/returncd.h"
#include "ugly/uargs.h"
#include "hscprj/license.h"
#include <unistd.h>
#include <getopt.h>

static STRPTR arg_inpfname = NULL;  /* temp vars for set_args() */
static STRPTR arg_outfname = NULL;
static STRPTR arg_extension = NULL;
static STRPTR arg_server_dir = NULL;
static BOOL arg_mode = FALSE;
static BOOL arg_compact = FALSE;
static BOOL arg_getsize = FALSE;
static BOOL arg_rplc_ent = FALSE;
static BOOL arg_rplc_quote = FALSE;
static BOOL arg_smart_ent = FALSE;
static BOOL arg_strip_cmt = FALSE;
static BOOL arg_strip_badws = FALSE;
static BOOL arg_strip_ext = FALSE;
static BOOL arg_license = FALSE;
static BOOL arg_help = FALSE;
static BOOL arg_debug = FALSE;
static BOOL arg_nonesterr = FALSE;
static BOOL arg_lctags = FALSE;
static BOOL arg_xhtml = FALSE;
static BOOL arg_nvcss = FALSE;
static BOOL arg_checkext = FALSE;
static STRPTR arg_iconbase = NULL;
static STRPTR arg_striptags = NULL;
static LONG arg_entitymode = EMODE_INVALID;
static LONG arg_quotemode = QMODE_DOUBLE;

static ARGFILE *argf = NULL;

/*
 * cleanup_hsc_args: free local resources
 */
void cleanup_hsc_args(void)
{
    del_argfile(argf);
    del_estr(fileattr_str);
    if (msg_browser != NULL)
        del_msg_browser(arg_hp);
}

#define ARG_NONE (0)
#define ARG_OBL (1)
#define ARG_OPT (2)
#define OPT_NONE (0)
#define OPT_TXT (1)
#define OPT_DEC (2)
#define OPT_FLT (3)
#define OPT_FNC (4)

struct hscoption {
   const char *lopt;
   const char *help;
   const char *arghelp;
   const BOOL (*argfunc)(char*, const char**);
   const char has_arg;
   const char arg_type; 
   const char sopt;
};


void print_help(const struct hscoption *o, int nopts)
{
   int i, j, helpx=0;
   for(i=0; i<nopts; ++i) {
      int thishelpx = 3;
      if(NULL != o[i].lopt)
         thishelpx += 2 + strlen(o[i].lopt);
      if(NULL != o[i].arghelp)
         thishelpx += 1 + strlen(o[i].arghelp) + 1;
      if(thishelpx > helpx)
         helpx = thishelpx;
   }
   for(i=0; i<nopts; ++i) {
      int xpos = 3;
      if('\0' != o[i].sopt)
         fprintf(stderr,"-%c ",o[i].sopt);
      else
         fputs("   ",stderr);
      if(NULL != o[i].lopt) {
         fprintf(stderr,"--%s",o[i].lopt);
         xpos += 2 + strlen(o[i].lopt);
         if(NULL != o[i].arghelp) {
            fprintf(stderr,"=%s",o[i].arghelp);
            xpos += 1 + strlen(o[i].arghelp);
         }
      }
      for(j=0; j<helpx-xpos; ++j)
         fputc(' ',stderr);
      fprintf(stderr," %s\n",o[i].help);
   }
}

static struct option *hscopts_to_getopt_long(const struct hscoption *o, int nopts)
{
   struct option *lop, *tlop;
   int i, n = 0;

   for(i=0; i<nopts; ++i)
      if(o[i].lopt)
         ++n;
   lop = tlop = umalloc((1+n) * sizeof(*lop));
   for(i=0; i<nopts; ++i) {
      if(o[i].lopt) {
         tlop->name = o[i].lopt;
         tlop->has_arg = o[i].has_arg;
         tlop->flag = NULL;
         tlop->val = o[i].sopt;
         ++tlop;
      }
   }
   tlop->name = NULL;
   tlop->flag = NULL;
   tlop->has_arg = tlop->val = 0;
   return lop;
}

static char *hscopts_to_getopt_short(const struct hscoption *o, int nopts)
{
   char *sop, *tsop;
   int i, n = 0;

   for(i=0; i<nopts; ++i)
      if('\0' != o[i].sopt)
         ++n;
   sop = tsop = umalloc(1 + (3*n+1) * sizeof(*sop));
   *tsop++ = '-';
   for(i=0; i<nopts; ++i) {
      if('\0' != o[i].sopt) {
         *tsop++ = o[i].sopt;
         switch(o[i].has_arg) {
            case ARG_OPT :
               *tsop++ = ':';
               /* fall through */
            case ARG_OBL :
               *tsop++ = ':';
            default : 
#if DEBUG
               fprintf(stderr,"WARNING: unknown value for has_arg in hscopts_to_getopt_short()\n");
#endif
               break;
         }
      }
   } 
   *tsop = '\0';
   return sop;
}

static const struct hscoption *find_long_option(HSCPRC *hp, const struct hscoption *o, int n, const char *s)
{
   const struct hscoption *co = NULL;
   int i;

   for(i=0; i<n; ++i)
      if(0 == strcmp(o[i].lopt,s))
         co = o+i;
   return co;
}

static const struct hscoption *find_short_option(HSCPRC *hp, const struct hscoption *o, int n, char c)
{
   const struct hscoption *co = NULL;
   int i;
   for(i=0; i<n; ++i)
      if(o[i].sopt == c)
         co = o+i;
   return co;
}

static BOOL verify_option(HSCPRC *hp, const struct hscoption *o, char *val)
{
   int i;
   char *end;
   const char *desc;
   BOOL ok=TRUE;

   if(!o) return FALSE;

   switch(o->arg_type) {
      case OPT_NONE:
      case OPT_TXT :
         /* nothing to verify */
         break;
      case OPT_DEC :
         desc = "decimal";
         if('\0' == *val) {
            ok = FALSE;
            break;
         }
         i = strtol(val,&end,10);
         if('\0' != *end)
            ok = FALSE;
         break;
      case OPT_FNC :
         ok = o->argfunc(val,&desc);
      case OPT_FLT :
         /* unused so far, so fall through */
      default :
         panic("Unknown option type, please notify author!\n");
   }
   if(!ok) {
      const char *s = o->lopt;
      char t[2]={0};
      if(!s) {
         t[0] = o->sopt;
         s = t;
      }
      fprintf(stderr,"Error: option `%s' requires a %s argument!\n", s, desc);
   }
   return ok;
}

void process_short_option(HSCPRC *hp, char c, char *val)
{
   const char *error;
   void *dest; 

   switch(c) {
      case 1   :
         /* default for non-options */
      case 'f' :  /* input file */
         app_dlnode(incfile,val);
         break;
      case 'o' :
         dest = &arg_outfname;
         break;
      case 'p' :
         dest = prjfilename;
         break;
      case 's' :
         dest = prefsfilename;
         break;
      case 'D' :
         /* TODO */
         break;
      case 'i' :
         /* TODO */
         break;
      case 'e' :
         /* TODO */
         break;
      case 'm' :
         if(error = arg_mode_CB(val)) {
            panic(error);
            /* TODO: handle this more intelligently */
         }
         break;
      case 'I' :
         arg_incdir_CB(val);
         break;
      case 'c' :
         break;
      case 'g' :
         break;
      case 'q' :
         break;
      case 'l' :
         break;
      case 'x' :
         break;
      case 'd' :
         break;
      case 'h' :
         break;

      default :
         panic("Unknwon short option `%c'");
         break;
   }
}

void process_long_option(HSCPRC *hp, const char *s, const char *val)
{
   static const struct BOOLOPT { const char *s; void (*v)(HSCPRC*,BOOL); } boolopts[] = {
      {"rplcent",&hsc_set_rplc_ent},
      {"rplcquote",&hsc_set_rplc_quote},
      {"stripbadws",&hsc_set_strip_badws},
      {"stripcomment",&hsc_set_strip_cmt},
      {"stripext",&hsc_set_strip_ext},
      {"nonesterr",&hsc_set_no_nested_errors},
      {"nocss",&hsc_set_novcss},
      {"ckxuri",&hsc_set_checkext}
   };
   /*
      "msgfile"
      "msgfmt"
      "browser"
      "maxerr"
      "maxmsg"
      "ext"
      "qstyle"
      "estyle"
      "striptags"
      "iconbase"
      "serverdir"
      "status"
      "license"
      */
}

/*
 * args_ok
 *
 * prepare args, check & parse user args, display error and
 * help message if neccessary
 *
 * result: TRUE if all args ok
 */
BOOL args_ok(HSCPRC * hp, int argc, char *argv[])
{
   static const struct hscoption opts[] = {
      {"from",        "Input file(s)",                                             "FILE",  NULL,ARG_OBL, OPT_TXT, 'f'},
      {"to",          "Output file",                                               "FILE",  NULL,ARG_OBL, OPT_TXT, 'o'},
      {"prj",         "Project file",                                              "FILE",  NULL,ARG_OBL, OPT_TXT, 'p'},
      {"syntax",      "Syntax definition file (default: hsc.prefs)",               "FILE",  NULL,ARG_OBL, OPT_TXT, 's'},
      {"msgfile",     "Message file for output (default: stderr)",                 "FILE",  NULL,ARG_OBL, OPT_TXT, 0},
      {"msgfmt",      "printf-style format for messages",                          "FORMAT",NULL,ARG_OBL, OPT_TXT, 0},
      {"browser",     "Message browser program to use",                            "PROG",  NULL,ARG_OBL, OPT_TXT, 0},
      {"maxerr",      "Maximum numer of errors (default: " DEFAULT_MAXERR ")",     "NUM",   NULL,ARG_OBL, OPT_DEC, 0},
      {"maxmsg",      "Maximum numer of messages (default: " DEFAULT_MAXMSG ")",   "NUM",   NULL,ARG_OBL, OPT_DEC, 0},
      {"ext",         "Output file extension (default: " DEFAULT_EXTENSION ")",    "WORD",  NULL,ARG_OBL, OPT_TXT, 0},
      {"define",      "Define global attribute",                                   "WORD",  NULL,ARG_OBL, OPT_TXT, 'D'},
      {"ignore",      "Ignore message number or class",                            "WORD",  NULL,ARG_OBL, OPT_TXT, 'i'},
      {"enable",      "Enable message number or class",                            "WORD",  NULL,ARG_OBL, OPT_TXT, 'e'},
      {"msgmode",     "Syntax checking mode (" MODE_ENUMSTR ")",                   "WORD",  NULL,ARG_OBL, OPT_TXT, 'm'},
      {"qstyle",      "Quote style  (" QMODE_ENUMSTR ")",                          "WORD",  NULL,ARG_OBL, OPT_TXT, 0},
      {"estyle",      "Entity style  (" EMODE_ENUMSTR ")",                         "WORD",  NULL,ARG_OBL, OPT_TXT, 0},
      {"incdir",      "Add include directory",                                     "DIR",   NULL,ARG_OBL, OPT_TXT, 'I'},
      {"compact",     "Strip superfluous whitespace",                              NULL,    NULL,ARG_NONE,OPT_NONE,'c'},
      {"getsize",     "Set width and height attributes for images",                NULL,    NULL,ARG_NONE,OPT_NONE,'g'},
      {"rplcent",     "Replace non-ASCII characters with entities (cf. --estyle)", NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"rplcquote",   "Replace double quotes with `&quot;'",                       NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"stripbadws",  "Strip bad whitespace",                                      NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"stripcomment","Strip SGML comments",                                       NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"stripext",    "Strip tags with external URIs",                             NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"striptags",   "Tags to be stripped",                                       "LIST",  NULL,ARG_OBL, ARG_TEXT, 0},
      {"iconbase",    "Base URI for icon entities",                                "URI",   NULL,ARG_OBL, ARG_TEXT, 0},
      {"serverdir",   "Base directory for server relative URIs",                   "DIR",   NULL,ARG_OBL, ARG_TEXT, 0},
      {"status",      "Status message verbosity (" STATUS_ENUM_STR ")",            "LIST",  NULL,ARG_OBL, ARG_TEXT, 0},
      {"quiet",       "Be quiet. Equivalent to --status=QUIET",                    NULL,    NULL,ARG_NONE,OPT_NONE,'q'}, 
      {"nonesterr",   "Don't show \"previous call\" tracebacks on error",          NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"lowercase",   "Force all tags and attributes to lowercase",                NULL,    NULL,ARG_NONE,OPT_NONE,'l'},
      {"xhtml",       "Use XHTML mode (implies -l --qstyle=DOUBLE)",               NULL,    NULL,ARG_NONE,OPT_NONE,'x'},
      {"nocss",       "Don't validate CSS in STYLE attributes",                    NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"ckxuri",      "Check external links",                                      NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"debug",       "Enable debugging output if enabled at compile time",        NULL,    NULL,ARG_NONE,OPT_NONE,'d'},
      {"license",     "Display the license",                                       NULL,    NULL,ARG_NONE,OPT_NONE,0},
      {"help",        "Show this help",                                            NULL,    NULL,ARG_NONE,OPT_NONE,'h'},
   };                                              
   int c;
   int nopts=sizeof(opts)/sizeof(opts[0]);
   struct option *lop = hscopts_to_getopt_long(opts,nopts);
   char *sop = hscopts_to_getopt_short(opts,nopts);
   EXPSTR *destdir = init_estr(32);       /* destination dir */
   EXPSTR *rel_destdir = init_estr(32);   /* relative destination dir */
   EXPSTR *kack_name = init_estr(0);      /* temp. str for outfilename */
   LONG maximum_number_of_errors = strtol(DEFAULT_MAXERR, (char **) NULL, 10);
   LONG maximum_number_of_messages = strtol(DEFAULT_MAXMSG, (char **) NULL, 10);

   arg_hp = hp;
   arg_mode_CB(DEFAULT_MODE_STR);

   while(1) {
      int opt_index;

      c = getopt_long(argc, argv, sop, lop, &opt_index);
      if(-1 == c) break;

      if(c) {
         if(!verify_option(hp, find_short_option(hp,opts,nopts,c), optarg))
            return FALSE;
         process_short_option(hp, c, optarg);
      } else {
         if(!verify_option(hp, find_long_option(hp,opts,nopts,lop[opt_index].name), optarg))
            return FALSE;
         process_long_option(hp, lop[opt_index].name, optarg);
         break;
      }
   }
   ufree(lop);
   ufree(sop);
   return TRUE;
}

