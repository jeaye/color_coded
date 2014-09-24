" Vim global plugin for semantic highlighting using libclang
" Last Change: Sept. 2014
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

ruby require './bin/color_coded.so'
set ft=cpp

function! s:try_work()
ruby << EOF
  line_count = VIM::Buffer.current.count
  data = ''
  for i in 1..line_count do
    data += VIM::Buffer.current[i] + "\n"
  end
  cc_work(data)
  #cc_work(VIM::Buffer.current.name)
EOF
endfunction!

au TextChanged * call s:try_work()
au TextChangedI * call s:try_work()
au CursorMoved * call s:try_work()

hi link StructDecl Type
hi link UnionDecl Type
hi link ClassDecl Type
hi link EnumDecl Type
hi link FieldDecl Comment
hi link EnumConstantDecl Type
hi link FunctionDecl Type
hi link VarDecl Type
hi link ParmDecl Type
hi link ObjCInterfaceDecl Type
hi link ObjCCategoryDecl Type
hi link ObjCProtocolDecl Type
hi link ObjCPropertyDecl Type
hi link ObjCIvarDecl Type
hi link ObjCInstanceMethodDecl Type
hi link ObjCClassMethodDecl Type
hi link ObjCImplementationDecl Type
hi link ObjCCategoryImplDecl Type
hi link TypedefDecl Type
hi link CXXMethod Comment
hi link Namespace Type
hi link LinkageSpec Type
hi link Constructor Type
hi link Destructor Type
hi link ConversionFunction Type
hi link TemplateTypeParameter Type
hi link NonTypeTemplateParameter Type
hi link TemplateTemplateParameter Type
hi link FunctionTemplate Type
hi link ClassTemplate Type
hi link ClassTemplatePartialSpecialization Type
hi link NamespaceAlias Type
hi link UsingDirective Type
hi link UsingDeclaration Type
hi link TypeAliasDecl Type
hi link ObjCSynthesizeDecl Type
hi link ObjCDynamicDecl Type
hi link CXXAccessSpecifier Type
hi link ObjCSuperClassRef Type
hi link ObjCProtocolRef Type
hi link ObjCClassRef Type
hi link TypeRef Type
hi link CXXBaseSpecifier Type
hi link NamespaceRef Type
hi link MemberRef Comment
hi link LabelRef Type
hi link OverloadedDeclRef Type
hi link VariableRef Type
hi link FirstInvalid Type
hi link NoDeclFound Type
hi link NotImplemented Type
hi link InvalidCode Type
hi link FirstExpr Type
hi link DeclRefExpr Label
hi link MemberRefExpr Comment
hi link CallExpr Type
hi link ObjCMessageExpr Type
hi link BlockExpr Type
hi link MacroDefinition Constant
hi link MacroInstantiation Constant
hi link IntegerLiteral Number
hi link FloatingLiteral Float
hi link ImaginaryLiteral Number
hi link StringLiteral String
hi link CharacterLiteral Character
hi link Punctuation Normal

" ------------------------------------------------------------------------------
let &cpo = s:keepcpo
unlet s:keepcpo
