<WEBPAGE chapter="hsc - " title="Source Code"
    PREV="updates.html"
    NEXT="bugs.html"
    QTEXT=("I've been swimming in the dirty water<BR>"
          +"I've been swimming where the fish won't go")
    QAUTHOR='The Jesus &amp; Mary Chain, "Dirty Water"'>

<H2>Availability</H2>

<P>The source archive should be available from the same location
where you are supposed to get <A HREF="updates.html">updates</A>
from. Look out for an archive called <FILE>hsc-source.lha</FILE>.

<H2>Compiling</H2>

<P>The source archive contains a <KBD>README</KBD> how to compile the whole
stuff. If you basically know how to cope with a C-compiler and a <makefile>,
there should be no real problem for you.</P>

<H2>Portability</H2>

<P><hsc> was written in naked &amp; masochistic ANSI-C. Even
worse, it only uses functions of the standard library and does
not require any Amiga- or Un*x-specific include files like
<TG>dir.h</TG>, <TG>String.h</TG> or functions like <CODE>ReadArgs()</CODE>,
<CODE>stricmp()</CODE> etc.</P>

<P>This should ensure a maximum of portability. However, the
handling of filenames is a system-dependant thing, which has
to be adapted for every OS. See also the section about 
<qq><A HREF="ports.html">Existing Ports</A></qq></P>

</WEBPAGE>
