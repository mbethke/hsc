<WEBPAGE chapter="hsc - Features - " title="Special Tags"
    PREV=":distrib.html"
    NEXT="syntax.html"
    QTEXT=("What was said was never done<BR>"
          +"Don't panic, it's not really worth your while")
    QAUTHOR='Blur, "Bang"'>

<P><hsc> adds several special tags to process macros, handle conditionals,
include files and lots of other things.</P>

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

<SPCTAG NAME="if" TITLE="Conditional conversion">
    As there is a lot to tell about this feature, there exists a whole
    <A HREF="if.html">section about <TG>$if</TG></A>.

<SPCTAG NAME="include" TITLE="Include file">
    A <hsc>-file can be included to the text using

    <BLOCKQUOTE>
    <TG>$include FILE="filename" [SOURCE] [PRE] [TEMPORARY]</TG>
    </BLOCKQUOTE>

    <P>If you include a file this way, it is handled as an usual
    <hsc>-file. That means, all tags and special characters are
    handled as before.</P>

    To include a source file, eg a C- or HTML-source so that the
    tags are not interpreted but displayed, add the optional
    boolean attribute <CODE>SOURCE</CODE>.

    <BLOCKQUOTE>
    <TG>$include FILE="hugo.c" SOURCE</TG>
    </BLOCKQUOTE>

    <P>Now the basic special characters "&lt;", "&gt;" and "&amp;" are
    replaced by their entities.<P>
    Note that this does <STRONG>not</STRONG> include a <TG>PRE</TG>-tag
    to render the text as it exists with the same line breaks and
    spaces.</P>

    <P>To get this done, you should use the optional boolean attribute
    <CODE>PRE</CODE>. This inserts a <TG>PRE</TG> before the included
    data and a <TG>/PRE</TG> afterwards.</P>

    <P>If you enable <CODE>TEMPORARY</CODE>, the file included will not
    effect the dependencies for the current document. This is only
    reasonable if you have the <KBD>PRJFILE</KBD> CLI-option set
    and want to include a temporary file which might not exist
    anymore when invokin <make> the next time.</P>

    <P>You should consider to enable this attribute, if invoking <make>
    returns something like<BR>
    <SAMP>make: *** No rule to make target `hsc0x395bf7e0001.tmp', 
          needed by `/html/hugo.html'.</SAMP>
    </P>

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

        Example:<BR>
          <UBQ><TG>$insert TIME FORMAT="%b %d  %y"</TG></UBQ>
        inserts current date with the strange ANSI-C
        <CODE>__TIME__</CODE>-format.

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

    Example:<BR>
    <$source PRE>
    <$define hugo:string="hugo">       <* create hugo and set to "hugo" *>
    <$let hugo=(hugo+" ist doof.")>    <* update it to "hugo ist doof." *>
    </$source>

<SPCTAG NAME="macro" TITLE="Define macros">
    As there is a lot to tell about this feature, there exists a whole
    <A HREF=":macro/macros.html">section about macros</A>...

<SPCTAG NAME="message" TITLE="User messages">
    During conversion, you can invoke messages by yourself using
    <$source PRE>
    <$message TEXT="message text" [CLASS="class"]>
    </$source>
    For an example, look at <A HREF="exec.html"><TG>$exec</TG></A>.

<SPCTAG NAME="expression" TITLE="Insert expression">

    This tag is used to insert data of attributes and
    <A HREF="expressions.html">expressions</A>. For example:

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
