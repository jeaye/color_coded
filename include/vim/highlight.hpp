#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <clang-c/Index.h>

#include "clang/string.hpp"
#include "clang/token_pack.hpp"
#include "clang/translation_unit.hpp"
#include "clang/token.hpp"
#include "clang/location.hpp"

#include "vim/commands.hpp"

namespace color_coded
{
  namespace vim
  {
    struct highlight
    {
      highlight() = delete;
      highlight(std::string const &ty, std::size_t const l,
                std::size_t const c, std::string const &tok)
        : type{ ty }
        , line{ l }
        , column{ c }
        , token{ tok }
      { }

      std::string type;
      std::size_t line, column;
      std::string token;
    };

    CXChildVisitResult visit(CXCursor const cursor, CXCursor const parent, CXClientData const data);

    struct visit_info
    {
      CXFile const file;
      class highlight_group * const group;
    };

    class highlight_group
    {
      public:
        using vec_t = std::vector<highlight>;
        using iterator = vec_t::iterator;
        using const_iterator = vec_t::const_iterator;
        using size_type = std::size_t;

        highlight_group() = default;
        highlight_group(clang::translation_unit const &trans_unit,
                        clang::token_pack &tokens)
        {
          auto &tu(trans_unit.impl);
          std::vector<CXCursor> cursors(tokens.size());
          clang_annotateTokens(tu, tokens.begin(), tokens.size(), cursors.data());

          visit_info vi
          { clang_getFile(tu, trans_unit.filename.c_str()), this };
          //clang_visitChildren(clang_getTranslationUnitCursor(tu), &visit, &vi);

          auto cursor(cursors.cbegin());
          for(auto token(tokens.cbegin()); token != tokens.cend(); ++token, ++cursor)
          {
            CXTokenKind const kind{ clang_getTokenKind(*token) };
            clang::string const spell{ clang_getTokenSpelling(tu, *token) };
            auto const loc(clang_getTokenLocation(tu, *token));

            CXFile file{};
            unsigned line{}, column{}, offset{};
            clang_getFileLocation(loc, &file, &line, &column, &offset);

            //auto const cur(clang_getCursor(tu, loc));
            auto const cur(*cursor);
            auto const cursor_kind(cur.kind);
            auto const cursor_type(clang_getCursorType(cur).kind);

            auto const mapped(clang::token::map_token_kind(kind, cursor_kind,
                                                             cursor_type));
            if(mapped.size())
            {
              std::cout << spell.c_str() << " : " << mapped << std::endl;
              group_.emplace_back(mapped, line, column, spell.c_str()); }

            //switch(kind)
            //{
            //  case CXToken_Punctuation:
            //    emplace_back("Punctuation", line, column, spell.c_str());
            //    break;
            //  case CXToken_Keyword:
            //    emplace_back("Keyword", line, column, spell.c_str());
            //    break;
            //  case CXToken_Identifier:
            //    clang_visitChildren(*cursor, &visit, this);
            //    break;
            //  case CXToken_Literal:
            //    clang_visitChildren(*cursor, &visit, this);
            //    break;
            //  case CXToken_Comment:
            //    emplace_back("Comment", line, column, spell.c_str());
            //    break;
            //  default:
            //    emplace_back("", line, column, spell.c_str());
            //}
          }
        }

        template <typename... Args>
        void emplace_back(Args &&...args)
        { group_.emplace_back(std::forward<Args>(args)...); }

        bool empty() const
        { return group_.empty(); }
        size_type size() const
        { return group_.size(); }

        iterator begin()
        { return group_.begin(); }
        const_iterator begin() const
        { return group_.begin(); }
        const_iterator cbegin() const
        { return group_.cbegin(); }
        iterator end()
        { return group_.end(); }
        const_iterator end() const
        { return group_.end(); }
        const_iterator cend() const
        { return group_.cend(); }

      private:
        std::vector<highlight> group_;
    };

    CXChildVisitResult visit(CXCursor const cursor, CXCursor const parent, CXClientData const data)
    {
      auto const &info(*static_cast<visit_info *const>(data));
      auto const cursor_kind(cursor.kind);
      auto const cursor_type(clang_getCursorType(cursor).kind);
      clang::string const spell{ clang_getCursorSpelling(cursor) };
      auto const loc(clang_getCursorLocation(cursor));

      auto const range(clang_getCursorExtent(cursor));
      auto const start(clang_getRangeStart(range));
      auto const end(clang_getRangeEnd(range));

      static CXCursor const null_cursor(clang_getNullCursor());
      static CXSourceLocation const null_location(clang_getNullLocation());
      if(clang_equalLocations(start, null_location) ||
          clang_equalLocations(end, null_location))
      { return CXChildVisit_Recurse; }

      auto const spelling_loc(clang::make_spelling_location(start, end));
      auto const presumed_loc(clang::make_presumed_location(start, end));
      auto const expansion_loc(clang::make_expansion_location(start, end));

      if(!clang_File_isEqual(spelling_loc.file, info.file))
      { return CXChildVisit_Recurse; }

      std::cout << spell.c_str() << ", ";
      std::cout << "(" << spelling_loc << ", "
                << presumed_loc << ", "
                << expansion_loc << ") ";
      std::cout << clang::string
      {clang_getCursorKindSpelling(clang_getCursorKind(cursor))}.c_str()
      << ", ";
      std::cout << clang::string
      {clang_getTypeKindSpelling(clang_getCursorType(cursor).kind)}.c_str()
      << ", ";
      std::cout << clang::string
      {clang_getCursorDisplayName(cursor)}.c_str()
      << std::endl;

      auto const mapped(clang::token::map_token_kind(CXToken_Identifier,
                                                     cursor_kind,
                                                     cursor_type));
      info.group->emplace_back(mapped, spelling_loc.start_line,
                               spelling_loc.start_col, spell.c_str());

      return CXChildVisit_Recurse;
    }
  }
}
