<WEBPAGE chapter="hsc - Features - " title="Special Tags"
    PREV=":envvar.html"
    NEXT="checkuri.html"
    QTEXT=("What was said was never done<BR>"
          +"Don't panic, it's not really worth your while")
    QAUTHOR='Blur, "Bang"'>

<P><hsc CAP> adds several special tags to process macros, handle
conditionals, include files and lots of other things.</P>

<H2>List Of Special Tags</H2>
<UL>
<LI><A HREF="#comments"><TG>* <I>comment</I> *</TG></A> - insert comments
<LI><A HREF="#content"><TG>$content</TG></A>
    - insert content of container macro
<LI><A HREF="prefs.html#defent"><TG>$defent</TG></A>, 
    <A HREF="prefs.html#deficon"><TG>$deficon</TG></A>, 
    <A HREF="prefs.html#deftag"><TG>$deftag</TG></A> 
    - define entities, icon-entities and tags
<LI><A HREF="#define"><TG>$define</TG></A> - define new (global) attribute
<LI><A HREF="#depend"><TG>$depend</TG></A> - add dependency
<LI><A HREF="exec.html"><TG>$exec</TG></A> - execute shell-command
<LI><A HREF="if.html"><TG>$if</TG>, <TG>$else</TG>, <TG>$elseif</TG></A> - conditionals
<LI><A HREF="#include"><TG>$include</TG></A> include a file
<*<LI><A HREF="#insert"><TG>$insert</TG></A> insert several stuff*>
<LI><A HREF="#let"><TG>$let</TG></A> - update attribute value
<LI><A HREF=":macro/macros.html"><TG>$macro</TG></A> - define macro-tag
<LI><A HREF="#message"><TG>$message</TG></A> - display user message
<LI><A HREF="#stripws"><TG>$stripws</TG></A> - strip previous/next white space
<LI><A HREF="#expression"><TG>( <I>expression</I> )</TG></A> - insert expression
<LI><A HREF="#dontparse"><TG>|  <I>verbatim data</I> |</TG></A> - insert verbatim data
</UL>

<$macro SPCTAG NAME:string/r TITLE:string/r TAGNAME:string>
<HR>
<$if COND=(not set TagName)><$let TagName=("$"+Name)></$if>
<H2><A NAME=(name)>&lt;<(TagName)>&gt; - <(title)></A></H2>
</$macro>

<$macro POSSATTR /CLOSE>
<P><STRONG>Possible attributes:</STRONG><DL><$content></DL>
</$macro>

<* attribute definition title/data *>
<$macro DTA><DT><CODE></$macro>
<$macro /DTA></CODE><DD></$macro>

<SPCTAG NAME="comments" TITLE="Comments" TAGNAME="* ... *">

    <P>One of the basic concepts of every computer language is that
    the user should be able to add comments. The machine will ignore
    them, but they usually help the programmer to understand his
    intention when looking at his source code later.</P>

    <P>As yet another proof of the incompetence of the html-creators,
    there is no reasonable way to add comments to your source code.
    Early versions of html did not offer any possibility to do that at
    all. Later ones support comments comparable to sgml. But as the
    way sgml handles comments is so messy and weird, nearly no one
    knows how they are really supposed to work (including myself and -
    more remarkable - most developers of w3-browsers).</P>

    <P>In general, there is <STRONG>no</STRONG> way to use
    sgml-comments so they will work with all browsers. Many browsers
    are unable to find out when a comment ends, independent of whether
    you did it right or wrong.</P>

    <P>Furthermore, such comments also waste bandwidth, as usually the
    reader will not see them (except he views the document source).
    And internal notes of w3-authors are usually not interesting for
    the public...</P>

    <P>It is really remarkable that they managed to fuck up such a
    simple and usually strait forward concept like comments; even
    most assembler languages are smarter with this.</P>

    <P>Anyway, with <hsc> you can insert a comment like</P>

    <$source PRE><* This is a <* nested *> hsc-comment *></$source>

    <P>As you can see, such comments can also be nested - yes, this is
    a documented behaviour.<P>

    <P>And you can comment out sections of html-source without any
    problems for the browser. This simply is possible because comments
    in the hsc-source are not written to the html-object. So they
    also will not waste bandwidth.</P>

    Of course, if you need the standard comments, you can still use
    <$source PRE><!-- This is a html/sgml-comment --></$source>
    as usual. But think twice before doing so.

<SPCTAG NAME="content" TITLE="Insert Content">
    The tag <TG>$content</TG> can be only used inside a
    <A HREF=":macro/macros.html#container">container macro</A>
    and inserts the content the user specified inside the start and
    end tag for the macro.

<SPCTAG NAME="if" TITLE="Conditionals">
    Conditionals are used to decide whether some part of the text should
    be processed or not.
    As there is a lot to tell about them, there exists a whole chapter 
    dealing with <A HREF="if.html">conditionals</A>.

<SPCTAG NAME="depend" TITLE="Add Dependencies">
    If your source should not only be updated if an included
    file has been modified, you can use the tag <TG>$depend</TG> to
    add an additional dependency which will be noted in the project
    data.

    <POSSATTR>
    <DTA>ON:string</DTA>
        URI to depend on (relative to destination directory)
    <DTA>FILE:bool</DTA>
        If this attribute is set, <CODE>ON</CODE> is no more
        interpreted as an URI, but as a local filename relative
        to the source directory.
    </POSSATTR>
    <ExampleNote>
<$source PRE>
<$depend ON="work:dings.dat" FILE>
<$exec COMMAND="convdings FROM work:dings.dat" INCLUDE TEMPORARY>
</$source>

    <P>In this example, <FILE>dings.dat</FILE> contains some data
    maintained by an external application. A script called
    <EXEC>convdings</EXEC> converts <FILE>dings.dat</FILE> to
    legal html data end send them to <stdout>, which are inserted
    into the current document.</P>

    Obviously, <FILE>dings.dat</FILE> is not maintained or included
    by hsc, so the current document does not depend on it.
    But by specifying the above <TG>$depend</TG>, the current
    source will be updated if <FILE>dings.dat</FILE> has been
    modified.

<SPCTAG NAME="include" TITLE="Include File">
    Text files can be included using <TG>$include</TG>.

    <POSSATTR>
    <DTA>FILE:string/required</DTA>
         This specifies the input file to be included
    <DTA>SOURCE:bool</DTA>
         by default, include files are interpreted as normal 
         <FILE>*.hsc</FILE> files. Therefor, they may defines macros
         or contain html tags to render the text. But if you for
         example want to include an excerpt of a source code, it is
         handy if a less-than character (<qqc>&lt;</qqc>) is not interpreted
         as an escape character, but converted to an entity.<BR>
         This attribute enables such a conversion for less than, greater
         than and ampersand (<qqc>&amp;</qqc>).
    <DTA>PRE:bool</DTA>
         The included data will be enclosed inside a 
         <TG>PRE</TG> ... <TG>/PRE</TG>, and the whole section will
         be rendered as pre-formatted.
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

    <ExampleNote>
    <DL>
    <DT><TG>$include FILE="macro.hsc"</TG>
    <DD>This can be used to include some macro definitions
    <DT><TG>$include FILE="hugo.mod" SOURCE PRE</TG>
    <DD>This is reasonable to include a source code that has been
        written using some programming language like Oberon, Pascal or E.
    </DL>
<*
<SPCTAG NAME="insert" TITLE="Insert Several Stuff">
    You can insert several stuff using the tag

    <BLOCKQUOTE>
    <TG>$insert what [options]</TG>
    </BLOCKQUOTE>

    <H3>Insert Current Date And Time</H3>
        You can insert the current date and time simply by
          <UBQ><TG>$insert TIME</TG></UBQ>
        using a default format.<P>

        Optionally, you can pass a format-string, that discribes the time.
        As <hsc> just calls the ANSI-C function
        <A HREF="strftime.txt"><CODE>strftime()</CODE></A>
        to perform that task, you can use the same format specifications.

        <ExampleNote>
        <DL><TG>$insert TIME FORMAT="%b %d  %y"</TG>
        inserts current date with the strange ANSI-C
        <CODE>__TIME__</CODE>-format.
        </DL>

    <H3>Insert text</H3>
        As an required argument, you must give a string
        that contains the text.

        <ExampleNote>
        <UBQ><TG>$insert TEXT="hugo was here!"</TG></UBQ>
        inserts the text "<CODE>hugo was here</CODE>". Of course, this does not
        make much sense.<TG>$insert TEXT="..."</TG> is suggested to be used
        with attribute values.

        <ExampleNote>
        <UBQ><TG>$insert TEXT=(href)</TG></UBQ>
        inserts the value of the macro-attribute <CODE>href</CODE>.
*>

<SPCTAG NAME="define" TITLE="Define A New Attribute">

    You can create an attribute and pass a value to it via

    <BLOCKQUOTE>
    <TG>$define <I><A HREF=":macro/attrib.html">attribute-declaration</A></I></TG>
    </BLOCKQUOTE>

    <P>If you define an attribute using <TG>$define</TG> inside a
    macro, it is of local existence only and is removed after
    processing the macro. You can suppress this with the attribute
    modifier <CODE>/GLOBAL</CODE>: in this case, the attribute exists
    until the end of conversion.</P>

    <P>You can use the modifier <CODE>/CONST</CODE> to make the
    attribute read-only. That means it can not be updated with
    <TG>$let</TG>.</P>

    For an example, see <TG>$let</TG>.

<SPCTAG NAME="let" TITLE="Update The Value Of An Attribute">

    The tag <TG>$let</TG> can be used to update an attribute with a
    new value. It has its own syntax and expects the name of the
    attribute and after an equal sign the new value, for instance:

<$source PRE>
<$define hugo:string="hugo">       <* create hugo and set to "hugo" *>
<$let hugo=(hugo+" ist doof.")>    <* update it to "hugo ist doof." *>
</$source>


    If you do not specify a value, the attribute will
    be unset (but still remains defined):

<$source PRE>
<$let hugo>                        <* unset hugo *>
</$source>


    You can also use <ln-cond-assigns> for updating:

<$source PRE>
<$let hugo?=sepp>                  <* if sepp has any value, copy it to hugo *>
</$source>

<SPCTAG NAME="macro" TITLE="Define Macro">

    Macros can be used to define your own short-cuts and templates. As
    there is a lot to tell about this feature, there exists a whole
    chapter dealing with <A HREF=":macro/macros.html">macros</A>.

<SPCTAG NAME="message" TITLE="Launch User Message">

    <P>During conversion, <a href=":messages.html">messages</a> might
    show up. But not only <hsc> creates messages, also the user is
    able to do so using <TG>$message</TG>. Messages created by means
    of this tag will always show up as <ln-msg id="39">, but the user
    can set text and class of it.</P>

    <P>For instance this can be useful, if he wants to perform some
    plausibility checks of macro arguments, or for debugging purpose
    (<qq>still alive</qq> messages).</P>

    <POSSATTR>
    <DTA>TEXT:string/required</DTA>
         Specifies message text
    <DTA>CLASS:enum("note|warning|error|fatal")='note'</DTA>
         Specifies message class
    </POSSATTR>
    <ExampleNote>
<$source PRE>
<$message TEXT="shit happens..." CLASS="fatal">
<$message TEXT="something's wrong" CLASS="warning">
</$source>

<SPCTAG NAME="stripws" TITLE="Strip White Space">
    <POSSATTR>
    <DTA>TYPE:enum("both|prev|succ|none")="both"</DTA>
    This specifies which white space at the current
    location should be removed.<BR>

    <CODE>Prev</CODE> will remove all white spaces, which have occured
    between the previous word and the <qqc>&lt;</qqc> of this tag,
    <CODE>succ</CODE> will skip all blanks between the <qqc>&gt;</qqc>
    and the next text or visible tag.<BR>

    <CODE>Both</CODE> will act like if both <CODE>prev</CODE> and
    <CODE>succ</CODE> would have been specified, and
    <CODE>none</CODE> has no effect on your data.
    </POSSATTR>

    <ExampleNote>
<$source PRE>
prev  <$stripws type=both>   succ
prev  <$stripws type=prev>   succ
prev  <$stripws type=succ>   succ
prev  <$stripws type=none>   succ
</$source>
    results in
<PRE>
prevsucc
prev   succ
prev  succ
prev     succ
</PRE>
    Note that the word <qqc>prev</qqc> is succeeded by two blanks,
    whereas the word <qqc>succ</qqc> is preceded by three spaces.

<SPCTAG NAME="expression" TITLE="Insert Expression" TAGNAME="( ... )">

    The tag <TG>(<I>expression</I>)</TG> is used to insert data
    of attributes and
    <A HREF="expressions.html">expressions</A>.<BR>
    <ExampleNote>
<$source PRE>
<$define hugo:string="hugo">       <* create hugo and set it to "hugo" *>
<(hugo+" ist doof.")>              <* insert text "hugo ist doof." *>
</$source>

<SPCTAG NAME="dontparse" TITLE="Insert Verbatim Data" TAGNAME="| ... |">

    If you created some perverted html-code or use features <hsc>
    can't handle (which is theoretically impossible), you can keep
    <hsc> from parsing this section by surrounding it with <TG>| ...
    |</TG>. Of course, this is a dirty hide-out and should be used
    only for special cases.

    <ExampleNote>
    <$source PRE><|<B>some &<> bull >> shit</B>|></$source>

    This can be useful when you want to use <A
    HREF=":others.html">other html-extensions</A> together with <hsc>.

<BR>
</WEBPAGE>
