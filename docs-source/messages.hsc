<WEBPAGE chapter="hsc - " title="Messages"
    PREV="examples.html"
    NEXT="copy.html"
    QTEXT=("What happened to the attitude you used to have at school<BR>"
          +"What happened to the attitude when you broke all the rules")
    QAUTHOR='The Farm, "Groovy Train"'>

<$macro MSG_NOTE><A HREF=":messages.html#msg_note">Note</A></$macro>
<$macro MSG_STYLE><A HREF=":messages.html#msg_style">Bad style</A></$macro>
<$macro MSG_WARN><A HREF=":messages.html#msg_warn">Warning</A></$macro>
<$macro MSG_ERR><A HREF=":messages.html#msg_err">Error</A></$macro>
<$macro MSG_FERR><A HREF=":messages.html#msg_ferr">Fatal error</A></$macro>
<$macro OPTN_IGNORE><CODE><A HREF=":options.html#ignore">IGNORE</A></CODE></$macro>

<$macro msg id:string/r name:string/r class:enum("note|style|port|warning|error|fatal") undoc:bool>
<P><DT><STRONG><$if cond=(set class)>
<$if COND=(class="style")>bad style
<$elseif COND=(class="fatal")>fatal error
<$else><(class)>
</$if></$if>
<(id)>: <(name)></STRONG>
<DD><$if COND=(undoc)>Undocumented.</$if>
</$macro>

<$MACRO insattr>attribute <VAR>attrib</VAR></$MACRO>
<$MACRO insent>entity &lt;<VAR>entity</VAR>&gt;</$MACRO>
<$MACRO instag>tag &lt;<VAR>tag</VAR>&gt;</$MACRO>
<$MACRO insendtag>end-tag &lt;<VAR>tag</VAR>&gt;</$MACRO>
<$MACRO insval>value <VAR>value</VAR></$MACRO>

<hsc> is able to produce lots of different warnings and
errors when parsing html-files. The appearence looks something
like:

<BLOCKQUOTE>
<CODE>file (line#, colum#): class and message-id: message</CODE>
</BLOCKQUOTE>

with
<PRE>
  file           input file that caused message
  line#,         position in input file that caused error
  colum#
  class          one of <MSG_NOTE>, <MSG_WARN>, <MSG_ERR> or <MSG_FERR>
  message        message text
</PRE>

This can be modified using the CLI-option
<KBD><A HREF="options.html#msgformat">MSGFORMAT</A></KBD>.

<HR>

<H2>Message Classes</H2>

<STRONG><A NAME="msg_note">Note</A></STRONG> is a message only for the
users information. It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_style">Bad style</A></STRONG> informs the user that
his altough legal html-code includes constructs that indicate a bad style.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_port">Portability problem</A></STRONG> informs the
user that his altough legal html-code includes constructs can lead to problems
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

<HR>

<STRONG>Note:</STRONG> I've already started to collect the messages, but
this list is not yet complete.

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

<MSG ID="7" CLASS="error" NAME="" UNDOC>
<MSG ID="8" CLASS="error" NAME="" UNDOC>
<MSG ID="9" CLASS="error" NAME="" UNDOC>
<MSG ID="10" CLASS="style" NAME='"click here" syndrome detected'>
A keyword defined with the special attribute <CODE>HSC.CLICK-HERE</CODE>
has been found within the text inside an anchor specification.<P>
Ok, now again in english: you used some term like "Click here" inside
the text describing a link. This is very clumsy and should be avoided;
see <A HREF="http://www.cs.cmu.edu/~tilt/cgh/index.html#linktext">Composing
Good HTML</A> for details.

<MSG ID="11" CLASS="error" NAME="unknown <instag>">
You refered to an unknown tag or macro.

<MSG ID="12" CLASS="error" NAME="<instag> occured too often">
A tag that is expected to occure only once appeared at least twice.
Remove all unneccessary occurences of the tag.

<MSG ID="13" CLASS="warning" NAME="unmatched end-<instag>">
You called a end-tag without using the corresponding start-tag
before.

<MSG ID="14" CLASS="warning" NAME="illegal end-tag nesting">
A end-tag appeard where a different end-tag was expected to
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

<MSG ID="17" CLASS="error" NAME="" UNDOC>
<MSG ID="18" CLASS="warning" NAME="unknown entity">
You used an entity, that <hsc> doesn't know. This can happen if you
made a typo, or the entity has not been defined within
<FILE>hsc.prefs</FILE>.

<MSG ID="19" CLASS="error" NAME="" UNDOC>
<MSG ID="20" CLASS="error" NAME="reference to unknown attribute">
You referered to an attribute that doesn't exist at all.

<MSG ID="21" CLASS="warning" NAME="path to URI not found: <I>file</I>">
A tag points to a local resource that doesn't exist.

<MSG ID="22" CLASS="warning" NAME="argument without quote">
You passed a value to an attribute, but did not embed it into single
or double quotes. This can lead to problems with older browsers.

<MSG ID="23" CLASS="error" NAME="empty reference to <insattr>">
An attribute referes to an attribute that has been defined, but no
value has been set for. Usually, you try to refer to an attribute
inside a macro, that no value has been passed to within the call
of the macro.

<MSG ID="24" CLASS="error" NAME="" UNDOC>
<MSG ID="25" CLASS="style" NAME="<TG>BLINK</TG> sucks">
Some browser support the non-html-tag <TG>BLINK</TG>. It is used
to make text blinking, which annoys many users. Additionally, most
of them don't know how to configure their browser that it doesn't
blink, so you really should avoid it.

<MSG ID="26" CLASS="error" NAME="" UNDOC>
<MSG ID="27" CLASS="error" NAME="" UNDOC>
<MSG ID="28" CLASS="error" NAME="" UNDOC>
<MSG ID="29" CLASS="error" NAME="" UNDOC>

<MSG ID="30" CLASS="error" NAME='unmatched "&gt;"'>
A "greater than" sign appeared inside the text. You should write
"<CODE>&amp;gt;</CODE>" instead. This can also happen, if you made
an error calling a tag, and <hsc>'s parser couln't recover.

<MSG ID="31" CLASS="error" NAME="" UNDOC>
<MSG ID="32" CLASS="error" NAME="" UNDOC>
<MSG ID="33" CLASS="error" NAME="" UNDOC>
<MSG ID="34" CLASS="error" NAME="" UNDOC>
<MSG ID="35" CLASS="error" NAME="unknown <insval> for enumerator <insattr>">
You tried to set an enumerator to a value it doesn't support.

<MSG ID="36" CLASS="error" NAME="" UNDOC>

<MSG ID="37" CLASS="warning" NAME="<instag> is obsolete">
The tag was defined within some old html-version, but should
not be used any more (eg. <TG>LISTING</TG>).

<MSG ID="38" CLASS="warning" NAME="<instag> is only used by jerks">
This tag is no legal html-tag and is only supportet by special
browsers.

<MSG ID="39" CLASS="error" NAME="" UNDOC>
<MSG ID="40" CLASS="error" NAME="" UNDOC>
<MSG ID="41" CLASS="error" NAME="" UNDOC>
<MSG ID="42" CLASS="error" NAME="" UNDOC>
<MSG ID="43" CLASS="error" NAME="unknown attribute option <I>option</I>">
You defined a new attribute, but used an
<A HREF=":macro/attrib.html#flags">option</A> that is unknown.

<MSG ID="44" CLASS="error" NAME="required <insattr> missing">
An attribute that is required has not been set within the call
of a tag or macro.

<MSG ID="45" CLASS="error" NAME="" UNDOC>
<MSG ID="46" CLASS="note" NAME="replaced <I>char</I> by <I>entity</I>">
Informs you that a special character (non-7-bit-ASCII) has been replaced
by it's corresponding entity.

<MSG ID="47" CLASS="error" NAME="" UNDOC>
<MSG ID="48" CLASS="error" NAME="" UNDOC>
<MSG ID="49" CLASS="error" NAME="" UNDOC>
<MSG ID="50" CLASS="error" NAME="" UNDOC>
<MSG ID="51" CLASS="error" NAME="" UNDOC>
<MSG ID="52" CLASS="error" NAME="" UNDOC>
<MSG ID="53" CLASS="error" NAME="unmatched <TG>$ELSE</TG>">

An <TG>$else</TG> tag occured without a correspong <TG>$if</TG>.

<MSG ID="54" CLASS="warning" NAME="calling external command returned <I>value</I>">

You've called a shell-command using <TG>$exec</TG> and it returned a
value unequal to zero, which usually denotes an error while processing
the command. For error analysis, look at the output that the command
(hopefully) has made.

<MSG ID="55" CLASS="error" NAME="" UNDOC>
<MSG ID="56" CLASS="error" NAME="" UNDOC>
<MSG ID="57" CLASS="error" NAME="" UNDOC>
<MSG ID="58" CLASS="port" NAME="icon-<insent> found">

Icon-entities are (not-yet) widly supported.

<MSG ID="59" CLASS="error" NAME="" UNDOC>
<MSG ID="60" CLASS="error" NAME="<instag> must be inside <instag>">
Before you are allowed to use the first tag, the second tag has to
occure before. Example: <TG>INPUT</TG> may only occure
inside <TG>FORM</TG>.

<MSG ID="61" CLASS="error" NAME="<instag> not allowed within <instag>">
A tag occures inside another tag that doesn't allowe this. For example,
you can't recursively use <TG>A</TG> inside <TG>A</TG>.

<MSG ID="62" CLASS="error" NAME="no attributes allowed for end-tags">
You tried to pass values to a end-tag or macro. If you need an attribute
for a end-macro only, you still have to define and pass it within the
start-macro; the end-macro will have access to all these attributes
and its values.

<MSG ID="63" CLASS="warning" NAME="redefinance of <insattr>">
You redefined an alredy existing attribute. If this occures during
the definition of a new macro, you just tried to give two arguments the
same name.

<MSG ID="64" CLASS="error" NAME="" UNDOC>
<MSG ID="65" CLASS="error" NAME="" UNDOC>
<MSG ID="66" CLASS="error" NAME="illegal end-tag">
You tried to use a simple tag as a container, for example <TG>/IMG</TG>

<MSG ID="67" CLASS="error" NAME="" UNDOC>
<MSG ID="68" CLASS="note" NAME="unknown filetype">

<hsc> could not <A HREF="features/getsize.html">determine the size</A>
of a link destination.

<MSG ID="69" CLASS="error" NAME="illegal entity definition">

Somethings wrong with a <ln_deftag> or <ln_deficon> call.

<MSG ID="70" CLASS="error" NAME="" UNDOC>
<MSG ID="71" CLASS="error" NAME="" UNDOC>
<MSG ID="72" CLASS="warning" NAME="illegal color value">

A color value must fit the template `#rrggbb' or be one
of the values declared in the special attribute
<A HREF=":features/spcattr.html#colornames"><CODE>HSC.COLOR-NAMES</CODE></A>
defined in <hsc.prefs>

<MSG ID="73" CLASS="error" NAME="" UNDOC>
<MSG ID="74" CLASS="error" NAME="" UNDOC>
<MSG ID="75" CLASS="error" NAME="" UNDOC>
<MSG ID="76" CLASS="style" NAME="frames are disgusting">

This message should need no comment. Anyway, here's the long
form: Frames are ill-minded, inconsitent and intollerant. Even
browser that support frames don't handle their Back/Forward-button
in a reasonable way anymore. And user seeking for informations have
a lot of fun reading a text like "This page can only be displayed on
browsers supporting frames". (In practice, this happens very seldom
because pages that use frames usually don't contain any useful
information.)

<MSG ID="77" CLASS="error" NAME="" UNDOC>
<MSG ID="78" CLASS="style" NAME="succeeding white-space for <instag>">

See below.

<MSG ID="79" CLASS="style" NAME="preceding white-space for <insendtag>">

These two messages point out that instead of (read the underscore (`_')
as a blank)

<BLOCKQUOTE>
<TG>STRONG</TG>__important__<TG>/STRONG</TG>
</BLOCKQUOTE>

you should better use

<BLOCKQUOTE>
<TG>STRONG</TG>important<TG>/STRONG</TG>
</BLOCKQUOTE>

It only affects the tags <TG>A</TG>, <TG>TITLE</TG>, headings and
physical/locigal styles.


</DL>

</WEBPAGE>

