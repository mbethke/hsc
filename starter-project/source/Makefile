#
# Makefile for hsc-projects
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
# HSCDEPP  - shell command that invokes hscdepp
#

DESTDIR	=/html/
#DESTIR	=../html/		# weenix
IGNORE	=ign=46
PRJFILE	=hsc.project
#STDINC	=inc/my_macros.hsc
HSCMISC	=rplcent

HSC	=hsc
HSCFLAGS=$(HSCMISC) $(IGNORE) prjfile=$(PRJFILE) to=$(DESTDIR) $(STDINC)

HSCDEPP	=hscdepp

#
# rule to update whole docs
# (all_hsc will be created by hscdepp)
#
all : all_hsc

#
# implicit rule for html-files
#
$(DESTDIR)%.html : %.hsc
	$(HSC) $(HSCFLAGS) $<

#
# update dependencies
#
depend :
	$(HSCDEPP) file=Makefile prjfile=$(PRJFILE) verbose

#
# add new file / change includes of existing file
#
# example: make NEW FILE=new.hsc "INCLUDE=inc/macro.hsc inc/sepp.hsc"
#
NEW :
ifndef FILE
	@echo *** please specify variable FILE (and on your option INCLUDE)
else
	$(HSC) $(HSCFLAGS) $(INCLUDE) from=$(FILE) 
endif

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies follow ---

# dependencies updated: Wednesday 26-Mar-1997 14:23:41

all_hsc : /html/Welcome.html

/html/Welcome.html : Welcome.hsc

# --- DO NOT MODIFY THIS LINE -- hsc-dependencies precede ---

#EOF
