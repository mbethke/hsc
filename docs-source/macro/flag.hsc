<WEBPAGE chapter="hsc - Features - Macros - "
         title="Modifiers for tag- and macro-declarations">

<$macro LIFLAG SHORT:string/r LONG:string/r ONLYDEFTAG:bool>
<DT><CODE>/<(long)></CODE>
    (short: <CODE>/<(short)></CODE>)
<DD>
</$macro>

<P>Within  <ln_macro> and <ln_deftag>,
you can use several modifiers to tell <hsc> how to handle this macro/tag.</P>


Legal modifiers for <ln_macro> are:
<DL>
<LIFLAG long="ONLYONCE" short="1">
    macro/tag is required at the most once within a document
<LIFLAG long="REQUIRED" short="R">
    macro/tag is required at least once within a document
<LIFLAG long="MUST_BE_INSIDE" short="MBI">
    Needs a string as argument, that contains a list of container tags,
    that must be started before tag is allowed to be used.<BR>
    Example:<BR>
    The tag <TG>LI</TG> must be used inside some sort of lists only,
    therefor it's <CODE>/MBI="ul|ol|dir|menu"</CODE> is used.
    The <vbar> is used as a separator for multiple tags.

<LIFLAG long="NOT_ALLOWED_WITHIN" short="NAW">
    Like <CODE>/MBI</CODE>, but this time tags that must not be
    used before. For example, a nested usage of <TG>A</TG>
    is not allowed, therefor <CODE>/NAW="a"</CODE> is used.
</DL>

Additionally, the following modifiers can be used with <ln_deftag>
<DL>
<LIFLAG long="CLOSE" short="C">
    tag is a container and requires a corresponding closing tag.
<LIFLAG long="AUTOCLOSE" short="AC">
    Used for <TG>P</TG> and <TG>LI</TG>, which can be used as container
    or single tag depending on the HTML version.
<LIFLAG long="JERK" short="J" ONLYDEFTAG>
    tag is only used by jerks
<LIFLAG long="LAZY" short="L" ONLYDEFTAG>
    This modifier has just been for my lazyness and defines some
    often needed attributes with a single letter;
    <qq><CODE>c</CODE></qq> for <qq><CODE>CLASS:string</CODE></qq>,
    <qq><CODE>h</CODE></qq> for <qq><CODE>HREF:uri</CODE></qq>,
    <qq><CODE>i</CODE></qq> for <qq><CODE>ID:id</CODE></qq>,
    <qq><CODE>k</CODE></qq> for <qq><CODE>CLEAR:bool</CODE></qq>,
    <qq><CODE>l</CODE></qq> for <qq><CODE>LANG:string</CODE></qq>,
    <qq><CODE>m</CODE></qq> for <qq><CODE>MD:string</CODE></qq>,
    <qq><CODE>s</CODE></qq> for <qq><CODE>SRC:uri</CODE></qq>,
    <qq><CODE>w</CODE></qq> for <qq><CODE>NOWRAP:bool</CODE></qq>.<BR>
    For example, use <CODE>/LAZY="cil"</CODE>.
<LIFLAG long="OBSOLETE" short="O">
    tag is obsolete and should not be used any more. This eg.
    concerns <TG>LISTING</TG>.
<LIFLAG long="SKIPLF" short="S">
    skip a linefeed ("\n") that comes immediatly after the tag. This
    is only useful for some of <hsc>'s special tags and only avoids
    some uneccessary empty lines.
<LIFLAG long="SPECIAL" short="SPC">
    This one marks tags which are no "normal" tags with some attributes,
    but have their own syntax. The only HTML-tag which has this modifier
    set is <TG>!</TG> (for SGML comments).
<LIFLAG long="WHTSPC" short="W">
    If this modifier is set, any occurence of the container is checked
    for succeeding/preceding white spaces. By default, this concerns
    the tags <TG>A</TG>, <TG>TITLE</TG>, headings and physical/logical
    styles.
</DL>

<P>To define ab end macro, don't use <CODE>/CLOSE</CODE>, but define
the macro a second time, with a "<KBD>/</KBD>" as the first character
of its name. For examples, see the section about <ln_macro>.</P>

</WEBPAGE>
