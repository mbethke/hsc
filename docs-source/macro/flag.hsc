<WEBPAGE chapter="hsc - Features - Macros - "
         title="Flags for tags and macros">

<$macro LIFLAG SHORT:string/r LONG:string/r ONLYDEFTAG:bool>
<DT><CODE>/<(long)></CODE>
    (short: <CODE>/<(short)></CODE>)
<DD>
</$macro>

Within  <ln_macro> and <ln_deftag>,
you can use several flag to tell <hsc> how to handle this macro/tag.


Legal flags for <ln_macro> are:
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

Additionally, the following flags can be used with <ln_deftag>
<DL>
<LIFLAG long="CLOSE" short="C">
    tag is a container and requires a corresponding closing tag.
<LIFLAG long="AUTOCLOSE" short="AC">
    Used for <TG>P</TG> and <TG>LI</TG>, which can be used as container
    or single tag depending on the HTML version.
<LIFLAG long="JERK" short="J" ONLYDEFTAG>
    tag is only used by jerks
<LIFLAG long="LAZY" short="L" ONLYDEFTAG>
    This flag has just been for my lazyness and defines some
    often needed attributes with a single letter;
    `<CODE>c</CODE>' for `<CODE>CLASS:string</CODE>',
    `<CODE>h</CODE>' for `<CODE>HREF:uri</CODE>',
    `<CODE>i</CODE>' for `<CODE>ID:id</CODE>',
    `<CODE>k</CODE>' for `<CODE>CLEAR:bool</CODE>',
    `<CODE>l</CODE>' for `<CODE>LANG:string</CODE>',
    `<CODE>m</CODE>' for `<CODE>MD:string</CODE>',
    `<CODE>s</CODE>' for `<CODE>SRC:uri</CODE>',
    `<CODE>w</CODE>' for `<CODE>NOWRAP:bool</CODE>'.<BR>
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
    but have their own syntax. The only HTML-tag which has this flag
    set is <TG>!</TG> (for SGML comments).
<LIFLAG long="WHTSPC" short="W">
    If this flag is set, any occurence of the container is checked
    for succeeding/preceding white spaces. By default, this concerns
    the tags <TG>A</TG>, <TG>TITLE</TG>, headings and physical/logical
    styles.
</DL>

<P>To define a closing macro, don't use <CODE>/CLOSE</CODE>, but define
the macro a second time, with a "<KBD>/</KBD>" as the first character
of its name. For examples, see the section about <ln_macro>.</P>

</WEBPAGE>
