<DOCPAGE chapter="hsc - Features - " title="Special Attributes"
    PREV="getsize.html"
    NEXT="strip.html">

<P><hsc CAP> adds some special attributes, which can be quite useful to
include special data or configure the parser.</P>

<UL>
<LI><A HREF="#anchor"><CODE>Hsc.Anchor</CODE></A> - destination URI of last anchor
<LI><A HREF="#clickhere"><CODE>Hsc.Click-Here</CODE></A> - keywords for <qq>click here</qq>-syndrome
<LI><A HREF="#colornames"><CODE>Hsc.Color-Names</CODE></A> - values to be used as colors-names
<LI><A HREF="#content"><CODE>Hsc.Content</CODE></A> - content text of current container macro
<LI><A HREF="#opts-xhtml"><CODE>Hsc.Opts.XHTML</CODE></A> - defined if HSC was called in XHTML mode
<LI><A HREF="#opts-entitystyle"><CODE>Hsc.Opts.Entitystyle</CODE></A> - reflects
the current entity rendering style
<LI><A HREF="#documentattr"><CODE>Hsc.Document.Name</CODE></A> - filename of HTML object
<LI><A HREF="#documentattr"><CODE>Hsc.Document.Path</CODE></A> - directory part
of HTML object, relative to the document tree's root
<LI><A HREF="#documentattr"><CODE>Hsc.Document.URI</CODE></A> - full relative URI of HTML object
<LI><A HREF="#documentattr"><CODE>Hsc.DestPath</CODE></A> - relative path to the
HTML object's destination directory
<LI><A HREF="#exec.result"><CODE>Hsc.Exec.Result</CODE></A> - return-code of command invoked by <ln-exec>
<LI><A HREF="#lf"><CODE>Hsc.LF</CODE></A> - a single linefeed-character
<LI><A HREF="#sourceattr"><CODE>Hsc.Source.Name</CODE></A> - filename of hsc-source
<LI><A HREF="#sourceattr"><CODE>Hsc.Source.Path</CODE></A> - directory part of hsc-source
<LI><A HREF="#sourceattr"><CODE>Hsc.Source.File</CODE></A> - full path and filename of hsc-source
<LI><A HREF="#system"><CODE>Hsc.System</CODE></A> - value depending on operating system
<LI><A HREF="#format.filesize"><CODE>Hsc.Format.Filesize</CODE></A> - how to render <ln-getfilesize>
<LI><A HREF="#format.time"><CODE>Hsc.Format.Time</CODE></A> - how to render <ln-gettime> and <ln-getgmtime>
<LI><A HREF="#tmp"><CODE>Hsc.TMP.*</CODE></A> - temporary attributes for internal use
</UL>

<* insert heading for another special attribute *>
<$macro SPCATTR NAME:string/r TITLE:string/r>
<H3><A NAME=(name)><(title)></A></H3>
</$macro>

<H2>Attributes Containing Special Data</H2>

<**********************************************************>
<* Hsc.Anchor                                             *>
<**********************************************************>
<SPCATTR NAME="anchor" TITLE="Hsc.Anchor">

<P>This attribute is defined internally during startup and is maintained
by <hsc>. It contains the URI of the last anchor referenced to.</P>

<STRONG>Example:</STRONG>

<$include FILE="exmpl/anchor.hsc" SOURCE PRE>

will be converted to:
<BLOCKQUOTE>
<$include FILE="exmpl/anchor.hsc">
</BLOCKQUOTE>

<**********************************************************>
<* Hsc.Content                                            *>
<**********************************************************>
<SPCATTR NAME="content" TITLE="Hsc.Content">

<P>This attribute is defined internally during startup and is
maintained by <hsc>. It contains the the text which will be inserted
if a <TG>$content</TG> shows up, and is updated at every call to a
container macro.</P>

<**********************************************************>
<* Hsc.Opts.XHTML                                         *>
<**********************************************************>
<SPCATTR NAME="opts-xhtml" TITLE="Hsc.Opts.XHMTL">

<P>This <A HREF="../macro/attrib.html#type">BOOL</A> attribute is defined and
set to TRUE (i.e. "1") whenever HSC was called with the XHTML commandline
switch. It allows macros to check the current mode and adapt their code, for
example with respect to doctypes or attributes used. Test it using
<PRE><TG>$if COND=(defined Hsc.Opts.XHMTL)</TG>
... XHTML code ...
<tg>$else</tg>
... old-style HTML code ...
<tg>/$if</tg></pre></P>

<**********************************************************>
<* Hsc.Opts.Entitystyle                                   *>
<**********************************************************>
<SPCATTR NAME="opts-entitystyle" TITLE="Hsc.Opts.Entitystyle">

<P>This is a <A HREF="../macro/attrib.html#type">STRING</A> attribute, which is
always defined and reflects the current entity rendering style, i.e. one of
<qq>keep</qq>, <qq>replace</qq>, <qq>numeric</qq>, <qq>symbolic</qq> or
<qq>utf-8</qq>. It is set by the
<a href=":options.html#entitystyle">ENTITYSTYLE</a> and
<a href=":options.html#xhtml">XHMTL</a> options. The only use of this attribute
will probably be in a macro for the automatic generation of an appropriate
charset declaration.</p>

<**********************************************************>
<* Hsc.Document                                           *>
<**********************************************************>
<SPCATTR NAME="documentattr" TITLE=" Hsc.Document.Name, Hsc.Document.Path, Hsc.Document.URI, Hsc.DestPath">

<P>These attributes are defined internally during startup and are read-only.
They contain the filename, the directory, the whole relative URI 
of the HTML object to be created and the path from the current directory to the
HTML object's destination, respectively.</P>

For example,
<$source PRE>
    hsc FROM people/hugo.hsc TO html:my_project/
</$source>
will lead to

<TABLE BORDER="1" style="font-family: monospace">
<TR>
<TH>attribute</TH>
<TH>value</TH>
</TR>
<TR>
<TD>Hsc.Document.Name</TD>
<TD>"hugo.html"</TD>
</TR>
<TR>
<TD>Hsc.Document.Path</TD>
<TD>"people/"</TD>
</TR>
<TR>
<TD>Hsc.Document.URI</TD>
<TD>"people/hugo.html"</TD>
</TR>
<TR>
<TD>Hsc.DestPath</TD>
<TD>html:my_project/people/</TD>
</TR>
</TABLE>

Note that <CODE>Hsc.Document.Path</CODE> does not contain the whole
destination directory, but only the relative path. If you need the latter, use
<CODE>Hsc.DestPath</CODE>.

<SPCATTR NAME="exec.result" TITLE="Hsc.Exec.Result">

This attribute contains the return-code of the command invoked 
using <ln-exec>. It is updated after every call of this hsc-tag.

<**********************************************************>
<* Hsc.LF                                                 *>
<**********************************************************>
<SPCATTR NAME="lf" TITLE="Hsc.LF">

This attribute contains a single linefeed-character
(<qq><CODE>\n</CODE></qq>) and can be used to include linefeeds into
attribute-values without <hsc> whining about them.

<**********************************************************>
<* Hsc.Source...                                          *>
<**********************************************************>
<SPCATTR NAME="sourceattr" TITLE="Hsc.Source.Name, Hsc.Source.Path, Hsc.Source.File">

<P>These attributes are defined internally during startup and are
read-only. They contain the filename, the directory and the full path
and filename of the hsc-source you have specified when invoking
<hsc>.</P>

For example,

<$source PRE>hsc FROM people/hugo.hsc TO html:my_project/</$source>

will lead to

<TABLE BORDER="1" style="font-family: monospace">
<TR>
<TH>attribute</TH>
<TH>value</TH>
</TR>
<TR>
<TD>Hsc.Source.Name</TD>
<TD>"hugo.hsc"</TD>
</TR>
<TR>
<TD>Hsc.Source.Path</TD>
<TD>"people/"</TD>
</TR>
<TR>
<TD>Hsc.Source.File</TD>
<TD>"people/hugo.hsc"</TD>
</TR>
</TABLE>

<**********************************************************>
<* Hsc.System                                             *>
<**********************************************************>
<SPCATTR NAME="system" TITLE="Hsc.System">

<P>This attribute is defined internally during startup and is
read-only. It contains a value depending on the operating
system <hsc> is running on.</P>

<*<PRE>
    OS      | Hsc.System
    --------+-----------
    AmigaOS | AMIGA
    RiscOS  | RISCOS
    Unixoid | UNIX
</PRE>*>

<TABLE BORDER="1">
<TR>
<TH>OS</TH>
<TH><CODE>Hsc.System</CODE></TH>
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
<TD>BeOS</TD>
<TD>BEOS</TD>
</TR>
<TR>
<TD>NextStep</TD>
<TD>NEXTSTEP</TD>
</TR>
<TR>
<TD>Unixoid</TD>
<TD>UNIX</TD>
</TR>
</TABLE>

This can be useful if you are executing a shell-command during
conversion. See <ln-exec> for an example how to include a
listing of the current directory, working with more then only
a specific OS.

<**********************************************************>
<* Hsc.TMP...                                             *>
<**********************************************************>
<SPCATTR NAME="tmp" TITLE="Hsc.TMP.*">

<P>For several tasks, <hsc> declares it's own temporary attributes for
internal use. Most of them you can not use inside your own
expressions. The only reason why I list them here is, that some of
them might show up in messages.</P>

<P>For example, if you are using the special tag <A
HREF="spctags.html#expression">&lt;(...)&gt;</A> (insert expression),
<hsc> creates an attribute <CODE>Hsc.TMP.insert.expression</CODE> to
assign the value of the expression you passed between the brackets. If
this expression contains errors, you will achieve a message mentioning
the attribute <CODE>Hsc.TMP.insert.expression</CODE>.</P>

<H2>Attributes for configuration</H2>

<**********************************************************>
<* Hsc.Color-Names                                        *>
<**********************************************************>

<SPCATTR NAME="colornames" TITLE="Hsc.Color-Names">

<P>This attribute is defined in <hsc.prefs> and can be altered to the
user's needs. However, it's value is scanned only once (immediately
after processing <hsc.prefs> and later changes will not be recognised
by <hsc>.</P>

<P>For attributes of type 
<A HREF=":macro/attrib.html#type"><CODE>COLOR</CODE></A>, 
you can not only use the
cryptic <CODE>#rrggbb</CODE>-style, but also use some predefined
values. With this attribute you can declare which values <hsc> should
know about, each separated by a <vbar>.</P>

<STRONG>Example:</STRONG>
<$source><$define Hsc.Color-Names:string/c="aqua|black|blue|fuchsia|gray|green|lime|maroon|navy|olive|purple|red|silver|teal|white|yellow"></$source>

<P>This one contains all values recommended for HTML-0.32 and is in
the <hsc.prefs> that came with this distribution.</P>

<**********************************************************>
<* Hsc.Click-Here                                         *>
<**********************************************************>
<SPCATTR NAME="clickhere" TITLE="Hsc.Click-Here">
<P>This attribute is defined in <hsc.prefs> and can be altered to the
user's needs. However, it's value is only scanned once and later
changes will not be recognised by <hsc>.</P>

Inside an anchor tag (<TG>A HREF="..."</TG>), the normal text is
scanned for special keywords indicating a <qq>click here</qq>-syndrome.
With this attribute you can declare these keywords,
each separated by a <vbar>.

<STRONG>Example:</STRONG>
<$source PRE><$define Hsc.Click-Here:string/c="click|here"></$source>

<P>When processing German documents, probably this one would
fit better:</P>
<$source PRE><$define Hsc.Click-Here:string/c="klicken|hier"></$source>

<**********************************************************>
<* Hsc.Format.filesize                                    *>
<**********************************************************>
<SPCATTR NAME="format.filesize" TITLE="Hsc.Format.Filesize">

This attribute contains a template that descibes how the result
of <ln-getfilesize> should be rendered. Conversion specifications:

<PRE>
    <STRONG>%b</STRONG>    is replaced by the filesize in bytes.
    <STRONG>%k</STRONG>    is replaced by the filesize in kilobytes.
    <STRONG>%m</STRONG>    is replaced by the filesize in megabytes.
    <STRONG>%g</STRONG>    is replaced by the filesize in gigabytes.
    <STRONG>%a</STRONG>    is replaced by the filesize, with a reasonable unit computed automatically
    <STRONG>%u</STRONG>    is replaced by the unit for <STRONG>%a</STRONG>
</PRE>
This attribute is defined internally during startup, and contains the
value <CODE>"%a%u"</CODE>, leading to results like <CODE>"123K"</CODE> or
<CODE>"12M"</CODE>. It can be changed by the user at any time.

<**********************************************************>
<* Hsc.Format.Time                                        *>
<**********************************************************>
<SPCATTR NAME="format.time" TITLE="Hsc.Format.Time">

This attribute contains a template that describes how the result
of <ln-gettime> and <ln-getgmtime> should be rendered.
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
    <STRONG>%d</STRONG>    is replaced by the day of the month as a decimal number (01-31).
    <STRONG>%e</STRONG>    is replaced by the day of month as a decimal number (1-31); single
           digits are preceded by a blank.
    <STRONG>%H</STRONG>    is replaced by the hour (24-hour clock) as a decimal number
           (00-23).
    <STRONG>%I</STRONG>    is replaced by the hour (12-hour clock) as a decimal number
           (01-12).
    <STRONG>%j</STRONG>    is replaced by the day of the year as a decimal number (001-366).
    <STRONG>%k</STRONG>    is replaced by the hour (24-hour clock) as a decimal number (0-23);
           single digits are preceded by a blank.
    <STRONG>%l</STRONG>    is replaced by the hour (12-hour clock) as a decimal number (1-12);
           single digits are preceded by a blank.
    <STRONG>%M</STRONG>    is replaced by the minute as a decimal number (00-59).
    <STRONG>%m</STRONG>    is replaced by the month as a decimal number (01-12).
    <STRONG>%n</STRONG>    is replaced by a newline.
    <STRONG>%p</STRONG>    is replaced by either <qq>AM</qq> or <qq>PM</qq> as appropriate.
    <STRONG>%R</STRONG>    is equivalent to <qq>%H:%M</qq>
    <STRONG>%r</STRONG>    is equivalent to <qq>%I:%M:%S %p</qq>.
    <STRONG>%t</STRONG>    is replaced by a tab.
    <STRONG>%S</STRONG>    is replaced by the second as a decimal number (00-60).
    <STRONG>%s</STRONG>    is replaced by the number of seconds since the Epoch, UCT.
    <STRONG>%T or %X</STRONG>    
           is equivalent to <qq>%H:%M:%S</qq>.
    <STRONG>%U</STRONG>    is replaced by the week number of the year (Sunday as the first day
           of the week) as a decimal number (00-53).
    <STRONG>%W</STRONG>    is replaced by the week number of the year (Monday as the first day
           of the week) as a decimal number (00-53).
    <STRONG>%w</STRONG>    is replaced by the weekday (Sunday as the first day of the week) as
           a decimal number (0-6).
    <STRONG>%x</STRONG>    is equivalent to <qq>%m/%d/%y %H:%M:%S</qq>.
    <STRONG>%Y</STRONG>    is replaced by the year with century as a decimal number.
    <STRONG>%y</STRONG>    is replaced by the year without century as a decimal number
           (00-99).
    <STRONG>%Z</STRONG>    is replaced by the time zone name.
    <STRONG>%%</STRONG>    is replaced by <qq>%</qq>.
</PRE>
This attribute is defined internally during startup, and contains the
value <qqc>%d-%b-%Y, %H:%M</qqc>, leading to results like
<CODE>"<(GetTime())></CODE>".
<BR>
</DOCPAGE>
