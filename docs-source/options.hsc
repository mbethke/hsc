<WEBPAGE chapter="hsc - " title="Invoking hsc"
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

This section covers which
<A HREF="#options">options</A> and
<A HREF="#switches">switches</A> you can pass to <hsc>, 
and which <A HREF="#exitcodes">exit codes</A> are returned to CLI.

<A NAME="general.rules"><H2>General rules for options</H2></A>

<P>As <hsc> is an tool developed under AmigaOS and I never liked
the cryptic "<KBD>cmd -cvf -osepp.x</KBD>"-way of invoking commands comming
from Unix, <hsc> uses a syntax like most commands of AmigaOS;
even for ports on other operating-systems. (<TechNote> AmigaOS provides
an OS-function called <CODE>ReadArgs()</CODE> for 
argument-parsing.)</P>

<P>All options and swiches are case-insensitive. If you start <hsc>
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

<A NAME="options"><H2>Options</H2></A>

<DL>
<EXPLAN TITLE="FROM/M" NAME="from">
    Specifies the input filename. To use <stdin> as
    input file, the pseudo filename <CODE>STDIN</CODE> (case sensitive)
    should be specified. If you specify more than one file,
    the last file is taken as main source, and all previous files are
    included before it. This is useful to include macro-definitions
    within the command-line instead of an <ln_include> in the hsc-source.
<EXPLAN TITLE="TO/K" NAME="to">
    Specifies the output filename or the destination directory.
    If the argument ends with a "<KBD>/</KBD>" (or also "<KBD>:</KBD>"),
    it is interpreted as a directory. In this case, the name of the
    outputfile is created from the name of the input file, with an
    extension specified (see below). If no output file is set, <stdout> 
    is used instead.<BR>
    For details, read the seaction about
    <A HREF="fileargs.html">File Options</A>
    
<EXPLAN TITLE="EXTENSION/K" NAME="extension">
    If you specify a directory for output, the output-filename
    will depend on the input-filename and the extension specified
    with this options. If you do not set an extension, "<CODE>html</CODE>"
    will be used. A <period> will be added automatically.
    Setting <KBD>EXTENSION=.</KBD> will ommit this and result into exactly 
    the same filename as the input.
    If you specify a complete filename for output, this option has no
    effect.
<EXPLAN TITLE="MSGFILE/K" NAME="msgfile">
    Redirects messages to a file. By default, <stderr> ist used.
<EXPLAN TITLE="MSGFORMAT/K" NAME="msgformat">
    Describes how messages should be displayed. Legal placeholders are:
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
<EXPLAN TITLE="PREFSFILE/K" NAME="prefsfile">
    Specifies filename for <A HREF=":features/prefs.html">sytax-definition</A>
    to be used, if you don't want to use the defaults.

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
    Stores some information about the document currently processing
    in a seperate file to extend functionality. Read the section
    about <A HREF="project/index.html">Project managment</A> for more
    details. Note that all documents within a project should use the
    same project-file.

<EXPLAN TITLE="DEFINE=DEF/K/M" NAME="define">
    Defines a global attribute. The syntax is equal to <ln_define>, exept
    that you need no quotes surrounding the value.<BR>
    Example: <CODE>DEFINE "hugo:string=hugo was here"</CODE>

<EXPLAN TITLE="IGNORE=IGN/K/M" NAME="ignore">
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
<EXPLAN TITLE="MODE/K" NAME="mode">
    Set base mode for syntax check. Legal values are <KBD>PEDANTIC,
    NORMAL</KBD> and <KBD>RELAXED</KBD>. Default is <KBD>NORMAL</KBD>.
    This only disables several messages and warnings from the beginning.
    Any <KBD>IGNORE</KBD>s passed before are reseted.

<EXPLAN TITLE="QUOTESTYLE/K" NAME="quotestyle">
    Specifies which kind of quotes should be used to surround 
    attribute-values. <KBD>KEEP</KBD> keeps the user-kind, 
    <KBD>DOUBLE</KBD> tries to assign double quotes 
    (eg. <CODE>"value"</CODE>), 
    <KBD>SINGLE</KBD> tries to assign single quotes 
    (eg. <CODE>'value'</CODE>) and <KBD>NONE</KBD> will remove
    quotes if possible.

<EXPLAN TITLE="INCLUDEDIR=IDIR/K/M" NAME="includedir">
    By default, only the current directory is scanned for files
    to be included (using the CLI option <op_from> or the
    special tags <ln_include> or <ln_exec>). This option adds a
    directory to the list of directories to search for include
    files.

<EXPLAN TITLE="STATUS/K" NAME="status">
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
    The above flags can be combined by separating them with a <vbar>,
    eg <KBD>STATUS=line|version</KBD>, exept for <KBD>QUIET</KBD>
    and <KBD>FULL</KBD>
<EXPLAN TITLE="STRIPTAGS/K" NAME="striptags">
    Strip some specific tags from document. You can specify a list of tags
    which is a string,
    that contains all tags that should be stripped, sepparated by a
    <vbar>.<BR>
    See also <op_stripcomment> and <op_stripexternal>.
<EXPLAN TITLE="ICONBASE/K" NAME="iconbase">
    Replace all icon entities by images that must be available from
    the base URI specified. If this URI contains an <asterisk>, it 
    will be substituted by the icon name. For
    example, with <KBD>ICONBASE=:icons/*.gif</KBD>, the entity
    <CODE>&amp;back;</CODE>
    is replaced by <TG>IMG SRC=":icons/back.gif" ALT="back"</TG>. Note
    that this argument does not denote a directory, but an URI. It's
    recommended to use an <fe_absuri>. 
</DL>

<A NAME="switches"><H2>Switches</H2></A>
<DL>
<EXPLAN TITLE="HELP/S" NAME="help">
    Display a short help message and exit.
<EXPLAN TITLE="LICENSE/S" NAME="license">
    Display a short version of the <A HREF="../COPYING">GNU General
    Public License</A> and exit.
<EXPLAN TITLE="COMPACT/S" NAME="compact">
    <A HREF="features/strip.html">Strip redundant linefeeds
    and white-spaces</A> from output.
<EXPLAN TITLE="-DEBUG/S" NAME="-debug">
    If you've compiled <hsc> in debug-mode, it will send lots of
    information to <stderr>. Otherwise, this option is
    equivalent to <KBD>STATUS=FULL</KBD>.
<EXPLAN TITLE="GETSIZE/S" NAME="getsize">
    <A HREF="features/getsize.html">Get width and height of images</A>
     and set corresponding attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> with these values;
<EXPLAN TITLE="MSGANSI/S" NAME="msgansi">
    Uses ANSI-sequences to make messages more readable. This option is
    recommended to be used by people who are not smart enough to integrate
    <hsc> in some of the existing project-managment environments.
<EXPLAN TITLE="RPLCENT/S" NAME="rplcent">
    <A HREF="features/rplcent.html">Replace special characters</A> with
    their entities (eg <qq><CODE>&uuml;</CODE></qq> becomes 
    <qq><CODE>&amp;uuml</CODE></qq>).
<EXPLAN TITLE="RPLCQUOTE/S" NAME="rplcquote">
    Replace double-quotes (<qq><CODE>"</CODE></qq>) found in the text by 
    <qq><CODE>&amp;quot;</CODE></qq>. This does not affect quotes used inside
    attribute-values.
<EXPLAN TITLE="SMARTENT/S" NAME="smartent">
    Replace special characters <qq><CODE>&amp;</CODE></qq>, 
    <qq><CODE>&lt;</CODE></qq>
    and <qq><CODE>&gt;</CODE></qq> with their entities
    (<CODE>&amp;amp;, &amp;lt;</CODE> and <CODE>&amp;gt;</CODE>) if they are
    surrounded by white-spaces. 
<EXPLAN TITLE="STRIPBASWS/S" NAME="stripbadws">
    Strip bad white spaces that would have caused message #78 or #79;
    this will not strip those wich cause message #3. The main advantage
    of this switch is that it allows you to write well structured
    macros.
<EXPLAN TITLE="STRIPCOMMENT/S" NAME="stripcomment">
    Strip all SGML-comments from input.
<EXPLAN TITLE="STRIPEXTERNAL/S" NAME="stripexternal">
    Strip tags which refer to an external URI.
</DL>

<A NAME="exitcodes"><H2>Exit Code</H2></A>

<P>The exit-code <hsc> returns to the CLI depends on the messages and errors
that have been displayed during runtime.</P>

<P>The version for AmigaOS returns 
<UL>
<LI><CODE>0</CODE> on success,
<LI><CODE>5</CODE> if warnings occured,
<LI><CODE>10</CODE> if errors showed up (also for errors in args) and 
<LI><CODE>20</CODE> if fatal errors happned (also for out-of-resources).
</UL>

For other systems, values like <CODE>0/0/1/2</CODE> are used instead.

</WEBPAGE>
