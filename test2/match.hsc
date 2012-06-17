<$macro matches S:string/R P:string/R NOCASE:bool>
<$define m:string/C="matches">
<$define nm:string/C="does not match">

<$if COND=(set NOCASE)>
  <p>"<(S)>" <$if COND=(S MATCHI P)><(m)><$else><(nm)></$if> <(P)> ignoring case</p>
<$else>
  <p>"<(S)>" <$if COND=(S MATCH P)><(m)><$else><(nm)></$if> <(P)></p>
</$if>
</$macro>

<$macro matchboth S:string/R P:string/R>
	<matches S=(S) P=(P)>
	<matches S=(S) P=(P) NOCASE>
</$macro>

<HTML>
<HEAD><TITLE>Pattern matching</TITLE></HEAD>
<BODY>
	<matchboth S="Blahfasel" P="b.*(el|em)">
	<matchboth S="Blahfasel" P="B.*">
	<matchboth S="Blahfasel" P="[abc].+(el|em)">
	<matchboth S="Blahfasel" P="^[abc].+(el|em)">
	<matchboth S="Blahfasel" P="^(B|b|k)[0-9]+el">
	<matchboth S="Blahfasel" P="^(B|b|k)[a-z]+el">
	<matchboth S="Blahfasel" P="Foo|Blahfasel|Bar">
</BODY>
</HTML>

<* vi: set et ts=2: *>
