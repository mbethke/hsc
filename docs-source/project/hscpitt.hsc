<WEBPAGE chapter="hsc - Project Management - " title="Hscpitt"
    BACK="index.html"
    PREV="prjfile.html"
    NEXT="make.html">

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string>
<** create named reference **>
<DT>
<$stripws>
<$IF COND=(SET NAME)>
    <A NAME=(Name)>
<$ELSE>
    <A NAME=(Title)>
</$IF>
<$stripws>
<KBD><(Title)></KBD></A>
<DD>
</$MACRO>

<*** render heading and link for pitt-command ***>
<$macro pittCmd NAME:string/required>
<H3><A NAME=(Name)><(Name)></A></H3>
</$macro>

<$macro pittCmdNote NAME:string/required NOTE:string/required>
<A HREF=("#"+Name)><KBD><(Name)></KBD></A> - <(Note)>
</$macro>

<STRONG>hsc Project Interfering And Trashing Tool</STRONG>

<P><hscpitt CAP> can be used to extract and manipulate data stored
in <hsc>'s project file. It lists all documents stored in the
project and removes entries and corresponding files for
documents.</P>

<P><hscpitt CAP> is part of the hsc-distribution and therefor the
same legal issues apply.</P>

<hscpitt CAP> is dedicated to Brad Pitt.

<P><STRONG>Important</STRONG>: <hscpitt> should be used with caution.
It assumes that the user knows what he is doing and does <EM>not</EM>
ask back before performing any action. Using <hscpitt> it is rather
easy to remove information within seconds that required hours
being implemented.</P>

<H2>Options And Switches</H2>

<DL>
<EXPLAN TITLE="HELP=-h=?/S">
Display short help message and exit.

<EXPLAN TITLE="LICENSE/S">    
Display a short version of <A HREF=":copy.html">Copyright</A>
and exit.

<EXPLAN TITLE="COMMAND" NAME="command">
Command to be applied to project. Supported commands are:
<UL>
<*<LI><pittCmdNote NAME="INFO" NOTE="show general info about project">*>
<LI><pittCmdNote NAME="NEW" NOTE="create new project">
<LI><pittCmdNote NAME="LIST" NOTE="show list of documents">
<LI><pittCmdNote NAME="ADD" NOTE="add new document to project">
<LI><pittCmdNote NAME="DELETE"
                 NOTE=("remove documents entries from project, "
                      +"but leave corresponding files intact")>
<LI><pittCmdNote NAME="ERASE"
                 NOTE=("remove documents entries from project and"
                      +"also corresponding document and source file.")>
<LI><pittCmdNote NAME="EXTRACT"
                NOTE=("show detailed list of all documents "
                     +"currently stored in project")>
</UL>

For a more detailed explanation of these commands see below.

<EXPLAN TITLE="ARG/M">
Arguments for command. Depends on the value of <KBD>COMMAND</KBD>.

<EXPLAN TITLE="PRJFILE/K" NAME="prjfile">
Specifies project file to be examined/manipulated. If
none given, <FILE>hsc.project</FILE> is used.

<EXPLAN TITLE="QUIET/S">
Act quietly, do not display progress and status messages.

<EXPLAN TITLE="FORCE/S">
Certain commands refuse to overwrite already existing data.
Nevertheless with this switch enabled the will.

<EXPLAN TITLE="-DEBUG/S">
Enable debug mode; only useful if compiled in debug mode.
</DL>

<H2>Commands</H2>

<pittCmd NAME="NEW">
This command will create a new project file containing no documents.
If the project file already exists, this command will refuse to
overwrite it. You have to enable the switch <KBD>FORCE</KBD> to
accomplish this.

<pittCmd NAME="COUNT">
Setting <KBD>COMMAND=COUNT</KBD> will output the number of
documents currently stored in the project file.

<pittCmd NAME="LIST">
<P>Setting <KBD>COMMAND=LIST</KBD> will output a short
list of documents to <stdout>. Every line contains the
name of a html-object.

<pittCmd NAME="ADD">

<P>Setting <KBD>COMMAND=ADD</KBD> will add a new document with it's
corresponding main source to the project. The first <KBD>ARG</KBD>
contains the <EM>full</EM> relative filename of the html-object, the
second <KBD>ARG</KBD> contains the name of the main hsc-source that
will be used to create the document.</P>

If the document already is part of the project, an error message will
be displayed and the old document will be left untouched. To replace a
document, you will have to enable the switch <KBD>FORCE</KBD> before.

<pittCmd NAME="DELETE">
Setting <KBD>COMMAND=DELETE</KBD> will remove the entries
specified in <KBD>ARG</KBD> from the project file. The document
and the corresponding source remains untouched.

<pittCmd NAME="ERASE">
Setting <KBD>COMMAND=ERASE</KBD> will remove the entries
specified in <KBD>ARG</KBD> from the project file. Different to
<KBD>COMMAND=DELETE</KBD>, it will also <EM>remove</EM>
the files for document and source. This command should be used
with caution.

<pittCmd NAME="EXTRACT">
<P>Setting <KBD>COMMAND=EXTRACT</KBD> will output a detailed
list of documents to <stdout>. This command is mend to be used
if you want to utilize the information stored in the project 
file. Just redirect the output of <hscpitt> and scan it by
your program.</P>

<P>All lines match the template
<BLOCKQUOTE>
    <I>identifier</I><CODE>="</CODE><I>value</I><CODE>"</CODE>
</BLOCKQUOTE>
You <STRONG>must not</STRONG> make any assumptions about the
sequence of identifiers. Your program should be able
to skip unknown identifiers. At the end of data about a
document, an <CODE>EOF</CODE> occurs or an empty line with
another document succeeding.
 
<P>Currently, possible identifers are:
<DL>
<DT><CODE>DOCUMENT</CODE>
<DD>URI of document, relative to current directory
<DT><CODE>SOURCE</CODE>
<DD>Filename of main source file used to create document, 
    relative to current directory
<DT><CODE>INCLUDE</CODE>
<DD>Name of a file that has been included during processing
    the document. This identifier can show up more then once,
    if multiple files have been included.
</DL>

See also <A HREF="hscpaltrow.html"><hscpaltrow></A>
to learn how you can utilize this command.

<H2>Examples</H2>
<DL>
<DT><KBD>hscpitt HELP</KBD>
<DD>Displays help and exits.

<DT><KBD>hscpitt PRJFILE=sepp.project COMMAND=NEW</KBD>
<DD>Create a new project and store data in <FILE>sepp.project</FILE>.

<DT><KBD>hscpitt PRJFILE=sepp.project LIST</KBD>
<DD>List all documents currently stored in project file 
    <FILE>sepp.project</FILE>.

<DT><KBD>hscpitt ADD ../docs/sepp.html sepp.hsc</KBD>
<DD>Adds document <FILE>sepp.html</FILE>, which is created from
<FILE>sepp.hsc</FILE>. Note that <FILE>../docs/</FILE> denotes the
relative path, where the html-objects are located. This usually is
the same value you used as <qqc>TO</qqc> argument to invoke <hsc>.

<DT><KBD>hscpitt EXTRACT</KBD>
<DD>List information about all documents currently stored in project file 
    <FILE>hsc.project</FILE>. 

<DT><KBD>hscpitt COMMAND=DELETE www:index.html www:sepp/hugo.html</KBD>
<DD>Remove entries for the documents <FILE>www:index.html</FILE> and 
    <FILE>www:sepp/hugo.html</FILE> from the project file. The html-object
    and hsc-sources for these documents will not be removed.
</DL>

</WEBPAGE>
