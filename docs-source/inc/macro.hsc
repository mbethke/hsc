<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">

<* macro defs (short cuts) *>
<$macro stdin><CODE>stdin</CODE></$macro>
<$macro stdout><CODE>stdout</CODE></$macro>
<$macro stderr><CODE>stderr</CODE></$macro>
<$macro dtd CAP:bool><EXEC><$if cond=(CAP)>D<$else>d</$if>td</EXEC></$macro>

<$macro qq /CLOSE>``<$content>''</$macro>
<$macro qqc /CLOSE>``<CODE><$content></CODE>''</$macro>
<$macro bracket>bracket (<qqc>(</qqc>)</$macro>
<$macro closing-bracket>bracket (<qqc>)</qqc>)</$macro>
<$macro hash>hash character (<qqc>#</qqc>)</$macro>
<$macro vbar>vertical bar (<qqc>|</qqc>)</$macro>
<$macro colon>colon (<qqc>:</qqc>)</$macro>
<$macro semicolon>semicolon (<qqc>;</qqc>)</$macro>
<$macro asterisk>asterisk (<qqc>*</qqc>)</$macro>
<$macro underscore>underscore (<qqc>_</qqc>)</$macro>
<$macro period>period (<qqc>.</qqc>)</$macro>
<$macro hyphen>hyphen (<qqc>-</qqc>)</$macro>
<$macro slash>slash (<qqc>/</qqc>)</$macro>
<$macro backslash>backslash (<qqc>\</qqc>)</$macro>
<$macro equal-sign>equal-sign (<qqc>=</qqc>)</$macro>
<$macro greater-than>greater-than (<qqc>&gt;</qqc>)</$macro>
<$macro less-than>less-than (<qqc>&lt;</qqc>)</$macro>

<$macro TG /CLOSE><CODE>&lt;<$content>&gt;</CODE></$macro>
<$macro EXEC /CLOSE><KBD><$content></KBD></$macro>
<$macro FILE /CLOSE><I><$content></I></$macro>

<$macro hsc CAP:bool><EXEC><$if cond=(CAP)>H<$else>h</$if>sc</EXEC></$macro>
<$macro hscdepp LINK:bool CAP:bool><$if cond=(LINK)><A HREF=":project/hscdepp.html"></$if><EXEC><$if cond=(CAP)>H<$else>h</$if>scdepp</EXEC><$if cond=(LINK)></A></$if></$macro>
<$macro hscpaltrow LINK:bool CAP:bool><$if cond=(LINK)><A HREF=":project/hscpaltrow.html"></$if><EXEC><$if cond=(CAP)>H<$else>h</$if>scpaltrow</EXEC><$if cond=(LINK)></A></$if></$macro>
<$macro hscpitt LINK:bool CAP:bool><$if cond=(LINK)><A HREF=":project/hscpitt.html"></$if><EXEC><$if cond=(CAP)>H<$else>h</$if>scpitt</EXEC><$if cond=(LINK)></A></$if></$macro>
<$macro makefile><FILE>Makefile</FILE></$macro>
<$macro makefile.agi><FILE>Makefile.agi</FILE></$macro>
<$macro make><EXEC>make</EXEC></$macro>

<$macro UBQ><BLOCKQUOTE><$content></BLOCKQUOTE></$macro>
<$macro WebLint><A HREF="http://www.unipress.com/cgi-bin/WWWeblint">WebLint</A></$macro>
<$macro Aminet FILE:string TEXT:string><$if COND=(not set TEXT)><$let TEXT=("aminet:"+FILE)></$if><A HREF=("http://wuarchive.wustl.edu/~aminet/dirs/aminet/"+file)><(text)></A></$macro>
<$macro AminetReadMe FILE:string TEXT:string><$if COND=(not set TEXT)><$let TEXT=("aminet:"+FILE)></$if><A HREF=("http://wuarchive.wustl.edu/~aminet/dirs/aminet/"+file)><(text)></A></$macro>
<$macro EMAIL address:string/required><* <A HREF=("mailto:"+address)><(address)></A> *><(address)></$macro>
<$macro hsc-support><A HREF="http://www.giga.or.at/~agi/hsc/">hsc-support-w3-page</A></$macro>
<$macro riscos-support><A HREF="http://www.alphawave.ltd.uk/sergio/hsc.html"><(hsc.Anchor)></A></$macro>
<$macro hsc.prefs><FILE>hsc.prefs</FILE></$macro>
<$macro ExampleNote><STRONG>Example</STRONG>: </$macro>
<$macro TechNote><STRONG>Technical note</STRONG>: </$macro>
<$macro NextStep>NeXTStep</$macro>

<$macro ln-msg id:num/required><A HREF=(":message-list.html#message."+id)>message #<(id)></A></$macro>
<$macro ln-bugs><qq><A HREF=":bugs.html">Known Bugs, Problems And Limitations</A></qq></$macro>
<$macro ln-define><A HREF=":features/spctags.html#define"><TG>$define</TG></A></$macro>
<$macro ln-defent><A HREF=":features/prefs.html#defent"><TG>$defent</TG></A></$macro>
<$macro ln-deficon><A HREF=":features/prefs.html#deficon"><TG>$deficon</TG></A></$macro>
<$macro ln-deftag><A HREF=":features/prefs.html#deftag"><TG>$deftag</TG></A></$macro>
<$macro ln-content><A HREF=":features/spctags.html#content"><TG>$content</TG></A></$macro>
<$macro ln-exec><A HREF=":features/exec.html"><TG>$exec</TG></A></$macro>
<$macro ln-include><A HREF=":features/spctags.html#include"><TG>$include</TG></A></$macro>
<$macro ln-skip-verbatim><A HREF=":features/spctags.html#dontparse"><TG>| .. |</TG></A> (skip verbatim data)</$macro>
<$macro ln-insert-expression><A HREF=":features/spctags.html#expression"><TG>( .. )</TG></A> (insert expression)</$macro>
<$macro ln-let><A HREF=":features/spctags.html#let"><TG>$let</TG></A></$macro>
<$macro ln-macro><A HREF=":macro/macros.html"><TG>$macro</TG></A></$macro>
<$macro ln-make><A HREF=":project/make.html"><make></A></$macro>
<$macro ln-makefile><A HREF=":project/makefile.html"><makefile></A></$macro>
<$macro ln-expression CAP:bool PLURAL:bool><A HREF=":features/expressions.html"><$if cond=(cap)>E<$else>e</$if>xpression<$if cond=(plural)>s</$if></A></$macro>
<$macro ln-related><A HREF=":related.html">Related Stuff</A></$macro>
<$macro ln-cond-assigns><A HREF=":features/assign.html#cond-assign">Conditional Assignments</A></$macro>
<$macro ln-syntax><A HREF=":features/prefs.html">Syntax Definition</A></$macro>
<$macro ln-getenv><A HREF=":features/expressions.html#getenv"><CODE>GetEnv()</CODE></A></$macro>
<$macro ln-getfilesize><A HREF=":features/expressions.html#getfilesize"><CODE>GetFileSize()</CODE></A></$macro>
<$macro ln-getgmtime><A HREF=":features/expressions.html#getgmtime"><CODE>GetGMTime()</CODE></A></$macro>
<$macro ln-gettime><A HREF=":features/expressions.html#gettime"><CODE>GetTime()</CODE></A></$macro>
<$macro ln-existing-ports><A HREF=":ports.html">Existing Ports</A></$macro>

<* features *>
<$macro fe_prjuri><A HREF=":features/uris.html#prjrel">Project Relative URI</A></$macro>
<$macro fe_chkuri><A HREF=":features/checkuri.html">Check Existence</A></$macro>

<* options *>
<$macro op-compact><A HREF=":options.html#compact"><KBD>Compact</KBD></A></$macro>
<$macro op-extension><A HREF=":options.html#extension"><KBD>Extension</KBD></A></$macro>
<$macro op-from><A HREF=":options.html#from"><KBD>From</KBD></A></$macro>
<$macro op-getsize><A HREF=":options.html#getsize"><KBD>GetSize</KBD></A></$macro>
<$macro op-iconbase><A HREF=":options.html#iconbase"><KBD>IconBase</KBD></A></$macro>
<$macro op-pipein><A HREF=":options.html#pipein"><KBD>PipeIn</KBD></A></$macro>
<$macro op-prjfile><A HREF=":options.html#prjfile"><KBD>PrjFile</KBD></A></$macro>
<$macro op-prefsfile><A HREF=":options.html#prjfile"><KBD>PrefsFile</KBD></A></$macro>
<$macro op-ignore><A HREF=":options.html#ignore"><KBD>Ignore</KBD></A></$macro>
<$macro op-includedir><A HREF=":options.html#includedir"><KBD>IncludeDir</KBD></A></$macro>
<$macro op-quotestyle><A HREF=":options.html#quotestyle"><KBD>QuoteStyle</KBD></A></$macro>
<$macro op-rplcent><A HREF=":options.html#rplcent"><KBD>RplcEnt</KBD></A></$macro>
<$macro op-stripcomment><A HREF=":options.html#stripcomment"><KBD>StripComment</KBD></A></$macro>
<$macro op-stripexternal><A HREF=":options.html#stripexternal"><KBD>StripExternal</KBD></A></$macro>
<$macro op-striptags><A HREF=":options.html#striptags"><KBD>StripTags</KBD></A></$macro>
<$macro op-serverdir><A HREF=":options.html#serverdir"><KBD>ServerDir</KBD></A></$macro>

<* environment variables *>
<$macro env-home><A HREF=":envvar.html"><VAR>HOME</VAR></A></$macro>
<$macro env-hscsalary><A HREF=":envvar.html#hscsalary"><VAR>HSCSALARY</VAR></A></$macro>
<$macro env-hscpath><A HREF=":envvar.html#hscpath"><VAR>HSCPATH</VAR></A></$macro>

<* misc references *>
<$macro jerk><A HREF=":questions.html#jerk">jerk</A></$macro>
<$macro uri><A HREF=":questions.html#uri">URI</A></$macro>

<* create reference to grafflwerk-stuff *>
<$macro grafflwerk href:string nolink:bool>
<file><$stripws>
<$if cond=(set href)>
<$define name:string=("grafflwerk/"+href)>
<$if cond=(nolink)>
<(name)>
<$else>
<A HREF=(":../"+name)><(name)></A>
</$if>
<$else>
grafflwerk
</$if>
<$stripws></file>
</$macro>



