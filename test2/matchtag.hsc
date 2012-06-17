<$macro foreach-dir PAT:string/R FMT:string/R>
<table border=1>
<tr>
<th>T</th>
<th>U</th>
<th>G</th>
<th>O</th>
<th>Links</th>
<th>Owner</th>
<th>Group</th>
<th>Size</th>
<th>Date</th>
<th>Name</th>
</tr>
<$exec COMMAND=("ls -l " + PAT + "|tail -n+2|sed 's/.*/<format-entry " + 's="&"' + ">/'")
INCLUDE TEMPORARY>
</table>
</$macro>

<$macro format-entry s:string/R>
<$match s?=s re=(
"(.)(.{9}) +([[:digit:]]+) "+
"([[:alnum:]]+) +([[:alnum:]]+) +([[:digit:]]+) +"+
"(... .. ..:..) +(.*)")
C1=type C2=perms C3=links C4=owner C5=group C6=size C7=date C8=name>
<$match s?=perms re="(...)(...)(...)" C1=p-u C2=p-g C3=p-o>
<tr>
<td><(type)></td>
<td><(p-u)></td>
<td><(p-g)></td>
<td><(p-o)></td>
<td><(links)></td>
<td><(owner)></td>
<td><(group)></td>
<td style="text-align:right"><(size)></td>
<td><(date)></td>
<td style="text-align:left"><(name)></td>
</tr>
</$macro>

<HTML>
<HEAD><TITLE>Pattern matching</TITLE>
<style type="text/css">
td { text-align: center; font-family: monospace; }
</style>
</HEAD>
<BODY>
  <foreach-dir fmt=format-entry pat="">
</BODY>
</HTML>

<* vi: set et ts=2: *>
