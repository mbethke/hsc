<WEBPAGE chapter="hsc - Features - Macros - " title="Macro/Tag flags">

<$macro LIFLAG SHORT:string/r LONG:string/r ONLYDEFTAG:bool>
<DT><STRONG><$insert text=<long>></STRONG>
    (short: <STRONG><$insert text=<short>></STRONG>)
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
</DL>

Additionally, the following flags can be used with <ln_deftag>
<DL>
<LIFLAG long="CLOSE" short="C">
    macro/tag is a container and requires a coresponding closing tag
<LIFLAG long="JERK" short="J" ONLYDEFTAG>
    tag is only used by jerks
<LIFLAG long="OBSOLETE" short="O">
    macro/tag is obsolete and should not be used any more
<LIFLAG long="SKIPLF" short="S">
    skip a linefeed ("\n") that comes immediatly after the tag
</DL>

For examples, see the section about <ln_macro>.
<P>

</WEBPAGE>


