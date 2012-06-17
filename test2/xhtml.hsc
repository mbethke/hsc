<$include file=macros.hsc>

<WEBPAGE>
<HEAD><TITLE>XHTML/HSC test</TITLE></HEAD>
<BODY>

foo<HR>
bar<HR/>
baz<HR  />
<HR WIDTH="10%" />
<HR width="20%"/>
<map>
<AREA noHRef aLt="fOo" SHAPE="PoLy">
<AREA noHRef="foo" ALT="FoO!" />
</map>
<*
attr after<HR / width="20%">
attr after nospace<HR /width="20%">
*>
<IMG STYLE="foo" SRC="foo.png" ALT="foo">
<BR><BR/><BR />
</BODY>
</WEBPAGE>

