#
# Makefile for hsc ("html sucks completely")
#
# Copyright (C) 1995,96  Thomas Aglassinger
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
#
#====================================================================
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
# Setup for Unix-Installation
#
# INSTALL	``install'' command to be used
# INSTDIR	base destination-directory 
#		("bin" and "lib" will be added)
#

INSTALL	= install
INSTDIR = /usr/
#INSTDIR	= (HOME)/

#--------------------------------------------------------------------
#
# normally, there should be no need to change anything below this line
#
#====================================================================

ifdef AMIGA_GCC
#
# gcc AMIGA
#
#CPU	= -m68000
CPU	= -m68020-40
SYS	= -DAMIGA -Damigados -fbaserel -noixemul $(CPU)
DEBUG	= -DDEBUG -DDEBUG_UGLY -ggdb
NORM	= -ggdb
OPTIM	= -O2 -fstrength-reduce -s -Wno-unused
COMP	= gcc -o $@ $(SYS) -Wall -W -ansi -I. -c
LINK	= gcc -o $@ $(SYS) -Wall -W -ansi -I.
COPY	= copy quiet clone
DEL	= delete quiet

else
ifdef POSIX_GCC
#
# gcc UNIX
#
SYS	= -DUNIX
DEBUG	= -DDEBUG -DDEBUG_UGLY -ggdb
NORM	= -ggdb
OPTIM	= -O2 -fstrength-reduce -s -Wno-unused
COMP	= gcc -o $@ $(SYS) -Wall -W -ansi -I. -c
LINK	= gcc -o $@ $(SYS) -Wall -W -ansi -I.
COPY	= cp
DEL	= rm -f

else
ifdef AMIGA_SASC
#
# sas/c AMIGA
#
SYS	= #DEF=OLDIFCOND
DEBUG	= DEBUG=SF DEF=DEBUG DEF=DEBUG_UGLY
NORM	= DEBUG=LINE
OPTIM	= STRIPDEBUG IGN=304 IGN=93 OPTIMIZE NOSTKCHK
LINK	= sc $(SYS) LINK
COMP	= sc $(SYS)
COPY	= copy quiet clone
DEL	= delete quiet

else
ifdef AMIGA_VBCC
#
# vbcc AMIGA
#
SYS 	= -DAMIGA -dontwarn=205
DEBUG	= -DDEBUG -DDEBUG_UGLY
NORM	=
OPTIM	=
COMP	= vc $(SYS) -I. -Iugly -o $@ -c
LINK	= vc $(SYS) -I. -Iugly -o $@
COPY	= copy quiet clone
DEL	= delete quiet

else
#
# cc (should work on most Unixoid systems)
#
SYS	= -DUNIX
DEBUG	= -DDEBUG -DDEBUG_UGLY
NORM	=
OPTIM	= -s -O
COMP	= cc -o $@ $(SYS) -I. -c
LINK	= cc -o $@ $(SYS) -I. 
COPY	= cp
DEL	= rm -f

endif	# AMIGA_VBCC
endif	# AMIGA_SASC
endif	# POSIX_GCC
endif	# AMIGA_GCC

#
# compiler mode 
#
ifeq ($(strip $(COMPILER_MODE)),opt)
CMODE	= $(OPTIM)
else
ifeq ($(strip $(COMPILER_MODE)),dbg)
CMODE	= $(DEBUG)
else
CMODE	= $(NORM)
endif
endif

#
# objects
#
OBJ_TAGS =\
	hsclib/tag_a.o \
	hsclib/tag_hsc.o \
	hsclib/tag_if.o \
	hsclib/tag_macr.o \
	hsclib/tag_misc.o

OBJ_HSCLIB =\
	hsclib/attrib.o \
	hsclib/defattr.o \
	hsclib/deftag.o \
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
	hsclib/size.o \
	hsclib/skip.o \
	hsclib/tag.o \
	hsclib/uri.o \
	$(OBJ_TAGS)

OBJ_HSCPRJ =\
	hscprj/document.o \
	hscprj/project.o \
	hscprj/readprj.o \
	hscprj/writeprj.o

OBJ_HSC =\
	hsc/hsc.o \
	hsc/args.o \
	hsc/callback.o \
	hsc/global.o \
	hsc/output.o \
	hsc/status.o

OBJ_HSCDEPP =\
	hscdepp/hscdepp.o

OBJ_UGLY =\
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
LINK_UGLY	= $(OBJ_UGLY)
LINK_HSCPRJ	= $(OBJ_HSCPRJ)
else
# empty strings
LINK_UGLY	= 
LINK_HSCPRJ	= 
endif


#
# implicit rule for object-file
#
%.o : %.c
	$(COMP) $*.c $(CMODE)

#
# compile all tools
#
.PHONY : all all_all clean all_clean sterile ugly hsclib hscprj hsc

all : ugly hsclib hscprj hsc hscdepp

all_all: ugly/all_ugly.o hsclib/all_hscl.o hscprj/all_hscp.o hsc/all_hsc \
	hscdepp/all_hscd

hsc/hsc: $(OBJ_UGLY) $(OBJ_HSCLIB) $(OBJ_HSC) $(OBJ_HSCPRJ)
	$(LINK) $(OBJ_HSC) $(OBJ_HSCLIB) $(LINK_HSCPRJ) $(LINK_UGLY)

hscdepp/hscdepp : $(OBJ_UGLY) $(OBJ_HSCPRJ) $(OBJ_HSCDEPP)
	$(LINK) $(OBJ_HSCDEPP) $(LINK_HSCPRJ) $(LINK_UGLY)

hsclib/all_hscl.o : hsclib/all_hscl.c
	$(COMP) hsclib/all_hscl.c $(CMODE)

hsc/all_hsc.o : hsc/all_hsc.c
	$(COMP) hsc/all_hsc.c $(CMODE)

hsc/all_hsc : hsclib/all_hscl.o hsc/all_hsc.o hscprj/all_hscp.o \
		ugly/all_ugly.o
	$(LINK) hsc/all_hsc.o hsclib/all_hscl.o hscprj/all_hscp.o \
		ugly/all_ugly.o $(CMODE)
	$(COPY) hsc/all_hsc hsc/hsc

hscdepp/all_hscd : hscdepp/all_hscd.o hscprj/all_hscp.o ugly/all_ugly.o
	$(LINK) hscdepp/all_hscd.o hscprj/all_hscp.o ugly/all_ugly.o $(CMODE)
	$(COPY) hscdepp/all_hscd hscdepp/hscdepp

#
# installation
#
install : all
	$(INSTALL) -m 755 hsc/hsc $(INSTDIR)bin/hsc
	$(INSTALL) -m 755 hscdepp/hscdepp $(INSTDIR)bin/hsc
	$(INSTALL) -m 644 hsc.prefs $(INSTDIR)lib/hsc.prefs

#
# cleanup - remove all objects and executables
#
clean :
	-$(DEL) hsc/*.o hsc/*.p hsc/*.s hsc/*.lnk
	-$(DEL) hscdepp/*.o hscdepp/*.p hscdepp/*.s hscdepp/*.lnk
	-$(DEL) hsclib/*.o hsclib/*.p hsclib/*.s hsclib/*.lnk
	-$(DEL) hscprj/*.o hscprj/*.p hscprj/*.s hscprj/*.lnk
	-$(DEL) ugly/*.o ugly/*.p ugly/*.s ugly/*.lnk

all_clean :
	-$(DEL) ugly/all_ugly.o
	-$(DEL) hsclib/all_hscl.o
	-$(DEL) hsclib/all_hscp.o
	-$(DEL) hsc/all_hsc.o
	-$(DEL) hsc/all_hsc
	-$(DEL) hscdepp/all_hscd.o
	-$(DEL) hscdepp/all_hscd

sterile : clean all_clean
	-$(DEL) hsc/hsc
	-$(DEL) hscdepp/hscdepp

#
# some shortcuts
#
hsclib : $(OBJ_HSCLIB)

hscprj : $(OBJ_HSCPRJ)

ugly   : $(OBJ_UGLY)

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

# copy executables to distribution dir
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

tgz : sterile
	@execute s/mktgz

wc :
	wc -l ugly/*.c hsclib/*.c hscprj/*.c hsc/*.c hscdepp/*.c

depend :
	MkDepend ugly/#?.c hsclib/#?.c hsc/#?.c hscdepp/#?.c hscprj/#?.c \
	-iugly -ihsclib -ihsc -ihscprj -x ugly/test#? -v

dodldep :
	MkDepend MAKE=Makefile.dodl \
	ugly/all_ugly.c hsclib/all_hscl.c hsc/all_hsc.c hscprj/all_hscp.c \
		hscdepp/all_hscd.c -iugly -ihsclib -ihsc -ihscprj -ihscdepp -v

#
# bump version (perverted 'cause I'm too stupid to 
# use RCS without frequently trashing my sources..)
#
version: hscvers hscdeppvers

hscvers :
	@echo "9" >hsc_rev.beta
	RevUp 0 hsc REV 9 BETA C EXTRA 
	@move hsc_rev.* hsc/ quiet
	@type hsc/hsc_rev.h

hscdeppvers :
	@echo "0" >hscdepp_rev.beta
	RevUp 1 hscdepp REV 2 C EXTRA
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
    hscprj/project.h hsc/global.h ugly/fname.h ugly/infile.h ugly/utypes.h \
    ugly/umemory.h ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/hsclib.h ugly/expstr.h ugly/dllist.h \
    ugly/udebug.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    hsclib/ldebug.h

hsc/args.o : ugly/returncd.h ugly/prginfo.h ugly/fname.h ugly/uargs.h \
    hsc/status.h hsc/global.h ugly/expstr.h ugly/utypes.h ugly/umemory.h \
    ugly/infile.h ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/hsclib.h ugly/udebug.h ugly/dllist.h \
    hscprj/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    hsclib/ldebug.h

hsc/callback.o : hsc/callback.h ugly/returncd.h hsc/status.h hsc/output.h \
    hsc/global.h ugly/infile.h ugly/utypes.h hsc/hdebug.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/hsclib.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hscprj/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    hsclib/ldebug.h

hsc/global.o : hsc/global.h ugly/returncd.h hsc/hdebug.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/infile.h ugly/dllist.h ugly/utypes.h \
    hsclib/hsclib.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hscprj/project.h hsclib/linit.h hsclib/include.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    hsclib/ldebug.h

hsc/hsc.o : hsc/hsc_rev.h hsc/status.h hsc/output.h hsc/callback.h \
    hsc/args.h ugly/returncd.h ugly/prginfo.h ugly/uargs.h hscprj/project.h \
    hsc/global.h ugly/infile.h ugly/utypes.h ugly/umemory.h ugly/utypes.h \
    hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hsclib/linit.h hsclib/include.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h hsclib/ldebug.h

hsc/output.o : ugly/returncd.h hsc/status.h hsc/global.h ugly/infile.h \
    ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h hscprj/project.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hscprj/document.h \
    hsclib/msgid.h hsclib/ldebug.h

hsc/status.o : ugly/returncd.h hsc/status.h hsc/global.h ugly/infile.h \
    ugly/utypes.h hsc/hdebug.h ugly/ustring.h ugly/umemory.h ugly/expstr.h \
    ugly/dllist.h hsclib/hsclib.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h hscprj/project.h hsclib/linit.h hsclib/include.h \
    hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hscprj/document.h \
    hsclib/msgid.h hsclib/ldebug.h

hscdepp/all_hscd.o : hscdepp/hscdepp.c hscdepp/hscdepp_rev.h \
    hscprj/project.h hscprj/document.h ugly/returncd.h ugly/prginfo.h \
    ugly/uargs.h ugly/infile.h ugly/expstr.h ugly/dllist.h ugly/ustring.h \
    ugly/umemory.h ugly/utypes.h hsclib/ldebug.h ugly/umemory.h \
    ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h

hscdepp/hscdepp.o : hscdepp/hscdepp_rev.h hscprj/project.h \
    hscprj/document.h ugly/returncd.h ugly/prginfo.h ugly/uargs.h \
    ugly/infile.h ugly/expstr.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h hsclib/ldebug.h ugly/umemory.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/all_hscl.o : hsclib/tag_macr.c hsclib/tag_if.c hsclib/tag_hsc.c \
    hsclib/tag_a.c hsclib/tag_misc.c hsclib/size.c hsclib/linit.c \
    hsclib/include.c hsclib/parse.c hsclib/deftag.c hsclib/defattr.c \
    hsclib/posteval.c hsclib/eval.c hsclib/uri.c hsclib/skip.c \
    hsclib/input.c hsclib/lstatus.c hsclib/hscprc.c hsclib/idref.c \
    hsclib/attrib.c hsclib/tag.c hsclib/entity.c hsclib/lmessage.c \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/msgid.h hsclib/ldebug.h hsclib/parse.h \
    hsclib/include.h hsclib/deftag.h hsclib/defattr.h hsclib/inc_tagcb.h \
    hsclib/skip.h hsclib/eval.h hsclib/tag_if.h hsclib/tag_macr.h \
    hsclib/uri.h ugly/fname.h hscprj/document.h hsclib/inc_base.h \
    hsclib/tag_misc.h hsclib/tag_hsc.h hsclib/tag_a.h hsclib/input.h \
    hsclib/size.h hscprj/project.h hsclib/posteval.h hsclib/idref.h \
    ugly/returncd.h hsclib/tag.h hsclib/entity.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h hsclib/hscprc.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/attrib.h

hsclib/attrib.o : hsclib/uri.h hsclib/eval.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h hscprj/project.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsclib/defattr.o : hsclib/input.h hsclib/eval.h hsclib/defattr.h \
    hsclib/inc_base.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hscprj/project.h

hsclib/deftag.o : hsclib/skip.h hsclib/input.h hsclib/eval.h \
    hsclib/defattr.h hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h \
    ugly/utypes.h hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hscprj/document.h hsclib/msgid.h \
    ugly/ustring.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    hsclib/ldebug.h hscprj/project.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h

hsclib/entity.o : hsclib/entity.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/eval.o : hsclib/uri.h hsclib/skip.h hsclib/input.h hsclib/eval.h \
    hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    hscprj/project.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h

hsclib/hscprc.o : hsclib/tag_if.h hsclib/idref.h hscprj/project.h \
    ugly/returncd.h ugly/fname.h hsclib/inc_base.h ugly/expstr.h \
    ugly/utypes.h hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h \
    hsclib/tag.h hsclib/entity.h hsclib/attrib.h hscprj/document.h \
    hsclib/msgid.h ugly/ustring.h ugly/infile.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h ugly/udebug.h \
    ugly/dllist.h

hsclib/idref.o : hsclib/idref.h hscprj/document.h hsclib/inc_base.h \
    ugly/infile.h ugly/ustring.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/ldebug.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hsclib/msgid.h ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hscprj/project.h

hsclib/include.o : hsclib/include.h hsclib/parse.h hsclib/input.h \
    hsclib/inc_base.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hscprj/project.h

hsclib/input.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/linit.o : hsclib/tag_misc.h hsclib/tag_macr.h hsclib/tag_if.h \
    hsclib/tag_hsc.h hsclib/tag_a.h hsclib/parse.h hsclib/include.h \
    hsclib/deftag.h hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/lmessage.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/lstatus.o : hsclib/inc_base.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/parse.o : hsclib/uri.h hsclib/size.h hsclib/skip.h hscprj/project.h \
    hsclib/posteval.h hsclib/parse.h hsclib/input.h hsclib/include.h \
    hsclib/idref.h hsclib/deftag.h hsclib/defattr.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h ugly/dllist.h ugly/expstr.h \
    ugly/utypes.h ugly/udebug.h

hsclib/posteval.o : hsclib/uri.h hsclib/idref.h hscprj/document.h \
    hsclib/inc_base.h ugly/infile.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h ugly/utypes.h hsclib/ldebug.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h \
    hsclib/entity.h hsclib/attrib.h hsclib/msgid.h ugly/dllist.h \
    ugly/expstr.h ugly/utypes.h ugly/udebug.h hscprj/project.h

hsclib/size.o : hsclib/uri.h hsclib/inc_base.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/hscprc.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/skip.o : hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h hscprj/project.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/tag.o : hsclib/tag.h hsclib/defattr.h hsclib/inc_base.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/hscprc.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/project.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hsclib/tag_a.o : hscprj/document.h hsclib/inc_tagcb.h ugly/infile.h \
    ugly/ustring.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h hsclib/hscprc.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hsclib/msgid.h hscprj/project.h

hsclib/tag_hsc.o : hsclib/tag_if.h hsclib/tag_macr.h hsclib/uri.h \
    hsclib/parse.h hsclib/include.h hsclib/eval.h hsclib/deftag.h \
    hsclib/defattr.h ugly/fname.h hsclib/inc_tagcb.h ugly/expstr.h \
    ugly/utypes.h hsclib/skip.h hsclib/input.h hsclib/inc_base.h \
    ugly/udebug.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    hscprj/project.h ugly/dllist.h

hsclib/tag_if.o : hsclib/skip.h hsclib/parse.h hsclib/eval.h \
    hsclib/inc_tagcb.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/input.h hsclib/inc_base.h hscprj/project.h hsclib/tag.h \
    hsclib/ldebug.h ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h

hsclib/tag_macr.o : hsclib/parse.h hsclib/include.h hsclib/deftag.h \
    hsclib/defattr.h hsclib/inc_tagcb.h hsclib/skip.h hsclib/input.h \
    hsclib/inc_base.h hsclib/hscprc.h ugly/infile.h ugly/utypes.h \
    hsclib/lstatus.h hsclib/lmessage.h hsclib/tag.h hsclib/entity.h \
    hsclib/attrib.h hscprj/document.h hsclib/msgid.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h hsclib/ldebug.h \
    hscprj/project.h ugly/dllist.h ugly/expstr.h ugly/utypes.h \
    ugly/udebug.h

hsclib/tag_misc.o : hsclib/skip.h hsclib/parse.h hsclib/inc_base.h \
    hsclib/hscprc.h ugly/infile.h ugly/utypes.h hsclib/lstatus.h \
    hsclib/lmessage.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/umemory.h \
    ugly/expstr.h ugly/dllist.h hsclib/ldebug.h hscprj/project.h \
    ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hsclib/uri.o : hsclib/uri.h hscprj/project.h hsclib/idref.h \
    hsclib/inc_base.h ugly/fname.h hsclib/lstatus.h hsclib/lmessage.h \
    hsclib/hscprc.h hsclib/tag.h hsclib/entity.h hsclib/attrib.h \
    hscprj/document.h hsclib/msgid.h ugly/ustring.h ugly/infile.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/expstr.h ugly/utypes.h ugly/dllist.h ugly/udebug.h

hscprj/all_hscp.o : hscprj/writeprj.c hscprj/readprj.c hscprj/project.c \
    hscprj/document.c hscprj/project.h hscprj/document.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hscprj/pdefs.h hscprj/pdebug.h hsclib/ldebug.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hscprj/document.o : hscprj/document.h ugly/infile.h ugly/ustring.h \
    ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hsclib/ldebug.h ugly/dllist.h ugly/expstr.h ugly/utypes.h ugly/udebug.h

hscprj/project.o : hscprj/project.h hscprj/document.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hscprj/pdebug.h hsclib/ldebug.h ugly/utypes.h ugly/dllist.h \
    ugly/expstr.h ugly/udebug.h

hscprj/readprj.o : hscprj/project.h hscprj/document.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hscprj/pdefs.h hscprj/pdebug.h hsclib/ldebug.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

hscprj/writeprj.o : hscprj/project.h hscprj/document.h ugly/ustring.h \
    ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h ugly/utypes.h \
    hscprj/pdefs.h hscprj/pdebug.h hsclib/ldebug.h ugly/utypes.h \
    ugly/dllist.h ugly/expstr.h ugly/udebug.h

ugly/all_ugly.o : ugly/prginfo.c ugly/infile.c ugly/uargs.c ugly/dllist.c \
    ugly/fname.c ugly/expstr.c ugly/ustring.c ugly/umemory.c ugly/udebug.h \
    ugly/utypes.h ugly/umemory.h ugly/infile.h ugly/fname.h ugly/ustring.h \
    ugly/expstr.h ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c \
    ugly/args_fre.c ugly/uargs.h ugly/dllist.h

ugly/args_fre.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_hlp.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_prp.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/args_set.o : ugly/uargs.h ugly/dllist.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/dllist.o : ugly/dllist.h ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/expstr.o : ugly/expstr.h ugly/umemory.h ugly/ustring.h ugly/utypes.h \
    ugly/udebug.h

ugly/fname.o : ugly/fname.h ugly/expstr.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

ugly/hello.o :

ugly/infile.o : ugly/umemory.h ugly/infile.h ugly/fname.h ugly/ustring.h \
    ugly/expstr.h ugly/utypes.h ugly/dllist.h ugly/udebug.h

ugly/prginfo.o : ugly/utypes.h ugly/udebug.h

ugly/uargs.o : ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c \
    ugly/args_fre.c ugly/uargs.h ugly/dllist.h ugly/ustring.h \
    ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/umemory.o : ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/ustring.o : ugly/ustring.h ugly/umemory.h ugly/utypes.h ugly/udebug.h

ugly/utime.o : ugly/utime.h ugly/expstr.h ugly/ustring.h ugly/umemory.h \
    ugly/utypes.h ugly/udebug.h

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS PRECEDE ---
