#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <array>

#include <clang-c/Index.h>

#include "ruby.h"

#include <juble/juble.hpp>

namespace color_coded
{
  void eval(std::string const &str)
  { rb_eval_string(str.c_str()); }

  template <typename T, typename... Ts>
  std::array<std::decay_t<T>, sizeof...(Ts) + 1> make_array(T &&t, Ts &&... ts)
  { return { std::forward<T>(t), std::forward<Ts>(ts)... }; }

  std::string get_token_spelling(CXTokenKind const kind)
  {
    switch (kind)
    {
      case CXToken_Punctuation:
        return "Punctuation";
      case CXToken_Keyword:
        return "Keyword";
      case CXToken_Identifier:
        return "Identifier";
      case CXToken_Literal:
        return "Literal";
      case CXToken_Comment:
        return "Comment";
      default:
        return "Unknown";
    }
  }

  std::string get_token_type(CXTokenKind const token_kind,
                             CXCursorKind const cursor_kind)
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
            /* A field (in C) or non-static data member (in C++) in a struct, union, or C++ class. */
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

               A type reference occurs anywhere where a type is named but not declared. For example, given:

               typedef unsigned size_type;
               size_type size;
               The typedef is a declaration of size_type (CXCursor_TypedefDecl), while the type of the variable "size" is referenced. The cursor referenced by the type of size is the typedef for size_type.  */
            return "TypeRef";
          case CXCursor_CXXBaseSpecifier: 	
            /* 
          A reference to a class template, function template, template template parameter, or class template partial specialization. */
            return "CXXBaseSpecifier";
          case CXCursor_NamespaceRef: 	
            /* A reference to a namespace or namespace alias. */
            return "NamespaceRef";
          case CXCursor_MemberRef: 	
            /* A reference to a member of a struct, union, or class that occurs in some non-expression context, e.g., a designated initializer. */
            return "MemberRef";
          case CXCursor_LabelRef: 	
            /* A reference to a labeled statement.

               This cursor kind is used to describe the jump to "start_over" in the goto statement in the following example:

               start_over:
                 ++counter;

               goto start_over;

               A label reference cursor refers to a label statement. */
            return "LabelRef";
          case CXCursor_OverloadedDeclRef: 	
            /* A reference to a set of overloaded functions or function templates that has not yet been resolved to a specific function or function template.

               An overloaded declaration reference cursor occurs in C++ templates where a dependent name refers to a function. For example:

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

               Here, the identifier "swap" is associated with an overloaded declaration reference. In the template definition, "swap" refers to either of the two "swap" functions declared above, so both results will be available. At instantiation time, "swap" may also refer to other functions found via argument-dependent lookup (e.g., the "swap" function at the end of the example).

               The functions clang_getNumOverloadedDecls() and clang_getOverloadedDecl() can be used to retrieve the definitions referenced by this cursor. */
            return "OverloadedDeclRef";
          case CXCursor_VariableRef: 	
            /* A reference to a variable that occurs in some non-expression context, e.g., a C++ lambda capture list. */
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
            /* An expression whose specific kind is not exposed via this interface.

               Unexposed expressions have the same operations as any other kind of expression; one can extract their location information, spelling, children, etc. However, the specific kind of the expression is not reported. */
            return "FirstExpr";
          case CXCursor_DeclRefExpr: 	
            /* An expression that refers to some value declaration, such as a function, variable, or enumerator. */
            return "DeclRefExpr";
          case CXCursor_MemberRefExpr: 	
            /* An expression that refers to a member of a struct, union, class, Objective-C class, etc. */
            return "MemberRefExpr";
          case CXCursor_CallExpr: 	
            /* An expression that calls a function. */
            return "CallExpr";
          case CXCursor_ObjCMessageExpr: 	
            /* An expression that sends a message to an Objective-C object or class. */
            return "ObjCMessageExpr";
          case CXCursor_BlockExpr: 	
            /* An expression that represents a block literal. */
            return "BlockExpr";
          case CXCursor_MacroDefinition:
            return "MacroDefinition";
          case CXCursor_MacroInstantiation:
            return "MacroInstantiation";

          default:
            return "Type";
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
          default:
            throw std::runtime_error{ "unknown literal" };
        }
      case CXToken_Comment:
        return "Comment";
      default:
        return "Normal";
    }
  }

  void show_all_tokens(CXTranslationUnit const &tu, CXToken *tokens, size_t num_tokens)
  {
    eval("VIM::command(\"call clearmatches()\")");

    std::vector<CXCursor> cursors(num_tokens);
    clang_annotateTokens(tu, tokens, num_tokens, cursors.data());

    for(size_t i{}; i < num_tokens; ++i)
    {
      CXToken const &token{ tokens[i] };
      CXTokenKind const kind{ clang_getTokenKind(token) };
      CXString const spell(clang_getTokenSpelling(tu, token));
      CXSourceLocation const loc(clang_getTokenLocation(tu, token));

      CXFile file{};
      unsigned line{}, column{}, offset{};
      clang_getFileLocation(loc, &file, &line, &column, &offset);
      CXString const filename(clang_getFileName(file));

      std::string const token_text{ clang_getCString(spell) };
      std::string const token_kind{ get_token_spelling(kind) };

      std::string const len{ std::to_string(token_text.size()) };
      std::string const col{ std::to_string(column) };
      std::string const lin{ std::to_string(line) };
      std::string typ{ get_token_type(kind, cursors[i].kind) };
      //if(cursors[i].kind == CXCursor_StructDecl && kind == CXToken_Identifier)
      //{ typ = "String"; }

      eval("VIM::command(\"call matchaddpos('" + typ + "', [[" + lin + ", " + col + ", " + len  + "]], -1)\")");

      clang_disposeString(filename);
      clang_disposeString(spell);
    }
  }

  size_t get_filesize(std::string const &file)
  {
    std::ifstream ifs{ file };
    if(!ifs.is_open())
    { throw std::runtime_error{ "failed to open " + file }; }
    ifs.seekg(0, std::ios::end);
    return ifs.tellg();
  }

  CXSourceRange get_filerange(CXTranslationUnit const &tu, std::string const &filename)
  {
    CXFile const file{ clang_getFile(tu, filename.c_str()) };
    size_t const size{ get_filesize(filename.c_str()) };

    CXSourceLocation const top(clang_getLocationForOffset(tu, file, 0));
    CXSourceLocation const bottom(clang_getLocationForOffset(tu, file, size));
    if(clang_equalLocations(top,  clang_getNullLocation()) ||
       clang_equalLocations(bottom, clang_getNullLocation()))
    { throw std::runtime_error{ "cannot retrieve location" }; }

    CXSourceRange const range(clang_getRange(top, bottom));
    if(clang_Range_isNull(range))
    { throw std::runtime_error{ "cannot retrieve range" }; }

    return range;
  }

  void work(std::string const &)
  {
    auto const args(make_array("-I/usr/include", "-I."));

    std::string const filename{ "test.cpp" };
    CXIndex const index{ clang_createIndex(true, true) };
    CXTranslationUnit const tu
    { clang_parseTranslationUnit(index, filename.c_str(),
        args.data(), args.size(), nullptr, 0, CXTranslationUnit_None) };

    size_t const errors{ clang_getNumDiagnostics(tu) };
    if(errors || !tu)
    {
      for(size_t i{}; i != errors; ++i)
      {
        CXDiagnostic const diag{ clang_getDiagnostic(tu, i) };
        CXString const string(clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions()));
        std::cout << clang_getCString(string) << std::endl;
        clang_disposeString(string);
      }
      throw std::runtime_error{ "unable to compile translation unit" };
    }

    CXSourceRange const range(get_filerange(tu, filename));

    CXToken *tokens{};
    unsigned num_tokens{};
    clang_tokenize(tu, range, &tokens, &num_tokens);
    show_all_tokens(tu, tokens, num_tokens);

    clang_disposeTokens(tu, tokens, num_tokens);
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
