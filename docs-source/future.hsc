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
<LI>Add a copy/move/add command to <hscpitt>
<LI>Add option <CODE>INDENT</CODE> for <TG>$include</TG> and
    <TG>$source</TG> to indent preformatted text
<LI>Add some inheritance-concept for attribute values, so
    that macros need no attribute-cloning code like
    <TG>A HREF=(HREF)</TG>
<LI>Support creation of an index-document (like found in most AmigaGuides)
<LI>Do checking of external URIs with an external program 
    (eg. <qq>GetURL</qq>)
<LI><TG>$select</TG>, <TG>$when</TG> and <TG>$otherwise</TG> as an extended version of <TG>$if</TG>
<LI>Plausibility checking of external URIs (unknown protocoll, missing domain)
<LI>Use system-independant paths for filenames stored in project-file, so
    you can work with the same project-file under multiple systems.
</UL>

<H2>Far future</H2>
The enhancements below <EM>might</EM> be implemented sometimes, but 
probably not in the near future.

<UL>
<LI>Fully support creation of a NoNet-version
<LI>Write a GUI-frontend for AmigaOS
<LI>Tool to link whole project into one document
</UL>

<H2>No future</H2>

For the thinggies below, from my point of view, there is no need to 
ever be implemented. 

<UL>
<LI>Something like "preccompiled" include files, which would have
    also speed up reading <hsc.prefs>; I experimented around a bit
    with this, and it seems that more time is wasted scanning those
    bloody linked lists then by checking the data. It's more likely
    that some sort of balanced binary tree will make it into 
    the source.
<LI>Type checking for attributes - anything more then the current 
    (very relaxed) checking wouldn't fit into the typeless 
    concept if plain-html.
<LI>Undefine attributes via <TG>$undefine</TG> - what for?
<LI>Move around in text using <TG>$goto</TG> and <TG>$label</TG> - this
    one's perverted to the core.
<LI>Support any other output-formats like texinfo or AmigaGuide; there
    are convertes around for that tasks.
</UL>

</WEBPAGE>
