<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<HTML><HEAD><TITLE>check conditional assignment</TITLE></HEAD><BODY>

<$macro CheckCA sepp:string hugo:string>

<$if cond=(set sepp)>SEPP=<(sepp)></$if>
<$if cond=(set hugo)>HUGO=<(hugo)></$if>

</$macro>

<$define resi:string="resi">
<$define hinz:string="hinz">
<$define kunz:string>

RESI=<(resi)>
HINZ=<(HINZ)>

<CheckCA sepp="no hugo">
<CheckCA sepp="sepp" hugo="hugo">
<CheckCA sepp?=resi hugo?=resi>
<CheckCA sepp?=hinz hugo?=kunz>

<$let resi=(hinz)>
RESI=<(resi)>
<$let resi?=resi>
RESI=<(resi)>

<$let resi>
<$if cond=(set resi)>RESI set<$else>RESI unset</$if>
<$let resi?=resi>
<$if cond=(set resi)>RESI set<$else>RESI unset</$if>


<$let resi?=hinz>
RESI=<(resi)>
<$let resi?=kunz><* kunz does not exist *>
<$if cond=(set resi)>RESI=<(resi)><$else>RESI unset</$if>


<* compute assignment *>
<$let resi="resi again">
<$let resi?=("hi"+"nz")>
<$if cond=(set resi)>RESI=<(resi)><$else>RESI unset</$if>

</BODY></HTML>
