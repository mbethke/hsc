<$INCLUDE FILE="inc/macro.hsc">
<WEBPAGE chapter="hsc - Features - " title="Macros">

Okay, here are some examples of macro definitions:

<$MACRO EXMAC>
<BLOCKQUOTE><TT>&lt;$MACRO
</$MACRO>
<$MACRO /EXMAC>
&lt;/$MACRO&gt;</TT></BLOCKQUOTE>
</$MACRO>

<H2>Use as a shortcut</H2>

You can define a macro called <TT>MY_ADDRESS</TT> that only is a
shortcut for your email-address like
<EXMAC>MY_ADDRESS&gt;hugo@some.where</EXMAC>
So every time, you insert the macro-tag
<BLOCKQUOTE>
<TG>MY_ADDRESS</TG>
</BLOCKQUOTE>
in your HSC-source, it will be replaced with the email address
seen above in the HTML-file.

<H2>Create your own logical style</H2>

One of the most embarassing facts within HTML are physical/logical
styles. Taking a look at the HTML 3.0 proposed things, you find
loads of new styles: <TG>AU</TG> for authors, <TG>SAMP</TG> for
    sequences of litteral characters
and so on... No one knows where this will end, and I'm just waiting
for physical styles like
<TG>TIM_BERNERS_LEE_S_LOGICAL_STYLE_TO_RENDER_NAMES_OF_HIS_FRIENDS</TG>.<P>

Anyway, while other people think about which logical style should get his
own tag and which not, you can define your own styles. For example, a
logical style to render a filename could look like this:

<EXMAC>FNM&gt; &lt;I&gt; </EXMAC>
<EXMAC>/FNM&gt; &lt;/I&gt; </EXMAC>

Your new style can be used like all other styles:
<BLOCKQUOTE><TT>
..open the file <TG>FNM</TG>hugo.txt<TG>/FNM</TG> and..
</TT></BLOCKQUOTE>

In this case, filenames will be rendered <I>italic</I>. This seems much
more reasonable to me then producing heaps of logical styles, but..<P>

Note that you just created a container that requires an openig and
closing tag.

<H2>Macros with custom attributes</H2>

Now the most subtile part starts: you can add attributes to macros.
Attributes are compareable to function arguments in programming languages
like Pascal, Oberon or E.<P>

An attribute is defined with
<BLOCKQUOTE>
<I>name</I> ":" <I>type</I> [ "=" <I>default value</I> ]
</BLOCKQUOTE>

Legal attribute types are:
<UL>
<LI>STRING: any text
<LI>URI: references to an URI. Is affected bye command line options like
    <A HREF="options.html">AbsUri</A> or <A HREF="options.html">CheckUri</A>.
<LI>BOOL: boolean flag
<LI>ENUM: enumerator type; accepts only given values (eg the <TT>ALIGN</TT>
    attribute of <TG>IMG</TG>).
</UL>

So here's an example to understand that rather theoretical excess you just read
and didn't understand at all:

<EXMAC>BUTTON_NEXT NXTREF:uri&gt;<BR>
&lt;A HREF=&lt;NXTREF&gt;&gt;<BR>
&lt;IMG SRC="image/next.gif" ALT="Next"&gt;<BR>
&lt;/A&gt;<BR>
</EXMAC>

This defines a macro that defines a button that references to the next page.
As every page has its own next page, you can set one attribute for this macro:
<TT>NXTREF</TT>, which is the URI that should be referenced as the "next"
page.<P>

So an example usage of this macro would be:
<BLOCKQUOTE><TT>
<TG>BUTTON_NEXT NXTREF="sepp.html"</TG>
</TT></BLOCKQUOTE>

Note that the value of <TT>NXTREF</TT> is passed to the <TT>HREF</TT> attribute
within the <TG>A</TG> tag when the macro is extracted.<P>

If an attribute should get its value from an other attribute, the source
attributes name needs to be enclosed in "<TT>&lt;..&gt;</TT>", like
<TT>HREF=&lt;NXTREF&gt;</TT> (see above), which assigns the value of the macro
attribute <TT>NXTREF</TT> to the tag attribute <TT>HREF</TT>.<P>

<H2>Some Notes</H2>

Names for macros and attributes are case-insensitive.<P>

Macro attributes can only be defined within opening macros. The
coresponding closing macro automatically inherits all attributes of
its opening macro. This seems more HTML-like to me.<P>

This is not all that can be told about <hsc>'s macros. Macros also
support default values and several flags. Yeah, I really should
improve the docs..

If you want
to take a look at some quite complex macros, take a look at the file
<FILE>
<* Note: this creates a link with absolute uri not converted *>
<|<A HREF="../../src_docs/inc/macro.hsc">src_docs/inc/macro.hsc</A>|>
</FILE> in this distribution.<P>

<MAIN>
<PREV href="features/absuris.html">
<NEXT href="features/rplcent.html">
</WEBPAGE>

