<WEBPAGE chapter="hsc - " title="Strip Specific Elements"
         PREV="spcattr.html"
         NEXT="syntax.html"
         QTEXT=("She looks like the Sunday Comics<BR>"
               +"She thinks she's Brenda Star<BR>"
               +"Her nose job is real atomic<BR>"
               +"All she needs is an old knife scar")
         QAUTHOR='Blondie, "Rip Her to Shreds"'>

<H2>SGML Comments</H2>

<P>Usually there should be no need to use sgml-comments, so why not
remove them at all? You can perform this by enabling the switch
<op-stripcomment> when invoking <hsc>.</P>

<P>Because of the numerous problems sgml-comments cause, you better use
<A HREF="spctags.html#comments">hsc-comments</A> anyway (see there also
for a discussion of these problems).</P>

<H2>Specific Tags</H2>

<P>..can be passed to the option <op-striptags>, separated by a
<vbar>. For example, to remove all physical styles, use
<KBD>STRIPTAG="B|I|U|TT"</KBD> when invoking <hsc>.</P>

<P>You can't strip tags, which have the <A HREF=":macro/flag.html">tag
modifier</A> <CODE>/SPECIAL</CODE> set. But this only concerns the
<TG>!</TG>-tag (use the above switch instead) and some of <hsc>'s
internal tags, which you should not strip anyway.</P>

<H2>Tags With External References</H2>

<P>These can be removed if you want to create a No-Net-version of your
document. Use the switch <op-stripexternal> for this task. Tags which
are affected by this must have an URI-type attribute, which has the <A
HREF=":macro/attrib.html#modifier">attribute modifier</A>
<CODE>/STRIPEXT</CODE> set within <hsc.prefs>.</P>

<P>Currently, this affects only the tags <TG>A</TG>, <TG>IMG</TG> and
<TG>LINK</TG>. This can be useful if you want to create a
<QQ>no-net-version</qq> of your documents.</P>

<H2>Unneeded Linefeeds And White Spaces</H2>

<P>These can be removed, if you heavy use them to structure your
source, but don't want to waste bandwidth for them. You only need to
enable the switch <op-compact>.</P>

<P>If now someone performs a "View Source" on your html object,it will
be less readable for him, but browsers won't care and display it the
same way. For example,</P>

<$source PRE>
This  is  a <B>complete</B>

    waste   of   space...
</$source>

will turn into

<$source PRE>
This is a <B>complete</B>
waste of space...
</$source>

In particular, <hsc> does:

<UL>
<LI>replace tabs by a single blank
<LI>replace multiple blanks by a single one
<LI>replace multiple linefeeds by a single one
<LI>remove blanks preceeding a linefeed
<LI>remove blanks at beginning of line
</UL>

<P>Linefeeds and white spaces inside a tag call are also affected by
this option. The tag <TG>PRE</TG> temporarily suppresses this option,
until a <TG>/PRE</TG> occures.</P>

</WEBPAGE>
