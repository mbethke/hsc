<html><head><title>redefining</title></head><body>
<$macro sepp>sepp1</$macro>
sepp1: <sepp>
<$macro sepp>sepp2</$macro>
sepp2: <sepp>

<$macro hugo>hugo-open</$macro>
<$macro /hugo>hugo-close</$macro>

<hugo>(inside)</hugo>

<$macro hugo>hugo-simple</$macro>

<hugo>
</body></html>
