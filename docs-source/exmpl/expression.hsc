    <$let name:string="hugo">
    <$let here:string="here">

    <IMG SRC=(name+".gif") ALT=(name+" was "+here)>
    <$if (name="hugo")>
    This is hugo!
    <$else>
    Maybe it's sepp?
    </$if>
    AmigaOS version: <$insert text=(GetEnv "KickStart")>

