<HTML><HEAD><TITLE>kack</TITLE></HEAD><BODY>

<$macro sepp_simple><$message TEXT="sepp_simple" CLASS=note></$macro>
<$macro sepp_nested>
    <sepp_simple>
</$macro>
<$macro sepp_out /CLOSE>sepp_out(<$content>)</$macro>
<$macro sepp_in /MBI="sepp_out">
sepp_in(
<$message TEXT="this is sepp_in" CLASS=note>
)</$macro>

sepp_simple: <sepp_simple>
sepp_nested: <sepp_nested>

<sepp_out><sepp_in></sepp_out>

<sepp_out> blub <$message TEXT="inside sepp_out" CLASS=note> fasel </sepp_out>

</BODY></HTML>
