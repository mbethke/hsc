<$macro DOCPAGE /CLOSE /OnlyOnce Title:string/r
                Chapter:string=""
                Next:uri prev:uri
                Back:uri=":index.html"
                NoIndex:bool="true"
                NoCopy:bool
                NoMain:bool
                QTEXT:string QAUTHOR:string>
<WEBPAGE>
  <HEADER TITLE=("<html-only>"+Chapter+"</html-only>"+Title)>
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
      <META name="ROBOTS" content="NOINDEX, NOFOLLOW">
    </html-only>
  </HEADER>

  <MENUBODY>
  
  <* insert quote *>
  <$if COND=(SET QTEXT)>
  <P class="quote"><(QTEXT)><BR>
    <$if COND=(SET QAUTHOR)>
      <span class="quoteattr">(<(QAUTHOR)>)</span>
    </$if>
  </P>
  </$if>
  
  <H1><(Title)></H1>
  <$content>
  </MENUBODY>
</WEBPAGE>
</$macro>

