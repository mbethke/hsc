<WEBPAGE chapter="hsc - Features - " title="Absolute URIs"
    PREV="checkuri.html"
    NEXT="macros.html">

Sometimes, when your project starts to become rather complex,
you create subdirectories for to structure the whole thing.<P>

As example, I prefer to create a directory <CODE>image/</CODE>,
where all images for buttons and logos are placed.<P>

Creating a link to an image from the main-directory is no
problem, it's URI simply is <FILE>image/logo.gif</FILE>.<P>

But if your current page is somewhere deeper in your project-path,
eg <FILE>people/hugo/hugo.html</FILE>, you need to refer to the
same image using <FILE>../../image/logo.gif</FILE>. So you always
have to know the directory-level of your current page.<P>

This is not only annoying: Inside a macro, you are not able to
find out where the file calling the macro is located!<P>

One solution would be to define a <TG>BASE="[path to main document]"</TG>.
But then, all your links have to be absolute. This is very annoying
if you need to refer to files located in the same directory.<P>

But, thanks to <hsc>, here's the solution to this problem: if you
don't define a <TG>BASE</TG>-URI, all links relative links are
processed as usual. Only if the URI starts with a colon (":"), it
refers to a file relative to the main-path of your project.<P>

<* TODO: example *>

</WEBPAGE>

