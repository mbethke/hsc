<**************************************
* WEBPAGE_NAVIGATION                  *
*   this is a local macro that is     *
*   allowed to be used from inside of *
*   <WEBPAGE> (see below)             *
*   it inserts a navigation bar at    *
*   current position                  *
***************************************>
<$macro WEBPAGE_NAVIGATION /MBI="WEBPAGE">
<** main button **>
<$if COND=(NOT SET NoMain)>
<A HREF=":index.html"><IMG SRC=":image/main.gif" ALT="Contents" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nomain.gif" ALT="----" ALIGN="middle">
</$if>
<** insert "index" button **>
<$if COND=(NOT SET NoIndex)>
<A HREF=":index.html"><IMG SRC=":image/index.gif" ALT="Index" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noindex.gif" ALT="-----" ALIGN="middle">
</$if>
<** copyright button **>
<$if COND=(NOT SET NoCopy)>
<A HREF=":copy.html"><IMG SRC=":image/copy.gif" ALT="Copyright" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nocopy.gif" ALT="---------" ALIGN="middle">
</$if>
<** insert "back" (up) button **>
<$if COND=(SET Back)>
<A HREF=(Back)><IMG SRC=":image/back.gif" ALT="Up" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noback.gif" ALT="--" ALIGN="middle">
</$if>
<** insert "previous" button **>
<$if COND=(SET Prev)>
<A HREF=(Prev)><IMG SRC=":image/prev.gif" ALT="Previous" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/noprev.gif" ALT="--------" ALIGN="middle">
</$if>
<** insert "next" button **>
<$if COND=(SET Next)>
<A HREF=(Next)><IMG SRC=":image/next.gif" ALT="Next" ALIGN="middle"></A>
<$else>
<IMG SRC=":image/nonext.gif" ALT="----" ALIGN="middle">
</$if>
</$macro>

<*****************************
* WEBPAGE                    *
******************************>
<$macro WEBPAGE /CLOSE /OnlyOnce Title:string/r
                Chapter:string=""
                Next:uri prev:uri
                Back:uri=":index.html"
                NoIndex:bool="true"
                NoCopy:bool
                NoMain:bool
                QTEXT:string QAUTHOR:string>
<HTML>
<HEAD>
  <TITLE><html-only><(Chapter)></html-only><(Title)></TITLE>
  <LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@giga.or.at">
  <$if COND=(SET next)>
  <LINK REL="Next" HREF=(Next)>
  </$if>
  <html-only>
    <$if COND=(SET NoMain)>
      <LINK REL="Home" HREF=":hsc.html">
    </$if>
    <LINK REL="Copyright" HREF=":copy.html">
    <$if COND=(SET prev)>
      <LINK REL="Previous" HREF=(Prev)>
    </$if>
    <*
     * This will tell w3-robots not to index any sub-documents,
     * as the main document index.html should contains all information
     * required to create a decent description for the robot.
     *
     * As a result, the robot will not scan the whole manual, and
     * therefore the waste of bandwidth is reduced.
     *
     * Assuming the robot supports this <meta>, of course...
     *>
     <META name="ROBOTS" content="NOINDEX, NOFOLLOW">
  </html-only>
</HEAD>
<BODY>

<*
 * insert navigation bar and quote
 * (not for PostScript-version)
 *>
<html-only>
<Webpage_Navigation>
<HR>
</html-only>

<* insert quote *>
<$if COND=(SET QTEXT)>
<P ALIGN="right"><I><(QTEXT)></I><BR>
  <$if COND=(SET QAUTHOR)>
    (<(QAUTHOR)>)
  </$if>
</P>
</$if>


<H1><(Title)></H1>
<$content>
<*
<HR>
<$let HSC.FORMAT.TIME="%d-%b-%Y">
<ADDRESS>
  Thomas Aglassinger
  (<A HREF="mailto:agi@giga.or.at">agi@giga.or.at</A>),
  <(GetTime())>
</ADDRESS>
*>
</BODY></HTML>
</$macro>

