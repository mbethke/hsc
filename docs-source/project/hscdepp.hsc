<WEBPAGE chapter="hsc - Project managment " title="hscdepp"
    BACK="index.html"
    PREV="prjfile.html"
    NEXT=":index.html">

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string>
<** create named reference **>
<$IF COND=(SET NAME)>
<A NAME=(Name)><$ELSE>
<A NAME=(Title)></$IF>
<DT><** create heading **>
<KBD><(Title)></KBD></A>
<DD>
</$MACRO>

<P><STRONG>dependency procreator for hsc-sources</STRONG></P>

<P><hscdepp> extracts dependencies from <hsc>'s project-file
and stores them in a <makefile>. But not all parts of the
<makefile> are overwritten: the relevant dependencies for
<hscdepp> are marked with taglines, so the next time only
this part is manipulated and the rest is left untouched.</P>

<P><hscdepp> is part of the hsc-archive and therefor the
same legal issues apply.</P>

<hscdepp> is dedicated to Johnny Depp.

<H2>Options</H2>

<DL>
<EXPLAN TITLE="HELP">
Display short help.

<EXPLAN TITLE="FILE=<I>makefile</I>" NAME="file">
Specifies makefile to be manipulated. If none given,
<hscdepp> searches for <FILE>GNUmakefile</FILE>, <FILE>makefile</FILE>
or <FILE>Makefile</FILE>. If it doesn't succeed in any of
these, a new one named <FILE>Makefile</FILE> will be
created.

<EXPLAN TITLE="PRJFILE=<I>project-file</I>" NAME="prjfile">
Specifies project-file to be scanned for dependencies. If
none given, <FILE>hsc.project</FILE> is used.

<EXPLAN TITLE="VERBOSE">
Verbose output

<EXPLAN TITLE="NOBACKUP">
Do not create backup of original <makefile>.

</DL>

<H2>Examples</H2>

<DL>
<DT><KBD>hscdepp HELP</KBD>
<DD>Displays help and exits.
<DT><KBD>hscdepp FILE=hsc_makefile PRJFILE=my_hsc_project VERBOSE</KBD>
<DD>Reads the file <FILE>hsc_makefile</FILE>, reads
project-information from file <FILE>my_hsc_project</FILE>,
creates a backup of the original makefile in
<FILE>hsc_makefile.bak</FILE> and writes the updated
makefile to <FILE>hsc_makefile</FILE>.
<DT><KBD>hscdepp</KBD>
<DD>Does the same as above, but uses <FILE>makefile</FILE> and
<FILE>hsc.project</FILE> as default.
</DL>

</WEBPAGE>
