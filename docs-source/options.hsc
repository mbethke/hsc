<WEBPAGE chapter="hsc - " title="Options and switches"
    PREV="install.html"
    NEXT="examples.html"
    QTEXT=("shoulder, bicep, elbow, arm<BR>"
          +"forearm, thumb, wrist, knuckle, palm<BR>"
          +"middle, pinky, index, ring<BR>"
          +"dinner bell dinner bell ding")
    QAUTHOR='They Might Be Giants, "Dinner Bell"'>

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string ONLYDEFTAG:bool>
<** create named reference **>
<$IF COND=(SET NAME)>
<A NAME=(Name)><$ELSE>
<A NAME=(Title)></$IF>
<DT><** create heading **>
<KBD><(title)></KBD></A>
<$IF COND=(ONLYDEFTAG)>
(for use within <ln_deftag> only)
</$IF>
<DD>
</$MACRO>

<*
** macro to format example for IGNORE
*>
<$MACRO exmp_ign VALUE:string/r>
<DT><KBD><(value)></KBD>
<DD>
</$MACRO>

<P>All options and swiches are case-insensitive. If you start <hsc>
without any options, a short help message will be displayed.</P>

<P>Options require an additional argument, for example a file.
All of the below terms are treated as the same:</P>
<PRE>
        FILE hugo.hsc
        file hugo.hsc
        file=hugo.hsc
</PRE>

<P>Switches are enabled if their keyword is detected within
the command-line.</P>

<H2>Options</H2>

<DL>
<EXPLAN TITLE="HELP" NAME="help">
    Display a short help message and exit.
<EXPLAN TITLE="LICENSE" NAME="license">
    Display a short version of the <A HREF="../COPYING">GNU General
    Public License</A> and exit.
<EXPLAN TITLE="[FROM=]<I>inputfile_name</I>" NAME="from">
    Specifies the input filename. To use <stdin> as
    input file, the peudo-filename <CODE>STDIN</CODE> (case sensitive)
    should be specified. If you specify more than one file,
    the last file is taken as mainfile, and all previous files are
    included before it. This is useful to include macro-definitions
    within the command-line instead of an <ln_include> in the hsc-source.
<EXPLAN TITLE="TO=<I>output_filename|destination_directory</I>" NAME="to">
    Specifies the output filename or the destination directory.
    If the argument ends with a "<KBD>/</KBD>" (or also "<KBD>:</KBD>"),
    it is interpreted as a directory. In this case, the name of the
    outputfile is created from the name of the input file, with an
    extension specified (see below).<BR>
    If no output file is set, <stdout> is used instead.
<EXPLAN TITLE="EXTENSION=<I>output_extension</I>" NAME="extension">
    If you specify a directory for output, the output-filename
    will depend on the input-filename and the extension specified
    with this options. If you do not set an extension, "<CODE>html</CODE>"
    will be used. A period (`<CODE>.</CODE>') will be added automatically.
    If you specify a complete filename for output, this option has no
    effect.
<EXPLAN TITLE="MSGFILE=<I>message_filename</I>" NAME="msgfile">
    Redirects messages to a file. By default, <stderr> ist used.
<EXPLAN TITLE="MSGFORMAT=<I>format_description</I>" NAME="msgformat">
    Describes how messages should be displayed. Legal placeholders:
    <PRE>
        %f      filename
        %x, %y  location in file (column, line)
        %c      message class
        %i      message id (numeric)
        %m      message text
        %n      inserts a linefeed
    </PRE>
    Example: To get GCC-like messages, use
    <CODE>MSGFORMAT="%f:%y: %c %i: %m"</CODE>.
<EXPLAN TITLE="PRJFILE=<I>project_filename</I>" NAME="prjfile">
    Stores some information about the document currently processing
    in a seperate file to extend functionality. Read the section
    about <A HREF="project/index.html">Project managment</A> for more
    details. Note that all documents within a project should use the
    same project-file.
<*
<EXPLAN TITLE="DEFINE" NAME="define">
    Defines a global attribute. The syntax is equal to <ln_define>, exept
    that you need no quotes surrounding the value.<BR>
    Example: <CODE>DEFINE "hugo:string=hugo was here"</CODE>
*>
<EXPLAN TITLE="IGNORE" NAME="ignore">
    Ignore warning message number. Usually, <KBD>IGNORE</KBD> needs a numeric
    argument and can occure multiple. Example:
<PRE>
        IGNORE=21 IGN=18 IGN 3         (ignore warnings #21, #18, #3)
</PRE>
    Additionally, you can give <KBD>IGNORE</KBD> some special values:
    <DL>
    <EXMP_IGN VALUE="ALL">
        Ignore all non-error messages
    <EXMP_IGN VALUE="NOTES">
        Ignore all notes
    <EXMP_IGN VALUE="BADSTYLE">
        Ignore all bad style warnings
    </DL>
    All these special values add new messages to the list of messages
    to be ignored, but do not reset any <KBD>IGNORE</KBD>s passed before.
<EXPLAN TITLE="MODE" NAME="mode">
    Set base mode for syntax check. Legal values are <KBD>PEDANTIC,
    NORMAL</KBD> and <KBD>RELAXED</KBD>. Default is <KBD>NORMAL</KBD>.
    This only disables several messages and warnings from the beginning.
    Any <KBD>IGNORE</KBD>s passed before are reseted.
<EXPLAN TITLE="STATUS" NAME="status">
    Display several status messages (output goes to stderr). Legal
    flags are:
    <UL COMPACT>
    <LI><KBD>QUIET</KBD>: no status messages
    <LI><KBD>LINE</KBD>: display name and line of file currently processing
        Included files are also displayed. After processing a file,
        the filename and the total number of lines remain visible.
    <LI><KBD>VERSION</KBD>: display program title and version
    <LI><KBD>VERBOSE</KBD>: enable verbose status messages; this includes
        the information displayed during the  processing of <ln_exec>
        and some details about images, if <op_getsize> has also been
        enabled.
    <LI><KBD>FULL</KBD>: enable all status messages
    </UL>
    The above flags can be combined by separating them with a "<KBD>|</KBD>",
    eg <KBD>STATUS=line|version</KBD>, exept for <KBD>QUIET</KBD>
    and <KBD>FULL</KBD>
<EXPLAN TITLE="STRIPTAGS=<I>tag_list</I>" NAME="striptags">
    Strip some specific tags from document. <I>tag_list</I> is a string,
    that contains all tags that should be stripped, sepparated by a
    vertical bar ("<KBD>|</KBD>").<BR>
    See also <op_stripcomment> and <op_stripexternal>.
<EXPLAN TITLE="ICONBASE=<I>base_uri</I>" NAME="iconbase">
    Replace all icon-entities by images that must be available in
    <I>base_uri</I>. For
    example, with <CODE>ICONBASE=:icons</CODE>, the entity
    <CODE>&amp;back;</CODE>
    is replaced by <TG>IMG SRC=":icons/back.gif" ALT="back"</TG>. Note
    that this argument does not denote a directory, but an URI. It's
    recommended to use an <fe_absuri>. If you don't want to use 
    <CODE>*.gif</CODE>-files for your icon-entities, you can use an asterisk
    (`<CODE>*'</CODE>) to be replaced by the name of the icon-entity;
   eg. <CODE>ICONBASE=:icons/*.png</CODE>.
</DL>

<H2>Switches</H2>
<DL>
<EXPLAN TITLE="COMPACT" NAME="compact">
    <A HREF="features/strip.html">Strip redundant linefeeds
    and white-spaces</A> from output.
<EXPLAN TITLE="-DEBUG" NAME="-debug">
    If you've compiled <hsc> in debug-mode, it will send lots of
    information to <stderr>. Otherwise, this option is
    equivalent to <KBD>STATUS=FULL</KBD>.
<EXPLAN TITLE="GETSIZE" NAME="getsize">
    <A HREF="features/getsize.html">Get width and height of images</A>
     and set corresponding attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> with these values;
<EXPLAN TITLE="RPLCENT" NAME="rplcent">
    <A HREF="features/rplcent.html">Replace special characters</A> with
    its entity (eg `<CODE>&uuml;</CODE>' becomes `<CODE>&amp;uuml</CODE>').
<EXPLAN TITLE="RPLCQUOTE" NAME="rplcquote">
    Replace double-quotes (`<CODE>"</CODE>') found in the text by 
    <CODE>&amp;quot;</CODE>. This does not affect quotes used inside
    attribute-values.
<EXPLAN TITLE="SMARTENT" NAME="smartent">
    Replace special characters `<CODE>&amp;</CODE>", `<CODE>&lt;</CODE>'
    and `<CODE>&gt;</CODE>' with their entities
    (<CODE>&amp;amp;, &amp;lt;</CODE> and <CODE>&amp;gt;</CODE>) if they are
    surrounded by white-spaces. 
<EXPLAN TITLE="STRIPCOMMENT" NAME="stripcomment">
    Strip all SGML-comments from input.
<EXPLAN TITLE="STRIPEXTERNAL" NAME="stripexternal">
    Strip tags which refer to an external URI.
</DL>

</WEBPAGE>
