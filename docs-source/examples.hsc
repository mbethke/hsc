<WEBPAGE chapter="hsc - " title="Examples"
    PREV="options.html"
    NEXT="messages.html">

<* macro to format an example *>
<$MACRO example EXMP:string>
<DT><CODE><(exmp)></CODE>
<DD>
</$MACRO>

<DL>
<EXAMPLE exmp="hsc FROM hugo.html &gt;NIL:">
    Simly performs a syntax check on <FILE>hugo.html</FILE> and passes
    the output to the null-device.<P>
<EXAMPLE exmp="hsc hugo.html TO t: STATUS=QUIET RPLCENT">
    Same as above, but does not display status messages during conversion.
    A dummy-output is created in the temporary directory
    Additionally, all special characters like "&Uuml;" or "&szlig;" are
    replaced by its entities ("<CODE>&amp;Uuml;</CODE>" and
    "<CODE>&amp;szlig;</CODE>").<P>
<EXAMPLE exmp="hsc FROM people/hugo.hsc TO /pub_html/">
    <P>Process subfile <FILE>people/hugo.hsc</FILE>. The current directory is the
    main directory of the project. The html-object is created in
    <FILE>/pub_html/people/hugo.html</FILE>.</P>
    <STRONG>Note</STRONG>: Also mind that the HSC-source ends with ".hsc", but
    the html-object automatically gets the extension ".html". Therefor, all
    references must end with ".html".
</DL>

</WEBPAGE>

