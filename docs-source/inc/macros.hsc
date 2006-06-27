<* $Id: macros.hsc,v 1.82 2006/03/15 14:36:03 mb Exp mb $ *>

<* This is a collection of HSC macros that evolved during my development
 * of a (gasp!) homepage and some other webpages. Most macros need
 * one of the newer versions of HSC to work correctly!
 *>
 
<*-----------------------------------------------------------------------*>
<* BASIC                                                                 *>
<*-----------------------------------------------------------------------*>
<* some abbreviations for HSC stuff *>
<$macro _NOTE T:string/R><$message CLASS=note TEXT=(T)></$macro>
<$macro _WARNING T:string/R><$message CLASS=warning TEXT=(T)></$macro>
<$macro _DEBUG T:string/R>
	<$if COND=(defined DEBUG)><$message CLASS=note TEXT=(T)></$if>
</$macro>

<*-------------------------------------*>
<* show warning if compiled with too old an HSC version *>
<$if COND=((not defined hsc.version) or 
           ((hsc.version = "0") and (hsc.revision < "931")))>
  <$message class=warning text=(HSC.LF + "You are using HSC V" + hsc.version + "." +
            hsc.revision +
            ", but this macro file requires HSC V0.931 or greater." +
				    HSC.LF + "Get it from " +
				    "http://www.linguistik.uni-erlangen.de/msbethke/software.html")> 
</$if>

<$define _MACRO-FILE-VERSION:string/C="$Id: macros.hsc,v 1.82 2006/03/15 14:36:03 mb Exp mb $">

<*-------------------------------------*>
<* how to delete temporary files *>
<$if COND=(defined HSC_SYSCMD_RM)>
	<$macro HSC_SYSCMD_RMFILE FILE:string/R>
		<$exec COMMAND=(HSC_SYSCMD_RM + ' ' + FILE)>
	</$macro>
<$else>
	<$if COND=(HSC.System = "amiga")>
		<$macro HSC_SYSCMD_RMFILE FILE:string/R>
			<$exec COMMAND=("delete " + FILE)>
		</$macro>
	<$elseif COND=((HSC.System = "unix") or (HSC.System = "nextstep") or (HSC.System = "beos"))>
		<$macro HSC_SYSCMD_RMFILE FILE:string/R>
			<$exec COMMAND=("rm -f " + FILE)>
		</$macro>
	<$else>
		<$macro HSC_SYSCMD_RMFILE FILE:string/R>
			<$message class=warning text=("Cannot remove file(s) - please define HSC_SYSCMD_RM: "
								  + FILE)>
		</$macro>
	</$if>
</$if>

<*-----------------------------------------------------------------------*>
<* LOOPS                                                                 *>
<*-----------------------------------------------------------------------*>

<* Did you know HSC can do loops? It can!
 * _ITERATE is the "raw" and somewhat awkward version that works like those
 * LISP iterators you have to write in 1st year CS =^>
 * FOR adds ease of use as a container macro with error handling and 
 * optional parameters. FOR's content is expanded in the context of its
 * respective surrounding _ITERATE macro.
 * Example:
 * <FOR VAR=i START=2 TO=10>
 *   <P>Counting: <(i)></P>
 * </FOR>
 * Think of calculated tag attributes...
 * If you want to qualify for the "Obfuscated HSC Contest", you can even
 * modify <VAR> in your loop's body: <$let x=(x - "1")>
 * Nested loops are possible, too:
 * <FOR START=0 TO=3 VAR=y>
 *   <FOR START=0 TO=3 VAR=x>
 *     <('Column ' + x + ', Line ' + y)>
 *   </FOR>
 * </FOR>
 *>
<$macro _ITERATE COND:string/R CONTENT:string/R>
  <(
    "<$if COND=(" + COND + ")>" +
      "<(CONTENT)>" +
      "<_ITERATE COND=(COND) CONTENT=(CONTENT)>" +
    "</$if>"
   )>
</$macro>

<$macro FOR /CLOSE VAR:string/R START:num=1 TO:num/R STEP:num=1>
  <$if COND=(STEP = "0")>
    <$message class=fatal text="STEP must not be zero!">
  </$if>

  <* define _loop_nestlevel if required, otherwise increment it *>
  <$if COND=(defined _loop_nestlevel)>
    <$let _loop_nestlevel = (_loop_nestlevel & "1")>
  <$else>
    <$define _loop_nestlevel:num="1">
  </$if>  
  
  <* define loop-local variable VAR and set it to START *>
  <("<$define " + VAR + ":num=" + START + ">")>
  
  <* define "_loop_running_<level>" required for the BREAK macro *>
  <( "<$define _loop_running_" + _loop_nestlevel + ":bool='1'>" )>
  
  <* Determine the loop condition and do the iterations *>
  <$if COND=(STEP < "0")>
    <_ITERATE COND=("_loop_running_" + _loop_nestlevel + " AND (" + VAR + ">='" +
        TO +"')")
      CONTENT=(HSC.Content + "<$let " + VAR + "=(" + VAR + "&'" + STEP + "')>")>
  <$else>
    <_ITERATE COND=("_loop_running_" + _loop_nestlevel + " AND (" + VAR + "<='" +
        TO +"')")
      CONTENT=(HSC.Content + "<$let " + VAR + "=(" + VAR + "&'" + STEP + "')>")>
  </$if>
</$macro>

<* This macro breaks a FOR-loop on the next iteration. I.e. it's not quite the
 * same as the C keyword that leaves immediately! Setting "N" breaks that
 * many levels of nested loops at once.
 *>
<$macro BREAK /MBI="for|while" N:num>
  <$if COND=(set N)>
    <FOR VAR=_the_break_loop_counter START="1" TO=(N)>
      <(
        "<$if COND=(defined _loop_running_" +
          (_loop_nestlevel - "1" & _the_break_loop_counter) +
        ")>" +
          "<$let _loop_running_" +
            (_loop_nestlevel - "1" & _the_break_loop_counter) +
            "=''>" +
        "</$if>"
      )>
    </FOR>
  <$else>
    <$let {_loop_running_ + _loop_nestlevel} = ''>
  </$if>  
</$macro>

<*-------------------------------------*>
<* The other kind of loop: while()
 * Dead simple to use:
 * <WHILE COND="a < 10">
 *   <$let a=(a & "1")>
 *   A is <(a)>.<br>
 * </WHILE>
 * (You have to $define "a" before, of course!)
 *> 
<$macro WHILE /CLOSE COND:string/R>
  <_ITERATE COND?=COND CONTENT=(HSC.Content)>
</$macro>

<*-----------------------------------------------------------------------*>
<* ARRAYS                                                                *>
<*-----------------------------------------------------------------------*>

<* this macro creates code to define an "array" of variables of type TYPE. The
 * code is written to the variable whose name is passed as VAR (and must exist
 * already) and when expanded will create N variables with prefix P
 * So to create an "array" called "foo" with 10 strings, use
 *
 * <$define code:string>
 * <_MAKE_ARRAYCODE N=10 P=foo TYPE=string VAR=code>
 * <(code)>
 * optional: for big arrays, <$let code=''> to free some KBytes
 * These steps are necessary to allow for macro-local arrays.
 * 
 * See below for some syntactic epsom salts (I wouldn't dare call
 * this "sugar" :)) for this sequence.
 * 
 * You can then access the variables as foo0,foo1 .. foo9.
 * To access varying indices, use dynamic source attributes with
 * symbolic references as described in the "Attribute Assignment"
 * section of the docs.
 * For reading: <$let var = {'foo' + fooindex}>
 * For writing: <$let {'foo' + fooindex} = 'new contents'>
 * If you need the variables initialized to a default value (so they don't
 * throw any errors when read) set the DEFAULT attribute.
 *>
<$macro _MAKE_ARRAYCODE N:num/R P:string/R VAR:string/R TYPE:string='string'
                        DEFAULT:string GLOBAL:bool>
	<$define _mkac-tmpcode:string>	     <* first level generated code *>
	<$define _mkac-varname:string>	     <* dynamic variable name *>
  <$define _mkac-tcode-1:string/C=('<$let ' + VAR + '=(' + VAR + '+"' +
                                   '<' + '$if COND=(not defined ')>
  <$define _mkac-tcode-2:string/C=("'><" + '/$if>")>')>
  <$define _mkac-tcode-3:string/C=("='" + DEFAULT + "'><" + "$else><" +
                                   "$let ")>
  <$define _mkac-init:bool/C=(set DEFAULT)>
  <$define _mkac-langb:string/C='<'>

	<$if COND=(N = '0')>
    <$message class=warning text=("Array size for '" + P + "' is zero!")>
  </$if>
	<$let {VAR}=""> <* clear the variable *>
	<FOR VAR=i START='0' TO=(N)>	    <* define N variables *>
		<$let _mkac-varname = (P + i)>
		<$let _mkac-tmpcode = (_mkac-tcode-1 + _mkac-varname + ')>' +
		      '<$define ' + _mkac-varname + ':' + TYPE)>
		<$if COND=(_mkac-init)>
      <$let _mkac-tmpcode = (_mkac-tmpcode + _mkac-tcode-3)>
      <$let _mkac-tmpcode = (_mkac-tmpcode + _mkac-varname + "='" + DEFAULT)>
      <$let _mkac-tmpcode = (_mkac-tmpcode + _mkac-tcode-2)>
    <$else>
      <$let _mkac-tmpcode = (_mkac-tmpcode + _mkac-tcode-2)>
    </$if>
		<* execute code to add to variable in VAR *>
		<(_mkac-tmpcode)>
	</FOR>
</$macro>

<* The following is a somewhat more consistent (if you want to call it that)
 * syntax for defining an array; it handles the temp variable on its own:
 *
 * <(DEFARRAY_PRE)>
 * <DEFARRAY_MAIN N=10 P=foo TYPE=string DEFAULT="foo-default">
 * <(DEFARRAY_POST)>
 *
 * (note the different notations as two variable expansions and one macro!)
 *>
<$define DEFARRAY_PRE:string/C=
  "<$if COND=(not (defined __code))><$define __code:string></$if>">
<$macro DEFARRAY_MAIN N:num/R P:string/R TYPE:string='string' DEFAULT:string
        GLOBAL:bool>
  <_MAKE_ARRAYCODE N=(N) P=(P) TYPE=(TYPE) DEFAULT?=DEFAULT VAR="__code"
                   GLOBAL?=GLOBAL>
</$macro>
<$define DEFARRAY_POST:string/C="<(__code)><$let __code=''>">

<*-----------------------------------------------------------------------*>
<* SCRIPTING                                                             *>
<*-----------------------------------------------------------------------*>

<* Scripting macros: call virtually any script from HSC and insert its
 * output or read it into a variable. See PERL/PYTHON and their DYN-
 * flavors!
 *>
<$macro INTERPRET-SCRIPT INTERPRETER:string/R PARAMS:string SCRIPT:string/R
        ARGS:string TOVAR:string>
  <$define cmd:string=(INTERPRETER + ' ')>
  <$define tempfile:string/CONST=(HSC.Source.Name + "_sctmp")>
  <$if COND=(set PARAMS)><$let cmd=(cmd + PARAMS + ' ')></$if>
  <$let cmd=(cmd + tempfile + ' ')>
  <$if COND=(set ARGS)><$let cmd=(cmd + ' ' + ARGS)></$if>
  <$export FILE=(tempfile) DATA=(SCRIPT)>
  <$if COND=(set TOVAR)>
    <$exec COMMAND=(cmd) ATTRIBUTE=(TOVAR) TEMPORARY>
  <$else>
    <$exec COMMAND=(cmd) INCLUDE TEMPORARY>
  </$if>
	<HSC_SYSCMD_RMFILE FILE=(tempfile)>
</$macro>

<*-------------------------------------*>
<$macro PERL /CLOSE ARGS:string TOVAR:string>
  <INTERPRET-SCRIPT INTERPRETER="perl" SCRIPT=(HSC.Content) ARGS?=ARGS
                    TOVAR?=TOVAR>
</$macro>

<*-------------------------------------*>
<$macro PYTHON /CLOSE ARGS:string TOVAR:string>
  <INTERPRET-SCRIPT INTERPRETER="python" SCRIPT=(HSC.Content) ARGS?=ARGS
                    TOVAR?=TOVAR>
</$macro>

<*-------------------------------------*>
<* here's for dynamically generated scripts *>

<$macro DYN-PERL SCRIPT:string/R ARGS:string TOVAR:string>
  <INTERPRET-SCRIPT INTERPRETER="perl" SCRIPT=(SCRIPT) ARGS?=ARGS
                    TOVAR?=TOVAR>
</$macro>

<*-------------------------------------*>
<$macro DYN-PYTHON SCRIPT:string/R ARGS:string TOVAR:string>
  <INTERPRET-SCRIPT INTERPRETER="python" SCRIPT=(SCRIPT) ARGS?=ARGS
                    TOVAR?=TOVAR>
</$macro>

<*-----------------------------------------------------------------------*>
<* Assign the entire contents of this container to VAR *>
<$macro TEXT_TO_STRING /CLOSE VAR:string/R>
  <$let {VAR}=(HSC.Content)>
</$macro>

<*-----------------------------------------------------------------------*>
<* Make an ordinal number: 1st, 2nd, etc.                                *>
<$macro ORDINAL N:num/R>
  <$define _ordinalmod:num/C=(N MOD '10')>
  <$define _ordinalsup:string="th">
  <$if COND=((N < '4') or (N > '20'))>
    <$if     COND=(_ordinalmod = '1')>
      <$let _ordinalsup="st">
    <$elseif COND=(_ordinalmod = '2')>
      <$let _ordinalsup="nd">
    <$elseif COND=(_ordinalmod = '3')>
      <$let _ordinalsup="rd">
    </$if>
  </$if>
  <(N)><sup><(_ordinalsup)></sup>
</$macro>

<*-----------------------------------------------------------------------*>
<* PAGES                                                                 *>
<*-----------------------------------------------------------------------*>

<* You may have to adapt this to the doctypes you actually use... *>
<$macro THISDOCTYPE>
<$if COND=(defined HSC.OPTS.XHTML)>
    <$if COND=(defined PROJECT_CHARSET)>
      <$define _tdtcharset:string=(PROJECT_CHARSET)>
    <$else>
      <$if COND=(HSC.OPTS.ENTITYMODE = "utf-8")>
        <$define _tdtcharset:string="utf-8">
      <$else>
        <$define _tdtcharset:string="iso-8859-1">
      </$if>
    </$if>
    <('<|<?xml version="1.0" encoding="' + _tdtcharset + '"?>|>')>
  <$if COND=(defined TRANSITIONAL_DTD)>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "DTD/xhtml1-transitional.dtd">
  <$else>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "DTD/xhtml1-strict.dtd">
  </$if>
<$else>
  <$if COND=(defined TRANSITIONAL_DTD)>
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd">
  <$else>
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
    "http://www.w3.org/TR/html4/strict.dtd">
  </$if>
</$if>
</$macro>

<*-------------------------------------*>
<* Generate a whole header with title and all the META crap in one go.
 * Global variables used (usually correspond to an attribute which takes
 * precedence if present):
 * PROJECT_AUTHOR: e.g. "Matthias Bethke"
 * PROJECT_COPYRIGHT: same as above
 * PROJECT_STYLESHEET: e.g  "main.css" (the name PROJECT_STYLEFILE is still supported)
 * PROJECT_LANGUAGE: e.g. "en_US"
 * PROJECT_HOMEPAGE: e.g. ":", "http://my.home.org/index.html"
 * PROJECT_HEADERDATE: creates a META DATE tag with the date/time of processing
 * PROJECT_HEADERPROFILE: set PROFILE attribute in HEAD
 * PROJECT_CHARSET: create a content-type tag
 * PROJECT_TOCLINKS: insert LINK tags from a table-of-contents file in
 *                   PROJECT_TOC_FILE
 * Set them in your Makefile to generate the corresponding header fields
 * automatically.
 *>
<$macro HEADER /CLOSE
        TITLE:string/R KEYWORDS:string DESCRIPTION:string AUTHOR:string
        LANGUAGE:string STYLESHEET:uri STYLEFILE:uri JAVASCRIPT:string JAVASCRIPT-EXT:uri
        SHORTCUTICON:uri DEFAULTFRAME:string BASE:uri
        ROBOTS:string PROFILE:uri REVISIT-AFTER:string CHARSET:string
        ALLOW-SMARTTAGS:bool NOPROXYCACHE:bool NOBROWSERCACHE:bool
        CLASSIFICATION:string DISTRIBUTION:enum("global|local")
        COPYRIGHT:string REPLY-TO:string TOCLINKS:bool HOMEPAGE:uri>
<$define _hdr_author:string>
<$define _hdr_cr:string>
<$define _hdr_lang:string>
<$define _hdr_profile:uri>
<$define _hdr_homepage:uri>

<* process options that may have project-global defaults *>
<$if COND=(defined PROJECT_AUTHOR)><$let _hdr_author=(PROJECT_AUTHOR)></$if>
<$if COND=(set AUTHOR)><$let _hdr_author=(AUTHOR)></$if>

<$if COND=(defined PROJECT_COPYRIGHT)><$let _hdr_cr=(PROJECT_COPYRIGHT)></$if>
<$if COND=(set COPYRIGHT)><$let _hdr_cr=(COPYRIGHT)></$if>

<$if COND=(defined PROJECT_LANGUAGE)><$let _hdr_lang=(PROJECT_LANGUAGE)></$if>
<$if COND=(set LANGUAGE)><$let _hdr_lang=(LANGUAGE)></$if>

<$if COND=(defined PROJECT_HOMEPAGE)><$let _hdr_homepage=(PROJECT_HOMEPAGE)></$if>
<$if COND=(set HOMEPAGE)><$let _hdr_homepage=(HOMEPAGE)></$if>

<$if COND=(defined PROJECT_HEADERPROFILE)><$let _hdr_profile=(PROJECT_HEADERPROFILE)></$if>
<$if COND=(set PROFILE)><$let _hdr_profile=(PROFILE)></$if>

<HEAD PROFILE?=_hdr_profile>
<$if COND=(set CHARSET)>
  <META HTTP-EQUIV="content-type" CONTENT=("text/html;" + "charset=" + CHARSET)>
<$elseif COND=(defined PROJECT_CHARSET)>
  <META HTTP-EQUIV="content-type" CONTENT=("text/html;" + "charset=" + PROJECT_CHARSET)>
</$if>
<$if COND=(set NOPROXYCACHE)>
  <META HTTP-EQUIV="pragma" CONTENT="no-cache">
</$if>
<$if COND=(set NOBROWSERCACHE)>
  <META HTTP-EQUIV="cache-control" CONTENT="no-cache">
</$if>
<$if COND=(set _hdr_lang)>
  <META HTTP-EQUIV="content-language" CONTENT=(_hdr_lang)>
</$if>

<_SET_HEADER_META PARAM?=KEYWORDS NAME="keywords">
<_SET_HEADER_META PARAM?=DESCRIPTION NAME="description">
<_SET_HEADER_META PARAM?=_hdr_author NAME="author">
<_SET_HEADER_META PARAM?=_hdr_cr NAME="copyright">
<_SET_HEADER_META PARAM?=ROBOTS NAME="robots">
<_SET_HEADER_META PARAM?=REVISIT-AFTER NAME="revisit-after">
<_SET_HEADER_META PARAM?=CLASSIFICATION NAME="classification">
<_SET_HEADER_META PARAM?=DISTRIBUTION NAME="distribution">
<_SET_HEADER_META PARAM?=REPLY-TO NAME="reply-to">
<META NAME="generator" CONTENT=("HSC/" + Hsc.System)>
<$if COND=(not set ALLOW-SMARTTAGS)>
  <META NAME="MSSmartTagsPreventParsing" CONTENT="TRUE">
</$if>

<$if COND=(defined PROJECT_HEADERDATE)>
  <$define _ttf:string=(HSC.Format.Time)>
  <$let HSC.Format.Time="%Y-%m-%dT%T">
  <META NAME="date" CONTENT=(GetTime())>
  <* GMT offsets are still unsupported *>
  <$let HSC.Format.Time=(_ttf)>
</$if>

<$if COND=(set SHORTCUTICON)>
  <LINK REL="shortcut icon" HREF=(SHORTCUTICON)>
</$if>

<$if COND=(set STYLEFILE)>
  <LINK REL="stylesheet" TYPE="text/css" HREF=(STYLEFILE)>
<$elseif COND=(defined PROJECT_STYLESHEET)>
  <LINK REL="stylesheet" TYPE="text/css" HREF=(PROJECT_STYLESHEET)>
  <$if COND=(defined PROJECT_STYLEFILE)>
    <$message class=warning text="PROJECT_STYLESHEET is defined, ignoring obsolete PROJECT_STYLEFILE!">
  </$if>
<$elseif COND=(defined PROJECT_STYLEFILE)>
  <LINK REL="stylesheet" TYPE="text/css" HREF=(PROJECT_STYLEFILE)>
</$if>

<$if COND=(set _hdr_homepage)>
  <LINK REL="start" TITLE="Home page" HREF=(_hdr_homepage)>
</$if>

<$if COND=((set TOCLINKS) or (defined PROJECT_TOCLINKS))>
  <_TABLE-OF-CONTENTS-LINKS>
</$if>

<$if COND=((set BASE) or (set DEFAULTFRAME))>
  <BASE HREF?=BASE TARGET?=DEFAULTFRAME>
</$if>

<$content>

<_INSERT_JAVASCRIPT JS?=JAVASCRIPT JSX?=JAVASCRIPT-EXT>
<TITLE><(TITLE)></TITLE>
</HEAD>
</$macro>

<* a little helper macro to avoid if-orgies in <HEADER> *>
<$macro _SET_HEADER_META PARAM:string NAME:string/R>
<$if COND=(set PARAM)><META NAME=(NAME) CONTENT=(PARAM)></$if>
</$macro>

<* This helper macro creates a bunch of <LINK> tags for sections if requested in
   HEADER (FILE unused as of yet) *>
<$macro _TABLE-OF-CONTENTS-LINKS FILE:string>
	<$define the_toclinks:string=''>
	<$define tocfile:string>

	<$if COND=(defined PROJECT_GENERATE_TOC)>
		<$if COND=(set FILE)>
			<$let tocfile=(FILE)>
		<$elseif COND=(defined PROJECT_TOC_FILE)>
			<$let tocfile=(PROJECT_TOC_FILE)>
		<$else>
			<$let tocfile=((basename (HSC.Source.File)) + '.toc')>
		</$if>

		<$if COND=(fExists(tocfile))>
		<* include file with some TABLE-OF-CONTENTS-ENTRY macros and expand them *>
			<$include FILE=(tocfile) TEMPORARY>
		<$else>
			<$message class=warning text=("Missing TOC file " + tocfile +
                   " in header - rerun HSC to get it right!")>
		</$if>
		<(the_toclinks)>
	</$if>
</$macro>

<* This is used in other places as well, so it gets its own macro *>
<$macro _INSERT_JAVASCRIPT JS:string JSX:uri>
  <$if COND=(set JS)>
    <script TYPE="text/javascript">
      <("<!-- " + HSC.LF + JS + HSC.LF + "//-->")>
    </script>
  </$if>
  <$if COND=(set JSX)>
    <script TYPE="text/javascript" SRC=(JSX)></script>
  </$if>
</$macro>

<*-------------------------------------*>

<$macro WEBPAGE /CLOSE LANG:string><THISDOCTYPE>
<$define _webpage-language:string>
<$if COND=(defined PROJECT_LANGUAGE)>
  <$let _webpage-language=(PROJECT_LANGUAGE)>
</$if>
<$let _webpage-language?=LANG>
<$if COND=(defined HSC.OPTS.XHTML)>
  <html lang?=_webpage-language xmlns="http://www.w3.org/1999/xhtml">
<$else>
  <HTML lang?=_webpage-language>
</$if>
<$content></HTML></$macro>

<$macro LAST_CHANGE TEXT:string="Last change:&nbsp;">
<(TEXT)><(GetFileDate(hsc.source.file))>
</$macro>

<$macro LAST_CHANGE_PAR TEXT:string HR:bool RIGHT:bool
        STYLE:string="font-size:small">
<$if COND=(set right)>
  <_MAKE-CSS-STYLE NAME=text-align ATTR=right DEST=STYLE>
</$if>
<$if COND=(set HR)><HR /></$if><p style=(STYLE)><LAST_CHANGE TEXT?=TEXT></p>
</$macro>


<*-----------------------------------------------------------------------*>
<* GRAPHICS                                                              *>
<*-----------------------------------------------------------------------*>
<* This lets you insert a thumbnail picture in the current document,
 * automagically linked to the full-size picture. Specify the big image's name
 * as IMG, and the macro will assume the thumbnail has the same name with
 * "_tnail" added before the extension. NEWWIN opens the picture in a new
 * window, SIZE lets you add the full picture's size in kilobytes to either the
 * ALT text or the title.
 *
 * TODO: fix problem with absolute URIs
 *>
<$macro THUMBNAIL PIC:uri/R ALT:string/R TITLE:string NEWWIN:bool
        SIZE:enum("alt|title") [__IAlign]>
  <$define _tnailname:string/C=((basename (PIC))+"_tnail."+(extension (PIC)))>
  <$define _tnailpath:string/C=(HSC.DestPath + HSC.Document.Path + _tnailname)>
  <$define _imgpath:string/C=(HSC.DestPath + HSC.Document.Path + PIC)>
  <$define _tgt:string>
  <$define _alttxt:string=(ALT)>
  <$define _titletxt:string>
  <$if COND=(set NEWWIN)>
  	<$let _tgt="_blank">
  </$if>
  <$if COND=(set SIZE)>
    <$if COND=(SIZE = "alt")>
      <$let _alttxt=(_alttxt + " [" + GetFileSize(PIC)+"]")>
    <$else>
      <$if COND=(set TITLE)>
        <$let _titletxt=(TITLE + " [" + GetFileSize(PIC)+"]")>
      <$else>
        <$let _titletxt=(GetFileSize(PIC))>
      </$if>
    </$if>
  <$else>
  </$if>
  <$if COND=(not (fexists(_tnailpath)))>
    <$if COND=(not ((defined _AUTOTHUMB-SCALE) and (defined _AUTOTHUMB-MINDIM)))>
      <$message class=error text="Auto-generated thumbnails need _AUTOTHUMB-SCALE and _AUTOTHUMB-MINDIM set!">
    </$if>
    <PERL ARGS=( _imgpath + " " + _tnailpath + " " + _AUTOTHUMB-SCALE + " " +
                _AUTOTHUMB-MINDIM)>
    my $pix="";
    my ($img,$tnail,$scale,$mindim) = @ARGV;
    $scale /= 100;
    my ($picw,$pich) = (`anytopnm $img 2>/dev/null | pnmfile` =~ /(\d+) by (\d+)/);
    $pix = "-xsize=$mindim",$scale=$mindim/$picw if($mindim > $picw*$scale);
    $pix = "-ysize=$mindim" if($mindim > $pich*$scale);
    $scale="" if(length $pix);
    # TODO: allow for output formats other than JPEG
    system("anytopnm $img 2>/dev/null | pnmscale 2>/dev/null $scale $pix | ppmtojpeg 2>/dev/null -optimize -quality=60 -dct=float >$tnail");
    </PERL>
  </$if>
  <$if COND=(defined HSC.Opts.XHTML)>
    <$if COND=(set NEWWIN)>
      <$message class=warning text="Sorry, NEWWIN doesn't work in XHTML mode!">
    </$if>
    <A HREF=(PIC)>
  <$else>
    <A HREF=(PIC) TARGET?=_tgt>
  </$if>
  <IMG SRC=(_tnailname) ALT=(_alttxt) TITLE?=_titletxt ALIGN?=ALIGN></A>
</$macro>

<*-----------------------------------------------------------------------*>
<* LAYOUT / TeX-ALIKE                                                    *>
<*-----------------------------------------------------------------------*>
<* the following is needed by both the footnote and bibliography macros
   below. It wraps TEXT in TAG like this:
   TEXT="foo" TAG="H1"  ->  <H1>foo</H1> *>
<$macro _WRAP_STRING_IN_TAGS TEXT:string TAG:string>
  <$if COND=(set TEXT)>
    <$if COND=(set TAG)>
      <('<' + TAG + '>' + TEXT + '<' + '/' + TAG + '>' + HSC.LF)>
    <$else>
      <(TEXT + HSC.LF)>
    </$if>
  <$elseif COND=(set TAG)>
    <$message class=warning text="TAG is set, but no TEXT - ignored"> 
  </$if>
</$macro>

<*-------------------------------------*>

<* Footnotes LaTeX style! *>

<* FOOTNOTE is a container that inserts a superscripted number in place of its
 * contents, and appends the contents to a global variable (which is
 * created if nonexistant) instead, marked with a NAMEd anchor.
 * FOOTNOTES, inserted below in the document, will then interpolate this
 * variable as a numbered list.
*>
<$macro FOOTNOTE /CLOSE>
<$if COND=(defined the_footnote_counter)><$stripws>
  <$let the_footnote_counter=(the_footnote_counter & "1")><$stripws>
<$else><$stripws>
  <$define the_footnote_counter:num/GLOBAL=1><$stripws>
  <$define the_footnote_bucket:string/GLOBAL=''><$stripws>
</$if><$stripws>
<$define ftnname:string/CONST=('footnote' + the_footnote_counter)><$stripws>
<$let the_footnote_bucket = (the_footnote_bucket + '<LI><A NAME="' + ftnname +
  '"></A>' + Hsc.Content + 
  '<$if COND=(set BACKLINK)>' +
    '&nbsp;[<A HREF="#footnote'+the_footnote_counter+'r"><(BACKLINK)></A>]' +
  '</$if>' +
  '</LI>' + HSC.LF)><$stripws>
<SMALL><SUP><A NAME=(ftnname + 'r')></A><A HREF=('#' + ftnname)><(the_footnote_counter)></A></SUP></SMALL>
</$macro> 

<*-------------------------------------*>
<* this macro inserts all footnotes accumulated during the processing of the
   current document as an <OL>. The footnote bucket is emptied, just in
   case you should want several "foot"note sections in one HTML file
   See bibliography macro below for TITLE!
*>
<$macro FOOTNOTES RULERS:bool BACKLINK:string TITLE:string>
  <* don't expand to anything if footnotes are not in use *>
  <$if COND=(defined the_footnote_bucket)>
		<* allow special attributes to be set via CSS *>
    <DIV class="footnotes">
    <$if COND=(set RULERS)><HR></$if>
		<$if COND=(set TITLE)>
			<(TITLE)>
		</$if>
    <* interpolate footnote bucket as entries in a numbered list *>
    <OL><(the_footnote_bucket)></OL></DIV>
    <$if COND=(set RULERS)><HR></$if>
    <$let the_footnote_bucket=''>
  </$if>
</$macro>

<$macro FOOTNOTESTITLED TITLE:string="Footnotes" TC:string="H1" RULERS:bool BACKLINK:string>
  <* don't expand to anything if footnotes are not in use *>
  <$if COND=(defined the_footnote_bucket)>
		<$if COND=(defined PROJECT_FOOTNOTES_CAPTION)>
			<FOOTNOTES TITLE?=PROJECT_FOOTNOTES_CAPTION RULERS?=RULERS BACKLINK?=BACKLINK>
		<$else>
			<FOOTNOTES TITLE=("<"+TC+">"+TITLE+"<"+"/"+TC+">") RULERS?=RULERS BACKLINK?=BACKLINK>
		</$if>
  </$if>
</$macro>

<*-------------------------------------*>
<* Some LaTeXish bibliography macros   *>

<* Mainly a shortcut for some CSS stuff...
   TITLE is something like "Bibliography". For no title instead of the default
   (which can be set globally as PROJECT_BIBLIOGRAPHY_CAPTION), use "TITLE=''"
   TC is the optional "Title Container", i.e. usually some formatting
   stuff. Use TITLE="foo" TC="H3" for a result of <H3>foo</H3>
   TITLE may contain further tags or macros!
 *>
<$macro THEBIBLIOGRAPHY /CLOSE TITLE:string="Bibliography" TC:string="H1">
  <DIV class="bibliography">
  <$if COND=(defined PROJECT_BIBLIOGRAPHY_CAPTION)>
		<(PROJECT_BIBLIOGRAPHY_CAPTION)>
	<$else>
		<_WRAP_STRING_IN_TAGS TEXT?=TITLE TAG?=TC>
	</$if>
	<TABLE BORDER="0" SUMMARY?=TITLE>
	<$content>
	</TABLE></DIV>
</$macro>

<*-------------------------------------*>
<* This should be fairly self-explanatory. More typing of attribute names than
   the typical LaTeX bibitem, but with an automagically consistent look! *>
<$macro BIBITEM KEY:string/R AUTHOR:string/R DATE:string/R TITLE:string/R
        PLACE:string PUBLISHER:string TYPE:string EDITION:string>
<$define entry:string=(AUTHOR + ': <CITE>' + TITLE + '</CITE>')>
<$if COND=(set TYPE)><$let entry=(entry + '. ' + TYPE)></$if>
<$if COND=(set EDITION)><$let entry=(entry + ', ' + EDITION)></$if>
<$if COND=(set PLACE)><$let entry=(entry + '. ' + PLACE)></$if>
<$if COND=(set PUBLISHER)><$let entry=(entry + ', ' + PUBLISHER)></$if>
<$let entry=(entry + ': ' + DATE)>
<TR><TD CLASS="key"><A NAME=("bib_"+KEY)></A>[<(KEY)>]</TD>
<TD><(entry)></TD></TR> 
</$macro>

<*-------------------------------------*>
<* Use this to refer to a citation in the text. SRC must be set to the
   BIBITEM's KEY value; TEXT is what is to appear as a "link". Leave it unset
   to get a clickable citation key.
   If you have a separate bibliography page, put its URI in BIBFILE or the
   global variable PROJECT_BIBLIOGRAPHY_URI (if both are set, BIBFILE takes
   precedence!) *>
<$macro CITATION SRC:string/R TEXT:string BIBFILE:uri>
<$define _linkto:string=''>
<$define _linktext:string=''>
<$let _linktext?=TEXT>
<$if COND=(_linktext = '')><$let _linktext=(SRC)></$if>
<$if COND=(defined PROJECT_BIBLIOGRAPHY_URI)>
  <$let _linkto=(PROJECT_BIBLIOGRAPHY_URI)>
</$if>
<$let _linkto?=BIBFILE>
<A HREF=(_linkto + '#bib_' + SRC)>[<(_linktext)>]</A>
</$macro>

<*-------------------------------------*>
<* Table of contents *>
<* The following macros implement LaTeX-like sections. To activate them, call
 * <GIMME-LATEX-SECTIONS DOC=...>
 * somewhere at the start of your document. Depending on <doc>, the top-level
 * element will be either "section" or "chapter". To use special features of the
 * _TEX-SECTION macro like EXTRA (passes extra attributes to the opening tag of
 * the container around the section title, like <h2 class="red">) or HDRLEVEL
 * (allows you to change the container tag around the title, although this is
 * not recommended), write your own wrappers like below.
 * SEPCHAR sets the character to separate section numbers with; NUMSTYLE can be
 * any one documented below for _CONVERT-NUMBER-SYSTEM's SYS attribute, so in
 * addition to decimal you can number alphabetically, roman, or even
 * hexadecimal :)
 * Experimental: BIAS allows starting at a different number than 1
 *>

<$macro GIMME-LATEX-SECTIONS /ONLYONCE DOC:enum("book|report|article")/R
        SEPCHAR:string NUMSTYLE:string BIAS:string>
  <$if COND=(DOC = "article")>
    <$macro SECTION       TITLE:string/R><_TEX-SECTION LEVEL=1 TITLE=(TITLE)></$macro>
    <$macro SUBSECTION    TITLE:string/R><_TEX-SECTION LEVEL=2 TITLE=(TITLE)></$macro>
    <$macro SUBSUBSECTION TITLE:string/R><_TEX-SECTION LEVEL=3 TITLE=(TITLE)></$macro>
    <$macro PARAGRAPH     TITLE:string/R><_TEX-SECTION LEVEL=4 TITLE=(TITLE)></$macro>
    <$macro SUBPARAGRAPH  TITLE:string/R><_TEX-SECTION LEVEL=5 TITLE=(TITLE)></$macro>
  <$else>
    <* same for book and report (PART is not supported) *>
    <$macro CHAPTER       TITLE:string/R><_TEX-SECTION LEVEL=1 TITLE=(TITLE)></$macro>
    <$macro SECTION       TITLE:string/R><_TEX-SECTION LEVEL=2 TITLE=(TITLE)></$macro>
    <$macro SUBSECTION    TITLE:string/R><_TEX-SECTION LEVEL=3 TITLE=(TITLE)></$macro>
    <$macro SUBSUBSECTION TITLE:string/R><_TEX-SECTION LEVEL=4 TITLE=(TITLE)></$macro>
    <$macro PARAGRAPH     TITLE:string/R><_TEX-SECTION LEVEL=5 TITLE=(TITLE)></$macro>
    <$macro SUBPARAGRAPH  TITLE:string/R><_TEX-SECTION LEVEL=6 TITLE=(TITLE)></$macro>
  </$if>
  <$if COND=(set SEPCHAR)>
    <$define PROJECT_SECTIONS_SEPCHAR:string/C/G=(SEPCHAR)>
  </$if>
  <$if COND=(set NUMSTYLE)>
    <$define PROJECT_SECTIONS_NUMSTYLE:string/C/G=(NUMSTYLE)>
  </$if>
  <$if COND=(set BIAS)>
    <$define _bias1:string><$define _bias2:string><$define _bias3:string>
    <$define _bias4:string><$define _bias5:string><$define _bias6:string>
    <$define _l:num='0'><$define _m:num='1'>
    <WHILE COND="not (BIAS == '')">
      <$let _l = (_l & '1')>
      <$let {"_bias" + _l} = (extension BIAS)>
      <$let BIAS = (basename BIAS)>
      <$message class=note text=('XBIAS: _bias' + _l + '=' + {_bias + _l} + ', BIAS=' + BIAS)>
    </WHILE>
    <WHILE COND="(_l gt '0')">
      <$let {'the_section' + _m + '_counter'} = {_bias + _l}>
      <$message class=note text=('BIAS: ' + 'the_section' + _m + '_counter=' + {'the_section' + _m + '_counter'})>
      <$let _l = (_l - '1')>
      <$let _m = (_m & '1')>
    </WHILE>
  </$if>
</$macro>

<* This is what the actual section macros are based on. It's a general
 * sectioning macro with unlimited stacking depth (in practice it is limited by
 * HTML's six header levels though).
 *>
<$macro _TEX-SECTION LEVEL:num TITLE:string EXTRA:string HDRLEVEL:string>
	<$define sectnum:string=''>
	<$define ctrname:string=('the_section' + LEVEL + '_counter')>
  <$define styledctr:string=''>

	<* this section's number *>
	<$define hlvl:string>
	<* headerlevel is either defined explicitely or "Hx" where 1<=x<=6 *>
	<$if COND=(set HDRLEVEL)>
		<$let hlvl=(HDRLEVEL)>
	<$else>
		<$if COND=((LEVEL < '1') or (LEVEL > "6"))>
			<$message class=error text="LEVEL must be between 1 and 6 for current HTML versions!">
		</$if>
		<$let hlvl=('H' + LEVEL)>
	</$if>

  <* add extra attributes if there are any defined *>
	<$if COND=(set EXTRA)>
		<$define open_hdr:string=("<" + hlvl + ' ' + EXTRA + ">")>
	<$else>
		<$define open_hdr:string=("<" + hlvl + ">")>
	</$if>
	<* simpler: the closing tag *>
	<$define close_hdr:string=("<" + "/" + hlvl + ">")>

	<* add to the current counter *>
	<(
    "<$if COND=(defined " + ctrname + ")>" +
      "<$let " + ctrname + "=(" + ctrname + " & '1')>" +
    "<$else>" +
      "<$define " + ctrname + ":num/GLOBAL='1'>" +
    "</$if>"
   )>

	<* make sure all higher-level counters exist *>
	<FOR VAR=i START=1 TO=(LEVEL - "1")>
    <(
      "<$if COND=(not defined the_section" + i + "_counter)>" +
        "<$define the_section" + i + "_counter:num/GLOBAL='1'>" +
      "</$if>"
    )>
  </FOR>

	<* zero all lower-level counters if there are any defined *>
	<FOR VAR=i START=(LEVEL & "1") TO="6">
    <$if COND=(defined {"the_section" + i + "_counter"})>
      <$let {"the_section" + i + "_counter"}='0'>
    </$if>
	</FOR>

	<* concatenate the actual counters, potentially converted according to the
   * current NUMSTYLE and separated by periods or SEPCHARs *>
	<FOR VAR=i START=1 TO=(LEVEL)>
		<$if COND=(i > "1")>
      <$if COND=(defined PROJECT_SECTIONS_SEPCHAR)>
			  <$let sectnum = (sectnum + PROJECT_SECTIONS_SEPCHAR)>
      <$else>
			  <$let sectnum = (sectnum + '.')>
      </$if>
		</$if>
    <$if COND=(defined PROJECT_SECTIONS_NUMSTYLE)>
      <_CONVERT-NUMBER-SYSTEM N={"the_section" + i + "_counter"} DEST="styledctr"
                             SYS=(PROJECT_SECTIONS_NUMSTYLE)>
    <$else>
      <$let styledctr={"the_section" + i + "_counter"}>
    </$if>
    <$let sectnum=(sectnum + styledctr)>
	</FOR>

	<* insert the counter string and title at current location *>
 	<( open_hdr + '<A NAME="hscsectname' + sectnum + '">' +
     sectnum + "</A>&nbsp;" +  TITLE + close_hdr )>

	<* if we want a table of contents, do this *>
	<$if COND=(defined PROJECT_GENERATE_TOC)>
		<* just write a single macro call to the TOC file. this gets expanded
       when TABLE-OF-CONTENTS reads in the file! *>
		<$export FILE=(HSC.Source.Path + (basename (HSC.Source.Name)) + ".toc")
             APPEND DATA=("<TABLE-OF-CONTENTS-ENTRY LEVEL=" + LEVEL +
             " NUMBER='" + sectnum + "' TITLE='" + TITLE + "'>" + HSC.LF)>
	</$if>
</$macro>

<*-------------------------------------*>
<* This macro converts a positive numeric value N to one of several numbering
 * systems SYS and stores the result in DEST:
 * numeric   : no conversion
 * decimal   : same as numeric
 * loweralpha: lowercase letters; a, b, c, ..., z, aa, ab,...
 * upperalpha: uppercase letters; A, B, C, ..., Z, AA, AB,...
 * alpha     : same as loweralpha
 * lowerroman: lowercase roman numerals: i, ii, iii, iv, v, vi, ...
 * upperroman: uppercase roman numerals: I, II, III, IV, V, VI, ...
 * roman     : same as upperroman
 * lowerhex  : lowercase hexadecimal: 1, 2, 3, ..., a, b, ..., f, 10, 11, ...
 * upperhex  : uppercase hexadecimal: 1, 2, 3, ..., A, B, ..., F, 10, 11, ...
 * hex       : same as lowerhex
 *>
<$macro _CONVERT-NUMBER-SYSTEM SYS:string/R DEST:string/R N:num/R>
  <$let {DEST} = ''>

  <$if COND=((SYS = 'numeric') or (SYS = 'decimal'))>
    <$let {DEST}=(N)>

  <$elseif COND=((SYS = 'roman') or (SYS = 'lowerroman') or (SYS = 'upperroman'))>
    <$if COND=(SYS = 'upperroman')>
      <$define _m:string/C='M'><$define _d:string/C='D'><$define _c:string/C='C'>
      <$define _l:string/C='L'><$define _x:string/C='X'><$define _v:string/C='V'>
      <$define _i:string/C='I'>
    <$else>
      <$define _m:string/C='m'><$define _d:string/C='d'><$define _c:string/C='c'>
      <$define _l:string/C='l'><$define _x:string/C='x'><$define _v:string/C='v'>
      <$define _i:string/C='i'>
    </$if>
    <$if COND=(N > '0')>
      <WHILE COND="N > '0'">
        <$if     COND=(N >= '1000')>
          <$let {DEST} = ({DEST} + _m)>     <$let N = (N - '1000')>
        <$elseif COND=(N >=  '900')>
          <$let {DEST} = ({DEST} + _c + _m)><$let N = (N -  '900')>
        <$elseif COND=(N >=  '500')>
          <$let {DEST} = ({DEST} + _d)>     <$let N = (N -  '500')>
        <$elseif COND=(N >=  '400')>
          <$let {DEST} = ({DEST} + _c + _d)><$let N = (N -  '400')>
        <$elseif COND=(N >=  '100')>
          <$let {DEST} = ({DEST} + _c)>     <$let N = (N -  '100')>
        <$elseif COND=(N >=   '90')>
          <$let {DEST} = ({DEST} + _x + _c)><$let N = (N -   '90')>
        <$elseif COND=(N >=   '50')>
          <$let {DEST} = ({DEST} + _l)>     <$let N = (N -   '50')>
        <$elseif COND=(N >=   '40')>
          <$let {DEST} = ({DEST} + _x + _l)><$let N = (N -   '40')>
        <$elseif COND=(N >=   '10')>
          <$let {DEST} = ({DEST} + _x)>     <$let N = (N -   '10')>
        <$elseif COND=(N >=    '9')>
          <$let {DEST} = ({DEST} + _i + _x)><$let N = (N -    '9')>
        <$elseif COND=(N >=    '5')>
          <$let {DEST} = ({DEST} + _v)>     <$let N = (N -    '5')>
        <$elseif COND=(N >=    '4')>
          <$let {DEST} = ({DEST} + _i + _v)><$let N = (N -    '4')>
        <$else>
          <$let {DEST} = ({DEST} + _i)>     <$let N = (N -    '1')>
        </$if>
      </WHILE>
    </$if>

  <$elseif COND=((SYS = 'hex') or (SYS = 'lowerhex') or (SYS = 'upperhex'))>
    <$define _q:num>
    <$define _r:num>
    <WHILE COND="N > '0'">
      <$let _q = (N / '16')>
      <$let _r = (N mod '16')>
      <$if COND=(_r >= '10')>
        <$if COND=(SYS = 'upperhex')>
          <$let {DEST}=(chr(ord('A') & _r - '10') + {DEST})>
        <$else> <* lowerhex or hex *>
          <$let {DEST}=(chr(ord('a') & _r - '10') + {DEST})>
        </$if>
      <$else>
        <$let {DEST}=(_r + {DEST})>
      </$if>
      <$let N=(_q)>
    </WHILE>

  <$elseif COND=((SYS = 'alpha') or (SYS = 'loweralpha') or (SYS = 'upperalpha'))>
     <WHILE COND="N > '0'">
      <$let N=(N - '1')>
      <$if COND=(SYS = "upperalpha")>
        <$let {DEST}=((CHR((N MOD '26') & ORD("A"))) + {DEST})>
      <$else> <* loweralpha or alpha *>
        <$let {DEST}=((CHR((N MOD '26') & ORD("a"))) + {DEST})>
      </$if>
      <$let N = (N / '26')>
    </WHILE>

  <$else>
    <$message class=warning text=("Unknown numbering system '" + SYS + "', using 'numeric'!")>
    <$let {DEST}=(N)>
  </$if>
</$macro>

<*-------------------------------------*>
<* this macro will only be used in TOC-files written by the above macros
 * LEVEL:  this entry's logical heading level
 * NUMBER: section number
 * TITLE:  section title
 *
 * TODO: extend the whole system to allow for global TOC pages... (numbering?)
 * TODO: output TOCs for inclusion as navigation menus
 *>
<$macro TABLE-OF-CONTENTS-ENTRY
				LEVEL:num/R NUMBER:string/R TITLE:string/R>
<* Distinguish between the TABLE-OF-CONTENTS environment and
   TABLE-OF-CONTENTS-LINKS in HEADER *>
<$if COND=(defined the_toc)>
  <* assign a style to the current row if text sizes should be adapted *>        
  <$if COND=(defined _TOC_USE_TEXT_SIZES)>
    <$if COND=(LEVEL = "1")>
      <$define _tocentry:string='<TR STYLE="font-size:x-large">'>
    <$elseif COND=(LEVEL = "2")>
      <$define _tocentry:string='<TR STYLE="font-size:large">'>
    <$elseif COND=(LEVEL = "3")>
      <$define _tocentry:string='<TR STYLE="font-size:medium">'>
    <$else>
      <$define _tocentry:string='<TR STYLE="font-size:small">'>
    </$if>  
  <$else>
    <$define _tocentry:string='<TR>'>
  </$if>
  
  <* update max_toc_level (must be defined in TABLE-OF-CONTENTS!) *>
  	<$if COND=(max_toc_level < LEVEL)><$let max_toc_level=(LEVEL)></$if>
  <* emit an empty cell for indentation according to level *>
    <$if COND=(LEVEL > "1")>
      <$let _tocentry = (_tocentry +
       '<TD ALIGN="left" COLSPAN="' + (LEVEL - "1") + '"></TD>')>
    </$if>
  <* add section number *>
  	<$let _tocentry = (_tocentry + '<TD>' + NUMBER + '</TD>')>
  <* make a cell that stretches to the right of the table *>
  	<$let _tocentry = (_tocentry +
     '<TD COLSPAN=(max_toc_level - "'+(LEVEL - "1")+'")>')>
  <* emit a link, add title, close anchor, cell and row *>
  	<$let _tocentry = (_tocentry + '<A HREF="#hscsectname' + NUMBER + '">' + TITLE)>
  	<$let _tocentry = (_tocentry + '</A></TD></TR>' + HSC.LF)>
  	<* add _tocentry to the actual TOC string *>
    <* we have to delay the actual TOC creation until after all
     * TABLE-OF-CONTENTS-ENTRY macros have been processed, so max_toc_level has
     * its final value, therefore everything has to go into the_toc first!  *>
  	<$let the_toc = (the_toc + _tocentry)>
<$else>
  <$let the_toclinks = (the_toclinks + '<LINK REL="section" HREF="#hscsectname' +
    NUMBER + '" TITLE="' + TITLE + '">' + HSC.LF)>
</$if>
</$macro>

<*-------------------------------------*>
<* This emits the actual table of contents in a nice TeXy format :-)
 * If your TOC-file is neither called <srcname>.toc nor defined in
 * PROJECT_TOC_FILE, you may specify it as FILE. "TITLE" should be
 * self-explanatory.
 * TEXTSIZE make the TOC use bigger fonts for levels 1 and 2, which is
 * non-standard but may look nice in places anyway.
 * If your TOC is above the actual text, you will have to rerun HSC
 * if you change anything about the sections to see the changes in the TOC. If
 * the TOC file is missing on the first run, you will get a warning.
 *>
<$macro TABLE-OF-CONTENTS TITLE:string FILE:string TEXTSIZE:bool>
	<$define max_toc_level:num=1>
	<$define the_toc:string=''>
	<$define tocfile:string>

	<$if COND=(defined PROJECT_GENERATE_TOC)>
    <$if COND=(set TEXTSIZE)>
     <$define _TOC_USE_TEXT_SIZES:bool>
    </$if>

		<$if COND=(set FILE)>
			<$let tocfile=(FILE)>
		<$elseif COND=(defined PROJECT_TOC_FILE)>
			<$let tocfile=(PROJECT_TOC_FILE)>
		<$else>
			<$let tocfile=(HSC.Source.Path + (basename (HSC.Source.Name)) + '.toc')>
		</$if>

		<$if COND=(Exists(tocfile))>
		<* include file with some TABLE-OF-CONTENTS-ENTRY macros and expand them *>
			<$include FILE=(tocfile)>
		<$else>
			<$define errormsg:string="Missing TOC file - rerun HSC to get it right!">
			<$let the_toc=("<TR><TD>" + errormsg + "</TD></TR>")>
			<$message class=warning text=(errormsg)>
		</$if>
		<* now expand the actual TOC in a context where max_toc_level has its
			 final value already *>
		<TABLE CLASS="table_of_contents" BORDER="0" SUMMARY="Table of Contents">
		<$if COND=(set TITLE)>
			<TR><TH COLSPAN=(max_toc_level & "1") ALIGN="left"><(TITLE)></TH></TR>
		</$if>
		<(the_toc)>
		</TABLE>
		<* get rid of the TOC file *>
		<HSC_SYSCMD_RMFILE FILE=(tocfile)>
	</$if>
</$macro>

<*-----------------------------------------------------------------------*>
<* HIERARCHICAL NAVIGATION MENUS                                         *>
<*-----------------------------------------------------------------------*>

<* MENUITEMs link to a certain page from a hierarchical menu. The URI to link to
 * must be given by HREF; TEXT is the text to appear as a link. HREF must
 * currently be a project-relative URI, i.e. start in ':'.
 * If the HREF attribute is missing, the item will become a "caption", i.e. it
 * will not yield a link and any submenu nested into it will always be displayed
 * when the MENUITEM is visible. Setting KEY to a character will add an
 * ACCESSKEY attribute to the item's A tag.
 * NOTE: Due to the way TOPMENU works, MENUITEMs must not contain anything but
 * a SUBMENU! HSC currently has no way of enforcing only a single SUBMENU inside
 * an item, but as far as I can see, anything else would be silly.
 *>
<$macro MENUITEM /CLOSE /MBI="topmenu|submenu" HREF:uri TEXT:string/R
        TITLE:string='_HSC_NO_TITLE_' KEY:string>
  
  <* increment the item counter for the current menu level *>
  <$let {"_menu-itemctr"+_menu-level} = ({"_menu-itemctr"+_menu-level} & '1')>
  
  <* make a complete menu path identifier *>
  <_make-current-item-counter>
  <$let {"_menu-currentitem"+_menu-level} = (_current-item-counter)>
  
  <* set boolean var for captions to be checked in SUBMENU *>
  <$let _menu-iscaption=(not set HREF)>
  <$if cond=(_menu-try)>
    <* just do the checks, don't output any text *>
    <$if cond=(({'_menu-expand' + _menu-level} = '') or _menu-iscaption)>
      <* if no expansion has been found yet for the current level 
         or this is a caption (i.e. no HREF set) *>
      <$if COND=(set HREF)>
        <$if cond=(_menu-rel-uri = HREF)>
          <* if HREF matches the current document's URI *>
          <* remember to expand the current level submenu and highlight the
             current item *>
          <$let {'_menu-expand' + _menu-level} = {'_menu-currentitem' + _menu-level}>
        </$if>
      </$if>
      <(hsc.content)>
      <* propagate any expansion request upwards *>
      <$if cond=({'_menu-currentitem'+_menu-level} = {'_menu-expand' + _menu-level})>
        <$let {'_menu-expand'+(_menu-level - '1')} = {'_menu-currentitem'+(_menu-level - '1')}>
      </$if>
    </$if>
  <$else>
    <* this generates the real menu in the second expansion *>
    <$if COND=(set HREF)>
      <$if cond=(_menu-rel-uri = HREF)>
        <* the current item wants to be highlighted *>
        <_MENUITEM-TAG TITLE=(TITLE)><$stripws>
        <(_activelinktagopen)><$stripws><(TEXT)><$stripws><(_activelinktagclose)>
        <$stripws>
        <(hsc.content)>
      <$else>
        <* a regular item, visible but not active *>
        <_MENUITEM-TAG TITLE=(TITLE)><a href=(HREF) ACCESSKEY?=KEY><(TEXT)></a>
        <(hsc.content)>
      </$if>
    <$else>
      <* a caption w/o HREF *>
      <_MENUITEM-TAG TITLE=(TITLE)><(TEXT)>
      <$stripws>
      <(hsc.content)>
    </$if>
    <_MENUITEM-TAG CLOSE>
  </$if>
</$macro>

<$macro _make-current-item-counter>
  <$let _current-item-counter=''>
  <FOR VAR=i START=1 TO=(_menu-level)>
    <$let _current-item-counter=(_current-item-counter + "." + {"_menu-itemctr" + i})>
  </FOR>
</$macro>


<* This opens a new menu. The maximum submenu level is given by
 * MAXNESTDEPTH with a default of 4, which should be plenty for normal
 * applications.
 * The attributes:
 * MENUSTYLE gives an extra STYLE attribute to apply to the whole
 *           (top-level, inherited downwards) menu, e.g. to set the font size.
 * AUTOCLASS gives every menu level its own class, using the prefix specified
 *           here, with a numeric suffix of 1..MAXNESTDEPTH.
 * MENUCLASS gives a certain CLASS attribute to this menu only
 * ITEMHLSTYLE specifies a style to apply to menuitems that get highlighted to
 *             show the user the position in the menu hierarchy corresponding
 *             to the current page. "background-color:#fff" or something...
 * ITEMHLID  specifies an ID to give to menuitems to be highlighted
 * LINKCURRENT makes the entry for the current page a link instead of a simple
 *             text entry. Note that this is considered bad under accessibility
 *             aspects!
 *          attribute
 * 
 * TOPMENUs may only contain MENUITEMs, which in turn may contain further
 * SUBMENUs. 
 *>
<$macro TOPMENU /CLOSE /MBI="body" /NAW="topmenu"
        MAXNESTDEPTH:num='4' MENUSTYLE:string='' ITEMHLSTYLE:string
        ITEMHLID:string AUTOCLASS:string MENUCLASS:string=''
        LINKCURRENT:bool JSCLICKS:bool>
  <$define _menu-try:bool='1'>
  <$define _menu-iscaption:bool=''>
  <$define _menu-level:num='1'>
  <$define _menu-rel-uri:uri/C=(":" + HSC.Document.URI)>
  <$define _activelinktagopen:string='<span'>
  <$define _activelinktagclose:string='</span>'>
  <$define _hiliteid:string=''>
  <$define _current-item-counter:string=''>

  <$let MAXNESTDEPTH=(MAXNESTDEPTH & '1')>
  <* define arrays for item counters, menu expansion and highlighting *>
  <$define _arraycode:string>
  <_MAKE_ARRAYCODE N=(MAXNESTDEPTH) P=_menu-itemctr TYPE=num VAR=_arraycode DEFAULT='0'> 
  <(_arraycode)>
  <_MAKE_ARRAYCODE N=(MAXNESTDEPTH) P=_menu-expand TYPE=string VAR=_arraycode DEFAULT=''> 
  <(_arraycode)>
  <_MAKE_ARRAYCODE N=(MAXNESTDEPTH) P=_menu-currentitem TYPE=string VAR=_arraycode DEFAULT=''> 
  <(_arraycode)>
  <$let _arraycode=''>

  <$if COND=(LINKCURRENT)>
    <$let _activelinktagopen  = '<a href=(HREF) accesskey?=KEY'>
    <$let _activelinktagclose = '</a>'>
  </$if>
  
  <$if COND=(set ITEMHLID)>
    <$let _activelinktagopen=(_activelinktagopen + ' ID="'+ITEMHLID+'"')>
  </$if>
  
  <$if COND=(set ITEMHLSTYLE)>
    <$let _activelinktagopen = (_activelinktagopen + ' style="' + ITEMHLSTYLE + '"')>
  </$if>

  <$let _activelinktagopen = (_activelinktagopen + '>')>
  <$if COND=(_activelinktagopen = "<span>")>
    <$let _activelinktagopen = '')>
    <$let _activelinktagclose = '')>
  </$if>

  <* expand content once with _menu-try set *>
  <(hsc.content)>
  <* reset some variables *>
  <$let _menu-try=''>
  <$let _menu-level='1'>
  <FOR VAR=i START=0 TO=(MAXNESTDEPTH - '1')>
    <$let {"_menu-itemctr" + i}='0'>
  </FOR>
  <* open a new menu and expand everything anew, with _menu-try reset *>
  <_SUBMENU-TAG>
  <(hsc.content)>
  <_SUBMENU-TAG CLOSE>
</$macro>

<* A SUBMENU may occur inside a MENUITEM and contain more MENUITEMs. See
 * TOPMENU for a description of MENUSTYLE/MENUCLASS.
 * Set FILE to a HSC source filename to get the menu items from a separate
 * source (which will help keeping down the number of files depending upon
 * any particular menu file).
 *>
<$macro SUBMENU /CLOSE /MBI="menuitem"
        MENUSTYLE:string='' MENUCLASS:string='' FILE:string>
  <* increase the current menu level for per-level counter etc. *>
  <$let _menu-level=(_menu-level & '1')>
  <* if we're just trying, simply expand all contents *>
  <$if cond=(_menu-try)>
    <$if COND=(set FILE)>
      <$include FILE=(FILE) TEMPORARY><* do not record this "trial" inclusion *>
    <$else>
      <(hsc.content)>
    </$if>
  <$else>
    <* only expand contents if _menu-expand<n-1> is our parent item's number or
       we are directly under a caption *>
    <$if cond=(_menu-iscaption or
                 ({"_menu-currentitem" + (_menu-level - '1')} =
                  {"_menu-expand"  + (_menu-level - '1')}))>
      <* put everything into an itemized list *>
      <_SUBMENU-TAG>
      <$if COND=(set FILE)>
        <$include FILE=(FILE)><* record dependency here if required *>
      <$else>
        <(hsc.content)>
      </$if>
      <_SUBMENU-TAG CLOSE>
    </$if>
  </$if>
  <* zero the item counter for further submenus on the same level *>
  <$let {"_menu-itemctr" + _menu-level}='0'>
  <$let _menu-level=(_menu-level - '1')>
</$macro>

<* Helper macros: generate submenu/item tags. They could be containers but
 * rather provide CLOSE attributes to avoid excessively deep container macro
 * nesting and consequent debugging problems.
 *>
<$macro _SUBMENU-TAG CLOSE:bool>
  <* if closing, just emit a closing list tag *>
  <$if COND=(set CLOSE)>
    </ul>
  <$else>
    <$define _tag:string="<ul">
    <* MENUSTYLE has an empty default to allow for nesting SUBMENU macros *>
    <$if COND=(not (MENUSTYLE = ''))>
      <$let _tag=(_tag + ' STYLE="' + MENUSTYLE + '"')>
    </$if>
    <$if COND=((not (MENUCLASS = '')) or (set AUTOCLASS))>
      <$if COND=(not (MENUCLASS = ''))>
        <$let _tag=(_tag + ' CLASS="' + MENUCLASS + '"')>
      <$else>
        <$let _tag=(_tag + ' CLASS="' + AUTOCLASS + _menu-level + '"')>
      </$if>
    </$if>
    <* this actually yields the opening tag :) *>
    <(_tag + ">")>
  </$if>
</$macro>

<$macro _MENUITEM-TAG CLOSE:bool STYLE:string TITLE:string='__HSC_NO_TITLE__' EXTRA:string=''>
  <$if COND=(set CLOSE)>
    <* just close a list item *>
    </li>
  <$else>
    <$define _menuitem_tag:string="<li">
    <$if COND=(set STYLE)>
      <$let _menuitem_tag=(_menuitem_tag + ' STYLE="' + STYLE + '" ' + EXTRA)>
    </$if>
    <$if COND=(not (TITLE = '_HSC_NO_TITLE_'))>
      <$let _menuitem_tag=(_menuitem_tag + ' TITLE="' + TITLE + '"')>
    </$if>
    <$if COND=(set JSCLICKS)>
      <$let _menuitem_tag=(_menuitem_tag + ' onClick="menuclick(this)"')>
    </$if>
    <* expand to tag *>
    <(_menuitem_tag + ">")><$stripws>
  </$if>
</$macro>


<*-----------------------------------------------------------------------*>
<* DYNAMIC LAYOUT: COLUMNS AND GROUPS                                    *>
<*-----------------------------------------------------------------------*>

<*-------------------------------------*>
<* Dynamic layout with tables: use ROWGROUP or COLUMNGROUP to layout
 * arbitrarily many HTML objects of any kind in a regular grid:
 * <ROWGROUP NELEMS=3>
 *   <GROUPENTRY><IMG SRC="i1"></GROUPENTRY>
 *   <GROUPENTRY><IMG SRC="i2"></GROUPENTRY>
 *   <GROUPENTRY><IMG SRC="i3"></GROUPENTRY>
 *   <GROUPENTRY><IMG SRC="i4"></GROUPENTRY>
 *   <GROUPENTRY><IMG SRC="i5"></GROUPENTRY>
 * </ROWGROUP>
 * yields this layout:
 *   i1 i4
 *   i2 i5
 *   i3
 * while COLUMNGROUP would put them like this:
 *   i1 i2 i3
 *   i4 i5
 *
 * To start a new row/column respectively, use the COLUMNGROUP-NEWROW
 * and ROWGROUP-NEWCOLUMN macros.
 *>
<$macro ROWGROUP /CLOSE NELEMS:num/R PAD:num BORDER:num STYLE:string
                        CLASS:string>
	<$if COND=(NELEMS = '0')><$message class=fatal text="NELEMS must not be zero!"></$if>

  <* the group level will be used in GROUPENTRY to deal with nested groups *>
	<$if COND=(not (defined _the_group_level))>
		<$define _the_group_level:string='1'>
    <$define _the_group_nelems:num=(NELEMS)>
    <$define _the_group_counter:num=(NELEMS)>
	  <$define _the_group_columnwise:bool=''>
	  <$define _the_group_arrayprefix:string=''>
	  <$define _the_group_newrow:bool=''> <* only for nested COLUMNGROUPs *>
	  <$define _groupentry_celltype:string=''> <* used in GROUPENTRYs *>
	<$else>
		<$let _the_group_level=(_the_group_level & '1')>
    <("<$define _the_group_nelems" + _the_group_level + ":num=(_the_group_nelems)>")>
    <("<$define _the_group_counter" + _the_group_level + ":num=(_the_group_counter)>")>
	  <("<$define _the_group_columnwise" + _the_group_level + ":bool=(_the_group_columnwise)>")>
	  <("<$define _the_group_arrayprefix" + _the_group_level + ":string=(_the_group_arrayprefix)>")>
	  <("<$define _groupentry_celltype" + _the_group_level + ":string=(_groupentry_celltype)>")>
    <$let _the_group_nelems=(NELEMS)>
    <$let _the_group_counter=(NELEMS)>
	  <$let _the_group_columnwise=''>
	</$if>

  <* construct prefix for the row array according to level *>
	<$let _the_group_arrayprefix=("_rowgroup_l" + _the_group_level + "r")>

  <* create variables <prefix>0 .. <prefix><NELEMS> *>
  <(DEFARRAY_PRE)>
  <DEFARRAY_MAIN N=(NELEMS) P=(_the_group_arrayprefix) DEFAULT=''>
  <(DEFARRAY_POST)>

  <* insert all GROUPENTRYs, which will only add text to array vars! *>
  <(HSC.Content)>

  <* start the layout table *>
  <TABLE CELLPADDING?=PAD BORDER?=BORDER SUMMARY="rowgroup-layout" CLASS?=CLASS>

  <* now emit all rows previously accumulated in the array *>
	<FOR VAR=("_rowgrp_emitctr" + _the_group_level)
       START=0 TO=(_the_group_nelems - '1')>
    <TR>
      <({_the_group_arrayprefix + {"_rowgrp_emitctr" + _the_group_level}})>
    </TR>
	</FOR>
  </TABLE>

  <$if COND=(_the_group_level > '1')>
    <$let _the_group_nelems={"_the_group_nelems" + _the_group_level}>
    <$let _the_group_counter={"_the_group_counter" + _the_group_level}>
	  <$let _the_group_columnwise={"_the_group_columnwise" + _the_group_level}>
	  <$let _the_group_arrayprefix={"_the_group_arrayprefix" + _the_group_level}>
	  <$let _groupentry_celltype={"_groupentry_celltype" + _the_group_level}>
  </$if>
  <$let _the_group_level=(_the_group_level - '1')>
</$macro>

<*-------------------------------------*>
<$macro COLUMNGROUP /CLOSE NELEMS:num/R PAD:num BORDER:num CLASS:string>
	<$if COND=(NELEMS = '0')>
    <$message class=fatal text="NELEMS must not be zero!">
  </$if>

	<$if COND=(not (defined _the_group_level))>
		<$define _the_group_level:string='1'>
    <$define _the_group_nelems:num=(NELEMS)>
    <$define _the_group_counter:num=(NELEMS)>
	  <$define _the_group_columnwise:bool='1'>
	  <$define _the_group_newrow:bool='1'> <* force new row on 1st element *>
	  <$define _the_group_arrayprefix:string=''> <* only for nested ROWGROUPs *>
	  <$define _groupentry_celltype:string=''> <* used in GROUPENTRYs *>
	<$else>
		<$let _the_group_level=(_the_group_level & '1')>
    <("<$define _the_group_nelems" + _the_group_level + ":num=(_the_group_nelems)>")>
    <("<$define _the_group_counter" + _the_group_level + ":num=(_the_group_counter)>")>
	  <("<$define _the_group_columnwise" + _the_group_level + ":bool=(_the_group_columnwise)>")>
	  <("<$define _the_group_newrow" + _the_group_level + ":bool=(_the_group_newrow)>")>
	  <("<$define _groupentry_celltype" + _the_group_level + ":string=(_groupentry_celltype)>")>
    <$let _the_group_nelems=(NELEMS)>
    <$let _the_group_counter=(NELEMS)>
	  <$let _the_group_columnwise='1'>
	  <$let _the_group_newrow='1'>
	</$if>

  <TABLE CELLPADDING?=PAD BORDER?=BORDER SUMMARY="columngroup-layout"
         CLASS?=CLASS>
    <(HSC.Content +
      "<$if COND=(_the_group_counter <> _the_group_nelems)></TR></$if>")>
  </TABLE>

  <$if COND=(_the_group_level > '1')>
    <$let _the_group_nelems={"_the_group_nelems" + _the_group_level}>
    <$let _the_group_counter={"_the_group_counter" + _the_group_level}>
	  <$let _the_group_columnwise={"_the_group_columnwise" + _the_group_level}>
	  <$let _the_group_newrow={"_the_group_newrow" + _the_group_level}>
	  <$let _groupentry_celltype={"_groupentry_celltype" + _the_group_level}>
  </$if>
  <$let _the_group_level=(_the_group_level - '1')>
</$macro>

<*-------------------------------------*>
<$macro GROUPENTRY /CLOSE /MBI="rowgroup|columngroup" HEAD:bool>

  <* decide whether we want data cells or table headers *>
  <$if COND=(HEAD)>
    <$let _groupentry_celltype="TH">
  <$else>
    <$let _groupentry_celltype="TD">
  </$if>

  <* distinguish between row and column layouts *>
	<$if COND=(_the_group_columnwise)>

    <* if a new row has to be started, emit <TR> and reset flag *>
	  <$if COND=(_the_group_newrow)>
		  <TR><$let _the_group_newrow=''>
  	</$if>

    <* simply put the contents into a table cell or header *>
	  <_WRAP_STRING_IN_TAGS TAG=(_groupentry_celltype) TEXT=(Hsc.Content)>

    <* decrement element counter *>
		<$let _the_group_counter=(_the_group_counter - '1')>

    <* check for end of row *>
		<_COLUMNGROUP_CHECKENDROW>

	<$else>

    <* if the element counter has expired, just reset it *>
	  <$if COND=(_the_group_counter = '0')>
		  <$let _the_group_counter=(_the_group_nelems)>
  	</$if>

    <* decrement element counter *>
		<$let _the_group_counter=(_the_group_counter - '1')>

    <* add macro contents to current row's array variable *>
    <$let {_the_group_arrayprefix +
           ((_the_group_nelems - _the_group_counter) - '1')} =
          ({_the_group_arrayprefix +
           ((_the_group_nelems - _the_group_counter) - '1')} +
          "<" + _groupentry_celltype + ">" + HSC.Content +
          "<" + "/" + _groupentry_celltype + ">")>
	</$if>
</$macro>




<* This is highly experimental... *>
<$macro COLUMNGROUP-NG /CLOSE NELEMS:num/R CLASS:string>
	<$if COND=(NELEMS = '0')>
    <$message class=fatal text="NELEMS must not be zero!">
  </$if>

	<$if COND=(not (defined _the_group_level))>
		<$define _the_group_level:string='1'>
    <$define _the_group_nelems:num=(NELEMS)>
    <$define _the_group_counter:num=(NELEMS)>
	  <$define _the_group_columnwise:bool='1'>
	  <$define _the_group_newrow:bool='1'> <* force new row on 1st element *>
	  <$define _the_group_arrayprefix:string=''> <* only for nested ROWGROUPs *>
	<$else>
		<$let _the_group_level=(_the_group_level & '1')>
    <("<$define _the_group_nelems" + _the_group_level + ":num=(_the_group_nelems)>")>
    <("<$define _the_group_counter" + _the_group_level + ":num=(_the_group_counter)>")>
	  <("<$define _the_group_columnwise" + _the_group_level + ":bool=(_the_group_columnwise)>")>
	  <("<$define _the_group_newrow" + _the_group_level + ":bool=(_the_group_newrow)>")>
    <$let _the_group_nelems=(NELEMS)>
    <$let _the_group_counter='0'>
	  <$let _the_group_columnwise='1'>
	  <$let _the_group_newrow='1'>
	</$if>

  <_GROUPING-TABLE CLASS?=CLASS>
    <(HSC.Content + "<$if COND=(_the_group_counter <> _the_group_nelems)><_GROUPING-TABLE-ROW CLOSE></$if>")>
  <_GROUPING-TABLE CLOSE>

  <$if COND=(_the_group_level > '1')>
    <$let _the_group_nelems={"_the_group_nelems" + _the_group_level}>
    <$let _the_group_counter={"_the_group_counter" + _the_group_level}>
	  <$let _the_group_columnwise={"_the_group_columnwise" + _the_group_level}>
	  <$let _the_group_newrow={"_the_group_newrow" + _the_group_level}>
  </$if>
  <$let _the_group_level=(_the_group_level - '1')>
</$macro>

<$macro GROUPENTRY-NG /CLOSE /MBI="rowgroup-ng|columngroup-ng">
  <* distinguish between row and column layouts *>
	<$if COND=(_the_group_columnwise)>

    <* if a new row has to be started, emit <TR> and reset flag *>
	  <$if COND=(_the_group_newrow)>
		  <_GROUPING-TABLE-ROW><$let _the_group_newrow=''>
  	</$if>

    <* simply put the contents into a table cell or header *>
    <_GROUPING-TABLE-CELL>
	    <(Hsc.Content + "<$if COND=(_the_group_counter <> _the_group_nelems)><_GROUPING-TABLE-ROW CLOSE></$if>")>
    <_GROUPING-TABLE-CELL CLOSE>

    <* decrement element counter *>
		<$let _the_group_counter=(_the_group_counter - '1')>

    <* check for end of row *>
		<_COLUMNGROUP-NG_CHECKENDROW>

	<$else>

    <* if the element counter has expired, just reset it *>
	  <$if COND=(_the_group_counter = '0')>
		  <$let _the_group_counter=(_the_group_nelems)>
  	</$if>

    <* decrement element counter *>
		<$let _the_group_counter=(_the_group_counter - '1')>

    <* add macro contents to current row's array variable *>
    <$let {_the_group_arrayprefix +
           ((_the_group_nelems - _the_group_counter) - '1')} =
          ({_the_group_arrayprefix +
           ((_the_group_nelems - _the_group_counter) - '1')} +
          "<_GROUPING-TABLE-ROW>" + HSC.Content + "<_GROUPING-TABLE-ROW CLOSE>")>
	</$if>
</$macro>

<$macro _COLUMNGROUP-NG_CHECKENDROW>
  <* close row if element counter has reached zero *>
	<$if COND=(_the_group_counter = '0')>
		<_GROUPING-TABLE-ROW CLOSE>
    <* reset counter and set flag for new row on next groupentry *>
		<$let _the_group_counter=(_the_group_nelems)>
		<$let _the_group_newrow='1'>
	</$if>
</$macro>


<$macro _GROUPING-TABLE CLOSE:bool CLASS:string PAD:string BORDER:num>
  <$if COND=(set CLOSE)>
    <$if COND=(defined HSC.Opts.XHTML)>
      </DIV>
    <$else>
      </TABLE>
    </$if>
  <$else>
    <$if COND=(defined HSC.Opts.XHTML)>
      <$if COND=(set CLASS)><DIV CLASS=(CLASS)><$else><DIV STYLE="display:table"></$if>
    <$else>
      <TABLE CELLPADDING?=PAD BORDER?=BORDER SUMMARY="columngroup-layout" CLASS?=CLASS>
    </$if>
  </$if>
</$macro>

<$macro _GROUPING-TABLE-ROW CLOSE:bool CLASS:string>
  <$message class=warning text="New row">
  <$if COND=(set CLOSE)>
    <$if COND=(defined HSC.Opts.XHTML)></DIV><$else></TR></$if>
  <$else>
    <$if COND=(defined HSC.Opts.XHTML)>
      <$if COND=(set CLASS)><DIV CLASS=(CLASS)><$else><DIV STYLE="display:table-row"></$if>
    <$else>
      <TR CLASS?=CLASS>
    </$if>
  </$if>
</$macro>

<$macro _GROUPING-TABLE-CELL CLOSE:bool CLASS:string>
  <$if COND=(set CLOSE)>
    <$if COND=(defined HSC.Opts.XHTML)></DIV><$else></TD></$if>
  <$else>
    <$if COND=(defined HSC.Opts.XHTML)>
      <$if COND=(set CLASS)><DIV CLASS=(CLASS)><$else><DIV STYLE="display:table-cell"></$if>
    <$else>
      <TD>
    </$if>
  </$if>
</$macro>

<*-------------------------------------*>

<$macro ROWGROUP-NEWCOLUMN /MBI="rowgroup" HEAD:bool>
  <$if COND=(_the_group_counter)>
    <$if COND=(HEAD)>
      <$define _newcolumn_fillcell:string/C="<TH></TH>">
    <$else>
      <$define _newcolumn_fillcell:string/C="<TD></TD>">
    </$if>
    <FOR VAR=i START=0 TO=(_the_group_counter) STEP=1>
      <$let {_the_group_arrayprefix + ((_the_group_nelems - i) - '1')} =
            ({_the_group_arrayprefix + ((_the_group_nelems - i) - '1')} +
             _newcolumn_fillcell)>
    </FOR>
    <$let _the_group_counter=0>
  </$if>
</$macro>

<*-------------------------------------*>

<$macro COLUMNGROUP-NEWROW /MBI="columngroup" CELLS:bool>
  <$if COND=(HEAD)>
    <$define _newrow_fillcell:string/C="<TH>&nbsp;</TH>">
  <$else>
    <$define _newrow_fillcell:string/C="<TD>&nbsp;</TD>">
  </$if>
  <$if COND=(set CELLS)>
    <WHILE COND='_the_group_counter > "0"'>
      <$let _the_group_counter=(_the_group_counter - '1')>
      <(_newrow_fillcell)>
    </WHILE>
  <$else>
    <$let _the_group_counter='0'>
  </$if>
  <_COLUMNGROUP_CHECKENDROW>
</$macro>

<*-------------------------------------*>
<$macro _COLUMNGROUP_CHECKENDROW>
  <* close row if element counter has reached zero *>
	<$if COND=(_the_group_counter = '0')>
		</TR>
    <* reset counter and set flag for new row on next groupentry *>
		<$let _the_group_counter=(_the_group_nelems)>
		<$let _the_group_newrow='1'>
	</$if>
</$macro>

<*-------------------------------------*>
<* This adds the string "<NAME>:<ATTR>" to DEST, possibly separated with a
 * semicolon if DEST was defined before. If ATTR is undefined, does nothing,
 * so it is safe to use it like this:
 * <$macro FOO SOME:string PARAMETER:num>
 *   <$define styleattr:string>
 *   <_MAKE-CSS-STYLE ATTR?=SOME NAME=some DEST=styleattr>
 *   <_MAKE-CSS-STYLE ATTR?=PARAMETER NAME=other DEST=styleattr>
 * </$macro>
 *>
<$macro _MAKE-CSS-STYLE ATTR:string NAME:string/R DEST:string/R>
  <$if COND=(set ATTR)>
    <$if COND=(set {DEST})>
      <$let {DEST}=({DEST} + ';' + NAME + ':' + ATTR)>
    <$else>
      <$let {DEST}=(NAME + ':' + ATTR)>
    </$if>
  </$if>
</$macro>

<*-----------------------------------------------------------------------*>
<*
 * $Log: macros.hsc,v $
 * Revision 1.82  2006/03/15 14:36:03  mb
 * Removed some error message shortcuts and log info
 * Added version symbol
 *
 * Revision 1.81  2006/03/15 00:10:33  mb
 * Fixed erroneous expansion of submenus under captions.
 * This still needs some optimizing.
 *
 * Revision 1.80  2006/02/28 11:10:39  mb
 * Fixed dependency issues with SUBMENU's FILE attribute
 * LAST_CHANGE split into LAST_CHANGE and LAST_CHANGE_PAR
 *
 * Revision 1.79  2005/09/11 16:39:06  mb
 * Renamed HEADER's PROJECT_STYLEFILE attr to PROJECT_STYLESHEET
 *
 * Revision 1.78  2005/09/11 16:09:42  mb
 * FOOTNOTES' BACKLINK attribute is now a string to allow for localized "back" text.
 * Fixed macro redefinition warning on multiple FOOTNOTES expansion
 *
 * Revision 1.77  2005/08/18 00:13:39  mb
 * Split FOOTNOTES (->FOOTNOTESTITLED); small fix for section numbering BIAS;
 * Added ITEMHLID to MENUITEM; added FILE to SUBMENU; added some documentation bits
 *>

<* vi: set tabstop=2 expandtab nowrap: *>
