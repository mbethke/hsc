/*
 * hsc - HTML sucks completely
 *
 * Another stupid HTML-preprocessor
 *
 * Copyright (C) 1995,96  Thomas Aglassinger
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
 *-------------------------------------------------------------------
 *
 * Author : Thomas Aglassinger (Tommy Saftwörx)
 * Email  : agi@giga.or.at
 * Address: Lissagasse 12/II/9
 *          8020 Graz
 *          AUSTRIA
 *
 *-------------------------------------------------------------------
 *
 * hsc/hsc.c
 *
 * updated: 12-Apr-1996
 * created:  1-Jul-1995
 */

#include "hsc/global.h"

/*
 * ugly includes
 */
#include "ugly/args.h"
#include "ugly/prginfo.h"
#include "ugly/returncode.h"

/*
 * local includes
 */
#include "hsc/hsc_rev.h"
#include "hsc/args.h"
#include "hsc/callback.h"
#include "hsc/output.h"
#include "hsc/status.h"

#ifdef AMIGA
/* AmigaOS version string
 * (imported from "hsc_rev.h")
 */
static const STRPTR AmigaOS_version = VERSTAG;
#endif

static HSCPRC *hp = NULL;

/*
 * includes_ok
 *
 * process include-files passed via CLI
 */
BOOL 
include_ok (HSCPRC * hp)
{
  BOOL ok = TRUE;

  if (incfile)
    {
      DLNODE *nd = incfile->first;

      while (nd && ok)
        {
          STRPTR filename = (STRPTR) nd->data;
          ok = hsc_include_file (hp, filename, 0);
          nd = nd->next;
        }
    }

  return (ok);
}

/*
 * cleanup
 */
VOID 
cleanup (VOID)
{
#if DEBUG
  /* just because I'm curious how long cleanup lasts */
  /* NOTE: obviously, it last very long */
  fputs( "(cleanup)\r", stderr );
  fflush( stderr );
#endif

  del_hscprc (hp);
  cleanup_global ();
  cleanup_output ();
  cleanup_msgfile ();

#if DEBUG
  fputs( "         \r", stderr );
  fflush( stderr );
#endif

}


/*
 * main function
 *
 */
int 
main (int argc, char *argv[])
{
  /* set program information */
  set_prginfo ("hsc", "Tommy-Saftwörx", VERSION, REVISION, BETA,
               "HTML Sucks Completely",
               "Freeware, type `hsc LICENSE' for details.");

#if DEBUG

  /* display a memory tracking report */
  /* at end of execution */
  atexit (atexit_uglymemory);

#endif

  /* install nomem-handler */
  ugly_nomem_handler = hsc_nomem_handler;

  /* use cleanup() as additional exit func */
  atexit (cleanup);

  /*
   * main procedure
   */
  hp = new_hscprc ();                  /* alloc new hsc-process */
  if (hp)
    {
      if (init_global ()               /* init global vars */
          && args_ok (hp, argc, argv)  /* process user args */
        )
        {
          STRPTR inpfname = NULL;      /* input-filename */

          /* display programm-info if requested */
          if (disp_status_version)
            fprintf_prginfo (stderr);

          if (init_msgfile (hp, msgfilename)     /* open message file */
              && init_output (hp))               /* open output file */
            {
              /* init return code; later modified by message() */
              return_code = RC_OK;

              /* evaluate input-filename; use NULL for stdin */
              inpfname = estr2str (inpfilename);
              if (!inpfname[0])
                inpfname = NULL;

              /* init process & read preferences */
              if (init_callback (hp)             /* assign callbacks */
                  && hsc_init_hscprc (hp)        /* init hsc-process */
                  && hsc_read_project_file( hp ) /* read project info */
                  && user_defines_ok (hp)        /* process user defines */
                  && include_ok (hp)             /* read include files (macros) */
                  && hsc_include_file (hp, inpfname,
                                       IH_PARSE_END
                                       | IH_UPDATE_PRJ)    /* read main file */
                )
                {
                  if ( write_output (hp) )       /* write output file */
                      hsc_write_project_file( hp );
                }
            }
        }
    }

  return (return_code);
}
