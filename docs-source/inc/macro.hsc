<* macro defs (short cuts) *>
<$macro stdin><CODE>stdin</CODE></$macro>
<$macro stdout><CODE>stdout</CODE></$macro>
<$macro stderr><CODE>stderr</CODE></$macro>
<$macro vbar>vertical bar (`<CODE>|</CODE>')</$macro>
<$macro TG><CODE>&lt;</$macro>
<$macro /TG>&gt;</CODE></$macro>
<$macro EXEC><KBD></$macro>
<$macro /EXEC></KBD></$macro>
<$macro FILE><I></$macro>
<$macro /FILE></I></$macro>

<$macro hsc><EXEC>hsc</EXEC></$macro>
<$macro hscdepp><EXEC>hscdepp</EXEC></$macro>
<$macro hscpitt><EXEC>hscpitt</EXEC></$macro>
<$macro makefile><FILE>Makefile</FILE></$macro>
<$macro make><EXEC>make</EXEC></$macro>

<$macro UBQ><BLOCKQUOTE></$macro>
<$macro /UBQ></BLOCKQUOTE></$macro>
<$macro WebLint><A HREF="http://www.unipress.com/cgi-bin/WWWeblint">WebLint</A></$macro>
<$macro Aminet FILE:string><A HREF=("http://wuarchive.wustl.edu/~aminet/"+file)>aminet:<(file)></A></$macro>
<$macro EMAIL address:string><A HREF=("mailto:"+address)><(address)></A></$macro>
<$macro hsc-support><A HREF="http://www.giga.or.at/~agi/hsc/index.html">hsc-support-w3page</A></$macro>
<$macro hsc.prefs><FILE>hsc.prefs</FILE></$macro>

<$macro ln_define><A HREF=":features/spctags.html#define"><TG>$define</TG></A></$macro>
<$macro ln_defent><A HREF=":features/prefs.html#defent"><TG>$defent</TG></A></$macro>
<$macro ln_deficon><A HREF=":features/prefs.html#deficon"><TG>$deficon</TG></A></$macro>
<$macro ln_deftag><A HREF=":features/prefs.html#deftag"><TG>$deftag</TG></A></$macro>
<$macro ln_exec><A HREF=":features/exec.html"><TG>$exec</TG></A></$macro>
<$macro ln_include><A HREF=":features/spctags.html#include"><TG>$include</TG></A></$macro>
<$macro ln_let><A HREF=":features/spctags.html#let"><TG>$let</TG></A></$macro>
<$macro ln_macro><A HREF=":macro/macros.html"><TG>$macro</TG></A></$macro>
<$macro ln_make><A HREF=":project/make.html"><make></A></$macro>
<$macro ln_makefile><A HREF=":project/makefile.html"><makefile></A></$macro>
<$macro ln_hscdepp><A HREF=":project/hscdepp.html"><EXEC>hscdepp</EXEC></A></$macro>
<$macro ln_related><A HREF=":related.html">related stuff</A></$macro>
<$macro ln_getenv><A HREF=":features/expressions.html#getenv"><CODE>GetEnv()</CODE></A></$macro>
<$macro ln_getfilesize><A HREF=":features/expressions.html#getfilesize"><CODE>GetFileSize()</CODE></A></$macro>
<$macro ln_getgmtime><A HREF=":features/expressions.html#getgmtime"><CODE>GetGMTime()</CODE></A></$macro>
<$macro ln_gettime><A HREF=":features/expressions.html#gettime"><CODE>GetTime()</CODE></A></$macro>

<* features *>
<$macro fe_absuri><A HREF=":features/absuris.html">absolute URI</A></$macro>
<$macro fe_chkuri><A HREF=":features/checkuri.html">check existence</A></$macro>

<* options *>
<*<$macro op_checkuri><A HREF=":options.html#checkuri"><KBD>CHECKURI</KBD></A></$macro>*>
<$macro op_compact><A HREF=":options.html#compact"><KBD>COMPACT</KBD></A></$macro>
<$macro op_getsize><A HREF=":options.html#getsize"><KBD>GETSIZE</KBD></A></$macro>
<$macro op_iconbase><A HREF=":options.html#iconbase"><KBD>ICONBASE</KBD></A></$macro>
<$macro op_pipein><A HREF=":options.html#pipein"><KBD>PIPEIN</KBD></A></$macro>
<$macro op_prjfile><A HREF=":options.html#prjfile"><KBD>PRJFILE</KBD></A></$macro>
<$macro op_ignore><A HREF=":options.html#ignore"><KBD>IGNORE</KBD></A></$macro>
<$macro op_stripcomment><A HREF=":options.html#stripcomment"><KBD>STRIPCOMMENT</KBD></A></$macro>
<$macro op_stripexternal><A HREF=":options.html#stripexternal"><KBD>STRIPEXTERNAL</KBD></A></$macro>
<$macro op_striptags><A HREF=":options.html#striptags"><KBD>STRIPTAGS</KBD></A></$macro>

<* misc references *>
<$macro jerk><A HREF=":questions.html#jerk">jerk</A></$macro>
<$macro uri><A HREF=":questions.html#uri">URI</A></$macro>
