<*
** here are the macros to our example project
*>

<*
** macro ITS_ME:
**
** this is just a simple shortcut for the authors email address
*>
<$macro ITS_ME><ADDRESS>Its Me(me@some.where)</ADDRESS></$macro>

<*
** macro WEBPAGE:
**
**
*>
<$macro WEBPAGE>
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">        <* SGML header *>
<HTML>
<!-- this page was designed using hsc -->           <* important comment *>
</$macro>

<*-------------------------------------*>

<$macro /WEBPAGE>
</BODY>                      <* closing BODY started in HEADING *>
</HTML>                      <* closing HTML started in WEBPAGE *>
</$macro>

<*
** macro HEADING
**
** setup HEAD-part of a page: set <TITLE>-text and Heading
*>
<$macro HEADING TITLE:string>
<HEAD>
  <TITLE><(Title)></TITLE>   <* insert title and heading *>
  <H1><(Title)></H1>
</HEAD>
<BODY>                       <* BODY starts here, but ends in </WEBPAGE> *>
</$macro>

<*
** macro FOOTLINE
**
** insert author's email address and current date & time
** at end of page
*>
<$macro FOOTLINE>
<HR>
  Updated by <ITS_ME> at <(GetTime())>
</$macro>
