<WEBPAGE chapter="hsc - " title="File Options"
    PREV="index.html" NEXT="copy.html">

There are several CLI options that specify where <hsc> should take
the input from and where the output should be placed. This section
describes the differences and (dis-)advanteges.

<H2>Converting single files</H2>

This one uses a single file for input and output:
<PRE>
    hsc FROM hugo.hsc TO hugo.html
</PRE>
It will use <FILE>hugo.hsc</FILE> and outputs to <FILE>hugo.html</FILE>.
Both input and output will be located in the current directory. This can
be useful if you only want to quickly process a single file trough <hsc>,
and do not want to set up a complete project.

<H2>Using different directories for input and output</H2>

<P>One of the features of <hsc> is that your object can be located at
a totally different place from the source. This enables you to
place your sources at a different disk as the one the WebServer
has access to, saving server space.</P>

For instance, you can use
<PRE>
    hsc FROM hugo.hsc TO www:sepp/hugo.html
</PRE>

<P>to use <FILE>hugo.hsc</FILE>, located in the current direcory, and
create <FILE>www:sepp/hugo.html</FILE>. Note that all images,
other documents etc. you reference fromout <FILE>hugo.hsc</FILE> using
relative URIs have to be located at the corresponding place within
the directory <FILE>www:sepp/</FILE>.</P>

<P>For example, if <FILE>hugo.hsc</FILE> contains a tag
<$source pre>
    <IMG SRC="image/back.gif" ALT="back">
</$source>
the image file <FILE>back.gif</FILE> will have to be located at
<FILE>www:sepp/image/back.gif</FILE>.

<H2>Using a destination directory</H2>

The same as above can be performed using
<PRE>
     hsc FROM hugo.hsc TO www:sepp/
</PRE>
<P>Note the trailing slash that tells <hsc> that it should now output
to a directory and determine the filename by itself. Usually, it
will use <CODE>(input filenname, without ".hsc", but a ".html"
appened)</CODE>, resulting into <FILE>www:sepp/hugo.html</FILE>
as before. You can change this using the CLI option 
<op_extension>.</P>

<H2>Using sub directories</H2>

<P>For complex project, usually not all documents are located at the
same directory. For example, <FILE>hugo.hsc</FILE> is no more
placed in the current directory, but in 
<FILE>people/hugo.hsc</FILE>.</P>

Invoking hsc like
<PRE>
    hsc FROM people/hugo.hsc TO www:sepp/
</PRE>
<P>will now ouput to <FILE>www:sepp/people/hugo.html</FILE>. Note that you
have to take care that the directory <FILE>www:sepp/people/</FILE>
exists, <hsc> will not create any new directories by itself.</P>

<P>If you now want to embed an image located at 
<FILE>www:sepp/image/back.gif</FILE>, you have to use
<$source pre>
    <IMG SRC="../image/back.gif" ALT="back">
</$source>
Alternatively, you can use and <fe_absuri>:
<$source pre>
    <IMG SRC=":image/back.gif" ALT="hugo">
</$source>

Another way to obtain the same result is using
<PRE>
    hsc FROM people/hugo.hsc TO www:sepp/people/hugo.html
</PRE>
<P>In this case, <hsc> is smart enough to figure out that the
destination directory is <FILE>www:sepp/</FILE>, and you
are using <FILE>people/</FILE> as subdirectory.</P>

But if you try to use
<PRE>
    hsc FROM people/hugo.hsc TO www:sepp/stuff/hugo.html
</PRE>
<hsc> can not figure out what to use as destination- or
subdirectory, and will show up an error message:
<PRE>
    unmatched corresponding relative directories:
      input  `people/'
      output `stuff/'
</PRE>
In this case, you will have to rename your directories.

<H2>Using pipes</H2>

<P>Short: It is possible to use pipes with <hsc>, but it should be
avoided in most cases; <hsc> isn't really <qq>pipable</qq>. It does not 
continuously read data from the input and write parts of the output.</P>

<P>(<Technote>It reads the whole
input file with a single call to <CODE>fread()</CODE>, creates the
output in memory and writes it with (more or less) a single 
<CODE>fwrite()</CODE>. The main reasons why it works this way are:
I hate to check for I/O errors, <CODE>fungetc()</CODE> usually does not
work after a linefeed and
memory mapped files are not supported by the standard ANSI library.)</P>

<P>Additionally, it impossible for <hsc> to maintain a project file
without knowledge of the filenames for document and source, so 
several features will be disabled.</P>

<P>If <hsc> needs to access relative URIs, it will have to use the
current directory as starting point.</P>

<P>Therefor, pipes should only be used if you quickly want to test
a feature on a single file or something like that.</P>

Anyway, here is an example:

<PRE>    hsc STDIN</PRE>

This specifies the (case sensitive) pseudo-filename <KBD>STDIN</KBD> 
as input, which will use <stdin>. 
Missing a <KBD>TO</KBD> option, the output will be written to <stdout>.
Now using this like

<$source PRE>    hsc STDIN <hugo.hsc >hugo.html</$source>

would be equal to
<PRE>    hsc hugo.hsc TO hugo.html</PRE>
Again: Try to avoid this.
</WEBPAGE>
