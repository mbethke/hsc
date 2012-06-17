<* simple macro without attributes *>
<$macro simple_resi SEPP:string="sepp_simple">
simple_resi.
sepp(simple)=<(sepp)>
<$let sepp="sepp_simple_2">
sepp(simple)=<(sepp)> (after <$source><$let></$source>)
</$macro>


<* macro that will be called recursively *>
<$macro recursive_resi SEPP:string="sepp_recursive">
  recursive_start: sepp=<(sepp)>
  <$if cond=(sepp="sepp_recursive_x_x_x")>
    ABORT recursion.
  <$else>
    <recursive_resi sepp=(sepp+"_x")>
  </$if>
  recursive_end  : sepp=<(sepp)>
</$macro>


<* old-style container *>
<$macro resi SEPP:string="sepp_resi">
    resi_start: sepp=<(sepp)>
</$macro>
<$macro /resi>
    resi_end  : sepp=<(sepp)>
</$macro>


<* new style container *>
<$macro paired_resi /CLOSE SEPP:string="sepp_paired">
(start paired resi)
  sepp(paired)=<(sepp)> 
  <$content>
  sepp(paired)=<(sepp)> 
(end paired resi)
</$macro>


<***************************************>
<HTML><HEAD><TITLE>kack</TITLE></HEAD><BODY>

----simple resi----
<simple_resi>

----old-style container----
<resi> (inside resi) </resi>

<* comm <* comm2 *> *>
<**** sepp ****>
<***  sepp  ***>

<$define sepp:string="sepp_global">


----paired resi----
<paired_resi>
<resi>
  sepp(paired)=<(sepp)>
</resi>
</paired_resi>--

----recursive resi----
<recursive_resi>

sepp(global)=<(sepp)>

----nesting a container----
<$macro pag /close>(pag<lnk><$content></lnk>pag)</$macro>
<$macro lnk /close>(lnk<hrf><$content></hrf>lnk)</$macro>
<$macro hrf /close>(hrf<$content>hrf)</$macro>

twice: <pag>#</pag>
once : <lnk>*</lnk>



</BODY></HTML>
