<$define Version:string="1.0">
<$define release-date:string="15-Mar-2006">

<$define DESCRIPTION:string=("This is the documentation to hsc, a "
  +"developer tool for HTML projects. It acts as a preprocessor "
  +"and supports macros, expressions, conditionals, include files, "
  +"replaces special characters, performs a structure check, tests "
  +"existence of local links and several other things. "
  +"Optionally it can keep track of documents being processed and "
  +"maintain the dependencies between them, making it relatively "
  +"easy to integrate it into existing developer environments."
 )>


<WEBPAGE>
<HEADER TITLE="HSC - Manual" DESCRIPTION=(DESCRIPTION)> 
  <LINK REV="owns" TITLE="Matthias Bethke" HREF="mailto:Matthias.Bethke@gmx.net">
  <LINK REL="Home" HREF=":index.html">
  <LINK REL="Copyright" HREF=":copy.html">
  <LINK REL="Next" HREF=":about.html">
  <META HTTP-EQUIV="keywords" CONTENT="hsc,html,xhtml,preprocessor,macro,include,suck">
</HEADER>
<MENUBODY>

<P class="quote">
Don't believe the hype<BR>Take a look at the small print<BR>Ignore the bold type<BR>
<span class="quoteattr">(Carter U.S.M, "Do Re Me, So Far So Good")</span>
</P>

<H1>HSC</H1>

<p class="version">(Version <(Version)>
<$stripws type="both">, <(release-date)>)</p>

<p lang="de"><img src="image/teutsch.png" ALT="[german]" ALIGN="middle">Es
gibt keine <A HREF="teutsch.html">deutschsprachige Dokumentation</A>.</P>

<P><(DESCRIPTION)></P>

<IMG SRC="image/hsc.png" ALT="..SUCK SUCK SUCK.." ALIGN="right">

<P>A current version of this program and manual should always be
available from <A HREF="http://www.linguistik.uni-erlangen.de/~msbethke/software.html"><(HSC.ANCHOR)></A></P>

</MENUBODY>
</WEBPAGE>

