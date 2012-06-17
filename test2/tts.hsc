<$macro CONTENT-TO-STRING /CLOSE VAR:string/R>
<$define c:string=("<$let " + VAR + "=(HSC.Content)>")>
<(c)>
</$macro>

<HTML>
<HEAD><TITLE>Text-to-String</TITLE></HEAD>
<BODY>

<$define test:string>
<content-to-string var=test>
<foo>
Test
</foo>
</content-to-string>

<$export file=foo data=(test)>
</BODY>
</HTML>

