<WEBPAGE chapter="hsc - Features - " title="Conditional conversion"
    PREV="macros.html"
    NEXT="macros.html">

<H2>Syntax</H2>

Conditional conversion looks like that:

<PRE>
<CODE>&lt;$IF</CODE> <I>expression</I><CODE>&gt;</CODE>

  <I>...if part...</I>

[
<CODE>&lt;$ELSE&gt;</CODE>

  <I>...optional else part...</I>

]

<CODE>&lt;/$IF&gt;</CODE>
</PRE>

<H2>Expressions</H2>

Possible expressions are:

<DL>
<DT><STRONG>NOT</STRONG> <I>expression</I>
    <DD>becomes true, if <I>expression</I> is false
<DT><STRONG>SET</STRONG> "<CODE>&lt;</CODE>" <I>attribute</I> "<CODE>&gt;</CODE>"
    <DD>becomes true, if a value for <I>attribute</I> was set (passed to a macro)
<DT>"<STRONG>&lt;</STRONG>" <STRONG><I>bool. attribute</I></STRONG> "<STRONG>&gt;</STRONG>"
    <DD>becomes value of boolean attribute
</DL>

<H2>Example</H2>

A good example for the usage of conditional conversion inside a macro is
the <TG>WEBPAGE</TG> macro used to create the documentation for <hsc>.<P>

It also shows how to create a navigation bar with optional references to
a next and previous page.

<PRE>
<$include file="inc/webpage.inc" SOURCE>
</PRE>

<H2>Notes</H2>

<TG>$IF ..</TG> is nestable.<P>

You can't compare <hsc>'s <TG>$IF ..</TG> to the C-preprocessor's
<CODE>#if</CODE>. The main difference is that you can use <TG>$IF ..</TG>
inside macros and that the expressions are recalculated for every new
call of the macro.

<P>
</WEBPAGE>


