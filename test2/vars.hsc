
<$macro outer>
<$define var:num='10'>
<("outer 1: "+var)>
<inner>
<("outer 2: "+var)>
<inner>
<("outer 3: "+var)>
</$macro>

<$macro inner>
<("inner 1: "+var)>
<$if cond=('1')><$let var=(var - '1')></$if>
<("inner 2: "+var)>
</$macro>

<HTML><HEAD><TITLE>Variable scope</TITLE></HEAD>
<BODY>
<outer>
</BODY>
</HTML>
