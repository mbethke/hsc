<WEBPAGE chapter="hsc - " title="Requirements"
    prev="updates.html"
    next="distrib.html">

This section focuses on things the user will need to successfully
utilize this program package. This does not only include hardware
requirements, but also certain skills of the user.

<H2>User Requirements</H2>

<P><hsc CAP> is not a tool for beginners. It does not make html
easier. It only makes it a bit less painful and brain-damaged - that's
all. Therefor, these documents assume that you are already familiar
with html. There exist several sources for beginners, and this won't
be page #1876545 which lists these sources.</P>

<P>The user is expected to have already worked with some macro
language before.</P>

<P>If you want to utilize the <A HREF="project/index.html">project
management capabilities</A>, you should also know how to work with
<make> and write your own <Makefile>s. As it does not make much sense
to use <hsc> without it, this probably is a must.</P>

<P>For several other advanced topics, it is required to have some
experiences with any programming language. Rexx is fully sufficient
for these task, and most examples and some supporting stuff is written
in this language.</P>

<P>As <hsc> is a CLI-only tool, any knowledge of how to work with
mouse and icons is not required.</P>

<P>Please note that I'm quite serious about all that. As I don't get
paid for every new user, there is no need for me to have as many
people as possible using this program.</P>

<P>Actually I do not care if you try to use it, find out you can not
cope with it, scream out "Dammit, I'm a looser!" and jump out of the
window from the 27th floor. But I start caring about as soon as you do
not jump out of the window and instead send me bloody emails with
silly questions.</P>

<P>So those who now say "Fine, I always wanted to learn how to write
Makefiles" should consider looking for some other tool or be prepared
for jumping out of the window - just in case.</P>


<H2>System Requirements</H2>

<P><STRONG>Memory:</STRONG> Well, this mostly depends on your
input-data. To process reasonable sized pages, about 300K of free ram
should be enough. For large pages and projects using loads of
documents, this can increase to 1MB or even more, so your machine
should have at least 2MB of memory.</P>

<P><STRONG>Stack:</STRONG> A stack value of 12 KB usually should be
sufficient; at least, this worked for these documents. The programs
coming with this archive are not linked with any stack-check or
-expansion code.</P>

<P><STRONG>AmigaOS and CPU:</STRONG> Basically, it works on an A2000
with AmigaOS 2.04 and a plain 68000, but as this isn't much fun, the
binaries are compiled for 68020. As <hsc> is not one of the fastest, a
68060 is recommended.</P>

</WEBPAGE>
