#
# Generic Makefile for hsc projects
#

#
# DESTDIR    - destination directory (usually relative to source directory)
# HSCMESSAGE - hsc message options
# HSCPROJECT - project file
# HSCINCLUDE - standard includes for all sources
# HSCMISC    - miscellaneous flags and options
# 
# HSC        - shell command that invokes hsc
# HSCFLAGS   - hsc options
#
# HSCDEPP    - shell command that invokes hscdepp
#

DESTDIR	=
HSCMESSAGE	=MsgMode=normal Ignore=46
HSCPROJECT	=hsc.project
HSCINCLUDE	=include/standard.hsc include/page.hsc
HSCMISC	=RplcEnt

HSC	=hsc
HSCFLAGS=$(HSCMISC) $(HSCMESSAGE) PrjFile=$(HSCPROJECT) to=$(DESTDIR) $(HSCINCLUDE)

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
	$(HSCDEPP) file=Makefile PrjFile=$(HSCPROJECT) verbose

