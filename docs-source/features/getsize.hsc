<DOCPAGE chapter="hsc - " title="Get Size Of Images"
         PREV="rplcent.html"
         NEXT="spcattr.html">

<P>Specifying the desired size of an image in its <tg>IMG</tg> tag can
speed up the browser's layout because the browser doesn't have to
wait for the image to be transfered or needs to re-layout the page
after the transfer. <hsc> can either add <code>WIDTH</code> and
<code>HEIGHT</code> attributes to the tag or use <a href="css.html">Cascading
Stylesheet</a> specifications to add the size information.</P>

<H2>Usage</H2>

<P>As you usually don't know the exact size of your images, let your
stupid computer handle this dull task by enabling the switch
<op-getsize> when invoking <hsc>. This tells <hsc> to analyse the
image the attribute <CODE>SRC</CODE> refers to, and either append the
attributes <CODE>WIDTH</CODE> and <CODE>HEIGHT</CODE> with the
dimensions obtained from the image data, oder add the properties
<CODE>width:</CODE> and <CODE>height:</CODE> to a <CODE>STYLE</CODE> attribute,
which will be created if it didn't exist yet.</P>

<P>If you have already set these attributes yourself, <hsc> will
only validate the values, and warn about mismatches.</P>

<H2>Example</H2>

<P>Take a look at this nice picture of some nice guy:</P>

<P ALIGN="center"><IMG SRC=":image/niceguy.png" ALT="Picture of some nice guy"></P>

<p>This can usually be included in a document using something like

<$source PRE><IMG SRC="image/niceguy.png" ALT="Picture of some nice guy"></$source>

but if a document called <FILE>niceguy.hsc</FILE> containing such a
tag is converted using

<PRE>hsc niceguy.hsc TO niceguy.html GETSIZE</PRE>

the <TG>IMG</TG>-tag seen above will be extended to

<$source PRE><IMG SRC="image/niceguy.png" ALT="Picture of some nice guy" WIDTH="64" HEIGHT="64"></$source>

in the HTML object. If you do not like the double quotes assigned to the
size values, use the commandline option <op-quotestyle> to change this behavior.</p>

<p>The use of STYLE attributes is currently tied to the XHTML mode, i.e. XHTML
output will use CSS while traditional HTML uses the <CODE>WIDTH</CODE> and
<CODE>HEIGHT</CODE> attributes. The above example would look like this after
running it through <hsc> with XHTML enabled:

<$source PRE><IMG SRC="image/niceguy.png" ALT="Picture of some nice guy" STYLE="width:64px; height:64px"></$source>

<H2>Supported Image Formats</H2>

In the recent years several formats to store image data have been
established &mdash; mostly because none of these was really useful. Only a
few of them are supported by <hsc>. However, these should be enough for
w3-authoring.

<H3>gif &mdash; Graphics Interchange Format</H3>

<P>In the early times of w3, this was the only format supported by
most browsers. It features only indexed-color (16 or 256 colors) and
an ugly looking progressive display option. It became very popular in
a negative sense because of it's compression algorithm and the
associated copyright.</P>

<P>Bye the way, did you know that "The Graphics Interchange Format(c) is
the Copyright property of CompuServe Incorporated. GIF(sm) is a
Service Mark property of CompuServe Incorporated."? So now you do; if
it makes them happy...</P>

<H3>jfif/jpeg</H3>

<P>Essentially, this format does a good job as an idiot indicator. This
already starts with the name: jfif is short for JPEG File Interchange
Format (because it uses jpeg compression for image data). Nevertheless
jfif-pictures will have a file extension set to <qqc>.jpg</qqc> or
<qqc>.jpeg</qqc>.</P>

<P>The main feature about jpeg compression is that it is lossy, which
means that the output image is not identical to the input image.
Documents about software using jpeg compression usually claim that you
can obtain good compression levels with no visible change. As long as
you do not start to rotate or apply other complex image processing
operations on pictures, this might even be true. Otherwise, an ugly grid
will appear soon.</P>

<P>Most important, jfif is commonly used to encode images of more or
less undressed people. To store as many such pictures as possible in
as little space as possible, the compression rate is usually set to a
high value. This makes these people look as if they suffered from
leprosy, or just a drawing by Egon Schiele.</P>

<P>Furthermore many people outside English speaking countries
pronounce jpeg (<qq>jay-peg</qq>) as GPEG (<qq>gee-peg</qq>), even if
they are normally capable of a proper English pronunciation.</P>


<H3>PNG &mdash; Portable Network Graphics</H3>

The <A HREF="http://www.w3.org/pub/WWW/TR/WD-png">PNG Specification</A> (<(HSC.Anchor)>)
claims this format to be

<BLOCKQUOTE>...an extensible file format for the lossless, portable,
well-compressed storage of raster images. PNG provides a patent-free
replacement for the GIF format and can also replace many common uses
of TIFF format. Indexed-color, grayscale, and truecolor images are
supported, plus an optional alpha channel. Sample depths range from 1
to 16 bits [and] it is fully streamable with a progressive display
option.</BLOCKQUOTE>

<P>Above all, PNG supports most obvious things several other formats
failed to include. There is nothing really remarkable about it, but
today one has to be glad even about this. So it can be said that PNG
is one of the few positive things that happened in the last ten
years.</P>

<P>But even despite there being free source code available to read
PNG-images, and the word <qq>Network</qq> is part of it's name (to
conform to the hype), it was only supported by very few applications
and w3-browsers for a long time. This slowly changes, so maybe you will
want to give it a try.</P>

<P>Bye the way, <hsc> has always supported it &mdash; actually it was the first
working image format because it took some releases to end up with a
bugfree scanner for gif...</P>

</DOCPAGE>
