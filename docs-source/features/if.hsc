<WEBPAGE chapter="hsc - Features - " title="Conditionals"
    PREV=":macro/macros.html"
    NEXT=":macro/macros.html">

<H2>Syntax</H2>

Conditionals looks like that:

<PRE>
<CODE>&lt;$if COND=</CODE><I><A HREF="expressions.html">expression</A></I><CODE>&gt;</CODE>

  <I>code to be processed if condition fullfilled</I>

<CODE>&lt;$elseif COND=</CODE><I><A HREF="expressions.html">expression</A></I><CODE>&gt;</CODE>

  <I>(optional) code to be processed if alternative condition fullfilled</I>

<CODE>&lt;$else&gt;</CODE>

  <I>(optional) code to be processed if none of previous conditions matched</I>

<CODE>&lt;/$if&gt;</CODE>
</PRE>

<P>Both <TG>$if</TG> and <TG>$elseif</TG> require a boolean attribute 
<CODE>COND</CODE>; <CODE>false</CODE> is repesented by an empty string,
<CODE>true</CODE> by any non-empty string. Normaly, you will like to
set <CODE>COND</CODE> using
<A HREF="expressions.html">expressions</A>.</P>

<H2>Some simple examples</H2>

Now let's see how this works in practice:

<$SOURCE PRE>
    <$if COND=(NAME="sepp")>
       You must be sepp!
    </$if>
</$SOURCE>

<P>This one inserts the text "<CODE>You must be sepp!</CODE>", if the attribute
<CODE>NAME</CODE> has the value "<CODE>sepp</CODE>". Note that the 
"<CODE>=</CODE>"-operator performs a case-insensitive string-comparison,
so setting <CODE>NAME="SEPP"</CODE> would lead to the same result.</P>

Now let's extend this:

<$SOURCE PRE>
    <$if COND=(NAME="sepp")>
       You must be sepp!
    <$else>
       I don't know you.
    </$if>
</$SOURCE>

<P>Setting <CODE>NAME="sepp"</CODE> will create the same text as above. Any
other value for <CODE>NAME</CODE> will insert 
"<CODE>I don't know you.</CODE>".</P>

<H2>Nesting conditionals</H2>

<P>Nesting them is also possible, of course:</P>

<$SOURCE PRE>
    <$if COND=(NAME="sepp")>
       You must be sepp!
       <$if COND=(HOME="austria")>
           Hollareiduliö! An Austrian!
       <$else>
           <(HOME)> is a strange country.
       </$if>
    <$else>
       A strange guy from a strange place.
    </$if>
</$SOURCE>


<H2>Conditionals and macros</H2>

<P>You can not compare <hsc>'s <TG>$if</TG> to primitive and clumsy 
<CODE>#if</CODE> of the C-preprocessor. The main difference is that 
you can use <TG>$if</TG> inside macros and that expressions are 
recalculated for every new call of the macro.</P>

<$SOURCE PRE>
    <$macro TRY-A HREF:uri/r TITLE:string/r>
    <$if COND=(Exists(HREF))>
        <A HREF=(HREF)><(TITLE)></A> <* insert link to HREF *>
    <$else>
        <(TITLE)>                    <* insert plain title *>
    </$if>
    </$macro>
</$SOURCE>

<P>This one inserts a link to an URI specified with <CODE>HREF</CODE>, using
<CODE>TITLE</CODE> as link text; but only, if the destination (local) URI
can be accesed. If the required document is not available, only the plain
text without a link will be inserted.</P>

<P>The "<CODE>/r</CODE>" behind the declaration of the macro-attributes
is short for "<CODE>/required</CODE>" and means that the macro needs both
of these attributes to be set during the macro-call.</P>

<P>For example, you can utilize this macro using</P>

<$SOURCE PRE>
    You should read the document about recent
    <TRY-A HREF="../bugfixes.html" TITLE="bufixes">
    if there are any.
</$SOURCE>

<P>This leads to the text</P>
<PRE>
    You should read the document about recent bugfixes if there are any.
</PRE>
<P>with a anchor around the term "<CODE>bugfixes</CODE>" if the document
"<FILE>../bugfixes.html</FILE>" exists.</P>

</WEBPAGE>
