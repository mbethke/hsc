<WEBPAGE chapter="hsc - " title="Future?"
    PREV="bugs.html"
    NEXT="related.html"
    QTEXT=("My future is static<BR>"
          +"It's already had it")
    QAUTHOR='Sonic Youth, "Schizophrenia"'>

<p>Except for bufixes, there will not be any further updates for <hsc>.</p>

<p>Ok, that was a bit rude, so now less brutal: I've abandoned this
project, but basically I'm still around and read your email. So minor
bugfixes will still happen, although more complicated problems will
simply end up on the known bugs list. Possible minor fixes relate
especially to the quite a lot of Amiga-specific features introduced in
version 0.917.</p>

<p>For the religious part: I still use my crappy Amiga 2000, I still
don't own a PC and I still have a Macintosh rotting in the attic. This
is not going to change in near future. Especially because none of the
ultra-alternative systems like pOS ever left alpha stage or stalled
like the BeBox, which meanwhile degenerated to BeOS, mostly used as
toy by Windiots who want to pretend they are different. If somebody
would create a reasonable computer, I would buy it. Currently it
doesn't look like this is going to happen in near future, therefor:
Amiga forever. (Shit! How I hate this outdated piece of crap, but
still it is the least useless I know.)</p>

<p>As <hsc> comes with full source code, somebody else might take over
the project. However, I hope this is not going to happen. It's
probably more efficient to start such a project from the scratch,
trying to learn from the mistakes that were made in <hsc>. You better
leave the source as it is, because the plain ANSI-C portability is one
of its main features - at least in my opinion.</p>

<p>Currently I don't think about starting to code a new <hsc>.</p>

<p>Anyway, as also html and browser development has stalled, <hsc>
should remain useful for quite some time to come.</p>

<*
<p>Basically, I consider <hsc> finished. However, some people have
convinced me that there are some things that should be implemented
before I release the final version.</p>

<H2>Minor Enhancements</H2>

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

<H2>Major Enhancements</H2>

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
<LI><STRONG>New copyright</STRONG>: This version probably is the last release
    distributed under GPL. The major lack of the GPL seems to be that I can't
    prevent someone else porting <hsc> to a MS-DOS-based system. But I don't
    think about not including the source or getting incredibly rich.
</UL>

<H2>Only If I'm Bored</H2>

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

<H2>Things Someone Else Can Do</H2>

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

<H2>No Future</H2>

For the thinggies below, from my point of view, there is no need to
ever be implemented. 

<UL>
<LI><STRONG>xml-support</STRONG>: The good thing about xml is that it confesses that
    html is impotent crap and sgml is unuseable. The bad thing is that xml is both.
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
    (very relaxed) checking would not fit into the rather typeless
    concept if plain html.
<LI><STRONG>Move around in text using <TG>$goto</TG> and <TG>$label</TG></STRONG>:
    This one's perverted to the core.
<LI><STRONG>Support other output-formats like texinfo or AmigaGuide</STRONG>: There
    are converters around for this task, and several people smarter than me
    have already failed to introduce the ultimate hypertext authoring tool.
</UL>
*>

</WEBPAGE>
