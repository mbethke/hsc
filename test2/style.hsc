
<$macro mystyle foo:string><p style?= foo ></p></$macro>
<HTML>
<HEAD><TITLE>STYLE handling</TITLE></HEAD>
<BODY>
<p style="text-color:black;blah:fasel;  foo:x;"
   style ="font-size:large; : y"
   align =   "center"
   style="foo:bar">
</p>
<mystyle foo="bar:x">
</BODY>
</HTML>

<* vi: set et ts=2: *>
