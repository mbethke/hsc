<WEBPAGE chapter="hsc - " title="About"
    PREV="hsc.html"
    NEXT="require.html" NOCOPY>

If you read this, it's quite likely that you tried to design a
HTML-page. And you probably found out that HTML is a very
clumsy thing: No macros, no include files and several other
features lacking.<P>

And what can you do after your page is ready? View it with a
browser and test-click all links?<P>

Pfuahahaha! Most browser are designed to handle as many errors
as possible and display a readable page, but don't mention your
errors! And link-testing is a very stupid task...<P>

Of course, there exist several tools: You can use a C-preprocessor to
define macros and include files, use <WebLint> as a syntax-checker
and run a link-test-tool on your page.<P>

Anyway, <hsc> does all this: it performs a (small) syntax check,
tests your (local) links and provides a very HTML-like way to
define macros. And much more..<P>

<hsc> simply acts like a preprocessor: You call it with a "extended"
HTML-source (I call it HSC-source) containig special commands <hsc>
interprets and produces a pure HTML-output as object file.
The output file can be viewed with a W3-browser.<P>

</WEBPAGE>
