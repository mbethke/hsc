<$macro outer-sepp>
  now in outer sepp

  <* define inner-sepp *>
  <$macro inner-sepp>
    now in inner sepp
  </$macro>

  <* use inner-sepp *>
  <inner-sepp>
</$macro>

<outer-sepp>

