<DOCPAGE chapter="hsc - Features - " title="URIs"
    PREV="checkuri.html"
    NEXT="rplcent.html">

This chapter shortly describes how far the different kind of URIs
are supported by <hsc>, and also introduces a new kind.

<H2>Local URIs</H2>

Local URIs refer to objects which are located on the same machine
as the document currently processed by <hsc>. Internally they are
simply remapped to filenames, which will be access by <hsc> to for
example <A HREF="checkuri.html">validate links</A> or to add
<A HREF="getsize.html">attributes for image sizes</A>.

<H3>Relative URIs</H3>

<P>This kind of URI specifies a local object relative to the current
directory (unless you have set a <TG>BASE HREF=..</TG>, which is not
recommended; see below). If you want to refer to an object located in
the parent directory, you can use <qqc>../</qqc> as prefix.</P>

<H3><A NAME="prjrel">Project Relative URIs</A></H3>

<P>Sometimes, when your project starts to become rather complex, you
create subdirectories to structure the whole thing. As example, I
prefer to create a directory <CODE>image/</CODE>, where all images for
buttons and logos are placed.</P>

<P>Creating a link to an image from a document in the project root
directory is no problem, it's URI simply is
<CODE>image/logo.gif</CODE>. But if your current page is somewhere
deeper in your project path, eg. <CODE>people/hugo/hugo.html</CODE>,
you need to refer to the same image using
<CODE>../../image/logo.gif</CODE>. So you always have to know the
directory level of your current page.</P>

<P>One solution would be to define a <TG>BASE HREF="<VAR>path to main
document</VAR>"</TG>. But then, all your links have to be relative to
the directory you have specified with the <CODE>HREF</CODE> attribute.
This is very annoying if you need to refer to files located in the
same directory.</P>

<P>But, thanks to <hsc>, here's the solution to this problem: if you
do not define a <TG>BASE</TG>-URI, all relative links are processed as
usual. Only if the URI starts with a <colon>, it refers to a file
relative to the root directory of your project.</P>

<P>For example, an <TG>IMG SRC=":image/back.png" ALT="back"</TG> will
always refer to the same image, regardless whether this tag is called
from a file called <FILE>welcome.hsc</FILE> or
<FILE>people/hugo/hugo.hsc</FILE>. The difference can only be
recognised in the HTML objects: for the first case, an <TG>IMG
SRC="image/back.png" ALT="back"</TG> will be created. For the second
one, it will be <TG>IMG SRC="../../image/back.png"
ALT="back"</TG>.</P>

<P>For the HTML document, these URIs will be translated to relative URIs
as described above.</P>

<H3><A NAME="serrel">Server Relative URIs</A></H3>

<P>Different to the project relative URIs described above, server
relative URIs are not a feature of <hsc>, but of the standard URI
schema. As they usually only cause problems (especially when reading
documents locally with an off-line browser), you will not find any
further information about them here. Although the idea behind them
would not be so bad at all, you should try to avoid them within
ridiculous linking model like the one of the w3, as this more or less
only defines a filecopy operation across networks.</P>

<P>For those who really can not live without them, the commandline option
<op-serverdir> adds some basic support. With this option set to a
valid directory, such URIs will be converted to relative URIs
internally, and most features of <hsc> will work with them as usual.
However they will show up in the HTML document with their original
value, with all problems remaining for other applications.</P>

<H2><A NAME="abs">Absolute URIs</A></H2>

These are used to refer to objects which are located on different
servers. Examples are <qqc>http://www.playboy.com/</qqc> or
<qqc>ftp://wuarchive.wustl.edu/pub/aminet/</qqc>. As <hsc> acts only
on objects which can be found on the local disk and does not establish
any network connections, several features like like link validation or
the commandline option <op-getsize> will not work on such URIs.

<H2><A NAME="id">Fragment Identifers</A></H2>

<P>For html, they point to a specific location in a document and can
be created using <TG>A NAME=..</TG> and are referenced liked <TG>A
HREF="file.html#id"</TG>. Normally, <hsc> will only validate
references within the current document. If you refer to an ID within
another document, only the existence of the corresponding file at all
will be validated.</P>

<P>This changes if you start to utilize the <A
HREF=":project/index.html">project management capabilities</A>. On
demand, <hsc> can remember which IDs have been specified within a
document, and will utilise these data to validate references to IDs,
too.</P>

</DOCPAGE>

