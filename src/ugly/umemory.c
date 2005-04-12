/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1993-1998  Thomas Aglassinger
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
 * ugly/umemory.c
 *
 * additional memory manegment functions;
 * implements some parts of Amiga-developer-tool
 * "MungWall" at source-level
 *
 * updated: 12-May-1997
 * created: 29-Mar-1994
 *
 */

/*
 *
 * Memory munging:
 *
 *   Except for ucalloc(), memory is pre-munged on allocation with
 *   $DEADFOOD. When this is used in an Enforcer report, the caller is
 *   allocating memory and doesn't initialize it before using it.
 *
 *   Memory is filled with $DEADBEEF before it is freed, encouraging
 *   programs reusing free'ed memory to crash.
 *
 * Memory watching:
 *
 *   Null sized malloc()'s are reported. The integrity of the walls will
 *   be tested according to the size specified when free'ed.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utypes.h"

#define NOEXTERN_UGLY_UMEMORY_H
#include "umemory.h"

/*
 * size of wall build around every memory block
 */
#define UMEM_WALLSIZE 16

/*
 * blocksize memory allocations are rounded up by OS
 * (this one's only needed to compute the amount of
 * slack-memory and won't cause any problems if wrong)
 */
#if defined(AMIGA)
#define UMEM_BLOCKSIZE 8	/* AmigaOS */
#else
#define UMEM_BLOCKSIZE 8
#endif

#ifndef modfit
#define modfit(x,by) ((by)*(((x)+(by-1))/(by)))
#endif

static UGLYMEM *first = NULL;

static UBYTE deadbeef[4] =
{0xDE, 0xAD, 0xBE, 0xEF};	/* used to fill mem after free() */
static UBYTE deadfood[4] =
{0xDE, 0xAD, 0xF0, 0x0D};	/* used to fill mem after malloc() */

static UBYTE ugly_fillchar = 0x81;

static ULONG ugly_umalloc_count = 0;	/* num. of calls to umalloc()/ucalloc() */
static ULONG ugly_ufree_count = 0;	/* num. of calls to ufree() */
static ULONG ugly_umalloc_count_fail = 0;	/* num. of failed calls to umalloc() */
static ULONG ugly_ufree_count_fail = 0;		/* num. of failed calls to ufree() */
static ULONG ugly_maxmem_usage = 0;	/* maximum memmory used */
static ULONG ugly_curmem_usage = 0;	/* current memory used */
static ULONG ugly_real_maxmem_usage = 0;	/* maximum memmory used */
static ULONG ugly_real_curmem_usage = 0;	/* current memory used */
static ULONG ugly_maxnod_usage = 0;	/* maximum num. of memmory nodes used */
static ULONG ugly_curnod_usage = 0;	/* current num. of memory nodes used */

/* forward reference */
void *ugly_malloc_notracking(size_t size);
static BOOL ugly_walldamaged(UGLYMEM * umem);

/* function pointer for nomem-handler */
BOOL(*ugly_nomem_handler) (size_t size) = NULL;

/*
 * send panic message to stderr
 */
void display_panic_message(char *msg, char *file, size_t line)
{
    fprintf(stderr, "\n\n");
    fprintf(stderr, "  I won't be a monkey in anyone's zoo\n");
    fprintf(stderr, "  I won't get fazed whatever you do\n");
    fprintf(stderr, "                   (Ride, \"Not Fazed\")\n\n");
    fprintf(stderr, "** internal error: \"%s\" (%lu): %s\n", \
	    file, (unsigned long) line, msg);
    exit(255);
}

/*
 * find_umem
 */
static UGLYMEM *find_umem(void *mem)
{
    UGLYMEM *nxtum = first;
    UGLYMEM *found = NULL;

    while (nxtum && (!found))
    {
	if (nxtum->ptr == mem)
	{
	    found = nxtum;
	}
	nxtum = nxtum->next;
    }

#if DEBUG_UGLY_MEMORY==2
    if (!found)
    {
	fprintf(stderr, "*memory* FIND_UMEM: couln't find %p\n", mem);
    }
#endif

    return (found);
}

/*
 * find_prev
 */
static UGLYMEM *find_prev(UGLYMEM * umem)
{
    UGLYMEM *prev = first;
    UGLYMEM *pprev = NULL;
    BOOL found = FALSE;

    while (prev && (!found))
    {
	found = (prev == umem);
	if (!found)
	{
	    pprev = prev;
	    prev = prev->next;
	}
    }

    return (pprev);
}

/*
 * fill_mem4, fill_mem
 *
 * fill memory with value specified
 */
static void fill_mem4(void *mem, size_t size, UBYTE value[4])
{
    size_t i;

    for (i = 0; i < size; i++)
    {
	(((UBYTE *) mem)[i]) = value[i % 4];
    }
}

static void fill_mem(void *mem, size_t size, UBYTE value)
{
    size_t i;

    for (i = 0; i < size; i++)
    {
	(((UBYTE *) mem)[i]) = value;
    }
}

/*
 * del_uglymem
 *
 * free an uglymem-entry
 */
static void del_uglymem(UGLYMEM * umem)
{
    UGLYMEM *prev = find_prev(umem);

    /* unlink from list */
    if (prev)
       prev->next = umem->next;
    else
       first = umem->next;

    /* check for damaged wall */
    if (!ugly_walldamaged(umem))
    {
	/* wall ok:
	 *
	 * fill memory with $DEADBEEF,
	 * free memory */
	fill_mem4(umem->lower, umem->size + 2 * UMEM_WALLSIZE, deadbeef);
	free(umem->lower);
    }

    /* free memory structure */
    umem->lower = NULL;
    umem->upper = NULL;
    umem->size = 0;
    umem->file = NULL;
    umem->line = 0;
    free(umem);
}

/*
 * new uglymem
 *
 * alloc & init a new entry of ugly mem
 */
static UGLYMEM *new_uglymem(size_t memsize, STRPTR memfile, ULONG memline)
{
    UGLYMEM *newmem = (UGLYMEM *) malloc(sizeof(UGLYMEM));

    if (newmem)
    {
	newmem->lower = (UBYTE *) ugly_malloc_notracking(memsize
						       + 2 * UMEM_WALLSIZE);
	if (newmem->lower)
	{
	    /* compute location of main mem/upper wall */
	    newmem->ptr = (void *) (newmem->lower + UMEM_WALLSIZE);
	    newmem->upper = (newmem->lower + UMEM_WALLSIZE + memsize);

	    /* link to list */
	    newmem->next = first;
	    first = newmem;

	    /* init data */
	    newmem->size = memsize;
	    newmem->file = memfile;
	    newmem->line = memline;
	    newmem->fillchar = ugly_fillchar;

	    /* fill new mem area with $DEADF00D */
	    fill_mem4(newmem->ptr, memsize, deadfood);

	    /* fill lower/upper wall */
	    fill_mem(newmem->lower, UMEM_WALLSIZE, ugly_fillchar);
	    fill_mem(newmem->upper, UMEM_WALLSIZE, ugly_fillchar);

	    /* update fillchar */
	    if (ugly_fillchar == 0xff)
	    {
		ugly_fillchar = 0x81;
	    }
	    else
	    {
		ugly_fillchar++;
	    }
	}
	else
	    free(newmem);
    }
    return (newmem);
}

static void uglymem_message(STRPTR msg)
{
    fprintf(stderr, "%s\n", msg);
}

static void ugly_memdump(void *ptr, size_t size)
{
    STRPTR data = (STRPTR) ptr;

    /* limit size */
    if (size > 16)
    {
	size = 16;
    }

    fprintf(stderr, "  %p:", ptr);
    if (data)
    {
	size_t i;

	/* hex dump */
	for (i = 0; i < size; i++)
	{
	    if (!(i % 4))
	    {
		fprintf(stderr, " ");
	    }
	    fprintf(stderr, "%02x", data[i]);

	}

	/* fill with blanks */
	while (i < 16)
	{
	    if (!(i % 4))
	    {
		fprintf(stderr, " ");
	    }
	    fprintf(stderr, "  ");
	    i++;
	}

	fprintf(stderr, "  \"");
	/* ascii dump */
	for (i = 0; i < size; i++)
	{
	    if (data[i] < ' ')
	    {
		fprintf(stderr, ".");
	    }
	    else
	    {
		fprintf(stderr, "%c", data[i]);
	    }
	}
	fprintf(stderr, "\"\n");

    }
    else
	fprintf(stderr, "NULL\n");

}

static void uglymem_meminfo(void *ptr, STRPTR file, ULONG line)
{
    fprintf(stderr, "  %p: from \"%s\" (%lu)\n", ptr, file, line);
}

static void umem_info(UGLYMEM * umem)
{
    fprintf(stderr, "  %p: %lu (0x%lx) bytes from \"%s\" (%lu)\n",
	    umem->ptr, (ULONG) umem->size, (ULONG) umem->size,
	    umem->file, umem->line);
}

/*
 *-------------------------------------
 * wall check functions
 *-------------------------------------
 */

/*
 * str_ubyte
 *
 * convert a UBYTE-value to hex/dez/char and return
 * results as a displayable string
 */
static STRPTR str_ubyte(UBYTE val)
{
    static STRARR strbuf[30];
    UBYTE ch = val;

    if (ch < 32)
	ch = '.';

    sprintf(strbuf, "(0x%02x/#%d/`%c')", val, val, ch);

    return (strbuf);
}

/*
 * ugly_walldamaged
 *
 * check memory walls a specifc entry in memory-list,
 * output message if wall is damaged
 */
static BOOL ugly_walldamaged(UGLYMEM * umem)
{
    size_t i = 0;
    BOOL damaged = FALSE;

    while (!damaged && (i < UMEM_WALLSIZE))
    {
	BOOL lower_damaged = (umem->lower[i] != umem->fillchar);
	BOOL upper_damaged = (umem->upper[i] != umem->fillchar);

	damaged = lower_damaged || upper_damaged;
	if (damaged)
	{
	    STRPTR wall;
	    UBYTE value;

	    if (lower_damaged)
	    {
		wall = "LOWER";
		value = umem->lower[i];
	    }
	    else
	    {
		wall = "UPPER";
		value = umem->upper[i];
	    }

	    fprintf(stderr, "*** MEMORY WALL DAMAGED!!!\n");
	    fprintf(stderr, "*** %s wall, byte#%lu is %s instead of 0x%02x\n",
		    wall, (ULONG) i, str_ubyte(value), umem->fillchar);
	    umem_info(umem);
	    ugly_memdump(umem->ptr, umem->size);
	    fprintf(stderr, "  * lower wall:\n");
	    ugly_memdump(umem->lower, UMEM_WALLSIZE);
	    fprintf(stderr, "  * upper wall:\n");
	    ugly_memdump(umem->upper, UMEM_WALLSIZE);

	}
	else
	{
	    i++;
	}
    }

    return (damaged);
}

/*
 * uglymem_wallcheck
 *
 * display a header message and check all walls for consistency
 */
void uglymem_wallcheck(STRPTR msg, STRPTR file, ULONG line)
{
    UGLYMEM *umem = first;

    if (umem)
    {
	/* report header */
	fprintf(stderr, "MEMORY WALL-CHECK (%s)", msg);
	if (file)
	    fprintf(stderr, " from `%s' (%lu)", file, line);
	fprintf(stderr, "\n");

	/* check all elements */
	while (umem)
	{
	    if (umem->ptr)
	    {
		ugly_walldamaged(umem);
		umem = umem->next;
	    }
	    else
	    {
		umem = NULL;
		fprintf(stderr, "\n** PANIC: memory list trashed\n");
	    }
	}
    }
}

/*
 *-------------------------------------
 * memory statistics functions
 *-------------------------------------
 */

/*
 * ugly_mem_report
 *
 * displaly all memory nodes currently allocated
 */
void uglymem_report(STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time)
{
    UGLYMEM *umem = first;

    if (umem)
    {
	/* report header */
	fprintf(stderr, "MEMORY REPORT (%s)\n", msg);
	if (file)
	{
	    fprintf(stderr, "(\"%s\" (%lu), at %s, %s)\n",
		    file, line, date, time);
	}

	/* print all elements */
	while (umem)
	{
	    if (umem->ptr)
	    {
		umem_info(umem);
		ugly_memdump(umem->ptr, umem->size);
		umem = umem->next;
	    }
	    else
	    {
		umem = NULL;
		fprintf(stderr, "##\n## panic: memory list trashed\n##\n");
	    }
	}
    }
}

/*
 * ugly_mem_stats
 *
 * display memory statistics (nodes & size allocated)
 */
void uglymem_stats(STRPTR msg, STRPTR file, ULONG line, STRPTR date, STRPTR time)
{
    /* statistics header */
    fprintf(stderr, "MEMORY STATISTICS (%s)\n", msg);
    if (file)
    {
	fprintf(stderr, "(\"%s\" (%lu), at %s, %s)\n",
		file, line, date, time);
    }

    /* memory statistics */
    fprintf(stderr, "  bytes used: %lu max: %lu/%lu  ",
	    ugly_curmem_usage, ugly_real_maxmem_usage,
	    ugly_maxmem_usage);
    if (ugly_maxmem_usage)
    {
	fprintf(stderr, "slack: %lu%%\n",
		(100 * (ugly_real_maxmem_usage - ugly_maxmem_usage))
		/ ugly_maxmem_usage);
    }
    else
    {
	fprintf(stderr, "no slack\n");
    }

    fprintf(stderr, "  nodes used: %lu (max: %lu)\n",
	    ugly_curnod_usage, ugly_maxnod_usage);
    fprintf(stderr, "  calls to: umalloc(%lu)   ufree(%lu)\n",
	    ugly_umalloc_count, ugly_ufree_count);
}

/*
 *-------------------------------------
 * atexit functions
 *-------------------------------------
 */

/*
 * atexit_uglymemory_real
 */
void atexit_uglymemory_real(void)
{
    ULONG mem_lost = ugly_curmem_usage;
    uglymem_report("at exit:  MEMORY LEAK detected!",
		   NULL, 0, NULL, NULL);
    uglymem_stats("[exit]", NULL, 0, NULL, NULL);

    /* release all lost mem */
    while (first)
    {
	del_uglymem(first);
    }

    if (mem_lost)
    {
	fprintf(stderr, "\n%lu bytes of memory lost!\n", mem_lost);
    }
}

/*
 * atexit_uglymemory_dummy
 */
void atexit_uglymemory_dummy(void) {
   /* do nufin */
}

/*
 *-------------------------------------
 * memory handling functions
 *-------------------------------------
 */

/*
 * ugly_malloc_notracking
 */
void *ugly_malloc_notracking(size_t size) {
   void *mem;
   BOOL retry;

   do
   {
      mem = malloc(size);
      if (!mem && ugly_nomem_handler) {
         /* call nomem-handler */
         retry = (*ugly_nomem_handler) (size);
         if (!retry)
            exit(EXIT_FAILURE);	/* abort programm */
      } else retry = FALSE;
   } while (retry);

   return (mem);
}

/*
 * ugly_malloc_tracking
 */
void *ugly_malloc_tracking(size_t size, STRPTR file, ULONG line) {
   void *mem = NULL;
   UGLYMEM *umem = NULL;

#if DEBUG_UGLY_MEMORY==2
   fprintf(stderr, "*memory* UMALLOC() from `%s' (%lu)\n", file, line);
#endif
   if (size) {
      /* update num. of calls to umalloc() */
      ugly_umalloc_count++;

      /* alloc new uglymem */
      umem = new_uglymem(size, file, line);
      if (umem) {
         mem = umem->ptr;

         /* update memory usage and num of nodes */
         ugly_curmem_usage += size;
         ugly_real_curmem_usage += modfit(size, UMEM_BLOCKSIZE);
         if (ugly_curmem_usage > ugly_maxmem_usage)
            ugly_maxmem_usage = ugly_curmem_usage;
         if (ugly_real_curmem_usage > ugly_real_maxmem_usage)
            ugly_real_maxmem_usage = ugly_real_curmem_usage;
         ugly_curnod_usage++;
         if (ugly_curnod_usage > ugly_maxnod_usage)
            ugly_maxnod_usage = ugly_curnod_usage;
      }
   } else {
      /* zero-alloc */

      /* update num. of failed calls to umalloc() */
      ugly_umalloc_count_fail++;

      uglymem_message("MALLOC: zero-sized allocation");
      uglymem_meminfo(NULL, file, line);
   }

   return (mem);
}

/*
 * ugly_free
 */
void ugly_free(void *ptr, STRPTR file, ULONG line) {
#if DEBUG_UGLY_MEMORY==2
   fprintf(stderr, "*memory* UFREE() from `%s' (%lu)\n", file, line);
#elif 0
   fputc('.', stderr);		/* still alive? */
   fflush(stderr);
#endif
   if (ptr) {
      UGLYMEM *umem = find_umem(ptr);

      if (umem) {
         /* update num. of calls to ufree() */
         ugly_ufree_count++;

         /* update memory usage */
         ugly_curmem_usage -= umem->size;
         ugly_real_curmem_usage -= modfit(umem->size, UMEM_BLOCKSIZE);

         /* remove node from mem-list */
         del_uglymem(umem);
         ugly_curnod_usage--;
      } else {
         /* ptr has never been allocated */

         /* update num. of calls to ufree() */
         ugly_ufree_count_fail++;

         /* -> error message */
         uglymem_message("*** FREE: memory never allocated "
               " or released twice");
         uglymem_meminfo(ptr, file, line);
      }
   }
}

/*
 * ugly_realloc
 *
 * replacement of realloc()
 */
void *ugly_realloc(void *ptr, size_t size, STRPTR file, ULONG line) {
   void *newptr = ugly_malloc_tracking(size, file, line);
   UGLYMEM *umem = find_umem(ptr);

   if (newptr && umem) {
      /* copy old data */
      memcpy(newptr, umem->ptr, umem->size);
      /* free old memory */
      ugly_free(ptr, file, line);
   }
   return (newptr);
}

/*
 * ugly_calloc
 *
 * replacement of calloc()
 */
void *ugly_calloc(size_t count, size_t size, STRPTR file, ULONG line) {
   /* alloc new mem */
   void *mem = ugly_malloc_tracking(count * size, file, line);

   /* fill mem with zero */
   if (mem) {
      memset(mem, 0, size * count);
   }
   return (mem);
}
