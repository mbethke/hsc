<DOCPAGE chapter="hsc - " title="Environment Variables"
         PREV="message-list.html"
         NEXT="features/spctags.html">

This section describes several environment variables that
affect how <hsc> operates.

<$macro env /CLOSE TITLE:string/required NAME:string/required>
<DT><A NAME=(NAME)><VAR><(TITLE)></VAR></A><DD><$content></DD>
</$macro>

<DL>
<env TITLE="HOME" NAME="home">
If this variable is set, its content will be interpreted as path to
the user's home directory. When looking for <hsc.prefs>, a
<file>lib/hsc.prefs</file> will be added to this value, and before
this also a directory separator (e.g. a <slash>), if necessary.
</env>

<env TITLE="HSCPATH" NAME="hscpath">
This variable specifies the directory where <hsc.prefs> 
(see also <ln-syntax>) is located.<BR>
<STRONG>Example:</STRONG> After a
<PRE>    export HSCPATH=/some/dir/hsc</PRE>
<hsc> will look for <FILE>/some/dir/hsc/hsc.prefs</FILE>.
</env>

<env TITLE="HSCSALARY" NAME="hscsalary">
Within several messages, <hsc> refers to the user as a <qq><jerk></qq>.
This happens if you are using features which are only supported by some
special browsers. Some people say they are forced by their employer to
use those features, and therefore feel insulted by <hsc>.<br>
As a solution, you can store the amount of your monthly payment in this
variable:
<PRE>    export HSCSALARY=1000</PRE>
After this, <hsc> will stop calling you a <qq>jerk</qq>. Instead, it will
now use the term <qq>prostitute</qq>.
</env>

<env TITLE="http_proxy" NAME="http_proxy"></env>
<env TITLE="ftp_proxy" NAME="ftp_proxy"></env>
<env TITLE="gopher_proxy" NAME="gopher_proxy">
These variables specify the address of a proxy if you want to use one for
external URI checking. It is definitely a good idea to do so, because otherwise
FTP and Gopher URIs cannot be checked at all, and also because it reduces
network load and can considerably speed up the checking process if the proxy is
allowed to cache responses. <strong>Note</strong> the lowercase variable
names&mdash;at least on Unixoid OSs case is significant!<br>
The syntax for these variables is compatible with the Lynx webbrowser: the must
specify a full URI starting in http://, followed by a hostname, optionally
followed by a colon and a port, plus an optional final slash. E.g.<br>
<code>ftp_proxy=http://my.proxy.org:8080/</code> (Bourne-Shell), or<br>
<code>setenv ftp_proxy http://192.168.1.42</code> (AmigaShell).
</env>

</DL>

</DOCPAGE>
