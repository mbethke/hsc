<WEBPAGE chapter="hsc - " title="Archive contents"
    PREV="require.html"
    NEXT="features/spctags.html">

<$macro ArcH NAME:string/r>
<H2><(name)></H2>
<P>(size: <(GetFileSize((":../../"+name)))>)</P>
</$macro>

This is a short explanation of the files and directories
coming with the archives:

<ArcH NAME="hsc.lha">

This one is the complete distribution of the AmigaOS-version and
includes the binaries, 
<PRE>
    hsc            executable for preprocessor
    hscdepp        executable for hsc's dependency procreator
    hscpitt        executable for hsc's project interfering and trashing tool
    hsc.prefs      contains html-syntax-definition
    CHANGES        program history
    README         short introduction
    IMPORTANT      where to start and what to read
    COPYING        The GNU General public license
    example/       a short example project
    grafflwerk/    miscellaneous stuff that might be useful
    docs/          documentation drawer; start browsing <qq>index.html</qq>
    src_docs/      hsc-sources for documentation drawer; as example

</PRE>

<P>If this release contains some major changes, there is also a file
named
<$if COND=(exists("/Hmpf_Grmbl_Wrcks"))>
    <qq><FILE><A HREF=":../Hmpf_Grmbl_Wrcks">Hmpf_Grmbl_Wrcks</A></FILE></qq>,
<$else>
    <qq><FILE>Hmpf_Grmbl_Wrcks</FILE></qq>,
</$if>
dealing with the most important
in-combat-ibilities and promising that this will never be
neccessary again.</P>

<P>This archive is available from the <hsc-support> and
<AMINET FILE="text/hyper/hsc.lha">.</P>

<ArcH NAME="hsc-source.lha">

These are the ANSI-C sources to compile <hsc> and <hscdepp>.
You probably will have to modify the <Makefile>.

<PRE>
    README         Instruction how to modify the Makefile
    COPYING        The GNU General public license
    Makefile       Makefile to compile the wohle thing
    Makefile.dodl  Makefile for CC and weenixes
    hsclib/        parser-functions
    hscprj/        project-management-functions
    hsc/           hsc, the preprocessor
    hscdepp/       hscdepp, the dependency-procreator
    ugly/          my personal function library; experimental stuff
</PRE>

<P>This archive is available from the <hsc-support> and 
<AMINET FILE="text/hyper/hsc-source.lha">.</P>

<ArcH NAME="hsc-all.lha">

<P>A compilation of the two above archives. Contains everything exept
the AmigaOS-executables. Ment to be compiled on other systems.</P>

This archive is only available from the <hsc-support>.

</WEBPAGE>
