<WEBPAGE chapter="hsc - " title="Examples"
    PREV="options.html"
    NEXT="errors.html">

<* macro to format an example *>
<$MACRO example EXMP:string>
<DT><CODE><$INSERT TEXT=<exmp>></CODE>
<DD>
</$MACRO>

<DL>
<EXAMPLE exmp="hsc FROM hugo.html &gt;NIL:">
    Simly performs a syntax check on <FILE>hugo.html</FILE> and passes
    the output to the null-device.<P>
<EXAMPLE exmp="hsc hugo.html t: STATUS RPLCENT">
    Same as above, but also displays a status message during conversion.
    A dummy-output is created in the temporary directory
    Additionally, all special characters like "&Uuml;" or "&szlig;" are
    replaced by its entities ("<TT>&amp;Uuml;</TT>" and
    "<TT>&amp;szlig;</TT>"). Additionally, <P>
<EXAMPLE exmp="hsc FROM people/hugo.hsc DESTDIR /pub_html STATUS">
    Process subfile <FILE>people/hugo.hsc</FILE>. The current directory is the
    main directory of the project. The HTML-object is created in
    <FILE>/pub_html/people/hugo.hsc</FILE>. All local URIs beginning with ":"
    referenced within
    <FILE>hugo.hsc</FILE> are interpreted as <A HREF="features/absuris.html">
    absolut URIs</A> and are converted to relative path when written to
    the HTML-object.<P>
<EXAMPLE exmp="hsc FROM people/hugo.hsc DESTDIR /pub_html STATUS CHECKURI">
    Same as above. Additionally, all URIs referenced are checked for existence.
    If eg, <FILE>people/hugo.hsc</FILE> references to <FILE>prog/project/thing.hsc</FILE>,
    the file <FILE>/pub_html/project/thing.html</FILE> must exist or an error will
    occure.<BR>
    <STRONG>Note</STRONG>: Also mind that the HSC-source ends with ".hsc", but
    the HTML-object automatically gets the extension ".html". Therefor, all
    references must end with ".html".
</DL>

<P>

</WEBPAGE>

