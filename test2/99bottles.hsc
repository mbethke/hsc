<* 99-bottles-of-beer in HSC, by Matthias Bethke
   Recursive version *>

<$define PAGE:string/C="99 Bottles of Beer">

<$macro BOB N:num/R OTW:bool>
  <$if COND=(N = '0')>No more<$else><(N)></$if>
  <$if COND=(N <> '1')>bottles<$else>bottle</$if> of beer
  <$if COND=(set OTW)>on the wall</$if>
</$macro>

<$macro BINGE BOTTLES:num/R>
<$if COND=(BOTTLES = '0')>
  <p>Go to the store, buy some more...</p>
<$else>
  <p><BOB N=(BOTTLES) OTW>,<br />
  <BOB N=(BOTTLES)><br />
  Take <$if COND=(BOTTLES = '1')>it<$else>one</$if> down, pass it around,<br />
  <$let BOTTLES=(BOTTLES - '1')>
  <BOB N=(BOTTLES) OTW>.</p>
  <BINGE BOTTLES=(BOTTLES)>
</$if>
</$macro>

<*** Page starts here ***>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"DTD/xhtml1-strict.dtd">
<html>
<head><title><(PAGE)></title></head>
<body>
<h1><(PAGE)></h1>
<BINGE BOTTLES=99>
</body>
</html>
