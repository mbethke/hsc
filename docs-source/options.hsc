<WEBPAGE chapter="hsc - " title="Options"
    PREV="install.html"
    NEXT="examples.html">

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string ONLYDEFTAG:bool>
<** create named reference **>
<$IF SET <NAME>>
<A NAME=<NAME>>
<$ELSE>
<A NAME=<TITLE>>
</$IF>
<** create heading **>
<DT><STRONG><$INSERT TEXT=<title>></STRONG>
</A>
<$IF SET <ONLYDEFTAG>>
(for use within
<A HREF=":features/spctags.html#deftag"><TG>$DEFTAG</TG></A>
only)
</$IF>
<DD>
</$MACRO>

<*
** macro to format example for IGNORE
*>
<$MACRO exmp_ign VALUE:string/r>
<DT><KBD><$INSERT TEXT=<value>></KBD>
<DD>
</$MACRO>

All options and swiches are case-insensitive. If you start <hsc>
without any options, a short help message will be displayed.<P>

<hsc> understands the following options:<P>

<DL>
<EXPLAN TITLE="Help">
    Display a short help message.
<EXPLAN TITLE="[from=]<I>infile</I>" NAME="From">
    Specifies the input filename. To use <CODE>stdin</CODE> as
    input file, see PipeIn.
<EXPLAN TITLE="[to=]<I>outfile</I>" NAME="To">
    Specifies the output filename. If no output file is given, <CODE>stdout</CODE>
    is used instead.
<EXPLAN TITLE="DestDir=<I>destination_directory</I>" NAME="DestDir">
    Specifies the destination directory for output file.
<EXPLAN TITLE="Errfile=<I>errfile</I>" NAME="ErrFile">
    Redirects error output to a file. By default, <CODE>stderr</CODE> ist used.
<EXPLAN TITLE="Include=INC" NAME="Include">
    File(s) to include before main-input file is read. Useful to include
    macro-definitions without an <A HREF="features/spctags.html#include">
    <TG>$INCLUDE FILE=".."</TG></A>. Recommended to be used within <makefile>s.
<EXPLAN TITLE="Ignore=IGN" NAME="Ignore">
    Ignore warning message number. Usually, <KBD>IGNORE</KBD> needs a numeric
    argument and 7can occure multiple. Example:
<PRE>
        IGNORE=21 IGN=18 IGN 3         (ignore warnings #21, #18. #3)
</PRE>
    Additionally, you can give <KBD>IGNORE</KBD> some special values:
    <DL>
    <EXMP_IGN VALUE="ALL">
        Ignore all non-error messages
    <EXMP_IGN VALUE="NOTES">
        Ignore all notes
    <EXMP_IGN VALUE="BADSTYLE">
        Ignore all bad style warnings
<*    <EXMP_IGN VALUE="WARNINGS">
        Ignore all non-error messages
*>
    </DL>
    All these special values add new messages to the list of messages
    to be ignored, but do not remove any.
</DL>

<H1>Switches</H1>
<DL>
<EXPLAN TITLE="CheckUri">
    <A HREF="features/checkuri.html">Check existence</A> of local URIs
<EXPLAN TITLE="PipeIn">
    Read input file from <CODE>stdin</CODE>
<EXPLAN TITLE="RplcEnt">
    <A HREF="features/rplcent.html">Replace special charaters</A> with its entity (eg "&uuml;"
    becomes "&amp;uuml").
<EXPLAN TITLE="SmartEnt">
    Replace special charaters "&amp;", "&lt;" and "&gt;" with its entity
    (<CODE>&amp;amp;, &amp;lt; and &amp;gt;</CODE>) if they are
    surrounded by white-spaces. Also replace quotes by
    <CODE>&amp;quot;</CODE>.
<EXPLAN TITLE="Status">
    Display file and line number during conversion. Included files
    are also displayed. After processing a file, the filename and
    the total number of lines remain visible. (output goes to stderr.)
<EXPLAN TITLE="Verbose">
    More verbose status output. Enables the Status-switch.
</DL>

<P>

</WEBPAGE>

