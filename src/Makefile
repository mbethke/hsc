#
# Makefile for hsc ("html sucks completely")
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
# conditionals (ifdef/ifeq/else/endif) and .PHONY, , it should 
# also work with other versions of `make'.
#
# If you have problems, maybe you will want to try "Makefile.dodl"
# in the same directory.
#
#====================================================================
#
# Selection of compiler and environment
#
# if you specify none, a cc-like compiler and a posix-compatible
# environment will be asumed
#

AMIGA_GCC	= 1	# amiga & gcc
#AMIGA_SASC	= 1	# amiga & sas/c 6.x
#AMIGA_VBCC 	= 1	# amiga & vbcc (experimental)
#POSIX_GCC	= 1	# posix & gcc

#--------------------------------------------------------------------
# Selection of compiler mode
#
# if you specify none, an unoptimised version with a few
# debugging stuff will be created
#

COMPILER_MODE	= opt	# create optimised version
#COMPILER_MODE	= dbg	# create debugging version

#--------------------------------------------------------------------
#
# normally, there should be no need to change anything below this line
#
#====================================================================

ifdef AMIGA_GCC
#
# gcc AMIGA
#
CPU	=-m68000
#CPU	=-m68020-40
SYS	=-DAMIGA -Damigados -fbaserel -noixemul $(CPU) -DOLDIFCOND
DEBUG	=-DDEBUG -DDEBUG_UGLY -ggdb
NORM	=-ggdb
OPTIM	=-O2 -fstrength-reduce -s -Wno-unused
COMP	=gcc $(SYS) -Wall -W -ansi -I. -o $@ -c
LINK	=gcc $(SYS) -Wall -W -ansi -I. -o $@
COPY	=copy quiet clone
DEL	=delete quiet

else
ifdef POSIX_GCC
#
# gcc UNIX
#
SYS	=-DUNIX #-DOLDIFCOND
DEBUG	=-DDEBUG -DDEBUG_UGLY -ggdb
NORM	=-ggdb
OPTIM	=-O2 -fstrength-reduce -s -Wno-unused
COMP	=gcc $(SYS) -Wall -W -ansi -I. -o $@ -c
LINK	=gcc $(SYS) -Wall -W -ansi -I. -o $@
COPY	=cp
DEL	=rm -f

else
ifdef AMIGA_SASC
#
# sas/c AMIGA
#
SYS	=DEF=OLDIFCOND
DEBUG	=DEBUG=SF DEF=DEBUG DEF=DEBUG_UGLY
NORM	=DEBUG=LINE
OPTIM	=STRIPDEBUG IGN=304 IGN=93 OPTIMIZE NOSTKCHK
LINK	=sc $(SYS) LINK
COMP	=sc $(SYS)
COPY	=copy quiet clone
DEL	=delete quiet

else
ifdef AMIGA_VBCC
#
# vbcc AMIGA
#
SYS 	=-DAMIGA -dontwarn=205
DEBUG	=-DDEBUG -DDEBUG_UGLY
NORM	=
OPTIM	=
COMP	=vc $(SYS) -I. -Iugly -o $@ -c
LINK	=vc $(SYS) -I. -Iugly -o $@
COPY	=copy quiet clone
DEL	=delete quiet

else
#
# cc (should work on most Unix-esh systems;
#    but no optimisation is performed)
#
SYS	=-DUNIX
DEBUG	=-DDEBUG -DDEBUG_UGLY
NORM	=
OPTIM	=
COMP	=cc $(SYS) -I. -o $@ -c
LINK	=cc $(SYS) -I. -o $@
COPY	=cp
DEL	=rm -f

endif	# AMIGA_VBCC
endif	# AMIGA_SASC
endif	# POSIX_GCC
endif	# AMIGA_GCC

#
# compiler mode 
#
ifeq ($(strip $(COMPILER_MODE)),opt)
CMODE	=$(OPTIM)
else
ifeq ($(strip $(COMPILER_MODE)),dbg)
CMODE	=$(DEBUG)
else
CMODE	=$(NORM)
endif
endif

#
# objects
#
obj_tags =\
	hsclib/tag_a.o \
	hsclib/tag_hsc.o \
	hsclib/tag_if.o \
	hsclib/tag_macr.o \
	hsclib/tag_misc.o

obj_hsclib =\
	hsclib/attrib.o \
	hsclib/defattr.o \
	hsclib/deftag.o \
	hsclib/document.o \
	hsclib/entity.o \
	hsclib/eval.o \
	hsclib/hscprc.o \
	hsclib/idref.o \
	hsclib/include.o \
	hsclib/input.o \
	hsclib/linit.o \
	hsclib/lmessage.o \
	hsclib/lstatus.o \
	hsclib/parse.o \
	hsclib/posteval.o \
	hsclib/project.o \
	hsclib/size.o \
	hsclib/skip.o \
	hsclib/tag.o \
	hsclib/uri.o \
	$(obj_tags)

obj_hsc =\
	hsc/hsc.o \
	hsc/args.o \
	hsc/callback.o \
	hsc/global.o \
	hsc/output.o \
	hsc/status.o \

obj_hscdepp =\
	hscdepp/hscdepp.o

obj_ugly =\
	ugly/dllist.o	\
	ugly/expstr.o	\
	ugly/fname.o	\
	ugly/infile.o	\
	ugly/prginfo.o	\
	ugly/uargs.o	\
	ugly/umemory.o	\
	ugly/ustring.o	\
	ugly/utime.o

#
# ugly modules to be linked
#
# sas/c-amiga: the modules are stored in SCOPTIONS, otherwise the command
#	calling the linker would be too long for #$%& Amiga-Shell..
#
ifndef AMIGA_SASC
link_ugly = $(obj_ugly)
else
link_ugly =	# empty string
endif


#
# generic rule for object-file
#
%.o : %.c
	$(COMP) $*.c $(CMODE)

#
# compile all tools
#
.PHONY : all ugly hsclib hsc

all : ugly hsclib hsc hscdepp

all_all: ugly/all_ugly.o hsclib/all_hscl.o hsc/all_hsc hscdepp/all_hscd

hsclib/all_hscl.o : hsclib/all_hscl.c
	$(COMP) hsclib/all_hscl.c $(CMODE)

hsc/all_hsc.o : hsc/all_hsc.c
	$(COMP) hsc/all_hsc.c $(CMODE)

hsc/all_hsc : hsclib/all_hscl.o hsc/all_hsc.o ugly/all_ugly.o
	$(LINK) hsc/all_hsc.o hsclib/all_hscl.o ugly/all_ugly.o $(CMODE)
	$(COPY) hsc/all_hsc hsc/hsc

hscdepp/all_hscd : hscdepp/all_hscd.o hsclib/all_hscl.o ugly/all_ugly.o
	$(LINK) hscdepp/all_hscd.o hsclib/all_hscl.o ugly/all_ugly.o $(CMODE)
	$(COPY) hscdepp/all_hscd hscdepp/hscdepp

#
# cleanup - remove all objects and executables
#
clean :
	$(DEL) hsc/*.o hsc/*.p hsc/*.s hsc/*.lnk
	$(DEL) hscdepp/*.o hscdepp/*.p hscdepp/*.s hscdepp/*.lnk
	$(DEL) hsclib/*.o hsclib/*.p hsclib/*.s hsclib/*.lnk
	$(DEL) ugly/*.o ugly/*.p ugly/*.s ugly/*.lnk

all_clean :
	$(DEL) ugly/all_ugly.o
	$(DEL) hsclib/all_hscl.o
	$(DEL) hsc/all_hsc.o
	$(DEL) hsc/all_hsc
	$(DEL) hscdepp/all_hscd.o
	$(DEL) hscdepp/all_hscd

hsclib : $(obj_hsclib)
ugly   : $(obj_ugly)
hsc/hsc: $(obj_ugly) $(obj_hsclib) $(obj_hsc)
	$(LINK) $(obj_hsc) $(obj_hsclib) $(link_ugly)
hscdepp/hscdepp : $(obj_ugly) $(obj_hsclib) $(obj_hscdepp)
	$(LINK) $(obj_hscdepp) $(obj_hsclib) $(link_ugly)

hsc : hsc/hsc
hscdepp : hscdepp/hscdepp


#--------------------------------------
#
# You better do not try to use any of
# the rules below; they probably won't 
# work on your system and are not 
# needed to compile `hsc'.
#
#--------------------------------------

#
# copy executables to distribution dir
#
DISTDIR = prog:hsc/distrib/hsc/

dist : 
	$(COPY) hsc/hsc $(DISTDIR)
	$(COPY) hscdepp/hscdepp $(DISTDIR)
	$(COPY) rexx/hScMsg.rexx $(DISTDIR)grafflwerk
	$(COPY) rexx/SendAWeb.rexx $(DISTDIR)grafflwerk
	$(COPY) rexx/SendScMsg.rexx $(DISTDIR)grafflwerk

dbg :
	$(COPY) dbg.prefs hsc.prefs

std :
	$(COPY) std.prefs hsc.prefs

bak :
	@execute s/bak

src :
	@execute s/mksrc

wc :
	wc -l ugly/*.c hsclib/*.c hsc/*.c hscdepp/*.c

depend :
	MkDepend ugly/#?.c ugly/#?.h hsclib/#?.c hsclib/#?.h hsc/#?.c hsc/#?.h \
	hscdepp/#?.c -iugly -ihsclib -ihsc -x ugly/test#? -v

#
# bump version (perverted 'cause I'm too stupid to 
# use RCS without frequently trashing my sources..)
#
version: hscvers hscdeppvers

hscvers :
	@echo "8" >hsc_rev.beta
	RevUp 0 hsc REV 9 BETA C EXTRA 
	@move hsc_rev.* hsc/ quiet clone
	@type hsc/hsc_rev.h

hscdeppvers :
	@echo "0" >hscdepp_rev.beta
	RevUp 1 hscdepp REV 1 C EXTRA
	@move hscdepp_rev.* hscdepp/ quiet clone
	@type hscdepp/hscdepp_rev.h

#
# the dependencies below are created using `MkDepend' by Lars Düning,
# available from amient:dev/c/MkDepend.lha
#

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS FOLLOW ---
hsc/all_hsc.o : hsc/hsc.c hsc/callback.c hsc/output.c hsc/args.c \
    hsc/status.c hsc/global.c hsc/hsc_rev.h hsc/status.h hsc/output.h \
    hsc/callback.h hsc/args.h ugly/returncd.h ugly/prginfo.h ugly/uargs.h \
    hsc/global.h ugly/fname.h ugly/infile.h ugly/utypes.h ugly/umemory.h \
    ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/hsclib.h ugly/expstr.h ugly/dllist.h ugly/udebug.h \
    hsclib/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h \
    hsclib/document.h

hsc/args.o : ugly/returncd.h ugly/prginfo.h ugly/fname.h ugly/uargs.h \
    hsc/status.h hsc/global.h ugly/expstr.h ugly/utypes.h ugly/umemory.h \
    ugly/infile.h ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/hsclib.h ugly/udebug.h ugly/dllist.h \
    hsclib/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h \
    hsclib/document.h

hsc/args.h :

hsc/callback.o : hsc/callback.h ugly/returncd.h hsc/status.h hsc/output.h \
    hsc/global.h ugly/infile.h ugly/utypes.h hsc/hdebug.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/hsclib.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hsclib/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h \
    hsclib/document.h

hsc/callback.h :

hsc/global.o : hsc/global.h ugly/returncd.h hsc/hdebug.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/infile.h ugly/dllist.h ugly/utypes.h \
    hsclib/hsclib.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hsclib/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h \
    hsclib/document.h

hsc/global.h : hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/infile.h ugly/dllist.h ugly/utypes.h hsclib/hsclib.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h hsclib/project.h \
    hsclib/linit.h hsclib/include.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h hsclib/document.h

hsc/hdebug.h :

hsc/hsc.o : hsc/hsc_rev.h hsc/status.h hsc/output.h hsc/callback.h \
    hsc/args.h ugly/returncd.h ugly/prginfo.h ugly/uargs.h hsc/global.h \
    ugly/infile.h ugly/utypes.h ugly/umemory.h ugly/utypes.h hsc/hdebug.h \
    ugly/ustring.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hsclib/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h hsclib/ldebug.h \
    hsclib/document.h

hsc/hsc_rev.h :

hsc/output.o : ugly/returncd.h hsc/status.h hsc/global.h ugly/infile.h \
    ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h hsclib/project.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    hsclib/ldebug.h hsclib/document.h

hsc/output.h :

hsc/status.o : ugly/returncd.h hsc/status.h hsc/global.h ugly/infile.h \
    ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h hsclib/project.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    hsclib/ldebug.h hsclib/document.h

hsc/status.h : ugly/infile.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h

hscdepp/all_hscd.o : hscdepp/hscdepp.c hscdepp/hscdepp_rev.h \
    hsclib/project.h hsclib/document.h hsclib/hsclib.h ugly/returncd.h \
    ugly/prginfo.h ugly/uargs.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h ugly/umemory.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/udebug.h ugly/dllist.h ugly/expstr.h

hscdepp/hscdepp.o : hscdepp/hscdepp_rev.h hsclib/project.h \
    hsclib/document.h hsclib/hsclib.h ugly/returncd.h ugly/prginfo.h \
    ugly/uargs.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    ugly/umemory.h ugly/utypes.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h ugly/udebug.h \
    ugly/dllist.h ugly/expstr.h

hsclib/all_hscl.o : hsclib/tag_macr.c hsclib/tag_if.c hsclib/tag_hsc.c \
    hsclib/tag_a.c hsclib/tag_misc.c hsclib/size.c hsclib/linit.c \
    hsclib/include.c hsclib/parse.c hsclib/deftag.c hsclib/defattr.c \
    hsclib/posteval.c hsclib/eval.c hsclib/project.c hsclib/uri.c \
    hsclib/skip.c hsclib/input.c hsclib/lstatus.c hsclib/hscprc.c \
    hsclib/document.c hsclib/idref.c hsclib/attrib.c hsclib/tag.c \
    hsclib/entity.c hsclib/lmessage.c ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/msgid.h \
    hsclib/ldebug.h hsclib/parse.h hsclib/include.h hsclib/deftag.h \
    hsclib/defattr.h hsclib/inc_tagcb.h hsclib/skip.h hsclib/eval.h \
    hsclib/tag_if.h hsclib/tag_macr.h hsclib/uri.h ugly/fname.h \
    hsclib/document.h hsclib/inc_base.h hsclib/tag_misc.h hsclib/tag_hsc.h \
    hsclib/tag_a.h hsclib/input.h hsclib/size.h hsclib/project.h \
    hsclib/posteval.h hsclib/idref.h ugly/returncd.h hsclib/tag.h \
    hsclib/entity.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hsclib/hscprc.h hsclib/lstatus.h hsclib/lmessage.h hsclib/attrib.h

hsclib/attrib.o : hsclib/uri.h hsclib/eval.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hsclib/document.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/attrib.h :

hsclib/defattr.o : hsclib/input.h hsclib/eval.h hsclib/defattr.h \
    hsclib/inc_base.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h hsclib/document.h

hsclib/defattr.h :

hsclib/deftag.o : hsclib/skip.h hsclib/input.h hsclib/eval.h \
    hsclib/defattr.h hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h \
    ugly/utypes.h hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    hsclib/document.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h

hsclib/deftag.h :

hsclib/document.o : hsclib/document.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/document.h :

hsclib/entity.o : hsclib/entity.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/document.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/entity.h :

hsclib/eval.o : hsclib/uri.h hsclib/skip.h hsclib/input.h hsclib/eval.h \
    hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h hsclib/document.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/eval.h :

hsclib/hsclib.h : hsclib/project.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h hsclib/document.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/hscprc.o : hsclib/project.h hsclib/idref.h ugly/returncd.h \
    ugly/fname.h hsclib/inc_base.h ugly/expstr.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/udebug.h hsclib/document.h ugly/dllist.h

hsclib/hscprc.h : hsclib/document.h hsclib/tag.h

hsclib/idref.o : hsclib/idref.h hsclib/document.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/idref.h :

hsclib/inc_base.h : hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h hsclib/document.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/inc_tagcb.h : hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/ldebug.h hsclib/document.h ugly/dllist.h \
    ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/include.o : hsclib/include.h hsclib/parse.h hsclib/input.h \
    hsclib/inc_base.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h hsclib/document.h

hsclib/include.h :

hsclib/input.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/document.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/input.h : ugly/infile.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h

hsclib/ldebug.h :

hsclib/linit.o : hsclib/tag_misc.h hsclib/tag_macr.h hsclib/tag_if.h \
    hsclib/tag_hsc.h hsclib/tag_a.h hsclib/parse.h hsclib/include.h \
    hsclib/deftag.h hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/document.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/linit.h :

hsclib/lmessage.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/document.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/lmessage.h :

hsclib/lstatus.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/document.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/lstatus.h :

hsclib/msgid.h : ugly/utypes.h ugly/udebug.h

hsclib/parse.o : hsclib/uri.h hsclib/size.h hsclib/skip.h hsclib/project.h \
    hsclib/posteval.h hsclib/parse.h hsclib/input.h hsclib/include.h \
    hsclib/idref.h hsclib/deftag.h hsclib/defattr.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/ldebug.h hsclib/document.h ugly/dllist.h \
    ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/parse.h :

hsclib/posteval.o : hsclib/uri.h hsclib/idref.h hsclib/document.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/posteval.h :

hsclib/project.o : hsclib/project.h hsclib/document.h hsclib/idref.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hsclib/msgid.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h

hsclib/project.h :

hsclib/size.o : hsclib/uri.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hsclib/document.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/size.h :

hsclib/skip.o : hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/ldebug.h hsclib/document.h ugly/dllist.h \
    ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/skip.h : hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/document.h hsclib/tag.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h

hsclib/tag.o : hsclib/tag.h hsclib/defattr.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hsclib/document.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/tag.h :

hsclib/tag_a.o : hsclib/document.h hsclib/inc_tagcb.h hsclib/skip.h \
    hsclib/input.h hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h \
    ugly/utypes.h hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/tag_a.h :

hsclib/tag_hsc.o : hsclib/tag_if.h hsclib/tag_macr.h hsclib/uri.h \
    hsclib/parse.h hsclib/include.h hsclib/eval.h hsclib/deftag.h \
    hsclib/defattr.h ugly/fname.h hsclib/inc_tagcb.h ugly/expstr.h \
    ugly/utypes.h hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    ugly/udebug.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h hsclib/document.h \
    ugly/dllist.h

hsclib/tag_hsc.h :

hsclib/tag_if.o : hsclib/skip.h hsclib/parse.h hsclib/eval.h \
    hsclib/inc_tagcb.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/input.h hsclib/inc_base.h hsclib/document.h hsclib/tag.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/ldebug.h

hsclib/tag_if.h :

hsclib/tag_macr.o : hsclib/parse.h hsclib/include.h hsclib/deftag.h \
    hsclib/defattr.h hsclib/inc_tagcb.h hsclib/skip.h hsclib/input.h \
    hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h hsclib/document.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/tag_macr.h :

hsclib/tag_misc.o : hsclib/skip.h hsclib/parse.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/ldebug.h hsclib/document.h ugly/dllist.h \
    ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/tag_misc.h :

hsclib/uri.o : hsclib/uri.h hsclib/project.h hsclib/idref.h \
    hsclib/inc_base.h ugly/fname.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h ugly/expstr.h \
    ugly/utypes.h hsclib/document.h ugly/dllist.h ugly/udebug.h

hsclib/uri.h :

ugly/all_ugly.o : ugly/prginfo.c ugly/infile.c ugly/uargs.c ugly/dllist.c \
    ugly/fname.c ugly/expstr.c ugly/ustring.c ugly/umemory.c ugly/udebug.h \
    ugly/utypes.h ugly/umemory.h ugly/infile.h ugly/ustring.h ugly/expstr.h \
    ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c ugly/args_fre.c \
    ugly/uargs.h ugly/dllist.h ugly/fname.h

ugly/args_fre.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_hlp.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_prp.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_set.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/dllist.o : ugly/dllist.h ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/dllist.h : ugly/utypes.h ugly/udebug.h

ugly/expstr.o : ugly/expstr.h ugly/umemory.h ugly/ustring.h ugly/utypes.h \
    ugly/udebug.h

ugly/expstr.h : ugly/utypes.h ugly/udebug.h

ugly/fname.o : ugly/fname.h ugly/expstr.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/fname.h : ugly/expstr.h ugly/utypes.h ugly/udebug.h

ugly/hello.o :

ugly/infile.o : ugly/umemory.h ugly/infile.h ugly/ustring.h ugly/expstr.h \
    ugly/utypes.h ugly/dllist.h ugly/udebug.h

ugly/infile.h : ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

ugly/prginfo.o : ugly/utypes.h ugly/udebug.h

ugly/prginfo.h :

ugly/returncd.h :

ugly/uargs.o : ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c \
    ugly/args_fre.c ugly/uargs.h ugly/dllist.h ugly/ustring.h \
    ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/uargs.h : ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/udebug.h :

ugly/umemory.o : ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/umemory.h : ugly/utypes.h ugly/udebug.h

ugly/ustring.o : ugly/ustring.h ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/ustring.h : ugly/utypes.h ugly/udebug.h

ugly/utime.o : ugly/utime.h ugly/expstr.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/utime.h : ugly/expstr.h ugly/utypes.h ugly/udebug.h

ugly/utypes.h : ugly/udebug.h

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS PRECEDE ---
