<WEBPAGE chapter="hsc - Project Management - " title="Hscpaltrow"
    BACK="index.html"
    PREV="hscdepp.html"
    NEXT=":usecases.html">

<STRONG>hscPitt's ALternative ThROW-out</STRONG>

<P><hscpaltrow CAP> is a sample ARexx-Script which explains how to utilize
the output of <hscpitt LINK> so that you can retrieve all information
stored in the project file.</P>

<P><hscpaltrow CAP> is placed in the public domain. Use at your own
risk.</P>

<hscpaltrow CAP> is dedicated to Gwynneth Paltrow.

<H2>Options</H2>

There is only a single required option which denotes the project file
to examine. If you pass no arguments, a short usage information will
be displayed.

<H2>Usage</H2>

<P>This script starts <hscpitt LINK> with <KBD>COMMAND=EXTRAXT</KBD>,
stores the output in a temporary file, and parses it so you can access
all data from the script. For future compatibility, unknown data
are ignored.</P>

Basically, you just have to invoke
<PRE>    hscpaltrow hsc.project</PRE>
This script is not designed to do anything useful with the data
obtained, it just displays them.

<P>Feel free to modify it, but don't forget to rename it after that,
or it will be overwritten when updating your hsc-directory with a new
release.</P>

You can find this script in <A
HREF=":../grafflwerk/hscpaltrow.rexx"><FILE>grafflwerk/hscpaltrow.rexx</FILE></A>.

</WEBPAGE>

