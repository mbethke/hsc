<WEBPAGE chapter="hsc - Features - " title="Macros"
    PREV=":features/absuris.html"
    NEXT=":features/rplcent.html">

A macro can be defines with

<BLOCKQUOTE><CODE>&lt;$MACRO</CODE> <I>MacroName</I>
  [ <I><A HREF="flag.html">flags</A></I> ]
  [ <I><A HREF="attrib.html">attributes</A></I> ]
  <CODE>&gt;</CODE><BR>

<BR>
<I>...macro text...</I><BR><BR>

<TG>/$MACRO</TG></BLOCKQUOTE>

Okay, here are some examples of macro definitions:


<$MACRO EXMAC>
<BLOCKQUOTE><TT>&lt;$MACRO
</$MACRO>
<$MACRO /EXMAC>
&lt;/$MACRO&gt;</TT></BLOCKQUOTE>
</$MACRO>

<H2>Use as a shortcut</H2>

You can define a macro called <CODE>MyAddress</CODE> that only is a
shortcut for your email-address like

<$INCLUDE FILE="exmpl/m_addr.inc" SOURCE PRE> <* show macro *>
<$INCLUDE FILE="exmpl/m_addr.inc">            <* define macro *>

So every time, you insert the macro-tag <TG>MyAddress</TG>
in your HSC-source, it will be replaced by

<PRE>
<MyAddress>
</PRE>

in the HTML-file.

<H2>Create your own logical style</H2>

One of the most embarassing facts within HTML are physical/logical
styles. Taking a look at the HTML 3.0 proposed things, you find
loads of new styles: <TG>AU</TG> for authors, <TG>SAMP</TG> for
    sequences of litteral characters
and so on... No one knows where this will end, and I'm just waiting
for logical styles like
<TG>TIM_BERNERS_LEE_S_LOGICAL_STYLE_TO_RENDER_THE_NAME_OF_HIS_DOG</TG>.
(Which would asume that TBL has a dog)<P>

Anyway, while other people think about which logical style should get his
own tag and which not, you can define your own styles. For example, a
logical style to render a filename could look like this:

<$INCLUDE FILE="exmpl/m_file.inc" SOURCE PRE> <* show macro *>
<* <FILE> is already defined within "inc/macro.hsc",
** so there is no need to include redefine it here
*>

Your new style can be used like all other styles:
<PRE>
  ..open the file <TG>FILE</TG>hugo.txt<TG>/FILE</TG> and..
</PRE>

In this case, filenames will be rendered <I>italic</I>:

<BLOCKQUOTE>
  ..open the file <FILE>hugo.txt</FILE> and..
</BLOCKQUOTE>

This seems much more reasonable to me then producing heaps of logical styles,
but..<P>

Note that you just created a container that requires an openig and
closing tag.

<H2>Macros with custom attributes</H2>

Take a look at this example:

<$INCLUDE FILE="exmpl/m_next.inc" SOURCE PRE> <* show macro *>
<$INCLUDE FILE="exmpl/m_next.inc">            <* define macro *>

This defines a macro that defines a button that references to the next page.
As every page has its own next page, you can set one attribute for this macro:
<CODE>NXTREF</CODE>, which is the URI that should be referenced as the "next"
page.<P>

So an example usage of this macro would be:
<PRE>
  <TG>BUTTON_NEXT NXTREF=":features/rplcent.html"</TG>
</PRE>

which will give the button seen below:<P>

<BUTTON_NEXT NXTREF=":features/rplcent.html"><P>

Note that the value of <CODE>NXTREF</CODE> is passed to the <CODE>HREF</CODE>
attribute within the <TG>A</TG> tag when the macro is extracted.<P>

If an attribute should get its value from an other attribute, the source
attributes name needs to be enclosed in "<CODE>&lt;..&gt;</CODE>", like
<CODE>HREF=&lt;NXTREF&gt;</CODE> (see above), which assigns the value of the
macro attribute <CODE>NXTREF</CODE> to the tag attribute <CODE>HREF</CODE>.<P>

<H2>Some Notes</H2>

Names for macros and attributes are case-insensitive.<P>

Macro attributes can only be defined within opening macros. The
coresponding closing macro automatically inherits all attributes of
its opening macro. This seems more HTML-like to me.<P>

To make your macros more powerful and flexible, you can use
<A HREF=":features/if.html">conditional conversion</A>.
<P>

</WEBPAGE>

