<WEBPAGE chapter="hsc - " title="Existing Ports"
         PREV="source.html"
         NEXT="others.html"
         QTEXT="Intel inside - idiot outside."
         QAUTHOR="Author unknown">

<*<IMG SRC="image/designed.gif" ALT="Designed for Nothing Specific" ALIGN="right">*>

<P>As <hsc> is cryptic to use, has no graphical user interface and
does nothing else apart from reading an input file and output to
another one, it is quite easy to port it to other systems, as long as
they support some sort of command line interface and a decent
C-compiler.</P>

<P>This chapter describes existing or planed ports and what are the 
differences to the version for AmigaOS. Currently ports exist for
<A HREF="#riscos">RiscOS</A>,
<A HREF="#nextstep"><Nextstep></A>,
<A HREF="#amixemul">AmigaOS with <FILE>ixemul.library</FILE></A>,
<A HREF="#beos">BeOS</A>
and <A HREF="#unix">Unixoid systems</A>.

There are also some comments on
<A HREF="#msdos">MS-DOS based systems</A>
and <A HREF="#others">other systems</A>.
</P>

<H2><A NAME="riscos">RiscOS</A></H2>

The port to RiscOS has been done by Sergio Monesi and Nick Craig-Wood.
For details, please refer to <A
HREF="http://www.axis.demon.co.uk/hsc/"><(HSC.Anchor)></A>. Basically,
it acts like the version for <NextStep> (see below).

<H2><A NAME="nextstep"><Nextstep></A></H2>

<H3>Searching For Syntax Definition</H3>

When searching for the <ln_syntax>, <hsc> will look at
<UL>
<LI>The file you've specified using the option <op_prefsfile>
<LI>The current directory
<LI>The directory you have specified using <env_hscpath>
<LI>The directory specified in the environment variable
    <env_home>, with a <qq><FILE>lib/</FILE></qq> added.
<LI>The directory <qq><FILE>/usr/local/lib/</FILE></qq>
<LI>The directory <qq><FILE>/usr/lib/</FILE></qq>
</UL>

If one of these directories does not exist at all, <hsc> will silently
proceed with the next one on this list.

<H3>Exit Code</H3>

On notes/warnings/errors/fatal errors showing up, <hsc> will
return an exit code of 0/0/1/2.

<H2><A NAME="amixemul">AmigaOS with ixemul.library</A></H2>

<P>Most GNU-tools have been ported to AmigaOS using an additional
library called <FILE>ixemul.library</FILE>. It is quite successful in
turning an Amiga in a moldy and dribbling Un*x-box, emulating all
those idiotic things like the <qqc>..</qqc> and <qqc>/bin</qqc> pretty
shitty well.</P>

<P>But if you prefer to rape your machine, you will have to compile it
yourself using the <EXEC>gcc</EXEC>-port of ADE. If you don't know
what I'm currently talking about, you don't want to do this anyway.
Actually the binary of <hsc> included in the archive for AmigaOS is
able to deal with native filenames and system directories, so why
bother?</P>

<H2><A NAME="beos">BeOS</A></H2>

<P>It has been reported to work. You should be able to compile it with
<EXEC>mwcc</EXEC>. It acts similar to the version for <Nextstep>.</P>

<P>But currently no one takes care about this port, and I never tried
it out myself. So there might be problems with newer versions of this
OS or the <EXEC>mwcc</EXEC> compiler. Most likely there are more
compiler options that could be specified for better code. So
suggestions and corrections about how to improve the BeOS-part of the
<Makefile> are welcome.</P>

<H2><A NAME="unix">Unixoid Systems</A></H2>

<P>This version acts similar to the one for <Nextstep>. Because of
fossil compilers and a messy structure for include files under this
system, you might have some trouble compiling <hsc> on your system. Oh
my god, it's ANSI-C!</P>

<P>Anyway, <hsc> has been reported to compile and run under Solaris,
IRIX, MkLinux for Power Macintosh and several others.</P>

<H2><A NAME="msdos">MS-DOS Based Systems</A></H2>

<P>As <hsc> is more portable than I want it to be, it is even possible
to make it somehow work with most MS-DOS based systems like Windows
NT/95 or OS/2. But as the whole philosophy and spirit of these systems
is a totally fascist one, there is no reason to support them. There is
no support within these documents or the the <Makefile>s coming with
the sources for any of these systems.</P>

<P>You <EM>must not</EM> contact me with problems compiling or using
<hsc> on <EM>any</EM> of these systems.</P>

<H2><A NAME="others">Other Systems</A></H2>

<P>Ah, well, there are still systems without <hsc>.</P>

<P>Recently several new operating systems for Amiga-based Hardware
have been announced or released in alpha versions (usually claiming
they are already being beta). For those supported by the ADE you
should have no trouble compiling the same way as for AmigaOS with
ixemul.library. For others it depends on the C-compilers available.
Basically it should be possible to make <hsc> work on all these
systems. As I do not have the nerves to fiddle around with them, I
probably will not release any ports. However, if someone else manages
to make it work, I will gracefully include the required changes.</P>

<P>For MacOS, I don't don't have a compiler and maybe one will have to
write a GUI. Internally <hsc> is <em>not</em> designed to be used from
command line only. If you take a look at the <CODE>main()</CODE> of
<FILE>source/hsc/hsc.c</FILE>, you will see that it's only about
ten lines of code. However, you will need to find a tool to do the
project management - or integrate this part into your GUI. And that's
probably where it starts to get tough...</P>



<P>Atari TOS counts as MS-DOS based (see above), especially the
filename limitations.</P>

<P>For C64/C128 <hsc> is too slow and bloated, which of course is my
fault. I'm pretty sure one could do a decent <hsc>-alike program on
these machines.</P>

<P>I think this should now include all systems of the 80ties and
90ties worth mentioning. Of course it does not include all those
esoteric mainframe trash and impotent pseudo-Un*x-alikes. (That's why
I used the term <qq>worth mentioning</qq> in the previous
sentence.)</P>

</WEBPAGE>
