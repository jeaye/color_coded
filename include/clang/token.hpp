#pragma once

#include <string>
#include <stdexcept>

#include <clang-c/Index.h>

namespace color_coded
{
  namespace clang
  {
    namespace token
    {
      inline std::string map_type_kind(CXTypeKind const kind)
      {
        switch(kind)
        {
          case CXType_Unexposed:
            return "Variable";

          case CXType_Void:
          case CXType_Bool:
          case CXType_Char_U:
          case CXType_UChar:
          case CXType_Char16:
          case CXType_Char32:
          case CXType_UShort:
          case CXType_UInt:
          case CXType_ULong:
          case CXType_ULongLong:
          case CXType_UInt128:
          case CXType_Char_S:
          case CXType_SChar:
          case CXType_WChar:
          case CXType_Short:
          case CXType_Int:
          case CXType_Long:
          case CXType_LongLong:
          case CXType_Int128:
          case CXType_Float:
          case CXType_Double:
          case CXType_LongDouble:
          case CXType_NullPtr:
          case CXType_Overload:
          case CXType_Dependent:
          case CXType_ObjCId:
          case CXType_ObjCClass:
          case CXType_ObjCSel:
            return "Variable";

          case CXType_Complex:
          case CXType_Pointer:
          case CXType_BlockPointer:
          case CXType_LValueReference:
          case CXType_RValueReference:
          case CXType_Record:
          case CXType_Typedef:
          case CXType_ObjCInterface:
          case CXType_ObjCObjectPointer:
          case CXType_ConstantArray:
          case CXType_Vector:
          case CXType_IncompleteArray:
          case CXType_VariableArray:
          case CXType_DependentSizedArray:
            return "Variable";

          case CXType_MemberPointer:
            return "Member";

          case CXType_Enum:
            return "EnumConstant";

          case CXType_FunctionNoProto:
          case CXType_FunctionProto:
            return "Function";
#if CINDEX_VERSION_MINOR >= 32
          case CXType_Auto:
            return "Variable";
#endif
          default:
            return "";
            //return "Error1 " + std::to_string(kind);
        }
      }

      inline std::string map_cursor_kind(CXCursorKind const kind,
                                         CXTypeKind const type)
      {
        switch(kind)
        {
          /********* Declarations **********/
          case CXCursor_UnexposedDecl: /* Unknown declaration */
            return "";
          case CXCursor_StructDecl:
            return "StructDecl";
          case CXCursor_UnionDecl:
            return "UnionDecl";
          case CXCursor_ClassDecl:
            return "ClassDecl";
          case CXCursor_EnumDecl:
            return "EnumDecl";
          case CXCursor_FieldDecl:
            return "FieldDecl";
          case CXCursor_EnumConstantDecl:
            return "EnumConstantDecl";
          case CXCursor_FunctionDecl:
            return "FunctionDecl";
          case CXCursor_VarDecl:
            return "VarDecl";
          case CXCursor_ParmDecl:
            return "ParmDecl";
          case CXCursor_ObjCInterfaceDecl:
            return "ObjCInterfaceDecl";
          case CXCursor_ObjCCategoryDecl:
            return "ObjCCategoryDecl";
          case CXCursor_ObjCProtocolDecl:
            return "ObjCProtocolDecl";
          case CXCursor_ObjCPropertyDecl:
            return "ObjCPropertyDecl";
          case CXCursor_ObjCIvarDecl:
            return "ObjCIvarDecl";
          case CXCursor_ObjCInstanceMethodDecl:
            return "ObjCInstanceMethodDecl";
          case CXCursor_ObjCClassMethodDecl:
            return "ObjCClassMethodDecl";
          case CXCursor_ObjCImplementationDecl:
            return "ObjCImplementationDecl";
          case CXCursor_ObjCCategoryImplDecl:
            return "ObjCCategoryImplDecl";
          case CXCursor_TypedefDecl:
            return "TypedefDecl";
          case CXCursor_CXXMethod:
            return "CXXMethod";
          case CXCursor_Namespace:
            return "Namespace";
          case CXCursor_LinkageSpec:
            return "LinkageSpec";
          case CXCursor_Constructor:
            return "Constructor";
          case CXCursor_Destructor:
            return "Destructor";
          case CXCursor_ConversionFunction:
            return "ConversionFunction";
          case CXCursor_TemplateTypeParameter:
            return "TemplateTypeParameter";
          case CXCursor_NonTypeTemplateParameter:
            return "NonTypeTemplateParameter";
          case CXCursor_TemplateTemplateParameter:
            return "TemplateTemplateParameter";
          case CXCursor_FunctionTemplate:
            return "FunctionTemplate";
          case CXCursor_ClassTemplate:
            return "ClassTemplate";
          case CXCursor_ClassTemplatePartialSpecialization:
            return "ClassTemplatePartialSpecialization";
          case CXCursor_NamespaceAlias:
            return "NamespaceAlias";
          case CXCursor_UsingDirective:
            return "UsingDirective";
          case CXCursor_UsingDeclaration:
            return "UsingDeclaration";
          case CXCursor_TypeAliasDecl:
            return "TypeAliasDecl";
          case CXCursor_ObjCSynthesizeDecl:
            return "ObjCSynthesizeDecl";
          case CXCursor_ObjCDynamicDecl:
            return "ObjCDynamicDecl";
          case CXCursor_CXXAccessSpecifier:
            return "CXXAccessSpecifier";

            /********* References **********/
          case CXCursor_ObjCSuperClassRef:
            return "ObjCSuperClassRef";
          case CXCursor_ObjCProtocolRef:
            return "ObjCProtocolRef";
          case CXCursor_ObjCClassRef:
            return "ObjCClassRef";
          case CXCursor_TypeRef:
            return "TypeRef";
          case CXCursor_CXXBaseSpecifier:
            return "CXXBaseSpecifier";
          case CXCursor_TemplateRef:
            return "TemplateRef";
          case CXCursor_NamespaceRef:
            return "NamespaceRef";
          case CXCursor_MemberRef:
            return "MemberRef";
          case CXCursor_LabelRef:
            return "LabelRef";
          case CXCursor_OverloadedDeclRef:
            return "OverloadedDeclRef";
          case CXCursor_VariableRef:
            return "VariableRef";

            /********* Errors **********/
          case CXCursor_InvalidFile:
            return "";
          case CXCursor_NoDeclFound:
            return "";
          case CXCursor_NotImplemented:
            return "";
          case CXCursor_InvalidCode:
            return "";

            /********* Expressions **********/
          case CXCursor_UnexposedExpr:
            break;
          case CXCursor_DeclRefExpr:
            return map_type_kind(type);
          case CXCursor_MemberRefExpr:
            return "MemberRefExpr";
          case CXCursor_CallExpr:
            return "CallExpr";
          case CXCursor_ObjCMessageExpr:
            return "ObjCMessageExpr";
          case CXCursor_BlockExpr:
            return "BlockExpr";
          case CXCursor_MacroDefinition:
            return "MacroDefinition";
          case CXCursor_MacroInstantiation:
            return "MacroInstantiation";
          case CXCursor_PreprocessingDirective: /* XXX: do not want */
            return "";
          case CXCursor_InclusionDirective: /* XXX: do not want */
            return "";
          case CXCursor_CompoundStmt:
            return "";
          case CXCursor_ParenExpr:
          case CXCursor_LambdaExpr:
          case CXCursor_CXXForRangeStmt:
          case CXCursor_DeclStmt:
            return "";
          default:
            return "";
            //return "Error2 " + std::to_string(kind);
        }
        return "";
        //return "Error3 " + std::to_string(kind);
      }

      inline std::string map_literal_kind(CXCursorKind const kind)
      {
        switch(kind)
        {
          case CXCursor_IntegerLiteral:
            return "Number";
          case CXCursor_FloatingLiteral:
            return "Float";
          case CXCursor_ImaginaryLiteral:
            return "Number";
          case CXCursor_StringLiteral:
            return ""; /* Allow vim to do this. */
          case CXCursor_CharacterLiteral:
            return "Character";
          case CXType_Unexposed:
            return "";
          default:
            return "";
            //return "Error4 " + std::to_string(kind);
        }
      }

      /* Clang token/cursor -> Vim highlight group. */
      inline std::string map_token_kind(CXTokenKind const token_kind,
          CXCursorKind const cursor_kind,
          CXTypeKind const cursor_type)
      {
        switch (token_kind)
        {
          case CXToken_Punctuation:
            return ""; /* Allow vim to do this. */
          case CXToken_Keyword:
            return ""; /* Allow vim to do this. */
          case CXToken_Identifier:
            return map_cursor_kind(cursor_kind, cursor_type);
          case CXToken_Literal:
            return map_literal_kind(cursor_kind);
          case CXToken_Comment:
            return ""; /* Allow vim to do this. */
          default:
            return "";
            //return "Error5 " + std::to_string(token_kind);
        }
      }
    }
  }
}
