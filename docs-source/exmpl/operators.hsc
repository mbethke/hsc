    <$define name:string="hugo">
    <$define here:string="here">

    <IMG SRC=(name+".gif") ALT=(name+" was "+here)>
    <$if (name="hugo")>
    This is hugo!
    <$else>
    Maybe it's sepp?
    </$if>
    hsc.prefs: <(GetEnv ("HSCPREFS"))>
