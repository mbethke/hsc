<WEBPAGE chapter="hsc - Features - Macros - " title="Attribute declaration">

<$macro LITYPE TYPE:string/r>
<DT><CODE><(type)></CODE>
<DD>
</$macro>

<$macro LIATTR SHORT:string/r LONG:string/r>
<DT><CODE>/<(long)></CODE>
    (short: <CODE>/<(short)></CODE>)
<DD>
</$macro>

<*
** macro to format an example
*>
<$macro exmpl exmpl:string/r>
<DT><CODE><(exmpl)></CODE>
<DD>
</$macro>

<P>Attributes are compareable to function arguments in programming languages
like Pascal, Oberon or E. So attributes are arguments passed to a tag or
macro.</P>

<P>Additionally, you can create/update attributes using <ln_define>
and <ln_let> therefor using them like variables.</P>

<H2>General syntax</H2>

<P>Within <ln_macro>, <ln_define> and <ln_deftag>,
an  attribute is declared using
<BLOCKQUOTE>
<I>name</I> ":" <I>type</I>  [ "/" <I>modifiers</I> ]  [ "=" <I>default value</I> ]
</BLOCKQUOTE>
</P>

<A NAME="type"><H2>Attribute types</H2></A>
<DL>
<LITYPE type="STRING"> any text
<LITYPE type="URI"> references to an URI. Is affected by CLI
    options like <op_stripexternal>.
<LITYPE type="BOOL"> boolean flag
<LITYPE type="ENUM"> enumerator type; accepts only given values (eg the
    <CODE>ALIGN</CODE>
    attribute of <TG>IMG</TG>).
<LITYPE type="ID"> id for current position in document; can be refered
  with <TG>A HREF="#id"</TG>
<LITYPE type="NUM"> decimal integer value
<LITYPE type="COLOR"> a color-value matching the template <CODE>"#rrggbb"</CODE>
 or one of the values defined using 
<A HREF=":features/spcattr.html#colornames"><CODE>HSC.COLOR-NAMES</CODE></A>.
</DL>

<A NAME="modifier"><H2>Attribute modifiers</H2></A>
<DL>
<LIATTR LONG="CONST" SHORT="C">
    Attribute is read-only; you can't set a new value using <ln_let>.
    This option is only allowed within <ln_define>
<LIATTR LONG="GLOBAL" SHORT="G">
    Attribute is global. Useful if you want to define an attribute inside
    a macro, that is not removed after macro has been processed.
    This option is only allowed within <ln_define>.
<LIATTR long="JERK" short="J">
    Attribute is only used by a <jerk>.
<LIATTR long="REQUIRED" short="R">
    Attribute must be set when macro/tag is called.
</DL>

Within <ln_macro> and <ln_deftag>, URIs also can have the following options:

<DL>
<LIATTR long="SIZE" short="Z">
    If the attribute references to a local file, try to evaluate
    the "size" of the file, search for attributes called
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> within the same tag
    and set them with these values (if they have not been set before).
    At they moment, only tags like <TG>IMG</TG> support this.
<LIATTR long="STRIPEXT" short="X">
    Strip whole tag, if the attribute references to an external URI
    and option <op_stripexternal> has been enabled.
</DL>

<H2>Default value</H2>
<P>The default value initialises the attribute every time the
macro is called. If the attribute is not set within the macro call, the
default value is used. Otherwise, the value passed to the macro
will be used as new value.</P>

<H2>Examples</H2>
<DL>
<EXMPL exmpl="DO_IT:bool">
    defines a boolean attribute
<EXMPL exmpl='UP:uri=":main.html"'>
    defines an URI attribute and sets it's default value to "<CODE>:main.html</CODE>".
    The ":" indicates that the default value is an <fe_absuri>.
<EXMPL exmpl='MODE:enum("ascii|bin|hex")'>
    defines an enumerator type; legal values are <CODE>ascii</CODE>,
    <CODE>bin</CODE> and <CODE>hex</CODE>.
</DL>

</WEBPAGE>
