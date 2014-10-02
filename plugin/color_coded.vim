" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
if v:version < 704 || !exists("*matchaddpos")
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires Vim 7.4p330+" |
        \ echohl None
  finish
endif
if !has('ruby')
  echohl WarningMsg |
        \ echomsg "color_coded unavailable: requires ruby" |
        \ echohl None
  finish
endif
if exists("g:loaded_color_coded") || &cp
  finish
endif
" ------------------------------------------------------------------------------

let g:loaded_color_coded = 1
let s:keepcpo = &cpo
set cpo&vim
" ------------------------------------------------------------------------------

if !exists("g:color_coded_filetypes")
  let g:color_coded_filetypes = ['c', 'h', 'cpp', 'hpp', 'cc', 'm', 'mm']
endif

let s:path = expand('<sfile>:p:h')
ruby << EOF
  require VIM::evaluate('s:path') + '/../bin/color_coded.so'
EOF

function! s:color_coded_push()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  line_count = VIM::Buffer.current.count
  data = ''
  for i in 1..line_count do
    data += VIM::Buffer.current[i] + "\n"
  end
  name = VIM::Buffer.current.name
  name = VIM::Buffer.current.number.to_s if name.nil?
  color_coded_push(name, data.nil? ? "" : data)
EOF
endfunction!

function! s:color_coded_pull()
  if index(g:color_coded_filetypes, &ft) < 0
    return
  endif
ruby << EOF
  color_coded_pull
EOF
endfunction!

au BufEnter * call s:color_coded_push()
au VimEnter * call s:color_coded_push()
au TextChanged * call s:color_coded_push()
au TextChangedI * call s:color_coded_push()
au CursorMoved * call s:color_coded_pull()
au CursorMovedI * call s:color_coded_pull()

hi Member ctermfg=Cyan guifg=Cyan
hi Variable ctermfg=Grey guifg=Grey
hi Namespace ctermfg=DarkYellow guifg=DarkYellow
hi EnumConstant ctermfg=LightGreen guifg=LightGreen

hi link StructDecl Type
hi link UnionDecl Type
hi link ClassDecl Type
hi link EnumDecl Type
hi link FieldDecl Member
hi link EnumConstantDecl EnumConstant
hi link FunctionDecl Function
hi link VarDecl Variable
hi link ParmDecl Variable
hi link ObjCInterfaceDecl Normal
hi link ObjCCategoryDecl Normal
hi link ObjCProtocolDecl Normal
hi link ObjCPropertyDecl Normal
hi link ObjCIvarDecl Normal
hi link ObjCInstanceMethodDecl Normal
hi link ObjCClassMethodDecl Normal
hi link ObjCImplementationDecl Normal
hi link ObjCCategoryImplDecl Normal
hi link TypedefDecl Type
hi link CXXMethod Function
hi link Namespace Namespace
hi link LinkageSpec Normal
hi link Constructor Function
hi link Destructor Function
hi link ConversionFunction Function
hi link TemplateTypeParameter Type
hi link NonTypeTemplateParameter Normal
hi link TemplateTemplateParameter Type
hi link FunctionTemplate Function
hi link ClassTemplate Type
hi link ClassTemplatePartialSpecialization Type
hi link NamespaceAlias Namespace
hi link UsingDirective Type
hi link UsingDeclaration Type
hi link TypeAliasDecl Type
hi link ObjCSynthesizeDecl Normal
hi link ObjCDynamicDecl Normal
hi link CXXAccessSpecifier Label
hi link ObjCSuperClassRef Normal
hi link ObjCProtocolRef Normal
hi link ObjCClassRef Normal
hi link TypeRef Type
hi link CXXBaseSpecifier Type
hi link NamespaceRef Namespace
hi link MemberRef Member
hi link LabelRef Label
hi link OverloadedDeclRef Function
hi link VariableRef Variable
hi link FirstInvalid Normal
hi link NoDeclFound Error
hi link NotImplemented Normal
hi link InvalidCode Error
hi link FirstExpr Normal
hi link DeclRefExpr Variable
hi link MemberRefExpr Member
hi link CallExpr Function
hi link ObjCMessageExpr Normal
hi link BlockExpr Normal
hi link MacroDefinition Macro
hi link MacroInstantiation Macro
hi link IntegerLiteral Number
hi link FloatingLiteral Float
hi link ImaginaryLiteral Number
hi link StringLiteral String
hi link CharacterLiteral Character
hi link Punctuation Normal

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
