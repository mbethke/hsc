<WEBPAGE chapter="hsc - " title="Installation"
         PREV="features/rplcent.html" NEXT="options.html">

This chapter will explain where all the files coming with the
archive should end up.

<H2>Simple installation</H2>

<P>It's recommended to leave the whole <hsc> directory at one piece.
Therefor extract the archive somewhere to your hard disk,
for example to <FILE>work:hsc</FILE>.</P>

You now have to add this directory to your workbench search path.
This can be performed adding a line like the one you can find
below to your <FILE>user-startup</FILE>

<PRE>    Path work:hsc ADD</PRE>

After rebooting, you can open a CLI and type in <qq><KBD>hsc help</KBD></qq>,
and <hsc> should come up with a short information message.

<H2>Minimum installation</H2>

<P>For a minimum installation, only <FILE>hsc</FILE> and <FILE>hsc.prefs</FILE>
are required. If you want to utilize the project management capabilities of 
<hsc>, also <hscdepp> and <hscpitt> are required.</P>

<P>Copy the binaries to somewhere in your workbench search path (for 
exmple, to <FILE>c:</FILE>) and <hsc.prefs> anywhere else and set
<env_hscpath> according to this.</P>

<H2>Making hsc resident</H2>

<P>All binaries should already have the the pure bit set. If not, you
can do this by entering</P>
<PRE>    protect hsc/hsc add p
    protect hsc/hscdepp add p
    protect hsc/hscpitt add p</PRE>
<P>in CLI.</P>

<P>To make the binaries resident on every startup, add the lines</P>
<PRE>    resident hsc
    resident hscdepp
    resident hscpitt</PRE>
<P>to the <FILE>user-startup</FILE>.</P>

<P><STRONG>Important:</STRONG> For resident programs, <CODE>PROGDIR:</CODE>
is not defined. You will have to take care that <hsc> is able to find
<hsc.prefs> before it tries to scan <CODE>PROGDIR:</CODE> for it. This
can be done by by setting the environment variable <env_hscpath> or by
placing <hsc.prefs> in the current directory for every project.</P>

</WEBPAGE>
