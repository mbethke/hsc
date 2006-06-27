<DOCPAGE chapter="hsc - Features - " title="Replace Entities"
    PREV="uris.html"
    NEXT="getsize.html"
    QTEXT=("Go west<BR>"
          +"Paradise is there<BR>"
          +"You'll have all that you can eat<BR>"
          +"Of milk &amp; honey over there")
    QAUTHOR='Natalie Merchant, "San Andreas Fault"'>

<P>Once upon a time, some guys invented ASCII. As these guys were
Americans, they did not care (or probably not even know) about the
rest of the world and said: <qq>128 different characters are enough.</qq> So
nowadays you still can't copy a simple German umlaut from one computer system
to another and be sure it will come out the same.</P>

<P>But, thank god, the guys who perpetrated HTML, after about 30 years
of a computer world without working umlauts, provided a solution to
this tricky problem: Instead of typing <qq>Ü</qq>, simply write
<CODE>&amp;Uuml;</CODE>. Congratulations, what a great idea! 7 bit
rulez!</P>

<p>So as consequence of this prehistoric crap, <hsc> can replace
special characters in the hsc-source with their entities in the
html-object. You just need to enable the <A HREF=":options.html">RPLCENT</A> switch. </P>

<p>Since XHTML has brought with it a more reasonable default character set,
namely UTF-8, you need not stick to the old 7-bit entity notation if you want
to use non-ASCII characters. Using the <a href=":options.html#entitystyle">ENTITYSTYLE</a> option you can have these
characters rendered in a variety of ways, see there for details.</p>

<P><STRONG>Note:</STRONG>For 8-bit characters, entity replacements only works for special
characters that have been defined with <ln-defent> in
<hsc.prefs>. This is so you can adapt your prefs to the 8-bit input charset you
actually use<mdash>some <a href="http://www.unicode.org/">Unicode</a> characters
outside the first code block have entity representation as well; these are
hardcoded in <hsc> as they are guaranteed not to change anyway.</P>

</DOCPAGE>
