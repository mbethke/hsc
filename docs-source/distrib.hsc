<WEBPAGE chapter="hsc - " title="Archive contents"
    PREV="require.html"
    NEXT="features/spctags.html">

This is a short explanation of the files and directories
coming with the archives:

<H2>hsc.lha</H2>

This one is the complete distribution of the AmigaOS-version and
includes the binaries, 
<PRE>
    hsc            executable for the preprocessor
    hsc.020        executable for the preprocessor, requires 68020
    hscdepp        executable for the dependency-procreator
    hsc.prefs      contains html-syntax-definition
    CHANGES        program history
    README         guess what
    README.docs    tells you where to start reading the docs
    COPYING        The GNU General public license
    example/       a short example project
    grafflwerk/    miscellaneous stuff that might be useful
    docs/          documentation drawer; start browsing `index.html'
    src_docs/      hsc-sources for documentation drawer; as example

</PRE>

<P>If this release contains some major changes, there is also a file
named
<$if COND=(exists("/Hmpf_Grmbl_Wrcks"))>
"<A HREF=":../Hmpf_Grmbl_Wrcks">Hmpf_Grmbl_Wrcks</A>",
<$else>
"Hmpf_Grmbl_Wrcks",
</$if>
dealing with the most important
in-combat-ibilities and promising that this will never be
neccessary again.</P>

<P>This archive is available from the <hsc-support> and
<AMINET FILE="text/hyper/hsc.lha">.</P>

<H2>hsc-source.lha</H2>

These are the ANSI-C sources to compile <hsc> and <hscdepp>.
You probably will have to modify the <Makefile>.

<PRE>
    README         Instruction how to modify the Makefile
    COPYING        The GNU General public license
    Makefile       Makefile to compile the wohle thing
    Makefile.dodl  Makefile for CC and weenixes
    hsclib/        sources for the parser
    hsc/           hsc, the preprocessor
    hscdepp/       hscdepp, the dependency-procreator
    ugly/          my personal function library; experimental stuff
</PRE>

<P>This archive is available from the <hsc-support> and 
<AMINET FILE="text/hyper/hsc-source.lha">.</P>

<H2>hsc-all.lha</H2>

<P>A compilation of the two above archives. Contains everything exept
the AmigaOS-executables. Ment to be compiled on other OS-es.</P>

This archive is only available from the <hsc-support>.

</WEBPAGE>
