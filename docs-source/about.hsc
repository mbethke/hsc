<WEBPAGE chapter="hsc - " title="Introduction"
    PREV="index.html"
    NEXT="copy.html"
    QTEXT="Welcome Boyz and Girls to the Non-Official Shit-Reduction Program"
    QAUTHOR='Novi Sad, "Me, Myself, All Over Again"'>

The main purpose of this chapter is to help you deciding whether you
should go on reading this documentation or not.

<H2>Motivation</H2>

<P>If you read this, it's quite likely that you tried to design a html
page. And you probably found out that html is a very clumsy thing: No
macros, no include files and several other features lacking.</P>

<P>And what can you do after your page is ready? View it with a
browser and test-click all links? Pfuahahaha! Most browser are
designed to handle as many errors as possible and display a readable
page, but don't mention your errors! And link-testing is a very stupid
task...</P>

<P>Of course, there are several tools around: You can use a macro
languages like <EXEC>m4</EXEC> to define macros and include files, use
some of the various html validators, replace your special characters
with <KBD>recode</KBD> and run a link validation tool on your
page.</P>

<P>So, after installing several Tools (and compilers and
interpreters), you are ready to go... and can start five programs
after a simple change to one of your pages.</P>

<*
<P>(Note: People who use the C-Preprocessor are easy to identify 
by the silly date format of <CODE>__DATE__</CODE>: who writes
<CODE>Feb 23 1995</CODE> on purpose?)</P>
*>

<H2>What It Is</H2>

<P><hsc CAP> tries to summarize the functionality of all
these tools in one program: it performs a (basic) syntax
check, validates your (local) links, replaces special characters
by entities and provides a way to define macros which should
remind you to html.</P>

<P>It also supports several features you probably will not find in
most other tools, like <A HREF="features/uris.html#prjrel">project
relative URIs</A>, stripping useless white spaces and automatically
setting the <A HREF="features/getsize.html">attributes for images
size</A>.</P>

<P>Furthermore it provides some concepts to maintain your project with
popular tools like <make>.</P>

<H2>How It Works</H2>

<P><hsc CAP> simply acts as a preprocessor: You give it an "extended"
html-source (later in this document referred as "hsc source")
containing special commands, and <hsc> interprets it and produces a
pure html output as object file. This output file can be viewed with
your w3-browser.</P>

<P>Normally you will create your hsc sources using a simple text
editor, maintain them in a <Makefile> and process them with <hsc>. At
least I consider this the recommend way of using this tool.</P>

<H2>What It Is Not</H2>

<P>Obviously, there is no fancy GUI, no (pseudo-)WYSIWYG, no drag
&amp; drop - there are other programs which provide these
functionalities. But a common lack of these programs usually is that
they give less support for large projects and are limited in
configurability.</P>

<P>If you just want to create your own personal homepage, write a
short html document with information about your three selfwritten
freeware-proggies, include a picture of your cat, send greets to Sepp
and Hugo or something like that, there are probably other tools which
are easier to handle and will serve your needs more or less well.</P>

<P>It should also be stated that you will require several experiences
usually not needed for w3-authoring (or better: not expected to be
needed by most people). This includes a basic understanding of macro
creation, writing <Makefile>s and playing around with scripting
languages like Rexx. If you have never done such things before, I
recommend you forget this package soon and come back in a few
years.</P>

<P>This also is no install-and-run package. You will need several
other tools to fully use it, and it is not within the scope of this
documentation which one you choose. Although it contains some comments
and suggestions about that at various places.</P>

<P>Despite the general html- and w3-hype of the recent years, both this
tool and the author of it consider many concepts introduced with it
ridiculous, braindead and sometimes even pure shit. So this manual
contains opinions and language you might consider offensive. If you
want to hear the usual "Oh how great are all those things!", you will
have to look for something else.</P>

</WEBPAGE>
