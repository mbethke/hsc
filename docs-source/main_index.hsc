<$include FILE="inc/macro.hsc">
<HTML><HEAD>
<TITLE>hsc - html sucks completely - Support</TITLE>
</HEAD><BODY>
<IMG SRC="docs/image/head.gif" ALT="hsc - Another stupid HTML preprocessor">
<H1>hsc - html sucks completely</H1>

<$macro ArcH NAME:string/r>
<A HREF=(name)><(name)></A>
(<(GetFileSize("../"+(name)))>)
</$macro>

<$define MinorUpdate:string="">

The is the support-page for <CODE>hsc</CODE>, a html-preprocessor.
It provides both introductory and detailed information about the purpose
and the usage of the program. You can also download the archives
containing the current version or pre-releases and bugfixes (if there
are any).

<H2>Current Version</H2>

The current version of <CODE>hsc</CODE> is <STRONG>version 0.913</STRONG>
and should always be available from <AMINET FILE="text/hyper/">. 

<UL>
<LI>View <AMINET FILE="text/hyper/hsc.readme" TEXT="README"> for 
    a short overview.
<$if COND=(not (MinorUpdate))>
<LI><A HREF="docs/index.html">Documentation</A> for more details
</$if>
<LI>Download <AMINET FILE="text/hyper/hsc.lha">, the binary-archive
    for AmigaOS. This one includes binary, preferences
    and documentation. 
<LI>Download <AMINET FILE="text/hyper/hsc-source.lha">, 
    the source-archive including the whole ANSI-C sources and the
    Makefile needed to compile all tools.
</UL>

<H2>Minor Updates</H2>

<$if COND=(MinorUpdate)>
<STRONG>Version 0.913, pre-release#9</STRONG> is now available.
This is supposed to be one of the last pre-releases, and should
already work fine.

<UL>
<LI><A HREF="README">README</A> - short introduction
<LI><A HREF="docs/index.html">Documentation</A> - more details
<LI><A HREF="CHANGES">Changes</A> - what's new
</UL>

The archives below are only available from this support-page:
<$else>
Currently, there are no minor updates available.
The archives below are identical to those available from Aminet:
</$if>

<UL>
<LI>Download <ArcH NAME="hsc.lha"> - documentation and binaries for AmigaOS
<LI>Download <ArcH NAME="hsc-source.lha"> - source code
<LI>Download <ArcH NAME="hsc-all.lha"> - source code and documentation, 
    but no executables; use this to compile under other systems.
</UL>

<H2>Additional Material</H2>

<P>If you don't know how to cope with <CODE>*.lha</CODE>-files, the sources
for a portable version of <CODE>lha</CODE> are available from 
<A HREF="http://wuarchive.wustl.edu/~aminet/misc/unix/lha_1_00.tar.Z">aminet:misc/unix/lha_1_00.tar.Z</A>
(84K).</P>
<*
<P>Users who are unfamiliar to <CODE>make</CODE> can download
<ArcH NAME="make-doc.lha">, which contains the manual
to <CODE>GNUmake</CODE> in Amigaguide-format.</P>
*>
<HR>
<$let hsc.format.time="%d-%b-%Y">
<ADDRESS>Thomas Aglassinger (<A HREF="mailto:agi@giga.or.at">agi@giga.or.at</A>),
<(GetTime())></ADDRESS>
</BODY></HTML>
