##
## makefile
##

##
## ddr - destination directory
## opt - hsc options
## inc - all files of the inc/ directory
## hsc - shell command that envokes hsc
##
pdr = ../
ddr = $(pdr)docs/
opt = status destdir=$(ddr) checkuri smartent inc=inc/macro.hsc inc=inc/webpage.inc ign=46 ign=21
inc = inc/macro.hsc inc/webpage.inc
hsc = $(pdr)hsc

all: $(ddr)about.html $(ddr)bugs.html $(ddr)copy.html $(ddr)distrib.html \
	$(ddr)messages.html $(ddr)examples.html \
	$(ddr)future.html $(ddr)install.html $(ddr)options.html $(ddr)questions.html \
	$(ddr)related.html $(ddr)require.html \
        $(ddr)features/spctags.html $(ddr)features/if.html \
	$(ddr)features/absuris.html $(ddr)features/checkuri.html \
	$(ddr)features/rplcent.html $(ddr)features/syntax.html \
	$(ddr)macro/attrib.html $(ddr)macro/flag.html $(ddr)macro/macros.html \
	$(ddr)index.html

$(ddr)index.html: index.hsc $(inc)
	$(hsc) index.hsc $(opt)

$(ddr)about.html: about.hsc $(inc)
	$(hsc) about.hsc $(opt)

$(ddr)bugs.html: bugs.hsc $(inc)
	$(hsc) bugs.hsc $(opt)

$(ddr)copy.html: copy.hsc $(inc)
	$(hsc) copy.hsc $(opt)

$(ddr)distrib.html: distrib.hsc $(inc)
	$(hsc) distrib.hsc $(opt)

$(ddr)messages.html: messages.hsc $(inc)
	$(hsc) messages.hsc $(opt)

$(ddr)examples.html: examples.hsc $(inc)
	$(hsc) examples.hsc $(opt)

$(ddr)future.html: future.hsc $(inc)
	$(hsc) future.hsc $(opt)

$(ddr)install.html: install.hsc $(inc)
	$(hsc) install.hsc $(opt)

$(ddr)require.html: require.hsc $(inc)
	$(hsc) require.hsc $(opt)

$(ddr)related.html: related.hsc $(inc)
	$(hsc) related.hsc $(opt)

$(ddr)options.html: options.hsc $(inc)
	$(hsc) options.hsc $(opt)

$(ddr)questions.html: questions.hsc $(inc)
	$(hsc) questions.hsc $(opt)

##
## features pages
##
$(ddr)features/spctags.html: features/spctags.hsc exmpl/exec.inc $(inc)
	$(hsc) features/spctags.hsc $(opt)

$(ddr)features/absuris.html: features/absuris.hsc $(inc)
	$(hsc) features/absuris.hsc $(opt)

$(ddr)features/checkuri.html: features/checkuri.hsc $(inc)
	$(hsc) features/checkuri.hsc $(opt)

$(ddr)features/if.html: features/if.hsc $(inc)
	$(hsc) features/if.hsc $(opt)

$(ddr)features/rplcent.html: features/rplcent.hsc $(inc)
	$(hsc) features/rplcent.hsc $(opt)

$(ddr)features/syntax.html: features/syntax.hsc $(inc)
	$(hsc) features/syntax.hsc $(opt)

##
## macros
##
$(ddr)macro/attrib.html: macro/attrib.hsc $(inc)
	$(hsc) macro/attrib.hsc $(opt)

$(ddr)macro/flag.html: macro/flag.hsc $(inc)
	$(hsc) macro/flag.hsc $(opt)

$(ddr)macro/macros.html: macro/macros.hsc $(inc) \
	  exmpl/m_addr.inc exmpl/m_file.inc exmpl/m_next.inc
	$(hsc) macro/macros.hsc $(opt)
## EOF
