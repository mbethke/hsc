" Vim syntax file
" Language:	HTML with HSC
" Maintainer:	Matthias Bethke <matthias.bethke@gmx.net>
" URL: 
" Last Change:	2003 Apr 10

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Same as for HTML
if version < 508
  command! -nargs=+ HscHiLink hi link <args>
else
  command! -nargs=+ HscHiLink hi def link <args>
endif

" we define it here so that included files can test for it
if !exists("main_syntax")
  let main_syntax='htmlhsc'
endif

if version < 600
  so <sfile>:p:h/html.vim
else
  runtime! syntax/html.vim
endif
unlet b:current_syntax

syntax case ignore
syntax sync minlines=150 maxlines=500
syn cluster htmlPreproc add=hscTop
setlocal iskeyword+=$,-

"-------------------------------------------------------------------------------
" Additional comments that will be stripped by HSC
syn region  hscComment start="<\*" end="\*>" contains=hscComment

" Verbatim data
syn region  hscVerbatimR matchgroup=hscVerbatim start="<|" end="|>"

" HSC-insert-expression
syn region  hscInsExprR matchgroup=hscInsExpr start="<(" end=")>" contains=hscString,hscExpr,hscOperator skipwhite skipempty

" Single- and double-quoted strings
syn match  hscString contained /"[^"]*"/
syn match  hscString contained /'[^']*'/

" Unary and binary operators
syn keyword hscOperator contained basename chr defined exists extension fexists
syn keyword hscOperator contained getenv getfilesize getgmtime gettime not ord
syn keyword hscOperator contained set urikind
syn match   hscOperator contained "[-_&+*/<>]"
syn match   hscOperator contained " mod \|<=\|>=\|==\| lt \| gt \| le \| ge \| and \| or \| xor "

" A complete HSC expression
syn region  hscExpr start="(" end=")" contains=hscString,hscExpr,hscOperator skipwhite skipempty
syn region  hscExpr start="{" end="}" contains=hscString,hscExpr,hscOperator skipwhite skipempty

" Attributes
syn match   hscAttr     contained "\w\([-.]\|\w\)*" nextgroup=hscCondEq,hscEq skipwhite
syn match   hscEq       contained "="          nextgroup=hscString,hscExpr,hscAttr skipwhite
syn match   hscCondEq   contained "?="         nextgroup=hscAttr skipwhite

" Normal HSC tags starting with $
syn region  hscNormalTagR matchgroup=hscNormalTag start="<\$\(content\|defent\|deficon\|defstyle\|depend\|elseif\|else\|exec\|export\|if\|include\|let\|message\|source\|stripws\)" end=">" contains=hscAttr

" Attribute declarations with types and modifiers
syn match hscName contained "\w\([-.]\|\w\)*"
syn match hscAttrDecl contained #\w\([-.]\|\w\)*:\a\+\s*\(/\a\+\s*\)*\s*=\?# nextgroup=hscString,hscExpr contains=hscAttrD,hscModifier,hscEq
syn match   hscAttrD    contained "\w\([-.]\|\w\)*:\a\+" contains=hscAttr,hscType
syn match   hscType     contained ":\a\+" nextgroup=hscModifier,hscEq
syn match   hscModifier contained "/\a\+" contains=hscModifierSlash
syn match   hscModifierSlash contained "/"

" The <$define> tag
syn region hscDefineR matchgroup=hscDefineTag start="<$define\>" end=">" contains=hscAttrDecl

" The <$macro>, <$deftag> and <$varlist> tags
syn region hscMacroTagR matchgroup=hscMacroTag start="<$\(macro\|deftag\|varlist\)\>" end=">" contains=hscMacroTag,hscAttrDecl
syn match hscMacroTag contained #\w\([-.]\|\w\)*\s*\(/\a\+\s*\)*# contains=hscName,hscModifier,hscAttrDecl

" The only closing tags
syn match hscEndTag "</$\(if\|macro\|source\)>"

syn region hscIfBody start="<$if\>" end="</$if>" contains=TOP,hscElse,hscElseif
syn region hscIfR matchgroup=hscIf start="<$if\>" end=">" contains=hscAttr
syn match hscElse contained "<$else>"
syn match hscElseif contained "<$elseif>"

" All tags together
syn cluster hscTag contains=hscNormalTagR,hscMacroTagR,hscDefineR,hscInsExprR

" Top Cluster
syn cluster hscTop contains=hscComment,hscVerbatim,@hscTag,hscMacro,hscIfBody

" Highlighting
hi def hscTag term=bold cterm=bold

HscHiLink hscComment Comment
HscHiLink hscVerbatim hscTag
hi link hscNormalTag hscTag
hi link hscMacroTag hscTag
hi link hscDefineTag hscTag
hi link hscInsExpr hscTag
hi link hscEndTag hscTag
hi link hscIf hscTag
hi link hscElseif hscTag
hi link hscElse hscTag
hi link hscOperator Operator
hi link hscAttr Identifier
hi link hscName Identifier
hi link hscType Type
hi link hscModifier Type
hi link hscModifierSlash None
hi link hscString htmlString

let b:current_syntax = "htmlhsc"

if main_syntax == 'htmlhsc'
  unlet main_syntax
endif
"
" $Id: htmlhsc.vim,v 1.3 2003/04/28 22:14:33 mb Exp mb $ 
