<$include FILE="macros.hsc">

<HTML><HEAD><TITLE>Loopz!</TITLE></HEAD>
<BODY>

<FOR VAR=i START=1 TO=10><(i)></FOR>

<FOR VAR=i START=30 TO=290 STEP=20>
  <SPAN style=("font-size:" + i + "%")><(i)></SPAN>
  <$let i=(i - "1")>
  <$if COND=(i > "100")><break></$if>
</FOR>

<FOR VAR=j START=1 TO=2>
  <FOR VAR=i START=1 TO=-100 STEP="-1">
    <(j+i)>
    <$if COND=(i < '-50')><break n=2></$if>
  </FOR>
</FOR>

<$define foo:num=10>
<WHILE COND="(foo>'-5')">
<(foo)><$let foo=(foo - "2")>
</WHILE>

</BODY>
</HTML>
