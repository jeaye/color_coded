" Vim global plugin for semantic highlighting using libclang
" Maintainer: Jeaye <contact@jeaye.com>

hi default Member ctermfg=Cyan guifg=Cyan
hi default Variable ctermfg=Grey guifg=Grey
hi default Namespace ctermfg=Yellow guifg=#BBBB00
hi default EnumConstant ctermfg=LightGreen guifg=LightGreen

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
hi link NonTypeTemplateParameter Variable
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
hi link TemplateRef Type
hi link NamespaceRef Namespace
hi link MemberRef Member
hi link LabelRef Label
hi link OverloadedDeclRef Function
hi link VariableRef Variable
hi link FirstInvalid Normal
hi link InvalidFile Error
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
