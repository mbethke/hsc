<WEBPAGE chapter="hsc - " title="Invoking Hsc"
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
<DT>
<$stripws>
<$IF COND=(SET NAME)>
    <A NAME=(Name)>
<$ELSE>
    <A NAME=(Title)>
</$IF>
<$stripws>
<KBD><(title)></KBD></A>
<$IF COND=(ONLYDEFTAG)>
(for use within <ln_deftag> only)
</$IF>
<DD>
</$MACRO>


This chapter explains which options and switches you can pass to
<hsc>, and which <A HREF="#exitcodes">exit codes</A> it might return
to the CLI. For easier understanding, related options are collected in
groups.

<P>Although for most descriptions there are short examples available,
this is intended to be a reference only. For several options, you can
find more detailed explanations in other sections of this manual.</P>

<H2><A NAME="general.rules">General Rules For Options</A></H2>

<P>As <hsc> is an tool developed under AmigaOS and I never liked
the cryptic "<KBD>cmd -cvf -osepp.x</KBD>"-way of invoking commands coming
from Unix, <hsc> uses a syntax like most commands of AmigaOS;
even for ports on other operating-systems. (<TechNote> AmigaOS provides
an OS-function called <CODE>ReadArgs()</CODE> for 
argument-parsing.)</P>

<P>All options and switches are case-insensitive. If you start <hsc>
without any options, a short help message will be displayed.</P>

<P>Options can be followed by modifiers, which specify things such as
the type of the option.  Modifiers are specified by following the
option with a '/' and a single character modifier.  Multiple modifiers
can be specified by using multiple '/'s.  Valid modifiers are:</P>

<DL>
<DT><KBD>/S</KBD> - Switch.  
<DD>This is considered a boolean variable, and will be
   set if the option name appears in the command-line.  
<DT><KBD>/K</KBD> - Keyword.  
<DD>This means that the option will not be filled unless
    the keyword appears.  For example if the template is 
    <qq><KBD>Name/K</KBD></qq>,
    then unless <qq><KBD>Name={string}</KBD></qq> 
    or <qq><KBD>Name {string}</KBD></qq> appears in the
    command line, Name will not be filled.

<DT><KBD>/N</KBD> - Number.  
<DD>This parameter is considered a decimal number.
    If an invalid number is specified, an error will be returned.  
<*
        /T - Toggle.  This is similar to a switch, but when specified causes
             the boolean value to "toggle".  Similar to /S.*>
<DT><KBD>/A</KBD> - Required.
<DD>This keyword must be given a value during command-line
    processing, or an error is returned.
<*
        /F - Rest of line.  If this is specified, the entire rest of the line
             is taken as the parameter for the option, even if other option
             keywords appear in it.*>
<DT><KBD>/M</KBD> - Multiple strings.
<DD>This means the argument will take any number strings.
    Any arguments not considered to be part of another option will be 
    added to this option.  
</DL>

<H2><A NAME="exitcodes">Exit Code</A></H2>

<P>The exit-code <hsc> returns to the CLI depends on the messages and errors
that have been displayed during runtime.</P>

<P>The version for AmigaOS returns 
<UL>
<LI><CODE>0</CODE> on success,
<LI><CODE>5</CODE> if warnings occurred,
<LI><CODE>10</CODE> if errors showed up (also for errors in arguments) and
<LI><CODE>20</CODE> if fatal errors happened (also for out-of-resources).
</UL>

For other systems, values like <CODE>0/0/1/2</CODE> are used instead.

<H2>Specifying Input and Output</H2>

<DL>
<EXPLAN TITLE="FROM/M" NAME="from">
    Specifies the input filename. To use <stdin> as
    input file, the pseudo filename <CODE>STDIN</CODE> (case sensitive)
    should be specified. If you specify more than one file,
    the last file is taken as main source, and all previous files are
    included before it. This is useful to include macro-definitions
    within the command-line instead of an <ln-include> in the hsc-source.
<EXPLAN TITLE="TO/K" NAME="to">
    Specifies the output filename or the destination directory.
    If the argument ends with a "<KBD>/</KBD>" (or also "<KBD>:</KBD>"),
    it is interpreted as a directory. In this case, the name of the
    output file is created from the name of the input file, with an
    extension specified (see below). If no output file is set, <stdout> 
    is used instead.<BR>
    For details, read the section about
    <A HREF="fileargs.html">File Options</A>
    
<EXPLAN TITLE="EXTENSION/K" NAME="extension">
    If you specify a directory for output, the output-filename
    will depend on the input-filename and the extension specified
    with this options. If you do not set an extension, <qqc>html</qqc>
    will be used. A <period> will be added automatically.
    Setting <KBD>EXTENSION=.</KBD> will omit this and result into exactly
    the same filename as the input.
    If you specify a complete filename for output, this option has no
    effect.
</DL>

<**************************************************************************>
<H2>Message Options</H2>
<**************************************************************************>

<DL>
<EXPLAN TITLE="IGNORE=IGN/K/M" NAME="ignore">

    <p>Ignore message number or class. Usually, <KBD>IGNORE</KBD>
    needs a numeric argument and can occur multiple times in the
    command line.</p>

    Additionally, you can specify some special values, which will
    ignore whole groups of messages: <KBD>ALL</KBD>, <KBD>NOTE</KBD>,
    <KBD>STYLE</KBD>, <KBD>PORTABILITY</KBD> and <KBD>JERK</KBD>.

    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.

<EXPLAN TITLE="ENABLE=ENA/K/M" NAME="enable">

    This is the opposite to <KBD>IGNORE</KBD> and can be used to
    enable certain messages. You can also use the same special values
    as with <kbd>IGNORE</kbd>.

    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.


<EXPLAN TITLE="MSGMODE/K" NAME="msgmode">
    Set base mode for syntax check. Legal values are <KBD>PEDANTIC,
    NORMAL</KBD> and <KBD>RELAXED</KBD>. Default is <KBD>NORMAL</KBD>.

    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.

<EXPLAN TITLE="MSGFILE/K" NAME="msgfile">
    Redirects messages to a file. By default, <stderr> is used.

    See <a href="messages.html#msgformat">Redirecting Messages and
    Changing the Message Format</a> for more information.

<EXPLAN TITLE="MSGFORMAT/K" NAME="msgformat">
    Describes how messages should be displayed.

    See <a href="messages.html#msgformat">Redirecting Messages and
    Changing the Message Format</a> for more information.

</DL>

<**************************************************************************>
<H2>Configuration</H2>
<**************************************************************************>

<DL>
<EXPLAN TITLE="DEFINE=DEF/K/M" NAME="define">
    Defines a global attribute. The syntax is equal to <ln-define>, exept
    that you need no quotes surrounding the value.<BR>
    Example: <KBD>DEFINE "hugo:string=hugo was here"</KBD>

<EXPLAN TITLE="INCLUDEDIR=IDIR/K/M" NAME="includedir">
    By default, only the current directory is scanned for files
    to be included (using the CLI option <op-from> or the
    special tags <ln-include> or <ln-exec>). This option adds a
    directory to the list of directories to search for include
    files.

<EXPLAN TITLE="PREFSFILE/K" NAME="prefsfile">
    Specifies filename for <A HREF=":features/prefs.html">sytax-definition</A>
    to be used, if you don't want to use the defaults.

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
    Stores some information about the document currently processing
    in a seperate file to extend functionality. Read the section
    about <A HREF="project/index.html">Project management</A> for more
    details. Note that all documents within a project should use the
    same project-file.

<EXPLAN TITLE="SERVERDIR/K" NAME="serverdir">
    With this you can tell <hsc> where it should look for files
    referenced with a server relative URI. The directory has to be
    specified as a <STRONG>native</STRONG> filename of your OS.
    Example:
    <KBD>SERVERDIR=aws:home</KBD> might work fine under AmigaOS,
    <KBD>SERVERDIR=/user/sepp/public_html</KBD> could be used
    on an Unixoid system.

</DL>

<**************************************************************************>
<H2>Options Affecting The Output</H2>
<**************************************************************************>

<DL>

<EXPLAN TITLE="GETSIZE/S" NAME="getsize">
    <A HREF="features/getsize.html">Get width and height of images</A>
     and set corresponding attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> with these values.

<EXPLAN TITLE="ICONBASE/K" NAME="iconbase">
    Replace all icon entities by images that must be available from
    the base URI specified. If this URI contains an <asterisk>, it 
    will be substituted by the icon name. For
    example, with <KBD>ICONBASE=:icons/*.gif</KBD>, the entity
    <CODE>&amp;back;</CODE>
    is replaced by <TG>IMG SRC=":icons/back.gif" ALT="back"</TG>. Note
    that this argument does not denote a directory, but an URI. It's
    recommended to use a <fe_prjuri>.

<EXPLAN TITLE="QUOTESTYLE/K" NAME="quotestyle">
    Specifies which kind of quotes should be used to surround 
    attribute-values. <KBD>KEEP</KBD> keeps the user-kind, 
    <KBD>DOUBLE</KBD> tries to assign double quotes 
    (eg. <CODE>"value"</CODE>), 
    <KBD>SINGLE</KBD> tries to assign single quotes 
    (eg. <CODE>'value'</CODE>) and <KBD>NONE</KBD> will remove
    quotes if possible.

<EXPLAN TITLE="RPLCENT/S" NAME="rplcent">
    <A HREF="features/rplcent.html">Replace special characters</A> with
    their entities. For instance, <qq><CODE>&uuml;</CODE></qq> turns into
    <qq><CODE>&amp;uuml</CODE></qq>).

<EXPLAN TITLE="RPLCQUOTE/S" NAME="rplcquote">
    Replace double-quotes (<qq><CODE>"</CODE></qq>) found in the text by 
    <qq><CODE>&amp;quot;</CODE></qq>. This does not affect quotes used inside
    attribute-values.

<*<EXPLAN TITLE="SMARTENT/S" NAME="smartent">
    Replace special characters <qq><CODE>&amp;</CODE></qq>, 
    <qq><CODE>&lt;</CODE></qq>
    and <qq><CODE>&gt;</CODE></qq> with their entities
    (<CODE>&amp;amp;, &amp;lt;</CODE> and <CODE>&amp;gt;</CODE>) if they are
    surrounded by white-spaces.*>

</DL>

<**************************************************************************>
<H2>Stripping Data From The Output</H2>
<**************************************************************************>

<DL>
<EXPLAN TITLE="COMPACT/S" NAME="compact">
    <A HREF="features/strip.html">Strip redundant linefeeds
    and white-spaces</A> from output.

<EXPLAN TITLE="STRIPBADWS/S" NAME="stripbadws">
    Strip bad white spaces that would have caused <ln-msg id="78">
    or <ln-msg id="79">; this will not strip those which cause
    <ln-msg id="47">. The main advantage of this switch is that
    it allows you to write well structured macros.

<EXPLAN TITLE="STRIPCOMMENT/S" NAME="stripcomment">
    Strip all SGML-comments from input.

<EXPLAN TITLE="STRIPEXTERNAL/S" NAME="stripexternal">
    Strip tags which refer to an external URI.

<EXPLAN TITLE="STRIPTAGS/K" NAME="striptags">
    Strip some specific tags from document. You can specify a list of tags
    which is a string,
    that contains all tags that should be stripped, separated by a
    <vbar>.<BR>
    See also <op-stripcomment> and <op-stripexternal>.
</DL>

<**************************************************************************>
<H2>Miscellaneous Options</H2>
<**************************************************************************>

<DL>

<EXPLAN TITLE="HELP=?=-h=--help/S" NAME="help">
    Display a short help message and exit.

<EXPLAN TITLE="LICENSE/S" NAME="license">
    Display a short version of the <A HREF="../COPYING">GNU General
    Public License</A> and exit.

<EXPLAN TITLE="-DEBUG/S" NAME="-debug">
    If you've compiled <hsc> in debug-mode, it will send lots of
    information to <stderr>. Otherwise, this option is
    equivalent to <KBD>STATUS=FULL</KBD>.

<EXPLAN TITLE="STATUS/K" NAME="status">
    Display several status messages (output goes to stderr). Legal
    flags are:
    <UL COMPACT>
    <LI><KBD>QUIET</KBD>: no status messages
    <LI><KBD>LINE</KBD>: display name and line of file currently processing.
        Names of files included are also displayed. After processing a file,
        the filename and the total number of lines remain visible.
    <LI><KBD>VERSION</KBD>: display program title and version
    <LI><KBD>VERBOSE</KBD>: enable verbose status messages; this includes
        the information displayed during the  processing of <ln-exec>
        and some details about images, if <op-getsize> has also been
        enabled.
    <LI><KBD>FULL</KBD>: enable all status messages
    </UL>
    The above flags can be combined by separating them with a <vbar>,
    eg <KBD>"STATUS=line|version"</KBD>, except for <KBD>QUIET</KBD>
    and <KBD>FULL</KBD>

</DL>

</WEBPAGE>
