<WEBPAGE chapter="hsc - " title="Known Bugs, Problems and Limitations"
    PREV="source.html"
    NEXT="future.html"
    QTEXT=("So we keep putting our trust in things that rust<BR>"
          +"And then we feel the pain of loss")
    QAUTHOR='James, "Stripmining"'>

Every program sucks. Read why this program sucks, too.

<H2><A NAME="panic">Panic messages</A></H2>

<P>This is a special kind of messages which do not fit into the
scema of all those <A HREF=":messages.html">messages</A>
described at another chapter. When processing it's input,
<hsc> does not only check if the user made a mistake, but
also the author of this tools. At several places of the
source code, <hsc> tests for things that must not happen.
Otherwise, <hsc> asumes it has completely fucked up. In this
case, it simple displays a message on the screen and aborts.</P>

An example panic message could look like this:

<PRE>  ** PANIC at "hugo.c" (123): sepp is not at home</PRE>

<P>These messages are not really meant to be interpreted by the
user, but by the programmer. When <hsc> puts out a panic message,
you should <A HREF=":author.html">contact the author</A> and
report this bug, including the complete text of the panic
message.</P>


<H2>Known bugs</H2>

The problems below should be fixed.. sometimes.
<UL>
<LI>You should try to avoid using a project file and pipes
    together; altough <hsc> should now be able to cope with
    this without enforcer hits, it might still behave
    unreasonable and end up with funny project files and
    dependencies.
<LI>There seems to be a problem with <CODE>GETSIZE</CODE> for
    machines based on CPUs that do not support networking byte order
    (like those from wIntel). 
<LI>I'm still not sure, if <CODE>GETSIZE</CODE> for JFIF/JPEG can handle
    everything correct - but hopefully it does. I've really never seen
    such an idiotic, brain-dead, ill-minded and chaotically structured
    image-format before.
</UL>

<H2>Known problems (to be fixed)</H2>

The problems below are no real bugs and should be fixed.. sometimes.
<UL>
<LI>Most NEXT and PREV buttons of the  documentation does not fit;
    I just wanted to
    show how to create a navigation bar, but I'm too lazy to maintain it
    while the structure of the documentation isn't fixed.
<LI>As I know myself, these documents still contain loads of typos and
    <qq>strange English</qq>..
</UL>

<H2>Known problems</H2>

The problems described below probably won't ever be fixed.
<UL>
<LI>URI checking is performed via trying to open a file. As AmigaOS
    is case-insensitive dealing with filenames, problems might
    occur when copying the html-object-tree to a case-sensitive
    file system. I recommend to rebuild the whole project at the
    target system.
<LI>If you specify a <TG>BASE HREF=".."</TG>, <hsc> is unable to
    find out how the base is related to the destination-directory
    and will treat all local URIs like external ones; Absolute URIs
    aren't possible with a base-URI set, of course.
<LI>The obsolete tags <TG>LISTING</TG> and <TG>XMP</TG> might not be
    treated correctly, as the behavior of these two seems so be not
    very strictly defined. You should avoid them anyway and use
    <TG>PRE</TG> or <TG>$source</TG> instead.
<LI><hsc> doesn't care much about filenames and their limitations; 
    if, for example, a filename gets too long, it depends on 
    <CODE>fopen()</CODE> of your compiler whether an error is reported 
    or the filename
    is truncated. If you insist on filenames containing 46587643
    characters, <hsc> can cope with it - but your OS probably won't.
<LI>Out-of-memory can cause small memory-leaks. This is because <hsc>
    uses a special <CODE>malloc()</CODE>-function
    which just displays an error message and quits using 
    <CODE>exit()</CODE>. As <hsc> keeps track of all allocated resources,
    it will release everything that could be fully initialised and made
    it into its supposed resource-list. For out-of-memories during
    partially initialising complex structures, this fails and can
    cause memory-leaks (normally approx. 100-500 bytes). Usually, these
    are handled by your OS or, as for AmigaOS, by the 
    <CODE>malloc()</CODE>-function of the run-time-library, so you
    shouldn't bother too much about that. Blame K&amp;R for their
    brain-damaged memory-management concepts.
<LI><CODE>GetFileSize()</CODE> probably won't work correctly on
    files greater than 2 GB.
<LI>Until now I couldn't test <hsc> on some 64-bit OS, so problems
    might occur if data leave 32-bit address-space.
</UL>

<H2>Limitations</H2>

<P><hsc> is fully dynamic and it's input size, number of syntax-elements
and etc. is only limited by memory and disk-space. Only some less important
status messages are created in classic, brain-dead zero-terminated
C-character-arrays and are truncated if they become too long.</P>

<P>However, for projects much larger then 100 documents, scanning the
project-file on every run takes an awful lot of time; due the lack of any
portable concept of keeping data resident after a program exits, there is
no workaround for this (same problem like with <hsc.prefs>); you will have
to get rid of the project-file and <hscdepp> for such projects.</P>

</WEBPAGE>
