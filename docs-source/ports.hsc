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
<A HREF="#amiga-ppc">AmigaOS with PowerPC</A>,
<A HREF="#amixemul">AmigaOS with <FILE>ixemul.library</FILE></A>,
<A HREF="#beos">BeOS</A>
and <A HREF="#unix">Unixoid systems</A>.

There are also some comments on
<A HREF="#msdos">MS-DOS based systems</A>
and <A HREF="#others">other systems</A>.
</P>

<H2><A NAME="riscos">RiscOS</A></H2>

<P>The port to RiscOS has been done by Sergio Monesi and Nick
Craig-Wood. For details, please refer to <riscos-support>. Basically,
it acts like the version for <NextStep> (see below), although you will
have to use a special <make> tool.</P>

<H2><A NAME="nextstep"><Nextstep></A></H2>

<H3>Searching For Syntax Definition</H3>

When searching for the <ln-syntax>, <hsc> will look for the file you've specified using the option <op-prefsfile>.
If you did not specify any, it will search the

<UL>
<LI>current directory
<LI>directory you have specified using <env-hscpath>
<LI>directory specified in the environment variable
    <env-home>, with a <qq><FILE>lib/</FILE></qq> added.
<LI>directory <qq><FILE>/usr/local/lib/</FILE></qq>
<LI>directory <qq><FILE>/usr/lib/</FILE></qq>
</UL>

for a file named <hsc.prefs>. If one of these directories does not
contain a <hsc.prefs> or does not exist at all, <hsc> will silently
proceed with the next one on this list.

<H3>Exit Code</H3>

On notes/warnings/errors/fatal errors showing up, <hsc> will
return an exit code of 0/0/1/2.

<H2><A NAME="amiga-ppc">AmigaOS with PowerPC</A></H2>

<p>There is a port available that compiles with SAS/c. However,
this was only done to show that it is possible. Although no changes to the
code are necessary, currently nobody maintains this version so the
executable might be slightly outdated. Concerning the pathetic mess
which link format to use: it is the one from p5.</p>

<p>You can find the binary in <aminet file="text/hyper/hscPPC.lha">,
so you still need the "normal" archive for the documentation.</p>

<H2><A NAME="amixemul">AmigaOS with ixemul.library</A></H2>

<P>Most GNU-tools have been ported to AmigaOS using an additional
library called <FILE>ixemul.library</FILE>. It is quite successful in
turning an Amiga in a moldy and dribbling Un*x-box, emulating all
those idiotic things like the <qqc>..</qqc> and <qqc>/bin</qqc> pretty
shitty well. Of course you can also use <hsc> with it.</P>

<P>But if you prefer to rape your machine, you have to compile it
yourself using the <EXEC>gcc</EXEC>-port from GeekGadgets. If you
don't know what I'm currently talking about, you don't want to do this
anyway. Actually the binary of <hsc> included in the archive for
AmigaOS is able to deal with native filenames and system directories,
so why bother?</P>

<H2><A NAME="beos">BeOS</A></H2>

<P>It has been reported to work. You should be able to compile it with
<EXEC>mwcc</EXEC>. It acts similar to the version for <Nextstep>.</P>

<P>But currently no one takes care about this port, and I never tried
it out myself. So there might be problems with newer versions of this
OS or the <EXEC>mwcc</EXEC> compiler. Most likely there are more
compiler options that could be specified for better code.</P>

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
NT/95/98/00/.. or OS/2. Of course there is no reason to support them, therefore
compilation, usage and differences are neither described within these
documents nor supported by the <Makefile> coming with the source
code.</P>

<P>If you now ask yourself "why?", then you are pretty
simpleminded.</p>

<P>You <EM>must not</EM> contact me with problems compiling or using
<hsc> on <EM>any</EM> of these systems.</P>

<H2><A NAME="others">Other Systems</A></H2>

<P>Ah, well, there are still systems without <hsc>.</P>

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

<P>Several other systems (like the dreadful <a
href="http://www.pios.comp">pOS</a> in its eternal alpha-state) should
have no trouble compiling the same way as for Unixoid systems of
AmigaOS with ixemul.library. It mostly depends on the C-compilers
available, basically it should be possible to make <hsc> work on all
these systems.</P>

<P>I think this should now include all systems of the 80ties and
90ties worth mentioning. Of course it does not include all those
esoteric mainframe trash and impotent pseudo-Un*x-alikes. That's why
I used the term <qq>worth mentioning</qq> in the previous
sentence.</P>

</WEBPAGE>
