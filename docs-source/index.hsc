<HTML><HEAD>
  <$define DESCRIPTION:string=("This is the documentation to hsc, a "
    +"developer tool for html-projects. It acts as a preprocessor "
    +"and supports include files, macros, expressions, conditionals, "
    +" replaces special characters, performs a structure check, tests "
    +" existence of local links and several other things.")>
  <TITLE>hsc - html sucks completely - Documentation</TITLE>
  <LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@giga.or.at">
  <LINK REL="Home" HREF=":index.html">
  <LINK REL="Copyright" HREF=":copy.html">
  <META HTTP-EQUIV="keywords" CONTENT="html, tool, preprocessor, macro, include, condition, suck">
  <META HTTP-EQUIV="description" CONTENT=(DESCRIPTION)>
</HEAD>

<BODY>
<*<IMG SRC=":image/head.gif" ALT="[hsc]">*>

<* pseudo-navigation-bar *>
<IMG SRC=":image/nomain.gif" ALT="----" ALIGN="middle">
<IMG SRC=":image/noindex.gif" ALT="-----" ALIGN="middle">
<A HREF=":copy.html"><IMG SRC=":image/copy.gif" ALT="Copyright" ALIGN="middle"></A>
<IMG SRC=":image/noback.gif" ALT="--" ALIGN="middle">
<IMG SRC=":image/noprev.gif" ALT="--------" ALIGN="middle">
<A HREF="about.html"><IMG SRC=":image/next.gif" ALT="Next" ALIGN="middle"></A>

<P ALIGN="right">
<I>Don't believe the hype<BR>
Take a look at the small print<BR>
Ignore the bold type<BR></I>
(Carter U.S.M, "Do Re Me, So Far So Good")
</P>

<H1>hsc</H1>

<P><STRONG>(Version 0.913<* (pre-release#9) *>, 26-May-1997)</STRONG></P>

<P><(DESCRIPTION)></P>

<IMG SRC="image/hsc.gif" ALT="..SUCK SUCK SUCK.." ALIGN="right">

<P>
A current version of this document should always be available from
<A HREF="http://www.giga.or.at/~agi/hsc/docs/"><(HSC.ANCHOR)></A>.</P>

<H2>About the program</H2>
<UL>
<LI><A HREF="about.html">Introduction</A> - What's this all about?
<LI><A HREF="copy.html">Copyright and disclaimer</A> - the usual legal stuff
<LI><A HREF="author.html">Author</A> - how to contact me
<LI><A HREF="../CHANGES">Changes</A> - what's new with this release
<LI><A HREF="updates.html">Updates</A> - where to find them
</UL>

<H2>Before you begin</H2>
<UL>
<LI><A HREF="require.html">Requirements</A> - what to own and know
<LI><A HREF="distrib.html">Archive contents</A> - things coming along
<LI><A HREF="install.html">Installation</A> - where to put the whole 
    damn thing
</UL>

<H2>Usage</H2>
<UL>
<LI><A HREF="options.html">Invoking <hsc></A>
    <UL>
    <LI><A HREF="options.html#options">Options</A> and 
        <A HREF="options.html#switches">switches</A> - there are loads of them
    <LI><A HREF="options.html#exitcodes">Exit-code</A> - return to sender
    <LI><A HREF="examples.html">Example usage</A> - how it is supposed to work
    </UL>
<LI><A HREF="fileargs.html">File arguments</A> - specifying input and output
<LI><A HREF="messages.html">Messages</A> - shit happens
    <UL>
    <LI><A HREF="messages.html#elements">Message elements</A>
    <LI><A HREF="messages.html#classes">Message classes</A>
    <LI><A HREF="messages.html#options">Message options</A>
    <LI><A HREF="messages.html#list">List of messages</A>
    </UL>
<LI><A HREF="envvar.html">Environment variables</A> - Save the nature
</UL>

<H2>Features</H2>
<UL>
<LI><A HREF="features/spctags.html">Special tags</A>
    to make life a bit easier
<LI><A HREF="features/checkuri.html">Check existence</A>
     of local URIs to avoid dangling links
<LI><A HREF="features/absuris.html">Absolut URIs</A>
    to avoid paths like "../../../image/back.gif"
<LI><A HREF="features/rplcent.html">Replace special characters</A>
    by their entities
<LI><A HREF="features/getsize.html">Get size</A> of images and set attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE>
<LI><A HREF="features/spcattr.html">Special attributes</A>
    to access various information
<LI><A HREF="features/strip.html">Strip specific elements</A> (comments,
    redundant linefeeds and white-spaces, unneeded tags,
    tags with external references)
<LI><A HREF="features/syntax.html">Structure and syntax check</A>
    to avoid surprises with different browsers
</UL>

<H2>Advanced topics</H2>
<UL>
<LI><A HREF="features/expressions.html">Expressions</A>
    - there's more to life than strings
    <UL>
    <LI><A HREF=":features/expressions.html#operators">Operators</A>
    <LI><A HREF=":features/expressions.html#boolean">Boolean expressions</A>
    <LI><A HREF=":features/expressions.html#priorities">Priorities</A>
    </UL>
<LI><A HREF="macro/macros.html">Macros</A>
    - create your own shortcuts and templates.
    <UL>
    <LI><A HREF=":macro/flag.html">Macro modifiers</A>
    <LI><A HREF=":macro/attrib.html">Attribute declaration</A>
    <LI><A HREF=":macro/attrib.html#type">Attribute types</A>
    <LI><A HREF=":macro/attrib.html#modifier">Attribute modifiers</A>
    </UL>
<LI><A HREF="features/if.html">Conditionals</A>
    - what if..?
    <UL>
    <LI><A HREF=":features/if.html#general">General syntax</A>
    <LI><A HREF=":features/if.html#simple">Some simple examples</A>
    <LI><A HREF=":features/if.html#nesting">Nesting conditionals</A>
    <LI><A HREF=":features/if.html#macros">Conditionals and macros</A>
    </UL>
<LI><A HREF="features/prefs.html">Syntax definition</A>
    - what to do if html version 17.3 is out
<LI><A HREF="features/exec.html">Execute shell-commands</A>
    - poor man's pseudo-CGI 
<LI><A HREF="project/index.html">Project management</A>
    - I'll <make> ya
    <UL>
    <LI><A HREF="project/make.html">make</A> - a general purpose tool for project management
    <LI><A HREF="project/prjfile.html">project files</A> - where <hsc> stores the
        required information.
    <LI><A HREF="project/makefile.html">Makefiles</A> - how to adopt them for <hsc>
    <LI><hscdepp LINK> -
        <hsc> dependency procreator
    <LI><hscpitt LINK> -
        <hsc> project interfering and trashing tool
    <LI><A HREF="project/hscpaltrow.html"><hscpaltrow></A> -
        a Rexx-script demonstrating how to utilize the output of <hscpitt>
    </UL>
</UL>

<H2>Miscellaneous</H2>
<UL>
<LI><A HREF="questions.html">Questions</A> - and maybe some answers..
<LI><A HREF="source.html">Source code</A> - gnugnugnugnugngnugngug
<LI><A HREF="ports.html">Existing ports</A> - Amiga rulez? Not really.
<LI><A HREF="bugs.html">Known bugs, problems and limitations</A> - <hsc> sucks completely
<LI><A HREF="future.html">Future Improvements</A> - what might happen
<LI><A HREF="related.html">Related stuff</A> - tools and further information
</UL>

<HR>
<$let hsc.format.time="%d-%b-%Y">
<ADDRESS>
  <A HREF=":author.html">Thomas Aglassinger</A> (agi@giga.or.at),
  <(gettime())>
</ADDRESS>
</BODY></HTML>
