<WEBPAGE chapter="hsc - " title="Examples"
    PREV="options.html"
    NEXT="fileargs.html">

<* macro to format an example *>
<$MACRO example EXMP:string>
<DT><CODE><(exmp)></CODE>
<DD>
</$MACRO>

<H2>Command Line Examples</H2>

<DL>
<EXAMPLE exmp="hsc FROM hugo.html &gt;NIL:">
    Simply performs a syntax check on <FILE>hugo.html</FILE> and passes
    the output to the null-device.<P>
<EXAMPLE exmp="hsc hugo.html TO t: STATUS=QUIET RPLCENT">
    Same as above, but does not display status messages during conversion.
    A dummy-output is created in the temporary directory
    Additionally, all special characters like <qqc>&Uuml;</qqc> or <qqc>&szlig;</qqc> are
    replaced by its entities (<qqc>&amp;Uuml;</qqc> and
    <qqc>&amp;szlig;</qqc>).<P>
<EXAMPLE exmp="hsc FROM people/hugo.hsc TO /pub_html/">
    <P>Process subfile <FILE>people/hugo.hsc</FILE>. The current directory is the
    main directory of the project. The html-object is created in
    <FILE>/pub_html/people/hugo.html</FILE>.</P>
    <STRONG>Note</STRONG>: Also mind that the hsc-source ends with <qqc>.hsc</qqc>, but
    the html object automatically gets the extension <qqc>.html</qqc>. Therefor, all
    references must end with <qqc>.html</qqc>.
</DL>

<H2>Project Examples</H2>

<P>There also is an <FILE>example</FILE> drawer, which contains some
small example projects. Every project includes a file called
<FILE>README</FILE>, which will explain further details.</P>

<P>All example projects are supposed to be performed from the command
line.</P>

</WEBPAGE>
