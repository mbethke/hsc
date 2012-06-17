<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"DTD/xhtml1-strict.dtd">
<!-- here's a regular comment -->
<$macro testib foo:bool=''>
<$if COND=(foo)><i><$else><b></$if>testib<$if COND=(foo)></b><$else></i></$if>
</$macro>

<html>
<head><title>Nesting tests</title></head>
<body>
<i><b>i-b-wrong</i></b>
<testib>
<testib foo>
</body>
</html>
