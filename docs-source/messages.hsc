<WEBPAGE chapter="hsc - " title="Messages"
    PREV="fileargs.html"
    NEXT="message-list.html">

<$macro MSG_NOTE><A HREF=":messages.html#msg_note">Note</A></$macro>
<$macro MSG_STYLE><A HREF=":messages.html#msg_style">Bad-style</A></$macro>
<$macro MSG_PORT><A HREF=":messages.html#msg_port">Portability-problem</A></$macro>
<$macro MSG_WARN><A HREF=":messages.html#msg_warn">Warning</A></$macro>
<$macro MSG_ERR><A HREF=":messages.html#msg_err">Error</A></$macro>
<$macro MSG_FERR><A HREF=":messages.html#msg_ferr">Fatal error</A></$macro>

<P><hsc CAP> is able to produce lots of different warnings and errors
when parsing hsc-sources. The are divided into several classes, and
there is a possibility to filter out specific messages or whole
message classes. There are also possibilities to change the appearance
of messages and redirect them to a file, making it easy to integrate
<hsc> into existing developer environments.</P>

<H2><A NAME="elements">Message Elements</A></H2>

For example, a typical message could be:

<$source PRE>hugo.hsc (17,23): Warning 11: unknown tag <SEPP></$source>

with being..<UL>

<LI>..<CODE>hugo.hsc</CODE> the name of the input file that caused the message.
<LI>..<CODE>17</CODE> and <CODE>23</CODE> the position
    in input file. 17 denotes the line, and 23 the column of the
    item that caused the message
<LI>..<CODE>Warning</CODE> the message class. There are several other
    messages classes: <MSG_NOTE>, <MSG_STYLE>, <MSG_PORT>, <MSG_ERR> or <MSG_FERR>.
    See below for details about those message classes.
<LI>..<CODE>11</CODE> the number of the message. You can use this to
    quickly find the description in the <a href="message-list.html">list of
    message</a> or to suppress it in further runs using <op-ignore>.
<LI>..<CODE>unknown tag &lt;SEPP&gt;</CODE> the message text, which describes what
    went wrong.
</UL>

<p>The message about would suggest to load the file
<file>hugo.hsc</file> in your editor and go to line 17, column 23.</p>

<H2><A NAME="classes">Message Classes</A></H2>

<P><STRONG><A NAME="msg_note">Note</A></STRONG> is a message only for the
user's information. It can be suppressed using <op-ignore>.</P>

<P><STRONG><A NAME="msg_style">Bad-style</A></STRONG> informs the user
that his although legal html-code includes constructs that indicate a
bad style. It can be suppressed using <op-ignore>.</P>

<P><STRONG><A NAME="msg_port">Portability-problem</A></STRONG> informs
the user that his although legal html-code includes constructs can
lead to problems on old or buggy browsers. It can be suppressed using
<op-ignore>.</P>

<P><STRONG><A NAME="msg_warn">Warning</A></STRONG> tells you that your
source includes some problematic parts that can lead to problems with
several browsers. It can be suppressed using <op-ignore>.</P>

<P><STRONG><A NAME="msg_err">Error</A></STRONG> indicates that there
is something wrong with your html-source. Conversion will continue,
but no output will written be written after finishing it.</P>

<P><STRONG><A NAME="msg_ferr">Fatal error</A></STRONG> tells you that
something serious is wrong and there is no way for <hsc> to recover
and continue the conversion. No output will be written.</P>

<P>There is also another class called <STRONG>internal error
messages</STRONG>, which are used by <hsc> to report that hsc itself
is broken. Normally this should not happen, but if you found a bug.
To find out more what to do in such a case, look at the chapter about
<ln-bugs>.</P>

<H2><A NAME="options">Message Options</A></H2>

<p>There are several CLI options to modify the behavior of <hsc>'s
messages. You can suppress messages you are not interested in, you can
redirect them to a file and change the output format (e.g. to pipe
them to an external message browser). On some systems, you can even
send them to some selected message browsers without messy piping and
scripting.</p>

<h3><a name="ignore">Ignore/Enable Certain Messages and Classes</a></h3>

<p>If you think that <hsc> reports too much or too less about problems
in your source code, you can specify several options to change its
behavior.</p>

<p>The first one  - and also the one with the most impact - is <kbd>MSGMODE</kbd>.
This switches <hsc> into one of three possible basic modes for the syntax
check:</p>

<ul>
<li><kbd>PEDANTIC</kbd> - Whine about nearly everything, including
    the whole bunch of bad style and portability problems. Also minor
    changes to the output like the replacing of entities is reported
    in this mode.
<li><kbd>NORMAL</kbd> - This is the default that should give a reasonable
    cocktail of messages, reporting most problems the average users
    should understand.
<li><KBD>RELAXED</KBD> - Perform only a very basic check and ignore
    nearly everything that might cause problems. This should only be
    used if you do not care much about your html-code (Read: you are
    a jerk).
</ul>

<p>These modes however give only very rough possibility to control the
amount of messages. For the fine-tuning, the more sophisticated
<kbd>IGNORE</kbd> and <KBD>ENABLE</KBD> exist.</p>

<p>Unless <KBD>MSGMODE=NORMAL</KBD> is ok for you, you have to specify
<kbd>MSGMODE</kbd> in the command line <em>before</em>
<KBD>IGNORE</KBD> or <KBD>ENABLE</KBD> is used the first time as it
resets several of the internal data structures.</p>

<p><KBD>IGNORE</KBD> can be used to suppress single messages or whole
message classes, if you are not interested in them. This option can
show up in the command line call multiple times, for example:</p>

<p><KBD>IGNORE=18 IGN=21 IGN 22</KBD> - ignore warnings #18, #21, #22</p>

<p>You can also specify a whole list of messages, separated by a <vbar>, for instance:</p>

<p><KBD>IGNORE=18|21|22</KBD> - same as the above example</p>

<p>(If the vertical bar is used as piping character, you have to quote
this value: <KBD>IGNORE "18|21|22"</kbd>)</p>

<p>There are some special, non-numeric values you can use to ignore whole
groups or classes of messages:</p>

    <UL>
    <LI><KBD>ALL</KBD> - Ignore all non-error messages
    <LI><KBD>JERK</KBD> - Ignore all stuff perpetrated by jerks only
    <LI><KBD>NOTE</KBD> - Ignore all notes
    <li><kbd>PORTABILITY</kbd> - Ignore all portability problems
    <LI><KBD>STYLE</KBD> - Ignore all bad style warnings
    </UL>

<p>If you don't care about portability problems and some other
messages, but still want to have a quite exhaustive check, you could
use something like this:</p>

<p><kbd>MSGMODE=pedantic IGNORE=portability|46|51</kbd></p>

<p>If on the other hand you are quite satisfied with the
<KBD>NORMAL</KBD> check but want to see one particular message (e.g.
replaced icon entities), this should do the trick:</p>

<p><kbd>MSGMODE=normal ENABLE=77</kbd></p>

<p>You can combine these options to even more complex rules:</p>

<p><kbd>MSGMODE=pedantic IGNORE=note ENABLE=77</kbd></p>

<p>This first enables all messages, then disables all notes but
finally enables again one specific note for <ln-msg id="77">.</p>

<p>To find out the number of a certain message to enable, look at the
<a href="message-list.html">list of messages</a>. To ignore a message,
simple get the number from the console once an unwanted message shows
up the first time.</p>


<h3><a name="msgformat">Redirecting Messages and Changing the Message Format</a></h3>

<KBD>MSGFORMAT</KBD> specifies the appearance of messages, for example
concerning the sequence of the elements described before. The message
format can contain several placeholders that are replaced by the
message elements described above:

    <TABLE BORDER="1">
    <TR><TD>%f     </TD><TD>filename                           </TD></TR>
    <TR><TD>%x, %y </TD><TD>location in file (column, line)    </TD></TR>
    <TR><TD>%c     </TD><TD>message class                      </TD></TR>
    <TR><TD>%i     </TD><TD>message id (numeric)               </TD></TR>
    <TR><TD>%m     </TD><TD>message text                       </TD></TR>
    <TR><TD>%n     </TD><TD>inserts a linefeed                 </TD></TR>
    <TR><TD>%%     </TD><TD>inserts percent sign (<qqc>%</qqc>)</TD></TR>
    </TABLE>

<p>To for example get GCC-like messages, use <KBD>MSGFORMAT="%f:%y: %c
%i: %m"</KBD>.<p>

<p>To disable output to the console, specify an empty string:
<kbd>MSGFORMAT=</kbd>. This is probably useful only if you use a
message browser (see below).</p>

<p><KBD>MSGFILE</KBD> can be used to redirect messages to a file, from
where they can be processed for example by some message parser that
controls your editor.</p>

<p>For an example script that invokes hsc, redirects its messages and
sends them to a message browser, see
<A HREF=":../grafflwerk/hScMsg.rexx"><FILE>grafflwerk/hScMsg.rexx</FILE></A>.</p>


<h3><a name="msgbrowser">Using a Message Browser</a></h3>

<p>By default, <hsc> writes messages to the console where it was
started from. For the user this means to read the filename and
position, switch to his editor and go to the line told. Because this
is pretty troublesome, many people use message browser where all
messages are listed nicely sorted by file. Clicking on a message puts
the editor to front and makes it jump to the requested position.</p>

<p>With the CLI-option <kbd>MSGBROWSER</kbd> you can specify one of
the following browsers (case insensitive):</p>

<ul>

<li><KBD>ScMsg</KBD>: The message browser coming with the SAS/c
compiler for AmigaOS. As ARexx is used to communicate with the
browser, <exec>RexxMast</exec> has to be running. The browser is
expected to be in <exec>sc:c/scmsg</exec> and will be started
automatically if it is not already running.

<li><KBD>VBrowse</KBD>: This uses public domain <exec>VBrowse</exec>
message browser for AmigaOS coming with the freely distributable
<exec>VBCC</exec> compiler. On the hsc support page there is an
archive containing this browser and describing how to use it together
with <hsc>. But you better keep your expectations low - keep in mind
that it is public domain and the compiler is the main application of
this package.

<li><KBD>ThrowBack</KBD>: This uses the throwback mechanism popular
under RiscOS.

<li><KBD>ANSI</KBD>: This is no browser but only uses some ANSI codes
when sending the message to the console. Your console has to support
ANSI escape sequences (most do nowadays). This is only useful if you
can not find any more comfortable way to read the messages.

</ul>

<p>So to to use <exec>ScMsg</exec>, specify <kbd>MSGBROWSER=SCMSG</KBD>,
for throwback set <kbd>MSGBROWSER=Throwback</KBD>.</p>

<p>If you specify a message browser on a system that does not support
it, e.g. attempting to <kbd>Throwback</kbd> under NeXTStep, no browser
will be used. That means you do not have to change your <makefile> if
you want to work on another system for a short time.</p>

<p>Note that status messages and non-input related error messages are
still viewed in the console.</p>

<h3><a name="limit">Limiting Messages</a></h3>

<p>By default, <hsc> stops to parse once more than 40 messages or 20
errors have show up. In such a case, you should either start to
<op-ignore> messages or fix errors.</p>

<p>If you don't like these numbers, you can change them using
<kbd>MAXMSG</kbd> respectively <kbd>MAXERR</kbd>. The value <kbd>0</kbd>
represents an "infinite" amount of messages.</p>

<p>Note that nested messages with all those <qqc>location of previous
call</qqc> texts still count as one, independent of the nesting
level.</p>


</WEBPAGE>
