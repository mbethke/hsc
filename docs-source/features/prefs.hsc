<DOCPAGE chapter="hsc - Features - " title="Syntax Definition"
         PREV="if.html"
         NEXT="exec.html">

<P>Everything <hsc> knows about HTML, it retrieves from a
file named <hsc.prefs> at startup. This file contains
information about all tags, entities and icon entites.
Additionally, some <A HREF="spcattr.html">special
attributes</A> are set up there also.</P>

<P>The main advantage of this concept is that it's rather
easy to add new syntax elements. For this purpose the <hsc> tags
<ln-deftag>, <ln-defent>, <ln-defstyle> and <ln-deficon> can be used.</P>

<H2><A NAME="default">Default Preferences</A></H2>

<P>It is a serious problem about HTML that no one can give you competent
answer to the question <qq>Now which tags are part of HTML?</qq>. On the
one hand, there is w3c, which you meanwhile can ignore, on the other
hand, there are the developers of popular browsers, which implement
whatever they just like.</P>

<P>The <hsc.prefs> coming with this distribution should support most elements
needed for everyday use. With the <hsc> V0.923 release, the prefs have been
updated to HTML 4.01; since V0.925 there has also been support for
automatic distinction between <qq>classic</qq> HTML and XHTML. If you run <hsc> in
XHTML mode, some obsolete attributes will not be known any more, and new ones
added.</P>

<H2><A NAME="search">Searching For The Preferences</A></H2>

If you do not explicitly specify certain preferences by means of the
commandline option <op-prefsfile>, <hsc> will look in several places when
trying to open <hsc.prefs>:

<UL>
    <LI>the current directory</li>
    <LI>the directory specified in the environment variable <env-hscpath></li>
    <LI><code>$HOME/lib</code></li>
    <LI><UL>
	<li>Unixoid: <code>/usr/local/lib/hsc/</code> and <code>/usr/lib/hsc/</code></li>
	<LI>AmigaOS: the directory <CODE>PROGDIR:</CODE>, which is automatically
        assigned to the same directory where the <hsc> binary resides
        when <hsc> is invoked</li>
	</ul></li>
</UL>

<P>If it is unable to find <hsc.prefs> anywhere, it will abort with
an error message.</P>

<P>If you want to find out where <hsc> has read <hsc.prefs> from,
you can use <KBD><A HREF=":options.html#status">STATUS=VERBOSE</A></KBD>
when invoking <hsc>. This will display the preferences used.</P>

<H2><A NAME="tags">Special Tags To Modify Syntax Definition</A></H2>

<$macro SPCTAG NAME:string/r TITLE:string/r>
<H3><A NAME=(name)><(NAME)>: <(title)></A></H3>
</$macro>

All the tags below should be used within <hsc.prefs> only.

<SPCTAG NAME="defent" TITLE="Define an entity">

<P>This tag defines
a new entity. The (required) attribute <CODE>NAME</CODE> declares the
name of the entity, <CODE>RPLC</CODE> the character that should be
replaced by this entity if found in the hsc-source and <CODE>NUM</CODE>
is the numeric representation of this entity. <CODE>NUM</CODE> may be in the
range 128-65535, allowing for any Unicode (UCS-2 to be exact) character to be
assigned a corresponding entity. Definitions in the range 128-255 are done in the
prefs-file to allow users with character sets other than ISO-8859-1 (Latin-1)
to change the replacement characters; some other characters such as
mathematical symbols or typographical entities are predefined internally by
<hsc>. They reside at fixed positions in the Unicode charset and are unlikely to
ever change.</P>

<P><STRONG>Example:</STRONG> <TG>$defent NAME="uuml" RPLC="ü" NUM="252"</TG></P>

<p>The <a href=":options.html#entitystyle"><code>ENTITYSTYLE</code></a>
commandline option affects the way <hsc> will render entities in the resulting
HTML file. Setting the <code>PREFNUM</code> attribute for an entity will make it
use the numeric representation if <code>ENTITYSTYLE=replace</code>, no matter
what representation was used in the source text.</p>

<p>Unlike previous versions, <hsc> 0.931 and later allow redefinition of
entities. In this case, symbolic and numeric representation must match the
previous definition; only the <code>PREFNUM</code> flag  and the
<code>RPLC</code> character will be updated. This allows to change the default
rendering/replacement of internally defined entities.
<a href=":message-list.html#message.92">Warning #92</a> will be issued and should be
<a href=":options.html#ignore">ignored</a> if you really want to do this.</p>

<SPCTAG NAME="deficon" TITLE="Define icon-entity">

<P>This tag defines
a new icon-entity. The only (required) attribute is <CODE>NAME</CODE>
which declares the name of the icon.</P>

<P><STRONG>Example:</STRONG> <TG>$deficon NAME="mail"</TG></P>

<SPCTAG NAME="deftag" TITLE="Define a tag">

<P>This tag defines
a new tag, and is used quite similar to <ln-macro>, exept that a
tag-definition requires no macro-text and end-tag to be followed.</P>

<P><STRONG>Example:</STRONG> <TG>$deftag IMG SRC:uri/x/z/r ALT:string ALIGN:enum("top|bottom|middle") ISMAP:bool WIDTH:string HEIGHT:string</TG></P>

<P>To fully understand the above line, you might also want to read the sections
about <A HREF=":macro/attrib.html">attributes</A> 
and <A HREF=":macro/flag.html">options</A> for tags and macros.</P>

<P>For those, who are not smart enough or simply to lazy, here are some
simple examples, which should also work somehow, though some features of
<hsc> might not work:</P>

<$source PRE>
<$deftag BODY /CLOSE BGCOLOR:string>
<$deftag IMG SRC:uri ALT:string ALIGN:string ISMAP:bool>
</$source>

<SPCTAG NAME="defstyle" TITLE="Define a CSS property">
<p>This tag lets you define a new CSS property and optionally a list of values
that are allowed for it. If you omit the <code>VAL</code> attribute, any value
will be permitted. Otherwise it should be a list in pretty much the same style as for
<code>enum</code> parameters: words (which may include spaces) separated by
vertical bars.</p>
<$source pre>
<$defstyle name="text-align" val="left|center|right|justify">
<$defstyle name="text-indent" val="%P">
<$defstyle name="clip" val="%r|auto">
</$source>
<p>The <code>text-align</code> property has a short list of four possible
values, so they are simply listed as an enumeration. <code>text-indent</code> on
the other hand is numeric, so its values cannot be listed exhaustively.
Therefore, a special code resembling C-style format strings is used. The
following are supported:</p>
<dl>
<dt>d</dt><dd>Decimal digits only, e.g. for the <code>z-index</code> property.</dd>
<dt>n</dt><dd>Positive numeric value with a unit (one of
		pt, pc, in, mm, cm, px, em or ex), e.g. for <code>word-spacing</code>.</dd>
<dt>N</dt><dd>Numeric value as above, but possibly negative. Currently just for
		completeness and not used in CSS 2.0</dd>
<dt>p</dt><dd>Numeric value as for <code>%n</code>, but also allows percentages
		e.g. <code>font-size</code>.</dd>
<dt>P</dt><dd>Numeric value as above, but possibly negative, e.g. for
		<code>text-indent</code>.</dd>
<dt>c</dt><dd>A color specification as for <code>background-color</code>. One of
		<ul>
		<li>Color name as defined in
			<a href=":features/spcattr.html#colornames"><code>HSC.COLOR-NAMES</code></a>.</li>
		<li>Hexadecimal colorspec of the form <qq><code>#rgb</code></qq>
			or <qq><code>#rrggbb</code></qq>.</li>
		<li>RGB-style spec of the form <qq><code>rgb(r,g,b)</code></qq>,
			where each of r, g and b may be a decimal value between 0 and
			255 or a percentage between 0 and 100.</li>
		</ul></dd>
<dt>u</dt><dd>A URI of the form <qq><code>uri(</code>...<code>)</code></qq>,
		e.g. for <code>background-image</code>.</dd>
<dt>r</dt><dd>A rectangle of the form <qq><code>rect(a,b,c,d)</code></qq> with
		a, b, c and d being numeric specs with a dimension, e.g. for
		<code>clip</code>.</dd>
</dl>
<p><strong>Note:</strong> If both the above placeholdes and an enumeration of
values are used, as for <qq><code>clip</code></qq>, the placeholder
<em>must</em> be the <em>first</em> element!</p>

<SPCTAG NAME="varlist" TITLE="Define an attribute list shortcut">

<p>This tag defines an attribute list shortcut to support your laziness
when editing the prefs file. It allows to collect an arbitrary number of
attribute declarations under a single name that you can use later in
<TG>$deftag</TG> or <TG>$macro</TG> tags by putting the shortcut name in
square brackets.</p>

<p><STRONG>Example:</STRONG><BR><TG>$varlist HVALIGN
ALIGN:enum("left|center|right|justify|char")
VALIGN:enum("top|middle|bottom|baseline")</TG><BR>
<TG>$deftag THEAD /AUTOCLOSE /LAZY=(__attrs) /MBI="table" [HVALIGN]</TG></p>

<p>This is the same as:</p>

<p><TG>$deftag THEAD /AUTOCLOSE /LAZY=(__attrs) /MBI="table"
ALIGN:enum("left|center|right|justify|char")
VALIGN:enum("top|middle|bottom|baseline")</TG></p>

<H2>Why It Can Not Read DTDs</H2>

DTD is short for Document Type Definition. One of the early concept of
HTML was that the first line of a document should contain a line that
tells which DTD has been used to create this document. This could look
like

<$source PRE><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN"></$source>

<P>Browsers should read that line, obtain the DTD and parse the source
according to it. The problem about DTDs: they are written in SGML. And
the problem about SGML: It's awful. It's unreadable. It's a pure
brain-wanking concept born by some wireheads probably never seriously
thinking about using it themselves. Even when there is free code
available to SGML-parse text.</P>

<P>As a result, only less browsers did support this because it was too
easy to write a browser spitting on the SGML-trash, simply parsing the
code <qq>tag-by-tag</qq>, developers decided to spend more time on
making their product more user-friendly than computer-friendly (which
is really understandable).</P>

<P>These browsers became even more popular when they supported tags
certain people liked, but were not part of DTDs. As DTDs were
published by w3c, and w3c did not like those tags, they did not made
it into DTDs for a long time or even not at all (which is really
understandable, too).</P>

<P>This did work for a certain degree until HTML-2.0. Several people
(at least most of the serious w3-authoring people) did prefer to
conform to w3c than use the funky-crazy-cool tags of some special
browsers, and the funky-crazy-cool people did not care about DTDs or
HTML-validators anyway.</P>

<P>However, after HTML-2.0, w3c fucked up. They proposed the infamous
HTML-3.0 standard, which was never officially released, and tried to
ignore things most browsers did already have implemented (which not
all of them were useless crap, I daresay.). After more than a year
without any remarkable news from w3c, they finally canceled HTML-3.0,
and instead came out with the pathetic HTML-0.32.</P>

<P>Nevertheless, many people were very happy about HTML-0.32, as it
finally was a statement after that many things became clear. It became
clear that you should not expect anything useful from w3c anymore. It
became clear that the browser developers rule. It became clear that no
one is going to provide useful DTDs in future, as browser developers
are too lazy and incompetent to do so. It became clear that anarchy
has broken out for HTML-specifications.</P>

<$macro even-sgml>
    even without reading three books about SGML and analysing
    20 example sources
</$macro>

So, as a conclusion, reasons not to use DTDs but an own format are:
<UL>
<LI>It's more readable and users can change it easier, <even-sgml>.
<LI>It has been easier for me to implement, <even-sgml>.
<LI>One does not depend on w3c. (Sad that this is a reason.)
<LI>The format for <hsc.prefs> contains some information which makes
    is possible to for example evaluate image sizes or strip tags with
    absolute URIs. This is realised with special attribute modifiers
    which have no equivalent in DTDs.
</UL>

<P>Quite unexpectedly, with HTML-4.0 this has changed to some extent,
as the DTDs are quite readable and well documented. The general syntax
of course still sucks, error handling is unbearable for <qq>normal</qq> users
and so on. Although it will take them more than this to get back the
trust they abused in the recent years, at least it is a little signal
suggesting there are some small pieces of brain intact somewhere in
this consortium.</P>

<H2>Problems</H2>

<P>There is also a disadvantage of this concept: reading <hsc.prefs>
every time on startup needs an awful lot of time. Usually, processing
your main data takes shorter than reading the preferences. You can
reduce this time, if you create your own <hsc.prefs> with all tags and
entities you don't need removed. But I recommend to avoid this because
you might have to edit your preferences again with the next update of
<hsc>, if any new features have been added.</P>

</DOCPAGE>
