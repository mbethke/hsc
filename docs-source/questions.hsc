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
<STRONG>A:</STRONG>&nbsp;</$MACRO>

<* define shortcuts for question-text *>
<$macro Q.lha>What about those strange <CODE>.lha</CODE> files?</$macro>
<$macro Q.lha.corrupt>I've downloaded the <CODE>.lha</CODE>-archives using my WWW-Browser, but they seem to be corrupt?!</$macro>
<$macro Q.lha.why>Why <CODE>.lha</CODE> at all?!</$macro>

<$macro Q.unknown><hsc> doesn't know about the tag <TG>XYZ</TG> (the attribute <CODE>ZYX</CODE>), but I need this one!</$macro>
<$macro Q.ignore><hsc> tells me loads of warnings I don't care about! Is there a way to prevent it from doing this?</$macro>
<$macro Q.tool>Somewhere in these manual You are talking about a tool called <EXEC>make/weblint/..</EXEC>. Where can I obtain it from?</$macro>
<$macro Q.slow>Why is <hsc> that slow?</$macro>

<$macro Q.uri>Why do You call it URI? I thought it's URL (<EM>Universal Resource Locator</EM> or <EM>U R lost</EM>)?</$macro>
<$macro Q.piss>But URI reminds me of pissing! Add a simple <qq>n</qq>, and there we are!</$macro>
<$macro Q.jerk>What are jerks?</$macro>

<P>This chapter provides a list of questions and answers which have come to
the mind of several people. Some of them are only neccessary because many
users don't read this manual, some of them are not very serious,.. but anyway,
here's a list of them:</P>

<STRONG>Questions dealing with the archives</STRONG>
<UL>
<LI><A HREF="#lha"><Q.lha></A>
<LI><A HREF="#lha.corrupt"><Q.lha.corrupt></A>
<*<LI><A HREF="#lha.why"><Q.lha.why></A>*>
</UL>

<STRONG>Questions about <hsc> and it's usage</STRONG>
<UL>
<LI><A HREF="#unknown"><Q.unknown></A>
<LI><A HREF="#ignore"><Q.ignore></A>
<LI><A HREF="#tool"><Q.tool></A>
<LI><A HREF="#slow"><Q.slow></A>
</UL>

<STRONG>Miscelleneous questions</STRONG>
<UL>
<LI><A HREF="#uri"><Q.uri></A>
<LI><A HREF="#piss"><Q.piss></A>
<LI><A HREF="#jerk"><Q.jerk></A>
</UL>

<HR>
<P><QUEST NAME="lha"><Q.lha><BR>
<ANSWR><FILE>LHA</FILE> is the standard compression utility for AmigaOS. 
A portable version of <FILE>lha</FILE> is available from 
<AMINET FILE="misc/unix/lha_1_00.tar.Z">.</P>

<P><QUEST NAME="lha.corrupt"><Q.lha.corrupt><BR>
<ANSWR>Seems that your browser has no MIME-type configured for 
<CODE>.lha</CODE>. Look up the manual of your browser how to configure
MIME-types.</P>
<*
<P><QUEST NAME="lha.why"><Q.lha.why><BR>
<ANSWR>Of course I know that <CODE>.lha</CODE>-archives can cause 
trouble to non-Amigoids. You should understand this as test for your
IQ being sufficient to deal with <hsc>. People who are not even able
to compile the portable version of <EXEC>LHA</EXEC> mentioned above 
(or find a already compiled one for their OS on the net) 
very probably won't be able to compile and use
<hsc>. You are allowed to feel insulted now.</P>*>

<HR>

<P><QUEST NAME="unknown"><Q.unknown><BR>
<ANSWR>Read the chapter about
<A HREF="features/prefs.html">Syntax definition</A>
and how to extend it.</P>

<P><QUEST NAME="ignore"><Q.Ignore><BR>
<ANSWR>All non-error messages can be suppressed adding a
simple <KBD>IGNORE=<I>message_id</I></KBD> to the call used to
invoke <hsc>. Take a look at the chapter about
<A HREF="options.html#ignore">Options and switches</A> to find out
more.</P>

<P><QUEST NAME="tool"><Q.tool><BR>
<ANSWR>All tools mentioned within these documents should be denoted
in the chapter about 
<A HREF="related.html">Related stuff</A>
(if not, let me know). Normally I only mention the
version for AmigaOS, but the archives or <CODE>ReadMe</CODE>s 
usually include some notes where to look for other systems.</P>

<P><QUEST NAME="slow"><Q.slow><BR>
<ANSWR>One reason is, that <hsc> handles (nearly) all resources dynamically
(<TechNote>That means, it often calls <CODE>malloc()</CODE> and
<CODE>free()</CODE>, which are known to be quite slow).</P>

<P>Another, quite embarrasing, reason is, that most of these resources are
kept in linked lists. And therefor, if <hsc> has to look for something, it
sequentially searches these list. Shame on me, I should use an AVL-tree for
such things. But the problem with AVL-tree is: though there are lots of
sources around, most of them are perverted <qq>real</qq> C-sources perpetrated by some
braindead Unix-fossiles, ignoring the fact that memory can run out or that
there is a "-Wall" option in most compilers; some of these sources also
date back to 1863, when no one even pretended there is a language definition
for <CITE>C</CITE>. Currently I'm too lazy to
write (and, even more work, test) an AVL-tree myself.
Maybe in another life...</P>

<HR>

<P><QUEST NAME="uri"><Q.uri><BR>
<ANSWR>Ok, put a finger into your mouth and try to reach as far inside
as possible. After some specific point, you will empty your stomach.
And what do you say then? Exactly, <qq><STRONG>URL</STRONG></qq>! So the main
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
And one of the users of <hsc> submitted this interpretation to me:
<PRE>
    To me it seems:

    URI = URL and RELURL
    URL = "http://www.intercom.no/~XXXXXX/index.html" - always the same doc
    RELURL = "docs/about.html"  - relative to which dir/machine you are on
</PRE>

<P>Anyway, I really like that term URI a lot more and I've never really 
understood what's the difference between URL and URI.
But who cares anyway?</P>

<P><QUEST NAME="piss"><Q.piss><BR>
<ANSWR>..which I personally prefer to puking. Certainly a matter
of taste.</P>

<QUEST NAME="jerk"><Q.jerk><BR>
<ANSWR>Jerks are persons suffered by a well-known desease called
<EM>Netscapissm</EM>. Their sources contain sections like
<PRE>
    <TG>BODY BGCOLOR=#123456</TG>
    <TG>BLINK</TG>Click here!<TG>/BLINK</TG>
</PRE>
<P>But, not all jerks became jerks because they like to be a jerk. Very often,
jerks are forced to use code as seen above by their employee. In this case,
they should be refered to as <qq>prostitutes</qq>.</P> 

</WEBPAGE>
