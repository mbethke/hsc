<WEBPAGE chapter="hsc - " title="Existing ports"
         PREV="features/rplcent.html" NEXT="options.html"
         QTEXT="Intel inside - idiot outside."
         QAUTHOR="Author unknown">

<P>As <hsc> is cryptic to use, has no graphical user interface and does
nothing else ahead from reading an input file and output to another one, 
it is quite easy to port it to other systems, as long as they support
some sort of command line interface.</P>

<P>This chapter describes existing or planed ports and what are the 
differences to the version for AmigaOS. Currently mentioned are
<A HREF="#unix">Un*x</A>,
<A HREF="#beos">BeOS</A> and
<A HREF="#msdos">MS-DOS based systems</A>.
</P>

<A NAME="unix"><H2>Un*x</H2></A>
The version for Unixoid systems is currenly the second best supported.
Usually, I do a bit of testing on a Macintosh running MkLinux before
releasing an official update.

<H3>Installation</H3>
As usual, you will have to compile the sources yourself. If you are
using <FILE>source/Makefile</FILE> and not 
<FILE>source/Makefile.dodl</FILE>, you can try a simple

<$source PRE>
    make install
</$source>

<P>after having created the binaries. This will atempt to copy the binaries
to <FILE>/usr/bin/</FILE> and <FILE>/usr/lib/</FILE>. This will only
work if you have write access to these directories.</P>

If you are normal luser, you can modify the <FILE>source/Makefile</FILE>
setting the symbol
<PRE>
    INSTDIR = $(HOME)/
</PRE>
Make sure that you also have created 
<FILE>$HOME/bin/</FILE> and <FILE>$HOME/lib/</FILE> before invoking 
<KBD>make install</KBD>.

<H3>Searching for syntax definition</H3>

When searching for the <ln_syntax>, <hsc> will look at
<UL>
<LI>The file you've specified using the option <op_prefsfile>
<LI>The current directory
<LI>The directory you have specified using <env_hscpath>
<LI>The directory specified in the environment variable
    <env_home>, with a <qq><FILE>lib/</FILE></qq> added.
<LI>The directory <qq><FILE>/usr/lib/</FILE></qq>
</UL>

<H3>Exit code</H3>

On notes/warnings/errors/fatal errors showing up, <hsc> will
return an exit code of 0/0/1/2.

<A NAME="beos"><H2>BeOS</H2></A>
Basically, the source code is prepared to compile under BeOS and
should utilize it's POSIX-layer. Laking a machine with BeOS running,
I couldn't test if it works. If you are interested to try this and
have some experience with compilers, contact me.

<A NAME="msdos"><H2>MS-DOS based systems</H2></A>

<P>As <hsc> is more portable than I want it to be, it is even
possible to make it somehow work with most MS-DOS based systems
like Windows NT/95 or OS/2. But as the whole philosophie and spirit
of these systems is a totally fashist one, there is no reason to
support them. There is no support within these documents or the
the <Makefile>s coming with the sources for any of these 
systems.</P>

<P>You <EM>must not</EM> contact me with problems compiling or using
<hsc> on <EM>any</EM> of these systems.</P>

</WEBPAGE>
