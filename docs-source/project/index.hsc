<WEBPAGE chapter="hsc - " title="Project managment"
    PREV=":features/exec.html"
    NEXT="make.html">

<*
** macro to format an explanation
*>
<$MACRO explan TITLE:string/r NAME:string>
<** create named reference **>
<$IF (SET NAME)>
<A NAME=(Name)><$ELSE>
<A NAME=(Title)></$IF>
<DT><** create heading **>
<KBD><$INSERT TEXT=(title)></KBD></A>
<DD>
</$MACRO>

<P>As the amount of documents your processing grows, so does
the amount of work required to maintain the whole bunch 
of your (wannabe-)information. To reduce timewaste for stupid 
project-managment-tasks, <hsc> is able to maintain a project-file
and to utilize some additional tools.<P>

<UL>
<LI><A HREF="make.html">make</A> - a general purpose pm-tool
<LI><A HREF="prjfile.html">project-files</A> - where <hsc> stores the
    required information.
<LI><A HREF="makefile.html">Makefiles</A> - how to use them for <hsc>
<LI><A HREF="hscdepp.html">hscdepp</A> - the dependency-procreator
    for hsc-sources
</UL>

</WEBPAGE>
