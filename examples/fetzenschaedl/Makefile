#
# makefile for hsc example project
#

#
# NOTE: this makefile is prepared for use under AmigaOS (default) and
#	Unix. To enable the Unix-version, enable the second 
#	definition of the symbol `HSC'.
#

#
# command used to envoke hsc
#
HSC	= //hsc
#HSC	= ../../hsc

#
# options for hsc
#
HSCOPTS   = TO= STATUS="line|verbose"

all : stupid.html hugo/hugo.html main.html 

main.html : main.hsc macro.hsc
	$(HSC) $(HSCOPTS) main.hsc 

stupid.html : stupid.hsc macro.hsc
	$(HSC) $(HSCOPTS) stupid.hsc 

hugo/hugo.html : hugo/hugo.hsc macro.hsc
	$(HSC) $(HSCOPTS) hugo/hugo.hsc 

# EOF
