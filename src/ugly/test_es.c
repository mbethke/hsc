/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1997  Thomas Aglassinger
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
 *
 * test_es.c
 *
 * test expandable-string functions
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "umemory.h"
#include "ustring.h"
#include "expstr.h"

EXPSTR *es, *res;

/* print current estr */
void pe(STRPTR comment)
{
    printf("es:%s: (%2lu/%2lu/%2lu) \"%s\"\n", comment,
           (ULONG) es->es_len, (ULONG) es->es_size,
           (ULONG) es->es_step, estr2str(es));
}

void pr(STRPTR comment)
{
    printf("rs:%s: (%2lu/%2lu/%2lu) \"%s\"\n", comment,
           (ULONG) res->es_len, (ULONG) res->es_size,
           (ULONG) res->es_step, estr2str(res));
}

int main(void)
{
#if 0
    LONG i;
#endif

#if DEBUG_UGLY_MEMORY
    atexit(atexit_uglymemory);
#endif

#if 0
    for (i = 0; i < 20; i++)
        printf("modadj(%-2d/%-2d) = %d\n",
               i, EXPSTR_MEMSTEP, modadj(i, EXPSTR_MEMSTEP));
#endif

    es = init_estr(8);
    pe("init  ");
    res = init_estr(8);
    pr("init  ");
    umem_wallcheck("after init");

#if 0

    /* test reference to NULL string */
    app_estrch(NULL, 'x');
    app_estr(es, NULL);

#endif

#if 0
#if 1
    printf("** test set\n");
    set_estr(es, "dummy");
    pr("set   ");
    umem_wallcheck("after set");
#if 1
    set_estr(es, "hugo ist doof.");
    pe("set   ");

    set_estrn(es, "hugo", 4);
    pe("setn:4  ");

    set_estrn(es, "hugo", 1);
    pe("setn:1 ");
    set_estrn(es, "hugo", 0);
    pe("setn:0 ");
    set_estrn(es, "hugo", 5);
    pe("setn:5 ");
#endif
    umem_wallcheck("after set");

#endif

#if 1
    printf("** test append-string\n");
    set_estr(es, "hugo ist doof...!!");
    pe("set   ");
    app_estrch(es, ' ');
    pe("appch ");
    app_estrch(es, 's');
    pe("appch ");

#if 1
    app_estr(es, "epp auch.");
    pe("appstr");
    app_estr(es, ".");
    pe("appstr");
    app_estr(es, ".");
    pe("appstr");
    app_estr(es, ".");
    pe("appstr");
    app_estr(es, " that's it, as you can see");
    pe("appstr");
#endif
    umem_wallcheck("after append");
#endif

#if 0
    /* test cutting functions */
    estrcpy(res, es);
    pr("copy  ");
    get_mid_estr(es, res, 5, 3);
    pe("mid   ");               /* "ist" */
    get_right_estr(es, res, 5);
    pe("right ");               /* "auch." */
    get_left_estr(es, res, 4);
    pe("left  ");               /* "hugo" */

    /* test special cases for cutting funtions */
    printf("** test get-part\n");
    set_estr(res, "hugo");
    pr("res=hugo ");
    get_mid_estr(es, res, 4, 3);
    pe("mid(4,5) ");
    get_mid_estr(es, res, 3, 2);
    pe("mid(3,2) ");
    get_mid_estr(es, res, 0, 9);
    pe("mid(0,9) ");
    get_left_estr(es, res, 5);
    pe("lef(5)   ");
    get_left_estr(es, res, 4);
    pe("lef(4)   ");
    get_right_estr(es, res, 5);
    pe("rig(5)   ");
    get_right_estr(es, res, 4);
    pe("rig(4)   ");
#endif
#endif

/*    umem_wallcheck("before delete"); */

    printf("** remove strings\n");
    del_estr(es);
    del_estr(res);

    return (0);
}

