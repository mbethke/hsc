<WEBPAGE chapter="hsc - Features - " title="Special Tags"
    PREV=":distrib.html"
    NEXT="syntax.html"
    QTEXT=("What was said was never done<BR>"
          +"Don't panic, it's not really worth your while")
    QAUTHOR='Blur, "Bang"'>

<P><hsc> adds several special tags to process macros, handle conditionals,
include files and lots of other things.</P>

<H2>List of special tags</H2>
<UL>
<LI><A HREF="#comments"><TG>* <I>comment</I> *</TG></A> insert comments
<LI><A HREF="prefs.html#defent"><TG>$defent</TG></A>, 
    <A HREF="prefs.html#deficon"><TG>$deficon</TG></A>, 
    <A HREF="prefs.html#deftag"><TG>$deftag</TG></A> 
    define entities, icon-entities and tags
<LI><A HREF="#define"><TG>$define</TG></A> define a new (global) attribute
<LI><A HREF="exec.html"><TG>$exec</TG></A> execute shell-command
<LI><A HREF="if.html"><TG>$if</TG>, <TG>$else</TG>, <TG>$elseif</TG></A> conditionals
<LI><A HREF="#include"><TG>$include</TG></A> include a file
<*<LI><A HREF="#insert"><TG>$insert</TG></A> insert several stuff*>
<LI><A HREF="#let"><TG>$let</TG></A> set new attribute value
<LI><A HREF=":macro/macros.html"><TG>$macro</TG></A> define macro-tags
<LI><A HREF="#message"><TG>$message</TG></A> display user message
<LI><A HREF="#expression"><TG>( <I>expression</I> )</TG></A> insert expression
<LI><A HREF="#dontparse"><TG>|  <I>don't parse</I> |</TG></A> don't parse section
</UL>

<$macro SPCTAG NAME:string/r TITLE:string/r>
<HR>
<H2><A NAME=(name)><(title)></A></H2>
</$macro>

<$macro POSSATTR>
<P><STRONG>Possible attributes:</STRONG><DL>
</$macro>
<$macro /POSSATTR></DL></$macro>

<* attribute definition title/data *>
<$macro DTA><DT><CODE></$macro>  
<$macro /DTA></CODE><DD></$macro>  

<$macro EXAMPLE>
<P><STRONG>Example:</STRONG>
</$macro>


<SPCTAG NAME="comments" TITLE="Comments">
    You can insert a comment with

    <BLOCKQUOTE>
    <TG>* This is a hsc-comment *</TG>
    </BLOCKQUOTE>

    You can also nest such comments.<P>

    And you can comment out sections of html-source without any problems
    for the browser. This simply is possible because comments in the
    hsc-source are not written to the html-object.<P>

    Of course, if you need the standard comments, you can use
    <BLOCKQUOTE>
    <TG>!-- This is a html/sgml-comment --</TG>
    </BLOCKQUOTE>
    as usual.<P>

<SPCTAG NAME="if" TITLE="Conditionals">
    Conditionals are used to decide whether some part of the text should
    be processed or not.
    As there is a lot to tell about them, there exists a whole chapter 
    dealing with <A HREF="if.html">conditionals</A>.

<SPCTAG NAME="include" TITLE="Include file">
    Text files can be included using <TG>$include</TG>.

    <POSSATTR>
    <DTA>FILE:string/required</DTA>
         This specifies the input file to be included
    <DTA>SOURCE:bool</DTA>
         by default, include files are interpreted as normal 
         <FILE>*.hsc</FILE> files. Therefor, they may defines macros
         or contain html tags to render the text. But it you for
         example want to include an exerpt of a source code, it is
         handy if a less-than character (<qqc>&lt;</qqc>) is not interpreted
         as an escape character, but converted to an entity.<BR>
         This attribute enables such a conversion for less than, greater
         than and ampersand (<qqc>&amp;</qqc>).
    <DTA>PRE:bool</DTA>
         The included data will be enclosed inside a 
         <TG>PRE</TG> ... <TG>/PRE</TG>, and the whole section will
         be rendered as preformated.
    <DTA>TEMPORARY:bool</DTA>
         Normally, <hsc> keeps track of all files included and stores 
         the names in the project file. Later, they can be used by
         <hscdepp> to find out dependencies.<BR>
         But if a file that is to be removed after the conversion
         ends up in the dependency list of your <Makefile>, it can
         cause trouble for <make>. If the attribute is enabled, the
         input file will not be added to the dependency list.<BR>
         You should consider to enable this attribute, if invoking
         <make> returns something like<BR>
        <SAMP>make: *** No rule to make target `hsc0x395bf7e0001.tmp', 
              needed by `/html/hugo.html'.</SAMP>
<* " INDENT:num TABSIZE:num=\"4\" " *>
    </POSSATTR>

    <EXAMPLE>
    <DL>
    <DT><TG>$include FILE="macro.hsc"</TG>
    <DD>This can be used to include some macro definitions
    <DT><TG>$include FILE="hugo.c" SOURCE PRE</TG>
    <DD>This is reasonable to include a source code that has been
        written using the programming langugage C.
    </DL>
<*
<SPCTAG NAME="insert" TITLE="Insert several stuff">
    You can insert several stuff using the tag

    <BLOCKQUOTE>
    <TG>$insert what [options]</TG>
    </BLOCKQUOTE>

    <H3>Insert current date and time</H3>
        You can insert the current date and time simply by
          <UBQ><TG>$insert TIME</TG></UBQ>
        using a default format.<P>

        Optionally, you can pass a format-string, that discribes the time.
        As <hsc> just calls the ANSI-C function
        <A HREF="strftime.txt"><CODE>strftime()</CODE></A>
        to perform that task, you can use the same format specifications.

        <EXAMPLE>
        <DL><TG>$insert TIME FORMAT="%b %d  %y"</TG>
        inserts current date with the strange ANSI-C
        <CODE>__TIME__</CODE>-format.
        </DL>

    <H3>Insert text</H3>
        As an required argument, you must give a string
        that contains the text. Example:
        <UBQ><TG>$insert TEXT="hugo was here!"</TG></UBQ>
        inserts the text "<CODE>hugo was here</CODE>". Of course, this does not
        make much sense.<TG>$insert TEXT="..."</TG> is suggested to be used
        with attribute values. Example:
        <UBQ><TG>$insert TEXT=(href)</TG></UBQ>
        inserts the value of the macro-attribute <CODE>href</CODE>.
*>

<SPCTAG NAME="define" TITLE="Define attributes">

    You can create an attribute and pass a value to it via

    <BLOCKQUOTE>
    <TG>$define <I><A HREF=":macro/attrib.html">attribute</A></I></TG>
    </BLOCKQUOTE>

    <P>If you define an attribute via <TG>$define</TG> inside a macro, it is of
    local existence only and is removed after processing the macro. You
    can suppress this with using the attribute flag <CODE>/GLOBAL</CODE>: in
    this case, the attribute exists until the end of conversion.</P>

    <P>You can use the flag <CODE>/CONST</CODE> to make the attribute read-only.
    That means it can't be overwritten by <TG>$let</TG></P>

<SPCTAG NAME="let" TITLE="Set new attribute value">

    You can update an attribute's value with

    <BLOCKQUOTE>
    <TG>$let <I><A HREF=":macro/attrib.html">attribute_name</A></I> =
    <I>new_value</I></TG>
    </BLOCKQUOTE>

    <EXAMPLE>
    <$source PRE>
    <$define hugo:string="hugo">       <* create hugo and set to "hugo" *>
    <$let hugo=(hugo+" ist doof.")>    <* update it to "hugo ist doof." *>
    </$source>

<SPCTAG NAME="macro" TITLE="Macros">
    Macros can used to define your own short-cuts and templates.
    As there is a lot to tell about this feature, there exists a whole
    chapter dealing with
    <A HREF=":macro/macros.html">macros</A>.

<SPCTAG NAME="message" TITLE="User messages">
    <P>During conversion, messages might show up. But not only <hsc>
    creates messages, also the user is able to so, if for instance
    he wants to perform some plausibility checks of macro arguments.</P>
    <POSSATTR>
    <DTA>TEXT:string/required</DTA>
         Specifies message text
    <DTA>CLASS:enum("note|warning|error|fatal")='note'</DTA>
         Specifies message class
    </POSSATTR>
    <EXAMPLE><BR>
    <TG>$message TEXT="shit happens..." CLASS="fatal"</TG><BR>
    <TG>$message TEXT="something's wrong" CLASS="warning"</TG>

<SPCTAG NAME="expression" TITLE="Insert expression">

    This tag is used to insert data of attributes and
    <A HREF="expressions.html">expressions</A>.<BR>
    <EXAMPLE>
    <$source PRE>
    <$define hugo:string="hugo">       <* create hugo and set to "hugo" *>
    <(hugo+" ist doof.")>              <* insert text "hugo ist doof." *>
    </$source>

<SPCTAG NAME="dontparse" TITLE="Don't parse section">

    If you created some perverted HTML source or use features
    <hsc> can't handle (which is theoretically impossible), you
    can keep of <hsc> from parsing this section by surrounding it with
    <TG>| ... |</TG>. Of course, this is a dirty hide-out and should
    be used only for special cases.

<BR>
</WEBPAGE>
