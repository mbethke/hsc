#
# Makefile for ugly functions
#
# Copyright (C) 1993,94,95,96  Thomas Aglassinger
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#--------------------------------------------------------------------

#
# This Makefile works fine with GNU make 3.74
#
# But as it doesn't require any special features besides from
# conditionals (ifdef/ifeq/else/endif), it should also work with 
# other versions of `make'.
#

#--------------------------------------------------------------------
# Selection of compiler and environment
#
# if you specify none, a cc-like compiler and a posix-compatible
# environment will be asumed
#

#AMIGA_GCC	= 1	# amiga & gcc
AMIGA_SASC	= 1	# amiga & sas/c 6.x
#AMIGA_VBCC 	= 1	# amiga & vbcc (experimental)
#POSIX_GCC	= 1	# posix & gcc

#--------------------------------------------------------------------
# Selection of compiler mode
#
# if you specify none, an unoptimised version without
# debugging stuff will be created
#

#COMPILER_MODE	= opt	# create optimised version
COMPILER_MODE	= dbg	# create debugging version

#--------------------------------------------------------------------
# usually, there should be no need to change anything below this line
#--------------------------------------------------------------------

#
# description of symbols:
#
# - COMP	compiler call to create an object file (*.o)
# - LINK	compiler call to link a file
# - SYS		defines operating system
#		(supported: AMIGA,UNIX)
# - DEBUG	compiler flags for developer version
# - NORM	compiler flags for non-optimised version
# - OPTIM	compiler flags for optimised version

ifdef AMIGA_GCC
#
# gcc AMIGA
#
SYS  = -DAMIGA -Damigados -noixemul
DEBUG= $(SYS) -DDEBUG_UGLY -ggdb -m68000
NORM = $(SYS) 
OPTIM= $(SYS) -O -s
COMP = gcc -Wall -W -ansi -c
LINK = gcc -Wall -W -ansi -o $@

else
ifdef POSIX_GCC
#
# gcc UNIX
#
SYS  = -DUNIX
DEBUG= $(SYS) -DDEBUG_UGLY -ggdb
NORM = $(SYS) 
OPTIM= $(SYS) -O -s
COMP = gcc -Wall -W -ansi -c
LINK = gcc -Wall -W -ansi -o $@

else
ifdef AMIGA_SASC
#
# sas/c AMIGA
#
SYS  =
opts =  
DEBUG= $(SYS) $(opts) DEBUG=SF DEF=DEBUG_UGLY
NORM = $(SYS) $(opts) DEBUG=LINE
OPTIM= $(SYS) $(opts) IGN=304 IGN=93 OPTIMIZE NOSTKCHK 
LINK = sc LINK 
COMP = sc

else
ifdef AMIGA_VBCC
#
# vbcc AMIGA
#
SYS  = -DAMIGA -dontwarn=205
DEBUG= -DDEBUG
NORM = 
OPTIM=
COMP = vc $(SYS) -o $@ -c
LINK = vc $(SYS) -o $@ 

else
#
# cc (should work on most Unix-esh systems;
#    but no optimisation is performed)
#
SYS  = -DUNIX
DEBUG= -DDEBUG_UGLY
NORM = 
OPTIM=
COMP = cc $(SYS) -o $@ -c
LINK = cc $(SYS) -o $@

endif	# AMIGA_VBCC
endif	# AMIGA_SASC
endif	# POSIX_GCC
endif	# AMIGA_GCC

#
# compiler mode 
#
ifeq ($(strip $(COMPILER_MODE)),opt)
mode = $(OPTIM)
else
ifeq ($(strip $(COMPILER_MODE)),dbg)
mode = $(DEBUG)
else
mode = $(NORM)
endif
endif

#--------------------------------------

#
# test files
#
all : test_es test_fn test_args test_file test_list test_mem test_str test_time

clean :
	delete (#?.o|#?.s|#?.p) quiet

##
## project
##

#ugly.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
#        $(COMP)  ugly.c

#ugly_dbg.o: args.o bntree.o dllist.o memory.o prginfo.o string.o umx.o fname.o infile.o ugly.c
#        $(COMP)  ugly.c -DUMEM_TRACKING_DEF -o ugly_dbg.o

#header: #?.h
#        copy #?.h sc:include/ugly

all_ugly.o :
	$(COMP) all_ugly.c $(mode)

##
## project files
##

## not include: types.h

args.o: args.h args.c args_hlp.c args_prp.c args_set.c string.h dllist.h
	$(COMP)  args.c $(mode)

#bntree.o  : types.h bntree.h bntree.c
#        $(COMP) $(SYS) bntree.c $(mode)

dllist.o  : types.h dllist.h dllist.c
	$(COMP) dllist.c $(mode)

expstr.o : types.h expstr.c
	$(COMP) expstr.c $(mode)

fname.o   : types.h expstr.h string.h fname.h fname.c
	$(COMP) fname.c $(mode)

memory.o   : types.h memory.c memory.h dllist.h
	$(COMP) memory.c $(mode)

prginfo.o : types.h prginfo.c
	$(COMP) prginfo.c $(mode)

string.o  : types.h string.h string.c
	$(COMP) string.c $(mode)

time.o  : types.h expstr.h memory.h string.h time.h time.c
	$(COMP) time.c $(mode)

#umx.o : types.h umx.h umx.c
#	$(COMP) umx.c $(mode)

infile.o : types.h string.h memory.h expstr.h infile.h infile.c
	$(COMP) infile.c $(mode)

#outfile.o : types.h string.h memory.h expstr.h outfile.h outfile.c
#	$(COMP) outfile.c $(mode)

##
## test files
##

test_fn : test_fn.c fname.o memory.o expstr.o
	$(LINK) $(mode) test_fn.c expstr.o fname.o string.o memory.o

test_args : test_args.c args.o dllist.o prginfo.o memory.o string.o
	$(LINK) $(mode) test_args.c args.o string.o dllist.o memory.o prginfo.o

test_es : test_es.c expstr.o memory.o string.o
	$(LINK) $(mode) test_es.c expstr.o string.o memory.o

test_file : test_file.c infile.o dllist.o expstr.o memory.o
	$(LINK) $(mode) test_file.c dllist.o expstr.o infile.o string.o memory.o

test_list : test_list.c dllist.o string.o memory.o
	$(LINK) $(mode) test_list.c dllist.o string.o memory.o

test_mem : test_mem.c memory.o dllist.o string.o
	$(LINK) $(mode) test_mem.c dllist.o memory.o string.o

#test_out : test_out.c outfile.o dllist.o expstr.o memory.o
#	$(LINK) $(mode) test_out.c dllist.o expstr.o outfile.o string.o memory.o

test_str : test_str.c memory.o string.o
	$(LINK) $(mode) test_str.c memory.o string.o

test_time.o : test_time.c time.o memory.o string.o expstr.o
	$(COMP) test_time.c $(mode)

test_time : test_time.o 
	$(LINK) $(mode) test_time.c memory.o string.o expstr.o time.o

# EOF
