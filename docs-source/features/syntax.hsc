<WEBPAGE chapter="hsc - Features - " title="Syntax Check"
    PREV="strip.html"
    NEXT="assign.html">

<P>When parsing your input, <hsc> performs a syntax check on it. It
tests for things like missing end-tags, illegal tag nesting or missing
required tags.It also tests attributes for tags, e.g. allowed values
for the <CODE>ALIGN</CODE> option of <TG>IMG</TG>. </P>

<P>However, the syntax check of <hsc> is not very powerful compared
to tools that only handle this task.</P>

<P>(<TechNote> Most errors <hsc> handles are of the kind "Well, now a
have to program
this <CODE>if (..)</CODE> to avoid an enforcer hit/segmentation
fault, why not display an error message now..".)

<P>So if <hsc> does not report any syntax errors, it doesn't mean that
your source is 100% correct. For a more complete and strict test, take
a look at tools mentioned at <ln-related>.</P>

</WEBPAGE>
