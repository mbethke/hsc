<WEBPAGE chapter="hsc - Features - " title="Link Validatation"
    PREV="spctags.html"
    NEXT="uris.html">

<P>One of the most annoying thing within creating  html pages is to
find out if all your links work. Concepts superior to html and http
usually maintain a database, which automatically is updated if a 
document is moved or deleted.</P>

<P>For local URIs (those which only refer to files), <hsc> checks
existence of documents in your html project. When you refer to a
non-existing local URI within your source, <hsc> will view
<ln_msg id="21">.</P>

<P>This can not only be caused by documents which are referred to
using <TG>A HREF=..</TG>, but also other resources being embedded or
linked by means of for instance <TG>IMG</TG> or <TG>LINK</TG>.</P>

<P>For absolute URIs, no checks are performed.</P>

</WEBPAGE>
