<DOCPAGE chapter="hsc - " title="Undocumented Features"
    QTEXT="But I gotta have it<BR>Watch out for the damage"
    QAUTHOR='Vanessa Paradis, "Gotta Have It"'>

This chapter covers some undocumented features of <hsc>.
Most of them are hardly tested, some of them are rather
inconsistent. Also, I did not make much effort to explain
them very well.

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

</DOCPAGE>
