<WEBPAGE chapter="hsc - Features - " title="Absolute URIs"
    PREV="checkuri.html"
    NEXT=":macro/macros.html">

<P>Sometimes, when your project starts to become rather
complex, you create subdirectories for to structure the
whole thing. As example, I prefer to create a directory
<CODE>image/</CODE>, where all images for buttons and logos
are placed.</P>

<P>Creating a link to an image from the main-directory is no
problem, it's URI simply is <CODE>image/logo.gif</CODE>. But
if your current page is somewhere deeper in your
project-path, eg <CODE>people/hugo/hugo.html</CODE>, you
need to refer to the same image using
<CODE>../../image/logo.gif</CODE>. So you always have to
know the directory-level of your current page.</P>

<P>One solution would be to define a <TG>BASE="<VAR>path to
main document</VAR>"</TG>.
But then, all your links have to be absolute. This is very annoying
if you need to refer to files located in the same directory.</P>

<P>But, thanks to <hsc>, here's the solution to this problem: if you
don't define a <TG>BASE</TG>-URI, all relative links are
processed as usual. Only if the URI starts with a colon (":"), it
refers to a file relative to the main-path of your project.</P>

<P>For example, an <TG>IMG SRC=":image/back.gif"
ALT="back"</TG> will always refer to the same image,
regardless wheter this tag is called from a file called
<FILE>welcome.hsc</FILE> or
<FILE>people/hugo/hugo.hsc</FILE>. The difference can only
be recognised in the html-objects: for the first case, an
<TG>IMG SRC="image/back.gif" ALT="back"</TG> will be
created. For the second one, it will be <TG>IMG
SRC="../../image/back.gif" ALT="back"</TG>.</P>

</WEBPAGE>

