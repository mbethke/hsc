<WEBPAGE chapter="hsc - " title="Questions and Answers"
    PREV="messages.html"
    NEXT="bugs.html"
    QTEXT=("Feel like a blind man with a color tv<BR>"
          +"Everything's fine but I just can't see")
    QAUTHOR='Element of Crime, "Almost dead"'>

<$MACRO QUEST NAME:string>
<$if COND=(SET NAME)><A NAME=(Name)></$if>
<STRONG>Q:</STRONG><$if COND=(SET Name)></A> </$if>

</$MACRO>

<$MACRO ANSWR>
<STRONG>A:</STRONG> <* <-- blank *>
</$MACRO>

<P><QUEST NAME="unknown">What the hell..?! <hsc> doesn't know about
the tag <TG>XYZ</TG> (the attribute <CODE>ZYX</CODE>),
but I need this one!<BR>

<ANSWR>Read the section about
<A HREF="features/prefs.html">Syntax definition</A>
and how to extend it.</P>

<P><QUEST NAME="ignore">Argh! hsc tells me loads of warnings
I don't care about! Is there a way to prevent it from doing
this?<BR>

<ANSWR>All non-error messages can be suppressed adding a
simple <KBD>IGNORE=<I>message_id</I></KBD> to the call used to
invoke <hsc>. Read the section about
<A HREF="options.html">Options and switches</A> to find out
more.</P>

<P><QUEST NAME="uri">Why do You call it URI? I thought it's URL (<EM>Universal Resource
Locator</EM> or <EM>U R lost</EM>)?<BR>

<ANSWR>Ok, put a finger into your mouth and try to reach as far inside
as possible. After some specific point, you will empty your stomach.
And what do you say then? Exactly, "<STRONG>URL</STRONG>"! So the main
reason why I do not like the term URL is because it always reminds me
of puking.</P>

Additionally, the Internet-Draft for HTML 2.0, June 16, 1995, tells
you:

<PRE>
    URI
            A Universal Resource Identifier is a formatted string
            that serves as an identifier for a resource, typically
            on the Internet. URIs are used in HTML to identify the
            destination of hyperlinks. URIs in common practice
            include Uniform Resource Locators (URLs)[URL] and
            Relative URLs [RELURL].
</PRE>
<P>I really like that term URI a lot more. Altough, I've never really understood
what's the difference between URL and URI. But who cares anyway?</P>

<P><QUEST NAME="piss">But URI reminds me of pissing! Add a simple `n',
and there we are!<BR>
<ANSWR>..which I personally prefer to puking. Certainly a matter
of taste.</P>

<QUEST NAME="jerk">What are jerks?<BR>
<ANSWR>Jerks are persons suffered by a well-known desease called
<EM>Netscapissm</EM>. Their sources contain sections like
<PRE>
    <TG>BODY BGCOLOR=#123456</TG>
    <TG>BLINK</TG>Click here!<TG>/BLINK</TG>
</PRE>
<P>But, not all jerks became jerks because they like to be a jerk. Very often,
jerks are forced to use code as seen above by their employee.</P>

<P><QUEST>Why is <hsc> that slow?<BR>
<ANSWR>One reason is, that <hsc> handles (nearly) all resources dynamically
(Techn. Note: That means, it often calls <CODE>malloc()</CODE> and
<CODE>free()</CODE>, which are known to be quite slow).</P>

<P>Another, quite embarrasing, reason is, that most of these resources are
kept in linked lists. And therefor, if <hsc> has to look for something, it
sequentially searches these list. Shame on me, I should use an AVL-tree for
such things. But the problem with AVL-tree is: though there are lots of
sources around, most of them are perverted `real' C-sources perpetrated by some
braindead Unix-fosilles, ignoring the fact that memory can run out or that
there is a "-Wall" option in most compilers; some of these sources also
date back to 1863, when no one even pretended there is a language definition
for <CITE>C</CITE>. Currently I'm too lazy to
write (and, even more work, test) an AVL-tree myself.
Maybe in another life...</P>

</WEBPAGE>
