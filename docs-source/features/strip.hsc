<WEBPAGE chapter="hsc - " title="Strip specific elements"
         PREV="rplcent.html" NEXT=":options.html"
         QTEXT=("She looks like the Sunday Comics<BR>"
               +"She thinks she's Brenda Star<BR>"
               +"Her nose job is real atomic<BR>"
               +"All she needs is an old knife scar")
         QAUTHOR='Blondie, "Rip Her to Shreds"'>

<H2>SGML-comments</H2>
can be removed from the output by enabling the switch
<op_stripcomment> when invoking <hsc>.

<H2>Specific tags</H2>

<P>must be passed to the option <op_striptags>, separated by a
"<KBD>|</KBD>". For example, to remove all physical styles,
use <KBD>STRIPTAG="B|I|U|TT"</KBD> when invoking <hsc>.</P>

<P>You can't strip tags, which have the <A
HREF=":macro/flag.html">tag flag</A> <CODE>/SPECIAL</CODE>
set. But this only concerns the <TG>!</TG>-tag (use the
above switch instead) and some of <hsc>'s internal tags,
which you should not strip anyway.</P>

<H2>Tags with external references</H2>

<P>can be removed if you want to create a NoNet-version of your
document. Use the switch <op_stripexternal> for this task.
Tags which are affected by this must have an URI-type attribute,
which has the <A HREF=":macro/attrib.html#flags">attribute flag</A>
<CODE>/STRIPEXT</CODE> set within <hsc.prefs>.</P>

<P>Currently, this affects only the tags <TG>A</TG>, <TG>IMG</TG>
and <TG>LINK</TG>.</P>

<H2>Redundant linefeeds and white-spaces</H2>

<P>can be removed, if you heavy use them to structure your
source, but don't want to waste bandwidth for them. You only
need to enable the switch <op_compact>.</P>

<P>If now someone performs a "View Source" on your
html-object,it will be less readable for him,
but browsers won't care and display it the same way.</P>

Currently, <hsc> does:

<UL>
<LI>replace tabs by a single space
<LI>replace multiple white-spaces by a single space
<LI>replace multiple linefeeds by a single one
<LI>remove white-spaces preceeding a linefeed
<LI>remove white-spaces at beginning of line
</UL>

<P>Linefeeds and white-spaces inside a tag-call are not
affected by this option. The <TG>PRE</TG>-tag temporarily
suppresses this option, until a <TG>/PRE</TG> occures.</P>

</WEBPAGE>
