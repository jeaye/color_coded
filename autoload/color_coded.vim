" Vim global plugin for semantic highlighting using libclang
" Maintainer:	Jeaye <contact@jeaye.com>

" ------------------------------------------------------------------------------
function! s:setup_highlighting()
  hi Member ctermfg=Cyan guifg=Cyan
  hi Variable ctermfg=Grey guifg=Grey
  hi Namespace ctermfg=DarkYellow guifg=DarkYellow
  hi EnumConstant ctermfg=LightGreen guifg=LightGreen

  hi link StructDecl Type
  hi link UnionDecl Type
  hi link ClassDecl Type
  hi link EnumDecl Type
  hi link TypedefDecl Type
  hi link TemplateTypeParameter Type
  hi link TemplateTemplateParameter Type
  hi link ClassTemplate Type
  hi link ClassTemplatePartialSpecialization Type
  hi link UsingDirective Type
  hi link UsingDeclaration Type
  hi link TypeAliasDecl Type
  hi link TypeRef Type
  hi link CXXBaseSpecifier Type

  hi link FieldDecl Member
  hi link MemberRef Member
  hi link MemberRefExpr Member

  hi link VarDecl Variable
  hi link ParmDecl Variable
  hi link VariableRef Variable
  hi link DeclRefExpr Variable
  hi link NonTypeTemplateParameter Variable

  hi link Namespace Namespace
  hi link NamespaceAlias Namespace
  hi link NamespaceRef Namespace

  hi link EnumConstantDecl EnumConstant
  hi link FunctionDecl Function
  hi link CXXMethod Function
  hi link Constructor Function
  hi link Destructor Function
  hi link ConversionFunction Function
  hi link FunctionTemplate Function
  hi link OverloadedDeclRef Function
  hi link CallExpr Function

  hi link LabelRef Label
  hi link CXXAccessSpecifier Label

  hi link MacroDefinition Macro
  hi link MacroInstantiation Macro

  hi link IntegerLiteral Number
  hi link FloatingLiteral Float
  hi link ImaginaryLiteral Number

  hi link StringLiteral String
  hi link CharacterLiteral Character

  hi link LinkageSpec Normal
  hi link FirstExpr Normal
  hi link BlockExpr Normal
  hi link Punctuation Normal

  hi link NoDeclFound Error
  hi link InvalidCode Error
  hi link FirstInvalid Error
  hi link NotImplemented Error

  " TODO: ObjectiveC
  hi link ObjCInterfaceDecl Normal
  hi link ObjCCategoryDecl Normal
  hi link ObjCProtocolDecl Normal
  hi link ObjCPropertyDecl Normal
  hi link ObjCIvarDecl Normal
  hi link ObjCInstanceMethodDecl Normal
  hi link ObjCClassMethodDecl Normal
  hi link ObjCImplementationDecl Normal
  hi link ObjCCategoryImplDecl Normal
  hi link ObjCSynthesizeDecl Normal
  hi link ObjCDynamicDecl Normal
  hi link ObjCSuperClassRef Normal
  hi link ObjCProtocolRef Normal
  hi link ObjCClassRef Normal
  hi link ObjCMessageExpr Normal
endfunction!
" ------------------------------------------------------------------------------
