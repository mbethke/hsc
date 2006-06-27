<DOCPAGE chapter="hsc - " title="Introduction"
    PREV="index.html"
    NEXT="copy.html"
    QTEXT="Welcome Boyz and Girls to the Non-Official Shit-Reduction Program"
    QAUTHOR='Novi Sad, "Me, Myself, All Over Again"'>

The main purpose of this chapter is to help you deciding whether you should go
on reading this documentation or not.

<H2>Motivation</H2>

<P>If you read this, it's quite likely that you tried to design an HTML page.
And you probably found out that HTML is a very clumsy thing: No macros, no
include files and several other features lacking.</P>

<P>And what can you do after your page is ready? View it with a browser and
test-click all links? Pfuahahaha! Most browsers are designed to handle as many
errors as possible and display a readable page, but don't mention your errors!
And link-testing is a very stupid task...</P>

<P>Of course, there are several tools around: You can use a macro languages like
<EXEC>m4</EXEC> to define macros and include files, use some of the various HTML
validators, replace your special characters with <KBD>recode</KBD> and run a
link validation tool on your page.</P>

<P>So, after installing several tools, compilers and interpreters, you are ready
to go&mdash;and have to start five programs after a simple change to one of your
pages.</P>

<H2>What It Is</H2>

<P><hsc CAP> tries to summarize the functionality of all these tools in one
program: it performs a (basic) syntax check, validates your links, replaces
special characters with entities and provides a way to define macros which
resembles HTML.</P>

<P>It also supports several features you probably will not find in most other
tools, like <A HREF="features/uris.html#prjrel">project relative URIs</A>,
stripping useless whitespace and automatically setting the <A
HREF="features/getsize.html">attributes for images size</A>.</P>

<P>Furthermore it provides some concepts to maintain your project with popular
tools like <make>.</P>

<H2>How It Works</H2>

<P><hsc CAP> simply acts as a preprocessor: You give it an <qq>extended</qq>
HTML-source (later on referred to as <qq>hsc source</qq>) containing special
commands, and <hsc> interprets it and produces a pure HTML output as object
file. This output file can be viewed with your browser.</P>

<P>Normally you will create your <hsc> sources using a simple text editor,
maintain them in a <Makefile> and process them with <hsc>. At least I consider
this the recommended way of using this tool.</P>

<H2>What It Is Not</H2>

<P>Obviously, there is no fancy GUI, no (pseudo-)WYSIWYG, no
drag&amp;drop&mdash;there are other programs which provide these
functionalities. But a common shortcoming of these programs usually is that they
yield incredibly crappy HTML that will look halfway decent on at most one make
of browser (i.e. IE), give less support for large projects and are limited in
configurability.</P>

<P>If you just want to create your own personal homepage, write a short HTML
document with information about your three freeware-proggies, include a picture
of your cat, send greets to Sepp and Hugo or something like that, there are
probably other tools which are easier to handle and will serve your needs more
or less well.</P>

<P>It should also be stated that you will require various skills usually not
needed for W3-authoring (or better: not expected to be needed by most people).
This includes a basic understanding of macro creation, writing <Makefile>s and
playing around with scripting languages like Rexx. If you have never done such
things before, I recommend you forget this package soon and come back in a few
years.</P>

<P>This also is no install-and-run package. You will need several other tools to
fully use it, and it is not within the scope of this documentation which one you
choose. However, it contains some comments and suggestions on that in various
places.</P>

<P>Despite the general HTML- and W3-hype of the recent years, both this tool and
its authors consider many concepts introduced with it ridiculous, braindead and
sometimes even pure crap. So this manual contains opinions and language you
might consider offensive. If you want to hear the usual <qq>Oh how great are all
those things!</qq>, you will have to look for something else.</P>
</DOCPAGE>
