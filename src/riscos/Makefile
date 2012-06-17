#
# Makefile for hsc ("html sucks completely")
#
# Copyright (C) 1994,95,96  Thomas Aglassinger
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
# This Makefile should work on most Unixoid systems, but no
# optimisation is performed. Feel free to fool around with the
# compiler-flags.
#
# (1) Use "make -f Makefile.dodl" to compile.
#
# (2) Copy "hsc/hsc", "hsctools/hscdepp" and "hsctools/hscpitt" to
#     anywhere in your search-path.
#
# (3) Use "make -f Makefile.dodl sterile" for a total cleanup. Maybe
#     you also have to pass an option to make to ignore the
#     return-code of the "rm"-command; usually, this is "-i".
#
# (4) Read the docs, section "Installation" what to do with
#     "hsc.prefs".
#
# IMPORTANT: Don't forget to hit the return-key after typing commands
#            in the shell!
#

#------------------------------------------------
# this one should always work
#
#CC	= cc
#CFLAGS	=

#------------------------------------------------
# this one should also work and even optimise
#
CC	= cc
CFLAGS	= -Otime

#------------------------------------------------
# create a debugging version
#
#CC	= cc
#CFLAGS	= -DDEBUG -DDEBUG_UGLY -g

#------------------------------------------------
# works with gcc
#
#CC	= gcc
#CFLAGS = -s -O2

#------------------------------------------------
# create a debugging version for gcc and gdb
#
#CC	= gcc
#CFLAGS	= -DDEBUG -DDEBUG_UGLY -ggdb

#--------------------------------------------------------------------
# avoid fooling around below this line
#--------------------------------------------------------------------

SYS	= -DRISCOS -throwback
COMP	= $(CC) -o $@ $(SYS) -I. -c
LINK	= link -o $@
MOVE	= rename
COPY	= copy
DELETE	= wipe
SQUEEZE = squeeze

#--------------------------------------------------------------------
# do not touch anything below this line
#--------------------------------------------------------------------

#
# symbols for objects and executables
#
OBJ_TOOLS	= ugly.o.all_ugly hscprj.o.all_hscp riscos.o.unixname riscos.o.UnixStubs
OBJ_HSC		= $(OBJ_TOOLS) hsclib.o.all_hscl hsc.o.all_hsc
OBJ_ALL		= $(OBJ_HSC)
EXE_ALL		= hsc.all_hsc hsctools.all_depp hsctools.all_pitt

#
# compile all tools
#
all : $(EXE_ALL)

# implict rule for object-files
# (old-style implicit rule)
.c.o :
	$(COMP) $*.c $(CFLAGS)

hsc/all_hsc : $(OBJ_HSC)
	$(LINK) $(OBJ_HSC) $(CMODE)
	$(SQUEEZE) hsc.all_hsc ^.hsc

hsctools/all_depp : $(OBJ_TOOLS) hsctools.o.all_depp
	$(LINK) hsctools.o.all_depp $(OBJ_TOOLS) $(CMODE)
	$(SQUEEZE) hsctools.all_depp ^.hscdepp

hsctools/all_pitt : $(OBJ_TOOLS) hsctools.o.all_pitt
	$(LINK) hsctools.o.all_pitt $(OBJ_TOOLS) $(CMODE)
	$(SQUEEZE) hsctools.all_pitt ^.hscpitt

### UnixName stuff ###
riscos/unixname.o: riscos/unixname.c riscos/unixname.h

#
# cleanup - remove all objects and executables
#
clean :
	create hsc.o.void 0
	wipe hsc.o.* V~C
	create hsctools.o.void 0
	wipe hsctools.o.* V~C
	create hsclib.o.void 0
	wipe hsclib.o.* V~C
	create hscprj.o.void 0
	wipe hscprj.o.* V~C
	create ugly.o.void 0
	wipe ugly.o.* V~C
	wipe riscos.o.unixname V~C

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
    hsc/output.c hsc/status.c hsc/global.h hsc/status.h hscprj/license.h \
    ugly/fname.h ugly/prginfo.h ugly/returncd.h ugly/uargs.h \
    amiga/msgbrowser.c hsc/callback.h hsc/output.h riscos/msgbrowser.c \
    hsclib/hsclib.h hsc/args.h hsc/hsc_rev.h hscprj/project.h \
    riscos/unixname.h hsc/hdebug.h ugly/dllist.h ugly/expstr.h \
    ugly/infile.h ugly/umemory.h ugly/ustring.h ugly/utypes.h ugly/expstr.h \
    ugly/utypes.h ugly/umemory.h hsclib/inc_base.h hsclib/include.h \
    hsclib/linit.h ugly/dllist.h ugly/udebug.h hsclib/attrib.h \
    hsclib/entity.h hsclib/hscprc.h hsclib/ldebug.h hsclib/lmessage.h \
    hsclib/lstatus.h hsclib/msgid.h hsclib/tag.h hscprj/document.h

hsclib/all_hscl.o : hsclib/tag_macro.c hsclib/tag_if.c hsclib/tag_hsc.c \
    hsclib/tag_a.c hsclib/tag_misc.c hsclib/size.c hsclib/linit.c \
    hsclib/include.c hsclib/parse.c hsclib/deftag.c hsclib/defattr.c \
    hsclib/posteval.c hsclib/eval.c hsclib/uri.c hsclib/skip.c \
    hsclib/input.c hsclib/lstatus.c hsclib/hscprc.c hsclib/idref.c \
    hsclib/attrib.c hsclib/tag.c hsclib/entity.c hsclib/lmessage.c \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hsclib/msgid.h hsclib/ldebug.h hsclib/parse.h \
    hsclib/include.h hsclib/deftag.h hsclib/defattr.h hsclib/inc_tagcb.h \
    hsclib/skip.h hsclib/eval.h hsclib/tag_if.h hsclib/tag_macro.h \
    hsclib/uri.h ugly/fname.h hscprj/document.h hsclib/inc_base.h \
    hsclib/tag_misc.h hsclib/tag_hsc.h hsclib/tag_a.h hsclib/input.h \
    ugly/ufile.h hsclib/size.h hscprj/project.h hsclib/posteval.h \
    hsclib/idref.h ugly/returncd.h ugly/ustrlist.h hsclib/tag.h \
    hsclib/entity.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h \
    hsclib/hscprc.h hsclib/lstatus.h hsclib/lmessage.h hsclib/attrib.h \
    ugly/ustring.h

hscprj/all_hscp.o : hscprj/writeprj.c hscprj/readprj.c hscprj/project.c \
    hscprj/license.c hscprj/document.c hscprj/project.h hscprj/document.h \
    ugly/ustring.h ugly/infile.h ugly/umemory.h ugly/expstr.h ugly/dllist.h \
    ugly/utypes.h hscprj/pdefs.h hscprj/pdebug.h hsclib/ldebug.h \
    ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsctools/all_depp.o : hsctools/hscdepp.c hscprj/license.h hscprj/project.h \
    hscprj/document.h ugly/returncd.h ugly/prginfo.h ugly/uargs.h \
    ugly/infile.h ugly/expstr.h ugly/dllist.h ugly/ustring.h \
    hsctools/depp_rev.h ugly/umemory.h ugly/utypes.h hsclib/ldebug.h \
    ugly/umemory.h ugly/utypes.h ugly/dllist.h ugly/expstr.h ugly/udebug.h

hsctools/all_pitt.o : hsctools/hscpitt.c hscprj/license.h hscprj/project.h \
    hscprj/document.h ugly/returncd.h ugly/prginfo.h ugly/ustrlist.h \
    ugly/uargs.h ugly/infile.h ugly/expstr.h ugly/dllist.h ugly/ustring.h \
    hsctools/pitt_rev.h ugly/umemory.h ugly/utypes.h hsclib/ldebug.h \
    ugly/dllist.h ugly/ustring.h ugly/utypes.h ugly/umemory.h ugly/expstr.h \
    ugly/udebug.h

ugly/all_ugly.o : ugly/prginfo.c ugly/infile.c ugly/ustrlist.c ugly/uargs.c \
    ugly/ufile.c ugly/dllist.c ugly/fname.c ugly/expstr.c ugly/ustring.c \
    ugly/umemory.c ugly/udebug.h ugly/utypes.h ugly/umemory.h ugly/infile.h \
    ugly/fname.h ugly/ustring.h ugly/expstr.h ugly/ustrlist.h \
    ugly/args_hlp.c ugly/args_prp.c ugly/args_set.c ugly/args_fre.c \
    ugly/uargs.h ugly/dllist.h ugly/ufile.h ugly/udebug.h

# --- DO NOT MODIFY THIS LINE -- AUTO-DEPENDS PRECEDE ---

# Dynamic dependencies:
o.unixname:	c.unixname
o.unixname:	h.unixname
