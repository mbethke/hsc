#
# Makefile for hsc ("html sucks completely")
#
# Copyright (C) 1995-1998  Thomas Aglassinger
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


#==========================================================
#
# For details on compilation, refer to "docs/source.html"
#
#==========================================================


#------------------------------------------------
#
# These symbols are only needed if you use the
# `install' rule:
#
# INSTALL       `install' command to be used
# INSTDIR       base destination-directory 
#               ("/bin" and "/lib" will be added)
#
#------------------------------------------------
INSTALL         = install
#INSTALL	= /bin/install

INSTDIR         = /usr
#INSTDIR	= /usr/local
#INSTDIR	= $(HOME)

#------------------------------------------------
#
# NeXTStep
#
#------------------------------------------------
CC	= cc
CFLAGS	=
SYS	= -DNEXTSTEP

#------------------------------------------------
#
# AmigaOS with ixemul.library
#
#------------------------------------------------
#CC	= gcc
#CFLAGS	= -m68020-40 -Wall -W -pedantic -Wno-unused -O2 -s \
#	-fstrength-reduce -fbaserel	
#SYS	= -DUNIX -UAMIGA

#------------------------------------------------
#
# BeOS
#
#------------------------------------------------
#CC	= mwcc
#CFLAGS	= -ansi off -char unsigned -dialect c
#SYS	= -DBEOS 

#------------------------------------------------
#
# AmigaOS with PowerUp (experimental)
#
#------------------------------------------------
#CC	= vc
#CFLAGS	= -Iugly
#SYS	= -DAMIGA

#------------------------------------------------
#
# Unixoid Systems
#
# Try to guesstimate the values that might work 
# with your system and compiler.
#
#------------------------------------------------

#------------------------------------------------
# this one should usually work
#
#CC	= cc
#CFLAGS	=
#SYS	= -DUNIX

#------------------------------------------------
# this one should also work and even optimise
#
#CC	= cc
#CFLAGS	= -s -O
#SYS	= -DUNIX

#------------------------------------------------
# create a debugging version
#
#CC	= cc
#CFLAGS	= -DDEBUG -DDEBUG_UGLY -g
#SYS	= -DUNIX

#------------------------------------------------
# works with gcc
#
#CC	= gcc
#CFLAGS = -s -O2
#SYS	= -DUNIX

#------------------------------------------------
# create a debugging version for gcc and gdb
#
#CC	= gcc
#CFLAGS	= -DDEBUG -DDEBUG_UGLY -ggdb
#SYS	= -DUNIX

#--------------------------------------------------------------------
# avoid fooling around below this line
#--------------------------------------------------------------------

COMP	= $(CC) -o $@ $(SYS) -I. -c
LINK	= $(CC) -o $@ $(SYS) -I.
MOVE	= mv
COPY	= cp
DELETE	= rm -f

#--------------------------------------------------------------------
# do not touch anything below this line
#--------------------------------------------------------------------

#
# symbols for objects and executables
#
OBJ_TOOLS	= ugly/all_ugly.o hscprj/all_hscp.o
OBJ_HSC		= $(OBJ_TOOLS) hsclib/all_hscl.o hsc/all_hsc.o
OBJ_ALL		= $(OBJ_HSC)
EXE_ALL		= hsc/hsc hsctools/hscdepp hsctools/hscpitt

#
# compile all tools
#
all : $(EXE_ALL)

# implict rule for object-files
# (old-style implicit rule)
.c.o :
	$(COMP) $< $(CFLAGS)

hsc/hsc : hsc/all_hsc.o $(OBJ_HSC)
	$(LINK) $(OBJ_HSC) $(CFLAGS)

hsctools/hscdepp : $(OBJ_TOOLS) hsctools/all_depp.o 
	$(LINK) hsctools/all_depp.o $(OBJ_TOOLS) $(CFLAGS)

hsctools/hscpitt : $(OBJ_TOOLS) hsctools/all_pitt.o 
	$(LINK) hsctools/all_pitt.o $(OBJ_TOOLS) $(CFLAGS)

#--------------------------------------------------------------------
# installation
#--------------------------------------------------------------------
install : all
	$(INSTALL) -m 755 hsc/hsc $(INSTDIR)/bin/hsc
	$(INSTALL) -m 755 hsctools/hscdepp $(INSTDIR)/bin/hscdepp
	$(INSTALL) -m 755 hsctools/hscpitt $(INSTDIR)/bin/hscpitt
	$(INSTALL) -m 644 ../hsc.prefs $(INSTDIR)/lib/hsc.prefs


#--------------------------------------------------------------------
# cleanup - remove all objects and executables
#--------------------------------------------------------------------
clean :
	$(DELETE) hsc/*.o
	$(DELETE) hsctools/*.o
	$(DELETE) hsclib/*.o
	$(DELETE) hscprj/*.o
	$(DELETE) ugly/*.o

all_clean :
	$(DELETE) $(OBJ_ALL) $(EXE_ALL)

sterile : clean all_clean
	$(DELETE) hsc/hsc
	$(DELETE) hsctools/hscdepp
	$(DELETE) hsctools/hscpitt

#
# the dependencies below are created using `MkDepend' by Lars Düning,
# available from amient:dev/c/MkDepend.lha
#

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS FOLLOW ---
hsc/all_hsc.o : hsc/args.c hsc/callback.c hsc/global.c hsc/hsc.c \
    hsc/output.c hsc/status.c hsc/callback.h hsc/global.h hsc/status.h \
    hscprj/license.h ugly/fname.h ugly/prginfo.h ugly/returncd.h \
    ugly/uargs.h amiga/msgbrowser.c hsc/output.h riscos/msgbrowser.c \
    hsclib/hsclib.h hsc/args.h hsc/hsc_rev.h hscprj/project.h \
    riscos/unixname.h hsc/hdebug.h ugly/dllist.h ugly/expstr.h \
    ugly/infile.h ugly/umemory.h ugly/ustring.h ugly/utypes.h ugly/expstr.h \
    ugly/utypes.h ugly/umemory.h hsclib/inc_base.h hsclib/include.h \
    hsclib/linit.h ugly/dllist.h ugly/udebug.h hsclib/attrib.h \
    hsclib/entity.h hsclib/hscprc.h hsclib/ldebug.h hsclib/lmessage.h \
    hsclib/lstatus.h hsclib/msgid.h hsclib/tag.h hscprj/document.h

hsclib/all_hscl.o : hsclib/attrib.c hsclib/defattr.c hsclib/deftag.c \
    hsclib/entity.c hsclib/eval.c hsclib/hscprc.c hsclib/idref.c \
    hsclib/include.c hsclib/input.c hsclib/ldebug.h hsclib/linit.c \
    hsclib/lmessage.c hsclib/lstatus.c hsclib/msgid.h hsclib/parse.c \
    hsclib/posteval.c hsclib/size.c hsclib/skip.c hsclib/tag.c \
    hsclib/tag_a.c hsclib/tag_hsc.c hsclib/tag_if.c hsclib/tag_macro.c \
    hsclib/tag_misc.c hsclib/uri.c ugly/dllist.h ugly/expstr.h \
    ugly/infile.h ugly/umemory.h ugly/ustring.h ugly/utypes.h hsclib/eval.h \
    hsclib/inc_base.h hsclib/uri.h hsclib/defattr.h hsclib/input.h \
    hsclib/skip.h hsclib/entity.h hsclib/idref.h hsclib/tag_if.h \
    hscprj/project.h ugly/fname.h ugly/returncd.h ugly/ustrlist.h \
    hscprj/document.h hsclib/include.h hsclib/parse.h ugly/ufile.h \
    hsclib/deftag.h hsclib/tag_a.h hsclib/tag_hsc.h hsclib/tag_macro.h \
    hsclib/tag_misc.h hsclib/posteval.h hsclib/size.h hsclib/tag.h \
    hsclib/inc_tagcb.h ugly/utypes.h ugly/dllist.h ugly/expstr.h \
    ugly/udebug.h hsclib/attrib.h hsclib/hscprc.h hsclib/lmessage.h \
    hsclib/lstatus.h ugly/ustring.h

hscprj/all_hscp.o : hscprj/document.c hscprj/license.c hscprj/project.c \
    hscprj/readprj.c hscprj/writeprj.c hscprj/document.h ugly/utypes.h \
    hsclib/ldebug.h hscprj/pdebug.h hscprj/project.h ugly/dllist.h \
    ugly/expstr.h ugly/infile.h ugly/umemory.h ugly/ustring.h \
    hscprj/pdefs.h ugly/udebug.h ugly/utypes.h ugly/dllist.h ugly/expstr.h

hsctools/all_depp.o : hsctools/hscdepp.c hscprj/document.h hscprj/license.h \
    hscprj/project.h hsctools/depp_rev.h ugly/dllist.h ugly/expstr.h \
    ugly/infile.h ugly/prginfo.h ugly/returncd.h ugly/uargs.h \
    ugly/ustring.h hsclib/ldebug.h ugly/umemory.h ugly/utypes.h \
    ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/umemory.h ugly/udebug.h

hsctools/all_pitt.o : hsctools/hscpitt.c hscprj/document.h hscprj/license.h \
    hscprj/project.h hsctools/pitt_rev.h ugly/dllist.h ugly/expstr.h \
    ugly/infile.h ugly/prginfo.h ugly/returncd.h ugly/uargs.h ugly/ufile.h \
    ugly/ustring.h ugly/ustrlist.h hsclib/ldebug.h ugly/umemory.h \
    ugly/utypes.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/umemory.h \
    ugly/ustring.h ugly/udebug.h

ugly/all_ugly.o : ugly/dllist.c ugly/expstr.c ugly/fname.c ugly/infile.c \
    ugly/prginfo.c ugly/uargs.c ugly/udebug.h ugly/ufile.c ugly/umemory.c \
    ugly/ustring.c ugly/ustrlist.c ugly/dllist.h ugly/umemory.h \
    ugly/utypes.h ugly/expstr.h ugly/ustring.h ugly/fname.h ugly/infile.h \
    ugly/args_fre.c ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c \
    ugly/uargs.h ugly/ufile.h ugly/ustrlist.h ugly/udebug.h

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS PRECEDE ---
