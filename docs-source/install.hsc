<DOCPAGE chapter="hsc - " title="Installation"
         PREV="require.html"
         NEXT="options.html">

This chapter will explain where all the files coming with the
archive should end up. <*; it least for AmigaOS. For other systems,
refer to <ln-existing-ports>.*>

<H2>AmigaOS</H2>

Because there is not much to do, there is no installer script. You
will have to perform some simple steps the first time you install
<hsc>. If the descriptions below are not sufficient for you to figure
out what to do, you do not want to use this tool anyway.

<H3>Simple Installation</H3>

<P>It's recommended to leave the whole <hsc> directory in one piece.
Therefor extract the archive somewhere to your hard disk,
for example to <FILE>work:hsc</FILE>.</P>

You now have to add this directory to your workbench search path.
This can be performed adding a line like the one you can find
below to your <FILE>user-startup</FILE>

<PRE>Path work:hsc ADD</PRE>

After rebooting, you can open a shell and type in <qq><KBD>hsc
help</KBD></qq>, and <hsc> should come up with a short information
message.

<H3>Minimum Installation</H3>

<P>For a minimum installation, only <FILE>hsc</FILE> and <FILE>hsc.prefs</FILE>
are required. If you want to utilize the project management capabilities of 
<hsc>, also <hscdepp> and <hscpitt> are needed.</P>

<P>Copy the binaries to somewhere in your workbench search path (for 
example, to <FILE>c:</FILE>) and <hsc.prefs> anywhere else and set
<env-hscpath> according to this.</P>

<H3>Making It Resident</H3>

<P>All binaries should already have the the pure bit set. If not, you
can do this by entering</P>

<PRE>
protect hsc/hsc add p
protect hsc/hscdepp add p
protect hsc/hscpitt add p
</PRE>

<P>in a shell.</P>

<P>To make the binaries resident on every startup, add the lines</P>

<PRE>
resident hsc
resident hscdepp
resident hscpitt
</PRE>

<P>to the <FILE>user-startup</FILE>.</P>

<P><STRONG>Important:</STRONG> For resident programs,
<CODE>PROGDIR:</CODE> is not defined. You will have to take care that
<hsc> is able to find <hsc.prefs> before it tries to scan
<CODE>PROGDIR:</CODE> for it. This can be done by by setting the
environment variable <env-hscpath> or by placing <hsc.prefs> in the
current directory for every project. Otherwise you will notice an
annoying requester, which will ask you to <QQ>insert volume
PROGDIR: into any drive</qq>.</P>

<H2>RiscOS</H2>

For the RiscOS distribution, you can find a file called
<FILE>ReadMe</FILE> in the directory <FILE>riscos</FILE> explaining
these details.

<H2><NextStep>, AmigaOS/ixemul, BeOS, Unixoid Systems</H2>

<P>Maybe you will have to compile the sources yourself. Refer to
the chapter about the <A HREF=":source.html">Source Code</A> for
details.</P>

<P>For installation, the first few lines of this <makefile> are
interesting for you. They define two symbols you might want to change.
<CODE>INSTALL</CODE> specifies the <exec>install</exec> program to be
used. Normally the default should be fine.</P>

<P>The default for <CODE>INSTDIR</CODE> will attempt to to copy the
binaries to <FILE>/usr/bin/</FILE> and some other data to
<FILE>/usr/lib/</FILE>. This will only work if you have write access
to these directories.</P>

If you are normal luser, you can modify this symbol to for instance

<PRE>INSTDIR = $(HOME)/</PRE>

Make sure that you also have created <FILE>$HOME/bin/</FILE> and
<FILE>$HOME/lib/</FILE> before starting the installation process. Or
maybe you prefer

<PRE>INSTDIR = /usr/local</PRE>

because of philosophical reasons I never really understood. In any
case, a

<PRE>make install</PRE>

should copy all files to the desired locations.

</DOCPAGE>
