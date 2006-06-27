<DOCPAGE chapter="hsc - " title="Existing Ports"
         PREV="source.html"
         NEXT="others.html"
         QTEXT="Intel inside - idiot outside."
         QAUTHOR="Author unknown">

<*<IMG SRC="image/designed.png" ALT="Designed for Nothing Specific" ALIGN="right">*>

<P>As <hsc> is cryptic to use, has no graphical user interface and
does nothing else apart from reading an input file and output to
another one, it is quite easy to port it to other systems as long as
they support some sort of command line interface and a decent
C-compiler.</P>

<P>This chapter describes existing or potential ports and their differences to
the Unix version. Currently ports exist for
<A HREF="#amiga">AmigaOS</A>,
<A HREF="#aros">AROS</A>,
<A HREF="#osx">MacOS X</A>,
<A HREF="#riscos">RiscOS</A>,
<A HREF="#beos">BeOS</A>,
<A HREF="#nextstep"><Nextstep></A>,
<A HREF="#windoze">Windoze</A> and
</P>

<H2><A NAME="amiga">AmigaOS</A></H2>

<p><hsc> was originally developed on AmigaOS and its roots still show. Although
development is going on under Linux, I try to maintain compatibility with
AmigaOS as long as possible. Which is not that hard for a program that's almost
completely pure ANSI C. There may be problems regarding TCP/IP networking as
that is still an optional add-on to AmigaOS, and with some weird
implementations of SAS/C's C library. AmigaOS with PPC extensions used to work
but has been untested for a dozen versions or so as I don't have the necessary
hardware. The same goes for AmigaOS with ixemul.library, though the reason foor
this is my laziness.</p>

<h2><a name="aros">AROS</a></h2>

<p>I recently added an <a href="http://www.aros.org/">AROS</a> port as this
system looks slightly more interesting than the comatose AmigaOS. Due to the
good job the AROS team did on the gcc+devkit, compiling <hsc> yourself should pose no
problems.</p>

<h2><a name="osx">MacOS X</a></h2>
<P>MacOS X as the first of Apple's systems to feature a
decent commandline shell (unless you want to call the Apple-II's <qq>decent</qq>,
which I don't) has no problems running and compiling HSC. To compile, you need
the Developer Tools (gcc, GNU make etc.) available by FTP from Apple. For
obscure reasons, the OSX linker doesn't like HSC to be stripped of symbols with
the <qqc>-s</qqc> switch, it wants <qqc>-x</qqc>. Otherwise the procedure is
the same as for other Unixoids.
</P>

<H2><A NAME="riscos">RiscOS</A></H2>

<P>The port to RiscOS has been done by Sergio Monesi and Nick
Craig-Wood. For details, please refer to <riscos-support>. Basically,
it acts like the version for <NextStep> (see below), although you will
have to use a special <make> tool. Note that this port hasn't been tested for a
long time and is likely to be broken in some way. If you still use <hsc> on RiscOS, let me
know.</P>

<H2><A NAME="nextstep"><Nextstep></A></H2>
<p><nextstep> counts as a unixoid system and should compile and run <hsc> just
fine using the included autoconf script.</p>

<H2><A NAME="beos">BeOS</A></H2>

<P>It has been reported to work. You should be able to compile it with
<EXEC>mwcc</EXEC>. It acts similar to the version for <Nextstep>.</P>

<P>But currently no one takes care about this port and I never tried
it out myself. So there might be problems with newer versions of this
OS or the <EXEC>mwcc</EXEC> compiler. Most likely there are more
compiler options that could be specified for better code.</P>

<H2><A NAME="windoze">Windoze</A></H2>

<P>Yes, HSC compiles under Windoze, at least under NT/XP. Options for M$ VC++
are included in the Makefile, although vor versions newer than 0.929 there might
be problems with the latest additions of files. The safest option would probably
be to use the CygWin environment and just compile with options for Unixoids. It
is not recommended to use this system for HSC, but if you really have to, by all
means stay away from Frontpage(tm), GoLive!(tm), Dreamweaver(tm) and similar
abominations and go for HSC.</P>

<H2><A NAME="msdos">MS-DOS Based Systems</A></H2>

<P>Anything running off a crippled 8.3-format filesystem is out of the question.
These systems should have been trashed 20 years ago and I will certainly not
call a <qq>.pref</qq>-file anything like <qq>.prf</qq> or <qq>.ini</qq> just
because this junk doesn't run HSC. It might be possible to compile HSC on
W32-based systems using FAT32, but I trust people still using them are not the
types who'd want HSC anyway. And that's a Good Thing.</P>

<H2><A NAME="others">Other Systems</A></H2>

<P>Ah, well, there are still systems without <hsc>.</P>

<P>For MacOS 9 and older, I don't don't have a compiler and I suppose the
current user base cares as much about <hsc> as I do about their OS.
Internally <hsc> is <em>not</em> designed to be used from
command line only. If you take a look at the <CODE>main()</CODE> of
<FILE>source/hsc/hsc.c</FILE>, you will see that it's only about
ten lines of code. However, you will need to find a tool to do the
project management - or integrate this part into your GUI. And that's
probably where it starts to get tough...</P>

<P>Atari TOS counts as MS-DOS based (see above), especially the
filename limitations.</P>

<P>Several other systems should have no trouble compiling the same way as for
Unixoid systems or AmigaOS with ixemul.library. It mostly depends on the
C-compilers available. Systems like VMS (I was told it's still in use in its
incarnation for Alpha CPUs) with weird path separators and other niceties to
keep programmers busy will probably need a few <code>#ifdef</code>s in places
where there are a few already.</P>

</DOCPAGE>
