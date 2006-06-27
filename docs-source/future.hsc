<DOCPAGE chapter="hsc - " title="Future?"
    PREV="bugs.html"
    NEXT="related.html"
    QTEXT=("My future is static<BR>"
          +"It's already had it")
    QAUTHOR='Sonic Youth, "Schizophrenia"'>

<p>To be honest, I didn't think <hsc> would make it to a state that I can call
V1.0 without too much of a stomach ache. Neither am I sure anybody would agree
that this has been reached, but here it is, over 10 years after the initial
release and just late enough to bring the number of people who actually care
down to a few dozen. Anyway, <hsc> is still useful to me and I will occasionally
continue implementing one or another new thing to play around with. Agi thought
of some of the things a long time ago so I just put the list below back again.
Comments welcome.</p>

<H2>Minor Enhancements</H2>

<UL>

<LI><STRONG>Improve hscpitt</STRONG>: Add a copy/move command.

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
    can use these to create an HTML object. Easy to implement for me, awfully
    cryptic and complicated, but rather flexible for the user.
<LI><STRONG>Functions with multiple arguments</STRONG>: Functions like
    <CODE>Exists()</CODE> or <CODE>GetFileSize</CODE> should use the
    same attribute syntax like tags, for example
    <CODE>GetEnv(VAR="HOME")</CODE>.
</UL>

<H2>Only If I'm Bored</H2>

<UL>
<LI><STRONG>More conditionals</STRONG>: <TG>$select</TG>, <TG>$when</TG> and
    <TG>$otherwise</TG> as an extended version of <TG>$if</TG>
<LI><STRONG>Indention</STRONG>: Add option <CODE>INDENT</CODE> for <TG>$include</TG>
    and <TG>$source</TG> to indent preformatted text
<LI><STRONG>Improve expression parser</STRONG>: the current implementation
    is an embarrassment for a student of computer science, but.. well, it does
    it's job for now.
<LI><STRONG>Filename compatibility mode</STRONG>: this mostly means that
    a <qqc>..</qqc> should act as parent directory on all systems without
    having to run additional system patches.
</UL>

<H2>Things Someone Else Can Do</H2>

<UL>
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
<LI><STRONG>Type checking for attributes</STRONG>: Anything more then the current
    (very relaxed) checking would not fit into the rather typeless
    concept if plain HTML.
<LI><STRONG>Move around in text using <TG>$goto</TG> and <TG>$label</TG></STRONG>:
    This one's perverted to the core.
<LI><STRONG>Support other output-formats like texinfo or AmigaGuide</STRONG>: There
    are converters around for this task, and several people smarter than me
    have already failed to introduce the ultimate hypertext authoring tool.
</UL>

</DOCPAGE>
