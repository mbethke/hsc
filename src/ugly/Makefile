##
## makefile for ugly functions
##
## (W) by Tommy-Saftwörx
##
## $VER: makefile 1.1.7 (20.4.1996)
##

#
# description of symbols:
#
# - sys		defines operating system
#		(supported: AMIGA,UNIX)
# - norm	compiler flags for developer version
# - optim	compiler flags for official version
# - comp	compiler call to create an object file (*.o)
# - link	compiler call to link a file

#**
#** gcc AMIGA
#**
#sys  = -DAMIGA -Damigados -noixemul

#**
#** gcc UNIX
#**
#sys  = -DUNIX

#**
#** gcc (any system)
#**
#debug= $(sys) -D_ANSI_SOURCE -DDEBUG_UGLY -ggdb
#norm = $(sys) -D_ANSI_SOURCE
#optim= $(sys) -D_ANSI_SOURCE -O -s
#comp = gcc -Wall -W -c
#link = gcc -Wall -W -o $@

#**
#** sas/c AMIGA
#**
sys  =
opts =  
debug= $(sys) $(opts) DEBUG=SF DEF=DEBUG_UGLY
norm = $(sys) $(opts) DEBUG=LINE
optim= $(sys) $(opts) IGN=304 IGN=93 OPTIMIZE NOSTKCHK 
link = sc LINK 
comp = sc


#**
#** compiler mode 
#**	debug..create developer version (mem tracking, debugging code)
#**	norm...create unoptimised version
#**	optim..create optimised version
#**
#mode = $(optim)
#mode = $(norm)
mode = $(debug)

#--------------------------------------

#
# test files
#
all : test_es test_fn test_args test_file test_list test_mem test_str

clean :
	delete (#?.o|#?.s|#?.p) quiet

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

fname.o   : types.h expstr.h string.h fname.h fname.c
	$(comp) fname.c $(mode)

memory.o   : types.h memory.c memory.h dllist.h
	$(comp) memory.c $(mode)

prginfo.o : types.h prginfo.c
	$(comp) prginfo.c $(mode)

string.o  : types.h string.h string.c
	$(comp) string.c $(mode)

#umx.o : types.h umx.h umx.c
#	$(comp) umx.c $(mode)

infile.o : types.h string.h memory.h expstr.h infile.h infile.c
	$(comp) infile.c $(mode)

#outfile.o : types.h string.h memory.h expstr.h outfile.h outfile.c
#	$(comp) outfile.c $(mode)

##
## test files
##

test_fn : test_fn.c fname.o memory.o expstr.o
	$(link) $(mode) test_fn.c expstr.o fname.o string.o memory.o

test_args : test_args.c args.o dllist.o prginfo.o memory.o string.o
	$(link) $(mode) test_args.c args.o string.o dllist.o memory.o prginfo.o

test_es : test_es.c expstr.o memory.o string.o
	$(link) $(mode) test_es.c expstr.o string.o memory.o

test_file : test_file.c infile.o dllist.o expstr.o memory.o
	$(link) $(mode) test_file.c dllist.o expstr.o infile.o string.o memory.o

test_list : test_list.c dllist.o string.o memory.o
	$(link) $(mode) test_list.c dllist.o string.o memory.o

test_mem : test_mem.c memory.o dllist.o string.o
	$(link) $(mode) test_mem.c dllist.o memory.o string.o

#test_out : test_out.c outfile.o dllist.o expstr.o memory.o
#	$(link) $(mode) test_out.c dllist.o expstr.o outfile.o string.o memory.o

test_str : test_str.c memory.o string.o
	$(link) $(mode) test_str.c memory.o string.o

# EOF
