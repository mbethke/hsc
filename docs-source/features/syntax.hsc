<WEBPAGE chapter="hsc - Features - " title="Syntax Check"
    PREV=":require.html"
    NEXT="spctags.html">

<P><hsc> performs a syntax check with your source. It testes it for
things like missing closing tags, illegal nestings or missing
required tags.It also tests attributes for tags, e.g. allowed values 
for the <CODE>ALIGN</CODE> option of <TG>IMG</TG>.
</P>

<P>However, the syntax check of <hsc> is not very powerful compaired
to tools that only handle this task.</P>

<P>(Tech. Note: Most errors <hsc> handles are of the kind "Well, now a 
have to program
this <CODE>if (..)</CODE> to avoid an enforcer hit/segmentation
fault, why not display an error message now..".)

<P>So if <hsc> does not report any syntax errors, it doesn't mean that
your source is 100% correct. For a more complete and strict test, take a 
look at the <ln_related>.</P>

</WEBPAGE>
