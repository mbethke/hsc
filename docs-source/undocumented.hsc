<WEBPAGE chapter="hsc - " title="Undocumented features"
    QTEXT="But I gotta have it<BR>Watch out for the damage"
    QAUTHOR='Vanessa Paradis, "Gotta Have It"'>

This chapter covers some undocumented features of <hsc>.
Most of them are hardly tested, some of them are rather
inconsistent. Also, I did not make much afford to explain
them very well. Try to use them only inside macros, so
you do not have to perform loads of modifications if some
of these are renamed/changed for future versions.

<H2>Additional operators</H2>

There are several boolean operators you can use within
<A HREF=":features/expressions.html">expressions</A>:

<UL>
<LI><qqc>AND</qqc> - boolean AND
<LI><qqc>OR</qqc> - boolean OR
<LI><qqc>XOR</qqc> - exclusive OR
<LI><qqc>&lt;</qqc>, <qqc>&gt;</qqc>, <qqc>&lt;=</qqc>, <qqc>&gt;=</qqc>, <qqc>&lt;&gt;</qqc>
    - case insensitive string comparison
</UL>

<H2>Additional tags</H2>

<H3><TG>$export</TG></H3>

This one can be used to export data to a file. Possile attributes:
<DL>
<DT>FILE:string/required
<DD>Name of file where to write output. If the file already exists,
    it will be overwritten without any warning.
<DT>DATA:string/required
<DD>Data to store in the file
<DT>APPEND:bool
<DD>If the output file already exists, the new data will be
    appended with leaving the old contents intact.
</DL>

<H2>Options file</H2>

<P>On startup, <hsc> will look for an options file, which will be parsed
for command line options <STRONG>before</STRONG> the actual
command line options passed from CLI. Values set in the options
file can be overwritten by command line options later.</P>

The format of the options file is easy as can be: it consists of
several lines, with every line containing one single options, and,
if necessary, also a <qqc>=</qqc> and a value. An example options
file could look like this:

<PRE>
    FROM=include/stdmacros.hsc
    TO=www:sepp/
    COMPACT
    IGNORE=notes|style
</PRE>

The options file always has to be names <FILE>hsc.options</FILE>.

<H2>Conditional assignments</H2>

When assigning new values to attributes, you can also make
them inherit values of other attributes by using

<PRE>    SEPP=(HUGO)</PRE>

However, if <CODE>HUGO</CODE> has not been defined and assigned
a value before, this will result in an error message.
Conditional assignments now only assign a value to the
target attribute, if the source attribute has been defined.
Simply use a <qqc>?=</qqc> instead of the <qqc>=</qqc> assignment
operator:

<PRE>    SEPP?=HUGO</PRE>

This becomes handy for macros which are more or less only
extensions to real html-tags:

<$source PRE>
    <$macro xBODY TEXT:color LINK:color>
        <BODY TEXT?=TEXT LINK?=LINK BACKGROUND="image/back.png">
    </$macro>
</$source>

Now you can use the macro <TG>xBODY</TG> as an replacement
for the tag <TG>BODY</TG>: using a simple

<$source PRE>    <xBODY></$source>
will result in
<$source PRE>    <BODY BACKGROUND="image/back.png"></$source>

but a
<$source PRE>    <xBODY TEXT='#123456' LINK='#654321'></$source>
will lead to
<$source PRE>    <BODY TEXT='#123456' LINK='#654321' BACKGROUND="image/back.png"></$source>
thus also adding the attributes <CODE>TEXT</CODE> and <CODE>LINK</CODE>
to the tag call.

<P>On the first sight, it might seem that there is only the simple
condition <qq>if attribute is set..</qq> is possible. But no one
prevents you from using code like this:

<$source PRE>
    <$if COND=(awfully complex condition))>
        <$define TEXT:color='#123456'>
    <$else>
        <$define TEXT:color='#001234'>
    </$if>

    <xBODY TEXT?=TEXT>
</$source>

Conditional assignments still seem to have a problem when
being used within <TG>$let</TG>, therefor use them only for assignments
within tags and macros.

</WEBPAGE>
