##
## makefile
##

##
## ddr - destination directory
## opt - hsc options
## inc - all files of the inc/ directory
## hsc - shell command that envokes hsc
##
ddr = /docs/
pdr = features/
ign = ign=46 ign=21
inc = /inc/macro.hsc
opt = status destdir=$(ddr) projdir=$(pdr) checkuri smartent inc=$(inc) $(ign)
hsc = //hsc $(opt)

all: $(ddr)spctags.html		\
	$(ddr)absuris.html	\
	$(ddr)checkuri.html	\
	$(ddr)macros.html	\
	$(ddr)rplcent.html	\
	$(ddr)syntax.html	\


##
## features pages
##
$(ddr)spctags.html: spctags.hsc $(inc)
	$(hsc) spctags.hsc

$(ddr)absuris.html: absuris.hsc $(inc)
	$(hsc) absuris.hsc

$(ddr)checkuri.html: checkuri.hsc $(inc)
	$(hsc) checkuri.hsc

$(ddr)macros.html: macros.hsc $(inc)
	$(hsc) macros.hsc

$(ddr)rplcent.html: rplcent.hsc $(inc)
	$(hsc) rplcent.hsc

$(ddr)syntax.html: syntax.hsc $(inc)
	$(hsc) syntax.hsc

## EOF
