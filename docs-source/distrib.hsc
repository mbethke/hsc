<WEBPAGE chapter="hsc - " title="Archive contents"
    PREV="require.html"
    NEXT="features/spctags.html">

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
    COPYING          The GNU General public license
    hsc              executable for preprocessor
    hscdepp          executable for hsc's dependency procreator
    hscpitt          executable for hsc's project interfering and trashing tool
    hsc.prefs        contains syntax definition
    source/          source code (ANSI-C)
    docs/            documentation drawer; start browsing <qq>index.html</qq>
    docs-source/     hsc-sources for documentation drawer; as example
    grafflwerk/      miscellaneous stuff that might be useful
    starter-project/ template to start a new project with
</PRE>

<ArcH NAME="hsc.lha">

This one is the complete distribution of the AmigaOS-version and
includes the binaries, documentation and all supporting material.
But it does not contain the source code.

<P>This archive is available from the <hsc-support> and
<AMINET FILE="text/hyper/hsc.lha">.</P>

<ArcH NAME="hsc-source.lha">

These are the ANSI-C sources to compile all tools of this package.
You probably will have to modify the <Makefile>. The most important
files and directories in <file>source/</file> are:

<PRE>
    README           how to compile this
    Makefile         Makefile to compile the whole thing
    Makefile.dodl    Makefile for CC and weenixes
    hsclib/          parser functions
    hscprj/          project functions
    hsc/             hsc, the preprocessor
    hsctools/        hscdepp and hscpitt
    ugly/            function library; don't try this at home
</PRE>

<P>Additionally, the hsc-sources for this documentation are also
included in <FILE>docs-source</FILE> for example purpose.</P>

<P>This archive is available from the <hsc-support> and 
<AMINET FILE="text/hyper/hsc-source.lha">.</P>

<ArcH NAME="hsc-all.lha">

<P>A compilation of the two above archives. Contains everything except
the AmigaOS-executables and -icons; additionally, there are some
test-routines for the functions of <file>ugly.lib</file> included.</P>

This archive is only available from the <hsc-support>.

</WEBPAGE>
