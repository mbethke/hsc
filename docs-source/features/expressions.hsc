<WEBPAGE chapter="hsc - Features - " title="Expressions"
    PREV="assign.html"
    NEXT=":macro/macros.html">

<P>Additionally to simple strings, you can use expressions to set the
value of an attribute. Inside an expressions, you can refer to other
attributes to obtain their value, and you also can use several operators.

<H2>General Syntax</H2>

<P>To assign an expression, you must start with a <bracket> after the
<equal-sign> of the assignment. The expression ends with a
<closing-bracket>.</P>

<P>String constants must be enclosed inside any kind of quotes -
different to values outside expressions, where the quotes can
be omitted under certain circumstances.</P>

<P>For referring to the value of another attribute, simply use the
name of it - without any enclosing quotes. The source attribute must
exist, either by using <ln_define> or by being part of a macro
declaration.</P>

<P>Furthermore it must contain a value, for example by means of
<ln_let> or by setting it within a macro call. Attributes being
defined without a default value or not being set within a macro call
do not contain any data and therefor will have to be updated using
<ln_let> before using them.</P>

<STRONG>Example:</STRONG><BR>
<$source PRE>
<$define image:string="hugo.gif">
<IMG SRC=(image) ALT="image">
</$source>

<P>will be converted to</P>

<$source PRE><IMG SRC="hugo.gif" ALT="image"></$source>

<H2><A NAME="operators">Operators</A></H2>

<H3>Unary Operators</H3>
<DL>
    <DT><A NAME="not"><CODE>not</CODE></A> <I>expression</I>
    <DD>Negotiate (boolean) expression
    <DT><A NAME="set"><CODE>set</CODE></A> <I>attribute</I>
    <DD>True, if attribute has been set (read: passed a value)
        within macro call.
    <DT><A NAME="defined"><CODE>defined</CODE></A> <I>attribute</I>
    <DD>True, if attribute was defined with <TG>$macro</TG> or
        <TG>$define</TG>
    <DT><A NAME="exists"><CODE>Exists(</CODE></A><I>local uri</I><CODE>)</CODE>
    <DD>True, if document at local URI exists (<CODE>bool</CODE>).
        This can also be specified as a <fe_prjuri>.<BR>
        <ExampleNote><CODE>Exists("index.html")</CODE>,
                     <CODE>Exists(":image/next.gif")</CODE>
    <DT><A NAME="fexists"><CODE>fExists(</CODE></A><I>filename</I><CODE>)</CODE>
    <DD>True, if a file exists (<CODE>bool</CODE>). If you do not specify
        a full filename (including a device name), it will be relative to
        the source root directory.<BR>
        <ExampleNote><CODE>fExists("sepp:hugo/resi.hsc")</CODE>,
                     <CODE>fExists("///child.txt")</CODE>,
                     <CODE>fExists("include/global.hsc")</CODE>
    <DT><CODE><A NAME="getenv">GetEnv</A>(</CODE><I>environment-variable</I><CODE>)</CODE>
    <DD>Get value of an environment variable.<BR>
        <ExampleNote><CODE>GetEnv("Workbench")</CODE>
    <DT><CODE><A NAME="getfilesize">GetFileSize</A>(</CODE><I>local uri</I><CODE>)</CODE>
    <DD>Get size of a specific document.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.filesize"><CODE>HSC.Format.FileSize</CODE></A>
        to change the appearence of the result.<BR>
        <ExampleNote><CODE>GetFileSize("../../download/hugo.lha")</CODE>,
                     <CODE>GetFileSize(":nudes/resi.jpg")</CODE>
    <DT><CODE><A NAME="getgmtime">GetGMTime()</A></CODE>
    <DD>Get current Greenwich Mean time.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.time"><CODE>HSC.Format.Time</CODE></A>
        to change the appearence of the result.<BR>
    <DT><CODE><A NAME="gettime">GetTime()</A></CODE>
    <DD>Get current local time.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.time"><CODE>HSC.Format.Time</CODE></A>
        to change the appearence of the result.<BR>
</DL>

<H3>Binary Operators</H3>
<DL>
    <DT><I>expression</I> <STRONG>=</STRONG> <I>expression</I>
    <DD>string comparison (case insensitive)
    <DT><I>expression</I> <STRONG>+</STRONG> <I>expression</I>
    <DD>string concatenation
    <DT><I>little</I> <STRONG>IN</STRONG> <I>big</I>
    <DD>search for substring <I>littel</I> in <I>big</I> (case insensitive)
</DL>

<STRONG>Example:</STRONG><BR>
<$source PRE>
<$define name:string="hugo">
<$define here:string="here">

<IMG SRC=(name+".gif") ALT=(name+" was "+here)>
<$if cond=(name="hugo")>
This is hugo!
<$else>
Maybe it's sepp?
</$if>
<$if cond=("SePp" IN "hugo,sepp and resi")>
Sepp is among them.
</$if>
AmigaOS version: <(GetEnv ("KickStart"))>
</$source>

<P>will be converted to</P>

<PRE>
<TG>IMG SRC="hugo.gif" ALT="hugo was here"</TG>
This is hugo!
Sepp is among them.
AmigaOS version: <(GetEnv ("KickStart"))>
</PRE>

<P>At least on my machine.</P>

<H2><A NAME="boolean">Boolean Expressions</A></H2>

<P>If you pass an expression to a boolean attribute, the expression is
evaluated as before. If the expression returned an empty string,
the boolean attribute is interpreted as <CODE>false</CODE>. This will
cause the attribute to be removed from the tag/macro-call.</P>

<P>Any none-empty string will set the attribute to <CODE>true</CODE>,
resulting in a value equal to the name of attribute. (In html, writing 
<CODE>ISMAP</CODE> is short for <CODE>ISMAP="ISMAP"</CODE>.)</P>

<STRONG>Example:</STRONG>

<$source PRE><IMG SRC=(name) ALT="nufin" ISMAP=(name="map.gif")></$source>

<P>will be converted to</P>

<$source PRE><IMG SRC="hugo.gif" ALT="nufin"></$source>

<P>if <CODE>name</CODE> has been set to <CODE>"hugo.gif"</CODE>,
or to</P>

<$source PRE><IMG SRC="map.gif" ALT="nufin" ISMAP></$source>

<P>if <CODE>name</CODE> has been set to <CODE>"map.gif"</CODE>. Note that
only the second call enables the boolean attribute <CODE>ISMAP</CODE>,
while it gets stripped for the first call.</P>

<H2><A NAME="priorities">Priorities</A></H2>
<P><STRONG>Important:</STRONG> Different to most programming languages, <hsc>
does not support priorities for different operators. Therefor, expressions
are simply processed sequentially (Programmer's lazyness rules).</P>

<P>But you can use nested brackets within complex expressions.</P>

</WEBPAGE>
