<WEBPAGE chapter="hsc - Features - " title="Special Attributes"
    PREV=":distrib.html"
    NEXT="syntax.html">

<P><B>hsc</B> adds some special attributes, which can be quite useful to
include special data or configure the parser.</P>

<UL>
<LI><A HREF="#anchor"><CODE>HSC.ANCHOR</CODE></A> contains destination URI of last anchor
<LI><A HREF="#clickhere"><CODE>HSC.CLICK-HERE</CODE></A> declare keywords for "click here"-syndrome
<LI><A HREF="#colornames"><CODE>HSC.COLOR-NAMES</CODE></A> declare values to be used as colors-names
<LI><A HREF="#colornames"><CODE>HSC.CONTENT</CODE></A> access content text of current container macro
<LI><A HREF="#documentattr"><CODE>HSC.DOCUMENT.NAME</CODE></A> filename of html-object
<LI><A HREF="#documentattr"><CODE>HSC.DOCUMENT.PATH</CODE></A> directory of html-object
<LI><A HREF="#documentattr"><CODE>HSC.DOCUMENT.URI</CODE></A> full relative URI of html-object
<LI><A HREF="#exec.result"><CODE>HSC.EXEC.RESULT</CODE></A> contains return-code of command invoked by <ln_exec>
<LI><A HREF="#lf"><CODE>HSC.LF</CODE></A> contains a single linefeed-character
<LI><A HREF="#sourceattr"><CODE>HSC.SOURCE.NAME</CODE></A> filename of hsc-source
<LI><A HREF="#sourceattr"><CODE>HSC.SOURCE.PATH</CODE></A> directory of hsc-source
<LI><A HREF="#sourceattr"><CODE>HSC.SOURCE.FILE</CODE></A> full path and filename of hsc-source
<LI><A HREF="#system"><CODE>HSC.SYSTEM</CODE></A> contains value depending on operating system
<LI><A HREF="#format.filesize"><CODE>HSC.FORMAT.FILESIZE</CODE></A> how to render <ln_getfilesize>
<LI><A HREF="#format.time"><CODE>HSC.FORMAT.TIME</CODE></A> how to render <ln_gettime> and <ln_getgmtime>
<LI><A HREF="#tmp"><CODE>HSC.TMP.*</CODE></A> temporary attributes for internal use
</UL>

<* insert heading for another special attribute *>
<$macro SPCATTR NAME:string/r TITLE:string/r>
<H3><A NAME=(name)><(title)></A></H3>
</$macro>

<H2>Attributes containing special data</H2>

<**********************************************************>
<* HSC.Anchor                                             *>
<**********************************************************>
<SPCATTR NAME="anchor" TITLE="HSC.Anchor">

<P>This attribute is defined internally during startup and is maintained
by <hsc>. It contains the URI of the last anchor referenced to.</P>

<STRONG>Example:</STRONG>

<$include FILE="exmpl/anchor.hsc" SOURCE PRE>

will be converted to:
<BLOCKQUOTE>
<$include FILE="exmpl/anchor.hsc">
</BLOCKQUOTE>

<**********************************************************>
<* HSC.Content                                            *>
<**********************************************************>
<SPCATTR NAME="content" TITLE="HSC.Content">

<P>This attribute is defined internally during startup and is
maintained by <hsc>. It contains the the text which will be inserted
if a <TG>$content</TG> shows up, and is updated at every call to a
container macro.

<**********************************************************>
<* HSC.Document                                           *>
<**********************************************************>
<SPCATTR NAME="documentattr" TITLE=" HSC.DOCUMENT.NAME, HSC.DOCUMENT.PATH, HSC.DOCUMENT.URI">

<P>These attributes are defined internally during startup and are read-only.
They contain the filename, the directory and the whole
relative URI of the to be created html-object.</P>

For example,
<$source PRE>
    hsc FROM people/hugo.hsc TO html:my_project/
</$source>
will lead to

<*<$source PRE>
    attribute         | value
    ------------------+----------------
    HSC.DOCUMENT.NAME | "hugo.html"
    HSC.DOCUMENT.PATH | "people/"
    HSC.DOCUMENT.URI  | "people/hugo.html"
</$source>*>

<TABLE BORDER="1">
<TR>
<TH>attribute</TH>
<TH>value</TH>
</TR>
<TR>
<TD>HSC.DOCUMENT.NAME</TD>
<TD>"hugo.html"</TD>
</TR>
<TR>
<TD>HSC.DOCUMENT.PATH</TD>
<TD>"people/"</TD>
</TR>
<TR>
<TD>HSC.DOCUMENT.URI</TD>
<TD>"people/hugo.html"</TD>
</TR>
</TABLE>

Note that <CODE>HSC.DOCUMENT.PATH</CODE> does not contain the whole
destination-directory, but only the relative path.

<SPCATTR NAME="exec.result" TITLE="HSC.EXEC.RESULT">

This attribute contains the return-code of the command invoked 
using <ln_exec>. It is updated after every call of this hsc-tag.

<**********************************************************>
<* HSC.LF                                                 *>
<**********************************************************>
<SPCATTR NAME="lf" TITLE="HSC.LF">

This attribute contains a single linefeed-character (<qq><CODE>\n</CODE></qq>)
and can be used to include linefeeds into attribute-values without
<hsc> whining about them.

<**********************************************************>
<* HSC.Source...                                          *>
<**********************************************************>
<SPCATTR NAME="sourceattr" TITLE="HSC.SOURCE.NAME, HSC.SOURCE.PATH, HSC.SOURCE.FILE">

<P>These attributes are defined internally during startup and are read-only.
They contain the filename, the directory and the full path and filename
of the hsc-source you've passed specified when invoking <hsc>.</P>

For example,
<$source PRE>
    hsc FROM people/hugo.hsc TO html:my_project/
</$source>
will lead to

<*<$source PRE>
    attribute       | value
    ----------------+----------------
    HSC.SOURCE.NAME | "hugo.hsc"
    HSC.SOURCE.PATH | "people/"
    HSC.SOURCE.FILE | "people/hugo.hsc"
</$source>*>

<TABLE BORDER="1">
<TR>
<TH>attribute</TH>
<TH>value</TH>
</TR>
<TR>
<TD>HSC.SOURCE.NAME</TD>
<TD>"hugo.hsc"</TD>
</TR>
<TR>
<TD>HSC.SOURCE.PATH</TD>
<TD>"people/"</TD>
</TR>
<TR>
<TD>HSC.SOURCE.URI</TD>
<TD>"people/hugo.hsc"</TD>
</TR>
</TABLE>

<**********************************************************>
<* HSC.System                                             *>
<**********************************************************>
<SPCATTR NAME="system" TITLE="HSC.SYSTEM">

<P>This attribute is defined internally during startup and is
read-only. It contains a value depending on the operating
system <hsc> is running on.</P>

<*<PRE>
    OS      | HSC.SYSTEM
    --------+-----------
    AmigaOS | AMIGA
    RiscOS  | RISCOS
    Unixoid | UNIX
</PRE>*>

<TABLE BORDER="1">
<TR>
<TH>OS</TH>
<TH><CODE>HSC.SYSTEM</CODE></TH>
</TR>
<TR>
<TD>AmigaOS</TD>
<TD>AMIGA</TD>
</TR>
<TR>
<TD>RiscOS</TD>
<TD>RISCOS</TD>
</TR>
<TR>
<TD>Unixoid</TD>
<TD>UNIX</TD>
</TR>
</TABLE>

This can be useful if you are executing a shell-command during
conversion. See <ln_exec> for an example how to include a
listing of the current directory, working with more then only
a specific OS.

<**********************************************************>
<* HSC.TMP...                                             *>
<**********************************************************>
<SPCATTR NAME="tmp" TITLE="HSC.TMP.*">

<P>For several tasks, <hsc> declares it's own temporary attributes
for internal use. Most of them can't even be refered to by you.
The only reason why I list them here is, that some of them might
show up in messages.</P>

<P>For example, if you are using the special tag 
<A HREF="spctags.html#expression">&lt;(...)&gt;</A> (insert expression),
<hsc> creates an attribute <CODE>HSC.TMP.insert.expression</CODE> to
assign the value of the expression you passed between the brackets.
If this expression now is corrupt, you will achieve a message mentioning
the attribute <CODE>HSC.TMP.insert.expression</CODE>.</P>

<H2>Attributes for configuration</H2>

<**********************************************************>
<* HSC.Color-Names                                        *>
<**********************************************************>
<SPCATTR NAME="colornames" TITLE="HSC.COLOR-NAMES">
<P>This attribute is defined in <hsc.prefs> and can be altered to the
user's needs. However, it's value is only scanned once and later
changes will not be recognised by <hsc>.</P>

<P>For attributes of type 
<A HREF=":macro/attrib.html#type"><CODE>COLOR</CODE></A>, 
you can not only use the
cryptic <CODE>#rrggbb</CODE>-style, but also use some predefined
values. With this attribute you can declare which values <hsc> should
know about, each separated by a <vbar>.</P>

<STRONG>Example:</STRONG>
<$source><$define HSC.COLOR-NAMES:string/c="aqua|black|blue|fuchsia|gray|green|lime|maroon|navy|olive|purple|red|silver|teal|white|yellow"></$source>

<P>This one contains all values recommended for html-0.32 and is in
the <hsc.prefs> that came with this distribution.</P>

<**********************************************************>
<* HSC.Click-Here                                         *>
<**********************************************************>
<SPCATTR NAME="clickhere" TITLE="HSC.CLICK-HERE">
<P>This attribute is defined in <hsc.prefs> and can be altered to the
user's needs. However, it's value is only scanned once and later
changes will not be recognised by <hsc>.</P>

Inside an anchor tag (<TG>A HREF="..."</TG>), the normal text is
scanned for special keywords indicating a <qq>click here</qq>-syndrome.
With this attribute you can declare these keywords,
each separated by a <vbar>.

<STRONG>Example:</STRONG>
<$source PRE><$define HSC.CLICK-HERE:string/c="click|here"></$source>

<P>When processing German documents, probably this one would
fit better:</P>
<$source PRE><$define HSC.CLICK-HERE:string/c="klicken|hier"></$source>

<**********************************************************>
<* HSC.Format.filesize                                    *>
<**********************************************************>
<SPCATTR NAME="format.filesize" TITLE="HSC.FORMAT.FILESIZE">

This attribute contains a template that descibes how the result
of <ln_getfilesize> should be rendered. Conversion specifications:

<PRE>
    <STRONG>%b</STRONG>    is replaced by the filesize in bytes.
    <STRONG>%k</STRONG>    is replaced by the filesize in kilo-bytes.
    <STRONG>%m</STRONG>    is replaced by the filesize in mega-bytes.
    <STRONG>%g</STRONG>    is replaced by the filesize in giga-bytes.
    <STRONG>%a</STRONG>    is replaced by the filesize, with a reasonable unit computed automatically
    <STRONG>%u</STRONG>    is replaced by the unit for <STRONG>%a</STRONG>
</PRE>
This attribute is defined internally during startup, and contains the
value <CODE>"%a%u"</CODE>, leading to results like <CODE>"123K"</CODE> or
<CODE>"12M"</CODE>. It can be changed by the user at any time.

<**********************************************************>
<* HSC.Format.Time                                        *>
<**********************************************************>
<SPCATTR NAME="format.time" TITLE="HSC.FORMAT.TIME">

This attribute contains a template that describes how the result
of <ln_gettime> and <ln_getgmtime> should be rendered. 
Conversion specifications 
(Techn. Note: These are the same as for ANSI-C's <CODE>strftime()</CODE>):
<PRE>
    <STRONG>%A</STRONG>    is replaced by the full weekday name.
    <STRONG>%a</STRONG>    is replaced by the abbreviated weekday name, where the abbreviation
           is the first three characters.
    <STRONG>%B</STRONG>    is replaced by the full month name.
    <STRONG>%b or %h</STRONG>
           is replaced by the abbreviated month name, where the abbreviation
           is the first three characters.
    <STRONG>%C</STRONG>    is equivalent to <qq>%a %b %e %H:%M:%S %Y</qq>
    <STRONG>%c</STRONG>    is equivalent to <qq>%m/%d/%y</qq>.
    <STRONG>%D</STRONG>    is replaced by the date in the format <qq>mm/dd/yy</qq>.
    <STRONG>%d</STRONG>    is replaced by the day of the month as a decimal number (01­31).
    <STRONG>%e</STRONG>    is replaced by the day of month as a decimal number (1­31); single
           digits are preceded by a blank.
    <STRONG>%H</STRONG>    is replaced by the hour (24­hour clock) as a decimal number
           (00­23).
    <STRONG>%I</STRONG>    is replaced by the hour (12­hour clock) as a decimal number
           (01­12).
    <STRONG>%j</STRONG>    is replaced by the day of the year as a decimal number (001­366).
    <STRONG>%k</STRONG>    is replaced by the hour (24­hour clock) as a decimal number (0­23);
           single digits are preceded by a blank.
    <STRONG>%l</STRONG>    is replaced by the hour (12­hour clock) as a decimal number (1­12);
           single digits are preceded by a blank.
    <STRONG>%M</STRONG>    is replaced by the minute as a decimal number (00­59).
    <STRONG>%m</STRONG>    is replaced by the month as a decimal number (01­12).
    <STRONG>%n</STRONG>    is replaced by a newline.
    <STRONG>%p</STRONG>    is replaced by either <qq>AM</qq> or <qq>PM</qq> as appropriate.
    <STRONG>%R</STRONG>    is equivalent to <qq>%H:%M</qq>
    <STRONG>%r</STRONG>    is equivalent to <qq>%I:%M:%S %p</qq>.
    <STRONG>%t</STRONG>    is replaced by a tab.
    <STRONG>%S</STRONG>    is replaced by the second as a decimal number (00­60).
    <STRONG>%s</STRONG>    is replaced by the number of seconds since the Epoch, UCT.
    <STRONG>%T or %X</STRONG>    
           is equivalent to <qq>%H:%M:%S</qq>.
    <STRONG>%U</STRONG>    is replaced by the week number of the year (Sunday as the first day
           of the week) as a decimal number (00­53).
    <STRONG>%W</STRONG>    is replaced by the week number of the year (Monday as the first day
           of the week) as a decimal number (00­53).
    <STRONG>%w</STRONG>    is replaced by the weekday (Sunday as the first day of the week) as
           a decimal number (0­6).
    <STRONG>%x</STRONG>    is equivalent to <qq>%m/%d/%y %H:%M:%S</qq>.
    <STRONG>%Y</STRONG>    is replaced by the year with century as a decimal number.
    <STRONG>%y</STRONG>    is replaced by the year without century as a decimal number
           (00­99).
    <STRONG>%Z</STRONG>    is replaced by the time zone name.
    <STRONG>%%</STRONG>    is replaced by <qq>%</qq>.
</PRE>
This attribute is defined internally during startup, and contains the
value <qqc>%d-%b-%Y, %H:%M</qqc>, leading to results like
<CODE>"<(GetTime())></CODE>".
<BR>
</WEBPAGE>
