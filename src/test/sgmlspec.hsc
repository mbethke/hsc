<!doctype html public "-//IETF//DTD HTML//EN">
<HTML><HEAD><TITLE>Special sgml tags</TITLE></HEAD>
<BODY>

<!-- simple comment --><B>bold</B>
<!--oneword--><B>bold</B>

<!-- one --><!-- after --><B>bold</B><!-- the --> <B>bold</B> <!-- other -->

<* some portability problems *>
<!-- With a `>' inside -->
<!-- multi line
     comment --><B>bold</B>

<* some Server Side Includes *>
<!--#include virtual="ssi/bull.sht"-->

<* legal switching context *>
<!-- switch ---- context -->

<$macro sepp>
<!-- inside a macro -->
</$macro>

<sepp>

</BODY></HTML>
