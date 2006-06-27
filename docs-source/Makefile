#
# Makefile for the hsc manual
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

#
# To compile the whole manual unders non-Amiga-OS, you have to perform the 
# following steps before the Unix parent directories are treated correctly
# by this Makefile:
#
# - Go to the part where the dependencies created by hscdepp start (quite 
#   at the end of this file) and replace all " /" by " /../". Note the
#   blanks at the biginning of these two search/replace expressions.
# - Activate the "PARENT = ../" symbol below.
# - In the shell, enter "hscpitt new" to clear the project file
# - For your convenience, change the IGNORE symbol below to
#   "IGNORE = ign=46 ign=51" as the project data can not yet be
#   validated by hsc.
#

#PARENT	= /
PARENT	= ../

DESTDIR = $(PARENT)docs/
IGNORE  = ign="46|51"
PRJFILE = hsc.project
STDINC  = inc/macro.hsc inc/webpage.hsc inc/menus.hsc
HSCMISC	= rplcent getsize compact msgmode=pedantic prefsfile=$(PARENT)hsc.prefs \
	  quotestyle=double maxmsg=0 maxerr=0 lowercasetags \
	  DEF PROJECT_STYLEFILE:string=":hscdoc.css"

HSCFLAGS= $(HSCMISC) $(IGNORE) $(STDINC) $(HSCEXTRAOPTS) status=quiet
HSC	= hsc

HSCDEPP = hscdepp
HSCPITT = hscpitt
DELALL	= rm -Rf

.PHONY: all all_hsc clean sterile distclean depend rebuild

#
# rule to update whole docs
#
all : dirs images css all_hsc

clean:
	$(DELALL) $(DESTDIR)/*

sterile: distclean
distclean: clean
	$(DELALL) Makefile Makefile.bak

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
ifdef VERBOSE
	$(HSC) inc/html.hsc $(HSCFLAGS) to=$(DESTDIR) prjfile=$(PRJFILE) $<
	chmod ugo+r $<
else
	@echo $@
	@$(HSC) inc/html.hsc $(HSCFLAGS) to=$(DESTDIR) prjfile=$(PRJFILE) $<
	@chmod ugo+r $<
endif
endif
#	@CheckHTML $@ QUIET

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

# Create all destination directories
dirs:
	for d in `find * -type d`; do test -d $(DESTDIR)$$d || mkdir $(DESTDIR)$$d; done

# Copy images over to destination if required
IMAGES=$(addprefix $(DESTDIR), $(wildcard image/*))

$(IMAGES): $(DESTDIR)image/%: image/%
	cp $< $@

images: $(IMAGES)

css: ../docs/hscdoc.css

../docs/hscdoc.css: hscdoc.css
	cp $< $@

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies follow ---

# dependencies updated: Saturday 17-Sep-2005 18:07:58

all_hsc : ../docs/macro/flag.html ../docs/macro/flag.html \
    ../docs/macro/flag.html ../docs/macro/flag.html \
    ../docs/message-list.html ../docs/macro/flag.html \
    ../docs/message-list.html ../docs/macro/flag.html \
    ../docs/message-list.html ../docs/macro/flag.html \
    ../docs/macro/attrib.html ../docs/questions.html ../docs/options.html \
    ../docs/message-list.html ../docs/distrib.html ../docs/macro/flag.html \
    ../docs/macro/attrib.html ../docs/questions.html ../docs/options.html \
    ../docs/message-list.html ../docs/index.html \
    ../docs/features/expressions.html ../docs/about.html \
    ../docs/project/make.html ../docs/examples.html ../docs/future.html \
    ../docs/copy.html ../docs/project/hscpitt.html \
    ../docs/features/getsize.html ../docs/features/strip.html \
    ../docs/teutsch.html ../docs/others.html ../docs/install.html \
    ../docs/features/if.html ../docs/source.html ../docs/features/exec.html \
    ../docs/features/syntax.html ../docs/features/assign.html \
    ../docs/features/spcattr.html ../docs/ports.html ../docs/envvar.html \
    ../docs/fileargs.html ../docs/features/rplcent.html \
    ../docs/features/uris.html ../docs/project/prjfile.html \
    ../docs/project/index.html ../docs/features/checkuri.html \
    ../docs/macro/macros.html ../docs/features/prefs.html \
    ../docs/features/spctags.html ../docs/messages.html ../docs/bugs.html \
    ../docs/project/hscpaltrow.html ../docs/related.html \
    ../docs/updates.html ../docs/project/makefile.html ../docs/require.html \
    ../docs/usecases.html ../docs/author.html ../docs/changes.html \
    ../docs/project/hscdepp.html ../docs/undocumented.html \
    ../docs/features/css.html

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/message-list.html : message-list.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/message-list.html : message-list.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/message-list.html : message-list.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/attrib.html : macro/attrib.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/questions.html : questions.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/options.html : options.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/message-list.html : message-list.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/distrib.html : distrib.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/flag.html : macro/flag.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/attrib.html : macro/attrib.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/questions.html : questions.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/options.html : options.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/message-list.html : message-list.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/index.html : index.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/expressions.html : features/expressions.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/about.html : about.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/make.html : project/make.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/examples.html : examples.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/future.html : future.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/copy.html : copy.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/hscpitt.html : project/hscpitt.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/getsize.html : features/getsize.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/strip.html : features/strip.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/teutsch.html : teutsch.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/others.html : others.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/install.html : install.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/if.html : features/if.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/source.html : source.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/exec.html : features/exec.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc exmpl/exec.hsc \
    exmpl/exec.hsc

../docs/features/syntax.html : features/syntax.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/assign.html : features/assign.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/spcattr.html : features/spcattr.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc \
    exmpl/anchor.hsc exmpl/anchor.hsc

../docs/ports.html : ports.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/envvar.html : envvar.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/fileargs.html : fileargs.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/rplcent.html : features/rplcent.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/uris.html : features/uris.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/prjfile.html : project/prjfile.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/index.html : project/index.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/checkuri.html : features/checkuri.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/macro/macros.html : macro/macros.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc \
    exmpl/macro_addr.hsc exmpl/macro_addr.hsc exmpl/macro_file.hsc \
    exmpl/macro_cntnest.hsc exmpl/macro_cntnest.hsc \
    exmpl/macro_cnt2nest.hsc exmpl/macro_cnt2nest.hsc exmpl/macro_next.hsc \
    exmpl/macro_next.hsc exmpl/macro_locale.hsc exmpl/macro_locale.hsc

../docs/features/prefs.html : features/prefs.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/spctags.html : features/spctags.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/messages.html : messages.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/bugs.html : bugs.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/hscpaltrow.html : project/hscpaltrow.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/related.html : related.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/updates.html : updates.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/project/makefile.html : project/makefile.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/require.html : require.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/usecases.html : usecases.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/author.html : author.hsc inc/macros.hsc inc/html.hsc inc/macro.hsc \
    inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/changes.html : changes.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc CHANGES

../docs/project/hscdepp.html : project/hscdepp.hsc inc/macros.hsc \
    inc/html.hsc inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/undocumented.html : undocumented.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

../docs/features/css.html : features/css.hsc inc/macros.hsc inc/html.hsc \
    inc/macro.hsc inc/webpage.hsc inc/menus.hsc menu.hsc

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies precede ---
