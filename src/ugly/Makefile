##
## makefile for ugly functions
##
## (W) by Tommy-Saftwörx
##
## $VER: makefile_ugly 1.1.3 (31.7.95)
##

opts= -ansi -DUNIX -ggdb
all: ugly_dbg.o test_fn test_args test_file test_mem



##
## project
##

ugly.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
	gcc -c $(opts) ugly.c

ugly_dbg.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
	gcc -c $(opts) ugly.c -DUMEM_TRACKING_DEF -o ugly_dbg.o

header: #?.h
	copy #?.hgnu:include/ugly

##
## project files
##

## not include: types.h

args.o: args.h args.c args_hlp.c args_prp.c args_set.c string.h dllist.h
	gcc -c $(opts) args.c -DUMEM_TRACKING

bntree.o  : types.h bntree.h bntree.c
	gcc -c $(opts) bntree.c -DUMEM_TRACKING

dllist.o  : types.h dllist.h dllist.c
	gcc -c $(opts) dllist.c -DUMEM_TRACKING

fname.o   : types.h string.h fname.h fname.c
	gcc -c $(opts) fname.c -DUMEM_TRACKING

memory.o   : types.h memory.c memory.h dllist.h
	gcc -c $(opts) memory.c

prginfo.o : types.h prginfo.c
	gcc -c $(opts) prginfo.c

string.o  : types.h string.h string.c
	gcc -c $(opts) string.c -DUMEM_TRACKING

umx.o : types.h umx.h umx.c
	gcc -c $(opts) umx.c -DUMEM_TRACKING

infile.o : types.h string.h memory.h infile.c
	gcc -c $(opts) infile.c -DUMEM_TRACKING

##
## test files
##

test_fn : test_fn.c fname.o memory.o
	gcc $(opts) test_fn.c fname.o string.o memory.o -DUMEM_TRACKING -o test_fn

test_args : test_args.c args.o prginfo.o memory.o
	gcc $(opts) test_args.c args.o string.o dllist.o memory.o prginfo.o -DUMEM_TRACKING -o test_args

test_file : test_file.c infile.o dllist.o memory.o
	gcc $(opts) test_file.c dllist.o infile.o string.o memory.o -DUMEM_TRACKING -o test_file

test_list : test_list.c dllist.o string.o memory.o
	gcc $(opts) test_list.c dllist.o string.o memory.o -DUMEM_TRACKING -o test_list

test_mem : test_mem.c memory.o dllist.o string.o
	gcc $(opts) test_mem.c dllist.o memory.o string.o -DUMEM_TRACKING -o test_mem


