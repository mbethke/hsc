<WEBPAGE chapter="hsc - " title="Archive Contents"
    PREV="require.html"
    NEXT="install.html">

<$macro ArcH NAME:string/r>
<H2><(name)></H2>
<P>(size: <(GetFileSize((":../../"+name)))>)</P>
</$macro>

This is a short explanation of the files and directories
coming with these archives:

<PRE>
IMPORTANT        where to start and what to read
README           short introduction
CHANGES          program history
NEWS             tells about incompatibilites to older versions
COPYING          the GNU General public license
hsc              executable for preprocessor
hscdepp          executable for hsc's dependency procreator
hscpitt          executable for hsc's project interfering and trashing tool
hsc.prefs        contains syntax definition
source/          source code
docs/            documentation drawer; start browsing <qq>index.html</qq>
docs-source/     hsc sources for documentation drawer; as example
examples/        some small example projects for tutorial purpose
grafflwerk/      miscellaneous stuff that might be useful
starter-project/ template to start a new project with
</PRE>

<ArcH NAME="hsc.lha">

This one is the complete distribution of the AmigaOS-version and
includes the binaries, documentation in html-format and all supporting
material. But it does not contain the source code.

<P>This archive is available from the <hsc-support> and
<AMINET FILE="text/hyper/hsc.lha">.</P>

<ArcH NAME="hsc-source.lha">

Contains everything like the above, except the AmigaOS-executables and
-icons. It also includes the ANSI-C source code to compile all tools
of this package. The most important files and directories in
<file>source/</file> are:

<PRE>
README           tells you to read <A HREF=":source.html">docs/source.html</A>
Makefile         makefile to compile the whole thing
Makefile.agi     makefile used by author
hsclib/          parser functions
hscprj/          project functions
hsc/             hsc, the preprocessor
hsctools/        hscdepp and hscpitt
ugly/            function library; don't try this at home
</PRE>

<P>Additionally, the hsc-sources for this documentation are included
in <FILE>docs-source</FILE> for example purpose.</P>

<P>This archive is available from the <hsc-support> and 
<AMINET FILE="text/hyper/hsc-source.lha">.</P>

<ArcH NAME="hsc-ps.lha">

<P>This archive contains the documentation in Postscript format and is
only needed if you want to have a printed manual. But before you send
it to the shared printer of your computer center or something like
that, note that these are more than 100 pages which will require some
resources (read: time, paper and ink).</P>

<P>It is generated from the html-version by using a conversion
utility, and only differs in a few aspects mostly concerning
navigability.</P>

<P>It is available from the <hsc-support> and
<AMINET FILE="text/hyper/hsc-ps.lha">.</P>

</WEBPAGE>
