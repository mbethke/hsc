<WEBPAGE chapter="hsc - " title="Environment Variables"
         PREV="message-list.html"
         NEXT="features/spctags.html">

This section describes several environment variables that
affect how <hsc> operates.

<$macro env TITLE:string/required NAME:string/required>
<DT><A NAME=(NAME)><VAR><(TITLE)></VAR></A><DD>
</$macro>

<DL>
<env TITLE="HOME" NAME="home">

If this variable is set, its content will be interpreted as path to
the user's home directory. When looking for <hsc.prefs>, a
<file>lib/hsc.prefs</file> will be added to this value, and before
this also a directory separator (e.g. a <slash>), if necessary.

<env TITLE="HSCPATH" NAME="hscpath">

This variable specifies the directory where <hsc.prefs> 
(see also <ln-syntax>) is located.<BR>
<STRONG>Example:</STRONG> After a
<PRE>    setenv HSCPATH work:hsc</PRE>
<hsc> will look for <FILE>work:hsc/hsc.prefs</FILE>.

<env TITLE="HSCSALARY" NAME="hscsalary">

<P>Within several messages, <hsc> refers to the user as a <qq><jerk></qq>.
This happens if you are using features which are only supported by some
special browsers. Some people say they are forced by their employer to
use those features, and therefor feel insulted by <hsc>.</P>

As a solution, you can store the amount of your monthly payment in this
variable:

<PRE>    setenv HSCSALARY 1000</PRE>

After this, <hsc> will stop calling you a <qq>jerk</qq>. Instead, it will
now use the term <qq>prostitute</qq>.
</DL>
</WEBPAGE>
