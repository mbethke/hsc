<DOCPAGE chapter="hsc - " title="Requirements"
    prev="updates.html"
    next="install.html">

This section focuses on things the user will need to successfully
utilize this program package. This does not only include hardware
requirements, but also certain skills of the user.

<H2>User Requirements</H2>

<P><hsc CAP> is not a tool for beginners. It does not make HTML
easier. It only makes it a bit less painful and brain-damaged<mdash>that's
all. Therefore, these documents assume that you are already familiar
with HTML. There exist several sources for beginners, and this won't
be page #1876545 to list these sources.</P>

<P>The user is expected to have already worked with some macro
language before.</P>

<P>If you want to utilize the <A HREF="project/index.html">project
management capabilities</A>, you should also know how to work with
<make> and write your own <Makefile>s. As it does not make much sense
to use <hsc> without it, this probably is a must.</P>

<P>For several other advanced topics, it is required to have some
experience with any programming language. Rexx is fully sufficient
for these tasks and most examples and some supporting stuff are written
in this language.</P>

<P>As <hsc> is a commandline tool, any knowledge of how to work with
a mouse and icons is not required.</P>

<P>Please note that I'm quite serious about all that. As I don't get
paid for every new user, there is no need for me to have as many
people as possible using this program.
Actually I do not care if you try to use it, find out you can not
cope with it, scream out <qq>Dammit, I'm a loser!</qq> and jump out of the
window from the 27th floor. But I start caring about it as soon as you do
not jump out of the window but send me bloody emails with
silly questions instead.</P>

<P>So those who now say <qq>Fine, I always wanted to learn how to write
Makefiles</qq> should consider looking for some other tool or be prepared
for jumping out of the window<mdash>just in case.</P>


<H2>System Requirements</H2>

<P><STRONG>Memory:</STRONG> Well, this mostly depends on your input data. Almost
every Unixish system in existence today should run <hsc> just fine. <hsc> was
developed on AmigaOS and is thus not very resource hungry; the absolute minimum
is around 500&nbsp;KB of free RAM, so an unexpanded A1200 should do. Using the
included macro library increases memory requirements to a few megabytes.</p>

<P><STRONG>Stack:</STRONG> On AmigaOS, the default stack size is 4&nbsp;KB, this
is too little for <hsc>. The binaries are not linked against any stack-checking
or -expansion code, so you should have at least 12&nbsp;KB of stack, better make
it 20&nbsp;KB. Users of other OSs don't have to waste a thought on stack size.</P>

<P><STRONG>OS and CPU:</STRONG>The AmigaOS binaries require at least
OS&nbsp;2.04 and a 68020 CPU, but a 68040 or higher ist strongly recommended.
The binaries in the Linux RPM were compiled for i386 and should run on any x86
system. The kernel version is not critical but the program was linked against
glibc V2.4. Linking against a replacement such as <a
href="http://www.fefe.de/dietlibc/">dietlibc</a> is not a problem, but glibc is
usually found on development systems anyway. If you run Linux or *BSD on a
non-x86 CPU, or HP-UX, Solaris, RiscOS etc., you probably know how to compile
your own software anyway.</P>

</DOCPAGE>
