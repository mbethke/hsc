<*****************************
* WEBPAGE                    *
******************************>
<$MACRO WEBPAGE title:string/r chapter:string=""
                next:uri prev:uri
                back:uri=":hsc.html"
                nocopy:bool nomain:bool>
<HTML>
<HEAD>
  <TITLE><$INSERT TEXT=<chapter>><$INSERT TEXT=<title>></TITLE>
  <LINK REV="owns" TITLE="Thomas Aglassinger" HREF="mailto:agi@sbox.tu-graz.ac.at">
  <$IF <NoMain>>
  <LINK REL="Home" HREF=":hsc.html">
  </$IF>
  <LINK REL="Copyright" HREF=":copy.html">
  <$IF SET <prev>>
  <LINK REL="Previous" HREF=<prev>>
  </$IF>
  <$IF SET <next>>
  <LINK REL="Next" HREF=<next>>
</$IF>
</HEAD>
<IMG SRC=":image/head.gif" ALT="hsc"><BR>
<H1><$INSERT TEXT=<title>></H1>
<BODY>
</$MACRO>

<*****************************
 * /WEBPAGE                  *
 *   foot line (sigh & date) *
 *****************************>
<$MACRO /WEBPAGE>

<** main button **>
<$IF NOT <NOMAIN>>
<A HREF=":index.html"><IMG SRC=":image/main.gif" ALT="MAIN"></A>
</$IF>
<** copyright button **>
<$IF NOT <NOCOPY>>
<A HREF=":copy.html"><IMG SRC=":image/copy.gif" ALT="COPY"></A>
</$IF>
<** insert "previos" button **>
<$IF SET <prev>>
<A HREF=<prev>><IMG SRC=":image/prev.gif" ALT="PREV"></A>
</$IF>
<** insert "next" button **>
<$IF SET <next>>
<A HREF=<next>><IMG SRC=":image/next.gif" ALT="NEXT"></A>
</$IF>

<HR>
<ADDRESS>
  Thomas Aglassinger (
  <A HREF="mailto:agi@sbox.tu-graz.ac.at">agi@sbox.tu-graz.ac.at</A>
  ), <$INSERT TIME>
</ADDRESS>
</BODY></HTML>
</$MACRO>

