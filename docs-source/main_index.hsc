<HTML>
<HEAD>
<TITLE>hsc - html sucks completely (Support)</TITLE>
</HEAD><BODY>
<IMG SRC="docs/image/head.gif" ALT="hsc - Another stupid HTML preprocessor">
<H1>hsc - html sucks completely</H1>

<$macro ArcH NAME:string/r>
<A HREF=(name)><(name)></A>
(<(GetFileSize("../"+(name)))>)
</$macro>

<$include FILE="inc/macro.hsc">
<$define MinorUpdate:string="">

The is the support-page for <CODE>hsc</CODE>, a html-preprocessor.
It provides both introductory and detailed information about the purpose
and the usage of the program. You can also download the archives
containing the current version or pre-releases and bugfixes (if there
are any).

<H2>Current Version</H2>

The current version of <CODE>hsc</CODE> is <STRONG>Version 0.911</STRONG>
and should always be available from <AMINET FILE="text/hyper/">. 

<UL>
<LI>View <AMINET FILE="text/hyper/hsc.readme" TEXT="README"> for 
    a short overview.
<$if COND=(not (MinorUpdate))>
<LI><A HREF="docs/index.html">Documentation</A> for more details
</$if>
<LI>Download 
    <AMINET FILE="text/hyper/hsc.lha">, 
    the binary-archive for AmigaOS. This one includes binary, preferences 
    and documentation. 
<LI>Download <AMINET FILE="text/hyper/hsc-source.lha">, 
    the source-archive including the whole ANSI-C sources and the Makefile 
    needed to compile all tools.
</UL>

<H2>Minor Updates</H2>

<$if COND=(MinorUpdate)>
<STRONG>Version 0.911, Pre-Release#8</STRONG> is now available. 
It contains some bugfixes, and some minor improvements 
have been made. Additionally, there is a new tool to examine/modify 
the project file. 

<UL>
<LI><A HREF="README">README</A> for a short introduction
<LI><A HREF="docs/index.html">Documentation</A> for more details
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
    but no executables; use this to compile under Unixoid systems.
</UL>

If you don't know how to cope with <CODE>*.lha</CODE>-files, the sources
for a portable version of <CODE>lha</CODE> are available from 
<A HREF="http://wuarchive.wustl.edu/~aminet/misc/unix/lha_1_00.tar.Z">aminet:misc/unix/lha_1_00.tar.Z</A>.

<HR>
<ADDRESS>Thomas Aglassinger (<A HREF="mailto:agi@giga.or.at">agi@giga.or.at</A>),
<(GetTime())></ADDRESS>
</BODY></HTML>
