<WEBPAGE chapter="hsc - Features - " title="Replace Entities"
    PREV="uris.html"
    NEXT="getsize.html"
    QTEXT=("Go west<BR>"
          +"Paradise is there<BR>"
          +"You'll have all that you can eat<BR>"
          +"Of milk &amp; honey over there")
    QAUTHOR='Natalie Merchant, "San Andreas Fault"'>

<P>Once upon a time, some guys invented ASCII. As these guys have been
Americans, they did not care (or probably not even know) about the
rest of the world and said: "128 different characters are enough." So,
nowadays you still can't write and read a simple german umlaut within
different computer systems.</P>

<P>But, thank god, the guys who perpetrated html, after about 30 years
of a computer world without working umlauts, provide a solution to
this tricky problem: Instead of typing "Ü", simply write
<CODE>&amp;Uuml;</CODE>. Congratulations, what a great idea! 7 bit
rulez!</P>

<p>So as consequence of this prehistoric {beep}, <hsc> can replace
special characters in the hsc-source with its entities in the
html-object. You just need to enable the <A
HREF=":options.html">RPLCENT</A> switch. </P>

<P><STRONG>Note:</STRONG>This only works for special
characters that have been defined with <ln_defent> in
<hsc.prefs></P>

</WEBPAGE>
