<WEBPAGE chapter="hsc - Features - Macros - " title="Macro/Tag attributes">

<$macro LITYPE TYPE:string/r/q>
<DT><STRONG><$insert text=<type>></STRONG>
<DD>
</$macro>

<$macro LIATTR SHORT:string/r LONG:string/r>
<DT><STRONG><$insert text=<long>></STRONG>
    (short: <STRONG><$insert text=<short>></STRONG>)
<DD>
</$macro>

<*
** macro to format an example
*>
<$MACRO exmpl exmpl:string/r>
<DT><CODE><$INSERT TEXT=<exmpl>></CODE>
<DD>
</$MACRO>

Attributes are compareable to function arguments in programming languages
like Pascal, Oberon or E. So attributes are arguments passed to a tag or
macro.<P>

Within <A HREF=":macro/macros.html"><TG>$MACRO</TG></A>,
<A HREF=":features/spctags.html#let"><TG>$LET</TG></A> and
<A HREF=":features/spctags.html#deftag"><TG>$DEFTAG</TG></A>,
an  attribute is defined with
<BLOCKQUOTE>
<I>name</I> ":" <I>type</I> [ "=" <I>default value</I> ] [ "/" <I>flag</I> ]
</BLOCKQUOTE>

Legal <STRONG>attribute types</STRONG> are:
<DL>
<LITYPE type=STRING> any text
<LITYPE type=URI> references to an URI. Is affected bye command line options like
    <A HREF=":options.html#CheckUri">CheckUri</A>.
<LITYPE type=BOOL> boolean flag
<LITYPE type=ENUM> enumerator type; accepts only given values (eg the <TT>ALIGN</TT>
    attribute of <TG>IMG</TG>).
</DL>

<STRONG>Default value</STRONG> initialises the attribute every time the
macro is called. If the attribute is not set within the macro call, the
default value stays active. Otherwise, the value passed to the macro
will be used as new value.<P>

Legal <STRONG>flag</STRONG>s are:
<DL>
<LIATTR long="JERK" short="J">
    attribute is only used by jerks
<LIATTR long="REQUIRED" short="R">
    attribute must be set when macro/tag is called
<LIATTR long="NOQUOTE" short="Q">
    value doesn't require quotes; mostly used for jerk attributes. To
    globaly disable warnings about missing quotes, use
    <A HREF=":options.html#Ignore"><CODE>IGNORE</CODE></A> when envoking
    <hsc>.
</DL>


<H2>Examples</H2>
<EXMPL exmpl="DO_IT:bool">
    defines a boolean attribute
<EXMPL exmpl='UP:uri=":main.html"'>
    defines an URI attribute and sets it's default value to "<CODE>:main.html</CODE>".
    The ":" indicates that the default value is an <fe_absuri>.
<EXMPL exmpl="MODE:enum(ascii|bin|hex)">
    defines an enumerator type; legal values are <CODE>ascii</CODE>,
    <CODE>bin</CODE> and <CODE>hex</CODE>.

<P>

</WEBPAGE>


