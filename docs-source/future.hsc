<WEBPAGE chapter="hsc - " title="Future improvements"
    PREV="bugs.html"
    NEXT="related.html"
    QTEXT=("My future is static<BR>"
          +"It's already had it<BR>"
          +"I could tuck you in<BR>"
          +"And we can talk about it")
    QAUTHOR='Sonic Youth, "Schizophrenia"'>

Basically, I consider <hsc> finished. However, some people have
convinced me that there are some things that should be implemented
before I release the final version.

<H2>Minor enhancements</H2>

<UL>

<LI><STRONG>Improve hscpitt</STRONG>: Add a copy/move command.

<LI><STRONG>Rename some message classes</STRONG>: message
    classes <qqc>bad style</qqc> and <qqc>fatal error</qqc> will
    probably be renamed to <qqc>flaw</qqc> and <qqc>failure</qqc>,
    so the class name will also consists of only one word. Furthermore,
    <qq>fatal</qq> sounds too hard.

<LI><STRONG>Rename internal attributes and filenames</STRONG>:
    currently, internal attributes are named like HSC.XY, and for
    filenames there are no templates at all. I will probably use the
    prefix <qq>hsc</qq> and a <hyphen> as separator, resulting into
    attributes like <CODE>hsc-click-here</CODE> and filenames like
    <FILE>hsc-XXXX.tmp</FILE>.

</UL>

<H2>Major enhancements</H2>

<UL>
<LI><STRONG>New project database</STRONG>: instead of a single project file,
    <hsc> should store it's data in separate files for every document. A
    document data file is only loaded during parsing if some information about a
    specific document is needed. This should speedup huge projects with more
    than 100 documents.
<LI><STRONG>Additional document data</STRONG>: Without remarkable changes,
    <hsc> could also collect useful information about a document, like
    document title, URIs referenced etc. Of course, some access functions
    will be provided, too.
<LI><STRONG>Index document creation</STRONG>: A new tag <TG>$index</TG> will
    allow the
    user to attach some private information to the document data, which later
    can be retrieved using <hscpitt>. A script the user will have to write
    can use these to create a html-object. Easy to implement for me, awfully
    cryptic and complicated, but rather flexible for the user.
<LI><STRONG>Checking of external URIs</STRONG>: I'm definitely not going to
    mess around with shitty TCP/IP-stuff, but a script program could
    retrieve information about HREFs from the document file, and pass them to
    an external program (like <EXEC>GetURL</EXEC>)
<LI><STRONG>Functions with multiple arguments</STRONG>: Functions like
    <CODE>Exists()</CODE> or <CODE>GetFileSize</CODE> should use the
    same attribute syntax like tags, for example
    <CODE>GetEnv(VAR="HOME")</CODE>.
<LI><STRONG>New copyright</STRONG>: Version 0.913 is probably the last release
    distributed under GPL. The major lack of the GPL seems to be that I can't
    prevent someone else porting <hsc> to a MS-DOS-based system. But I don't
    think about not including the source or getting incredibly rich.
</UL>

<H2>Only if I'm bored</H2>

<UL>
<LI><STRONG>More conditionals</STRONG>: <TG>$select</TG>, <TG>$when</TG> and
    <TG>$otherwise</TG> as an extended version of <TG>$if</TG>
<LI><STRONG>Indention</STRONG>: Add option <CODE>INDENT</CODE> for <TG>$include</TG>
    and <TG>$source</TG> to indent preformatted text
<LI><STRONG>Plausibility checking of external URIs</STRONG>:
    unknown protocol, missing domain etc.
<LI><STRONG>Improve expression parser</STRONG>: the current implementation
    is an embarrassment for a student of computer science, but.. well, it does
    it's job for now.
<LI><STRONG>GUI-Version for AmigaOS</STRONG>: I ought to play around with
    MUI sometimes anyway
<LI><STRONG>Filename compatibility mode</STRONG>: this mostly means that
    a <qqc>..</qqc> should act as parent directory on all systems without
    having to run additional system patches.
</UL>

<H2>Things someone else can do</H2>

<UL>
<LI><STRONG>Autoconf-support</STRONG>: In the unix-world, a cryptic tool package
    called autoconf has become quite popular, as it tries to compensate the
    lack that even at the end of the 20th centaury, no one knows in which
    header file to expect <CODE>strftime()</CODE> on this ridiculous system
    (ANSI has specified this in the early 80ies, bye the way). Although I don't
    care much about
    outdated unix-versions, and I don't like unreadable and badly documented
    macro languages, I might include the required changes if someone else
    wastes his time with setting up a working autoconf-installation.
<LI><STRONG>Improve hscdepp</STRONG>: As I do not consider dependency generators
    a reasonable concept, someone else will have to add things like excluding
    specific documents or pattern matching stuff. Hey, it's only about 700 lines of
    code..
<LI><STRONG>Document relation editor</STRONG>: It would be nice, if one could
    edit all those next/prev/up/.. relations with a program, and include these
    into hsc-sources later. It would make sense to store these relations in
    the document data file.
</UL>

<H2>No future</H2>

For the thinggies below, from my point of view, there is no need to
ever be implemented. 

<UL>
<LI><STRONG>XML-support</STRONG>: It's too ridiculous. If you want to have
    a jolly good laugh, try a html validator on
    <A HREF="http://www.w3.org/pub/WWW/TR/WD-xml-lang-970331.html"><(HSC.Anchor)></A>,
    to get an impression of how competent these people are. After that, read
    <A HREF="http://www.w3.org/pub/WWW/TR/WD-xml-lang.html"><(HSC.Anchor)></A>
    to laugh again... This time I'm really glad if all browser developers will
    ignore it; just another prove that w3c has not produced anything useful after
    html-2.0.
<LI><STRONG>Precompiled include files</STRONG>: This would
    also speed up reading <hsc.prefs>; I experimented around a bit
    with this, and it seems that more time is wasted scanning those
    bloody linked lists then by checking the data. It's more likely
    that some sort of balanced binary tree will make it into 
    the source.
<LI><STRONG>Undefine macros or attributes</STRONG>: I do not
    consider <qq>undefining</qq> a clean concept, but a result of
    mental impotence.
<LI><STRONG>Type checking for attributes</STRONG>: Anything more then the current
    (very relaxed) checking would not fit into the typeless
    concept if plain html.
<LI><STRONG>Move around in text using <TG>$goto</TG> and <TG>$label</TG></STRONG>:
    This one's perverted to the core.
<LI><STRONG>Support other output-formats like texinfo or AmigaGuide</STRONG>: There
    are converters around for this task, and several people smarter than me
    have already failed to introduce the ultimate hypertext authoring tool.
</UL>

</WEBPAGE>
