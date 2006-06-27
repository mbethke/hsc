<DOCPAGE chapter="hsc - " title="Invoking Hsc"
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
<$MACRO explan /close TITLE:string/r NAME:string ONLYDEFTAG:bool>
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
</DT><DD><$content></DD>
</$MACRO>


This chapter explains which options and switches you can pass to
<hsc>, and which <A HREF="#exitcodes">exit codes</A> it might return
to the shell. For easier understanding, related options are collected in
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
<DD>This parameter wants a decimal number.
    If an invalid number is specified, an error will be returned.  
<*
        /T - Toggle.  This is similar to a switch, but when specified causes
             the boolean value to "toggle".  Similar to /S.*>
<DT><KBD>/A</KBD> - Required. (think: ``Always''!)
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

<P>The exit-code <hsc> returns to the shell depends on the messages and errors
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
    should be specified, or alternatively the unixoid <qqc>-</qqc>.
    If you specify more than one file,
    the last file is taken as main source, and all previous files are
    included before it. This is useful to include macro-definitions
    within the command-line instead of an <ln-include> in the hsc-source.
</EXPLAN>

<EXPLAN TITLE="TO/K" NAME="to">
    Specifies the output filename or the destination directory.
    If the argument ends with a "<KBD>/</KBD>" (or also "<KBD>:</KBD>"),
    it is interpreted as a directory. In this case, the name of the
    output file is created from the name of the input file, with an
    extension specified (see below). If no output file is set, <stdout> 
    is used instead.<BR>
    For details, read the section about
    <A HREF="fileargs.html">File Options</A>
</EXPLAN>
    
<EXPLAN TITLE="EXTENSION/K" NAME="extension">
    If you specify a directory for output, the output-filename
    will depend on the input-filename and the extension specified
    with this options. If you do not set an extension, <qqc>html</qqc>
    will be used. A <period> will be added automatically.
    Setting <KBD>EXTENSION=.</KBD> will omit this and result into exactly
    the same filename as the input.
    If you specify a complete filename for output, this option has no
    effect.
</EXPLAN>
</DL>

<**************************************************************************>
<H2>Message Options</H2>
<**************************************************************************>

<DL>
<EXPLAN TITLE="IGNORE=IGN/K/M" NAME="ignore">
    Ignore message number or class. Usually, <KBD>IGNORE</KBD>
    needs a numeric argument and can occur multiple times in the
    command line.
    Additionally, you can specify some special values, which will
    ignore whole groups of messages: <KBD>ALL</KBD>, <KBD>NOTE</KBD>,
    <KBD>STYLE</KBD>, <KBD>PORTABILITY</KBD> and <KBD>JERK</KBD>.
    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.
</EXPLAN>

<EXPLAN TITLE="ENABLE=ENA/K/M" NAME="enable">

    This is the opposite to <KBD>IGNORE</KBD> and can be used to
    enable certain messages. You can also use the same special values
    as with <kbd>IGNORE</kbd>.

    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.
</EXPLAN>


<EXPLAN TITLE="MSGMODE/K" NAME="msgmode">
    Set base mode for syntax check. Legal values are <KBD>PEDANTIC,
    NORMAL</KBD> and <KBD>RELAXED</KBD>. Default is <KBD>NORMAL</KBD>.

    See <a href="messages.html#ignore">Ignore/Enable Certain Messages
    and Classes</a> for more information.
</EXPLAN>

<EXPLAN TITLE="MSGFILE/K" NAME="msgfile">
    Redirects messages to a file. By default, <stderr> is used.

    See <a href="messages.html#msgformat">Redirecting Messages and
    Changing the Message Format</a> for more information.
</EXPLAN>

<EXPLAN TITLE="MSGFORMAT/K" NAME="msgformat">
    Describes how messages should be displayed.

    See <a href="messages.html#msgformat">Redirecting Messages and
    Changing the Message Format</a> for more information.
</EXPLAN>

<EXPLAN TITLE="NONESTERR/S" NAME="nonesterr">
    Suppresses the <qq>location of last call</qq> messages that usually provide
    a <qq>traceback</qq> when errors/warnings are displayed. If you use deeply nested
    macros, these confuse more than they clarify, so you can use this switch
    to see only the place where an error was detected.
</EXPLAN>

<EXPLAN TITLE="NOVALIDATECSS=NVCS/S" NAME="nvcs">
    Disables the checking of CSS style specifications. See
    <a href="features/css.html">CSS support</a> for further information.
    In XHTML mode, CSS checking cannot be disabled, trying this will yield a warning.
</EXPLAN>
</DL>

<**************************************************************************>
<H2>Configuration</H2>
<**************************************************************************>

<DL>
<EXPLAN TITLE="DEFINE=DEF/K/M" NAME="define">
    Defines a global attribute. The syntax is equal to <ln-define>, exept
    that you need no quotes surrounding the value.<BR>
    Example: <KBD>DEFINE "hugo:string=hugo was here"</KBD>
</EXPLAN>

<EXPLAN TITLE="INCLUDEDIR=IDIR/K/M" NAME="includedir">
    By default, only the current directory and the main source's directory
    are scanned for files
    to be included (using the commandline option <op-from> or the
    special tags <ln-include> or <ln-exec>). This option adds a
    directory to the list of directories to search for include
    files.
</EXPLAN>

<EXPLAN TITLE="PREFSFILE/K" NAME="prefsfile">
    Specifies the filename for a <A HREF=":features/prefs.html">syntax definition</A>
    to be used, in case you don't want to use the default.
</EXPLAN>

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
    Stores some information about the document currently processing
    in a seperate file to extend functionality. Read the section
    about <A HREF="project/index.html">Project management</A> for more
    details. Note that all documents within a project should use the
    same project-file.
</EXPLAN>

<EXPLAN TITLE="SERVERDIR/K" NAME="serverdir">
    This option tells <hsc> where to look for files
    referenced by server-relative URIs. The directory has to be
    specified as a <STRONG>native</STRONG> filename of your OS.
    Example:
    <KBD>SERVERDIR=aws:home</KBD> might work fine under AmigaOS,
    <KBD>SERVERDIR=/home/sepp/public_html</KBD> could be used
    on a unixoid system.
</EXPLAN>

<EXPLAN TITLE="CHECKEXTERNAL=CKX/S" NAME="checkexternal">
    In times of <qq>always on</qq> internet connections it may seem a little
    anachronistic to check only local URIs for existence. Using this switch, you
    can enable external URI checking for HTTP, so <hsc> will actually ask the
    remote server for any document you reference.
    This can and will tremendously slow it down, but for a final
    run before upload it can be worth the time.<br>
    So far, <hsc> only speaks HTTP. However, proxies are supported via the the
    same <a href=":envvar.html#http_proxy">environment variables</a> as in
    <a href="http://lynx.browser.org/">Lynx</a>: <code>http_proxy</code>,
    <code>ftp_proxy</code> and <code>gopher_proxy</code>. So if you can use a
    proxy, you can even check URIs of the latter types.
</EXPLAN>


</DL>

<**************************************************************************>
<H2>Options Affecting The Output</H2>
<**************************************************************************>

<DL>

<EXPLAN TITLE="GETSIZE/S" NAME="getsize">
    <A HREF="features/getsize.html">Get width and height of images</A>
     and set corresponding attributes
    <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> (or the appropriate CSS
    attributes within a <CODE>STYLE</CODE> attribute when running in XHTML mode)
    with these values.
</EXPLAN>

<EXPLAN TITLE="ICONBASE/K" NAME="iconbase">
    Replace all icon entities by images that must be available from
    the base URI specified. If this URI contains an <asterisk>, it 
    will be substituted by the icon name. For
    example, with <KBD>ICONBASE=:icons/*.gif</KBD>, the entity
    <CODE>&amp;back;</CODE>
    is replaced by <TG>IMG SRC=":icons/back.png" ALT="back"</TG>. Note
    that this argument does not denote a directory but an URI. It is
    recommended to use a <fe_prjuri>.
</EXPLAN>

<EXPLAN TITLE="QUOTESTYLE/K" NAME="quotestyle">
    Specifies which kind of quotes should be used to surround 
    attribute-values. <KBD>KEEP</KBD> keeps the kind found in the source,
    <KBD>DOUBLE</KBD> tries to assign double quotes 
    (eg. <CODE>"value"</CODE>), 
    <KBD>SINGLE</KBD> tries to assign single quotes 
    (eg. <CODE>'value'</CODE>) and <KBD>NONE</KBD> will remove
    quotes if possible.<br>
    In XHTML mode, only double quotes are permitted. DOUBLE is also the default
    in old-style HTML mode.
</EXPLAN>

<EXPLAN TITLE="ENTITYSTYLE/K" NAME="entitystyle">
    Sets the rendering of entities either found in the source or generated using
    <a href="#rplcent">RPLCENT</a>. The following styles are possible:
<dl>
    <dt>KEEP</dt><dd>Keep entities as they were found in the source. If entities
                were generated from hi-ASCII characters (see
                <a href="#rplcent">RPLCENT</a>), they will be in the symbolic
                form, unless their <a href=":features/prefs.html#defent">entity
                definition</a> says otherwise.</dd>
    <dt>REPLACE</dt><dd>Replace entities with the form their definition says is
                preferred.</dd>
    <dt>NUMERIC</dt><dd>Always render entities numerically.</dd>
    <dt>SYMBOLIC</dt><dd>Always render entities symbolically (if a symbol
    exists<mdash>numerically otherwise).</dd>
    <dt>UTF-8</dt><dd>Don't write HTML entities at all, except for
    <qq>&amp;lt;</qq>, <qq>&amp;gt;</qq>, <qq>&amp;quot;</qq> and
    <qq>&amp;amp;</qq>, but write UTF-8 text. This applies to both entities
    found in the source and replacements for hi-ASCII characters. You must
    declare the UTF-8 charset in a META tag, otherwise most browsers would
    assume it to be Latin-1 and render it incorrectly. UTF-8 mode is the default
    for XHTML,</dd>
</dl>
    To let you react on the entity style currently used in <a
    href=":features/if.html">conditionals</a>, <hsc> defines the
    <code><a href=":features/spcattr.html#opts-entitystyle">Hsc.Opts.Entitystyle</a></code>
    variable.
</EXPLAN>

<EXPLAN TITLE="RPLCENT/S" NAME="rplcent">
    <A HREF="features/rplcent.html">Replace special characters</A> with
    their entities. For instance, <qq><CODE>&uuml;</CODE></qq> turns into
    <qq><CODE>&amp;uuml</CODE></qq>. Also see <a
    href="#entitystyle">ENTITYSTYLE</a>!
</EXPLAN>

<EXPLAN TITLE="RPLCQUOTE/S" NAME="rplcquote">
    Replace double quotes (<qq><CODE>"</CODE></qq>) found in the text by 
    <qq><CODE>&amp;quot;</CODE></qq>. This does not affect quotes used inside
    attribute values.
</EXPLAN>

<EXPLAN TITLE="LOWERCASETAGS=LCT/S" NAME="lowercasetags">
    There are many different conventions for tag case that people find
    convenient, and as it didn't matter for HTML, that was fine. For XML it does.
    Even if you are not
    writing XHTML, you can force your tags and attributes to lowercase with this
    switch and keep your tagging idiom and your old macro files as they are.
</EXPLAN>

<EXPLAN TITLE="XHTML/S" NAME="xhtml">
    XHTML introduces some new conventions that are incompatible with HSC's usual
    opinion about correct HTML:
    <ul compact>
    <li>Boolean attributes must not be minimized. I.e. it's <TG>hugo
        resi="resi"</TG>
        now, instead of <TG>hugo resi</TG>.
    <li>Empty tags may (and, to reduce bogosity and waste of bandwidth, should)
        be minimized to a form like <TG>hugo/</TG>, or, to stay compatible
        with older HTML browsers, <TG>hugo /</TG> (note the space!). With the
        trailing-slash-recognition being a bit of a hack, HSC supports only the
        latter form, i.e. <em>with</em> space.
    <li>All tags and attributes must be lowercase.
    </ul>
    There are probably other criteria that I forgot, but this is what HSC
    enforces so far if you set this switch.
</EXPLAN>

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
</EXPLAN>

<EXPLAN TITLE="STRIPBADWS/S" NAME="stripbadws">
    Strip bad white spaces that would have caused <ln-msg id="78">
    or <ln-msg id="79">; this will not strip those which cause
    <ln-msg id="47">. The main advantage of this switch is that
    it allows you to write well structured macros.
</EXPLAN>

<EXPLAN TITLE="STRIPCOMMENT/S" NAME="stripcomment">
    Strip all SGML-comments from input.
</EXPLAN>

<EXPLAN TITLE="STRIPEXTERNAL/S" NAME="stripexternal">
    Strip tags which refer to an external URI.
</EXPLAN>

<EXPLAN TITLE="STRIPTAGS/K" NAME="striptags">
    Strip some specific tags from document. You can specify a list of tags
    which is a string,
    that contains all tags that should be stripped, separated by a
    <vbar>.<BR>
    See also <op-stripcomment> and <op-stripexternal>.
</EXPLAN>
</DL>

<**************************************************************************>
<H2>Miscellaneous Options</H2>
<**************************************************************************>

<DL>

<EXPLAN TITLE="HELP=?=-h=--help/S" NAME="help">
    Display a short help message and exit.
</EXPLAN>

<EXPLAN TITLE="LICENSE/S" NAME="license">
    Display a short version of the <A HREF="../COPYING">GNU General
    Public License</A> and exit.
</EXPLAN>

<EXPLAN TITLE="-DEBUG/S" NAME="-debug">
    If you've compiled <hsc> in debug-mode, it will send lots of
    information to <stderr>. Otherwise, this option is
    equivalent to <KBD>STATUS=FULL</KBD>.
</EXPLAN>

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
</EXPLAN>

</DL>

</DOCPAGE>

<* vi: set ts=4 et: *>
