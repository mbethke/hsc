<WEBPAGE chapter="hsc - " title="Get size of images"
         PREV="rplcent.html" NEXT=":options.html">

<P>Several browsers support the attributes <CODE>WIDTH</CODE> and
<CODE>HEIGHT</CODE> for tags like <TG>IMG</TG>. If you know the
exact size of your image, you can tell it the browser. This might
speed up the layout-engine, because the browser doesn't have to
wait for the image to be transfered or needs to relayout the page
after the transfer.</P>

<H2>Usage</H2>

<P>As you usually don't know the exact size of your images, let your
stupid computer handle that tricky task by enabling the switch
<op_getsize> when invoking <hsc>. This will <hsc> tell to analyse
the image the attribute <CODE>SRC</CODE> refers to, and append the
attributes <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> with the
dimensions obtained from the image data.</P>

<P>If you have already set those attributes yourself, <hsc> will
only validate the values, and warn about missmatches.</P>

<P>Supported image formats are GIF, JFIF/JPEG and PNG.</P>

<H2>Example</H2>

<P>Take a look at this nice picture of some nice guy:
<IMG SRC=":image/niceguy.gif" ALT="Picture of some nice guy" ALIGN="middle"></P>

This can usually be included in a document using
<$source PRE>
    <IMG SRC="image/niceguy.gif" ALT="Picture of some nice guy">
</$source>

but if a document called <FILE>niceguy.hsc</FILE> is converted using
<PRE>
    hsc niceguy.hsc TO niceguy.html GETSIZE
</PRE>
the <TG>IMG</TG>-tag seen above will be extended to
<$source PRE>
    <IMG SRC="image/niceguy.gif" ALT="Picture of some nice guy" WIDTH="64" HEIGHT="64">
</$source>
in the html-object. If you do not like the double-quotes assigned to the 
size-values, use the CLI-option <op_quotestyle> to change this behavior.

</WEBPAGE>
