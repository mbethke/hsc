<WEBPAGE chapter="hsc - " title="Changes And News"
    PREV="author.html"
    NEXT="updates.html"
    QTEXT=("You need a fix I'm your prostitute<BR>"
          +"Repression says depravity is cute<BR>"
          +"I'll feed you lines to make you smile<BR>"
          +"You're so easy to dehumanise")
    QAUTHOR='Maniac Street Preachers, "So Dead"'>

<*
 * little macro to insert name and a link to one of
 * these two changes-files.
 *>
<$macro support-link file:string/required>
<A HREF=(":../"+file)><(file)></A>
<$depend on=(":../"+file)>
</$macro>

<*
 * Under AmigaOS, call an ARexx-script that extract the first entry (or
 * first few entries) from the CHANGES and NEWS file and inserts them as
 * performatted html-code.
 *>
<$if cond=(hsc.System="Amiga")>

<$macro extract-from FILE:string/required>
<$exec TEMPORARY INCLUDE COMMAND=("rx script/extract-changes.rexx /"+file)>
</$macro>

This chapter describes the differences and incompatibilities compared
to the previous version. For older versions, refer to the files
<support-link file="CHANGES"> and <support-link file="NEWS"> coming
with the archives.

<H2>Changes</H2>

These are the improvements and bugfixes:

<extract-from file="CHANGES">

<H2>News</H2>

These are the things you have to take care of only if you upgrade an
already existing installation. They are most likely of no use for new
users.

<extract-from file="NEWS">
<$else>

<*
 * On all other systems, simply provide a link to these two files. Looks
 * less stylish, but still provides all information - although only
 * indirectly.
 *>

The differences and incompatibilities compared to the previous version
are described in <support-link file="CHANGES"> and <support-link
file="NEWS"> coming with the archives.

</$if>

</WEBPAGE>
