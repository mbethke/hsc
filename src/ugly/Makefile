##
## makefile for ugly functions
##
## (W) by Tommy-Saftwörx
##
## $VER: makefile 1.1.4 (30.9.95)
##

#
# description of symbols:
#
# - sys		defines operating system
#		(supported: AMIGA,UNIX)
# - dest	destination dir for object files
#		(useful to compile to ram-disk)
# - ign		ignore warning for unreferenced vars
# - norm	compiler flags for developer version
# - optim	compiler flags for official version
# - comp	compiler call to create an object file (*.o)
# - link	compiler call to link a file

 
#**
#** sas/c AMIGA
#**
#sys  = DEFINE=AMIGA
#dest = t:
#ign  = IGNORE=93
#opts = STRICT ANSI STRINGMERGE SMALLCODE SMALLDATA NOICONS MEMORYSIZE=HUGE
#norm = $(sys) $(opts) DEFINE=UMEM_TRACKING DEBUG=LINE
#optim= $(sys) $(opts) NODBG STRIPDEBUG SMALLCODE OPT OPTSIZE IGN=304
#link = sc LINK $(ign)
#comp = sc


#**
#** gcc AMIGA
#**
#dest = 
#sys  = -DAMIGA -DGCC

#**
#** gcc UNIX
#**
dest =
sys  = -DUNIX

#**
#** gcc (any system)
#**
norm = $(sys) -DUMEM_TRACKING -ansi -g
optim= $(sys) -ansi -O
comp = cc -c
link = cc -o $@
ign  =

#**
#** compiler mode 
#**   norm...create developer version (mem tracking, debugging code)
#**   optim..official release (optimised)
#**
#mode = $(optim)
mode = $(norm)

#
# test files
#
all: test_es test_fn test_args test_file test_mem test_out test_str



##
## project
##

#ugly.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
#        $(comp)  ugly.c

#ugly_dbg.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
#        $(comp)  ugly.c -DUMEM_TRACKING_DEF -o ugly_dbg.o

#header: #?.h
#        copy #?.h sc:include/ugly

##
## project files
##

## not include: types.h

args.o: args.h args.c args_hlp.c args_prp.c args_set.c string.h dllist.h
	$(comp)  args.c $(mode)

#bntree.o  : types.h bntree.h bntree.c
#        $(comp) $(sys) bntree.c $(mode)

dllist.o  : types.h dllist.h dllist.c
	$(comp) dllist.c $(mode)

expstr.o : types.h expstr.c
	$(comp) expstr.c $(mode)

fname.o   : types.h string.h fname.h fname.c
	$(comp) fname.c $(mode)

memory.o   : types.h memory.c memory.h dllist.h
	$(comp) memory.c

prginfo.o : types.h prginfo.c
	$(comp) prginfo.c $(mode)

string.o  : types.h string.h string.c
	$(comp) string.c $(mode)

#umx.o : types.h umx.h umx.c
#	$(comp) umx.c $(mode)

infile.o : types.h string.h memory.h infile.c
	$(comp) infile.c $(mode)

outfile.o : types.h string.h memory.h outfile.c
	$(comp) outfile.c $(mode)

##
## test files
##

test_fn : test_fn.c fname.o memory.o
	$(link) test_fn.c fname.o string.o memory.o $(mode)

test_args : test_args.c args.o dllist.o prginfo.o memory.o
	$(link) test_args.c args.o string.o dllist.o memory.o prginfo.o $(mode)

test_es : test_es.c expstr.o memory.o string.o
	$(link) test_es.c expstr.o string.o memory.o $(mode)

test_file : test_file.c infile.o dllist.o expstr.o memory.o
	$(link) test_file.c dllist.o expstr.o infile.o string.o memory.o $(mode)

test_list : test_list.c dllist.o string.o memory.o
	$(link) test_list.c dllist.o string.o memory.o $(mode)

test_mem : test_mem.c memory.o dllist.o string.o
	$(link) test_mem.c dllist.o memory.o string.o $(mode)

test_out : test_out.c outfile.o dllist.o expstr.o memory.o
	$(link) test_out.c dllist.o expstr.o outfile.o string.o memory.o $(mode)

test_str : test_str.c memory.o string.o
	$(link) test_str.c memory.o string.o $(mode)

# EOF
