<WEBPAGE chapter="hsc - " title="Related Stuff"
    PREV="future.html"
    QTEXT=("I've got my air-jack hammer<BR>"
          +"I've got my saber saw<BR>"
          +"I've got all I need<BR>"
          +"Cuz I got my tools")
    QAUTHOR='Flour, "Tools"'>
<*    QTEXT=("And if you take a taste<BR>"
          +"You better leave no waste<BR>"
          +"You gotta eat up all you get")
    QAUTHOR='Soup Dragons, "Beauty Freak"'*>

<P>As <hsc> is only one part of a puzzle, this chapter should give you
some hints where you can find other parts. However this does not claim
to point out all additional tools that can be useful inc combination
with <hsc>. But at least it should list all utilities mentioned
somewhere in this manual.</P>

Some stuff that should be useful for <STRONG>project management</STRONG>:

<UL>
<LI><AMINET FILE="dev/c/make-3.75-bin.lha">, an Amiga-port of <EXEC>GNUmake</EXEC>.
    Different to several other Unix-ports, no <CODE>ixemul.library</CODE>
    is required, as it was compiled with SAS/c.
<LI><AMINET FILE="dev/gcc/" TEXT="aminet:dev/gcc/gccXXXdoc.lha">, which contains
    (among other documents) the manual to <EXEC>GNUmake</EXEC> in 
    AmigaGuide format (the exact filename depends on the version of
    <EXEC>gcc</EXEC>).
<LI><EXEC>GNUmake</EXEC> and <EXEC>diff</EXEC> (and all other GNU
    utilities) can be obtained from <A
    HREF="ftp://prep.ai.mit.edu/pub/gnu/"><(hsc.anchor)></A>. There
    are several mirror sites of this material, scan the
    <FILE>ReadMe</FILE>s there for details.
</UL>

As <hsc>'s <STRONG>syntax check</STRONG> is quite small and clumsy, maybe you should also
have a look at one of the following sources:

<UL>
<LI><AminetReadMe FILE="comm/www/CheckHTML.readme" TEXT="CheckHTML">
    - based on <EXEC>SGMLS</EXEC>, but easier to use. Available from
    <Aminet FILE="comm/www/CheckHTML.lha">;
    for those who insist on using a <dtd>.
<LI><WebLint>, a Perl-script that is even more cryptic to configure
    than <hsc>. And it's Perl.. würg, kotz, reiha.. (<(HSC.ANCHOR)>)
<LI><A HREF="http://www.webtechs.com/html-val-svc/">HalSoft's
    validation service</A> maybe provides the most competent syntax-check,
    but most likely the slowest. (<(HSC.ANCHOR)>)
</UL>

<p>To show a document in your browser from the <makefile> once <hsc>
has written it, you can use <aminet file="comm/www/OpenURL.lha">.</p>

To use <STRONG>Unixoid filenames</STRONG> within AmigaOS, there are numerous tools
available. For example, you can use
<UL>
<LI><Aminet FILE="dev/gcc/ixpath.lha"> - CLI only, source code included
<LI><Aminet FILE="util/boot/" TEXT="aminet:util/boot/DosWedgeXX"> -
    implemented as a commodity
</UL>


And some resources that have been quite useful during the development
of <hsc>, but are probably also <STRONG>worth a reading</STRONG> if
you are serious about w3-authoring:

<UL>
<LI><A HREF="http://www.cs.cmu.edu/~tilt/cgh/">Composing Good HTML</A>,
    a general introduction to the whole html-problem. It's also useful to
    better understand several messages of <hsc>. (<(HSC.ANCHOR)>)
<LI><A HREF="http://www.useit.com/alertbox/">The Alertbox</A>: Current Issues in Web Usability
    - might frustrate you, but answers several "Why?"'s. (<(HSC.ANCHOR)>)
<LI>Richard Scarry: "Mein allerschönstes Buch vom Backen, Bauen und
    Flugzeugfliegen" - a general introduction to life and how to cope
    with it.
</UL>

</WEBPAGE>
