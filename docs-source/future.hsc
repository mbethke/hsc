<WEBPAGE chapter="hsc - " title="Future improvements"
    PREV="bugs.html"
    NEXT="related.html"
    QTEXT=("My future is static<BR>"
          +"It's already had it<BR>"
          +"I could tuck you in<BR>"
          +"And we can talk about it")
    QAUTHOR='Sonic Youth, "Schizophrenia"'>

<H2>Near future</H2>

The enhancements below are planed to implemented.. sometimes
<UL>
<LI>Additional tool to add/remove/rename documents to project-file
<LI>Improve <KBD>COMPACT</KBD> to also act inside tag calls
<LI>Add option <CODE>INDENT</CODE> for <TG>$include</TG> and
    <TG>$source</TG> to indent preformatted text
<LI>Add some inheritance-concept for attribute values, so
    that macros need no attribute-cloning code like
    <TG>A HREF=(HREF)</TG>
<LI>Support creation of an index-document (like found in most AmigaGuides)
<LI>Do checking of external URIs with an external program (eg. `GetURL')
<LI><TG>$select</TG>, <TG>$when</TG> and <TG>$otherwise</TG> as an extended version of <TG>$if</TG>
<LI>Plausibility checking of external URIs (unknown protocoll, missing domain)
<LI>After having read the specs for html 0.32, maybe I should rename
    the program to `hsemtics' (short for `HTML sucks even more than
    I can say")
<LI>Improve docs...
</UL>

<H2>Far future</H2>
The enhancements below <EM>might</EM> be implemented sometimes, but 
probably not in the near future.

<UL>
<LI>Fully support creation of a NoNet-version
<LI>Write a GUI-frontend for AmigaOS
<LI>Tool to link whole project directory into one document
</UL>

<H2>No future</H2>

For the thinggies below, from my point of view, there is no need to 
ever be implemented. But if you think, there is really a need for one
or more of them, fell free trying to convince me.

<UL>
<LI>Something like "preccompiled" include files, which would have
    also speed up reading <hsc.prefs>; I experimented around a bit
    with this, and it seems that more time is wasted scanning those
    bloody linked lists then by checking the data. It's more likely
    that an AVL-tree will make it into the source.
<LI>Type checking for attributes - anything more then the current 
    (very relaxed) checking wouldn't fit into the typeless 
    concept if plain-html.
<LI>Undefine attributes via <TG>$undefine</TG> - what for?
<LI>Move around in text using <TG>$goto</TG> and <TG>$label</TG> - this
    one's perverted to the core.
</UL>

</WEBPAGE>
