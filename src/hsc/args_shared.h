#include "hsc/global.h"

/* default values for some arguments */
#define DEFAULT_EXTENSION "html"
#define DEFAULT_MAXERR    "20"
#define DEFAULT_MAXMSG    "40"

extern HSCPRC *arg_hp;
/* contains defines for source-, destination- and global attributes */
extern EXPSTR *fileattr_str;

STRPTR arg_ignore_CB(STRPTR arg);
STRPTR arg_enable_CB(STRPTR arg);
STRPTR arg_incdir_CB(STRPTR arg);
STRPTR arg_mode_CB(CONSTRPTR arg);
STRPTR arg_status_CB(STRPTR arg);
VOID set_dest_attribs(HSCPRC * hp, STRPTR destpath, STRPTR reldestpath, STRPTR destname);
VOID set_source_attribs(HSCPRC * hp, STRPTR sourcepath, STRPTR sourcename);
VOID set_global_attribs(HSCPRC * hp);
VOID define_file_attribs(HSCPRC * hp);

