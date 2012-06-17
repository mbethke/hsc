<$include FILE="macros.hsc">

<$macro GROUPCONTENTS>
	<GROUPENTRY-NG>Cell 1</GROUPENTRY-NG>
	<GROUPENTRY-NG>Cell 2</GROUPENTRY-NG>
	<GROUPENTRY-NG>
		Cell 3: <strong>A nested table</strong>
		<table border=1 summary="Multiplication table">
		  <FOR VAR=i START=1 TO=4><TR>
			  <FOR VAR=j START=1 TO=4>
					<TD><(i*j)></TD>
				</FOR></TR>
			</FOR>
		</TABLE>
	</GROUPENTRY-NG>
	<GROUPENTRY-NG>Cell 4</GROUPENTRY-NG>
	<GROUPENTRY-NG>Cell 5:<BR>This group has NELEMS=<(NELEMS)></GROUPENTRY-NG>
</$macro>


<WEBPAGE>
<HEADER Title="Dynamic layout groups - Next Generation"></header>
<BODY>
<H1>Groups</H1>
<p style="font-size:150%">The following two groups have been created with
exactly the same contents, but they get arranged differently:</p>
<*----------------------------------------------*>
<P>Column groups lay out their contents in a fixed number of columns.</P>
<COLUMNGROUP-NG NELEMS=2>
	<GROUPCONTENTS>
</COLUMNGROUP-NG>

<*----------------------------------------------*><HR>
<*
<P>Row groups lay out their contents - guess how! :-)</P>
<ROWGROUP NELEMS=3 BORDER=2>
	<GROUPCONTENTS>
</ROWGROUP>
<*----------------------------------------------*><HR>
<p>Complex layouts...</p>
<COLUMNGROUP NELEMS=2 BORDER=3 PAD=3>
	<GROUPENTRY-NG>This is</GROUPENTRY-NG>
	<GROUPENTRY-NG>a columngroup</GROUPENTRY-NG>
	<GROUPENTRY-NG>with a</GROUPENTRY-NG>
	<GROUPENTRY-NG>
		nested rowgroup
		<ROWGROUP NELEMS=2 BORDER=2>
			<GROUPENTRY-NG>One</GROUPENTRY-NG>
			<GROUPENTRY-NG>Two</GROUPENTRY-NG>
			<GROUPENTRY-NG>Three</GROUPENTRY-NG>
			<GROUPENTRY-NG>
				<ROWGROUP NELEMS=1 BORDER=1>
					<GROUPENTRY-NG>And another</GROUPENTRY-NG>
					<GROUPENTRY-NG>single-row rowgroup</GROUPENTRY-NG>
				</ROWGROUP>
			</GROUPENTRY-NG>
		</ROWGROUP>
	</GROUPENTRY-NG>
	<GROUPENTRY-NG>
		and 5 cells
		<COLUMNGROUP NELEMS=2 BORDER=2 PAD=2>
			<GROUPENTRY-NG>And</GROUPENTRY-NG>
			<GROUPENTRY-NG>another</GROUPENTRY-NG>
			<GROUPENTRY-NG>columngroup</GROUPENTRY-NG>
		</COLUMNGROUP>
	</GROUPENTRY-NG>
</COLUMNGROUP>
<*----------------------------------------------*><HR>
<$macro NESTGROUPS>
<COLUMNGROUP NELEMS=2 BORDER=1>
	<GROUPENTRY-NG>C1</GROUPENTRY-NG>
	<GROUPENTRY-NG>C2</GROUPENTRY-NG>
	<GROUPENTRY-NG>C3</GROUPENTRY-NG>
	<GROUPENTRY-NG>
		<ROWGROUP NELEMS=2 BORDER=1>
			<GROUPENTRY-NG>R1</GROUPENTRY-NG>
			<GROUPENTRY-NG>
				<$if COND=(nestit > '0')>
					<$let nestit=(nestit - '1')>
					<NESTGROUPS>
				<$else>
					END
				</$if>
			</GROUPENTRY-NG>
			<GROUPENTRY-NG>R3</GROUPENTRY-NG>
		</ROWGROUP>
	</GROUPENTRY-NG>
	<GROUPENTRY-NG>C5</GROUPENTRY-NG>
</COLUMNGROUP>
</$macro>

Deep nesting...
<$define nestit:num='15'>
<NESTGROUPS>
*>
</BODY>
</WEBPAGE>

<* vi: set ts=2: *>
