<HTML><HEAD><TITLE>Test Image sizes</TITLE></HEAD><BODY>

<IMG SRC="test.gif" ALT="test">
<IMG SRC="test.png" ALT="test">
<IMG SRC="test.jfif" ALT="test">

<* gif with a #26/EOF in header - should still work *>
<IMG SRC="test_eof.gif" ALT="test">

<IMG SRC="test.ilbm" ALT="test"> <* will be ignored *>

</BODY></HTML>
