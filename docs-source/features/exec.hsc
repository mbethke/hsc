<WEBPAGE chapter="hsc - " title="Execute shell-commands"
         PREV="rplcent.html" NEXT=":options.html">

<P>You can invoke shell-commands during processing of a hsc-source. The
output of the command can be assigned to an special attribute or
immediatly being included into the document. This can be useful to 
include data from external applications (like databases), prepared
by scripts (eg. Rexx), which of course you have to write yourself.</P>

<P>This functionality is provided by the hsc-tag <TG>$exec</TG></P>

<H2>Possible attributes</H2>

<DL>
<DT><CODE>COMMAND="</CODE><I>shell-command</I><CODE>"</CODE>
<DD>Specifies command to be executed. This attribute is required

<DT><CODE>INCLUDE</CODE>
<DD>If this attribute occures, the output the command sends to 
    <stdout> (or the filename specified with <CODE>FILE</CODE>)
    is included in the document.

<DT><CODE>ATTRIBUTE=</CODE><I>destination_attribute_name</I>
<DD>Using this attribute, you can select an attribute you've
    created before using <ln_define> to store the data the
    command sends to <stdout>.

<DT><CODE>FILE="</CODE><I>output-filename</I><CODE>"</CODE>
<DD>Specifies filename where the command is expected to store
    the output. The filename is relative to the source-directory.
    If this attribute is not set, <hsc> will asume the command
    to output to <stdout> and will redirect the output to a 
    temporary file. This attribute is only useful if you have
    also set at least one of <CODE>INCLUDE</CODE> or 
    <CODE>ATTRIBUTE</CODE>

<DT><CODE>TEMPORARY</CODE>
<DD>This attribute has the same meaning as within <ln_include>. Note that
    it can be reasonable to specify it, even if you set a specific 
    output-file, and not only for temporary-files created by <hsc>.

<DT><CODE>REMOVE="auto|on|off"</CODE> (default: <CODE>auto</CODE>)
<DD>This attribute tells <hsc> what to do with the output-file after
    all data have been read. If you set it to 
    <CODE>REMOVE="off"</CODE>, the file will be left untouched. If no one
    else is going to remove it, it will continue to exist until 
    doomsday or the next head-crash.

    <P>If you didn't specifiy a output-filename (using <CODE>FILE</CODE>), 
    this  can clutter your temporary-directory (usually "<FILE>t:</FILE>")
    with loads of files having strange names like 
    "<FILE>hsc0x321764.tmp</FILE>".</P>
    <P>To avoid this, you can set <CODE>REMOVE="on"</CODE>, so the output-file
    will always be removed.</P>

    <P>By default, <CODE>REMOVE="auto"</CODE> is set. This will remove the
    file, if <CODE>TEMPORARY</CODE> has been enabled, and - if 
    <CODE>INCLUDE</CODE> has been activated, too - no messages
    showed up during including the file.</P>

    <STRONG>Note</STRONG>: Never trust Freeware-programmers when they
    start removing your files. So never use <TG>$exec</TG> on important
    data without any backup, and always use <CODE>REMOVE="off"</CODE>
    if your data are not really completely temporary.
</DL>

<P>Additionally, you can use all attributes of <TG>$include</TG> that change
the appearence of the included data, like <CODE>PRE</CODE>, 
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
    this list into the current document. The ouptput of the
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

<H2>A more complex example</H2>

If you want to create a portable hsc source to insert a listing of a
directory, you could try something like this:

<$include FILE="exmpl/exec.hsc" SOURCE PRE>

And the data created by this code sequence would look like this:

<$include FILE="exmpl/exec.hsc">

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

<P>Another problem will occure, if you redirect the output yourself
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

<P>The behavior of this call is system-dependent, but there's a general
statement on this topic: it most likely won't work.</P>
</WEBPAGE>
