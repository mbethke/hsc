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
# implicit rule for object-file
#
%.o : %.c
	$(COMP) $*.c $(CMODE)

#
# test files
#
all : test_es test_fn test_args test_file test_list test_mem test_str test_time

clean :
	delete (#?.o|#?.s|#?.p) quiet

depend :
	MkDepend #?.c -i/ -v


##
## project
##

#ugly.o: uargs.o bntree.o dllist.o memory.o prginfo.o ustring.o umx.o fname.o infile.o ugly.c
#        $(COMP)  ugly.c

#ugly_dbg.o: uargs.o bntree.o dllist.o memory.o prginfo.o ustring.o umx.o fname.o infile.o ugly.c
#        $(COMP)  ugly.c -DUMEM_TRACKING_DEF -o ugly_dbg.o

#header: #?.h
#        copy #?.h sc:include/ugly

all_ugly.o :
	$(COMP) all_ugly.c $(mode)

##
## test files
##

test_fn : test_fn.c fname.o umemory.o expstr.o
	$(LINK) $(mode) test_fn.c expstr.o fname.o ustring.o umemory.o

test_args : test_args.c uargs.o dllist.o prginfo.o umemory.o ustring.o
	$(LINK) $(mode) test_args.c uargs.o ustring.o dllist.o umemory.o prginfo.o

test_es : test_es.c expstr.o umemory.o ustring.o
	$(LINK) $(mode) test_es.c expstr.o ustring.o umemory.o

test_file : test_file.c infile.o dllist.o expstr.o umemory.o
	$(LINK) $(mode) test_file.c dllist.o expstr.o infile.o ustring.o umemory.o

test_list : test_list.c dllist.o ustring.o umemory.o
	$(LINK) $(mode) test_list.c dllist.o ustring.o umemory.o

test_mem : test_mem.c umemory.o dllist.o ustring.o
	$(LINK) $(mode) test_mem.c dllist.o umemory.o ustring.o

#test_out : test_out.c outfile.o dllist.o expstr.o umemory.o
#	$(LINK) $(mode) test_out.c dllist.o expstr.o outfile.o ustring.o umemory.o

test_str : test_str.c umemory.o ustring.o
	$(LINK) $(mode) test_str.c umemory.o ustring.o

test_time.o : test_time.c utime.o umemory.o ustring.o expstr.o
	$(COMP) test_time.c $(mode)

test_time : test_time.o 
	$(LINK) $(mode) test_time.c umemory.o ustring.o expstr.o utime.o

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS FOLLOW ---
all_ugly.o : /ugly/prginfo.c /ugly/infile.c /ugly/ustrlist.c /ugly/ufile.c \
    /ugly/uargs.c /ugly/dllist.c /ugly/fname.c /ugly/expstr.c \
    /ugly/ustring.c /ugly/umemory.c /ugly/udebug.h utypes.h umemory.h \
    infile.h fname.h ustring.h expstr.h ustrlist.h ufile.h args_hlp.c \
    args_prp.c args_set.c args_fre.c uargs.h dllist.h udebug.h

args_fre.o : uargs.h dllist.h ustring.h umemory.h utypes.h udebug.h

args_hlp.o : uargs.h dllist.h ustring.h umemory.h utypes.h udebug.h

args_prp.o : uargs.h dllist.h ustring.h umemory.h utypes.h udebug.h

args_set.o : uargs.h dllist.h ustring.h umemory.h utypes.h udebug.h

dllist.o : dllist.h umemory.h utypes.h udebug.h

expstr.o : expstr.h umemory.h ustring.h utypes.h udebug.h

fname.o : fname.h expstr.h ustring.h umemory.h utypes.h udebug.h

hello.o :

infile.o : umemory.h infile.h fname.h ustring.h expstr.h utypes.h dllist.h \
    udebug.h

prginfo.o : utypes.h udebug.h

test_args.o : umemory.h prginfo.h dllist.h uargs.h utypes.h udebug.h

test_es.o : expstr.h ustring.h umemory.h utypes.h udebug.h

test_file.o : infile.h umemory.h utypes.h dllist.h expstr.h udebug.h

test_fn.o : fname.h expstr.h ustring.h utypes.h udebug.h

test_list.o : dllist.h ustring.h umemory.h utypes.h udebug.h

test_mem.o : ustring.h umemory.h utypes.h udebug.h

test_prg.o : prginfo.h utypes.h udebug.h

test_str.o : ustring.h umemory.h utypes.h udebug.h

test_time.o : utime.h expstr.h ustring.h umemory.h utypes.h udebug.h

uargs.o : args_hlp.c args_prp.c args_set.c args_fre.c uargs.h dllist.h \
    ustring.h umemory.h utypes.h udebug.h

ufile.o : ufile.h expstr.h ustring.h umemory.h utypes.h udebug.h

umemory.o : umemory.h utypes.h udebug.h

ustring.o : ustring.h umemory.h utypes.h udebug.h

ustrlist.o : ustrlist.h dllist.h ustring.h utypes.h udebug.h

utime.o : utime.h expstr.h ustring.h umemory.h utypes.h udebug.h

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS PRECEDE ---
