#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include <clang-c/Index.h>

#include "clang/string.hpp"
#include "clang/token_pack.hpp"
#include "clang/translation_unit.hpp"
#include "clang/index.hpp"
#include "clang/resource.hpp"
#include "clang/token.hpp"
#include "clang/source_range.hpp"

#include "ruby/vim.hpp"

namespace color_coded
{
  void show_all_tokens(clang::translation_unit const &trans_unit,
                       clang::token_pack &tokens)
  {
    ruby::vim::clearmatches();

    auto &tu(trans_unit.impl.get());
    std::vector<CXCursor> cursors(tokens.size());
    clang_annotateTokens(tu, tokens.begin(), tokens.size(), cursors.data());

    auto cursor(cursors.cbegin());
    for(auto token(tokens.cbegin()); token != tokens.cend(); ++token, ++cursor)
    {
      CXTokenKind const kind{ clang_getTokenKind(*token) };
      clang::string const spell{ clang_getTokenSpelling(tu, *token) };
      CXSourceLocation const loc(clang_getTokenLocation(tu, *token));

      CXFile file{};
      unsigned line{}, column{}, offset{};
      clang_getFileLocation(loc, &file, &line, &column, &offset);
      clang::string const filename{ clang_getFileName(file) };

      std::string const token_text{ spell.c_str() };
      std::string const typ{ clang::token::to_string(kind, cursor->kind) };

      ruby::vim::matchaddpos(typ, line, column, token_text.size());
    }
  }

  void work(std::string const &filename)
  {
    clang::index const index{ clang_createIndex(false, false) };
    clang::translation_unit const trans_unit{ index, filename };
    auto &tu(trans_unit.impl.get());

    std::size_t const errors{ clang_getNumDiagnostics(tu) };
    if(errors || !tu)
    {
      for(std::size_t i{}; i != errors; ++i)
      {
        CXDiagnostic const diag{ clang_getDiagnostic(tu, i) };
        clang::string const str{ clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions()) };
        std::cout << str.c_str() << std::endl;
      }
      throw std::runtime_error{ "unable to compile translation unit" };
    }

    clang::token_pack tp{ trans_unit, clang::source_range(trans_unit) };
    show_all_tokens(trans_unit, tp);
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
