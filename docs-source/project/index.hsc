<WEBPAGE chapter="hsc - " title="Project Management"
    PREV=":features/exec.html"
    NEXT="prjfile.html">

<P>As the amount of documents you are processing grows, so does
the amount of work required to maintain the whole bunch 
of your (wannabe-)information. To reduce waste of time for stupid
project management tasks, <hsc> is able to maintain a project file
and to utilize some additional tools.<P>

<H2>Overview</H2>

As there are several tools and concepts involved in project
management. Within the next paragraphs, the basic ideas will be
roughly described. If you already worked with a command line based
compiler environment and know how <Makefile>s and a dependency creator
work, there should not be many new things.


<P>If you did not, there is quite some work in front of you. For
details about the aforementioned things, you can refer to the chapters
listed below, but this is not meant to be an idiot-proof tutorial to
<make>.</P>

<UL>
<LI><A HREF="prjfile.html">Project Files</A> - where <hsc> stores the
    required information.
<LI><A HREF="hscpitt.html"><hscpitt></A> - <hsc>'s project
    interfering and trashing tool
<LI><A HREF="make.html"><make></A> - a general purpose tool for project management
<LI><A HREF="makefile.html"><Makefile>s</A> - how to adept them for <hsc>
<LI><A HREF="hscdepp.html"><hscdepp></A> - <hsc>'s dependency procreator
<LI><A HREF="hscpaltrow.html"><hscpaltrow></A> - utilizing the output
    of <hscpitt>.
</UL>

<H2>Creating A New Project</H2>

The first step is to create a project-file. This simply can be done by

<PRE>
hscpitt new
</PRE>

This will create an empty project in the current directory with no
documents in it. To see which documents are part of your project, you
can use

<PRE>
hscpitt list
</PRE>

or, if you do not want to see the header displayed:

<PRE>
hscpitt quiet list
</PRE>

<P>All these commands will use a default name, <qqc>hsc.project</qqc> for
the project-file. If you want to use a different name, you can use the
CLI-option <CODE>PRJFILE</CODE>, for example:</P>

<PRE>
hscpitt prjfile=sepp.project new
</PRE>

But note that then you will have to specify this option every time you
invoke <hscpitt> or <hscdepp>.

<H2>Adding New Documents To The Project</H2>

There are two ways to add a document to the project: the first is
<hsc>, because is will automatically add or update document entries in
the project-file <EM>after</EM> it successfully wrote a document to
the storage.

<P>To make <hsc> keep the project-file up-to-date, you have to use the
CLI-option <CODE>PRJFILE=<FILE>projec_file</FILE></CODE>.

For example,you can invoke <hsc> using
<PRE>
hsc FROM=sepp.hsc TO=html:sepp-pages/ PRJFILE=hsc.project
</PRE>


<H2>Maintaining The Makefile</H2>

Another possibility is for adding documents is <hscpitt>. For example:

<PRE>
hscpitt add html:sepp-pages/sepp.html sepp.hsc
</PRE>

will add the same document and main source file as before to the
project. This can be handy if you have a big and fat pattern rule with
many options that will take care of invoking <hsc> after a new
dependency has been added to the <makefile>. This relieves you from
typing in a long sequence of command line options.

<P>Nevertheless <hscpitt> leaves the <Makefile> untouched. Actually
you do not need a <Makefile> at all to use <hscpitt> - it only stores
information in project-file.</P>

<P>So if you invoke <make> right after <hscpitt>, nothing will have
changed. To translate the contents of the project-file into a list of
dependencies, you will have to use <hscdepp>. This tool simply creates
a dependency line for every document it finds in the project-file,
and stores this information in a <makefile>.</P>

For example, after the call to <hscpitt> done before, <hscdepp> will
create the dependency

<PRE>
html:sepp-pages/sepp.html : sepp.hsc
</PRE>

Or, if you would have used relative paths on a <NextStep> system like

<PRE>
hscpitt add ../../sepp-pages/sepp.html sepp.hsc
</PRE>

the dependency would look like:

<PRE>
../../sepp-pages/sepp.html : sepp.hsc
</PRE>

However, <hscdepp> does not care about the rules and options for hsc
you use in the <Makefile>. It only takes care of the dependencies, and
everything else is up to you.

<H2>When To Call Hscdepp</H2>

As all <hsc> and <hscpitt> are completely independent from <make>,
and <hscdepp> only provides an optional link between them. In other
words: <hsc> does not care if it is invoked from a <Makefile> or
not. And <make> does not care about the project-file, but only about
its own <Makefile>.

<P>Therefor every time something changed in the project-file, it is left
to the user to invoke <hscdepp> and also reflect these changes in the
<Makefile>.</P>

In general, it is a good idea to call <hscdepp> after..
<UL>
<LI>adding a new document document using <hscpitt> or <hsc>
<LI>successfully processing new a document with <hsc> the first time
    after adding it with <hscpitt>, because then it not only depends
    on its main source, but also on the files included during
    processing.
<LI>changing the include files of your pattern rule in the <Makefile>
<LI>once in a while.
</UL>

<P>Of course sometimes it can be convenient not to call <hscdepp>, for
example when you add multiple documents by calling <hscpitt> several
times, without invoking <hsc> or <make> meanwhile. Nevertheless,
invoking <hscdepp> too often should not cause any harm.</P>

<H2>Checking IDs</H2>

<P>The usage of the project-file also improves the functionality of
<hsc>. It will now also remember which IDs for link targets inside a
document have been defined (for example using html-code like <TG>A
NAME="id"</TG>).</P>

Now <hsc> will not only whine about code like

<$source PRE><A HREF="#unknown"></$source>

but also

<$source PRE><A HREF="existing.html#unknown"></$source>

with in both cases the ID <qqc>unknown</qqc> not being defined within
the document, but the document <FILE>existing.html</FILE> being in
place.

<P>Until all documents have been added to the project (either by <hsc>
or <hscpitt>), <ln_msg id="51"> ("no entry for document .. to check
id") might show up some times. This can be ignored.</P>

<P>Until all documents have been processed by <hsc> with a
<CODE>PRJFILE</CODE> set at least once (not only been added by
<hscpitt>) to the project-file), there will also be some occurrences
of <ln_msg id="74"> ("unknown id") you can ignore. This is because
even if the IDs are actually defined in the document, <hsc> does not
yet know about them because it did not store them in the project-file,
too.</P>

<H2>Alternative Approaches</H2>

<P>It should be emphasized that all of the above tools and concepts
are optional to <hsc>. You do not have to use <hscdepp> to maintain
your dependencies, but can also do manually, without any pattern rules
in the <Makefile>.</P>

<P>You even do not have to use <make>, but a different tool for
project management. Although then you will have too find a substitute
for <hscdepp>, if you still want to maintain the dependencies
automatically. Maybe <hscpaltrow> can help you with this.</P>

<P>Of course you can use <hsc> without a project-file at all, and for
example use a simple Shell- or Rexx-script to convert all your
documents, if there are only few of them.</P>

<H2>Example Projects</H2>

<* macro to insert name and link to example file *>
<$macro example-file dir:string/required name:string/required>
<A HREF=(":../examples/"+dir+"/"+name)><FILE><(name)></FILE></A>
</$macro>

There are some example project included, with most of them consisting
of only a few sample documents. To try them out, open a Shell and
change into the corresponding directory with the <Makefile>. View the
README if there are some preparations needed.

<P>Maybe you will also have to change the variables <CODE>HSC</CODE>
to point to the hsc-executable and <CODE>DESTDIR</CODE> to contain the
name of the parent directory, which differs from system to system. Use
<qqc>/</qqc> for AmigaOS or <qqc>../</qqc> for most other systems.</P>

<DL>
<DT>Fetzenschädl
<DD>This project is located in <FILE>examples/fetzenschaedl/</FILE>.
    There you will also find a
    <example-file name="README" dir="fetzenschaedl">
    and a
    <example-file name="Makefile" dir="fetzenschaedl">.

    Both html-document and hsc-source will be located in the same
    directory, which makes maintenence easier, but has several
    obstacles shortly discussed there. It uses a <Makefile>, but no
    project-file, so only <hsc> and <make> are required to try is out.

<DT>Simple
<DD>This project is located in <FILE>examples/simple/</FILE>.
    There you will also find a
    <example-file name="README" dir="simple">
    and a
    <example-file name="Makefile" dir="simple/source_hsc">.

    These are the same documents as before, but now html-objects will
    be located in a different directory you will have to create
    manually. Only a few changes to the <Makefile> are required to
    accomplish this.

<DT>Advanced
<DD>This project is located in <FILE>docs/source/</FILE> and contains
    the source code used to create this manual.

    See <A HREF=":../docs-source/README"><FILE>docs-source/README</FILE></A>.

    The <Makefile> used for this utilizes <hscdepp> and contains
    a pattern rule to also validate the created document with an
    external html-checker.

</DL>

</WEBPAGE>
