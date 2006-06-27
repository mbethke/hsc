<DOCPAGE chapter="hsc - Features - " title="Link Validatation"
    PREV="spctags.html"
    NEXT="uris.html">

<P>One of the most annoying thing within creating HTML pages is to
find out if all your links work. Concepts superior to HTML and HTTP
usually maintain a database which is automatically updated if a
document is moved or deleted. (And they already did that in they
seventies.)</P>

<P>For local URIs (those which only refer to files), <hsc> checks
existence of documents in your HTML project by default. When you refer to a
non-existing local URI within your source, <hsc> will print
<ln-msg id="21">. This can not only be caused by documents which are referred to
using <TG>A HREF=...</TG>, but also other resources being embedded or
linked to by means of <TG>IMG</TG>, <TG>LINK</TG>, etc.</P>

<P>Since V0.932, <hsc> has been able to check external URIs as well, as long as
your OS provides a basic socket API (the only notable exception being a plain
AmigaOS 3.1) and the necessary connectivity. As this will considerably slow down
the translation, it is off by default and can be enabled by means of the <a
href=":options.html#checkexternal">CHECKEXTERNAL</a> option.</p>

<p>For checking external URIs, see also the notes on <a
href=":envvar.html#http_proxy">proxy-related environment variables</a>.</p>

</DOCPAGE>
