<WEBPAGE chapter="hsc - Features - " title="Special Tags"
    PREV=":distrib.html"
    NEXT="syntax.html">

<B>hsc</B> adds some special tags, which can be quite useful for maintainers
of big HTML-projects. If you use them right, they can save a lot of time
and nerves.

<UL>
<LI><A HREF="#comments"><TG>* <I>comment</I> *</TG></A> insert comments
<LI><A HREF="#exec"><TG>$EXEC</TG></A> execute shell commands during conversion
<LI><A HREF="if.html"><TG>$IF</TG> <TG>$ELSE</TG></A> conditional conversion
<LI><A HREF="#include"><TG>$INCLUDE</TG></A> include a file
<LI><A HREF="#insert"><TG>$INSERT</TG></A> insert several stuff
<LI><A HREF="#let"><TG>$LET</TG></A> set global attributes
<LI><A HREF=":macro/macros.html"><TG>$MACRO</TG></A> define macro-tags
<LI><A HREF="#dontparse"><TG>|  <I>don't parse</I> |</TG></A> don't parse section
</UL>

<$MACRO SPCTAG NAME:string/r TITLE:string/r>
<HR>
<A NAME=<name>><H2><$INSERT TEXT=<title>></H2></A>
</$MACRO>

<SPCTAG NAME="comments" TITLE="Comments">
    You can insert a comment with

    <BLOCKQUOTE>
    <TG>* This is a hsc-comment *</TG>
    </BLOCKQUOTE>

    You can also nest such comments.<P>

    And you can comment out sections of html-source without any problems
    for the browser. This simply is possible because comments in the
    HSC-source are not written to the HTML-object.<P>

    Of course, if you need the standard comments, you can use
    <BLOCKQUOTE>
    <TG>!-- This is a html/sgml-comment --</TG>
    </BLOCKQUOTE>
    as usual.<P>

<SPCTAG NAME="exec" TITLE="Execute shell command">

    A shell command can be executed with
    <BLOCKQUOTE>
    <TG>$INCLUDE COMMAND="command"</TG>
    </BLOCKQUOTE>

    An example usage would be to insert a listing of a
    directory:

<$include FILE="exmpl/exec.inc" SOURCE PRE>

    And the data created by this code sequence would look
    like this:

    <$include FILE="exmpl/exec.inc">

    If the command's return code is not equal to zero, a
    warning message will be displayed.

<SPCTAG NAME="if" TITLE="Conditional conversion">
    As there is a lot to tell about this feature, it has it's <A HREF="if.html">
    own page</A>.

<SPCTAG NAME="include" TITLE="Include file">
    A <hsc>-file can be included to the text using

    <BLOCKQUOTE>
    <TG>$INCLUDE FILE="filename" [SOURCE] [PRE]</TG>
    </BLOCKQUOTE>

    If you include a file this way, it is handled as an usual
    <hsc>-file. That means, all tags and special characters are
    handled as before.<P>

    To include a source file, eg a C- or HTML-source so that the
    tags are not interpreted but displayed, add the optional
    boolean attribute <CODE>SOURCE</CODE>.

    <BLOCKQUOTE>
    <TG>$INCLUDE FILE="hugo.c" SOURCE</TG>
    </BLOCKQUOTE>

    Now the basic special characters "&lt;", "&gt;" and "&amp;" are
    replaced by their entities.<P>
    Note that this does <STRONG>not</STRONG> include a <TG>PRE</TG>-tag
    to render the text as it exists with the same line breaks and
    spaces.<P>

    To get this done, you should use the optional boolean attribute
    <CODE>PRE</CODE>. This inserts a <TG>PRE</TG> before the included
    data and a <TG>/PRE</TG> afterwards.


<SPCTAG NAME="insert" TITLE="Insert several stuff">
    You can insert several stuff using the tag

    <BLOCKQUOTE>
    <TG>$INSERT what [options]</TG>
    </BLOCKQUOTE>

    <H3>Insert current date and time</H3>
        You can insert the current date and time simply by
          <UBQ><TG>$INSERT TIME</TG></UBQ>
        using a default format.<P>

        Optionally, you can pass a format-string, that discribes the time.
        As <hsc> just calls the ANSI-C function <CODE>strftime()</CODE> to
        perform that task, I've included an extract of the man-pages to this
        function:

<PRE>
        Each conversion specification is replaced by the characters as follows
        which are then copied into the buffer.

         %A    is replaced by the full weekday name.

         %a    is replaced by the abbreviated weekday name, where the abbreviation
              is the first three characters.

         %B    is replaced by the full month name.

         %b or h
              is replaced by the abbreviated month name, where the abbreviation
              is the first three characters.

         %C    is equivalent to ``%a %b %e %H:%M:%S %Y'' (the format produced by
              asctime(3).

         %c    is equivalent to ``%m/%d/%y''.

         %D    is replaced by the date in the format ```mm/dd/yy'''.

         %d    is replaced by the day of the month as a decimal number (01­31).

         %e    is replaced by the day of month as a decimal number (1­31); single
              digits are preceded by a blank.

         %H    is replaced by the hour (24­hour clock) as a decimal number
              (00­23).

         %I    is replaced by the hour (12­hour clock) as a decimal number
              (01­12).

         %j    is replaced by the day of the year as a decimal number (001­366).

         %k    is replaced by the hour (24­hour clock) as a decimal number (0­23);

              single digits are preceded by a blank.

         %l    is replaced by the hour (12­hour clock) as a decimal number (1­12);
              single digits are preceded by a blank.

         %M    is replaced by the minute as a decimal number (00­59).

         %m    is replaced by the month as a decimal number (01­12).

         %n    is replaced by a newline.

         %p    is replaced by either ``AM'' or ``PM'' as appropriate.

         %R    is equivalent to ``%H:%M''

         %r    is equivalent to ``%I:%M:%S %p''.

         %t    is replaced by a tab.

         %S    is replaced by the second as a decimal number (00­60).

         %s    is replaced by the number of seconds since the Epoch, UCT (see
              mktime(3)).

         %T or X
              is equivalent to ``%H:%M:%S''.

         %U    is replaced by the week number of the year (Sunday as the first day
              of the week) as a decimal number (00­53).

         %W    is replaced by the week number of the year (Monday as the first day
              of the week) as a decimal number (00­53).

         %w    is replaced by the weekday (Sunday as the first day of the week) as
              a decimal number (0­6).

         %x    is equivalent to ``%m/%d/%y %H:%M:%S''.

         %Y    is replaced by the year with century as a decimal number.

         %y    is replaced by the year without century as a decimal number
              (00­99).

         %Z    is replaced by the time zone name.

         %%
            is replaced by `%'.
</PRE>
        Example:<BR>
          <UBQ><TG>$INSERT TIME FORMAT="%b %d  %y"</TG></UBQ>
        inserts current date with the strange ANSI-C
        <CODE>__TIME__</CODE>-format.

    <H3>Insert text</H3>
        As an required argument, you must give a string
        that contains the text. Example:
        <UBQ><TG>$INSERT TEXT="hugo was here!"</TG></UBQ>
        inserts the text "<CODE>hugo was here</CODE>". Of course, this does not
        make much sense.<TG>$INSERT TEXT="..."</TG> is suggested to be used
        with attribute values. Example:
        <UBQ><TG>$INSERT TEXT=&lt;href&gt;</TG></UBQ>
        inserts the value of the macro-attribute <CODE>href</CODE>.

<SPCTAG NAME="let" TITLE="Set attributes">

    You can create a global attribute and pass a value to it via

    <BLOCKQUOTE>
    <TG>$LET <I><A HREF=":macro/attrib.html">attribute</A></I></TG>
    </BLOCKQUOTE>

    The new attribute is globaly and must have a value passed when
    created.

<SPCTAG NAME="macro" TITLE="Define macros">
    I think, this feature needs an <A HREF=":macro/macros.html">
    own page</A>...

<SPCTAG NAME="dontparse" TITLE="Don't parse section">

    If you created some perverted HTML source or use features
    <hsc> can't handle (which is theoretically impossible), you
    can keep of <hsc> from parsing this section by surrounding it with
    <TG>| ... |</TG>. Of course, this is a dirty hide-out and should
    be used only for special cases.
<*
    <BLOCKQUOTE>
    <TG>$LET <I><A HREF=":macro/attrib.html">attribute</A></I></TG>
    </BLOCKQUOTE>*>

<SPCTAG NAME="defent" TITLE="Define a new entity">
<SPCTAG NAME="deftag" TITLE="Define a new tag">

At the moment, these tags are undocumented. See <FILE>hsc.prefs</FILE> for
a small description.<P>

Usage of these tags only allowed inside <FILE>hsc.prefs</FILE>.

<P>

</WEBPAGE>
