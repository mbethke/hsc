<WEBPAGE chapter="hsc - " title="Undocumented Features"
    QTEXT="But I gotta have it<BR>Watch out for the damage"
    QAUTHOR='Vanessa Paradis, "Gotta Have It"'>

This chapter covers some undocumented features of <hsc>.
Most of them are hardly tested, some of them are rather
inconsistent. Also, I did not make much afford to explain
them very well. Try to use them only inside macros, so
you do not have to perform loads of modifications if some
of these are renamed/changed for future versions.

<H2>Additional Operators</H2>

There are several boolean operators you can use within
<A HREF=":features/expressions.html">expressions</A>:

<UL>
<LI><qqc>AND</qqc> - boolean AND
<LI><qqc>OR</qqc> - boolean OR
<LI><qqc>XOR</qqc> - exclusive OR
<LI><qqc>&lt;</qqc>, <qqc>&gt;</qqc>, <qqc>&lt;=</qqc>, <qqc>&gt;=</qqc>, <qqc>&lt;&gt;</qqc>
    - case insensitive string comparison
</UL>

For numeric attributes, you can use operators for integer arithmetic:

<UL>
<LI><qqc>&amp;</qqc> - addition.
<LI><qqc>-</qqc> - subtraction. As <qqc>-</qqc> is also a legal character in attribute names,
you must enclose it into two white spaces, so that it will not be interpreted as part
of the next or previous word.
<LI><qqc>*</qqc> - multiplication
<LI><qqc>/</qqc> - division
<LI><qqc>MOD</qqc> - modulo
</UL>

Currently there are no checks for out-of-range and division-by-zero
etc., so take care. Empty strings count as <qqc>0</qqc>, any text not
consisting only of digits counts as <qqc>1</qqc> (no warnings). This
seems to make sense for auto-casting <CODE>BOOL</CODE> (not sure about
that).

<H2>Additional Tags</H2>

<H3><TG>$export</TG> - Export Data To Files</H3>

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

<H2>Options File</H2>

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
    IGNORE=note|style
</PRE>

The options file always has to be named <FILE>hsc.options</FILE>.

</WEBPAGE>
