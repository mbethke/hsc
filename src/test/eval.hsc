<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<HTML><HEAD><TITLE>check operators</TITLE></HEAD><BODY>

<**************************************
 *                                    *
 * test string constants              *
 *                                    *
 **************************************>
<$define susi:string>
<$let susi=susi><(susi)>
<$let susi="susi-double"><(susi)>
<$let susi='susi-single'><(susi)>
<$let susi=("susi-double-in-brackets")><(susi)>
<$let susi=(susi)><(susi)>

<**************************************
 *                                    *
 * test string operators              *
 *                                    *
 **************************************>

<**************************************
 * checkIn                            *
 **************************************>
<$macro checkIn little:string/r big:string/r>
<$if cond=(little IN big)>IN<$else>-NOT IN</$if>
</$macro>

<checkin little="sepp" big="hugo resi sepp">
<checkin little="SepP" big="hugo resi sepp">
<checkin little="hinz" big="hugo resi sepp">

<**************************************
 * checkConcat                        *
 **************************************>
<$macro CheckConcat v1:string/r v2:string/r vu:string/r>
<$define vc:string=(v1+v2)><$stripws type=prev>
[<(v1)>] + [<(v2)>] = [<(vc)>] = [<(vu)>]
<$if cond=(vu<>vc)><$stripws type=prev> ERROR!</$if>
</$macro>

<CheckConcat v1="" v2="" vu="">
<CheckConcat v1='' v2='' vu=''>
<CheckConcat v1="hugo " v2="ist doof" vu="hugo ist doof">
<CheckConcat v1=("hugo "+"ist ") v2="doof" vu="hugo ist doof">

<$let susi="susi">
<$let susi=(susi+" and sepp")><(susi)>
<$let susi=("hugo, "+susi)><(susi)>

<**************************************
 *                                    *
 * test boolean operators             *
 *                                    *
 **************************************>

<**************************************
 * printBool:                         *
 * insert boolean value as "0" or "1" *
 **************************************>
<$macro printBool v:bool><$if cond=(v)>1<$else>0</$if></$macro>

<**************************************
 * check boolean function, 2 operands *
 *                                    *
 * inputs:                            *
 *   v1, v2...boolean operands        *
 *   vc.......result (computed)       *
 *   vu.......result (user suggestion)*
 **************************************>
<$macro checkBool2 v1:bool v2:bool op:string vu:bool>
<$define vc:bool>
<("<$let vc=(v1 "+op+" v2)>")><$stripws type=prev>
<printBool v=(v1)>*<printBool v=(v2)>=<printBool v=(vc)>=<printBool v=(vu)>
<$if cond=(vc xor vu)><$stripws type=prev>  ERROR!</$if>
</$macro>

CheckBool2: error handling
<checkBool2 v1 v2 op=and>

<**************************************
 * check AND,OR,XOR with 2 operands   *
 **************************************>
Check AND (2):
<checkBool2       op=and   >
<checkBool2    v2 op=and   >
<checkBool2 v1    op=and   >
<checkBool2 v1 v2 op=and vu>

Check OR (2):
<checkBool2       op=or   >
<checkBool2    v2 op=or vu>
<checkBool2 v1    op=or vu>
<checkBool2 v1 v2 op=or vu>

Check XOR (2):
<checkBool2       op=xor   >
<checkBool2    v2 op=xor vu>
<checkBool2 v1    op=xor vu>
<checkBool2 v1 v2 op=xor   >

<**************************************
 *                                    *
 * test numeric operators             *
 *                                    *
 **************************************>

<**************************************
 * check numeric function, 2 operands *
 *                                    *
 * inputs:                            *
 *   v1, v2...numeric operands        *
 *   vc.......result (computed)       *
 *   vu.......result (user suggestion)*
 **************************************>
<$macro checkNumeric2 v1:num/r v2:num/r op:string vu:num/r>
<$define vc:num>
<("<$let vc=(v1 "+op+" v2)>")><$stripws type=prev>
<(v1)> <(op)> <(v2)> = <(vc)> = <(vu)>
<$if cond=(vc<>vu)><$stripws type=prev>  ERROR!</$if>
</$macro>

CheckNumeric2: error handling
<checkNumeric2 v1=1 v2=2 op="+" vu=17>

<**************************************
 * check numeric with 2 operands      *
 **************************************>
Check "+" (2):
<checkNumeric2 v1="1"  v2="2"  op="+" vu="3">
<checkNumeric2 v1="1"  v2="-2" op="+" vu="-1">
<checkNumeric2 v1="-1" v2="2"  op="+" vu="1">
<checkNumeric2 v1="-1" v2="-2" op="+" vu="-3">

</BODY></HTML>

