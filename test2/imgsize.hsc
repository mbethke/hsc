<$include file=macros.hsc>

<$macro foo width:num style:string>
  <IMG SRC="foo.png" STYLE?=style ALT=foo WIDTH?=width />
</$macro>

<HTML>
<HEAD><TITLE>Imagesize</TITLE></HEAD>
<BODY>
  <foo>
  <foo style="border:thin solid black">
  <foo WIDTH="100">
</BODY>
</HTML>

<* vi: set et ts=2: *>
