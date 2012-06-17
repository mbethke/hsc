<$define ROWCOUNT:num="-1">
<$define COLORS-0:string/c="red">
<$define COLORS-1:string/c="green">
<$define COLORS-2:string/c="yellow">

<$macro COLORTR /CLOSE>
<$let ROWCOUNT=(ROWCOUNT & "1")>
<tr style=("background-color: " +
	{"COLORS-" + (ROWCOUNT MOD "3"))}><$content></tr>
<$if cond=("1")><tr></$if></tr><!-- nesting check -->
</$macro>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"DTD/xhtml1-strict.dtd">
<!-- here's a regular comment -->
<html>
<head><title>Colored Rows</title></head>
<body>
<table>
<colortr><td>Blah</td></colortr>
<colortr><td>Fasel</td></colortr>
<colortr><td>Foo</td></colortr>
<colortr><td>Bar</td></colortr>
<colortr><td>Baz</td></colortr>
</table>
</body>
</html>
