<WEBPAGE chapter="hsc - Features - " title="Expressions"
    PREV=":require.html"
    NEXT="spctags.html">

<P>Additionally to simple strings, you can use expressions to set the
value of an attribute. Expressions <STRONG>must</STRONG> be enclosed
inside "<CODE>(</CODE>" and "<CODE>)</CODE>".
Inside expressions, you can refer to attributes simply by their name.
String constants must be enclosed inside quotes.</P>

<STRONG>Example:</STRONG><BR>
<$source PRE>
    <$define image:string="hugo.gif">
    <IMG SRC=(image) ALT="image">
</$source>

<P>will be converted to</P>

<$source PRE>
    <IMG SRC="hugo.gif" ALT="image">
</$source>

<A NAME="operators"><H2>Operators</H2></A>

<H3>Unary operators</H3>
<DL>
    <DT><CODE>not</CODE> <I>expression</I>
    <DD>negotiate (boolean) expression
    <DT><CODE>set</CODE> <I>attribute</I>
    <DD>True, if attribute has been set during macro call
    <DT><CODE>defined</CODE> <I>attribute</I>
    <DD>True, if attribute was defined with <TG>$macro</TG> or
        <TG>$define</TG>
    <DT><CODE>exists(</CODE><I>local uri</I><CODE>)</CODE>
    <DD>True, if document at local URI exists (<CODE>bool</CODE>)
    <DT><CODE><A NAME="getenv">GetEnv</A>(</CODE><I>environment-variable</I><CODE>)</CODE>
    <DD>get value of an environment variable
    <DT><CODE><A NAME="getfilesize">GetFileSize</A>(</CODE><I>uri</I><CODE>)</CODE>
    <DD>get size of a specific file
    <DT><CODE><A NAME="getgmtime">GetGMTime()</A></CODE>
    <DD>get current Greenwich Mean time
    <DT><CODE><A NAME="gettime">GetTime()</A></CODE>
    <DD>get current local time
</DL>

<H3>Binary operators</H3>
<DL>
    <DT><I>expression</I> <STRONG>=</STRONG> <I>expression</I>
    <DD>string comparison (case insensitive)
    <DT><I>expression</I> <STRONG>+</STRONG> <I>expression</I>
    <DD>string concatenation
</DL>

<STRONG>Example:</STRONG><BR>
<$source PRE>
    <$define name:string="hugo">
    <$define here:string="here">

    <IMG SRC=(name+".gif") ALT=(name+" was "+here)>
    <$if (name="hugo")>
    This is hugo!
    <$else>
    Maybe it's sepp?
    </$if>
    AmigaOS version: <(GetEnv ("KickStart"))>
</$source>

<P>will be converted to</P>

<PRE>
    <TG>IMG SRC="hugo.gif" ALT="hugo was here"</TG>
    This is hugo!
    AmigaOS version: <(GetEnv ("KickStart"))>
</PRE>

<P>At least on my machine.</P>

<A NAME="boolean"><H2>Boolean expressions</H2></A>

<P>If you pass an expression to a boolean attribute, the expression is
evaluated as before. If the expression returned an empty string,
the boolean attribute is interpreted as <CODE>false</CODE>. This will
cause the attribute to be removed from the tag/macro-call.</P>

<P>Any none-empty string will set the attribute to <CODE>true</CODE>,
resulting in a value equal to the name of attribute. (In html, writing 
<CODE>ISMAP</CODE> is short for <CODE>ISMAP="ISMAP"</CODE>.)</P>

<STRONG>Example:</STRONG><BR>
<$source PRE>
    <IMG SRC=(name) ALT="nufin" ISMAP=(name="map.gif")>
</$source>

<P>will be converted to</P>

<$source PRE>
    <IMG SRC="hugo.gif" ALT="nufin">
</$source>

<P>if <CODE>name</CODE> has been set to <CODE>"hugo.gif"</CODE>, or to</P>

<$source PRE>
    <IMG SRC="map.gif" ALT="nufin" ISMAP>
</$source>

<P>if <CODE>name</CODE> has been set to <CODE>"map.gif"</CODE>. Note that
only the second call enables the boolean attribute <CODE>ISMAP</CODE>,
while it gets stripped for the first call.</P>

<A NAME="priorities"><H2>Priorities</H2></A>
<P><STRONG>Important:</STRONG> Different to most programming languages, <hsc>
does not support priorities for different operators. Therefor, expressions
are simply processed sequentialy (I'm too lazy to build a tree).</P>

<P>But you can use nested brakets within complex expressions.</P>

</WEBPAGE>
