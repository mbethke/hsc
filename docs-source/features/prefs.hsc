<WEBPAGE chapter="hsc - Features - " title="Syntax Definition"
         PREV="if.html"
         NEXT="exec.html">

<P>Everything <hsc> knows about html, it retrieves from a
file named <hsc.prefs> at startup. This file contains
information about all tags, entities and icon entites.
Additionally, some <A HREF="spcattr.html">special
attributes</A> are set up there also.</P>

<P>The main advantage of this concept is that it's rather
easy to add new syntax elements. For this task, the hsc tags
<ln_deftag>, <ln_defent> and <ln_deficon> can be used.</P>

<H2><A NAME="default">Default Preferences</A></H2>

<P>It is a serious problem about html that no one can give you competent
answer on the question <qq>Now which tags are part of html?</qq>. On the
one hand, there is w3c, which you meanwhile can ignore, on the other
hand, there are the developers of popular browsers, which implement
whatever they just like.</P>

<P>The <hsc.prefs> coming with this distribution should support most
elements needed for everyday use. This includes full html-2.0-support,
tables, client-side image-maps and several elements only used by
special browsers.</P>

<P>However, they are currently rather outdated and could need a major
rework. This should come with one of the next releases.</P>

<H2><A NAME="search">Searching For The Preferences</A></H2>

If you do not explicitely specify certain preferences by means of the
CLI-option <op_prefsfile>, <hsc> will look at several places when
trying to open <hsc.prefs>:

<UL>
    <LI>the current directory
    <LI>the directory specified in the environment variable <env_hscpath> 
    <LI>the directory <CODE>PROGDIR:</CODE>, which is automatically
        assigned to the same directory where the <hsc> binary resides
        when <hsc> is invoked
</UL>

<P>If it is unable to find <hsc.prefs> anywhere, it will abort with
an error message.</P>

<P>If you want to find out where <hsc> has read <hsc.prefs> from,
you can use <KBD><A HREF=":options.html#status">STATUS=VERBOSE</A></KBD>
when invoking <hsc>. This will display the preferences used.</P>

<H2><A NAME="tags">Special Tags To Modify Syntax Definition</A></H2>

<$macro SPCTAG NAME:string/r TITLE:string/r>
<H3><A NAME=(name)><(title)></A></H3>
</$macro>

All the tags below should be used within <hsc.prefs> only.
<SPCTAG NAME="defent" TITLE="Define an entity">

<P>This tag defines
a new entity. The (required) attribute <CODE>NAME</CODE> declares the
name of the entity, <CODE>RPLC</CODE> the character that should be
replaced by this entity if found in the hsc-source and <CODE>NUM</CODE>
is the numeric representation of this entity.</P>

<STRONG>Example:</STRONG> <TG>$defent NAME="uuml" RPLC="ü" NUM="252"</TG>

<SPCTAG NAME="deficon" TITLE="Define icon-entity">

<P>This tag defines
a new icon-entity. The only (required) attribute is <CODE>NAME</CODE>
which declares the name of the icon.</P>

<STRONG>Example:</STRONG> <TG>$deficon NAME="mail"</TG>

<SPCTAG NAME="deftag" TITLE="Define a tag">

<P>This tag defines
a new tag, and is used quite similar to <ln_macro>, exept that a
tag-definition requires no macro-text and end-tag to be followed.</P>

<STRONG>Example:</STRONG> <TG>$deftag IMG SRC:uri/x/z/r ALT:string ALIGN:enum("top|bottom|middle") ISMAP:bool WIDTH:string HEIGHT:string</TG>

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

<H2>Why It Can Not Read DTDs</H2>

DTD is short for Document Type Definition. One of the early concept of
html was that the first line of a document should contain a line that
tells which DTD has been used to create this document. This could look
like

<$source PRE><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN"></$source>

<P>Browsers should read that line, obtain the DTD and parse the source
according to it. The problem about DTDs: they are written in sgml. And
the problem about sgml: It's awful. It's unreadable. It's a pure
brain-wanking concept born by some wireheads probably never seriously
thinking about using it themselves. Even when there is free code
available to sgml-parse text.</P>

<P>As a result, only less browsers did support this. As it was rather
easy to write a browser spiting on the sgml-trash, but simply parsing
the code <qq>tag-by-tag</qq>, developers decided to spend more time on
making their product more user-friendly than computer-friendly (which
really can understand).</P>

<P>These browsers became even more popular when they supported tags
certain people liked, but were not part of DTDs. As DTDs were
published by w3c, and w3c did not like those tags, they did not made
it into DTDs for a long time or even not at all (which I really can
understand, too).</P>

<P>This did work for a certain degree until html-2.0. Several people
(at least most of the serious w3-authoring people) did prefer to
conform to w3c than use the funky-crazy-cool tags of some special
browsers, and the funky-crazy-cool people did not care about DTDs or
html-validators anyway.</P>

<P>However, after html-2.0, w3c fucked up. They proposed the infamous
html-3.0 standard, which was never officially released, and tried to
ignore things most browsers did already have implemented (which not
all of them were useless crap, I daresay.). After more than a year
without any remarkable news from w3c, they finally canceled html-3.0,
and instead came out with the pathetic html-0.32.</P>

<P>Nevertheless, many people were very happy about html-0.32, as it
finally was a statement after that many things became clear. It became
clear that you should not expect anything useful from w3c anymore. It
became clear that the browser developers rule. It became clear that no
one is going to provide useful DTDs in future, as browser developers
are too lazy and incompetent to do so. It became clear that anarchy
has broken out for html-specifications.</P>

<$macro even-sgml>
    even without reading three books about sgml and analysing
    20 example sources
</$macro>

So, as a conclusion, reasons not to use DTDs but an own format are:
<UL>
<LI>It's more readable, and users can change it easier, <even-sgml>.
<LI>It has been easier for me to implement, <even-sgml>.
<LI>One does not depend on w3c. (Sad, that this is a reason.)
<LI>The format for <hsc.prefs> contains some information, which makes
    is possible to for example evaluate image-sizes or strip tags with
    absolute URIs. This is realised with special attribute modifiers
    which have no equivalent in DTDs.
</UL>

<P>Quite unexpected, with html-4.0 this has changed for some extent,
as the DTDs are quite readable and well documented. Although it will
take them more than this to get back the trust they abused in the
recent years, at least it is a little signal suggesting there are some
small pieces of brain intact somewhere in this consortium.</P>

<H2>Problems</H2>

<P>There is also a disadvantage of this concept: reading <hsc.prefs>
every time on startup needs an awful lot of time. Usually, processing
your main data takes shorter than reading the preferences. You can
reduce this time, if you create your own <hsc.prefs> with all tags and
entities you don't need removed. But I recommend to avoid this because
you might have to edit your preferences again with the next update of
<hsc>, if any new features have been added.</P>

</WEBPAGE>
