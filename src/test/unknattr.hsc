<HTML><HEAD><TITLE>unknown attributes</TITLE></HEAD><BODY>
<$macro sepp resi:string>do nufin</$macro>

<unknown-tag this will be ignore="anyway">
<sepp resi="1" hugo="2" susi>
<B resi hugo="sepp">bold</B>
<A NAME=("sepp"+hugo)>name</A>
<$let undefined-attr="sepp">

</BODY></HTML>
