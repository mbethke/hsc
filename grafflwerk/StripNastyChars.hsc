<*
 * StripNastyChar.hsc
 *
 * This document provides an advanced example usage of <$exec>.
 * For details refer to the main documentation.
 *>

<HTML><HEAD><TITLE>Strip Nasty Characters</TITLE></HEAD><BODY>

<$define nasty-data:string="This#Text|Contains!Ñâ§ïÿ/Characters">
<$define clean-data:string>

<$exec COMMAND=("rx StripNastyChars.rexx " + nasty-data) ATTRIBUTE=clean-data>

Converted "<(nasty-data)>"
to        "<(clean-data)>".

</BODY></HTML>
