<$macro SIMON-SAYS /CLOSE>
  <$if COND=(defined SIMONSUCKS)>Muhammad says:
  <$else>Simon says:</$if>
  <$content>
</$macro>

<$macro NOSIMON /CLOSE>
<$define SIMONSUCKS:bool='1'>
<(hsc.content)>
</$macro>

<HTML>
<HEAD><TITLE>Simon</TITLE></HEAD>
<BODY>
<SIMON-SAYS>use HSC!</SIMON-SAYS>

<NOSIMON>
<SIMON-SAYS>use HSC!</SIMON-SAYS>
</NOSIMON>
</BODY>
</HTML>

