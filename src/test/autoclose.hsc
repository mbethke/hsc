<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<HTML><HEAD><TITLE>autoclose</TITLE></HEAD><BODY>
  <UL>
    <LI>eins</LI>
    <LI>zwei
        <UL>
          <LI>bla</LI>
       </UL>
    </LI>
  </UL>

<P>sepp<P>hugo</P>

<* now for some errors *>
</P>
<P>sepp.<P>hugo ist <B>fett</B></P></P>

<UL><LI>eins</LI>         <* ok *>
    <LI>zwei
        <UL>
        <LI>bla</LI></LI> <* error *>
        </UL>
    </LI>
</UL>

</BODY></HTML>
