<WEBPAGE chapter="hsc - " title="Messages"
    PREV="examples.html"
    NEXT="copy.html"
    QTEXT=("What happened to the attitude you used to have at school<BR>"
          +"What happened to the attitude when you broke all the rules")
    QAUTHOR='The Farm, "Groovy Train"'>

<$macro MSG_NOTE><A HREF=":messages.html#msg_note">Note</A></$macro>
<$macro MSG_STYLE><A HREF=":messages.html#msg_style">Bad-style</A></$macro>
<$macro MSG_PORT><A HREF=":messages.html#msg_port">Portability-problem</A></$macro>
<$macro MSG_WARN><A HREF=":messages.html#msg_warn">Warning</A></$macro>
<$macro MSG_ERR><A HREF=":messages.html#msg_err">Error</A></$macro>
<$macro MSG_FERR><A HREF=":messages.html#msg_ferr">Fatal error</A></$macro>
<$macro OPTN_IGNORE><CODE><A HREF=":options.html#ignore">IGNORE</A></CODE></$macro>

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

<P><hsc> is able to produce lots of different warnings and
errors when parsing hsc-sources. The are devided into several classes,
and there is a possibility to filter out specific messages or whole
message classes. 
There are also possibilities to change the rendering of messages and
redirect them to a file, making it easy to integrate <hsc> into
existing developer environments.

<H2><A NAME="elements">Message elements</A></H2>
By default, messages show up as seen below:

<BLOCKQUOTE>
<CODE>file (line#, colum#): class and message-id: message</CODE>
</BLOCKQUOTE>

with being<UL>

<LI><CODE>file</CODE> the name of the input file that caused message
<LI><CODE>line#</CODE> and <CODE>colum#</CODE> the position
    in input file
<LI><CODE>class</CODE> the message class, 
    one of <MSG_NOTE>, <MSG_STYLE>, <MSG_PORT>, <MSG_WARN>, <MSG_ERR> or <MSG_FERR>.
    See below for details about message classes.
<LI><CODE>message</CODE> the message text, which describes what
    happened
</UL>

For example, a typical message would be:
<$source PRE>
    hugo.hsc (17,23): Warning 11: unknown tag <SEPP>
</$source>

<H2><A NAME="classes">Message classes</A></H2>

<STRONG><A NAME="msg_note">Note</A></STRONG> is a message only for the
users information. It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_style">Bad-style</A></STRONG> informs the user that
his although legal html-code includes constructs that indicate a bad style.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_port">Portability-problem</A></STRONG> informs the
user that his although legal html-code includes constructs can lead to problems
on old or buggy browsers.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_warn">Warning</A></STRONG> tells you that your source
includes some problematic parts, that can lead to problems with several
browsers.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_err">Error</A></STRONG> indicates that there is something
wrong with your html-source. No output is written.
<P>
<STRONG><A NAME="msg_ferr">Fatal error</A></STRONG> tells you that something
terrible has happened and there is no way to continue the conversion.
No output is written.
<P>

There is also another class called <qq>panic messages</qq>, which
are used by <hsc> to report internal errors. To find out more
about them, look at the chapter about <ln_bugs>.

<H2><A NAME="options">Message options</A></H2>

There are several CLI options to modify the behavior of <hsc>'s messages:
<UL>
<LI><op_ignore> can be used to suppress single messages or whole 
message classes, if you are not interested in them
<LI><KBD><A HREF="options.html#msgformat">MSGFORMAT</A></KBD> specifies a
template how to use message elements
<LI><KBD><A HREF="options.html#msgformat">MSGFILE</A></KBD> is used to 
redirect messages into a file, where they for example can be processed
by some message parser that controls your editor
<LI><KBD><A HREF="options.html#msgformat">MSGANSI</A></KBD> tells <hsc>
to use some ANSI escape sequences to make messages easier to read, if
you only let them show up on the screen (and don't use any IDE)
</UL>

<HR>

<H2><A NAME="list">List of messages</A></H2>

Below you can find a list of messages that might show up when processing
hsc sources. Most of them also include a short explanation what could
have caused the problem and how to fix it.
However, this is not a html-tutorial. Fully
understanding these messages requires some experience from the user.

<P>[ <A HREF="#message.1">1-10</A>
   | <A HREF="#message.11">11-20</A>
   | <A HREF="#message.21">21-30</A>
   | <A HREF="#message.31">31-40</A>
   | <A HREF="#message.41">41-50</A>
   | <A HREF="#message.51">51-60</A>
   | <A HREF="#message.61">61-70</A>
   | <A HREF="#message.71">71-80</A>
   | <A HREF="#message.71">81-..</A>
   ]
</P>

<DL>
<MSG ID="1" CLASS="warning" NAME="unable to open project-file">

The project-file specified could not been open. This indicates
no problem, if you specified a project-file using <op_prjfile>
the first time; it will be created automatically after the
html-object has successfully been processed.

<MSG ID="2" CLASS="fatal" NAME="project-file corrupt">

The data found in the project file are not of the required
form. Most likely, the file-format-version is outdated.
Remove the old file, <hsc> will create a new one next time it
is invoked.

<MSG ID="3" CLASS="fatal" NAME="unexpected end of file">
There are still characters to be read from input, but the file
already ends; <hsc> tells you what it still expects to come.

<MSG ID="4" CLASS="error" NAME="" UNDOC>

<MSG ID="5" CLASS="error" NAME="" UNDOC>
<MSG ID="6" CLASS="fatal" NAME="can not open <I>file</I> for input">
An input file could not been opened. A more detailed message that tells
you why is displayed, too.

<MSG ID="7" CLASS="note" NAME="stripped <instag>">
Notifies user that a tag has been removed. This usually happens
because the user requested this with one of the CLI-options.

<MSG ID="8" CLASS="error" NAME="" UNDOC>
<MSG ID="9" CLASS="style" NAME="expected heading <TG>Hx</TG>">
A heading should not be more than one level below the heading
which preceded it. That is, an <TG>H3</TG> element should not
follow an <TG>H1</TG> element directly. The first heading
showing up in a document should be <TG>H1</TG>.

<MSG ID="10" CLASS="style" NAME='"click here" syndrome detected'>
A keyword defined with the special attribute <CODE>HSC.CLICK-HERE</CODE>
has been found within the text inside an anchor specification.<P>
Ok, now again in English: you used some term like "Click here" inside
the text describing a link. This is very clumsy and should be avoided;
see <A HREF="http://www.cs.cmu.edu/~tilt/cgh/index.html#linktext">Composing
Good HTML</A> for details.

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

<MSG ID="19" CLASS="warning" NAME='";" expected'>

A <semicolon> has been expected, but didn't occur. For instance,
you could have an entity <qq><CODE>&amp;uuml</CODE></qq> instead of
<qq><CODE>&amp;uuml;</CODE></qq>.

<MSG ID="20" CLASS="error" NAME="reference to unknown attribute">
You referered to an attribute that doesn't exist at all.

<MSG ID="21" CLASS="warning" NAME="path to URI not found: <I>file</I>">
A tag points to a local resource that doesn't exist.

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
You tried to call <ln_content> outside any container macro.

<MSG ID="29" CLASS="error" NAME="tag &lt;A&gt; without HREF or NAME">
An anchor tag has been specified without one of the required attributes
<CODE>HREF</CODE> or <CODE>NAME</CODE>.

<MSG ID="30" CLASS="error" NAME='unmatched "&gt;"'>
A "greater than" sign appeared inside the text. You should write
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
bytes in your html-object, but will make your documents unreadable
if stored on your local disk, as they depend on a specific server
structure.</P>

<P>Normally, <hsc> is unable to validate links or obtain image sizes
of data referenced using server relative URIs because of reasons that
should be obvious to everyone.</P>

<P>If you because of some obscure reasons (or simply stupidity) insist
on using URIs of such kind, you can set the CLI option <op_serverdir>
to specifiy the root directory for server relative URIs.</P>

<MSG ID="37" CLASS="warning" NAME="<instag> is obsolete">
The tag was defined within some old html-version, but should
not be used any more (eg. <TG>LISTING</TG>).

<MSG ID="38" CLASS="warning" NAME="<instag> is only used by jerks">
This tag is no legal html-tag and is only supported by special
browsers.

<MSG ID="39" CLASS="error" NAME="<I>custom user message</I>">
This text and class of this message can be controlled by the
user using <A HREF="features/spctags.html#message"><TG>$message</TG></A>.

<MSG ID="40" CLASS="fatal" NAME="can not open preferences file">
The file <hsc.prefs> could not be found at any of the 
<A HREF="features/prefs.html#search">expected locations</A>.

<MSG ID="41" CLASS="warning" NAME="unknown environment variable <qq><I>envvar</I></qq>">
The function <ln_GetEnv> 
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
This message can show up if you have specified <op_getsize> when invoking <hsc>, 
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
This message is active only if you specified a project file when invoking <hsc> using
the option <op_prjfile>. It tell you that a reference to a local id within a
document could not be checked because the document linked to is not mentioned
in the project file.

<P>Usually, this happens when you did not process the above mentioned document
with a project file specified before. Do not worry about that too much,
after processing all documents of your project at least once, this message
usually does not show up any more.</P>

If it still is there, then you are refering to documents that are part of
none or another project using another project-file; if so, there is no 
work-around for this case.

<MSG ID="52" CLASS="error" NAME="redefined <insendtag>">

You tried to redefine an end macro.

<MSG ID="53" CLASS="error" NAME="unmatched conditional <TG>$else|$elseif</TG>">
An <TG>$else</TG> tag has been at an unexptected position. Usually,
a single preceding <TG>$if</TG> has two or more corresponding <TG>$else</TG>
tags assigned.

<MSG ID="54" CLASS="warning" NAME="calling external command returned <I>value</I>">
You have invoked a shell-command using <ln_exec> and it returned a
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
occure before. Example: <TG>INPUT</TG> may only occure
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
Within an 
<A HREF="features/expressions.html">expression</A>,
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

Somethings wrong with a <ln_defent> or <ln_deficon> call.

<MSG ID="70" CLASS="warning" NAME="can not strip special <instag>">
You have tried to strip one of <hsc>'s 
<A HREF="features/spctags.html">special tags</A> using the CLI-option
<op_striptags>. This does not make much sense, because all these tags
will not show up in the output anyway.

<MSG ID="71" CLASS="error" NAME="illegal numeric <insval> for <insattr>/entity">
A numeric value must only consist of letters. Only integer values are
allowed, therefor even a <period> must not be used.

<MSG ID="72" CLASS="warning" NAME="illegal color value">

A color value must fit the template <qq><CODE>#rrggbb</CODE></qq> 
or be one of the values declared in the special attribute
<A HREF=":features/spcattr.html#colornames"><CODE>HSC.COLOR-NAMES</CODE></A>
defined in <hsc.prefs>

<MSG ID="73" CLASS="error" NAME="" UNDOC>
<MSG ID="74" CLASS="warning" NAME="unknown id <insid>">

You forgot to specify the ID mentioned using <TG>A NAME=..</TG>,
or just made a typo.

<MSG ID="75" CLASS="warning" NAME="local id <insid> already declared">

You tried to redefine an ID that has already been declared before within
the current document.

<MSG ID="76" CLASS="style" NAME="frames are disgusting">

This message should need no comment; anyway, read 
<A HREF="http://www.ummed.edu:8000/pub/i/ijosh/frames/">Why Frames Suck</A> 
for more details.

<MSG ID="77" CLASS="note" NAME="replacing icon-entity">

A icon-entity has been found is was replaced by an <TG>IMG</TG>.
This message will only show up if you have passed the CLI option
<op_iconbase>.

<MSG ID="78" CLASS="style" NAME="succeeding white space for <instag>">

See below.

<MSG ID="79" CLASS="style" NAME="preceding white space for <insendtag>">

These two messages point out that instead of (read the <underscore>)
as a blank)

<BLOCKQUOTE>
<TG>STRONG</TG>__important__<TG>/STRONG</TG>
</BLOCKQUOTE>

you better should use

<BLOCKQUOTE>
<TG>STRONG</TG>important<TG>/STRONG</TG>
</BLOCKQUOTE>

It only affects tags like <TG>A</TG>, <TG>TITLE</TG>, headings and
physical/locigal styles.

<MSG ID="80" CLASS="error" NAME="" UNDOC>
<MSG ID="81" CLASS="warning" NAME="value for <insattr> requires quotes">

If a attribute value contains any characters other then letters,
digits, <period> or <hyphen>, it needs to be quoted.

<MSG ID="82" CLASS="note" NAME="changed quotes for <insattr> from [<I>style</I>] to [<I>style</I>]">

The quote style has been changed for a specific attribute. 
This message will only show up if you have passed the CLI option
<op_quotestyle>.

<*
<MSG ID="83" CLASS="error" NAME="" UNDOC>
<MSG ID="84" CLASS="error" NAME="" UNDOC>
*>
</DL>

</WEBPAGE>
