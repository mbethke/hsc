<$include file="macros.hsc">

<HTML><HEAD><TITLE>Sections</TITLE></HEAD>
<BODY>
<$define dst:string>

<TABLE BORDER=1>
<FOR VAR=i START=1 TO=1000>
	<TR>
	<_STYLE-SECTION-NUMBER STYLE="numeric" DEST=dst N=(i)>
	<TD><(dst)></TD>
	<_STYLE-SECTION-NUMBER STYLE="roman" DEST=dst N=(i)>
	<TD><(dst)></TD>
	<_STYLE-SECTION-NUMBER STYLE="upperalpha" DEST=dst N=(i)>
	<TD><(dst)></TD>
	<_STYLE-SECTION-NUMBER STYLE="loweralpha" DEST=dst N=(i)>
	<TD><(dst)></TD>
	</TR>
</FOR>
</TABLE>
</BODY>
</HTML>
