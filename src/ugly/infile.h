/*
 * ugly/infile.h
 *
 * ugly input file handling functions, header file
 *
 */

#ifndef UGLY_INFILE_H           /* avoid include twice */
#define UGLY_INFILE_H

/*
 * includes
 */
#include "utypes.h"
#include "expstr.h"
#include "dllist.h"

/* reasonable value for buffer-step-size */
#define IF_BUFFER_VALUE 128

/*
 * ugly input file structure (PRIVATE)
 */
typedef struct infile
{
    STRPTR filename;            /* file name */
    FILE *infile;               /* file opened if fopen() */
    EXPSTR *lnbuf;              /* buffer for inputline */
    EXPSTR *wordbuf;            /* word buffer */
    EXPSTR *wspcbuf;            /* word buffer (white spaces) */

    ULONG filepos;              /* file position */
    ULONG pos_y;                /* line number in file */
    ULONG pos_x;                /* pos. in current line */
    ULONG wpos_y;               /* line number in file of current word */
    ULONG wpos_x;               /* pos. in current line of current word */
    ULONG base_x;               /* base position */
    ULONG base_y;

    DLLIST *pos_list;           /* list of pending pos-request */
    ULONG pos_count;            /* number of entries in poslist */

      BOOL(*is_ws) (int ch);    /* ptr to func that checks if a char */
    /* is a white-space */
      BOOL(*is_nc) (int ch);    /* deto, but for "normal char" */

    BOOL eof_reached;           /* flag: TRUE, if end of file */
    BOOL out_of_mem;            /* flag: TRUE, if ran out of memory */
    BOOL skipped_ws;            /* flag: TRUE, if last infgetw */
    /*       skipped a white-space */
    BOOL closed;                /* flag: TRUE, if closed, but pending */
    /*       fpos-requested exist */
}
INFILE;

typedef struct infile_pos
{
    INFILE *inpf;
    STRPTR fname;               /* filename */
    ULONG x;                    /* column */
    ULONG y;                    /* line */
    ULONG fpos;                 /* file position */
}
INFILEPOS;

/*
 * global macros
 */
#define infclose( f ) ((f) ? (infclose1( f ), f=NULL) : (NULL))

/*
 * global vars
 */
extern STRPTR FNAME_STDIN;

/*
 *
 * extern function prototypes & global vars
 *
 */
#ifndef NOEXTERN_UGLY_INFILE_H

extern INFILE *infopen(CONSTRPTR name, size_t step_size);
extern INFILE *infopen_str(CONSTRPTR fname, CONSTRPTR s, size_t step_size);
extern int infclose1(INFILE * inpf);    /* PRIVATE; use infclose() */

extern int infgetc(INFILE * inpf);
extern STRPTR infgetw(INFILE * inpf);
extern STRPTR infgetall(INFILE * inpf);

extern STRPTR infgetcws(INFILE * inpf);
extern STRPTR infgetcw(INFILE * inpf);

extern int inungetc(int ch, INFILE * inpf);
extern size_t inungets(STRPTR s, INFILE * inpf);
extern size_t inungetcwws(INFILE * inpf);
extern size_t inungetcw(INFILE * inpf);

extern ULONG infget_x(INFILE * inpf);
extern ULONG infget_y(INFILE * inpf);
extern ULONG infget_wx(INFILE * inpf);
extern ULONG infget_wy(INFILE * inpf);
extern STRPTR infget_fname(INFILE * inpf);
extern BOOL infget_skws(INFILE * inpf);

extern BOOL inf_isws(char ch, INFILE * inpf);
extern size_t infskip_ws(INFILE * inpf);

extern int infeof(INFILE * inpf);
extern int infgotoeol(INFILE * inpf);

extern VOID del_infilepos(INFILEPOS * pos);
extern INFILEPOS *new_infilepos(INFILE * inpfile);
extern INFILEPOS *new_winfilepos(INFILE * inpfile);
extern STRPTR ifp_get_fname(INFILEPOS * pos);
extern ULONG ifp_get_x(INFILEPOS * pos);
extern ULONG ifp_get_y(INFILEPOS * pos);
extern BOOL set_infilepos(INFILE * inpf, INFILEPOS * pos);
extern BOOL set_infile_base(INFILE * inpf, INFILEPOS * pos);

#if 0
extern void inflog_enable(INFILE * inpf);
extern void inflog_disable(INFILE * inpf);
extern BOOL inflog_clear(INFILE * inpf);
extern BOOL inflog_app(INFILE * inpf, STRPTR s);
extern STRPTR infget_log(INFILE * inpf);
#endif

#endif /* NOEXTERN_UGLY_INFILE_H */

#endif /* UGLY_INFILE_H */

