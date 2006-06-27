<DOCPAGE chapter="hsc - Project Management - " title="Makefile"
    BACK="index.html"
    PREV="make.html"
    NEXT="hscdepp.html">

Inside the <FILE>starter-project</FILE>-drawer, you can find an rather empty
<A HREF=":../starter-project/Makefile"><makefile></A>
to be used for new projects. This will be a short explanation
of the symbols and rules defined in this <makefile>.

<H2>Symbols</H2>

<PRE>
DESTDIR   =
HSCMESSAGE=MsgMode=normal Ignore=46
HSCPROJECT=hsc.project
HSCINCLUDE=include/standard.hsc include/page.hsc
HSCMISC   =RplcEnt
</PRE>

<P><CODE>DESTDIR</CODE> describes the destination-directory for your
html-objects and is relative to the path where you invoke <make> (and
usually your hsc-sources are located, too). <CODE>IGNORE</CODE>
specifies which messages should be ignored, <CODE>HSCPROJECT</CODE> is
the project-file to be used both by <hsc> and <hscdepp>.
<CODE>HSCINCLUDE</CODE> specifies include files which should be
processed within the command-call invoking <hsc> from the <makefile>;
these files will be included for all hsc-sources. <CODE>HSCMISC</CODE>
contains all other options and switches that should be passed to
<hsc>.</P>

<PRE>
HSC     =hsc
HSCFLAGS=$(HSCMISC) $(HSCMESSAGE) prjfile=$(HSCPROJECT) to=$(DESTDIR) $(HSCINCLUDE)
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

<P>This is a pattern rule that will create an HTML object in
<CODE>DESTDIR</CODE> using the corresponding hsc-source and the
options you specified above. The automatic variable
"<CODE>$&lt;</CODE>" contains the name of the first dependency,
normally this is the main hsc-source.</P>

<$source PRE>
depend :
        $(HSCDEPP) file=Makefile prjfile=$(HSCPROJECT) verbose
</$source>

<P>This rule will invoke <hscdepp> and update your dependencies; this
will modify your <makefile>.</P>

<H2>Pre- And Post Processing</H2>

<P>A fine thing about the pattern rule used above to create your
html-object is that it can be extended to do several things
before or after <hsc> performes its task.</P>

<P>For example, extending it to</P>

<$source PRE>
$(DESTDIR)%.html : %.hsc
        $(HSC) $(HSCFLAGS) $<
        chmod 644 $@
</$source>

<P>will make sure that the file-permission-bits are set to the usual
value required by w3-documents. This, of course, works only for
Unixoid systems.</P>

<P>If you are using AmigaOS and have an ARexx capable w3 browser installed, you can
use</P>

<$source PRE>
$(DESTDIR)%.html : %.hsc
        $(HSC) $(HSCFLAGS) $<
        OpenURL FILE $@
</$source>

<P>to immediately display updated documents in your browser. OpenURL
is available from Aminet.</p>

<P>You can also use a better syntax checker than <hsc> on your newly
created document:</P>

<$source PRE>
$(DESTDIR)%.html : %.hsc
        $(HSC) $(HSCFLAGS) $<
        CheckHTML $@
</$source>

<P>Note that errors found during the second check are reported from
the HTML object and might be a bit difficult to backtrace in the hsc
source.</P>

</DOCPAGE>
