##
## makefile for "html sucks completely"
##
## (W) by Tommy-Saftworx in 1995
## 

#
# compiler symbols
#

#**
#** sas/c AMIGA
#**
sys  = DEF=AMIGA
dest = t:
norm = $(sys) DEF=UMEM_TRACKING DBG=SF OBJNAME=$(dest) DEF=DEBUG
optim= $(sys) NODBG STRIPDEBUG SMALLCODE OPT OPTSIZE IGN=304
link = sc LINK
comp = sc
ign  =


#**
#** gcc AMIGA
#**
#dest =
#sys  = -DAMIGA -noixemul

#**
#** gcc UNIX
#**
#dest =
#sys  = -DUNIX


#**
#** gcc (any system)
#**
#norm = $(sys) -DUMEM_TRACKING -ansi -ggdb
#optim= $(sys) -ansi -O
#comp = gcc -c
#link = gcc -o $@
#ign  =

#
# compiler mode
# (norm/optim)
#
mode = $(optim)
#mode = $(norm)

#
# copy symbol
#
copy1 = copy $(dest)hsc prog:hsc				# AMIGA
copy2 = copy $(dest)hsc c:hsc
copy3 = copy $(dest)hsc arc/hsc/hsc
#copy1 = 							# UNIX
#copy2 =
#copy3 =

#---------------------------------

#
# ugly modules
#
ugly_mod= ugly/args.o ugly/dllist.o ugly/expstr.o ugly/fname.o ugly/infile.o ugly/memory.o ugly/prginfo.o ugly/string.o

tag_handles = $(dest)tag_a.o $(dest)tag_hsc.o $(dest)tag_if.o $(dest)tag_macr.o $(dest)tag_misc.o

all : hsc

#
# compile hsc
#
#

hsc : $(dest)hsc.o $(dest)args.o $(dest)cleanup.o $(dest)config.o $(dest)entity.o $(dest)error.o\
	    $(dest)deftag.o $(dest)eval.o $(dest)global.o $(dest)output.o $(dest)parse.o\
	    $(dest)skip.o $(dest)status.o $(dest)tagargs.o $(dest)tag.o $(dest)vars.o $(dest)uri.o\
	    $(tag_handles) $(ugly_mod)
	$(link) $(sys) $(dest)hsc.o $(dest)args.o $(dest)cleanup.o $(dest)config.o $(dest)deftag.o\
	 $(dest)entity.o $(dest)error.o $(dest)eval.o $(dest)global.o $(dest)output.o $(dest)parse.o\
	 $(dest)skip.o $(dest)status.o $(dest)tagargs.o $(dest)tag.o $(dest)vars.o $(dest)uri.o\
	 $(tag_handles) $(ugly_mod)
	$(copy1)
	$(copy2)
	$(copy3)

#test_file : test_file.c $(ugly_mod)
#	$(comp) $(mode) test_file.c $(ugly_mod) LINK

$(dest)hsc.o : hsc.c args.h config.h cleanup.h error.h global.h output.h parse.h status.h
	$(comp) $(mode) hsc.c $(ign)

$(dest)args.o : args.c global.h error.h
	$(comp) $(mode) args.c

$(dest)cleanup.o : cleanup.c entity.h global.h tag.h vars.h
	$(comp) $(mode) cleanup.c

$(dest)config.o : config.c entity.h error.h global.h msgid.h tag.h tag_macr.h tag_hsc.h vars.h
	$(comp) $(mode) config.c

$(dest)deftag.o : deftag.c error.h global.h vars.h
	$(comp) $(mode) deftag.c

$(dest)entity.o : entity.c global.h error.h
	$(comp) $(mode) entity.c

$(dest)error.o : error.c global.h
	$(comp) $(mode) error.c

$(dest)eval.o : eval.c global.h vars.h
	$(comp) $(mode) eval.c

$(dest)global.o : global.c
	$(comp) $(mode) global.c

$(dest)output.o : output.c global.h error.h
	$(comp) $(mode) output.c

$(dest)parse.o : parse.c global.h entity.h error.h output.h skip.h status.h tag.h vars.h
	$(comp) $(mode) parse.c

$(dest)skip.o : skip.c global.h
	$(comp) $(mode) skip.c

$(dest)status.o : status.c global.h
	$(comp) $(mode) status.c

$(dest)tag.o : tag.c error.h global.h vars.h
	$(comp) $(mode) tag.c

$(dest)tagargs.o : tagargs.c global.h error.h
	$(comp) $(mode) tagargs.c

$(dest)tag_a.o : tag_a.c global.h error.h tagargs.h
	$(comp) $(mode) tag_a.c 

$(dest)tag_macr.o : tag_macr.c global.h error.h skip.h tag.h tagargs.h
	$(comp) $(mode) tag_macr.c

$(dest)tag_misc.o : tag_misc.c global.h error.h skip.h tagargs.h vars.h
	$(comp) $(mode) tag_misc.c

$(dest)tag_hsc.o : tag_hsc.c global.h error.h parse.h skip.h tag.h tagargs.h vars.h
	$(comp) $(mode) tag_hsc.c

$(dest)tag_if.o : tag_if.c global.h error.h parse.h skip.h tag.h tagargs.h vars.h
	$(comp) $(mode) tag_if.c

$(dest)vars.o : vars.c global.h error.h
	$(comp) $(mode) vars.c

$(dest)uri.o : uri.c global.h error.h
	$(comp) $(mode) uri.c

## EOF
