<$include file="macros.hsc">

<$define COLUMNS:num=10>
<$define ROWS:num=20>

<HTML>
<HEAD><TITLE>Table Forms</TITLE></HEAD>
<BODY>

<FORM ACTION="http://fooo.bar/cgi-bin/process.pl" METHOD="post">
<COLUMNGROUP NELEMS=(COLUMNS)>
<FOR START=0 TO=(ROWS) VAR="y">
  <FOR START=0 TO=(COLUMNS) VAR="x">
    <GROUPENTRY>
	<input type="text" name=("fld" + x + "." + y) size=(('100' / COLUMNS) + "%")>
    </GROUPENTRY>
  </FOR>
</FOR>
</COLUMNGROUP>
</FORM>
</BODY>
</HTML>

