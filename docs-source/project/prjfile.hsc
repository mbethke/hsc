<WEBPAGE chapter="hsc - Project managment - " title="The Project file"
    BACK="index.html"
    PREV="makefile.html"
    NEXT="hscdepp.html">

<H2>Introduction</H2>

Optionally, <hsc> can maintain a project-file, where it stores
some information about the document it currently processes.
This includes
<UL>
<LI>name of the html-object created
<LI>name of the main hsc-source used
<LI>names of all files include (both via user arguments or <ln_include>)
<LI>local IDs defined in this document (using <TG>A NAME="id"</TG>)
</UL>

This enables <hsc> to check references to local IDs in other documents 
of your project. Additionally, you can now use <ln_hscdepp> to maintain 
the dependencies between hsc-source and html-object and update your
<makefile>.

<H2>How to get started with it</H2>

<P>To make <hsc> keep the project-file up-to-date, you have to use the
CLI-option <CODE>PRJFILE=<FILE>projec_file</FILE></CODE>. It doesn't
matter how you name your project-file, however I suggest to call it
<FILE>hsc.project</FILE>; this is the default-name <hscdepp> asumes.</P>

<P>But it's very important to use the <EM>same</EM> name for the
project-file every time you invoke <hsc>. Otherwise, <hsc> and <hscdepp>
won't be able to find all required information and will launch warnings
or create incomplete dependencies.</P>

For example,you can invoke <hsc> using
<PRE>
   hsc FROM=hugo.hsc TO=html: PRJFILE=hsc.project
</PRE>

<P>If the project-file does not yet exist, <hsc> displays a warning.
You can ignore this, and the project-file will be created after
the (successful) creation of the html-object. The next time you 
start <hsc> passing the same project-file, no warning will appear.</P>

</WEBPAGE>
