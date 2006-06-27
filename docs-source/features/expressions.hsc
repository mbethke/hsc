<DOCPAGE chapter="hsc - Features - " title="Expressions"
    PREV="assign.html"
    NEXT=":macro/macros.html">

<P>In addition to simple strings, you can use expressions to set the
value of an attribute. Within expressions, you can refer to other
attributes to obtain their value, and you can also use various operators.

<H2>General Syntax</H2>

<P>Usually, expressions have to be enclosed in parentheses
(``<code>(...)</code>'') and may appear after the <equal-sign> of an assignment
(see <a href="#symref">below</a> for an exception!).</p>

<P>String constants must be enclosed inside any kind of quotes -
different to values outside expressions, where the quotes can
be omitted under certain circumstances. As a difference from most ``real''
programming languages, the same is true of
<a href="assign.html#numconst">numeric constants</a>!</P>

<P>To refer to the value of another attribute, simply use its
name, without any enclosing quotes. The source attribute must
exist, either defined via <ln-define> or by being part of a macro
declaration.</P>

<P>Furthermore, it must have been assigned a value, for example using
<ln-let>, or by setting it within a macro call. Attributes 
defined without a default value or not being set within a macro call
do not contain any data and therefore will have to be updated using
<ln-let> before using them.</P>

<STRONG>Example:</STRONG><BR>
<$source PRE>
<$define image:string="hugo.gif">
<IMG SRC=(image) ALT="image">
</$source>

<P>will be converted to</P>

<$source PRE><IMG SRC="hugo.png" ALT="image"></$source>

<H2><A NAME="operators">Operators</A></H2>

<H3>Unary Operators</H3>
<DL>
    <DT><A NAME="not"><CODE>not</CODE></A> <I>expression</I>
    <DD>Negate (boolean) expression

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

    <DT><A NAME="chr"><CODE>chr(</CODE></A><I>number</I><CODE>)</CODE>
    <DD>Yields the character whose ASCII code is <i>number</i>. If
        <i>number</i> is greater than 255, it will be taken modulo 256. An
        empty value, zero or a non-numeric value yields an empty string.

    <DT><A NAME="ord"><CODE>ord(</CODE></A><I>character</I><CODE>)</CODE>
    <DD>Yields the numeric ASCII code of <i>character</i>. If the expression
        passed to <code>ord()</code> is a string of more than one character,
        all but the first are ignored. An empty string yields zero.

    <DT><CODE><A NAME="getenv">GetEnv</A>(</CODE><I>environment-variable</I><CODE>)</CODE>
    <DD>Get value of an environment variable.<BR>
        <ExampleNote><CODE>GetEnv("Workbench")</CODE>

    <DT><CODE><A NAME="getfilesize">GetFileSize</A>(</CODE><I>local uri</I><CODE>)</CODE>
    <DD>Get size of a specific document.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.filesize"><CODE>HSC.Format.FileSize</CODE></A>
        to change the appearance of the result.<BR>
        <ExampleNote><CODE>GetFileSize("../../download/hugo.lha")</CODE>,
                     <CODE>GetFileSize(":nudes/resi.jpg")</CODE>

    <DT><CODE><A NAME="getgmtime">GetGMTime()</A></CODE>
    <DD>Get current Greenwich Mean time.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.time"><CODE>HSC.Format.Time</CODE></A>
        to change the appearance of the result.<BR>

    <DT><CODE><A NAME="gettime">GetTime()</A></CODE>
    <DD>Get current local time.
        You can use the attribute
        <A HREF=":features/spcattr.html#format.time"><CODE>HSC.Format.Time</CODE></A>
        to change the appearance of the result.<BR>

    <DT><CODE><A NAME="getfiledate">GetFileDate()</A></CODE>
    <DD>Get the modification time of a file
        You can use the attribute
        <A HREF=":features/spcattr.html#format.time"><CODE>HSC.Format.Time</CODE></A>
        to change the appearance of the result.<BR>
        To show the time of the last change to a page, this is often a more
        logical choice than <A HREF="#gettime">GetTime()</A>, especially if you
        include a lot of macros and use only a few in a certain document.<BR>
        <ExampleNote><code>Last change: <tg>(GetFileDate(hsc.source.file))</tg></code>

    <DT><CODE><A NAME="basename">basename</A></CODE>
    <DD>Returns the part of a string before the last period. If the name has no
extension, returns the entire input string.

    <DT><CODE><A NAME="extension">extension</A></CODE>
    <DD>Returns the extension of a filename, i.e. the part after the last
        period, or the empty string if there is no extension.

    <DT><CODE><A NAME="urikind">urikind</A></CODE>
    <DD>Returns the kind or URI for its argument as a three-character string:
        <UL>
        <LI>abs: Absolute URI, i.e. relative to your project root (starts in ':')
        <LI>ext: External URI, not on your server.
        <LI>rel: Relative URI
        <LI>rsv: Server-relative URI
        </UL>
Unfortunately, these are sort of <qq>internal</qq> terms that are not quite
compatible with what the docs on <A
HREF=":features/uris.html#prjrel">project-relative URIs</A> say, but you can
easily find out of what kind an URI is using the expression <CODE><tg>(urikind (myuri))</tg></CODE>.
        
</DL>

<H3>Binary Operators</H3>
<H4>Integer arithmetics</H4>
These operators are fairly simple. Division by zero is caught, but no
overflows or stuff like that. But as long as you don't do scientific
maths in your HTML macros, you should be fine.<BR>
Empty strings count as <qqc>0</qqc>, any text not
consisting only of digits counts as <qqc>1</qqc> (no warnings). This
seems to make sense for auto-casting <CODE>BOOL</CODE>.
<DL>
    <DT><I>expression</I> <STRONG>&amp;</STRONG> <I>expression</I>
    <DD>Addition. Weird operator, but using '+' for string concatenation
        seems to make sense as it is the far more frequent operation.

    <DT><I>expression</I> <STRONG>-</STRONG> <I>expression</I>
    <DD>Subtraction. As <qqc>-</qqc> is also a legal character in attribute names,
        you must enclose it in whitespace, lest it be interpreted as part
        of the next or previous word.

    <DT><I>expression</I> <STRONG>*</STRONG> <I>expression</I>
    <DD>Multiplication. Straightforward...

    <DT><I>expression</I> <STRONG>/</STRONG> <I>expression</I>
    <DD>Division.

    <DT><I>expression</I> <STRONG>MOD</STRONG> <I>expression</I>
    <DD>Modulo (the remainder of an integer division).

    <DT><I>expression</I> <STRONG>&lt;</STRONG> <I>expression</I>
    <DD>Less than.

    <DT><I>expression</I> <STRONG>&gt;</STRONG> <I>expression</I>
    <DD>Greater than.

    <DT><I>expression</I> <STRONG>&lt;=</STRONG> <I>expression</I>
    <DD>Less than or equal.

    <DT><I>expression</I> <STRONG>&gt;=</STRONG> <I>expression</I>
    <DD>Greateer than or equal.
</DL>
<P>The latter four operators were present in HSC V0.917 already, but their
semantics have changed (that's why they were undocumented in the first place)!
They used to compare <EM>strings</EM>, now they compare <EM>numbers</EM> -- but
they still have a string equivalent each, see below.</P>

<a name="stringop"></a><H4>String operators</H4>
<DL>
    <DT><I>expression</I> <STRONG>=</STRONG> <I>expression</I>
    <DD>string comparison (case insensitive)

    <DT><I>expression</I> <STRONG>==</STRONG> <I>expression</I>
    <DD>string comparison (case sensitive)

    <DT><I>expression</I> <STRONG>+</STRONG> <I>expression</I>
    <DD>string concatenation

    <DT><I>little</I> <STRONG>IN</STRONG> <I>big</I>
    <DD>search for substring <I>little</I> in <I>big</I> (case insensitive)

    <DT><I>string</I> <STRONG>MATCH</STRONG> <I>pattern</I>
    <DD>Matches <I>string</I> against the regular expression <I>pattern</I>. RE
syntax follows that of <kbd>egrep</kbd> or VIM's <qq>very magic</qq> mode, i.e. metacharacters
(<kbd>(|),[],{},.,+,*,?</kbd>) are active by default and have to be
backslash-escaped to be interpreted as literals.

    <DT><I>string</I> <STRONG>MATCHI</STRONG> <I>pattern</I>
    <DD>Like <STRONG>MATCH</STRONG>, but case-insensitive.

    <DT><I>expression</I> <STRONG>LT</STRONG> <I>expression</I>
    <DD>Less than.

    <DT><I>expression</I> <STRONG>GT</STRONG> <I>expression</I>
    <DD>Greater than.

    <DT><I>expression</I> <STRONG>LE</STRONG> <I>expression</I>
    <DD>Less than or equal.

    <DT><I>expression</I> <STRONG>GE</STRONG> <I>expression</I>
    <DD>Greater than or equal.
</DL>
<P>These comparison operators, added in V0.920 to do the job of what are now 
integer operators, all work case-insensitively. Maybe that's not such
a smart choice after all and they should be case-sensitive while you get a
unary <qqc>upstr</qqc> operator to upcase strings yourself. Then again, who
needs case-sensitive comparisons in HTML? Perhaps I'll change
them, so don't complain later ;)</P>

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
<TG>IMG SRC="hugo.png" ALT="hugo was here"</TG>
This is hugo!
Sepp is among them.
AmigaOS version: 40.70 <* 2b compiled on Linux... *>
</PRE>

<P>At least on my machine.</P>

<H2><A NAME="boolean">Boolean Expressions</A></H2>

<P>If you pass an expression to a boolean attribute, the expression is
evaluated as before. If the expression returned an empty string,
the boolean attribute is interpreted as <CODE>false</CODE>. This will
cause the attribute to be removed from the tag/macro-call.</P>

<P>Any none-empty string will set the attribute to <CODE>true</CODE>,
resulting in a value equal to the name of attribute. (In HTML, writing 
<CODE>ISMAP</CODE> is short for <CODE>ISMAP="ISMAP"</CODE>, while in
XHTML the latter has to be, and automatically is, spelled out.)</P>

<STRONG>Example:</STRONG>

<$source PRE><IMG SRC=(name) ALT="nufin" ISMAP=(name="map.gif")></$source>

<P>will be converted to</P>

<$source PRE><IMG SRC="hugo.png" ALT="nufin"></$source>

<P>if <CODE>name</CODE> has been set to <CODE>"hugo.gif"</CODE>,
or to</P>

<$source PRE><IMG SRC="map.png" ALT="nufin" ISMAP></$source>

<P>if <CODE>name</CODE> has been set to <CODE>"map.gif"</CODE>. Note that
only the second call enables the boolean attribute <CODE>ISMAP</CODE>,
while it gets stripped for the first call.</P>

<P>The following operators yield a boolean (<CODE>TRUE</CODE> or
<CODE>FALSE</CODE>) result and/or can be used to manipulate boolean
values.</P>
<DL>
    <DT><I>expression1</I> <STRONG>AND</STRONG> <I>expression2</I>
    <DD><CODE>TRUE</CODE> if both <I>expression1</I> and <I>expression2</I>
        are <CODE>TRUE</CODE>.

    <DT><I>expression1</I> <STRONG>OR</STRONG> <I>expression2</I>
    <DD><CODE>TRUE</CODE> if <I>expression1</I> or <I>expression2</I>
        or both are <CODE>TRUE</CODE>.

    <DT><I>expression1</I> <STRONG>XOR</STRONG> <I>expression2</I>
    <DD><CODE>TRUE</CODE> if either <I>expression1</I> or
        <I>expression2</I>, but not both, are <CODE>TRUE</CODE>.

</DL>
<H2><A NAME="priorities">Priorities</A></H2>
<P><STRONG>Important:</STRONG> Different to most programming languages, <hsc>
does not support priorities for different operators. Therefor, expressions
are simply processed sequentially (Programmer's lazyness rules).</P>

<P>But you can use nested brackets within complex expressions.</P>

<H2><A NAME="symref">Symbolic References</A></H2>

<p>Starting with V0.926, HSC can also handle a special kind of expressions called
<em>symbolic references</em> (in PERL-speak), which are a bit of an exception to
the above syntax. A symbolic reference is an <em>expression</em> whose
<em>value</em> will be taken as the <em>name of a variable</em>. The following
macro employs this mechanism:
<$source pre>
<$macro NAMES SELECT:num>
  <$define NAME1:string="Hugo">
  <$define NAME2:string="Resi">
  This is <( {"NAME" + SELECT} )>
</$macro>
</$source>
<p><code><tg>NAMES SELECT=1</tg></code> yields ``This is Hugo'', while
<code><tg>NAMES SELECT=2</tg></code> yields ``This is Resi''.</p>

<p>A symbolic reference is an expression enclosed in ``curly braces''
(``<code>{...}</code>''). HSC takes its value as the name of an attribute, and
the value of this attribute is the value of the entire expression. The above
just concatenates the string <code>"NAME"</code> and the ``selector'' number and
returns the value of the appropriate variable. This may not seem very useful,
but for things like emulating arrays it can come in quite handy.</p>

<p>One major difference between a symbolic reference and a normal expression is
that the former may be used on the left hand side of an expression, it yields a
so-called <em>lvalue</em>. So you can not only <em>read</em> an attribute whose
name is determined at compilation time, but also <em>write</em> to it:</p>
<$source pre>
<$let {"foo" + "bar"}='1'>
</$source>
<p>is the same as:</p>
<$source pre>
<$let foobar='1'>
</$source>
</DOCPAGE>

<* vi: set ts=4 expandtab: *>
