<DOCPAGE chapter="hsc - " title="Using Other HTML-Extensions"
    PREV="ports.html"
    NEXT="bugs.html">

<P>As HTML has turned out to be incredibly useless very soon, many
people have written several extension trying to reduce this
uselessness. Most of them only increased it. However, this will
shortly comment on some of these extensions, and how you can use them
together with <hsc>.</P>

<H2>General Comments</H2>

<P>When you process a page with <hsc>, there is no way to change any
data of the HTML object after it has been created; <hsc> only does a
static job. Many of the extensions described below allow documents or
parts of them to be created dynamically. But then you will have to
care about several security problems.</P>

<P>Most of these concepts integrate themselves into HTML in a fairly
clean way (usually by means of sgml-comments). For others you will
need to utilise <ln-skip-verbatim> to make <hsc> accept them. If this
still does not help, there is no way to use a specific extension
together with <hsc>. There definitely will not be built-in support for
any of these extensions.</P>

<P>This chapter does not pretend to describe all of them, but most
suggestions provided here should also work with most others
extensions. You also will not find any information about where to
obtain these tools from. Ask your favourite w3-search engine for
details.</P>

<H2>Server Side Includes</H2>

<P>Server Side Includes (SSI) are one of the oldest extensions for
w3-servers. An example command would be

<$source PRE><!--#echo="DATE_GMT"--></$source>

which more or less does the same as

<$source PRE><(GetGMTime())></$source>

<P>in <hsc>, with the difference that the ssi-version will be executed
every time one accesses the page.</P>

<P>As the SSI-call is done inside a sgml-comment, <hsc> will
not care about it and simply skip it. Just make sure you did not set
the command line option <op-stripcomment>. The sgml-comment will
be substituted by your server later (if it supports SSI).</P>

<P>Usually SSI-operations result in an increased CPU load of your
server, as they have to be performed every time one accesses the page.
</P>

<H2>Common Gateway Interface</H2>

<P>Most w3-servers also support the so called Common Gateway Interface
(CGI). This is just the simple capability to return the output a
program wrote to <stdout> as w3-page. CGI-programs are usually
unreadable and unmaintainable Perl-scripts more or less entirely
consisting of statements like
<CODE><$source>printf("<HTML><HEAD><TITLE>cool
script</TITLE>..")</$source></CODE>.</P>

<P>Ill-minded people could think about invoking <hsc> inside a
CGI-script. I do not recommend this for several reasons that
should be obvious.</P>

<H2>JavaScript</H2>

<P>JavaScript has been developed to show scrolling messages on your
pages. It should have been named ScrollerScript, but as it came out
more or less  at the same time when the hype on a programming language
called Java (poor man's SmallTalk raped by C++ which is expected to
solve all problems of the world, including traffic, pollution and
AIDS) started, its creators decided to use the term <qq>Java</qq> in
it's name, too<footnote>In <a
href="http://weblogs.mozillazine.org/roadmap/archives/2006/02/js_and_python_news.html">the
words of its inventor</a>:
<blockquote>JS, [&hellip;] was a rushed little hack for Netscape 2 that was then frozen
prematurely during the browser wars, and evolved significantly only once by
ECMA. So its early flaws were never fixed, and worse, no virtuous cycle of
fine-grained community feedback in the form of standard library code
construction, and revision of the foundational parts of the language to better
support the standard library and its common use-cases, ever
occurred.</blockquote></footnote>. Scripts implemented in a clean way like

<$source PRE><script type="text/javascript"><!--
// ..script text..
// -->
</script></$source>

should not cause problems, as they are also embedded inside
sgml-comments like ssi.</p>

<H2>Personal Home Page Tool/Form Interpreter</H2>

<P>The Personal Home Page Tool/Form Interpreter (PHP/FI) started as a
messy Perl-script acting as some simple CGI-wrapper. As time passed,
it grew to a inflated and inconsistent moloch providing a
simple programming language inside HTML-documents. It also supports
things like embedded SQL, and some people with other presuicidal
syndromes even use it as a plug-in with their w3-server.</P>

<P>The syntax does a very good job on combining the drawbacks of C
with the disadvantages of Perl, in general resulting in a completely
unreadable sequence of seemingly random characters. Therefore, it became
quite popular among Unix-fossils. For example, one could have this
line in a source code:</P>

<$source PRE><?echo "Hi $Name!<P>"></$source>

If you pass this to <hsc>, it will not be very happy about it.
However, you can use <ln-skip-verbatim> to include PHP/FI-stuff:

<$source PRE><|<?echo "Hi $Name!<P>">|></$source>

If you want to access data of <hsc> into your PHP/FI-section, it will
become a bit tricky. But with <ln-insert-expression> even this is
possible:

<$source PRE>
<$define Name:string="Sepp">
<(
   '<|'                             +
   '<?echo "Hello, ' + Name + '!">' +
   '|>'
)></$source>

will temporarily result in

<$source PRE><|<?echo "Hello, Sepp!">|></$source>

and after being processed by <hsc> a

<$source PRE><?echo "Hello, Sepp!"></$source>

<P>will show up in the HTML object. If you are really sure that code
like the above will not significantly reduce your life expectancy, do
not hesitate to use it. But if you use PHP/FI, you apparently are
sure about that.</P>

<footnotes rulers backlink="[back]">
</DOCPAGE>
