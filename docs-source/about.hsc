<WEBPAGE chapter="hsc - " title="Introduction"
    PREV="index.html" NEXT="copy.html"
    QTEXT="Welcome Boyz and Girls to the Non-Official Shit-Reduction Program"
    QAUTHOR='Novi Sad, "Me, Myself, All Over Again"'>

<H2>Motivation</H2>

<P>If you read this, it's quite likely that you tried to design a
html-page. And you probably found out that html is a very
clumsy thing: No macros, no include files and several other
features lacking.</P>

<P>And what can you do after your page is ready? View it
with a browser and test-click all links? Pfuahahaha! Most
browser are designed to handle as many errors as possible
and display a readable page, but don't mention your errors!
And link-testing is a very stupid task...</P>

<P>Of course, there are several tools around: You can use a
C-preprocessor to define macros and include files, use
some of the various html-validators, replace your special
characters with <KBD>recode</KBD> and run a link-test-tool
on your page.</P>

<P>So, after installing several Tools (and compilers and
interpreters), you are ready to go... and can start five
programs after a simple change to one of your pages.</P>

<P>(Note: People who use the C-Preprocessor are easy to identify 
by the silly date format of <CODE>__DATE__</CODE>: who writes
<CODE>Feb 23 1995</CODE> on purpose?)</P>

<H2>What it is</H2>

<P><hsc> tries to summarize the functionality of all
these tools in one program: it performs a (small) syntax
check, tests your (local) links, replaces special characters
by their entities and provides a very html-like way to
define macros.</P>

<P>It also supports several features you probably won't
find in most other tools, like absolute URIs, stripping useless
white-spaces and automatically setting the size-attributes
for images.
For complex projects, a project-file can be maintained
and a dependency generator helps you keeping your Makefile
up-to-date.</P>

<H2>How it works</H2>

<P><hsc> simply acts as a preprocessor: You give it an
"extended" html-source (later in this document refered as
"hsc-source") containig special commands, and <hsc>
interprets it and produces a pure html-output as object
file. This output-file can be viewed with your
w3-browser.</P>

<P>Normally I write my hsc-sources using a simple text-editor,
maintain them in a <FILE>Makefile</FILE> and process them
with <hsc>. From my point of view, this is the recommend
way of using this tool.</P>

<H2>What it isn't</H2>

Obviously, there is no fancy gui, no (pseudo-)WYSIWYG, no drag &amp;
drop - there are other programs which provide these
functionalities. But a common lack of these programs usually
is that they give less support for large projects and are limited
in configurability.</P>

<P>If you just want to create your own personal homepage,
write a short html-document with information about your
three selfwritten freeware-proggies, include a picture of your cat,
send greets to Sepp and Hugo or something like that,
there are probably other tools which are easier to handle and
will serve your needs well. But of course, I can't prevent you from
using <hsc> for these tasks, too.</P>

</WEBPAGE>
