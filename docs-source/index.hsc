<HTML><HEAD>
  <$define DESCRIPTION:string=("This is the documentation to hsc, a "
    +"developer tool for html-projects. It acts as a preprocessors "
    +"and supports include files, macros, expressions, conditionals, "
    +" replaces special characters, performs a structure check, tests "
    +" existence of local links and several other things.")>
  <TITLE>hsc - html sucks completely</TITLE>
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

<P><STRONG>(Version 0.9.10 <*(PRE-RELEASE#3)*>, 30-Sep-1996)</STRONG></P>

<P><(DESCRIPTION)></P>

<IMG SRC="image/hsc.gif" ALT="..SUCK SUCK SUCK.." ALIGN="right">

<H2>About</H2>
<UL>
<LI>What's this all <A HREF="about.html">about</A>?
<LI><A HREF="copy.html">Copyright and disclaimer</A> - the usual legal stuff
<LI><A HREF="require.html">Requirements</A> - what to own and know
<LI><A HREF="distrib.html">Archive contents</A> - things coming along
<LI><A HREF="../CHANGES">Changes</A> - what's new with this release
</UL>

<H2>Usage</H2>
<UL>
<LI><A HREF="install.html">Installation</A> - where to put the whole 
    damn thing
<LI><A HREF="options.html">Invoking <hsc></A> - Options and swiches
<LI><A HREF="examples.html">Example usage</A> - how it is supposed to work
<LI><A HREF="messages.html">Messages</A> - shit happens
<LI><A HREF="questions.html">Questions</A> (and maybe some answers)
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
<LI><A HREF="macro/macros.html">Macros</A>
    - create your own shortcuts and templates.
<LI><A HREF="features/expressions.html">Expressions</A>
    - there's more to life than strings
<LI><A HREF="features/if.html">Conditionals</A>
    - what if..?
<LI><A HREF="features/prefs.html">Syntax definition</A>
    - what to do if html-version-17.3 is out
<LI><A HREF="features/exec.html">Execute shell-commands</A>
    - poor man's pseudo-CGI 
<LI><A HREF="project/index.html">Project management</A>
    - I'll <make> ya
</UL>

<H2>Stuff</H2>
<UL>
<LI><A HREF="updates.html">Updates</A> - where to get them and
    how to reach the author
<LI>About the <A HREF="source.html">sources</A>
<LI><A HREF="bugs.html">Known bugs &amp; limitations</A>
<LI><A HREF="future.html">Future Improvements</A> - what might happen
<LI><A HREF="related.html">Related tools</A> and further information
</UL>

<HR>
<$let hsc.format.time="%d-%b-%Y">
<ADDRESS>
  Thomas Aglassinger (<A HREF="mailto:agi@giga.or.at">agi@giga.or.at</A>),
  <(gettime())>
</ADDRESS>
</BODY></HTML>
