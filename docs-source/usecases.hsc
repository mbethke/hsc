<DOCPAGE chapter="hsc - " title="Some Use-Cases"
    PREV="project/hscpaltrow.html"
    NEXT="questions.html">


This chapter gives some hints how you can do certain things you might
be interested in. It is probably only interesting for experienced
users.

<********************************************************************>

<H2>Multi-lingual Documents</H2>

<********************************************************************>

This will shortly outline how to create multiple versions of one
source file, for example to support more than one language. For
example, you want to have a document with a picture being the same in
all flavours, but a text depending on the language. Maybe you have a
source file <FILE>hugo.hsc</FILE> with the interesting part looking
like

<$source PRE>
<img src="hugo.gif" alt="hugo>
<english>This is Hugo.</english>
<suomi>Tämä on Hugo.</suomi>
</$source>

and as result, you want two documents: an English one

<$source PRE>
<img src="hugo.gif" alt="hugo>
This is Hugo.
</$source>

and a Finnish one

<$source PRE>
<img src="hugo.gif" alt="hugo>
Tämä on Hugo.
</$source>

This can easily be achieved by defined two macro sets, one
being stored as <FILE>english.hsc</FILE>

<$source PRE>
<$macro english /close><$content></$english>
<$macro suomi /close></$suomi>
</$source>

and another one stored as <FILE>suomi.hsc</FILE>

<$source PRE>
<$macro english /close></$english>
<$macro suomi /close><$content></$suomi>
</$source>

<P>The first one defines two container macros, with <TG>english</TG>
simply every time inserting the whole content passed to it, and
<TG>suomi</TG> always removing any content enclosed in it.</P>

If you now invoke <hsc> with a call like

<PRE>hsc english.hsc hugo.hsc to en-hugo.html</PRE>

it will look like the first output document described above. To gain
a result looking like the second one, you only have to use

<PRE>hsc suomi.hsc hugo.hsc to fi-hugo.html</PRE>

<P>This is simply because the macros declared in
<FILE>suomi.hsc</FILE> work just the other way round like those in
<FILE>english.hsc</FILE>: everything enclosed in <TG>english</TG> will
be ignored, and everything being part of <TG>suomi</TG> remains.</P>

Of course you can have multiple occurrences of both macros, and of
course you can define similar macros for other languages.

<********************************************************************>

<H2>Extended Character Encodings</H2>

<********************************************************************>

<P>This version of <hsc> officially only supports Latin-1 as input
character set. The exact definition of that is a bit messy, but
basically it refers to most of those 255 characters you can input on
your keyboard.</P>

<P>For this character set, all functions described herein should work,
especially the commandline option <op-rplcent>.</P>

<P>Although Latin-1 is widely used within most decadent western
countries, it does not provide all characters some people might need.
For instance those from China and Japan, as their writing systems work
completely different.</P>

<P>As the trivial idea if Latin was to use 8 bit instead of the rotten
7 bit of ASCII (note that the <qq>A</qq> in ASCII, stands for
American), the trivial idea of popular encodings like JIS, Shift-JIS or
EUC is to use 8 to 24 bit to encode one character.</P>

<P>Now what does <hsc> say if you feed such a document to it?</P>

<P>Unless you do not specify <KBD>RPLCENT</KBD>, it should work
without much bothering about it. However, you will need a w3-browser
that also can display these encodings, and some fiddling with
<TG>META</TG> and related tags.</P>

<P>If you think you are funny and enable <KBD>RPLCENT</KBD>, <hsc>
will still not mind your input. But with great pleasure it will cut
all your nice multi-byte characters into decadent western 8-bit
<qq>cripplets</qq> (note the pun). And your browser will display loads
of funny western characters - but not a single funny Japanese one.</P>

<P>Recently an old western approach to these encodings problems has
gained popularity: Unicode - that's the name of the beast - was
created as some waste product of the Taligent project around 1988 or
so, as far as I recall.</P>

<P>Initially created as an unpopular gadget not supported by anything,
it is now in everybody's mouth, because Java, the language-of-hype,
several MS-DOS based operating systems and now - finally - the rotten
hypertext language-of-hype support it. At least to some limited
extent. (Technical note: Usually you only read of UCS-2 instead of
UCS-4 in all those specifications, and maybe some blurred proposals to
use UTF-16 later.)</P>

<P>As <hsc> is written in the rotten C-language (an American product,
by the way), it can not cope with zero-bytes in its input data, and
therefore is unable to read data encoded in UCS-4, UTF-16 or (würg,
kotz, reiha) UCS-2; it simply will stop after the first zero in the
input.</P>

<P>Because the rotten C-language is so widely used, there are some
zero-byte work-around formats for Unicode, most remarkably UTF-8 and
UTF-7. These work together with <hsc>, although with the same
limitations you have to care for when using the eastern encodings
mentioned earlier. Read: Don't use the option <code>RplcEnt</code>.</P>

<P>Note that it needs at least five encodings to make Unicode work
with most software - again in alphabetical order: UCS-2, UCS-4,
UTF-16, UTF-7 and UTF-8. I wonder what the <qq>Uni</qq> stands for...</P>

Anyway, as conclusion: you can use several extended character sets,
but you must not enable <KBD>RPLCENT</KBD>.

<********************************************************************>

<H2>Html 4.0</H2>

<********************************************************************>

<P>Once upon a time, HTML-4.0 was released, and it sucked surprisingly
less (as far as <qq>sucks less</qq> is applicable at all to HTML). Of course
there still is no browser capable of displaying all these things,
but nevertheless you can use <hsc> to author for it - with some
limitations. This will shortly outline how.</P>

<P>As already mentioned, HTML now supports those extended character
encodings. See above how to deal with input files using such an
encoding, and which to avoid.</P>

<P>If your system does not allow you to input funny characters (for
instance one can easily spend ATS 500.000 on a Workstation just for
being absolutely unable to enter a simple <qq>ä</qq>), you can use
numeric entities, both in their decimal or hexadecimal representation:
for example, to insert a Greek Alpha, you can use
<CODE>&amp;#913</CODE> or <CODE>&amp;#x391</CODE>, <hsc> will accept
both. However, you still can not define entities beyond 8-bit range
using <ln-defent>.</P>

<P>Some highlights are that the <CODE>ALT</CODE> attribute of <TG>IMG</TG>
is no required and that there are now loads of <qq>URIs</qq> instead of
<qq>URLs</qq> around. Nothing new for old <hsc>-users... he he he.</P>

<P>Another interesting thing is that the DTD now contains some
meta-information that was not part of earlier DTDs so it maybe can
make sense to use the DTD as a base for a <hsc.prefs> converter.</P>

<********************************************************************>

<H2>XHTML</H2>

<********************************************************************>

<P>2002 update: seems the W3 committee has learned a thing or two. XHTML has
been out for a while now, and they are working on the 2.0 specification. While
the chances of turning the official DTD into an HSC prefs file using a dumb
ARexx script have gotten even slimmer (anyone for a real parser using Expat or
something?), XHTML seems a move in the right direction, regarding the
separation of content and presentation and putting an end to the <qq>tag
soup</qq> that much of the Web is today. It remains to be seen how successful
it will be. HSC now has some rudimentary support for authoring XHTML documents,
mainly regarding lowercase tag and attribute names and the new empty-tag syntax
with a trailing slash, as in <qq><tg>br /</tg></qq>. CSS support should be
better though, perhaps some automatic rewriting of obsolete presentation
attributes to CSS <tg>style</tg> tags...</p>

<********************************************************************>

<H2>Creating A Postscript Version</H2>

<********************************************************************>

<$macro link-include file:uri/required>
    <A HREF=(":../"+file)><FILE><(file)></FILE></A>
</$macro>

<P>As you can now optionally read this manual in a Postscript version,
there might be some interest how it was done.</P>

<P>The rudimentarily bearable application used for conversion is (very
originally) called <EXEC>html2ps</EXEC> and can be obtained from <A
HREF="http://www.tdb.uu.se/~jan/html2ps.html"><(hsc.Anchor)></A>. As
common with such tools, <qq>it started out as a small hack</qq> and <qq>what
really needs to be done is a complete rewriting of the code", but "it
is quite unlikely that this [...] will take place</qq>. The usual standard
disclaimer of every public Perl-script. All quotes taken from the
manual to <EXEC>html2ps</EXEC>.</P>

<P>Basically the HTML  and the Postscript-version contain the same
words. However, there are still some differences, for example the
printed version does not need the toolbar for navigation provided at
the top of every HTML page.</P>

<P>Therefore, I wrote two macros, <TG>html-only</TG> and
<TG>postscript-only</TG>. The principle works exactly like the one
described for <TG>english</TG> and <TG>suomi</TG> earlier in this
chapter, and you can find them in <link-include
file="docs-source/inc/html.hsc"> and <link-include
file="docs-source/inc/ps.hsc">.</P>

<P>However, there is a small difference to the multi-lingual examples,
as I do not really want to create two versions all the time. Instead,
I prefer to create either a fully hypertext featured version or
a crippled Postscript-prepared HTML document in the same location.</P>

<P>You can inspect <link-include file="docs-source/Makefile"> how this
is done: if <make> is invoked without any special options, the
hypertext version is created. But if you instead use <EXEC>make
PS=1</EXEC> and therefor define a symbol named <CODE>PS</CODE>, the
pattern rule responsible for creating the HTML documents acts
differently and produces a reduced, Postscript-prepared document
without toolbar.</P>

Basically, the rule looks like this:

<$source PRE>
$(DESTDIR)%.html : %.hsc
ifdef PS
        @$(HSC) inc/ps.hsc   $(HSCFLAGS) $<
else
        @$(HSC) inc/html.hsc $(HSCFLAGS) $<
endif
</$source>

<P>Needless to say that the conditional in the <makefile> does not
work with every <make> - I used <EXEC>GNUmake</EXEC> for that, your
<make>-tool maybe has a slightly different syntax.</P>

<P>For my convenience, there are two rules called <CODE>rebuild</CODE>
and <CODE>rebuild_ps</CODE> with their meanings being obvious: they
rebuild the whole manual in the desired flavour.</P>

<P>So after a successful <CODE>make rebuild_ps</CODE>, everything only
waits for <EXEC>html2ps</EXEC>. Maybe you want to have a look at the
<link-include file="docs-source/html2ps.config"> used, although it is
strait forward and does not contain anything special. This should not
need any further comments, as there is a quite useful manual supplied
with it.</P>

<P>However, making <EXEC>html2ps</EXEC> work with an Amiga deserves some
remarks. As you might already have guessed, you will need the
Perl-archives of GG/ADE - no comments on that, everybody interested
should know what and where GG is.</P>

I suppose you can try the full Unix-alike approach with <hsc> compiled
for AmigaOS/ixemul and GG more or less taking over your machine, and
therefor directly invoke <EXEC>perl</EXEC>. This will require a rule
like

<PRE>
ps :
        html2ps -W l -f html2ps.config -o ../../hsc.ps ../docs/index.html
</PRE>

<P>As I am a dedicated hater of this, I used the AmigaOS-binary, a
SAS-compiled <EXEC>GNUmake</EXEC> and the standard CLI. A usually
quite successful way to make such things work is with the help of
<EXEC>ksh</EXEC>, which, for your confusion, is in a archive at GG
called something like <FILE>pdksh-xxx.tgz</FILE> (for <qq>Public
Domain ksh</qq>). Invoking <EXEC>ksh</EXEC> with no arguments will
start a whole shell-session (würg!), but you can use the switch
<KBD>-c</KBD> to pass a single command to be executed. After that,
<EXEC>ksh</EXEC> will automatically exit, and you are back in your
cosy CLI, just as if nothing evil had had happened seconds before.</P>

<P>So finally the rule to convert all those HTML files into one
huge Postscript file on my machine is:

<PRE>
ps :
        ksh -c "perl /bin/html2ps -W l -f html2ps.config -o ../../hsc.ps ../docs/index.html"
</PRE>

<P>Note that <EXEC>html2ps</EXEC> is smart enough to follow those
(normally invisible) <TG>LINK REL="next" ..</TG> tags being part of
the HTML documents, so only the first file is provided as argument,
and it will automatically convert the other ones.</P>

<P>Well, it least you see it can be done.</P>

</DOCPAGE>

