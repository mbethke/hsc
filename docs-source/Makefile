#
# Makefile for hsc-documentation
#

#
# DESTDIR  - destination directory (relative)
# IGNORE   - messages to be ignored
# PRJFILE  - project file
# STDINC   - standard includes for all sources
# HSCMISC  - miscellaneous flags and options
# 
# HSC      - shell command that invokes hsc
# HSCFLAGS - hsc options
#
# HSCDEPP  - shell command to invoke hscdepp
# HSCPITT  - shell command to invoke hscpitt
#

PARENT	= ../
#PARENT	= /
DESTDIR = $(PARENT)docs/
IGNORE  =ign=46
PRJFILE =hsc.project
STDINC  =inc/macro.hsc inc/webpage.hsc
HSCMISC	=rplcent getsize compact mode=pedantic prefsfile=$(PARENT)hsc.prefs #quotemode=double

HSC     =$(PARENT)hsc
HSCFLAGS=$(HSCMISC) $(IGNORE) $(STDINC) \
		msgansi status=quiet

HSCDEPP = $(PARENT)hscdepp
HSCPITT = $(PARENT)hscpitt

.PHONY: all all_hsc depend rebuild

#
# rule to update whole docs
#
all : all_hsc $(PARENT)/index.html

#
# pattern rule for html-files
#
# The lines below will do the following:
# - View name of document currently processing
# - Create html-document
# - Validate document just created using CheckHTML 
#
# If the symbols `PS' has been defined, a version prepared for
# html2ps will be created.
#
$(DESTDIR)%.html : %.hsc
ifdef PS
	@echo ps:$@
	@$(HSC) inc/ps.hsc   $(HSCFLAGS) to=$(DESTDIR) $<
else
	@echo $@
	@$(HSC) inc/html.hsc $(HSCFLAGS) to=$(DESTDIR) prjfile=$(PRJFILE) $<
endif
	@CheckHTML $@ QUIET

#
# update dependencies
#
depend :
	$(HSCDEPP) file=Makefile prjfile=$(PRJFILE) verbose

#
# rebuild all documents
#
rebuild :
	$(MAKE) -W inc/macro.hsc

#
# rebuild PostScript-version
#
rebuild_ps :
	$(MAKE) -W inc/macro.hsc PS=1

#
# create PostScript documentation
#
ps :
	ksh -c "perl /bin/html2ps -W l -f html2ps.config -o ../../hsc.ps ../docs/index.html"
#	ksh -c "perl /bin/html2ps -O -f html2ps.config -o ../../hsc.ps ../docs/features/getsize.html"

#
# main page
#
$(PARENT)index.html : main_index.hsc
	$(HSC) inc/html.hsc to=$@ $(HSCMISC) $<
	@CheckHTML $@ QUIET

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies follow ---

# dependencies updated: Sunday 11-Jan-1998 22:46:37

all_hsc : ../docs/about.html ../docs/macro/attrib.html \
    ../docs/project/make.html ../docs/examples.html \
    ../docs/features/rplcent.html ../docs/project/index.html \
    ../docs/features/checkuri.html ../docs/envvar.html ../docs/options.html \
    ../docs/fileargs.html ../docs/macro/macros.html ../docs/future.html \
    ../docs/messages.html ../docs/macro/flag.html ../docs/copy.html \
    ../docs/distrib.html ../docs/features/uris.html \
    ../docs/project/hscpitt.html ../docs/features/getsize.html \
    ../docs/project/prjfile.html ../docs/project/makefile.html \
    ../docs/project/hscdepp.html ../docs/features/spcattr.html \
    ../docs/features/strip.html ../docs/features/spctags.html \
    ../docs/related.html ../docs/source.html ../docs/teutsch.html \
    ../docs/features/expressions.html ../docs/undocumented.html \
    ../docs/others.html ../docs/bugs.html ../docs/ports.html \
    ../docs/install.html ../docs/features/exec.html \
    ../docs/features/if.html ../docs/features/prefs.html \
    ../docs/features/syntax.html ../docs/require.html \
    ../docs/features/assign.html ../docs/author.html ../docs/updates.html \
    ../docs/changes.html ../docs/questions.html \
    ../docs/project/hscpaltrow.html ../docs/index.html \
    ../docs/usecases.html

../docs/about.html : about.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/macro/attrib.html : macro/attrib.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/project/make.html : project/make.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/examples.html : examples.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/rplcent.html : features/rplcent.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/project/index.html : project/index.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/checkuri.html : features/checkuri.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/envvar.html : envvar.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/options.html : options.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/fileargs.html : fileargs.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/macro/macros.html : macro/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc exmpl/macro_addr.hsc exmpl/macro_addr.hsc \
    exmpl/macro_file.hsc exmpl/macro_cntnest.hsc exmpl/macro_cntnest.hsc \
    exmpl/macro_cnt2nest.hsc exmpl/macro_cnt2nest.hsc exmpl/macro_next.hsc \
    exmpl/macro_next.hsc exmpl/macro_locale.hsc exmpl/macro_locale.hsc

../docs/future.html : future.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/messages.html : messages.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/macro/flag.html : macro/flag.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/copy.html : copy.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/distrib.html : distrib.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/uris.html : features/uris.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/project/hscpitt.html : project/hscpitt.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/features/getsize.html : features/getsize.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/project/prjfile.html : project/prjfile.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/project/makefile.html : project/makefile.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/project/hscdepp.html : project/hscdepp.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/features/spcattr.html : features/spcattr.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc exmpl/anchor.hsc exmpl/anchor.hsc

../docs/features/strip.html : features/strip.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/spctags.html : features/spctags.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/related.html : related.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/source.html : source.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/teutsch.html : teutsch.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/expressions.html : features/expressions.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/undocumented.html : undocumented.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/others.html : others.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/bugs.html : bugs.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/ports.html : ports.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/install.html : install.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/exec.html : features/exec.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc exmpl/exec.hsc exmpl/exec.hsc

../docs/features/if.html : features/if.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/prefs.html : features/prefs.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/syntax.html : features/syntax.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/require.html : require.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/features/assign.html : features/assign.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/author.html : author.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/updates.html : updates.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/changes.html : changes.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/questions.html : questions.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

../docs/project/hscpaltrow.html : project/hscpaltrow.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc

../docs/index.html : index.hsc inc/html.hsc inc/macro.hsc inc/webpage.hsc

../docs/usecases.html : usecases.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies precede ---
