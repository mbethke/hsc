<WEBPAGE chapter="hsc - Features - " title="Macros"
    PREV=":features/absuris.html"
    NEXT=":features/rplcent.html">

Macros are a powerful feature which enables you to create shortcuts
for often used text. As you can also pass arguments to macros, it is
even possible to create templates for whole pages.

<**********************************************************>
<H2>General Syntax</H2>
<**********************************************************>

Macros can be defined like

<BLOCKQUOTE><CODE>&lt;$macro</CODE> <I>MacroName</I>
  [ <I><A HREF="flag.html">modifiers</A></I> ]
  [ <I><A HREF="attrib.html">attributes</A></I> ]
  <CODE>&gt;</CODE><BR>
<BR>
<I>...macro text...</I><BR><BR>
<TG>/$macro</TG></BLOCKQUOTE>

Here are some examples of macro definitions:

<**********************************************************>
<H2>Use as Shortcuts</H2>
<**********************************************************>

You can define a macro called <CODE>Hugo-Address</CODE> that only is a
shortcut for your email-address like

<$INCLUDE FILE="exmpl/macro_addr.hsc" SOURCE PRE> <* show macro *>
<$INCLUDE FILE="exmpl/macro_addr.hsc">            <* define macro *>

So every time, you insert the macro-tag <TG>Hugo-Address</TG> in your
hsc-source, it will be replaced by

<PRE>
<Hugo-Address>
</PRE>

in the html-object.

<**********************************************************>
<H2><A NAME="container">Container Macros</A></H2>
<**********************************************************>

<P>Container macros allow you to create a macros with a text before
and after the content. While the preceding/succeeding text is
specified with the macro declaration, the content is assigned during
the macro call.</P>

<P>To declare a macro as container, you have to specify the modifier
<qqc>/CLOSE</qqc>. To insert the contents, use the special tag
<TG>$content</TG>.</P>

For example, one could declare his own <qq>logical</qq> style. Below a
macro <TG>FILE</TG> will be created, which can be used to render
filenames:


<$INCLUDE FILE="exmpl/macro_file.hsc" SOURCE PRE> <* show macro *>
<* <FILE> is already defined within "inc/macro.hsc",
** so there is no need to include redefine it here
*>

Your new style can be used like all other styles:
<$source PRE>
  ..open the file <FILE>hugo.txt</FILE> and..
</$source>

In this case, filenames will be rendered <I>italic</I>:

<BLOCKQUOTE>
  ..open the file <FILE>hugo.txt</FILE> and..
</BLOCKQUOTE>

<P>It should be rather obvious how this one works: When calling the
macro using <TG>FILE</TG>hugo.txt<TG>/FILE</TG>, hsc will scan the
input until it reaches a <TG>/FILE</TG>. Anything between the
corresponding start and end tag will be used as content, in this case
<qq>hugo.txt</qq>.</P>

<P>Now the macro text will be interpreted: The first part is easy, a
simple <TG>I</TG> will be inserted. After that, a <TG>$content</TG>
shows up, and the content read before will be inserted. In this case,
this is a simple plain text, but of course you could also use tags or
even other (container) macros. At the end of the macro, a closing
<TG>/I</TG> is appended, and the macro call exits.</P>

<P>You can use <TG>$content</TG> multiple times inside the same macro,
too.</P>


<**********************************************************>
<H2>Macros with Attributes</H2>
<**********************************************************>

Take a look at this example:

<$INCLUDE FILE="exmpl/macro_next.hsc" SOURCE PRE> <* show macro *>
<$INCLUDE FILE="exmpl/macro_next.hsc">            <* define macro *>

This defines a macro that defines a button that references to the next
page. As every page has its own next page, you can set one attribute
for this macro: <CODE>NXTREF</CODE>, which is the URI that should be
referenced as the "next" page.<P>

So an example usage of this macro would be:
<$source PRE>
  <Button.Next NXTREF=":features/rplcent.html">
</$source>

which will give the button seen below:<P>

<Button.Next NXTREF=":features/rplcent.html"><P>

Note that the value of <CODE>NXTREF</CODE> is passed to the
<CODE>HREF</CODE> attribute within the <TG>A</TG> tag when the macro
is extracted.<P>

If you wonder, what <CODE>HREF=(NxtRef)</CODE> (see above) should
mean: This sets <CODE>HREF</CODE> with the value stored in
<CODE>NxtRef</CODE>. For details, read the section about <A
HREF=":features/expressions.html">expressions</A>.

<**********************************************************>
<H2>Nesting Container Macros</H2>
<**********************************************************>

<P>You should be aware that the macro content (the text specified
between the corresponding start and end macro tag) can not access
attributes which have been declared for the macro text (the text which
is assigned to the macro while declaring it using
<TG>$macro</TG>).</P>

For example:

<$include FILE="exmpl/macro_cntnest.hsc" SOURCE PRE>

will result in

<$include FILE="exmpl/macro_cntnest.hsc" PRE>

The line
<$source PRE>content: hugo=<(hugo)></$source>

<P>does not - as some might have expected - access the attribute
<CODE>hugo</CODE> passed to the container macro <TG>sepp</TG> just
before, but still reads the attribute <CODE>hugo</CODE> declared above
using <TG>$define</TG>.</P>

The reason for this shows up soon when you start using container
macros inside container macros: every time a <TG>$content</TG> shows
up while just processing a <TG>$content</TG>, it does not make much
sense to include the same content as just before. Instead, the parser
uses the content passed to the previous but one container macro.

For example:

<$include FILE="exmpl/macro_cnt2nest.hsc" SOURCE PRE>

will result in

<$include FILE="exmpl/macro_cnt2nest.hsc" PRE>


<**********************************************************>
<H2>Macros inside Macros</H2>
<**********************************************************>

Currently, locale macros are not supported. If you declare a macro
inside a macro, the inner macro will be declared when the outer macro
is called. For example:

<$include FILE="exmpl/macro_locale.hsc" SOURCE PRE>

will result in
<$include FILE="exmpl/macro_locale.hsc" PRE>

<P>At the moment you can call <TG>inner-sepp</TG> even outside of
<TG>outer-sepp</TG>, as it is defined globally. When calling
<TG>outer-sepp</TG> another time, you will receive a <ln_msg id="59">,
as this tries to redefine <TG>inner-sepp</TG>.</P>

Obviously this behaviour doesn't make sense, but local macros are not
supported in this release.



<*
<H2>See also</H2>

To make your macros more powerful and flexible, you can use
<A HREF=":features/expressions.html">expressions</A> and
<A HREF=":features/if.html">conditional conversion</A>.
<P>
*>

</WEBPAGE>
