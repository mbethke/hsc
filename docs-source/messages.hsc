<WEBPAGE chapter="hsc - " title="Messages"
    PREV="examples.html"
    NEXT="copy.html">

<$macro MSG_NOTE><A NAME=":errors.html#msg_note">Note</A></$macro>
<$macro MSG_STYLE><A NAME=":errors.html#msg_style">Bad style</A></$macro>
<$macro MSG_WARN><A NAME=":errors.html#msg_warn">Warning</A></$macro>
<$macro MSG_ERR><A NAME=":errors.html#msg_err">Error</A></$macro>
<$macro MSG_FERR><A NAME=":errors.html#msg_ferr">Fatal error</A></$macro>
<$macro OPTN_IGNORE><CODE><A HREF=":options.html#ignore">IGNORE</A></CODE></$macro>

<$macro msg id:string/r name:string/r class:enum(note|style|warning|error|fatal) undoc:bool>
<H3>
<$if set <class>>
  <$insert text=<class>>
</$if>
<$insert text=<id>> : <$insert text=<name>>
</H3>
<$if <undoc>>
Undocumented.
</$if>

</$macro>
<hsc> is able to produce lots of different warnings and
errors when parsing html-files. The messages alway look the same:
<BLOCKQUOTE>
<CODE>"file" (line#, colum#), class and message-id, message</CODE>
</BLOCKQUOTE>

with
<PRE>
  file           input file that caused message; file names surrounded by
                 "[" and "]" indicate pseudo-filenames like macros.
  line#,         position in input file that caused error
  colum#
  class          one of <MSG_NOTE>, <MSG_WARN>, <MSG_ERR> or <MSG_FERR>
  message        message text
</PRE>

<HR>

<H2>Message Classes</H2>

<STRONG><A NAME="msg_note">Note</A></STRONG> is a message only for the
users information. It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_note">Bad style</A></STRONG> informs the user that
his altough legal HTML-code includes constructs that indicate a bad style.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_warn">Warning</A></STRONG> tells you that your source
includes some problematic parts.
It can be suppressed using <OPTN_IGNORE>.
<P>
<STRONG><A NAME="msg_err">Error</A></STRONG> indicate that there is something
wrong with your HTML-source. <* No output is written. *>
<P>
<STRONG><A NAME="msg_ferr">Fatal error</A></STRONG> tell you that something
terrible has happened and there is no way to continue the conversion.
<* No output is written. *>
<P>

<HR>

<STRONG>Note:</STRONG> I've already started to collect the messages, but
this list is not yet complete.
<P>

<MSG ID="1" CLASS="fatal" NAME="out of memory">
There is not enough memory available. Free some memory and restart
<hsc> again.

<MSG ID="2" CLASS="fatal" NAME="write error">
An error occured writing the output file or some of <hsc>'s own
files. Usualy, the cause of the error should be displayed with
this message.

<MSG ID="3" CLASS="fatal" NAME="unexpected end of file">
There are still characters to be read from input, but the file
already ends. Usualy, <hsc> tells you what it still expects to come.

<MSG ID="4" CLASS="error" NAME="" UNDOC>
<MSG ID="5" CLASS="error" NAME="" UNDOC>
<MSG ID="6" CLASS="error" NAME="" UNDOC>
<MSG ID="7" CLASS="error" NAME="" UNDOC>
<MSG ID="8" CLASS="error" NAME="" UNDOC>
<MSG ID="9" CLASS="error" NAME="" UNDOC>
<MSG ID="10" CLASS="error" NAME="" UNDOC>
<MSG ID="11" CLASS="error" NAME="unknown tag">
You refered to an unknown tag or macro.
<MSG ID="12" CLASS="error" NAME="" UNDOC>
<MSG ID="13" CLASS="error" NAME="" UNDOC>
<MSG ID="14" CLASS="error" NAME="" UNDOC>
<MSG ID="15" CLASS="error" NAME="" UNDOC>
<MSG ID="16" CLASS="error" NAME="" UNDOC>
<MSG ID="17" CLASS="error" NAME="" UNDOC>
<MSG ID="18" CLASS="error" NAME="" UNDOC>
<MSG ID="19" CLASS="error" NAME="" UNDOC>
<MSG ID="20" CLASS="error" NAME="" UNDOC>
<MSG ID="21" CLASS="error" NAME="" UNDOC>
<MSG ID="22" CLASS="error" NAME="" UNDOC>
<MSG ID="23" CLASS="error" NAME="" UNDOC>
<MSG ID="24" CLASS="error" NAME="" UNDOC>
<MSG ID="25" CLASS="error" NAME="" UNDOC>
<MSG ID="26" CLASS="error" NAME="" UNDOC>
<MSG ID="27" CLASS="error" NAME="" UNDOC>
<MSG ID="28" CLASS="error" NAME="" UNDOC>
<MSG ID="29" CLASS="error" NAME="" UNDOC>
<MSG ID="30" CLASS="error" NAME="" UNDOC>
<MSG ID="31" CLASS="error" NAME="" UNDOC>
<MSG ID="32" CLASS="error" NAME="" UNDOC>
<MSG ID="33" CLASS="error" NAME="" UNDOC>
<MSG ID="34" CLASS="error" NAME="" UNDOC>
<MSG ID="35" CLASS="error" NAME="" UNDOC>
<MSG ID="36" CLASS="error" NAME="" UNDOC>
<MSG ID="37" CLASS="error" NAME="" UNDOC>
<MSG ID="38" CLASS="error" NAME="" UNDOC>
<MSG ID="39" CLASS="error" NAME="" UNDOC>
<MSG ID="40" CLASS="error" NAME="" UNDOC>
<MSG ID="41" CLASS="error" NAME="" UNDOC>
<MSG ID="42" CLASS="error" NAME="" UNDOC>
<MSG ID="43" CLASS="error" NAME="" UNDOC>
<MSG ID="44" CLASS="error" NAME="" UNDOC>
<MSG ID="45" CLASS="error" NAME="" UNDOC>
<MSG ID="46" CLASS="error" NAME="" UNDOC>
<MSG ID="47" CLASS="error" NAME="" UNDOC>
<MSG ID="48" CLASS="error" NAME="" UNDOC>
<MSG ID="49" CLASS="error" NAME="" UNDOC>
<MSG ID="50" CLASS="error" NAME="" UNDOC>
<MSG ID="51" CLASS="error" NAME="" UNDOC>
<MSG ID="52" CLASS="error" NAME="" UNDOC>
<MSG ID="53" CLASS="error" NAME="" UNDOC>
<MSG ID="54" CLASS="error" NAME="" UNDOC>
<MSG ID="55" CLASS="error" NAME="" UNDOC>
<MSG ID="56" CLASS="error" NAME="" UNDOC>
<MSG ID="57" CLASS="error" NAME="" UNDOC>
<MSG ID="58" CLASS="error" NAME="" UNDOC>
<MSG ID="59" CLASS="error" NAME="" UNDOC>
<MSG ID="60" CLASS="error" NAME="" UNDOC>
<MSG ID="61" CLASS="error" NAME="" UNDOC>
<MSG ID="62" CLASS="error" NAME="" UNDOC>
<MSG ID="63" CLASS="error" NAME="" UNDOC>
<MSG ID="64" CLASS="error" NAME="" UNDOC>
<MSG ID="65" CLASS="error" NAME="" UNDOC>
<MSG ID="66" CLASS="error" NAME="" UNDOC>
<MSG ID="67" CLASS="error" NAME="" UNDOC>
<MSG ID="68" CLASS="error" NAME="" UNDOC>
<MSG ID="69" CLASS="error" NAME="" UNDOC>
<P>
</WEBPAGE>

