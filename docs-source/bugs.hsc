<WEBPAGE chapter="hsc - " title="Known Bugs, Problems And Limitations"
    PREV="others.html"
    NEXT="future.html"
    QTEXT=("So we keep putting our trust in things that rust<BR>"
          +"And then we feel the pain of loss")
    QAUTHOR='James, "Stripmining"'>

Every program sucks. Read why this program sucks, too.

<H2>Known Bugs</H2>

Currently everything repairable has been fixed.

<H2><A NAME="internal">Internal Error Messages</A></H2>

<P>This is a special kind of messages which does not fit into the
schema of all those <A HREF=":messages.html">messages</A>
described at another chapter. When processing it's input,
<hsc> does not only check if the user made a mistake, but
also the author of this tools. At several places of the
source code, <hsc> tests for things that must not happen.
Otherwise, <hsc> assumes it has completely fucked-up. In this
case, it simple displays a message on the screen and aborts.</P>

An example message could look like this:

<PRE>** internal error at "hugo.c" (123): sepp is not at home</PRE>

<P>These messages are not really meant to be interpreted by the user,
but by the programmer. When <hsc> puts out an internal error message,
you should <A HREF=":author.html">contact the author</A> and report
this bug, including the complete text of the panic message. In such a
case, you are not expected to create a sophisticated bug report, as
such problems are hard to back-trace.</P>

<H2>Known Problems</H2>

The problems described below probably won't ever be fixed.
<UL>
<LI>URI checking is performed via trying to open a file. As AmigaOS
    is <STRONG>case-insensitive dealing with filenames</STRONG>, problems might
    occur when copying the html-object-tree to a case-sensitive
    file system. I recommend to rebuild the whole project at the
    target system.

<LI>If you specify a <STRONG><TG>BASE HREF=".."</TG></STRONG>, <hsc> is unable to
    find out how the base is related to the destination directory
    and will treat all local URIs like external ones. Therefor no
    link validation will be performed. Project relative URIs
    are not possible with a base-URI set, of course.

<LI>The <STRONG>obsolete tags</STRONG> <TG>LISTING</TG> and <TG>XMP</TG> might not be
    treated correctly, as the behavior of these two seems so be not
    very strictly defined. You should avoid them anyway and use
    <TG>PRE</TG> or <TG>$source</TG> instead.

<LI>With CLI-option <STRONG><KBD>STRIPBADWS</KBD></STRONG> enabled, source code like
    <qqc><$source><B>hello <!-- xy --></B></$source></qqc>
    is not converted to
    <qqc><$source><B>hello<!-- xy --></B></$source></qqc>. That means that
    the blank after <qqc>hello</qqc> will remain, but does not display a
    <ln_msg id="79">, as the <TG>!..</TG> caused all white spaces preceding
    it to be flushed. I already said it several times, and I say it again:
    Do not use those bloody sgml-comments!

<LI><hsc CAP> doesn't care much about the <STRONG>maximum length of filenames</STRONG>.
    If a filename gets too long, it depends on
    <CODE>fopen()</CODE> of your compiler whether an error is reported 
    or the filename
    is truncated. If you insist on filenames containing 46587643
    characters, <hsc> can cope with it - but your OS probably won't.

<LI>You should try to avoid using a <STRONG>project file and pipes</STRONG>
    together; although <hsc> should now be able to cope with
    this without enforcer hits, it might still behave
    unreasonable and end up with funny project files and
    dependencies.

<LI>Out-of-memory can cause small <STRONG>memory-leaks</STRONG>. This is because <hsc>
    uses a special <CODE>malloc()</CODE>-function which just displays
    an error message and quits using <CODE>exit()</CODE>. As <hsc>
    keeps track of all allocated resources, it will release everything
    that could be fully initialised and made it into its supposed
    resource list. For out-of-memories during partially initialising
    complex structures, this fails and can cause memory-leaks
    (normally approx. 100-500 bytes). Usually, these are handled by
    your OS or, as for AmigaOS, by the <CODE>malloc()</CODE>-function
    of the run-time-library, so you shouldn't bother too much about
    that. Blame K&amp;R (or whoever perpetrated
    <CODE>malloc()</CODE>)for their brain-damaged memory management
    concepts.

<LI><STRONG><CODE>GetFileSize()</CODE></STRONG> probably won't work correctly on
    files greater than 2 GB. This usually caused by the limited abilities of
    <CODE>fseek()</CODE> and <CODE>ftell()</CODE>.
</UL>

<H2>Limitations</H2>

<P><hsc CAP> is fully dynamic and it's input size, number of
syntax-elements and etc. is only limited by memory and disk space.
Only some less important status messages are created in classic,
brain-dead zero-terminated C-character-arrays and are truncated if
they become too long.</P>

<P>However, for projects much larger then 100 documents, scanning the
project file on every run takes an awful lot of time; due the lack of any
portable concept of keeping data resident after a program exits, there is
no workaround for this (same problem like with <hsc.prefs>); you will have
to get rid of the project-file and <hscdepp> for such projects.</P>

</WEBPAGE>
