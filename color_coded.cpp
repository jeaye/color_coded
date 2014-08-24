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
  {
    std::cout << str << std::endl;
    rb_eval_string(str.c_str()); }

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

  std::string get_token_type(CXTokenKind const kind)
  {
    switch (kind)
    {
      case CXToken_Punctuation:
        return "Normal";
      case CXToken_Keyword:
        return "keyword";
      case CXToken_Identifier:
        return "Type";
      case CXToken_Literal:
        return "Number";
      case CXToken_Comment:
        return "Comment";
      default:
        return "Unknown";
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
      std::string typ{ get_token_type(kind) };
      if(cursors[i].kind == CXCursor_StructDecl && kind == CXToken_Identifier)
      { typ = "String"; }

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
    std::cout << ifs.tellg() << std::endl;
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
