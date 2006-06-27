<DOCPAGE chapter="hsc - " title="Execute Shell Commands"
         PREV="prefs.html"
         NEXT=":project/index.html">

<P>You can invoke shell-commands during processing of a hsc source. The
output of the command can be assigned to an special attribute or
immediately being included into the document. This can be useful to
include data from external applications (like databases), prepared
by scripts (eg. Rexx), which of course you have to write yourself.</P>

<P>This functionality is provided by the hsc tag <TG>$exec</TG></P>

<H2>Possible Attributes</H2>

<DL>
<DT><CODE>COMMAND:string/required</CODE>
<DD>Specifies command to be executed. This attribute is required

<DT><CODE>INCLUDE:bool</CODE>
<DD>If this attribute occurs, the output the command sends to
    <stdout> (or the filename specified with <CODE>FILE</CODE>)
    is included in the document.

<DT><CODE>ATTRIBUTE:string</CODE>
<DD>Using this attribute, you can select an attribute you've
    created before using <ln-define> to store the data the
    command sends to <stdout>.

<DT><CODE>FILE:string</CODE>
<DD>Specifies filename where the command is expected to store
    the output. The filename is relative to the source-directory.
    If this attribute is not set, <hsc> will assume the command
    to output to <stdout> and will redirect the output to a 
    temporary file. This attribute is only useful if you have
    also set at least one of <CODE>INCLUDE</CODE> or 
    <CODE>ATTRIBUTE</CODE>

<DT><CODE>TEMPORARY:bool</CODE>
<DD>This attribute has the same meaning as within <ln-include>. Note that
    it can be reasonable to specify it, even if you set a specific 
    output-file, and not only for temporary-files created by <hsc>.

<DT><CODE>REMOVE:enum("auto|on|off")="auto"</CODE>
<DD>This attribute tells <hsc> what to do with the output-file after
    all data have been read. If you set it to 
    <CODE>REMOVE="off"</CODE>, the file will be left untouched. If no one
    else is going to remove it, it will continue to exist until 
    doomsday or the next head-crash.

    <P>If you didn't specify an output filename (using <CODE>FILE</CODE>),
    this  can clutter your temporary-directory (usually "<FILE>t:</FILE>")
    with loads of files having strange names like 
    "<FILE>hsc0x321764.tmp</FILE>".</P>
    <P>To avoid this, you can set <CODE>REMOVE="on"</CODE>, so the output-file
    will always be removed.</P>

    <P>By default, <CODE>REMOVE="auto"</CODE> is set. This will remove the
    file, if <CODE>TEMPORARY</CODE> has been enabled, and - if 
    <CODE>INCLUDE</CODE> has been activated, too - no messages
    showed up during including the file.</P>

    <STRONG>Note</STRONG>: Never trust freeware-programmers when they
    start removing your files. So never use <TG>$exec</TG> on important
    data without any backup, and always use <CODE>REMOVE="off"</CODE>
    if your data are not really completely temporary.
</DL>

<P>Additionally, you can use all attributes of <TG>$include</TG> that change
the appearance of the included data, like <CODE>PRE</CODE>,
<CODE>SOURCE</CODE> etc. - Of course, this only makes sense if the
<CODE>INCLUDE</CODE>-attribute has been set.</P>

<P>You can set both <CODE>INCLUDE</CODE> and <CODE>ATTRIBUTE</CODE> with
one call to <TG>$exec</TG>.</P>

<H2>Examples</H2>

<DL>
<DT><TG>$exec COMMAND="dpaint"</TG>
<DD>Invokes Deluxe-Paint. Now you can paint whatever you like.
    When you quit the program, <hsc> will continue to process 
    it's source. Note that this is not a very useful example.
<DT><TG>$exec COMMAND="list #?.hsc" TEMPORARY INCLUDE SOURCE PRE</TG>
<DD>List all hsc-sources in the current directory and include
    this list into the current document. The output of the
    <KBD>list</KBD>-command is redirected to a temporary file,
    which is removed afterwards.
<DT><TG>$exec COMMAND="echo Hello! &gt;echo.tmp" TEMPORARY INCLUDE FILE="echo.tmp"</TG>
<DD>List all hsc-sources in the current directory and include
    this list into the current document. The output of the
    <KBD>list</KBD>-command is obviously redirected to a file 
    called <FILE>echo.tmp</FILE>, which <hsc> tries to read because of
    the <CODE>FILE</CODE>-attribute.
<DT><TG>$define output:string</TG><BR>
    <TG>$exec COMMAND="list #?.hsc" ATTRIBUTE="output"</TG>
<DD>List all *.hsc-files in current directory and assign output to
    the attribute <CODE>output</CODE>; no data are included into the 
    current document.
</DL>

<H2>A More Complex Example</H2>

If you want to create a portable hsc source to insert a listing of a
directory, you could try something like this:

<$include FILE="exmpl/exec.hsc" SOURCE PRE>

And the data created by this code sequence would look like this:

<$include FILE="exmpl/exec.hsc">

<H2>Computing Complex Data</H2>

<$define snc.rex:string="StripNastyChars.rexx">
<$define snc.hsc:string="StripNastyChars.hsc">
<$macro snc.rex><file><(snc.rex)></file></$macro>
<$macro snc.hsc><file><(snc.hsc)></file></$macro>

<P>As <hsc> is only a simple preprocessor, but not a programming language, it
can not do certain things. For example, there are no string functions. But
you can use external commands to gain the same result.</P>

<P>The Rexx-script <grafflwerk href=(snc.rex)> together with
<grafflwerk href=(snc.hsc)> shows how to strip all but alpha numeric
characters from a given string.</P>

To better understand what <snc.rex>, you can first try to call it from
the command line. Set your working directory to <grafflwerk>, and
type:

<PRE>rx <(snc.rex)> bla:sülz:fasel</PRE>
results in
<PRE>blaslzfasel</PRE>

If you tried this yourself, you maybe noticed that the Rexx-script does not
output a linefeed at the end of line. This is done by avoiding to use

<PRE>SAY stripped</PRE>

to display the converted data. Instead, you will find a

<PRE>call WriteCH(stdout, stripped)</PRE>

This simply is because <snc.hsc> assigns the output of this script to
a target attribute called <CODE>clean-data</CODE> by means of

<$source pre>
<$exec command=("rx StripNastyChars.rexx " + nasty-data) ATTRIBUTE=clean-data>
</$source>

Because of obvious reasons, the linefeed character created by
<CODE>SAY</CODE> would be unwanted in the value of the target
attribute. Therefor, the interesting part of the created HTML document
will look like:

<PRE>
Converted "This#Text|Contains!Ñâ§ïÿ/Characters"
to        "ThisTextContainsCharacters".
</PRE>

It should be easy for you to put this functionality in a macro.

<H2>Problems</H2>

<P>Technically speaking, <hsc> redirects the output of you command by
appending a " <CODE>&gt;</CODE><I>filename</I>" to the command-string.
For example, if you called</P>

<$source PRE>
<$exec COMMAND="list #?.hsc" INCLUDE>
</$source>
<P><hsc> will invoke the command like</P>

<$source PRE>
list #?.hsc >hsc0x321764.tmp
</$source>

<P>with <FILE>hsc0x321764.tmp</FILE> chosen as temporary output file by
<hsc>. If you specify the <CODE>FILE</CODE> attribute, you tell <hsc>
where to expect the output. But now you are responsible to redirect 
it yourself. For example,</P>

<$source PRE>
<$exec COMMAND="list #?.hsc >list.tmp"   INCLUDE FILE="list.tmp">
<$exec COMMAND="list #?.hsc TO list.tmp" INCLUDE FILE="list.tmp">
</$source>

<P>will both create an output in <FILE>list.tmp</FILE>, where <hsc> will
also look for it. But the next one will fail:</P>

<$source PRE>
<$exec COMMAND="list #?.hsc >list.tmp" FILE="tmp.list">
</$source>

<P>The reason is obvious: The command outputs to <FILE>list.tmp</FILE>,
but <hsc> tries to read <FILE>tmp.list</FILE>, which most likely will
not exist.</P>

<P>Another problem will occur, if you redirect the output yourself
within the command call, but do not specify the 
<CODE>FILE</CODE>-attribute: <hsc> will try to redirect the output twice: 
For example,</P>

<$source PRE>
<$exec COMMAND="list #?.hsc >list.tmp" INCLUDE>
</$source>

<P>will try to invoke something like</P>

<$source PRE>
list #?.hsc >list.tmp >hsc0x321764.tmp
</$source>

<P>The behavior of this call is system dependent, but there's a general
statement on this topic: it most likely won't work.</P>
</DOCPAGE>
