<WEBPAGE chapter="hsc - Project Management - " title="Make"
    BACK="index.html"
    PREV="hscpitt.html"
    NEXT="makefile.html">

<P>This chapter shortly describes a popular, but nearly unusable tool
called <make>, which can be used to solve project management tasks
when using <hsc>. This is not the only tool which is able to do so,
but it should exist on most platforms.</P>

<H2>What is Make?</H2>

Let's start with a quote from the documentation to <EXEC>GNUmake</EXEC>:

<BLOCKQUOTE> <P>The `make' utility automatically determines which
pieces of a large program need to be recompiled, and issues commands
to recompile them. [...]</P>

<P>You can use `make' with any programming language whose compiler can
be run with a shell command. Indeed, `make' is not limited to
programs. You can use it to describe any task where some files must be
updated automatically from others whenever the others change.
[...]</P>

<P>To prepare to use `make', you must write a file called the
"makefile" that describes the relationships among files in your program
and provides commands for updating each file.</P>
</BLOCKQUOTE>

<P>As of it's general purpose design, <make> also fits to be used
in combination with <hsc>. So far for the good part.</P>

<H2>Which Make to Use</H2>

<P>One of the worst things about <make> is that - as far as I know -
it comes from the fossil Unix world, with the usual results. There are
several programs with the same name, but different features.
Furthermore, you will have to bother with many occurrences of cryptic
looking characters.</P>

<P>This documentation will always refer to a version of make commonly
known as <EXEC>GNUmake</EXEC>, as it is freely distributable, exists
as source code and has been ported to most platforms.</P>

<P>(For Amigoids: A tool called <EXEC>smake</EXEC> has become very
popular under this system. Not because it is so powerful - on the
contrary - but because it was shipped with the SAS/c package. You can
put away this tool for <hsc>, probably not a single example given
within this manual or the supporting material will work.)</P>

<H2>Why Make Sucks</H2>

<P>One thing nearly all <make>-tools have in common is the inability to
produce useful error messages - if any at all.</P>

<P>Most of them are very picky about blanks and tabs (which can be
created by pressing the <KBD>TAB</KBD> key). Tabs in plain text file
are one of the most brain-damaged concept the (so called) modern
computer industry has established. Of course it is easy for a machine
to distinguish between spaces and tabs by their ASCII code. But the
user will see a blank space on the screen in both cases.</P>

<P>This becomes even worse, as many editor applications do not care
about tabs and replace them by spaces, normally without notifying the
user about that. This can not only be annoying, as these editors in
most cases are not sure if they should replace a tab by four or eight
space characters, often resulting in an unexpected looking display.
Even worse, picky tools (like most <make> tools) will interpret the
<makefile> as trashed. So make sure you are using a decent editor. For
example, <EXEC>memacs</EXEC> coming with the standard Workbench does a
good job on this.</P>

<H2>Makefiles and Pattern Rules</H2>

<P>As already mentioned above, you will have to write a so called
<Makefile>. This is by far the funniest part of <make> (apart from
interpreting esoteric error messages). Basically, you have to specify
rules how a target depending on specifics source is created. For
<hsc>, the target is usually the html document, and the sources are
all those files with the <KBD>.hsc</KBD> extension needed to create
the document.</P>

<P>As it would be dull to tell <make> exactly what to do for every
single target, you can specify so called pattern rules, which tell
<make> how to generally handle whole groups of files.</P>

<P>Ancient versions of <make> often only support a rather impotent way
for such pattern rules. An example of such an outdated rule:</P>

<$source PRE>
.c.o:
    $(CC) $(CFLAGS) -c $<
</$source>

<P>The first line tells <make> that now a rule will follow how to
create a target with a <qqc>.o</qqc> extension from a file with a
<qqc>.c</qqc> extension. You shouldn't bother too much about the
second line. The only thing you will have to know: You can forget
these kind of rules for <hsc>. They did a good job for the ridiculous
bunch of C-programmers and their ridiculous compilers, but they are
useless for everything more sophisticated. However, many <make>-tools
still only support such rules.</P>

The equivalent pattern rule in a more reasonable syntax look like
this:

<$source PRE>
%.o : %.c
    $(CC) $(CFLAGS) -c $<
</$source>

<P>This should give you a clue which sections of the manual of <make>
should be relevant for you. Within the next chapter, you will find
some remarks about how to write a <makefile> for <hsc>, but there
probably still will be no way around at least giving a glimpse to the
manual of <make>.</P>

<H2>So Why Make?</H2>

<P>After reading the above, one might think no one uses <make> as long
there any alternative left. See, and there is the problem: If you want
to have something quite powerful and flexible, which is available for
most platforms and is freeware, there currently is no alternative.</P>

<P>If you know of any, tell me and I will kick the whole chapter about
<make> and <Makefile>s immediately.</P>

<H2>Where to Get Make</H2>

<P>See also the section about <ln_related> where to obtain the version
of <make> recommended for use with <hsc>.</P>

</WEBPAGE>
