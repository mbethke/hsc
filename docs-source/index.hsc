<HTML><HEAD>
  <$define Version:string="0.917">
  <$define pre-release:string="0">
  <$define release-date:string="21-Sep-1998">

  <$define DESCRIPTION:string=("This is the documentation to hsc, a "
    +"developer tool for html-projects. It acts as a preprocessor "
    +"and supports macros, expressions, conditionals, include files, "
    +" replaces special characters, performs a structure check, tests "
    +" existence of local links and several other things."
    +" Optionally it can keep track of documents being processed and"
    +" maintain the dependencies between them, making it relatively"
    +" easy to integrate it into existing developer environments."
   )>


  <TITLE>Hsc - Manual</TITLE>
  <LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@giga.or.at">
  <LINK REL="Home" HREF=":index.html">
  <LINK REL="Copyright" HREF=":copy.html">
  <LINK REL="Next" HREF=":about.html">
  <*
   * Some day this might work, but currently the Postscript manual
   * is compressed with lha, therefor it is not "printable", and
   * so this LINK is still omitted
   *
  <LINK media="print" title="The manual in postscript"
        type="application/postscript"
        rel="alternate"
        href="http://www.giga.or.at/~agi/hsc/hsc.ps">
   *>
  <META HTTP-EQUIV="keywords" CONTENT="html,tool,preprocessor,macro,include,condition,suck">
  <META HTTP-EQUIV="description" CONTENT=(DESCRIPTION)>
</HEAD>

<BODY>

<html-only>
<* pseudo-navigation-bar *>
<IMG SRC=":image/nomain.gif" ALT="----" ALIGN="middle">
<IMG SRC=":image/noindex.gif" ALT="-----" ALIGN="middle">
<A HREF=":copy.html"><IMG SRC=":image/copy.gif" ALT="Copyright" ALIGN="middle"></A>
<IMG SRC=":image/noback.gif" ALT="--" ALIGN="middle">
<IMG SRC=":image/noprev.gif" ALT="--------" ALIGN="middle">
<A HREF="about.html"><IMG SRC=":image/next.gif" ALT="Next" ALIGN="middle"></A>
<hr>
</html-only>

<***************************************************************************
 *
 * Contents of header
 *
 * This includes:
 * - silly quote
 * - headings
 * - version information
 *
 **************************************************************************>

<P ALIGN="right">
<I>Don't believe the hype<BR>
Take a look at the small print<BR>
Ignore the bold type<BR></I>
(Carter U.S.M, "Do Re Me, So Far So Good")
</P>

<* heading *>
<html-only>
<H1>Hsc</H1>
</html-only>

<* version info *>
<html-only>
<P><STRONG>(Version <(Version)>
<$if cond=(pre-release<>"0")>
  (pre-release#<(pre-release)>)
</$if>
<$stripws type="both">
, <(release-date)>)</STRONG></P>

<P><IMG SRC="image/austria.gif" ALT="[A.E.I.O.U.]" ALIGN="middle"> Es
gibt keine <A HREF="teutsch.html">deutschsprachige
Dokumentation</A>.</P>
</html-only>

<postscript-only>
<H1>Preface</H1>
<H2>About The Program</H2>
</postscript-only>

<P><(DESCRIPTION)></P>

<html-only>
<IMG SRC="image/hsc.gif" ALT="..SUCK SUCK SUCK.." ALIGN="right">
</html-only>

<P>A current version of this program and manual should always be
available from <A
HREF="http://www.giga.or.at/~agi/hsc/"><(HSC.ANCHOR)></A>, both as
hypertext (html) and printabel (Postscript) document.</P>

<***************************************************************************
 *
 * Contents of Postscript-version
 *
 **************************************************************************>
<postscript-only>

<P>The version of the program described herein is <STRONG>version <(version)></STRONG>,
released <(release-date)>.</P>

<H2>About This Manual</H2>

This manual was generated from the online documentation, however
there are a few minor differences:

<UL>
<LI>This first chapter looks totally different.

<LI>There is no tool-bar for navigation, as this apparently does not
    make sense in a printed document.

<LI>The table of contents is at the end of the manual - not because I
    think that is very smart, but because the conversion is easier for
    me that way. But as these pages are numbered using Roman digits,
    you can easily grab them and put them to the beginning of the
    manual without getting confused.

<LI>Several plain text files, like <CODE>CHANGES</CODE>,
    <CODE>NEWS</CODE>, but also some example scripts in <grafflwerk>,
    are not part of this manual, but can be viewed in the
    hypertext version by means of links.

<LI>Instead of images, you will only see their <CODE>ALT</CODE>-text.
    (Maybe I get this EPS-stuff to work some day,
    but do not count on it.)
</UL>

<P>As this is the first release with a printable manual, it might
contain some quirks, where a specific rendering or a certain phrase
does not make sense. Although I tried to avoid this of course - at
least there should not be any shitty <qq>Click here!</qq>. If you feel
like, you might considers these to be bugs and report them (with page
number, please). Hopefully no whole chapters are missing. </P>

<P>To those who are now going to mock "And it took you more than two
years to convert the manual to Postscript?" (the first public release
of <hsc> was in October 1995) I'd like to say: No, but it took more
than two years for somebody to provide an at least rudimentarily
bearable application to do this. See the chapter about Use-Cases for
details.</P>

<P>(Can you imagine a w3-browser, where you can push <qq>Print
all</qq>, and it automatically analyzes the document for stuff like
<TG>LINK REL="next" HREF=..</TG>, gets all related documents and
prints them in correct sequence? Can you imagine getting rid of these
crappy html2<I>insert-your-favourite-print-format-here</I> converters?
Even if you can, the browser developers can't. At least not in 1998,
several years after this html-and-w3-disaster was initiated.)</P>

<P>Furthermore I have to admit that I never really read this flavour
of the manual myself. Partially, because Ghostscript for Amiga has a
painful user interface, therfore I just gave a glimpse to a few
randomly selected pages.</P>

<P>Partially because I am a dedicated Postscript hater, as you can not
do much with files in this format: you can not in search it, you can not
quickly display it, you can not link around in it and you can hardly
convert it to something more reasonable. - "Hey, you can convert it to
PDF or DVI!" - Yeah, great! That changes everything.</P>

<P>And finally, because you will have to reprint most of it after the
next release and throw away all the paper. - "Who cares, I printed it
for free at my university!" - Oh really, and who pays back the tree
they had to cut?</P>

<P>(Note for Americans: paper usually is made from trees. - "Can't be,
in my town we have a so called paper factory!" - Oh, sorry for
confusing you. Forget what I said.)</P>

<P>So this is dedicated to those who prefer to waste paper instead of
bandwidth or disk storage,</P>

<P Align="right">
<$let hsc.format.time="%B %Y">
<I>Thomas Aglassinger<BR>
Oulu, <(gettime())></I></P>
</postscript-only>

<***************************************************************************
 *
 * Contents of html-version
 *
 **************************************************************************>
<html-only>
<H2>About The Program</H2>
<UL>
<LI><A HREF="about.html">Introduction</A> - What's this all about?
<LI><A HREF="copy.html">Copyright And Disclaimer</A> - not-my-fault, Freeware, source included
<LI><A HREF="author.html">Author</A> - how to contact me
<LI><A HREF="changes.html">Changes And News</A> - what's new with this release
<LI><A HREF="updates.html">Updates</A> - where to find them
</UL>

<H2>Before You Begin</H2>
<UL>
<LI><A HREF="require.html">Requirements</A> - what to own and know
<LI><A HREF="distrib.html">Archive Contents</A> - things coming along
<LI><A HREF="install.html">Installation</A> - where to put the whole 
    damn thing
</UL>

<H2>Usage</H2>
<UL>
<LI><A HREF="options.html">Invoking <hsc></A> - options, switches, exit code
<LI><A HREF="examples.html">Example Usage</A> - how it is supposed to work
<LI><A HREF="fileargs.html">File Arguments</A> - specifying input and output
<LI><A HREF="messages.html">Messages</A> - shit happens
    <UL>
    <LI><A HREF="messages.html#elements">Message Elements</A>
    <LI><A HREF="messages.html#classes">Message Classes</A>
    <LI><A HREF="messages.html#options">Message Options</A>
    <LI><A HREF="message-list.html">List Of Messages</A>
    </UL>
<LI><A HREF="envvar.html">Environment Variables</A> - save the nature
</UL>

<H2>Features</H2>
<UL>
<LI><A HREF="features/spctags.html">Special Tags</A>
    - make life a bit easier
<LI><A HREF="features/checkuri.html">Check Existence</A>
     of local URIs to avoid dangling links
<LI><A HREF="features/uris.html#prjrel">Project Relative URIs</A>
    - avoid paths like "../../../image/back.gif"
<LI><A HREF="features/rplcent.html">Replace Special Characters</A>
    by their entities
<LI><A HREF="features/getsize.html">Get Size</A> of images and set attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE>
<LI><A HREF="features/spcattr.html">Special Attributes</A>
    - access various information
<LI><A HREF="features/strip.html">Strip Specific Elements</A> - comments,
    redundant linefeeds and white-spaces, unneeded tags,
    tags with external references
<LI><A HREF="features/syntax.html">Structure And Syntax Check</A>
    to avoid surprises with different browsers
</UL>

<H2>Advanced Topics</H2>
<UL>
<LI><A HREF="features/assign.html">Attribute Assignments</A>
<LI><A HREF="features/expressions.html">Expressions</A>
    - there's more to life than strings
    <UL>
    <LI><A HREF=":features/expressions.html#operators">Operators</A>
    <LI><A HREF=":features/expressions.html#boolean">Boolean Expressions</A>
    <LI><A HREF=":features/expressions.html#priorities">Priorities</A>
    </UL>
<LI><A HREF="macro/macros.html">Macros</A>
    - create your own shortcuts and templates.
    <UL>
    <LI><A HREF=":macro/flag.html">Macro Modifiers</A>
    <LI><A HREF=":macro/attrib.html">Attribute Declaration</A>
    <LI><A HREF=":macro/attrib.html#type">Attribute Types</A>
    <LI><A HREF=":macro/attrib.html#modifier">Attribute Modifiers</A>
    </UL>
<LI><A HREF="features/if.html">Conditionals</A>
    - what if..?
    <UL>
    <LI><A HREF=":features/if.html#general">General Syntax</A>
    <LI><A HREF=":features/if.html#simple">Some Simple Examples</A>
    <LI><A HREF=":features/if.html#nesting">Nesting Conditionals</A>
    <LI><A HREF=":features/if.html#macros">Conditionals And Macros</A>
    </UL>
<LI><A HREF="features/prefs.html">Syntax Definition</A>
    - what to do if html version 17.3 is out
<LI><A HREF="features/exec.html">Execute Shell Commands</A>
    - poor man's pseudo-CGI 
<LI><A HREF="project/index.html">Project Management</A>
    - I'll <make> ya
    <UL>
    <LI><A HREF="project/prjfile.html">Project Files</A> -
        where <hsc> stores information about a project
    <LI><hscpitt LINK> -
        <hsc> project interfering and trashing tool
    <LI><A HREF="project/make.html">make</A> - a general purpose tool for project management
    <LI><A HREF="project/makefile.html">Makefiles</A> - how to adopt them for <hsc>
    <LI><hscdepp LINK> -
        <hsc> dependency procreator
    <LI><A HREF="project/hscpaltrow.html"><hscpaltrow></A> -
        a Rexx-script utilizing <hscpitt>
    </UL>
<LI><A HREF="usecases.html">Some Use-Cases</A> -
    hints how to do certain things
</UL>

<H2>Miscellaneous</H2>

<UL>
<LI><A HREF="questions.html">Questions</A> - and maybe some answers..
<LI><A HREF="source.html">Source Code</A> - compiling it
<LI><A HREF="ports.html">Existing Ports</A> - Amiga rulez? Not really.
<LI><A HREF="others.html">Other html-Extensions</A> - pre²protranscessingpost
<LI><A HREF="bugs.html">Known Bugs, Problems and Limitations</A> - don't whizz on the electric fence
<LI><A HREF="future.html">Future?</A> - No future!
<LI><A HREF="related.html">Related stuff</A> - tools and further information
</UL>

<HR>

<$let hsc.format.time="%d-%b-%Y">
<ADDRESS>
  <A HREF=":author.html">Thomas Aglassinger</A> (agi@giga.or.at),
  <(gettime())>
</ADDRESS>

</html-only>

</BODY></HTML>

