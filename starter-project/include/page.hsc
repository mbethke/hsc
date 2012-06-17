<****** page ****************************************************************
* NAME
*   page -- Macro to insert the dull html stuff for a page.
* SYNOPSIS
*   <$macro page section:string heading:string/required>
* FUNCTION
*   This macro inserts the dull <head> and <body> stuff and computes a
*   document title consisting of a optional section and a required heading.
* INPUTS
*   section - Optional name of the section of your site (e.g. "Support"),
*       used in title but not in heading. If this is omitted, the title is
*       equal to the heading.
*   heading - Heading displayed at top of document; also used for title
****************************************************************************>
<$macro page /close
    section:string
    heading:string/required
>
<* Figure out the title (usually displayed in the browser window title) *>
<$define title:string="">
<$if cond=(set section)>
   <$let title=(section + " - ")>
</$if>
<$let title=(title + heading)>

<* Insert head *>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<html><head>
<title><(title)></title>
</head>

<* Insert body including macro contents *>
<body>
<h1><(heading)></h1>
<$content>
</body></html>

</$macro>
