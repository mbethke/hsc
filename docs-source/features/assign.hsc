<WEBPAGE chapter="hsc - " title="Attribute Assignments"
    PREV="syntax.html"
    NEXT="expressions.html">

When assigning new values to attributes, you can use string constants, like in html.
Furthermore, you can also use expression to compute a new value depending on certain
things. And there you can make the assignment of a value depend on the value of
another attribute.

<H2>String Constants</H2>

As usual with html, you can use an assignment like

<$source pre><IMG SRC="hugo.gif" ALT='hugo'></$source>

to set the attribute <CODE>SRC</CODE> to <qqc>hugo.gif</qqc> and
<CODE>ALT</CODE> to <qqc>hugo</qqc>. The first assignment uses double
quotes to denote the string boundaries, the second one uses single
quotes. There is no difference in the functionality between these to
kinds of quotes.

<P>Again, like in html, one kind of quotes might show up inside a string,
as long as it is quoted with other kind, for example:</P>

<$source pre><IMG SRC="quote.gif" ALT='"'></$source>

<P>Now <CODE>ALT</CODE> contains a double quote as value.</P>

<H2>Constants Without Quotes</H2>

If only certain characters are used, you even can omit the quotes at
all. For example,

<$source pre><IMG SRC=hugo.gif ALT=hugo></$source>

is also legal. As this can cause problems with (very) old browsers, it
might result in <ln-msg id="22">, if <hsc> is configured to. However,
the following is not allowed according to the specifications of html:

<$source pre><IMG SRC=image/hugo.gif ALT=hugo></$source>

<P>Because of the <slash> in the value of <CODE>SRC</CODE>, it would
have been required to put it inside quotes. As this is not the case,
<ln-msg id="81"> will show up. Although most browsers can cope with
this, and will use a white space or a <greater-than> as delimiter,
this behavior is not standard.</P>

<H2><A NAME="expressions">Compute Expressions</A></H2>

<P>When the assigned value starts with a <bracket>, it denotes an
expression to be computed before its result is used as new value.
There are several operators you can use for that, and of course you
also can refer to other attributes.</P>

A very basic example would be

<$source PRE><IMG SRC=(name+".gif") ALT=(name)></$source>

If the attribute <CODE>name</CODE> has been set to <qqc>sepp</qqc> before,
this will result in

<$source PRE><IMG SRC="sepp.gif" ALT="sepp"></$source>

<P>The <qqc>+</qqc> is used to concatenate two values together.</P>

For more details on this, see the chapter about <ln-expression PLURAL>.

<H2>Using Quotes Inside Quoted Strings</H2>

<p>There are two kind of quotes you can use for string constants,
namely single quotes (') and double quotes ("). You can use the one
inside of the other just as a normal character:</p>

<$source pre>
<IMG SRC="single.gif" ALT="a single (') quote">
<IMG SRC='double.gif' ALT='a double (") quote'>
</$source>

<p>Note that this is no "escape mechanism", the other quote just is
treated as a normal character. For the normal user, this should be all
you want to know about "quotes inside quotes". Freaky ones, read
on.</p>

<p>Because of the limitations of html it is not possible to use both
kind of quotes in one value. However, in hsc you could use string
concatenation to push both quotes into one value:</p>

<$source pre><$define sepp:string=("a single (') quote"+' and a double quote (") together')></$source>

<p>The value of sepp then would be &gt;&gt;<code>a single (') quote
and a double quote (") together</code>&lt;&lt;. Different to html, hsc
doesn't mind about that internally. The trouble starts when you assign
the value later on to an attribute of a html tag:</p>

<$source pre><IMG SRC="sepp.gif" ALT=(sepp)></$source>

<p>Now hsc can't use neither single nor double quotes to assign the value
of <code>sepp</code> to <code>ALT</code>. To solve that problem, it
uses double quotes to embed the value in and replaces all double quotes inside
by its entity <code>&amp;quot;</code>, thus resulting in:</p>

<$source pre><IMG SRC="sepp.gif" ALT="a single (') quote and a double quote (&quot;) together"></$source>

<p>Note that several browsers are too stupid to display this
correctly.</p>

<H2><A NAME="cond-assign">Conditional Assignments</a></H2>

<P>This paragraph deals with a feature you probably do not want to use
(and understand) before you understood how those macros work. You can
skip this part for now and return later.</P>

You can easily let an attribute obtain it's value from another
attribute. For example, within a tag call you can use an assignment
like

<PRE>sepp=(hugo)</PRE>

However, if <CODE>hugo</CODE> has not been defined and assigned a
value before, this will result in an error message. Conditional
assignments now only assign a value to the target attribute, if the
source attribute has been set; in any case, the source attribute must
have been defined before (using <TG>$macro</TG> or <TG>$define</TG>).
Simply use a <qqc>?=</qqc> instead of the <qqc>=</qqc> assignment
operator:

<PRE>sepp?=hugo</PRE>

This becomes handy for such macros which are more or less only
extensions of real html-tags:

<$source PRE>
<$macro MY-BODY BackGround:uri>
    <BODY BackGround?=BackGround">
</$macro>
</$source>

<P>The macro <TG>MY-BODY</TG> just inserts a <TG>BODY</TG> tag. But
optionally it can also handle the attribute
<CODE>BackGround</CODE>.</P>

<P>But there has not necessarily a <CODE>BackGround</CODE> attribute to
be set when calling <TG>MY-BODY</TG>. If you do not specify any,
there also will not be one in the call to <TG>BODY</TG> after
the macro has been processed.</P>

Two examples should point out this behavior:

<$source PRE><MY-BODY></$source>

will result in

<$source PRE><BODY></$source>

but a

<$source PRE><MY-BODY BackGround='image/backgr.png'></$source>

will lead to

<$source PRE><BODY BackGround="image/backgr.png"></$source>

thus in the second case also adding the attribute
<CODE>BackGround</CODE> to the <TG>BODY</TG>-tag.

<P>If <TG>MY-BODY</TG> would have been declared without conditional
assignment, it could have looked something like:

<$source PRE>
<$macro MY-BODY BackGround:uri>
    <BODY BackGround=(BackGround)>
</$macro>
</$source>

If you would try to call it without a <CODE>BackGround</CODE> passed,
this attribute would have been unset, and the attempt to copy the
value of <CODE>BackGround/MY-BODY</CODE> to
<CODE>BackGround/BODY</CODE> using
<CODE>BackGround=(BackGround)</CODE> would result in <ln-msg id="23">

<H2>Complex Conditions</H2>

<P>On the first sight, it might seem that there is only the simple
condition <qq>if attribute is set..</qq> possible. But no one
prevents you from using code like this:

<$source PRE>
<$define TEXT:color>
<$if COND=(awfully complex condition))>
    <$let TEXT='#123456'>
</$if>

<MY-BODY TEXT?=TEXT>
</$source>

This also works for <ln-let>:

<$source PRE>
<$define sepp:string>
<$define hugo:string>

<$if COND=(awfully complex condition)>
    <$let hugo="hugo-value">
</$if>

<$let sepp?=hugo>
</$source>

and you can also use <ln-expression plural> to compute the source
attribute. For instance, the last line of the above example also could
have been

<$source PRE><$let sepp?=("hu"+"go")></$source>

</WEBPAGE>
