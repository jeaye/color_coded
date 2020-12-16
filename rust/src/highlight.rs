#[derive(Debug)]
pub struct Highlight {
  pub r#type: &'static str,
  /* All of these are zero-based. */
  pub line: usize,
  pub column: usize,
  pub size: usize,
}

#[derive(Debug)]
pub struct Group {
  pub highlights: Vec<Highlight>,
}

impl Group {
  pub fn new(tokens: Vec<crate::clang::Token>) -> Self {
    Self {
      highlights: tokens
        .iter()
        .filter_map(|tok| match Self::map_token_kind(&tok) {
          "" => None,
          mapped => Some(Highlight {
            r#type: mapped,
            line: tok.line - 1,
            column: tok.column - 1,
            size: tok.size,
          }),
        })
        .collect(),
    }
  }

  fn map_token_kind(token: &crate::clang::Token) -> &'static str {
    match token.token_kind {
      clang::token::TokenKind::Identifier => Self::map_cursor_kind(&token),
      clang::token::TokenKind::Literal => Self::map_literal_kind(&token),

      /* Allow Vim to handle these. */
      clang::token::TokenKind::Comment
      | clang::token::TokenKind::Keyword
      | clang::token::TokenKind::Punctuation => "",
    }
  }

  fn map_cursor_typekind(token: &crate::clang::Token) -> &'static str {
    if token.cursor_typekind.is_none() {
      return "";
    }

    match token.cursor_typekind.unwrap() {
      /********* Declarations **********/
      clang::TypeKind::Unexposed
      | clang::TypeKind::Void
      | clang::TypeKind::Bool
      | clang::TypeKind::CharS
      | clang::TypeKind::CharU
      | clang::TypeKind::SChar
      | clang::TypeKind::UChar
      | clang::TypeKind::WChar
      | clang::TypeKind::Char16
      | clang::TypeKind::Char32
      | clang::TypeKind::Short
      | clang::TypeKind::UShort
      | clang::TypeKind::Int
      | clang::TypeKind::UInt
      | clang::TypeKind::Long
      | clang::TypeKind::ULong
      | clang::TypeKind::LongLong
      | clang::TypeKind::ULongLong
      | clang::TypeKind::Int128
      | clang::TypeKind::UInt128
      | clang::TypeKind::Half
      | clang::TypeKind::Float16
      | clang::TypeKind::ShortAccum
      | clang::TypeKind::Accum
      | clang::TypeKind::LongAccum
      | clang::TypeKind::UShortAccum
      | clang::TypeKind::UAccum
      | clang::TypeKind::ULongAccum
      | clang::TypeKind::Float
      | clang::TypeKind::Double
      | clang::TypeKind::LongDouble
      | clang::TypeKind::Nullptr
      | clang::TypeKind::Complex
      | clang::TypeKind::Dependent
      | clang::TypeKind::Overload
      | clang::TypeKind::ObjCId
      | clang::TypeKind::ObjCClass
      | clang::TypeKind::ObjCSel
      | clang::TypeKind::Float128
      | clang::TypeKind::ObjCInterface
      | clang::TypeKind::ObjCObjectPointer
      | clang::TypeKind::Pointer
      | clang::TypeKind::BlockPointer
      | clang::TypeKind::LValueReference
      | clang::TypeKind::RValueReference
      | clang::TypeKind::Auto
      | clang::TypeKind::Record
      | clang::TypeKind::Typedef
      | clang::TypeKind::ConstantArray
      | clang::TypeKind::DependentSizedArray
      | clang::TypeKind::IncompleteArray
      | clang::TypeKind::VariableArray
      | clang::TypeKind::Vector
      | clang::TypeKind::Elaborated
      | clang::TypeKind::Pipe
      | clang::TypeKind::OCLImage1dRO
      | clang::TypeKind::OCLImage1dArrayRO
      | clang::TypeKind::OCLImage1dBufferRO
      | clang::TypeKind::OCLImage2dRO
      | clang::TypeKind::OCLImage2dArrayRO
      | clang::TypeKind::OCLImage2dDepthRO
      | clang::TypeKind::OCLImage2dArrayDepthRO
      | clang::TypeKind::OCLImage2dMSAARO
      | clang::TypeKind::OCLImage2dArrayMSAARO
      | clang::TypeKind::OCLImage2dMSAADepthRO
      | clang::TypeKind::OCLImage2dArrayMSAADepthRO
      | clang::TypeKind::OCLImage3dRO
      | clang::TypeKind::OCLImage1dWO
      | clang::TypeKind::OCLImage1dArrayWO
      | clang::TypeKind::OCLImage1dBufferWO
      | clang::TypeKind::OCLImage2dWO
      | clang::TypeKind::OCLImage2dArrayWO
      | clang::TypeKind::OCLImage2dDepthWO
      | clang::TypeKind::OCLImage2dArrayDepthWO
      | clang::TypeKind::OCLImage2dMSAAWO
      | clang::TypeKind::OCLImage2dArrayMSAAWO
      | clang::TypeKind::OCLImage2dMSAADepthWO
      | clang::TypeKind::OCLImage2dArrayMSAADepthWO
      | clang::TypeKind::OCLImage3dWO
      | clang::TypeKind::OCLImage1dRW
      | clang::TypeKind::OCLImage1dArrayRW
      | clang::TypeKind::OCLImage1dBufferRW
      | clang::TypeKind::OCLImage2dRW
      | clang::TypeKind::OCLImage2dArrayRW
      | clang::TypeKind::OCLImage2dDepthRW
      | clang::TypeKind::OCLImage2dArrayDepthRW
      | clang::TypeKind::OCLImage2dMSAARW
      | clang::TypeKind::OCLImage2dArrayMSAARW
      | clang::TypeKind::OCLImage2dMSAADepthRW
      | clang::TypeKind::OCLImage2dArrayMSAADepthRW
      | clang::TypeKind::OCLImage3dRW
      | clang::TypeKind::OCLSampler
      | clang::TypeKind::OCLEvent
      | clang::TypeKind::OCLQueue
      | clang::TypeKind::OCLReserveID
      | clang::TypeKind::ObjCObject
      | clang::TypeKind::ObjCTypeParam
      | clang::TypeKind::Attributed
      | clang::TypeKind::OCLIntelSubgroupAVCMcePayload
      | clang::TypeKind::OCLIntelSubgroupAVCImePayload
      | clang::TypeKind::OCLIntelSubgroupAVCRefPayload
      | clang::TypeKind::OCLIntelSubgroupAVCSicPayload
      | clang::TypeKind::OCLIntelSubgroupAVCMceResult
      | clang::TypeKind::OCLIntelSubgroupAVCImeResult
      | clang::TypeKind::OCLIntelSubgroupAVCRefResult
      | clang::TypeKind::OCLIntelSubgroupAVCSicResult
      | clang::TypeKind::OCLIntelSubgroupAVCImeResultSingleRefStreamout
      | clang::TypeKind::OCLIntelSubgroupAVCImeResultDualRefStreamout
      | clang::TypeKind::OCLIntelSubgroupAVCImeSingleRefStreamin
      | clang::TypeKind::OCLIntelSubgroupAVCImeDualRefStreamin
      | clang::TypeKind::ExtVector => "Variable",

      clang::TypeKind::MemberPointer => "Member",

      clang::TypeKind::Enum => "EnumConstant",

      clang::TypeKind::FunctionPrototype | clang::TypeKind::FunctionNoPrototype => "Function",
    }
  }

  fn map_literal_kind(token: &crate::clang::Token) -> &'static str {
    match token.cursor_kind {
      clang::EntityKind::IntegerLiteral | clang::EntityKind::ImaginaryLiteral => "Number",
      clang::EntityKind::FloatingLiteral | clang::EntityKind::FixedPointLiteral => "Float",
      clang::EntityKind::CharacterLiteral => "Character",

      clang::EntityKind::NullPtrLiteralExpr
      | clang::EntityKind::CompoundLiteralExpr
      | clang::EntityKind::StringLiteral
      | clang::EntityKind::ObjCStringLiteral
      | clang::EntityKind::ObjCBoolLiteralExpr
      | clang::EntityKind::BoolLiteralExpr
      | _ => "",
    }
  }

  /* TODO: Update syntax file to include all of these. */
  fn map_cursor_kind(token: &crate::clang::Token) -> &'static str {
    match token.cursor_kind {
      /********* Declarations **********/
      clang::EntityKind::UnexposedDecl => "", /* Unknown declaration. */
      clang::EntityKind::StructDecl => "StructDecl",
      clang::EntityKind::UnionDecl => "UnionDecl",
      clang::EntityKind::ClassDecl => "ClassDecl",
      clang::EntityKind::EnumDecl => "EnumDecl",
      clang::EntityKind::FieldDecl => "FieldDecl",
      clang::EntityKind::EnumConstantDecl => "EnumConstantDecl",
      clang::EntityKind::FunctionDecl => "FunctionDecl",
      clang::EntityKind::VarDecl => "VarDecl",
      clang::EntityKind::ParmDecl => "ParmDecl",
      clang::EntityKind::ObjCInterfaceDecl => "ObjCInterfaceDecl",
      clang::EntityKind::ObjCCategoryDecl => "ObjCCategoryDecl",
      clang::EntityKind::ObjCProtocolDecl => "ObjCProtocolDecl",
      clang::EntityKind::ObjCPropertyDecl => "ObjCPropertyDecl",
      clang::EntityKind::ObjCIvarDecl => "ObjCIvarDecl",
      clang::EntityKind::ObjCInstanceMethodDecl => "ObjCInstanceMethodDecl",
      clang::EntityKind::ObjCClassMethodDecl => "ObjCClassMethodDecl",
      clang::EntityKind::ObjCImplementationDecl => "ObjCImplementationDecl",
      clang::EntityKind::ObjCCategoryImplDecl => "ObjCCategoryImplDecl",
      clang::EntityKind::TypedefDecl => "TypedefDecl",
      clang::EntityKind::Method => "Method",
      clang::EntityKind::Namespace => "Namespace",
      clang::EntityKind::LinkageSpec => "LinkageSpec",
      clang::EntityKind::Constructor => "Constructor",
      clang::EntityKind::Destructor => "Destructor",
      clang::EntityKind::ConversionFunction => "ConversionFunction",
      clang::EntityKind::TemplateTypeParameter => "TemplateTypeParameter",
      clang::EntityKind::NonTypeTemplateParameter => "NonTypeTemplateParameter",
      clang::EntityKind::TemplateTemplateParameter => "TemplateTemplateParameter",
      clang::EntityKind::FunctionTemplate => "FunctionTemplate",
      clang::EntityKind::ClassTemplate => "ClassTemplate",
      clang::EntityKind::ClassTemplatePartialSpecialization => "ClassTemplatePartialSpecialization",
      clang::EntityKind::NamespaceAlias => "NamespaceAlias",
      clang::EntityKind::UsingDirective => "UsingDirective",
      clang::EntityKind::UsingDeclaration => "UsingDeclaration",
      clang::EntityKind::TypeAliasDecl => "TypeAliasDecl",
      clang::EntityKind::ObjCSynthesizeDecl => "ObjCSynthesizeDecl",
      clang::EntityKind::ObjCDynamicDecl => "ObjCDynamicDecl",
      clang::EntityKind::AccessSpecifier => "AccessSpecifier",

      /********* References **********/
      clang::EntityKind::ObjCSuperClassRef => "ObjCSuperClassRef",
      clang::EntityKind::ObjCProtocolRef => "ObjCProtocolRef",
      clang::EntityKind::ObjCClassRef => "ObjCClassRef",
      clang::EntityKind::TypeRef => "TypeRef",
      clang::EntityKind::BaseSpecifier => "BaseSpecifier",
      clang::EntityKind::TemplateRef => "TemplateRef",
      clang::EntityKind::NamespaceRef => "NamespaceRef",
      clang::EntityKind::MemberRef => "MemberRef",
      clang::EntityKind::LabelRef => "LabelRef",
      clang::EntityKind::OverloadedDeclRef => "OverloadedDeclRef",
      clang::EntityKind::VariableRef => "VariableRef",

      /********* Errors **********/
      clang::EntityKind::InvalidFile
      | clang::EntityKind::InvalidDecl
      | clang::EntityKind::NotImplemented
      | clang::EntityKind::InvalidCode => "",

      /********* Expressions **********/
      clang::EntityKind::UnexposedExpr => "",
      clang::EntityKind::DeclRefExpr => Self::map_cursor_typekind(&token),
      clang::EntityKind::MemberRefExpr => "MemberRefExpr",
      clang::EntityKind::CallExpr => "CallExpr",
      clang::EntityKind::ObjCMessageExpr => "ObjCMessageExpr",
      clang::EntityKind::BlockExpr => "BlockExpr",
      clang::EntityKind::IntegerLiteral => "IntegerLiteral",
      clang::EntityKind::FloatingLiteral => "FloatingLiteral",
      clang::EntityKind::ImaginaryLiteral => "ImaginaryLiteral",
      clang::EntityKind::StringLiteral => "StringLiteral",
      clang::EntityKind::CharacterLiteral => "CharacterLiteral",
      clang::EntityKind::ParenExpr => "ParenExpr",
      clang::EntityKind::UnaryOperator => "UnaryOperator",
      clang::EntityKind::ArraySubscriptExpr => "ArraySubscriptExpr",
      clang::EntityKind::BinaryOperator => "BinaryOperator",
      clang::EntityKind::CompoundAssignOperator => "CompoundAssignOperator",
      clang::EntityKind::ConditionalOperator => "ConditionalOperator",
      clang::EntityKind::CStyleCastExpr => "CStyleCastExpr",
      clang::EntityKind::CompoundLiteralExpr => "CompoundLiteralExpr",
      clang::EntityKind::InitListExpr => "InitListExpr",
      clang::EntityKind::AddrLabelExpr => "AddrLabelExpr",
      clang::EntityKind::StmtExpr => "StmtExpr",
      clang::EntityKind::GenericSelectionExpr => "GenericSelectionExpr",
      clang::EntityKind::GNUNullExpr => "GNUNullExpr",
      clang::EntityKind::StaticCastExpr => "StaticCastExpr",
      clang::EntityKind::DynamicCastExpr => "DynamicCastExpr",
      clang::EntityKind::ReinterpretCastExpr => "ReinterpretCastExpr",
      clang::EntityKind::ConstCastExpr => "ConstCastExpr",
      clang::EntityKind::FunctionalCastExpr => "FunctionalCastExpr",
      clang::EntityKind::TypeidExpr => "TypeidExpr",
      clang::EntityKind::BoolLiteralExpr => "BoolLiteralExpr",
      clang::EntityKind::NullPtrLiteralExpr => "NullPtrLiteralExpr",
      clang::EntityKind::ThisExpr => "ThisExpr",
      clang::EntityKind::ThrowExpr => "ThrowExpr",
      clang::EntityKind::NewExpr => "NewExpr",
      clang::EntityKind::DeleteExpr => "DeleteExpr",
      clang::EntityKind::UnaryExpr => "UnaryExpr",
      clang::EntityKind::ObjCStringLiteral => "ObjCStringLiteral",
      clang::EntityKind::ObjCEncodeExpr => "ObjCEncodeExpr",
      clang::EntityKind::ObjCSelectorExpr => "ObjCSelectorExpr",
      clang::EntityKind::ObjCProtocolExpr => "ObjCProtocolExpr",
      clang::EntityKind::ObjCBridgedCastExpr => "ObjCBridgedCastExpr",
      clang::EntityKind::PackExpansionExpr => "PackExpansionExpr",
      clang::EntityKind::SizeOfPackExpr => "SizeOfPackExpr",
      clang::EntityKind::LambdaExpr => "LambdaExpr",
      clang::EntityKind::ObjCBoolLiteralExpr => "ObjCBoolLiteralExpr",
      clang::EntityKind::ObjCSelfExpr => "ObjCSelfExpr",
      clang::EntityKind::OmpArraySectionExpr => "OmpArraySectionExpr",
      clang::EntityKind::ObjCAvailabilityCheckExpr => "ObjCAvailabilityCheckExpr",
      clang::EntityKind::FixedPointLiteral => "FixedPointLiteral",
      clang::EntityKind::UnexposedStmt => "UnexposedStmt",
      clang::EntityKind::LabelStmt => "LabelStmt",
      clang::EntityKind::CompoundStmt => "CompoundStmt",
      clang::EntityKind::CaseStmt => "CaseStmt",
      clang::EntityKind::DefaultStmt => "DefaultStmt",
      clang::EntityKind::IfStmt => "IfStmt",
      clang::EntityKind::SwitchStmt => "SwitchStmt",
      clang::EntityKind::WhileStmt => "WhileStmt",
      clang::EntityKind::DoStmt => "DoStmt",
      clang::EntityKind::ForStmt => "ForStmt",
      clang::EntityKind::GotoStmt => "GotoStmt",
      clang::EntityKind::IndirectGotoStmt => "IndirectGotoStmt",
      clang::EntityKind::ContinueStmt => "ContinueStmt",
      clang::EntityKind::BreakStmt => "BreakStmt",
      clang::EntityKind::ReturnStmt => "ReturnStmt",
      clang::EntityKind::AsmStmt => "AsmStmt",
      clang::EntityKind::ObjCAtTryStmt => "ObjCAtTryStmt",
      clang::EntityKind::ObjCAtCatchStmt => "ObjCAtCatchStmt",
      clang::EntityKind::ObjCAtFinallyStmt => "ObjCAtFinallyStmt",
      clang::EntityKind::ObjCAtThrowStmt => "ObjCAtThrowStmt",
      clang::EntityKind::ObjCAtSynchronizedStmt => "ObjCAtSynchronizedStmt",
      clang::EntityKind::ObjCAutoreleasePoolStmt => "ObjCAutoreleasePoolStmt",
      clang::EntityKind::ObjCForCollectionStmt => "ObjCForCollectionStmt",
      clang::EntityKind::CatchStmt => "CatchStmt",
      clang::EntityKind::TryStmt => "TryStmt",
      clang::EntityKind::ForRangeStmt => "ForRangeStmt",
      clang::EntityKind::SehTryStmt => "SehTryStmt",
      clang::EntityKind::SehExceptStmt => "SehExceptStmt",
      clang::EntityKind::SehFinallyStmt => "SehFinallyStmt",
      clang::EntityKind::SehLeaveStmt => "SehLeaveStmt",
      clang::EntityKind::MsAsmStmt => "MsAsmStmt",
      clang::EntityKind::NullStmt => "NullStmt",
      clang::EntityKind::DeclStmt => "DeclStmt",
      clang::EntityKind::OmpParallelDirective => "OmpParallelDirective",
      clang::EntityKind::OmpSimdDirective => "OmpSimdDirective",
      clang::EntityKind::OmpForDirective => "OmpForDirective",
      clang::EntityKind::OmpSectionsDirective => "OmpSectionsDirective",
      clang::EntityKind::OmpSectionDirective => "OmpSectionDirective",
      clang::EntityKind::OmpSingleDirective => "OmpSingleDirective",
      clang::EntityKind::OmpParallelForDirective => "OmpParallelForDirective",
      clang::EntityKind::OmpParallelSectionsDirective => "OmpParallelSectionsDirective",
      clang::EntityKind::OmpTaskDirective => "OmpTaskDirective",
      clang::EntityKind::OmpMasterDirective => "OmpMasterDirective",
      clang::EntityKind::OmpCriticalDirective => "OmpCriticalDirective",
      clang::EntityKind::OmpTaskyieldDirective => "OmpTaskyieldDirective",
      clang::EntityKind::OmpBarrierDirective => "OmpBarrierDirective",
      clang::EntityKind::OmpTaskwaitDirective => "OmpTaskwaitDirective",
      clang::EntityKind::OmpFlushDirective => "OmpFlushDirective",
      clang::EntityKind::OmpOrderedDirective => "OmpOrderedDirective",
      clang::EntityKind::OmpAtomicDirective => "OmpAtomicDirective",
      clang::EntityKind::OmpForSimdDirective => "OmpForSimdDirective",
      clang::EntityKind::OmpParallelForSimdDirective => "OmpParallelForSimdDirective",
      clang::EntityKind::OmpTargetDirective => "OmpTargetDirective",
      clang::EntityKind::OmpTeamsDirective => "OmpTeamsDirective",
      clang::EntityKind::OmpTaskgroupDirective => "OmpTaskgroupDirective",
      clang::EntityKind::OmpCancellationPointDirective => "OmpCancellationPointDirective",
      clang::EntityKind::OmpCancelDirective => "OmpCancelDirective",
      clang::EntityKind::OmpTargetDataDirective => "OmpTargetDataDirective",
      clang::EntityKind::OmpTaskLoopDirective => "OmpTaskLoopDirective",
      clang::EntityKind::OmpTaskLoopSimdDirective => "OmpTaskLoopSimdDirective",
      clang::EntityKind::OmpDistributeDirective => "OmpDistributeDirective",
      clang::EntityKind::OmpTargetEnterDataDirective => "OmpTargetEnterDataDirective",
      clang::EntityKind::OmpTargetExitDataDirective => "OmpTargetExitDataDirective",
      clang::EntityKind::OmpTargetParallelDirective => "OmpTargetParallelDirective",
      clang::EntityKind::OmpTargetParallelForDirective => "OmpTargetParallelForDirective",
      clang::EntityKind::OmpTargetUpdateDirective => "OmpTargetUpdateDirective",
      clang::EntityKind::OmpDistributeParallelForDirective => "OmpDistributeParallelForDirective",
      clang::EntityKind::OmpDistributeParallelForSimdDirective => {
        "OmpDistributeParallelForSimdDirective"
      }
      clang::EntityKind::OmpDistributeSimdDirective => "OmpDistributeSimdDirective",
      clang::EntityKind::OmpTargetParallelForSimdDirective => "OmpTargetParallelForSimdDirective",
      clang::EntityKind::OmpTargetSimdDirective => "OmpTargetSimdDirective",
      clang::EntityKind::OmpTeamsDistributeDirective => "OmpTeamsDistributeDirective",
      clang::EntityKind::OmpTeamsDistributeSimdDirective => "OmpTeamsDistributeSimdDirective",
      clang::EntityKind::OmpTeamsDistributeParallelForSimdDirective => {
        "OmpTeamsDistributeParallelForSimdDirective"
      }
      clang::EntityKind::OmpTeamsDistributeParallelForDirective => {
        "OmpTeamsDistributeParallelForDirective"
      }
      clang::EntityKind::OmpTargetTeamsDirective => "OmpTargetTeamsDirective",
      clang::EntityKind::OmpTargetTeamsDistributeDirective => "OmpTargetTeamsDistributeDirective",
      clang::EntityKind::OmpTargetTeamsDistributeParallelForDirective => {
        "OmpTargetTeamsDistributeParallelForDirective"
      }
      clang::EntityKind::OmpTargetTeamsDistributeParallelForSimdDirective => {
        "OmpTargetTeamsDistributeParallelForSimdDirective"
      }
      clang::EntityKind::OmpTargetTeamsDistributeSimdDirective => {
        "OmpTargetTeamsDistributeSimdDirective"
      }
      clang::EntityKind::BitCastExpr => "BitCastExpr",
      clang::EntityKind::OmpMasterTaskLoopDirective => "OmpMasterTaskLoopDirective",
      clang::EntityKind::OmpParallelMasterTaskLoopDirective => "OmpParallelMasterTaskLoopDirective",
      clang::EntityKind::OmpMasterTaskLoopSimdDirective => "OmpMasterTaskLoopSimdDirective",
      clang::EntityKind::OmpParallelMasterTaskLoopSimdDirective => {
        "OmpParallelMasterTaskLoopSimdDirective"
      }
      clang::EntityKind::OmpParallelMasterDirective => "OmpParallelMasterDirective",
      clang::EntityKind::TranslationUnit => "TranslationUnit",
      clang::EntityKind::UnexposedAttr => "UnexposedAttr",
      clang::EntityKind::IbActionAttr => "IbActionAttr",
      clang::EntityKind::IbOutletAttr => "IbOutletAttr",
      clang::EntityKind::IbOutletCollectionAttr => "IbOutletCollectionAttr",
      clang::EntityKind::FinalAttr => "FinalAttr",
      clang::EntityKind::OverrideAttr => "OverrideAttr",
      clang::EntityKind::AnnotateAttr => "AnnotateAttr",
      clang::EntityKind::AsmLabelAttr => "AsmLabelAttr",
      clang::EntityKind::PackedAttr => "PackedAttr",
      clang::EntityKind::PureAttr => "PureAttr",
      clang::EntityKind::ConstAttr => "ConstAttr",
      clang::EntityKind::NoDuplicateAttr => "NoDuplicateAttr",
      clang::EntityKind::CudaConstantAttr => "CudaConstantAttr",
      clang::EntityKind::CudaDeviceAttr => "CudaDeviceAttr",
      clang::EntityKind::CudaGlobalAttr => "CudaGlobalAttr",
      clang::EntityKind::CudaHostAttr => "CudaHostAttr",
      clang::EntityKind::CudaSharedAttr => "CudaSharedAttr",
      clang::EntityKind::VisibilityAttr => "VisibilityAttr",
      clang::EntityKind::DllExport => "DllExport",
      clang::EntityKind::DllImport => "DllImport",
      clang::EntityKind::NSReturnsRetained => "NSReturnsRetained",
      clang::EntityKind::NSReturnsNotRetained => "NSReturnsNotRetained",
      clang::EntityKind::NSReturnsAutoreleased => "NSReturnsAutoreleased",
      clang::EntityKind::NSConsumesSelf => "NSConsumesSelf",
      clang::EntityKind::NSConsumed => "NSConsumed",
      clang::EntityKind::ObjCException => "ObjCException",
      clang::EntityKind::ObjCNSObject => "ObjCNSObject",
      clang::EntityKind::ObjCIndependentClass => "ObjCIndependentClass",
      clang::EntityKind::ObjCPreciseLifetime => "ObjCPreciseLifetime",
      clang::EntityKind::ObjCReturnsInnerPointer => "ObjCReturnsInnerPointer",
      clang::EntityKind::ObjCRequiresSuper => "ObjCRequiresSuper",
      clang::EntityKind::ObjCRootClass => "ObjCRootClass",
      clang::EntityKind::ObjCSubclassingRestricted => "ObjCSubclassingRestricted",
      clang::EntityKind::ObjCExplicitProtocolImpl => "ObjCExplicitProtocolImpl",
      clang::EntityKind::ObjCDesignatedInitializer => "ObjCDesignatedInitializer",
      clang::EntityKind::ObjCRuntimeVisible => "ObjCRuntimeVisible",
      clang::EntityKind::ObjCBoxable => "ObjCBoxable",
      clang::EntityKind::FlagEnum => "FlagEnum",
      clang::EntityKind::ConvergentAttr => "ConvergentAttr",
      clang::EntityKind::WarnUnusedAttr => "WarnUnusedAttr",
      clang::EntityKind::WarnUnusedResultAttr => "WarnUnusedResultAttr",
      clang::EntityKind::AlignedAttr => "AlignedAttr",
      clang::EntityKind::PreprocessingDirective => "PreprocessingDirective",
      clang::EntityKind::MacroDefinition => "MacroDefinition",
      clang::EntityKind::MacroExpansion => "MacroExpansion",
      clang::EntityKind::InclusionDirective => "InclusionDirective",
      clang::EntityKind::ModuleImportDecl => "ModuleImportDecl",
      clang::EntityKind::TypeAliasTemplateDecl => "TypeAliasTemplateDecl",
      clang::EntityKind::StaticAssert => "StaticAssert",
      clang::EntityKind::FriendDecl => "FriendDecl",
      clang::EntityKind::OverloadCandidate => "OverloadCandidate",
    }
  }
}
