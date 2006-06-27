<DOCPAGE chapter="hsc - " title="The Source Code"
    PREV="questions.html"
    NEXT="ports.html"
    QTEXT=("I've been swimming in the dirty water<BR>"
          +"I've been swimming where the fish won't go")
    QAUTHOR='The Jesus &amp; Mary Chain, "Dirty Water"'>

Not for all systems there already compiled versions of this package
available. If you can not find binaries anywhere, it seems you will
have to go on. Otherwise, you can skip this chapter, as it does not
contain anything you would want to know.

<H2>Availability</H2>

<P>The source archive should be available from the same locations
where you are supposed to get <A HREF="updates.html">updates</A>
from. Look out for an archive called <FILE>hsc-source.lha</FILE>.

<H2>Portability</H2>

<P><hsc CAP> was written in naked &amp; masochistic ANSI-C. Even worse, it
only uses functions of the standard library and does not require any
Amiga- or Un*x-specific include files like <TG>dir.h</TG>,
<TG>String.h</TG> or functions like <CODE>ReadArgs()</CODE>,
<CODE>stricmp()</CODE> etc.</P>

<P>This should ensure a maximum of portability. However, the handling
of filenames is a system-dependent thing, which has to be adapted for
every OS. See also the section about <A HREF="ports.html">Existing
Ports</A>.</P>

<H2>Compiling Under RiscOS</H2>

For RiscOS, there is a special <makefile> and <FILE>ReadMe</FILE>
you can find in the <FILE>riscos</FILE> directory.

<H2>Compiling</H2>

<P>Basically, compilation works like this: You will need a command
line based C-compiler. The compiler <EM>must</EM> support ANSI-C.
There is a <makefile> which contains settings for all supported
systems. You will have to comment in yours. After that you can launch
the <make> tool and should hopefully end up with the binaries.</P>

Ok, now all those steps in detail:

<OL>

<LI>If you've compiled older versions of <hsc> before, remove all
files of the old sources before you continue. Do not extract a newer
source archive into a directory containing older ones.

<LI>Change your current directory to the <FILE>source</FILE> directory

<LI>Now load the <makefile> into your editor. Make sure your editor is
able to preserve TABs. If it does not, <make> might have some trouble
afterwards.

<LI> Figure out which system and compiler you have, and remove the
<hash> from the corresponding lines.

<LI>Go back to command line and type <KBD>make</KBD>. Lots of strange
message should show up indicating that a compiler is started and that
there are many options passed to it.

<LI>If there is an error - bad luck. If you do not know what to do now
(for example starting to fool around with the <makefile>, fix and
report the bug) - worse luck. Go to the cinema or do something else,
but forget about this software.
</OL>

<H2>Compiler Warnings</H2>

<P>If the compilation was successful, but some compiler warnings
showed up, it depends if you should take it serious or not. There are
certain parts of the code which declare functions and symbols, but not
actually use them (they use them in debug-mode, but I did not want to
clutter the source with too many <CODE>#ifdef</CODE>s). During linking
stage this code should be kicked out anyway. You can safely ignore
this.</P>

<P>Other warnings indicate a compiler bug. Contact your compiler
vendor.</P>

<P>Ok, maybe not. If you think it could be a nasty one, do a bug
report on <hsc>.</P>

<H2>The Binaries</H2>

If all went well, you now should have created three files:

<PRE>
hsc/hsc
hsctools/hscdepp
hsctools/hscpitt
</PRE>

<P>These are the executables. Congratulations. Mark the current day in
your calendar with a red circle. After that, follow the <A
HREF="install.html">installation instructions</A>. (In short: put the
binaries somewhere in your search-path, and setup hsc.prefs in the
appropriate place.)</P>

<H2>Cleaning Up</H2>

To remove the object files, use

<PRE>make clean</PRE>

To also remove binaries, temporary files, core dumps and other trash, use

<PRE>make sterile</PRE>

In ideal case, the <FILE>source</FILE> directory should then be in
exactly the same state as it was before you started with the
compilation.

<H2>Basic Test Run</H2>

Now it is probably a good time to check if <hsc> basically runs without
crashing. Later in this chapter you will read how to do a more exhaustive
test run, but for now simply type this into your command line:

<PRE>hsc/hsc test/simple.hsc to test/simple.html status=full prefsfile=../hsc.prefs</PRE>

If there now is a file <FILE>test/simple.html</FILE> which looks
pretty much the same as <FILE>test/simple.hsc</FILE>, and the output
on the display looks something like

<PRE>hsc - HTML sucks completely, v0.XXX (dd-mmm-yy)
(C) T.Aglassinger/Tommy-Saftw&ouml;rx. Freeware, type `hsc LICENSE' for details.
../hsc.prefs: preferences read
test/simple.hsc (9)</PRE>

<hsc> passed this test. In this case, you can continue reading at the
next heading.

<P>But if you now see something like</P>

<PRE>*** atexit() failed: Unknown error code</PRE>

<P>on your display, you are most likely running under <Nextstep>.
There it seems to be a known bug of the ANSI-C function
<CODE>atexit()</CODE> to return with in error code even in case of
success. However, nobody ever cared about fixing this, and even in
OpenStep 4.2 this bug still seems to be there.</P>

<P>As I do not care about faulty implementations of basic ANSI-C
functions, you will have to change the source code to make it ignore
the result of <CODE>atexit()</CODE>. Load
<file>source/hsc/hsc.c</file> into your editor and change the line</P>

<PRE>if (atexit(func))</PRE>

into something like

<PRE>if (atexit(func) &amp;&amp; 0)</PRE>

<P>to still call this function and add the additional exit-function,
but ignore any errors returned by it. At least this is the work-around
suggested to me. And no, there will not be a <CODE>#ifdef
NEXTSTEP</CODE> for this stupid bug in the runtime library.</P>

<H2>About Makefile.agi</H2>

<P>If you sneaked around about in the <FILE>source</FILE> directory,
you maybe also stumbled across a file called
<FILE>Makefile.agi</FILE>.</P>

<P>Actually I don't use the normal <Makefile> described in the early
chapters above for developing. The reason should be obvious, if you
watched your compiler before. It only has been invoked a few times,
but the there are several small source files.</P>

<P>This is simply because every sub-part of this package has a
file which includes all corresponding small files into a single
big one. The compiler will have to work quite a while on this,
and it will take much more resources - especially memory.</P>

<P>But the optimizer will be more efficient (shorter and faster code),
and, most important, there are no problems with any library managers,
complex and hardly portable rules in the <Makefile> and so on.</P>

<P>If you just want to compile it once, this is fine for you. However,
if one still develops on this package, and has to change and compile
sources very often, this is not acceptable: A small change will cause
the compiler to recompile large parts, lasting a very long time.</P>

<P>Therefor <Makefile.agi> uses a library manager to avoid this. It
is easy for me to make sure that this specific <Makefile> runs on my
machine, with the <make> tool installed on my machine. But it might not
necessarily work with yours.</P>

<P>But the standard <makefile> (probably) did. So that's why.</P>

<P>However, there might be one reason why you still want to partially
use it:</P>

<H2>Performing a Test Run</H2>

<P>To test if hsc works as expected, some test data are included in
<FILE>source/test/</FILE>. Please note that this material might not be
up to date for pre-releases you have downloaded from the
support-w3-page. Some of the tests might file in such releases.</P>

<P>The <CODE>test</CODE> rule is only declared in <makefile.agi>. This
<makefile> uses some features not supported by all <make> tools. It
has been designed to work with <EXEC>GNUmake</EXEC>, which is freely
available together with it's source code (see <ln-related>).</P>

<P>To initiate the test sequence, simple type</P>

<PRE>make -f Makefile.agi test</PRE>

<P>from the same directory you have started the compilation before.</P>

<P>Technically speaking, the test run does the following: it will
invoke <hsc> several times: some <FILE>.hsc</FILE> files will be used
to create some <FILE>.html</FILE> files. These are compared with
<FILE>.expected</FILE> files, which have been created before and
contain the expected output.</P>

<P>For this comparison, <EXEC>diff --brief</EXEC> will used. Make sure
that such a command is available before starting the test process
(again, see <ln-related> if you do not have it).</P>

<P>Additionally, <hsc> will redirect messages to <FILE>.msg</FILE>
files. These will be compared with <FILE>.mex</FILE>
files(<qq>messages expected</qq>) the same way as described above.</P>

<P>If all works fine, `diff' should not produce any output.</P>

<P>But you should be aware that the test run also includes some faulty
input data to test if hsc is able to cope with certain error
situations. These will result in an output like</P>

<PRE>
testing test/unknattr.hsc (messages only)..
hsc/hsc failed returncode 10
make: [test/unknattr.msg] Error 10 (ignored)
</PRE>

<P>and should not confuse you. Only an error reported by
<EXEC>diff</EXEC> counts as a failed test run.</P>

<H2><A NAME="debug">Debug Mode</A></H2>

<P>These paragraphs are most likely only interesting for freaky
people. Normally you do not want to compile <hsc> in debug mode.
Therefor I am not going to explain many details about it, but it
should be sufficient for those it is directed to.</P>

<P>In debug mode, <hsc> performs some additional plausibility checks.
Most remarkable, it will check for unreleased resources and trashed
memory. It will also output some statistic at the end of every
run.</P>

<P>To compile it for this mode, you have to define two symbols called
<CODE>DEBUG</CODE> and <CODE>DEBUG_UGLY</CODE>. After that, you can
use the commandline option <KBD>-DEBUG</KBD> to enable this additional
output.</P>

<P>It does not make sense to run <hsc> in debug mode all the time, as
it is (even more) horrible slow in it. You should only use this if you
want to trace internal error messages, some completely unreasonable
behavior or crashes.</P>

<P>If you still can not get enough of debugging information, you can
take a look at the source files <FILE>hsclib/ldebug.h</FILE> and
<FILE>ugly/udebug.h</FILE>, where you can set some additional
flags to <CODE>1</CODE>.</P>

</DOCPAGE>
