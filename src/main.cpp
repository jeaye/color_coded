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
#include "clang/compile.hpp"

#include "ruby/vim.hpp"

#include "async/queue.hpp"

namespace color_coded
{
  std::vector<vim::highlight_group> show_all_tokens(clang::translation_unit const &trans_unit,
                       clang::token_pack &tokens)
  {
    auto &tu(trans_unit.impl);
    std::vector<CXCursor> cursors(tokens.size());
    clang_annotateTokens(tu, tokens.begin(), tokens.size(), cursors.data());

    std::vector<vim::highlight_group> groups;
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
      std::string const type{ clang::token::to_string(kind, cursor->kind) };

      groups.emplace_back(type, line, column, token_text);
    }

    return groups;
  }

  void apply_highlighting(std::vector<vim::highlight_group> const &groups)
  {
    if(groups.empty())
    { return; }

    ruby::vim::clearmatches();
    for(auto const &group : groups)
    { ruby::vim::matchaddpos(group.type, group.line, group.column, group.token.size()); }
  }

  void work(std::string const &data)
  try
  {
    static async::queue<async::task, async::result> q{
    [](async::task const &t)
    {
      std::string const filename{ ".tmp.cpp" };
      {
        std::ofstream ofs{ filename };
        ofs << t.code << std::endl;
      }
      try
      {
        clang::translation_unit trans_unit{ clang::compile(filename) };
        clang::token_pack tp{ trans_unit, clang::source_range(trans_unit) };
        return async::result{ show_all_tokens(trans_unit, tp) };
      }
      catch(clang::compilation_error const&)
      { return async::result{{}}; }
    } };

    auto const pulled(q.poll());
    if(pulled.second)
    { apply_highlighting(pulled.first.groups); }

    q.push({ data });
  }
  catch(clang::compilation_error const &)
  { }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
