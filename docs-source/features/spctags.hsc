<DOCPAGE chapter="hsc - Features - " title="Special Tags"
    PREV=":envvar.html"
    NEXT="checkuri.html"
    QTEXT=("What was said was never done<BR>"
          +"Don't panic, it's not really worth your while")
    QAUTHOR='Blur, "Bang"'>

<$macro SPCTAG NAME:string/r TITLE:string/r TAGNAME:string>
<HR>
<$if COND=(not set TagName)><$let TagName=("$"+Name)></$if>
<H2><A NAME=(name)>&lt;<(TagName)>&gt; - <(title)></A></H2>
</$macro>

<$macro POSSATTR /CLOSE>
<P><STRONG>Possible attributes:</STRONG><DL><$content></DL>
</$macro>

<* attribute definition title/data *>
<$macro DTA /CLOSE><DT><CODE><$content></CODE></DT></$macro>

<P><hsc CAP> adds several special tags to process macros, handle
conditionals, include files and lots of other things.</P>

<H2>List Of Special Tags</H2>
<UL>
<LI><A HREF="#comments"><TG>* <I>comment</I> *</TG></A><mdash>insert comments
<LI><A HREF="#content"><TG>$content</TG></A><mdash>insert content of container macro
<li><ln-defent>,<ln-deficon>,<ln-deftag>,<ln-defstyle>,<ln-varlist><mdash>define
entities, icon-entities, tags, styles and attribute-list shotcuts
<LI><A HREF="#define"><TG>$define</TG></A><mdash>define new (global) attribute
<LI><A HREF="#depend"><TG>$depend</TG></A><mdash>add dependency
<LI><A HREF="exec.html"><TG>$exec</TG></A><mdash>execute shell-command
<LI><A HREF="if.html"><TG>$if</TG>, <TG>$else</TG>, <TG>$elseif</TG></A><mdash>conditionals
<LI><A HREF="#include"><TG>$include</TG></A><mdash>include a file
<*<LI><A HREF="#insert"><TG>$insert</TG></A><mdash>insert several stuff*>
<LI><A HREF="#let"><TG>$let</TG></A><mdash>update attribute value
<LI><A HREF=":macro/macros.html"><TG>$macro</TG></A><mdash>define macro-tag
<LI><A HREF="#match"><TG>$match</TG></A><mdash>perform pattern matching and capture subpatterns.
<LI><A HREF="#message"><TG>$message</TG></A><mdash>display user message
<LI><A HREF="#stripws"><TG>$stripws</TG></A><mdash>strip previous/next white space
<LI><A HREF="#export"><TG>$export</TG></A><mdash>export data to a file
<LI><A HREF="#expression"><TG>( <I>expression</I> )</TG></A><mdash>insert expression
<LI><A HREF="#dontparse"><TG>|  <I>verbatim data</I> |</TG></A><mdash>insert verbatim data
</UL>

<SPCTAG NAME="comments" TITLE="Comments" TAGNAME="* ... *">

    <P>One of the basic concepts of every computer language is that
    the user should be able to add comments. The machine will ignore
    them, but they usually help the programmer to understand his
    intention when looking at his source code later.</P>

    <P>As yet another proof of the incompetence of the HTML creators,
    there is no reasonable way to add comments to your source code.
    Early versions of HTML did not offer any possibility to do that at
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

    <P>And you can comment out sections of HTML source without any
    problems for the browser. This simply is possible because comments
    in the hsc-source are not written to the HTML object. So they
    also will not waste bandwidth.</P>

    Of course, if you need the standard comments, you can still use
    <$source PRE><!-- This is an HTML/sgml-comment --></$source>
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
        <dd>URI to depend on (relative to destination directory)</dd>
    <DTA>FILE:bool</DTA>
        <dd>If this attribute is set, <CODE>ON</CODE> is no more
        interpreted as an URI, but as a local filename relative
        to the source directory.</dd>
    </POSSATTR>
    <ExampleNote>
<$source PRE>
<$depend ON="work:dings.dat" FILE>
<$exec COMMAND="convdings FROM work:dings.dat" INCLUDE TEMPORARY>
</$source>

    <P>In this example, <FILE>dings.dat</FILE> contains some data
    maintained by an external application. A script called
    <EXEC>convdings</EXEC> converts <FILE>dings.dat</FILE> to
    legal HTML data end send them to <stdout>, which are inserted
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
         <dd>This specifies the input file to be included</dd>
    <DTA>SOURCE:bool</DTA>
         <dd>By default, include files are interpreted as normal 
         <FILE>*.hsc</FILE> files. Therefor, they may defines macros
         or contain HTML tags to render the text. But if you for
         example want to include an excerpt of a source code, it is
         handy if a less-than character (<qqc>&lt;</qqc>) is not interpreted
         as an escape character, but converted to an entity.<BR>
         This attribute enables such a conversion for less than, greater
         than and ampersand (<qqc>&amp;</qqc>).</dd>
    <DTA>PRE:bool</DTA>
         <dd>The included data will be enclosed inside a 
         <TG>PRE</TG> ... <TG>/PRE</TG>, and the whole section will
         be rendered as pre-formatted.</dd>
    <DTA>TEMPORARY:bool</DTA>
         <dd>Normally, <hsc> keeps track of all files included and stores 
         the names in the project file. Later, they can be used by
         <hscdepp> to find out dependencies.<BR>
         But if a file that is to be removed after the conversion
         ends up in the dependency list of your <Makefile>, it can
         cause trouble for <make>. If the attribute is enabled, the
         input file will not be added to the dependency list.<BR>
         You should consider to enable this attribute, if invoking
         <make> returns something like<BR>
        <SAMP>make: *** No rule to make target `hsc0x395bf7e0001.tmp', 
              needed by `/html/hugo.html'.</SAMP></dd>
    </POSSATTR>

    <ExampleNote>
    <DL>
    <DT><TG>$include FILE="macro.hsc"</TG></dt>
    	<DD>This can be used to include some macro definitions</dd>
    <DT><TG>$include FILE="hugo.mod" SOURCE PRE</TG></dt>
    	<DD>This is reasonable to include a source code that has been
        written using some programming language like Oberon, Pascal or E.</dd>
    </DL>

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
         <dd>Specifies message text</dd>
    <DTA>CLASS:enum("note|warning|error|fatal")='note'</DTA>
         <dd>Specifies message class</dd>
    </POSSATTR>
    <ExampleNote>
<$source PRE>
<$message TEXT="shit happens..." CLASS="fatal">
<$message TEXT="something's wrong" CLASS="warning">
</$source>

<SPCTAG NAME="match" TITLE="Perform pattern matching">
    Perform pattern matching on a string and capture subpatterns, much like
    Perl's <code>m//</code> operator.

    <POSSATTR>
    <DTA>RE:string/required</DTA>
        <dd>The regular experssion to match against. This uses the same syntax as
	the <a href=":features/expressions.html#stringop"><code>MATCH</code>/<code>MATCHI</code>
	string operators</a>, with the important addition that bracketed
	subpatterns do make sense here while the operators simply ignore them.</dd>
    <DTA>S:string/required</DTA>
        <dd>The string to match against the regular expression specified as
	<code>RE</code>.</dd>
    <DTA>NOCASE:bool</DTA>
    	<dd>Perform case-insensitive matching.</dd>
    <DTA>C0:string</DTA>
    	<dd>Specifies a variable name in which the whole string matched by RE will
	be stored; equivalent to Perl's <code>$&amp;</code> variable.</dd>
    <DTA>C1, C2, C3, C4, C5, C6, C7, C8, C9</DTA>
    	<dd>Same as <code>C0</code> but stores the corresponding bracketed
	subpattern; equivalent to Perl's <code>$1 .. $9</code>. If you need more
	fields, you have to match multiple times as the regex library used does
	not support more than 9 subexpressions. The variables specified in
	attributes of this tag need not exist; <hsc> will automatically
	create them if they were undefined before.</dd>
    </POSSATTR>
<$source PRE>
<$match S="-rw-r--r--  1 mb users 15838 Mar 15 23:35 features/spctags.hsc"
        RE="^(...)(...)(...)" C1=puser C2=pgroup C2=pother>
</$source>
<p>The above will extract the user, group and other permission bits from a line
of `<code>ls -l</code>' output and put them into the variables
<code>puser</code>, <code>pgroup</code> and <code>pother</code>
respectively.</p>
<SPCTAG NAME="stripws" TITLE="Strip White Space">
    <POSSATTR>
    <DTA>TYPE:enum("both|prev|succ|none")="both"</DTA>
    <dd>This specifies which white space at the current
    location should be removed.<BR>

    <CODE>Prev</CODE> will remove all white spaces, which have occured
    between the previous word and the <qqc>&lt;</qqc> of this tag,
    <CODE>succ</CODE> will skip all blanks between the <qqc>&gt;</qqc>
    and the next text or visible tag.<BR>

    <CODE>Both</CODE> will act like if both <CODE>prev</CODE> and
    <CODE>succ</CODE> would have been specified, and
    <CODE>none</CODE> has no effect on your data.</dd>
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

<SPCTAG NAME="export" TITLE="Export data to a file">
<POSSATTR>
<DTA>FILE:string/required</DTA>
<dd>Name of file where to write output. If the file already exists,
it will be overwritten without any warning.</dd>
<DTA>DATA:string/required</DTA>
<dd>Data to store in the file</dd>
<DTA>APPEND:bool</DTA>
<dd>If the output file already exists, the new data will be
appended, leaving the old contents intact.</dd>
</POSSATTR>

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

    If you created some perverted HTML code or use features <hsc>
    can't handle (which is theoretically impossible), you can keep
    <hsc> from parsing this section by surrounding it with <TG>| ...
    |</TG>. Of course, this is a dirty hide-out and should be used
    only for special cases.

    <ExampleNote>
    <$source PRE><|<B>some &<> bull >> shit</B>|></$source>

    This can be useful when you want to use <A
    HREF=":others.html">other HTML extensions</A> together with <hsc>.

<BR>
</DOCPAGE>
