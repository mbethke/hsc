<$macro sepp /close hugo:string>
sepp   : hugo=<(hugo)>
<$content>
sepp   : hugo=<(hugo)>
</$macro>

<$define hugo:string="content's hugo">

<sepp hugo="sepp's hugo">
content: hugo=<(hugo)>
</sepp>

