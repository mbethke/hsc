<WEBPAGE chapter="hsc - Project managment " title="hscpitt"
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

<STRONG>hsc Project Interfering and Trashing Tool</STRONG>

<P><hscpitt> can be used to extract and manipulate data stored
in <hsc>'s project file. It lists all documents stored in the
project and removes entries and correspoding files for 
documents.</P>

<P><hscpitt> is part of the hsc-distribution and therefor the
same legal issues apply.</P>

<hscpitt> is dedicated to Brad Pitt.

<P><STRONG>Important</STRONG>: <hscpitt> should be used with caution.
It asumes that the user knows what he is doing and does <EM>not</EM>
ask back before performing any action. Using <hscpitt> it is rather
easy to remove information within seconds that required hours
being implemented.</P>

<H2>Options and switches</H2>

<DL>
<EXPLAN TITLE="HELP=-h=?/S">
Display short help message and exit.

<EXPLAN TITLE="LICENSE/S">    
Display a short version of <A HREF=":copy.html">Copyright</A>
and exit.

<EXPLAN TITLE="COMMAND" NAME="command">
Command to be applied to project. Supported commands are:
<UL>
<LI><KBD>INFO</KBD> - show general info about project
<LI><KBD>LIST</KBD> - show list of documents 
<LI><KBD>EXTRACT</KBD> - show detailed list of all documents 
    currently stored in project
<LI><KBD>DELETE</KBD> - remove documents entries from 
    project, but leave corresponding files intact  
<LI><KBD>ERASE</KBD> - remove documents entries from 
    project and also corresponding document and source file.  
</UL>

For a more detailed explanation of these commands see below.

<EXPLAN TITLE="ARG/M">
Arguments for command. Depends on the value of <KBD>COMMAND</KBD>.

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
Specifies project file to be examined/manipulated. If
none given, <FILE>hsc.project</FILE> is used.

<EXPLAN TITLE="QUIET/S">
Act quietly, do not display progress and status messages.

<EXPLAN TITLE="-DEBUG/S">
Enable debug mode; only usefull if compiled in debug mode.
</DL>

<H2>Commands</H2>
<H3>COUNT</H3>
Setting <KBD>COMMAND=COUNT</KBD> will output the number of
documents currently stored in the project file.
<H3>LIST</H3>
<P>Setting <KBD>COMMAND=LIST</KBD> will output a short
list of documents to <stdout>. Every line contains the
name of a html object.

<H3>EXTRACT</H3>
<P>Setting <KBD>COMMAND=EXTRACT</KBD> will output a detailed
list of documents to <stdout>. This command is ment to be used 
if you want to utilize the information stored in the project 
file. Just redirect the output of <hscpitt> and scan it by
your program.</P>

<P>All lines match the template
<BLOCKQUOTE>
    <I>identifier</I><CODE>="</CODE><I>value</I><CODE>"</CODE>
</BLOCKQUOTE>
You <STRONG>must not</STRONG> make any assumptions about the
sequence of identifiers. Your programm should be able
to skip unknown identifiers. At the end of data about a
document, an <CODE>EOF</CODE> occures or an empty line with
another document succeeding.</P>
 
Currently, possible identifers are:
<DL>
<DT>DOCUMENT
<DD>URI of document, relative to current directory
<DT>SOURCE
<DD>Filename of main source file used to create document, 
    relative to current directory
</DL>

<H3>DELETE</H3>
Setting <KBD>COMMAND=DELETE</KBD> will remove the entries
specified in <KBD>ARG</KBD> from the project file. The document
and the corresponding source remains untouched.

<H3>ERASE</H3>
Setting <KBD>COMMAND=ERASE</KBD> will remove the entries
specified in <KBD>ARG</KBD> from the project file. Different to
<KBD>COMMAND=DELETE</KBD>, it will also <EM>remove</EM>
the files for document and source. This command should be used
with caution.

<H2>Examples</H2>
<DL>
<DT><KBD>hscpitt HELP</KBD>
<DD>Displays help and exits.
<DT><KBD>hscpitt PRJFILE=sepp.project COMMAND=LIST</KBD>
<DD>List all documents currently stored in project file 
    <FILE>sepp.project</FILE>.
<DT><KBD>hscpitt EXTRACT</KBD>
<DD>List information about all documents currently stored in project file 
    <FILE>hsc.project</FILE>. 
<DT><KBD>hscpitt COMMAND=DELETE www:index.html www:sepp/hugo.html</KBD>
<DD>Remove entries for the documents <FILE>www:index.html</FILE> and 
    <FILE>www:sepp/hugo.html</FILE> from the project file. The html object
    and hsc sources for these documents will not be removed.
</DL>

</WEBPAGE>
