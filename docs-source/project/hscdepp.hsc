<DOCPAGE chapter="hsc - Project Management - " title="Hscdepp"
    BACK="index.html"
    PREV="makefile.html"
    NEXT="hscpaltrow.html">

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string>
<DT>
<$stripws>
<** create named reference **>
<$IF COND=(SET NAME)>
    <A NAME=(Name)>
<$ELSE>
    <A NAME=(Title)>
</$IF>
<$stripws>
<KBD><(Title)></KBD></A>
<DD>
</$MACRO>

<STRONG>hsc DEPendency Procreator</STRONG>

<P><hscdepp CAP> extracts dependencies from <hsc>'s project-file
and stores them in a <makefile>. But not all parts of the
<makefile> are overwritten: the relevant dependencies for
<hscdepp> are marked with taglines, so the next time only
this part is manipulated and the rest is left untouched.</P>

<P><hscdepp CAP> is part of the hsc-distribution and therefor the
same legal issues apply.</P>

<hscdepp CAP> is dedicated to Johnny Depp.

<H2>Options</H2>

<DL>
<EXPLAN TITLE="HELP=-h=?/S">
Display short help message and exit.

<EXPLAN TITLE="LICENSE/S">    
Display a short version of <A HREF=":copy.html">Copyright</A>
and exit.

<EXPLAN TITLE="FILE" NAME="file">
Specifies makefile to be manipulated. If none given,
<hscdepp> searches for <FILE>GNUmakefile</FILE>, <FILE>makefile</FILE>
or <FILE>Makefile</FILE>. If it doesn't succeed in any of
these, a new one named <FILE>Makefile</FILE> will be
created.

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
Specifies project-file to be scanned for dependencies. If
none given, <FILE>hsc.project</FILE> is used.

<EXPLAN TITLE="NAMEALL/K" NAME="nameall">
All document rules are collected together in a rule by default
called <qq><CODE>all_hsc</CODE></qq>. This is used to update
all documents using a simpe <KBD>make all_hsc</KBD>, or, even
more useful, to be part of your <CODE>all</CODE> rule, which
usually depends on all other things that are maintained by
the <makefile>. This option changes the name of this rule.

<EXPLAN TITLE="VERBOSE/S">
Verbose output

<EXPLAN TITLE="NOBACKUP/S">
Do not create backup of original <makefile>.

<EXPLAN TITLE="NOTAGLINES/S">
Do not write taglines that identify beginning/ending of
dependencies. This should only be used if you keep the
dependencies in a separate file.
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
makefile to <FILE>hsc_makefile</FILE>.usr/bin/vim
<DT><KBD>hscdepp</KBD>
<DD>Does the same as above, but uses <FILE>makefile</FILE> and
<FILE>hsc.project</FILE> as default.
</DL>

</DOCPAGE>
