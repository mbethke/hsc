<DOCPAGE chapter="hsc - Features - " title="Cascading Style Sheets"
    PREV="syntax.html"
    NEXT="assign.html">

<P><hsc> has some limited support for CSS. Limited because it
doesn't have a full stylesheet parser yet, which would allow syntax checks of
<tg>STYLE</tg> sections. However, CSS specifications in <code>STYLE</code>
<em>attributes</em> can be parsed, checked and merged already. The following
features support CSS:
<ul>
<li>The <ln-defstyle> tag allows to define a new style property and optionally
a list of legal values therefor.
Properties specified in <code>STYLE</code> attributes will be checked
against this list.</li>
<li>The attributes added by the <a href=":features/getsize.html"><code>GETSIZE/S</code></a>
function are CSS by default if you use XHTML mode.</li>
<li>Multiple <code>STYLE</code> attributes in a single tag will be merged into
one. This can simplify the writing of macros where you want to supply a few
default styles and let the user pass in additional ones:
<$source pre><P STYLE="text-align:justify" STYLE?=STYLE></$source></li>
<li>If the same CSS attribute was specified more than once for a single tag,
<hsc> will issue a warning and use the value encountered last.</li>
</ul>

</DOCPAGE>

<* vi: set ts=4 expandtab: *>
