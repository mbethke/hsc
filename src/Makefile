#
# makefile for "hsc - html sucks completely"
#
# (W) by Tommy-Saftwörx in 1995,96
# 

#
# compiler symbols
#
# sys	system define and system depandant compiler flags
# 	 -DAMIGA compile amiga version
#	 -noixemul disable ixemul.library for amiga-gcc
# 	 -DUNIX  compile unix version
# comp	compiler call to create object file
# link	compiler call to create executable
# debug	compiler flags & defines to create debugging version
# norm	.. unoptimised version
# optim .. optimised version
# mode  must be set to one of $(debug), $(norm) or $(optim)
# olhp  special options for LoadHscPrefs
#

#
# command symbols
#
# copy - a shell command that should accept two argument: source and
#	destination of a file to be copied
# delete - a shell command that should accept one argument: a pattern
#	of files to be removed (no need to work recursively)
#

# AMIGA commands
copy = copy QUIET
delete = delete

# UNIX commands
#copy = cp
#delete = rm

#
# ugly modules 
#
# sas/c-amiga: the modules are stored in SCOPTIONS, otherwise the command
#	calling the linker would be too long for #$%& Amiga-Shell..
#
# IMPORTANT: The modules for ugly/*.o have to be compiled for
# 	the same version as the hsc's *.o; otherwise, you will get
#	strange messages and results.
#

#obj_ugly = ugly/args.o ugly/dllist.o ugly/expstr.o ugly/fname.o ugly/infile.o\
#	ugly/memory.o ugly/prginfo.o ugly/string.o
#obj_ugly =

#**
#** cc (should work on most Unix-esh systems;
#**    but no optimisation is performed)
#**
#sys  = -DUNIX
#debug= -DDEBUG
#norm = 
#optim=
#comp = cc $(sys) -I. -o $@ -c
#link = cc $(sys) -I. -o $@

#**
#** gcc AMIGA
#**
#sys  = -DAMIGA -D_ANSI_SOURCE -Damigados -noixemul #-m68020

#**
#** gcc UNIX
#**
#sys  = -DUNIX -D_ANSI_SOURCE


#**
#** gcc (any system)
#**
#debug= -DDEBUG -ggdb
#norm = -ggdb
#optim= -O -s
#comp = gcc $(sys) -Wall -W -I. -o $@ -c
#link = gcc $(sys) -Wall -W -I. -o $@

#**
#** sas/c AMIGA
#**
sys  =
debug= DEBUG=SF DEF=DEBUG
norm = DEBUG=LINE
optim= STRIPDEBUG IGN=304 IGN=93 OPTIMIZE NOSTKCHK
olhp = STARTUP=cres
link = sc $(sys) LINK
comp = sc $(sys)


#
# compiler mode
# (norm/optim)
#
#mode = $(optim)
#mode = $(norm)
mode = $(debug)

#
# symbols for paths of sub-projects
#
# ldest - hsc library
# hdest - hsc preprocessor
# mdest - misc. test files

hdest = hsc/
ldest = hsclib/
mdest = misc/

#---------------------------------
#
# usually, there should be no need
# to change anything below this line.
#
#---------------------------------

obj_tags =\
	hsclib/tag_a.o \
	hsclib/tag_hsc.o \
	hsclib/tag_if.o \
	hsclib/tag_macr.o \
	hsclib/tag_misc.o

obj_hsclib =\
	hsclib/attribute.o \
	hsclib/defattr.o \
	hsclib/deftag.o \
	hsclib/entity.o \
	hsclib/message.o \
	hsclib/eval.o \
	hsclib/hscprc.o \
	hsclib/id.o \
	hsclib/include.o \
	hsclib/init.o \
	hsclib/input.o \
	hsclib/parse.o \
	hsclib/project.o \
	hsclib/size.o \
	hsclib/skip.o \
	hsclib/status.o \
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

#
# compile all tools
#
all : hsclib \
	hsc

#
# cleanup - remove all objects and executables
#
clean :
	$(delete) bin/*
	$(delete) hsclib/*.o
	$(delete) hsc/*.o
	$(delete) LoadHscPrefs/*.o
	$(delete) ugly/*.o

#
# ugly - authors personal function library
#
# change to "ugly/" and compile the objects
# using the local Makefile there.
#
uglylib :
	make -C ugly 

#
# hsclib - functions to process hsc-data
#

#headers used by "hsclib/inc_base.h"
head_incbase = \
	hsclib/attribute.h \
	hsclib/entity.h \
	hsclib/hscprc.h \
	hsclib/message.h \
	hsclib/status.h \
	hsclib/tag.h \

# headers used by tag-callbacks
head_tagcb = \
	$(head_incbase) \
	hsclib/input.h \
	hsclib/skip.h \

hsclib : $(obj_hsclib)

hsclib/attribute.o : $(head_incbase) hsclib/attribute.c
	$(comp) $(mode) hsclib/attribute.c

hsclib/cleanup.o : $(head_incbase) hsclib/cleanup.c
	$(comp) $(mode) hsclib/cleanup.c

hsclib/defattr.o : $(head_incbase) hsclib/defattr.c hsclib/eval.h
	$(comp) $(mode) hsclib/defattr.c

hsclib/deftag.o : $(head_incbase) hsclib/deftag.c hsclib/deftag.h \
		hsclib/defattr.h hsclib/eval.h \
		hsclib/input.h hsclib/skip.h
	$(comp) $(mode) hsclib/deftag.c

hsclib/entity.o : $(head_incbase) hsclib/entity.c
	$(comp) $(mode) hsclib/entity.c

hsclib/message.o : $(head_incbase) hsclib/message.c
	$(comp) $(mode) hsclib/message.c

hsclib/eval.o : $(head_incbase) hsclib/eval.c hsclib/eval.h \
		hsclib/input.h hsclib/skip.h hsclib/uri.h
	$(comp) $(mode) hsclib/eval.c

hsclib/hscprc.o : $(head_incbase) hsclib/hscprc.c hsclib/hscprc.h
	$(comp) $(mode) hsclib/hscprc.c

hsclib/include.o : $(head_incbase) hsclib/include.c
	$(comp) $(mode) hsclib/include.c

hsclib/id.o : $(head_incbase) hsclib/id.c
	$(comp) $(mode) hsclib/id.c

hsclib/init.o : $(head_incbase) hsclib/init.c \
		hsclib/deftag.h hsclib/include.h hsclib/parse.h \
		hsclib/tag_a.h hsclib/tag_hsc.h hsclib/tag_if.h \
		hsclib/tag_macr.h hsclib/tag_misc.h
	$(comp) $(mode) hsclib/init.c

hsclib/input.o : $(head_incbase) hsclib/input.c
	$(comp) $(mode) hsclib/input.c

hsclib/parse.o : $(head_incbase) hsclib/parse.c hsclib/parse.h \
		hsclib/deftag.h hsclib/id.h hsclib/input.h \
		hsclib/project.h \
		hsclib/size.h hsclib/skip.h hsclib/uri.h
	$(comp) $(mode) hsclib/parse.c

hsclib/project.o : $(head_incbase) hsclib/project.c hsclib/id.h
	$(comp) $(mode) hsclib/project.c

hsclib/size.o : $(head_incbase) hsclib/size.c hsclib/uri.h
	$(comp) $(mode) hsclib/size.c

hsclib/skip.o : $(head_incbase) hsclib/skip.c hsclib/skip.h
	$(comp) $(mode) hsclib/skip.c

hsclib/status.o : $(head_incbase) hsclib/status.c
	$(comp) $(mode) hsclib/status.c

hsclib/tag.o : $(head_incbase) hsclib/tag.c
	$(comp) $(mode) hsclib/tag.c

hsclib/tag_a.o : $(head_tagcb) hsclib/tag_a.c
	$(comp) $(mode) hsclib/tag_a.c 

hsclib/tag_hsc.o : $(head_tagcb) hsclib/tag_hsc.c \
		hsclib/defattr.h hsclib/deftag.h hsclib/eval.h \
		hsclib/include.h hsclib/parse.h \
		hsclib/tag_macr.h hsclib/tag_if.h
	$(comp) $(mode) hsclib/tag_hsc.c

hsclib/tag_if.o : $(head_tagcb) hsclib/tag_if.c hsclib/parse.h
	$(comp) $(mode) hsclib/tag_if.c

hsclib/tag_macr.o : $(head_tagcb) hsclib/tag_macr.c \
		hsclib/defattr.h hsclib/deftag.h
	$(comp) $(mode) hsclib/tag_macr.c

hsclib/tag_misc.o : $(head_tagcb) hsclib/tag_misc.c \
		hsclib/parse.h 
	$(comp) $(mode) hsclib/tag_misc.c

hsclib/uri.o : $(head_incbase) hsclib/uri.c hsclib/id.h hsclib/project.h
	$(comp) $(mode) hsclib/uri.c


#
# hsc - the main preprocessor
#
hsc_incbase = hsc/global.h hsclib/hscprc.h 

hsc : hsc/hsc

hsc/hsc : $(obj_hsclib) $(obj_hsc) hsc/hsc_rev.h
	$(link) $(sys) $(mode) \
		$(obj_hsc) $(obj_hsclib) $(obj_ugly)

hsc/hsc.o : $(hsc_incbase) hsc/hsc.c hsc/args.h \
		hsc/output.h hsc/status.h
	$(comp) $(mode) hsc/hsc.c

hsc/args.o : $(hsc_incbase) hsc/args.c hsc/status.h
	$(comp) $(mode) hsc/args.c

hsc/callback.o : $(hsc_incbase) hsc/callback.c hsc/status.h
	$(comp) $(mode) hsc/callback.c

#hsc/error.o : $(hsc_incbase) hsc/error.c
#	$(comp) $(mode) hsc/error.c
#
hsc/global.o : $(inc_base) hsc/global.c
	$(comp) $(mode) hsc/global.c

hsc/output.o : $(hsc_incbase) hsc/output.c
	$(comp) $(mode) hsc/output.c

hsc/status.o : $(hsc_incbase) hsc/status.c
	$(comp) $(mode) hsc/status.c

#
# LoadHscPrefs - Amiga only tool to load hsc.prefs permanently
#
LoadHscPrefs/LoadHscPrefs.o : LoadHscPrefs/LoadHscPrefs.c
	$(comp) $(mode) LoadHscPrefs/LoadHscPrefs.c

LoadHscPrefs/LoadHscPrefs : LoadHscPrefs/LoadHscPrefs.o $(obj_hsclib)
	$(link) $(olhp) $(sys) $(mode) LoadHscPrefs/LoadHscPrefs.o \
		$(obj_hsclib) $(obj_ugly)

lpre : LoadHscPrefs/LoadHscPrefs

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
dist_dir = prog:hsc/distrib/hsc/

dist : 
	$(copy) hsc/hsc $(dist_dir)

dbg :
	$(copy) dbg.prefs hsc.prefs

std :
	$(copy) std.prefs hsc.prefs

#
# bump version (perverted 'cause I'm too stupid to 
# use RCS without frequently trashing my sources..)
#
hscvers : 
	@echo "6" >hsc_rev.beta
	RevUp 0 hsc REV 9 BETA C EXTRA 
	@move hsc_rev.* hsc/ quiet clone
	@type hsc/hsc_rev.h
#
# some experimental stuff
#
$(mdest)gifsize : $(mdest)gifsize.c
	sc LINK $(mdest)gifsize.c PNAME=gifsize IGN=93

$(mdest)jfifsize : $(mdest)jfifsize.c
	sc LINK $(mdest)jfifsize.c PNAME=jfifsize IGN=93

$(mdest)showargs : $(mdest)showargs.c
	sc LINK $(mdest)showargs.c PNAME=showargs IGN=93

$(mdest)now : $(mdest)now.c
	sc LINK $(mdest)now.c

# EOF (Makefile)
