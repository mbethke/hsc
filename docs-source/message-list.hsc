<DOCPAGE chapter="hsc - " title="List of Messages"
    PREV="messages.html"
    NEXT="envvar.html"
    QTEXT=("What happened to the attitude you used to have at school<BR>"
          +"What happened to the attitude when you broke all the rules")
    QAUTHOR='The Farm, "Groovy Train"'>

<$macro msg id:string/r name:string class:enum("note|style|port|warning|error|fatal") undoc:bool>
<DT><$if COND=(undoc)><STRONG>message <(id)>:</STRONG><DD>Unused or undocumented.
<$else><A NAME=("message."+id)><STRONG><$if COND=(class="style")>bad style
<$elseif COND=(class="port")>portability problem
<$elseif COND=(class="fatal")>fatal error
<$else><(class)>
</$if>
<(id)>: <(name)></STRONG></A><DD></$if>
</$macro>

<$MACRO insattr>attribute <I>attrib</I></$MACRO>
<$MACRO insent>entity <I>entity</I></$MACRO>
<$MACRO instag>tag &lt;<I>tag</I>&gt;</$MACRO>
<$MACRO insendtag>end tag &lt;<I>tag</I>&gt;</$MACRO>
<$MACRO insval>value <I>value</I></$MACRO>
<$MACRO insid>id <I>id</I></$MACRO>
<$MACRO inssval><I>value</I></$MACRO>

<*** insert quick-link to messages ***>
<html-only>
<P>[ <A HREF="#message.1">1-10</A>
   | <A HREF="#message.11">11-20</A>
   | <A HREF="#message.21">21-30</A>
   | <A HREF="#message.31">31-40</A>
   | <A HREF="#message.41">41-50</A>
   | <A HREF="#message.51">51-60</A>
   | <A HREF="#message.61">61-70</A>
   | <A HREF="#message.71">71-80</A>
   | <A HREF="#message.81">81-90</A>
   | <A HREF="#message.91">91-..</A>
   ]
</P>
</html-only>

<p>Below you can find a list of messages that can show up when processing hsc
sources. Most of them also include a short explanation what could have caused
the problem and how to fix it. However, this is not an HTML tutorial. Fully
understanding these messages requires some experience from the user.</p>

<DL>
<MSG ID="1" CLASS="warning" NAME="unable to open project file">

The project-file specified could not be opened. This indicates
no problem if you specified a project file using <op-prjfile>
for the first time; it will be created automatically after the
HTML object has successfully been processed.

<MSG ID="2" CLASS="fatal" NAME="project file corrupt">

The data found in the project file are not of the required
format. Most likely, the file version is outdated.
Remove the old file, <hsc> will create a new one next time it
is invoked.

<MSG ID="3" CLASS="fatal" NAME="unexpected end of file">
There are still characters to be read from input, but the file
is already at the end; <hsc> tells you what it still expected to read.

<MSG ID="4" CLASS="fatal" NAME="error reading <I>file</I>">

On attempting to read from an input file, an error has occured. This
usually is on a hardware level, and can only be fixed by one of those
tools to validate and repair a disk structure common on every
platform.

<MSG ID="5" CLASS="fatal" NAME="too many messages or errors">

There were more messages/errors than specified via
<code>MAXERR</code>/<code>MAXMSG</code>. <hsc> will abort.

<MSG ID="6" CLASS="fatal" NAME="can not open <I>file</I> for input">

An input file could not be opened. A more detailed description that
tells you why is displayed, too.

<MSG ID="7" CLASS="note" NAME="stripped <instag>">
Notifies user that a tag has been removed. This usually happens
because the user requested this with one of the commandline options.

<MSG ID="8" CLASS="error" NAME="image <i>file</i> corrupt">

While looking for size information inside a picture file, <hsc> found something
about the format it could not understand. It is likely that this image is
corrupted and will not display correctly in a browser, so this is considered an
error. If you are sure the image is OK, you should mail it to the <a
href=":author.html">author</a> to get <hsc> fixed.

<MSG ID="9" CLASS="style" NAME="expected heading <TG>Hx</TG>">
A heading should not be more than one level below the heading
which preceded it. That is, an <TG>H3</TG> element should not
follow an <TG>H1</TG> element directly. The first heading
showing up in a document should be <TG>H1</TG>.

<MSG ID="10" CLASS="style" NAME='"click here" syndrome detected'> A keyword
defined with the special attribute <CODE>HSC.CLICK-HERE</CODE> has been found
within the text inside an anchor specification. I.e. you used some term like
<qq>Click here</qq> inside the text describing a link. This is very clumsy and
should be avoided; see <A HREF="http://www.ology.org/tilt/cgh/#meaningless_link_text">Composing Good
HTML</A> or the <A HREF="http://www.w3.org/2001/06tips/noClickHere">W3C
Recommendations</A> for details.

<MSG ID="11" CLASS="error" NAME="unknown <instag>">
You refered to an unknown tag or macro.

<MSG ID="12" CLASS="error" NAME="<instag> occured too often">
A tag that is expected to occure only once appeared at least twice.
Remove all unneccessary occurences of the tag.

<MSG ID="13" CLASS="error" NAME="unmatched end-<instag>">
You called a end-tag without using the corresponding start-tag
before.

<MSG ID="14" CLASS="warning" NAME="illegal end-tag nesting">
A end-tag appeared where a different end-tag was expected to
appear before. Example: instead of
<$SOURCE PRE>
<B><I>bold and italic</B></I>
</$SOURCE>
you should write
<$SOURCE PRE>
<B><I>bold and italic</I></B>
</$SOURCE>

Note the different location of <TG>/I</TG> in both cases.

<MSG ID="15" CLASS="warning" NAME="required <instag> missing">
A tag that is required for every document is missing.

<MSG ID="16" CLASS="warning" NAME="end-<instag> missing">
You called an start-tag, but didn't end it. Insert the required
end-tag.

<MSG ID="17" CLASS="error" NAME="unknown tag modifier <I>/modifier</I>">

You have used an unknown <A HREF=":macro/flag.html">modifier</A> 
to define the characteristics of an tag or macro.

<MSG ID="18" CLASS="warning" NAME="unknown entity">
You used an entity, that <hsc> doesn't know. This can happen if you
made a typo, or the entity has not been defined within
<hsc.prefs>.

<MSG ID="19" CLASS="warning" NAME='";" expected after entity'>

Entities require a <semicolon> to end them. For instance, if you try
to use <qq><CODE>&amp;uuml</CODE></qq> instead of
<qq><CODE>&amp;uuml<STRONG>;</STRONG></CODE></qq> (to get an
<qqc>&uuml;</qqc>, this message will show up. Simply append the
<qqc>;</qqc> to fix it.

<MSG ID="20" CLASS="error" NAME="unknown <insattr>">
You tried to access an attribute which has not been defined before.

<MSG ID="21" CLASS="warning" NAME="path to URI not found: <I>file</I>">

An attribute refers to a local resource (like another document or
image) that doesn't exist. Common reasons for that are typos, a
case-sensitive file system or indeed a missing file.

<MSG ID="22" CLASS="warning" NAME="argument without quote">
You passed a value to an attribute, but did not embed it into single
or double quotes. This can lead to problems with older browsers.

<MSG ID="23" CLASS="error" NAME="empty reference to <insattr>">
An attribute refers to an attribute that has been defined, but no
value has been set for. Usually, you try to refer to an attribute
inside a macro, that no value has been passed to within the call
of the macro.

<MSG ID="24" CLASS="warning" NAME="attribute option <qq><I>option</I></qq> not allowed in this context">
You have used an <A HREF=":macro/attrib.html#modifier">option</A>
to define the characteristics of an attribute, which is not allowed to
appear at this location.

<MSG ID="25" CLASS="style" NAME="<TG>BLINK</TG> sucks">
Some browser support the non-html-tag <TG>BLINK</TG>. It is used
to make text blinking, which annoys many users. Additionally, most
of them don't know how to configure their browser that it doesn't
blink, so you really should avoid it.

<MSG ID="26" CLASS="error" NAME="default value for <insattr> already set">
You tried to assign more than one default value within an attribute declaration.
For instance, like in
<$source PRE>
    <$define HUGO:string="hugo"="or sepp?">
</$source>
Remove the part corresponding to <qq><CODE>="or sepp?"</CODE></qq>.

<MSG ID="27" CLASS="error" NAME="attempt to modify constant <insattr>">
You tried to assign a new value to an attribute declared as constant.
That is, the <A HREF="macro/attrib.html#modifier">attribute option</A>
<CODE>/CONST</CODE> has been specified when defining it earlier.

<MSG ID="28" CLASS="fatal" NAME="no content within current context">
You tried to call <ln-content> outside any container macro.

<MSG ID="29" CLASS="error" NAME="tag &lt;A&gt; without HREF or NAME">
An anchor tag has been specified without one of the required attributes
<CODE>HREF</CODE> or <CODE>NAME</CODE>.

<MSG ID="30" CLASS="error" NAME='unmatched "&gt;"'>
A <qq>greater than</qq> sign appeared inside the text. You should write
"<CODE>&amp;gt;</CODE>" instead. This can also happen, if you made
an error calling a tag, and <hsc>'s parser couln't recover.

<MSG ID="31" CLASS="error" NAME='expected "<I>element_expected</I>", found "<I>element_found</I>"'>
A syntax element did not occur where it has been expected.

<MSG ID="32" CLASS="error" NAME="" UNDOC>
<MSG ID="33" CLASS="warning" NAME="linefeed found inside string">

<MSG ID="34" CLASS="error" NAME="" UNDOC>
<MSG ID="35" CLASS="error" NAME="unknown <insval> for enumerator <insattr>">
You tried to set an enumerator to a value it doesn't support.

<MSG ID="36" CLASS="warning" NAME="server relative URI to <I>uri</I>">
<P>An URI-attribute started with a <slash> (like for example
<qqc>/image/next.png</qqc>), denoting a so called server relative URI.
This is a really braindead concept, which might help to save a few
bytes in your HTML object, but will make your documents unreadable
if stored on your local disk, as they depend on a specific server
structure.</P>

<P>Normally, <hsc> is unable to validate links or obtain image sizes
of data referenced using server relative URIs because of reasons that
should be obvious to everyone.</P>

<P>If you because of some obscure reasons (or simply stupidity) insist
on using URIs of such kind, you can set the commandline option <op-serverdir>
to specifiy the root directory for server relative URIs.</P>

<MSG ID="37" CLASS="warning" NAME="<instag> is obsolete">
The tag was defined within some old HTML version, but should
not be used any more (eg. <TG>LISTING</TG>).

<MSG ID="38" CLASS="warning" NAME="<instag> is only used by jerks">
This tag is no legal HTML tag and is only supported by special
browsers.

<MSG ID="39" CLASS="error" NAME="<I>custom user message</I>">
This text and class of this message can be controlled by the
user using <A HREF="features/spctags.html#message"><TG>$message</TG></A>.

<MSG ID="40" CLASS="fatal" NAME="can not open preferences file">
The file <hsc.prefs> could not be found at any of the 
<A HREF="features/prefs.html#search">expected locations</A>.

<MSG ID="41" CLASS="warning" NAME="unknown environment variable <qq><I>envvar</I></qq>">
The function <ln-GetEnv>
could not access the environment variable specified an returned an 
empty value.

<MSG ID="42" CLASS="error" NAME="missing value for <insattr>">
Non-boolean attributes require an value set.

<MSG ID="43" CLASS="error" NAME="unknown attribute option <I>option</I>">
You defined a new attribute, but used an
<A HREF=":macro/attrib.html#modifier">option</A> that is unknown.

<MSG ID="44" CLASS="error" NAME="required <insattr> missing">
An attribute that is required has not been set within the call
of a tag or macro.

<MSG ID="45" CLASS="warning" NAME="unexpected value for <insattr>: expected <qq><inssval></qq>, found <qq><inssval></qq>">
This message can show up if you have specified <op-getsize> when invoking <hsc>,
and have set the attribute <CODE>WIDTH</CODE> and/or <CODE>HEIGHT</CODE> by
hand. It denotes that the values you have used differ from those <hsc> has
evaluated from the image data.

<MSG ID="46" CLASS="note" NAME="replaced <I>char</I> by <I>entity</I>">
Informs you that a special character (non-7-bit-ASCII) has been replaced
by it's corresponding entity.

<MSG ID="47" CLASS="error" NAME="illegal white space">
A white space occurred at a place where it was not supposed to.

<MSG ID="48" CLASS="port" NAME="line feed inside sgml-comment">
<MSG ID="49" CLASS="port" NAME='"&gt;" inside sgml-comment'>
<MSG ID="50" CLASS="port" NAME="sgml-comment ends inside quotes">
The messages#48 to #50 point out problems that are known bugs of
several browsers. A general comment on sgml-comments: Try to avoid
them, use the <A HREF="features/spctags.html#comments"><TG>* <I>comment</I> *</TG></A>
tag instead.

<MSG ID="51" CLASS="warning" NAME='no entry for document "<I>document</I>" in project data to check <insid>'>

This message is active only if you specified a project file when
invoking <hsc> using the option <op-prjfile>. It tells you that a
reference to an ID inside another document could not be checked
because the document linked to is not mentioned in the project file.

<P>Usually, this happens when you did not process the above mentioned
document with a project file specified before. Do not worry about that
too much, after processing all documents of your project at least
once, this message usually does not show up any more.</P>

If it still is there, then you are refering to documents that are part
of none or another project using another project-file; if so, there is
no work-around for this case.

<MSG ID="52" CLASS="error" NAME="" UNDOC>

<MSG ID="53" CLASS="error" NAME="unmatched conditional <TG>$else|$elseif</TG>">

An <TG>$else</TG> tag has been at an unexpected position. Usually a
single preceding <TG>$if</TG> has two or more corresponding
<TG>$else</TG> tags assigned.

<MSG ID="54" CLASS="warning" NAME="calling external command returned <I>value</I>">
You have invoked a shell-command using <ln-exec> and it returned a
value unequal to zero, which usually denotes an error while processing
the command. For error analysis, look at the output that the command
(hopefully) has made.

<MSG ID="55" CLASS="warning" NAME="text outside sgml-comment context">
This message often shows up, if you are not using an even number of
hypens (<qqc>-</qqc>) as delimters inside your source. For example,
<$source PRE><!--------></$source>
(8 hyphens) works fine, whereas
<$source PRE><!---------></$source>
(9 hyphens) will cause problems.

<MSG ID="56" CLASS="error" NAME="" UNDOC>
<*
<MSG ID="56" CLASS="port" NAME="sgml-comment consists of a single word">
A sgml-comment consisting of a single word, for instance 
<qq><TG>!--sepp--</TG></qq>,
has been detected. Note that there are no blanks preceding/succeeding
<qq>sepp</qq>)
*>

<MSG ID="57" CLASS="error" NAME="no start tag for <insendtag>">
An end tag has been detected without it's corresponding start tag
occuring before.

<MSG ID="58" CLASS="port" NAME="icon-<insent> found">
Icon-entities are not (yet) widely supported.

<MSG ID="59" CLASS="warning" NAME="redefined <instag>">
You have just redefined a tag or macro that has already been
declared before. The previous declaration will be thrown away.

<MSG ID="60" CLASS="error" NAME="<instag> must be inside <instag>">
Before you are allowed to use the first tag, the second tag has to
occur before. Example: <TG>INPUT</TG> may only occur
inside <TG>FORM</TG>.

<MSG ID="61" CLASS="error" NAME="<instag> not allowed within <instag>">
A tag occurs inside another tag that does not allow this. For example,
you can't recursively use <TG>A</TG> inside <TG>A</TG>.

<MSG ID="62" CLASS="error" NAME="no attributes allowed for end-tags">
You tried to pass values to a end-tag or macro. If you need an attribute
for a end-macro only, you still have to define and pass it within the
start-macro; the end-macro will have access to all these attributes
and its values.

<MSG ID="63" CLASS="warning" NAME="redefinition of <insattr>">
You redefined an alredy existing attribute. If this occures during
the definition of a new macro, you just tried to give two arguments the
same name.

<MSG ID="64" CLASS="error" NAME='illegal attribute identifier "<I>attribute_name</I>"'>
You have tried to use an attribute name which contains characters not 
allowed for that. Legal characters for attribute names are letters, digits, 
<underscore>, <period> and <hyphen>.

<MSG ID="65" CLASS="error" NAME='unknown binary operator "<I>operator</I>"'>
Within an <ln-expression>,
you have used an binary operator that is not
one of those mentioned in the 
<A HREF="features/expressions.html#operators">list of operators</A>.

<MSG ID="66" CLASS="error" NAME="illegal end-tag">
You tried to use a simple tag as a container, for example <TG>/IMG</TG>

<MSG ID="67" CLASS="style" NAME="recommended <instag> missing">
A tag that ought to appear is missing.

<MSG ID="68" CLASS="note" NAME="unknown filetype">

<hsc> could not <A HREF="features/getsize.html">determine the size</A>
of a link destination.

<MSG ID="69" CLASS="error" NAME="illegal entity definition">

Something is wrong with a <ln-defent> or <ln-deficon> call.

<MSG ID="70" CLASS="warning" NAME="can not strip special <instag>">
You have tried to strip one of <hsc>'s 
<A HREF="features/spctags.html">special tags</A> using the commandline option
<op-striptags>. This does not make much sense, because all these tags
will not show up in the output anyway.

<MSG ID="71" CLASS="error" NAME="illegal numeric <insval> for <insattr>/entity">
A numeric value must only consist of digits. Only integer values are
allowed, therefor even a <period> must not be used.

<MSG ID="72" CLASS="warning" NAME="illegal color value">

A color value must fit the template <qq><CODE>#rrggbb</CODE></qq> 
or be one of the values declared in the special attribute
<A HREF=":features/spcattr.html#colornames"><CODE>HSC.COLOR-NAMES</CODE></A>
defined in <hsc.prefs>

<MSG ID="73" CLASS="error" NAME="" UNDOC>
<MSG ID="74" CLASS="warning" NAME="unknown <insid>">

You forgot to specify the ID mentioned using <TG>A NAME=..</TG>,
or just made a typo.

<MSG ID="75" CLASS="warning" NAME="local <insid> already declared">

You tried to redefine an ID that has already been declared before within
the current document.

<MSG ID="76" CLASS="style" NAME="frames are disgusting">

This message should need no comment; anyway, read
<A HREF="http://www.useit.com/alertbox/9612.html">Why Frames Suck (Most of the Time)</A>
or
<A HREF="http://www.ummed.edu/pub/i/ijosh/frames/">Why Frames Suck</A>
for more details.

<MSG ID="77" CLASS="note" NAME="replacing icon-entity">

A icon-entity has been found is was replaced by an <TG>IMG</TG>.
This message will only show up if you have passed the commandline option
<op-iconbase>.

<MSG ID="78" CLASS="style" NAME="succeeding white space for <instag>">

See below.

<MSG ID="79" CLASS="port" NAME="preceding white space for <insendtag>">

These two messages point out that instead of (read the <underscore>)
as a blank)

<BLOCKQUOTE>
<TG>STRONG</TG>__important__<TG>/STRONG</TG>
</BLOCKQUOTE>

you better should use

<BLOCKQUOTE>
<TG>STRONG</TG>important<TG>/STRONG</TG>
</BLOCKQUOTE>

<P>It only affects tags like <TG>A</TG>, <TG>TITLE</TG>, headings and
physical/locigal styles.</P>

<P>This message might seem unreasonable, as according to the
specifications for HTML readers such white spaces should be skipped.
However, some browsers fail to do so.

<MSG ID="80" CLASS="error" NAME="" UNDOC>
<MSG ID="81" CLASS="warning" NAME="value for <insattr> requires quotes">

If a attribute value contains any characters other then letters,
digits, <period> or <hyphen>, it needs to be quoted.

<MSG ID="82" CLASS="note" NAME="changed quotes for <insattr> from [<I>style</I>] to [<I>style</I>]">

The quote style has been changed for a specific attribute. 
This message will only show up if you have passed the commandline option
<op-quotestyle>.

<MSG ID="83" CLASS="error" NAME="" UNDOC>
<MSG ID="84" CLASS="warning" NAME="unknown <insattr> for <instag>">

A tag call contains an unknown attribute, which was not defined with
the corresponding <ln-deftag>. This does not denote a serious problem,
as HTML provides a vast selection of esoteric attributes and browsers
usually ignore unknown attributes, but you should confirm you did not
make a typo.

<MSG ID="85" CLASS="error" NAME="unknown <insattr> for <instag>">

A macro call contains an unknown attribute, which was not defined
with the corresponding <ln-macro>. As it would not make sense to
pass unknown attributes to macros, this is considered an error.
Look at the macro declaration which attribute you meant.

<MSG ID="86" CLASS="warning" NAME="error removing file <i>filename</i>">

After processing an <ln-exec> with <code>REMOVE</code> enabled, the file
could not be deleted. This can happen if you for example declared macros
or opened container tags without closing them in the file created by
an external command and included it afterwards; <hsc> keeps a
lock on files that might need to be referred to later in messages.

<MSG ID="87" CLASS="warning" NAME="attribute <insattr> is obsolete">
This attribute is considered obsolete, most likely because its function
should now be implemented using <a href="features/css.html">Cascading Style
Sheets</a> (CSS). You may still
want to use it (and ignore this message) to stay compatible with older browsers.

<MSG ID="88" CLASS="warning" NAME="empty tag not closed">
A tag with empty content model was not closed in XHTML mode. Tags that are not
containers may be minimized to <tg>TAG /</tg> in XHTML. Actually,
<tg>TAG</tg><tg>/TAG</tg> would be legal, too, but <hsc> doesn't support this,
both for technical reasons and because the first form is shorter.
The reason for not allowing the form <tg>TAG/</tg> (note the missing
blank!) is mainly a technical one, apart from the fact that it should be avoided
so older browsers will still recognize the tag and throw away the slash as an
unknown <qq>attribute</qq>. If you forget the closing slash in a tag
such as <tg>br /</tg>, <hsc> will complain about it with this message and add
the slash in the output. If you want to create HTML4 and XHTML from the same
<hsc> source, omit the slash in the source and ignore this message.

<MSG ID="89" CLASS="error" NAME="attribute after slash">
This message shows up if you put any attributes after the closing slash in an
empty tag in XHTML mode (see <a href="#message.88">msg. #88</a> above), as in
<tg>img src="foo.gif" / alt="bar"</tg>.

<MSG ID="90" CLASS="warning" NAME="syntax error in CSS">
A <code>STYLE</code> attribute contained CSS specifications <hsc> could not
understand. There will usually be a message hinting at the reason (such as
<qq>missing value</qq>, <qq>value <em>foo</em> is invalid for CSS property
<em>bar</em></qq>, etc.) displayed with this warning.

<MSG ID="91" CLASS="warning" NAME="CSS style redefined">
A CSS property appeared more than once in a tag. This can happen when <hsc>
tries to merge several <code>STYLE</code> attributes into one, or if you specify
a <code>width</code> or <code>height</code> property while running <hsc> with
the <a href="options.html#getsize"><code>GETSIZE</code> commandline option</a>.

<MSG ID="92" CLASS="warning" NAME="dubious entity definition">
A <ln-defent> call was found that might not do what you wanted. Either you
specified a RPLC code for an entity with a numeric code &gt;255, or you tried to
redefine an entity. Both may indeed be intended, in which case you should tell
<hsc> to <a href=":options.html#ignore">ignore</a> this warning.

<MSG ID="93" CLASS="warning" NAME="External URI does not exist">
An external URI was found nonexistant, i.e. the server returned a 5xx or 4xx
error code.

<MSG ID="94" CLASS="warning" NAME="Dubious external URI">
An external URI was checked and could not be successfully retrieved, i.e. the
server returned something that was neither a success nor an error code. This
includes temporarily or permanently moved resources for which the server will
issue a redirect. This can usually be ignored, but for best performance you
should still check and possibly adjust your link.

<MSG ID="95" CLASS="error" NAME="Cannot check external URI">
External URIs cannot currently be checked. On Unices, this is a fairly severe
condition, as it means that <hsc> failed to create a network socket. On AmigaOS, it
can simply mean you don't have bsdsocket.library and should get a TCP stack
before asking <hsc> to use one.

</DL>
</DOCPAGE>
