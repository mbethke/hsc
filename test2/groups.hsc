<$include FILE="macros.hsc">

<$macro GROUPCONTENTS>
	<GROUPENTRY>Cell 1</GROUPENTRY>
	<GROUPENTRY>Cell 2</GROUPENTRY>
	<GROUPENTRY>
		Cell 3: <strong>A nested table</strong>
		<table border=1 summary="Multiplication table">
		  <FOR VAR=i START=1 TO=4><TR>
			  <FOR VAR=j START=1 TO=4>
					<TD><(i*j)></TD>
				</FOR></TR>
			</FOR>
		</TABLE>
	</GROUPENTRY>
	<GROUPENTRY>Cell 4</GROUPENTRY>
	<GROUPENTRY>Cell 5:<BR>This group has NELEMS=<(NELEMS)></GROUPENTRY>
</$macro>


<WEBPAGE>
<HEADER Title="Dynamic layout groups with HSC">
<BODY>
<H1>Groups</H1>
<p style="font-size:150%">The following two groups have been created with
exactly the same contents, but they get arranged differently:</p>
<*----------------------------------------------*>
<P>Column groups lay out their contents in a fixed number of columns.</P>
<COLUMNGROUP NELEMS=2 BORDER=2>
	<GROUPCONTENTS>
</COLUMNGROUP>

<*----------------------------------------------*><HR>

<P>Row groups lay out their contents - guess how! :-)</P>
<ROWGROUP NELEMS=3 BORDER=2>
	<GROUPCONTENTS>
</ROWGROUP>
<*----------------------------------------------*><HR>
<p>Complex layouts...</p>
<COLUMNGROUP NELEMS=2 BORDER=3 PAD=3>
	<GROUPENTRY>This is</GROUPENTRY>
	<GROUPENTRY>a columngroup</GROUPENTRY>
	<GROUPENTRY>with a</GROUPENTRY>
	<GROUPENTRY>
		nested rowgroup
		<ROWGROUP NELEMS=2 BORDER=2>
			<GROUPENTRY>One</GROUPENTRY>
			<GROUPENTRY>Two</GROUPENTRY>
			<GROUPENTRY>Three</GROUPENTRY>
			<GROUPENTRY>
				<ROWGROUP NELEMS=1 BORDER=1>
					<GROUPENTRY>And another</GROUPENTRY>
					<GROUPENTRY>single-row rowgroup</GROUPENTRY>
				</ROWGROUP>
			</GROUPENTRY>
		</ROWGROUP>
	</GROUPENTRY>
	<GROUPENTRY>
		and 5 cells
		<COLUMNGROUP NELEMS=2 BORDER=2 PAD=2>
			<GROUPENTRY>And</GROUPENTRY>
			<GROUPENTRY>another</GROUPENTRY>
			<GROUPENTRY>columngroup</GROUPENTRY>
		</COLUMNGROUP>
	</GROUPENTRY>
</COLUMNGROUP>
<*----------------------------------------------*><HR>
<$macro NESTGROUPS>
<COLUMNGROUP NELEMS=2 BORDER=1>
	<GROUPENTRY>C1</GROUPENTRY>
	<GROUPENTRY>C2</GROUPENTRY>
	<GROUPENTRY>C3</GROUPENTRY>
	<GROUPENTRY>
		<ROWGROUP NELEMS=2 BORDER=1>
			<GROUPENTRY>R1</GROUPENTRY>
			<GROUPENTRY>
				<$if COND=(nestit > '0')>
					<$let nestit=(nestit - '1')>
					<NESTGROUPS>
				<$else>
					END
				</$if>
			</GROUPENTRY>
			<GROUPENTRY>R3</GROUPENTRY>
		</ROWGROUP>
	</GROUPENTRY>
	<GROUPENTRY>C5</GROUPENTRY>
</COLUMNGROUP>
</$macro>

Deep nesting...
<$define nestit:num='15'>
<NESTGROUPS>

</BODY>
</WEBPAGE>

<* vi: set ts=2: *>
