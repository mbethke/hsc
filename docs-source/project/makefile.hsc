<WEBPAGE chapter="hsc - Project managment - " title="Makefile"
    BACK="index.html"
    PREV="make.html"
    NEXT="prjfile.html">

Inside the <FILE>starter-project</FILE>-drawer, you can find an rather empty
<A HREF=":../starter-project/source/Makefile"><makefile></A>
to be used for new projects. This will be a short explanation
of the symbols and rules defined in this <makefile>.

<H2>Symbols</H2>

<PRE>
        DESTDIR =/docs/
        IGNORE  =ign=46
        PRJFILE =hsc.project
        STDINC  =inc/my_macros.hsc
        HSCMISC =rplcent
</PRE>

<P><CODE>DESTDIR</CODE> describes the destination-directory for your
html-objects and is relative to the path where you invoke <make> (and
usually your hsc-sources are located, too). <CODE>IGNORE</CODE>
specifies which messages should be ignored,
<CODE>PRJFILE</CODE> is the project-file to be used both by <hsc> and
<hscdepp>. <CODE>STDINC</CODE> specifies include-files which should
be processed within the command-call invoking <hsc> from the
<makefile>; these files will be included for all hsc-sources.
<CODE>HSCMISC</CODE> contains all other options and switches that
should be passed to <hsc>.</P>


<PRE>
        HSC     =hsc
        HSCFLAGS=$(HSCMISC) $(IGNORE) prjfile=$(PRJFILE) to=$(DESTDIR) $(STDINC)
</PRE>

<P><CODE>HSC</CODE> is the command to be used to invoke <hsc>; if it isn't
already in your search-path, you can also enter the full path here.
<CODE>HSCFLAGS</CODE> is computed from the values above and contains
all parameters for <hsc>, exept the main hsc-source.</P>

<PRE>
        HSCDEPP =hscdepp
</PRE>

<P>This is the command to be used to invoke <hscdepp>.</P>

<H2>Rules</H2>
<$source PRE>
        $(DESTDIR)%.html : %.hsc
                $(HSC) $(HSCFLAGS) $< 
</$source>

<P>This is an implicit rule that will create a html-object in
<CODE>DESTDIR</CODE> using the corresponding hsc-source and
the options you specified above. The automatic variable 
"<CODE>$&lt;</CODE>" contains the name of the first dependency,
normally this is the main hsc-source.</P>

<$source PRE>
        depend :
                $(HSCDEPP) file=Makefile prjfile=$(PRJFILE) verbose
</$source>

<P>This rule will invoke <hscdepp> and update your dependencies; this
will modify your <makefile>.</P>

<$source PRE>
        NEW :
                $(HSC) $(HSCFLAGS) $(INCLUDE) from=$(FILE) 
</$source>

<P>This rule can be useful to add new files to the project. You can
also use it to change the include-dependencies of an already existing
file. This just updates the project-file, but not the <makefile>, so
make sure to invoke <KBD>make depend</KBD> afterwards.</P>

<P>For example, you could use</P>

<PRE>
       make NEW FILE=newfile.hsc
</PRE>

<P>to add <FILE>newfile.hsc</FILE> to the project.</P>

<H2>Post-processing</H2>

<P>A fine thing about the implicit rule used above to create your
html-object is that it can be extended to do several things
after <hsc> has performed it's task.</P>

<P>For example, extending it to</P>
<$source PRE>
        $(DESTDIR)%.html : %.hsc
                $(HSC) $(HSCFLAGS) $<
                chmod 644 $@
</$source>
<P>will make sure that the file-permission-bits are set to the usual
value required by w3-documents. This, of course, works only for
Unixoid systems.</P>

<P>If you are using AmigaOS and have AWeb running, you can use</P>
<$source PRE>
        $(DESTDIR)%.html : %.hsc
                $(HSC) $(HSCFLAGS) $<
                rx SendBrowser.rexx $@
</$source>
<P>to immediately display updated documents in your browser. The
Rexx-Script <A HREF=":../grafflwerk/SendBrowser.rexx">SendBrowser.rexx</A>
can be found in the <FILE>grafflwerk</FILE>-drawer of the hsc-archive.</P>

</WEBPAGE>
