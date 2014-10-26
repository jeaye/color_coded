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
      /* Clang token/cursor -> Vim highlight group. */
      std::string to_string(CXTokenKind const token_kind,
                            CXCursorKind const cursor_kind,
                            CXTypeKind const cursor_type)
      {
        switch (token_kind)
        {
          case CXToken_Punctuation:
            return "Punctuation";
          case CXToken_Keyword:
            return "keyword";
          case CXToken_Identifier:
            switch(cursor_kind)
            {
              case CXCursor_StructDecl: 	
                /* A C or C++ struct. */
                return "StructDecl";
              case CXCursor_UnionDecl: 	
                /* A C or C++ union. */
                return "UnionDecl";
              case CXCursor_ClassDecl: 	
                /* A C++ class. */
                return "ClassDecl";
              case CXCursor_EnumDecl: 	
                /* An enumeration. */
                return "EnumDecl";
              case CXCursor_FieldDecl: 	
                /* A field (in C) or non-static data member (in C++) in a
                 * struct, union, or C++ class. */
                return "FieldDecl";
              case CXCursor_EnumConstantDecl: 	
                /* An enumerator constant. */
                return "EnumConstantDecl";
              case CXCursor_FunctionDecl: 	
                /* A function. */
                return "FunctionDecl";
              case CXCursor_VarDecl: 	
                /* A variable. */
                return "VarDecl";
              case CXCursor_ParmDecl: 	
                /* A function or method parameter. */
                return "ParmDecl";
              case CXCursor_ObjCInterfaceDecl: 	
                /* An Objective-C @interface. */
                return "ObjCInterfaceDecl";
              case CXCursor_ObjCCategoryDecl: 	
                /* An Objective-C @interface for a category. */
                return "ObjCCategoryDecl";
              case CXCursor_ObjCProtocolDecl: 	
                /* An Objective-C @protocol declaration. */
                return "ObjCProtocolDecl";
              case CXCursor_ObjCPropertyDecl: 	
                /* An Objective-C @property declaration. */
                return "ObjCPropertyDecl";
              case CXCursor_ObjCIvarDecl:
                /* An Objective-C instance variable. */
                return "ObjCIvarDecl";
              case CXCursor_ObjCInstanceMethodDecl: 	
                /* An Objective-C instance method. */
                return "ObjCInstanceMethodDecl";
              case CXCursor_ObjCClassMethodDecl: 	
                /* An Objective-C class method. */
                return "ObjCClassMethodDecl";
              case CXCursor_ObjCImplementationDecl: 	
                /* An Objective-C @implementation. */
                return "ObjCImplementationDecl";
              case CXCursor_ObjCCategoryImplDecl: 	
                /* An Objective-C @implementation for a category. */
                return "ObjCCategoryImplDecl";
              case CXCursor_TypedefDecl: 	
                /* A typedef. */
                return "TypedefDecl";
              case CXCursor_CXXMethod: 	
                /* A C++ class method. */
                return "CXXMethod";
              case CXCursor_Namespace: 	
                /* A C++ namespace. */
                return "Namespace";
              case CXCursor_LinkageSpec: 	
                /* A linkage specification, e.g. 'extern "C"'. */
                return "LinkageSpec";
              case CXCursor_Constructor: 	
                /* A C++ constructor. */
                return "Constructor";
              case CXCursor_Destructor: 	
                /* A C++ destructor. */
                return "Destructor";
              case CXCursor_ConversionFunction: 	
                /* A C++ conversion function. */
                return "ConversionFunction";
              case CXCursor_TemplateTypeParameter: 	
                /* A C++ template type parameter. */
                return "TemplateTypeParameter";
              case CXCursor_NonTypeTemplateParameter: 	
                /* A C++ non-type template parameter. */
                return "NonTypeTemplateParameter";
              case CXCursor_TemplateTemplateParameter: 	
                /* A C++ template template parameter. */
                return "TemplateTemplateParameter";
              case CXCursor_FunctionTemplate: 	
                /* A C++ function template. */
                return "FunctionTemplate";
              case CXCursor_ClassTemplate: 	
                /* A C++ class template. */
                return "ClassTemplate";
              case CXCursor_ClassTemplatePartialSpecialization: 	
                /* A C++ class template partial specialization. */
                return "ClassTemplatePartialSpecialization";
              case CXCursor_NamespaceAlias: 	
                /* A C++ namespace alias declaration. */
                return "NamespaceAlias";
              case CXCursor_UsingDirective: 	
                /* A C++ using directive. */
                return "UsingDirective";
              case CXCursor_UsingDeclaration: 	
                /* A C++ using declaration. */
                return "UsingDeclaration";
              case CXCursor_TypeAliasDecl: 	
                /* A C++ alias declaration. */
                return "TypeAliasDecl";
              case CXCursor_ObjCSynthesizeDecl: 	
                /* An Objective-C @synthesize definition. */
                return "ObjCSynthesizeDecl";
              case CXCursor_ObjCDynamicDecl: 	
                /* An Objective-C @dynamic definition. */
                return "ObjCDynamicDecl";
              case CXCursor_CXXAccessSpecifier: 	
                /* An access specifier. */
                return "CXXAccessSpecifier";
              case CXCursor_ObjCSuperClassRef:
                return "ObjCSuperClassRef";
              case CXCursor_ObjCProtocolRef:
                return "ObjCProtocolRef";
              case CXCursor_ObjCClassRef:
                return "ObjCClassRef";
              case CXCursor_TypeRef:
                /* A reference to a type declaration.

                   A type reference occurs anywhere where a type is
                   named but not declared. For example, given:

                   typedef unsigned size_type;
                   size_type size;
                   The typedef is a declaration of size_type
                   (CXCursor_TypedefDecl), while the type of the variable
                   "size" is referenced. The cursor referenced by the
                   type of size is the typedef for size_type.  */
                return "TypeRef";
              case CXCursor_CXXBaseSpecifier: 	
                /* A reference to a class template, function template,
                 * template template parameter, or class template partial
                 * specialization. */
                return "CXXBaseSpecifier";
              case CXCursor_NamespaceRef: 	
                /* A reference to a namespace or namespace alias. */
                return "NamespaceRef";
              case CXCursor_MemberRef: 	
                /* A reference to a member of a struct, union, or
                 * class that occurs in some non-expression context,
                 * e.g., a designated initializer. */
                return "MemberRef";
              case CXCursor_LabelRef: 	
                /* A reference to a labeled statement.

                   This cursor kind is used to describe the jump
                   to "start_over" in the goto statement in the
                   following example:

                   start_over:
                     ++counter;

                   goto start_over;

                   A label reference cursor refers to a label statement. */
                return "LabelRef";
              case CXCursor_OverloadedDeclRef: 	
                /* A reference to a set of overloaded functions or
                   function templates that has not yet been resolved
                   to a specific function or function template.

                   An overloaded declaration reference cursor occurs
                   in C++ templates where a dependent name refers to a
                   function. For example:

                   template<typename T> void swap(T&, T&);

                   struct X { ... };
                   void swap(X&, X&);

                   template<typename T>
                   void reverse(T* first, T* last) {
                     while (first < last - 1) {
                       swap(*first, *--last);
                       ++first;
                     }
                   }

                   struct Y { };
                   void swap(Y&, Y&);

                   Here, the identifier "swap" is associated with an
                   overloaded declaration reference. In the template
                   definition, "swap" refers to either of the two
                   "swap" functions declared above, so both results
                   will be available. At instantiation time, "swap"
                   may also refer to other functions found via
                   argument-dependent lookup (e.g., the "swap" function
                   at the end of the example).

                   The functions clang_getNumOverloadedDecls() and
                   clang_getOverloadedDecl() can be used to retrieve the
                   definitions referenced by this cursor. */
                return "OverloadedDeclRef";
              case CXCursor_VariableRef: 	
                /* A reference to a variable that occurs in some
                 * non-expression context, e.g., a C++ lambda capture list. */
                return "VariableRef";
              case CXCursor_FirstInvalid:
                return "FirstInvalid";
              case CXCursor_NoDeclFound:
                return "NoDeclFound";
              case CXCursor_NotImplemented:
                return "NotImplemented";
              case CXCursor_InvalidCode:
                return "InvalidCode";
              case CXCursor_FirstExpr:
                /* An expression whose specific kind is not exposed via
                 * this interface.

                   Unexposed expressions have the same operations as any
                   other kind of expression; one can extract their location
                   information, spelling, children, etc. However, the
                   specific kind of the expression is not reported. */
                return "FirstExpr";
              case CXCursor_DeclRefExpr: 	
                /* An expression that refers to some value declaration,
                 * such as a function, variable, or enumerator. */
                switch(cursor_type)
                {
                  case CXType_Enum:
                    return "EnumConstantDecl";
                  case CXType_Typedef:
                    return "Type";
                  case CXType_FunctionProto:
                  case CXType_FunctionNoProto:
                    return "Function";
                  case CXType_MemberPointer:
                    return "MemberRef";
                  default:
                    return "Variable";
                }
                return "DeclRefExpr";
              case CXCursor_MemberRefExpr: 	
                /* An expression that refers to a member of a struct,
                 * union, class, Objective-C class, etc. */
                return "MemberRefExpr";
              case CXCursor_CallExpr: 	
                /* An expression that calls a function. */
                return "CallExpr";
              case CXCursor_ObjCMessageExpr: 	
                /* An expression that sends a message to an Objective-C
                 * object or class. */
                return "ObjCMessageExpr";
              case CXCursor_BlockExpr: 	
                /* An expression that represents a block literal. */
                return "BlockExpr";
              case CXCursor_MacroDefinition:
                return "MacroDefinition";
              case CXCursor_MacroInstantiation:
                return "MacroInstantiation";
              case CXCursor_PreprocessingDirective:
                return "MacroDefinition";
              case CXCursor_InclusionDirective:
                return "MacroDefinition";
              case CXCursor_ParenExpr:
                return "Variable"; /* TODO wrong? */
              case CXCursor_LambdaExpr:
                return "Normal"; /* TODO wrong */
              case CXCursor_CXXForRangeStmt:
                return "Normal"; /* TODO wrong */
              case CXCursor_DeclStmt: /* TODO What's this? */
                return "Type";
              case CXCursor_TemplateRef:
                return "Type";

              default:
                return "Error";
            }
          case CXToken_Literal:
            switch(cursor_kind)
            {
              case CXCursor_IntegerLiteral:
                return "Number";
              case CXCursor_FloatingLiteral:
                return "Float";
              case CXCursor_ImaginaryLiteral:
                return "Number";
              case CXCursor_StringLiteral:
                return "String";
              case CXCursor_CharacterLiteral:
                return "Character";
              case CXCursor_PreprocessingDirective:
                return "MacroDefinition";
              case CXType_Unexposed:
              default:
                return "Normal"; /* TODO: Why have unhandled things? */
            }
          case CXToken_Comment:
            return "Comment";
          default:
            return "Error";
        }
      }
    }
  }
}
