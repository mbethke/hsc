<* 99-bottles-of-beer in HSC, by Matthias Bethke
   Pseudo-iterative version, process using:
   hsc xhtml compact 99bottles-iter.hsc *>

<$define PAGE:string/C="99 Bottles of Beer">

<$macro BOB N:num/R OTW:bool>
  <$if COND=(N = '0')>No more<$else><(N)></$if>
  <$if COND=(N <> '1')>bottles<$else>bottle</$if> of beer
  <$if COND=(set OTW)>on the wall</$if>
</$macro>

<$macro ITERATE COND:string/R CONTENT:string/R>
  <(
    "<$if COND=(" + COND + ")>" +
      "<(CONTENT)>" +
      "<ITERATE COND=(COND) CONTENT=(CONTENT)>" +
    "</$if>"
  )>
</$macro>

<$macro FOR /CLOSE VAR:string/R START:num=1 TO:num/R STEP:num=1>
  <("<$define " + VAR + ":num=" + START + ">")>
  <$if COND=(STEP < "0")>
    <ITERATE COND=(VAR + ">='" + TO +"'")
      CONTENT=(HSC.Content + "<$let " + VAR + "=(" + VAR + "&'" + STEP + "')>")>
  <$else>
    <ITERATE COND=(VAR + "<='" + TO +"'")
      CONTENT=(HSC.Content + "<$let " + VAR + "=(" + VAR + "&'" + STEP + "')>")>
  </$if>
</$macro>

<*** Page starts here ***>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"DTD/xhtml1-strict.dtd">
<html>
<head><title><(PAGE)></title></head>
<body>
<h1><(PAGE)></h1>
<FOR VAR=bottles START=99 TO=0 STEP=-1>
  <$if COND=(bottles = '0')>
    <p>Go to the store, buy some more...</p>
  <$else>
    <p><BOB N=(bottles) OTW>,<br />
    <BOB N=(bottles)><br />
    Take <$if COND=(bottles = '1')>it<$else>one</$if> down, pass it around,<br />
    <BOB N=(bottles - '1') OTW>.</p>
  </$if>
</for>
</body>
</html>
