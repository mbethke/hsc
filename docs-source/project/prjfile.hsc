<WEBPAGE chapter="hsc - Project Management - " title="The Project File"
    BACK="index.html"
    PREV="index.html"
    NEXT="hscpitt.html">

<H2>Contents</H2>

The project-file, <hsc> stores some information about the document it
just processed. This includes

<UL>
<LI>name of the document (html-object) created
<LI>name of the main hsc-source used
<LI>names of all files include (both via user arguments or <ln_include>)
<LI>local IDs defined in this document (using <TG>A NAME="id"</TG>
    or <TG><I>tag</I> ID="id"</TG>)
</UL>

<P>This enables <hsc> to check references to local IDs in other
documents of your project. Additionally, you can now use <hscdepp LINK>
to maintain the dependencies between hsc-source and html-object, and
update a <makefile> according to them.</P>

<P>Another tool called <hscpitt LINK> allows you to examine, add and
remove documents from the project-file.</P>

<H2>Naming The Project-File</H2>

It doesn't matter which name you give the project-file, but I suggest
to call it <FILE>hsc.project</FILE>; this is the default name both
<hscpitt> and <hscdepp> assume, and you do not have to specify another
command line option when invoking them.

<P>But it's very important to use the <EM>same</EM> name for the
project-file every time you invoke <hsc>. Otherwise, <hsc> and
<hscdepp> won't be able to find all required information and will
launch warnings or create incomplete dependencies.</P>

<H2>Using The Project-File</H2>

<P>To create an empty project-file, you have to <hscpitt> with the command
<CODE>ADD</CODE>. To make <hsc> maintain it, you have to specify the
CLI-option <CODE>PRJFILE</CODE> when invoking it.</P>

<P>If you want to utilize the information stored in the project-file
outside <hsc>, you can use <hscpitt> with the command
<CODE>extract</CODE> and for instance parse it with a short program
written in some scripting language. See <hscpaltrow LINK> for an
example.</P>

</WEBPAGE>
