/*
 *
 * test_fn.c
 *
 * test filename manipulaton functions
 *
 * (W) by Tommy-Saftwörx in 1994,95,96
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utypes.h"
#include "ustring.h"
#include "expstr.h"
#include "fname.h"

#define NUM_NAME 6

char newp[MAX_FPATH];           /* new path (result of get_relfname() ) */

EXPSTR *dest = NULL;

void test_link_fname(STRPTR dir, STRPTR fn)
{
    BOOL ok = link_fname(dest, dir, fn);

    if (!dir)
        dir = "NULL";
    if (!fn)
        fn = "NULL";
    printf("file \"%s\" in \"%s\" -> [%d] \"%s\"\n",
           fn, dir, ok, estr2str(dest));

}

void test_link_envfname(STRPTR env, STRPTR dir, STRPTR fn)
{
    BOOL ok = link_envfname(dest, env, dir, fn);

    if (!env)
        env = "NULL";
    if (!dir)
        dir = "NULL";
    if (!fn)
        fn = "NULL";
    printf("file `%s', `%s' and `%s' -> [%d] `%s'\n",
           env, dir, fn, ok, estr2str(dest));

}

void test_getfne(void)
{
    STRPTR name[NUM_NAME] =
    {
        "c:/hugo/sucks.txt",
        "//silly/file.tar.gz",
        "test:dir/no_ext",
        "test:no_ext",
        "no_ext",
        "test:emptyext."
    };
    int i;
    BOOL ok;

    for (i = 0; i < NUM_NAME; i++)
    {
        ok = get_fext(dest, name[i]);
        printf("get_fext (`%-25s') -> [%d] `%s'\n",
               name[i], ok, estr2str(dest));
        ok = get_fname(dest, name[i]);
        printf("get_fname(`%-25s') -> [%d] `%s'\n",
               name[i], ok, estr2str(dest));

        printf("\n");

    }

}

void test_tmpnamstr(STRPTR prefix)
{
    int i=0;

    for (i=0; i<5; i++) {
        STRPTR s = tmpnamstr(prefix);
        printf("tmpnamstr: `%s'\n", s);
    }
}
void test_reldir(STRPTR absn, STRPTR curp)
{
    BOOL ok = get_relfname(dest, absn, curp);
    printf("`%s' and `%s' -> [%d] `%s\'n",
           absn, curp, ok, estr2str(dest));

}

void test_fsdir(STRPTR path)
{
    BOOL ok = get_fsdir(dest, path);

    printf("get_fsdir: `%s' -> [%d]`%s'\n",
           path, ok, estr2str(dest));

}

void test_fpath(STRPTR path)
{
    BOOL ok = get_fpath(dest, path);

    printf("get_fpath: `%s' -> [%d]`%s'\n",
           path, ok, estr2str(dest));

}

void test_setfext(STRPTR fn, STRPTR ext)
{
    BOOL ok;

    set_estr(dest, fn);
    ok = set_fext(dest, ext);

    if (!fn)
        fn = "NULL";
    if (!ext)
        ext = "NULL";

    printf("set_fext: `%s' with `%s' -> [%d] `%s'\n",
           fn, ext, ok, estr2str(dest));
}

int main(void)
{
#if 0
    LONG i;
    STRPTR nam;
#endif

    /* init global destination */
    dest = init_estr(20);

    printf("Testing ugly fname functions:\n\n");

#if 0
    printf("temp names (%d):", L_tmpnam);
    for (i = 0; i < 10; i++)
    {

        nam = tmpnamstr();
        if (nam)
        {
            printf(" `%s'", nam);
            ufreestr(nam);
        }
        else
            printf(" <NULL>");

    }
    printf("\n");
#endif

#if 0
    /*
     * test get_fext & get_fname
     */
    test_getfne();
#endif

#if 0
    /* test tmpnamstr */
    test_tmpnamstr(NULL);
    test_tmpnamstr("testfn");
    test_tmpnamstr("a_very_long_name_that_should_be_truncated_somehow");

#endif

#if 0
    /*
     * test link_fname
     */
    test_link_fname("c:tools", "sucktool.exe");
    test_link_fname("c:", "sucktool.exe");
    test_link_fname("", "sucktool.exe");
    test_link_fname(NULL, "sucktool.exe");
    test_link_fname("tools", "");
    test_link_fname("tools/", "");
    test_link_fname(NULL, NULL);
#endif

#if 1
    /*
     * test link_fname
     */
    system("setenv TEST_DEV  sepp:");
    system("setenv TEST_DIR  sepp:hugo/");
    system("setenv TEST_DIR2 sepp:hugo");
    test_link_envfname("TEST_DEV", "confix", "sucktool.exe");
    test_link_envfname("TEST_DEV", NULL, "sucktool.exe");
    test_link_envfname("TEST_DEV", "confix", NULL);
    test_link_envfname("TEST_DEV", NULL, NULL);
    test_link_envfname("TEST_DIR", NULL, "sucktool.exe");
    test_link_envfname("TEST_DIR2", NULL, "sucktool.exe");
    test_link_envfname(NULL, NULL, NULL);

#endif

#if 0
    /*
     * test set_fext: set a new file extension
     */
    test_setfext("hugo.tmp", "suck");
    test_setfext("hugo", "suck");
    test_setfext("hugo.oh.yeah.honey", "sucker");
    test_setfext("", "suck");
#endif

#if 0
    /*
     * test get_fpath: get only dirs from path
     */
    test_fpath("quite/long/path/for/hugo");
    test_fpath("hugo/heini/");
    test_fpath("hugo/heini");
    test_fpath("hugo/");
    test_fpath("hugo");
    test_fpath("");
#endif

#if 0
    /*
     * test get_fsdir: get first subdir from path
     */
    test_fsdir("hugo/heini/");
    test_fsdir("hugo/hein");
    test_fsdir("hugo/");
    test_fsdir("hugo");
    test_fsdir("");
#endif

#if 0
#if 0
    test_reldir("image/back.gif", "image/hugo/");
    test_reldir("image/back.gif", "");
#endif
#if 0
    test_reldir("image/back.gif", "people/");
    test_reldir("image/image.gif", "image/");
#endif
#if 1                           /* sucks */
    test_reldir("image/symbol/main.gif", "people/hugo/picture/");
#endif
#if 0
    test_reldir("image/symbol/main.gif", "image/hugo/picture/");
    test_reldir("image.gif", "people/");
    test_reldir("image.gif", "people/hugo/");
#endif
#if 0
    test_reldir("icons/icon2/image.gif", "hugo/");
    test_reldir("icons/image.gif", "hugo/");
#endif

#endif

    return (0);

}

