<DOCPAGE chapter="hsc - " title="Archive Contents"
    PREV="require.html"
    NEXT="install.html">

<$macro ArcH NAME:string/r>
<H2><(name)></H2>
<*<P>(size: <(GetFileSize((":../../"+name)))>)</P>*>
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

<ArcH NAME="hsc-X.X.tar.bz2">

<p>Contains the ANSI-C source code to compile all tools of this package, plus
all macro and preferences files. Because of the latter, you will probably want
this archive even if you don't want to compile <hsc> yourself. The
most important files and directories in this package are:

<PRE>
README           tells you to read <A HREF=":source.html">docs/source.html</A>
Makefile         makefile to compile the whole thing
hsclib/          parser functions
hscprj/          project functions
hsc/             hsc, the preprocessor
hsctools/        hscdepp and hscpitt
ugly/            function library; don't try this at home
win32/           files only needed for the Win32 platform
amigaos/         files only needed for the AmigaOS platform
</PRE>

Additionally, the hsc-sources for this documentation are included
in <FILE>docs-source</FILE>.</p>

<ArcH NAME="hsc-X.X-AmigaOS.lha">
<p>This is the distribution for AmigaOS, including only the binaries.</P>

<ArcH NAME="hsc-X.X-Win32.zip">
<p>This is the distribution for 32bit Windows systems including only the binaries.</P>

</DOCPAGE>
