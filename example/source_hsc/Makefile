#
# makefile for hsc example project
#

#
# NOTE: this makefile is prepared for use under AmigaOS (default) and
#	Unix. To enable the Unix-version, enable the second 
#	definition of the symbol "HSC" and "DESTDIR".
#

#
# command used to envoke hsc
#
HSC	= //hsc
#HSC	= ../../hsc

#
# project destination dir
#
DESTDIR	= /object_html/
#DESTDIR	= ../object_html/

#
# options for hsc
#
HSCOPTS   = TO=$(DESTDIR) STATUS=line|verbose

all : $(DESTDIR)stupid.html $(DESTDIR)hugo/hugo.html $(DESTDIR)main.html 

$(DESTDIR)main.html : main.hsc macro.hsc
	$(HSC) $(HSCOPTS) main.hsc 

$(DESTDIR)stupid.html : stupid.hsc macro.hsc
	$(HSC) $(HSCOPTS) stupid.hsc 

$(DESTDIR)hugo/hugo.html : hugo/hugo.hsc macro.hsc
	$(HSC) $(HSCOPTS) hugo/hugo.hsc 

# EOF
