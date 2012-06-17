<HTML>
<HEAD><TITLE>Varlist</TITLE></HEAD>

<$varlist vl foo:string="foo" blah:num>
<$deftag foo [vl]>
<$macro bar [vl]><foo foo="blah"></$macro>

<BODY>
<foo blah=1>
</BODY>
</HTML>

<* vi: set et ts=2: *>
