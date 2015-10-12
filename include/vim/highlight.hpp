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

    class highlight_group
    {
      public:
        using vec_t = std::vector<highlight>;
        using iterator = vec_t::iterator;
        using const_iterator = vec_t::const_iterator;
        using size_type = std::size_t;

        highlight_group() = default;
        highlight_group(clang::translation_unit const &trans_unit,
                        clang::token_pack &&tokens)
        {
          auto &tu(trans_unit.impl);
          std::vector<CXCursor> cursors(tokens.size());
          clang_annotateTokens(tu, tokens.begin(), tokens.size(), cursors.data());

          auto cursor(cursors.cbegin());
          for(auto token(tokens.cbegin()); token != tokens.cend();
              ++token, ++cursor)
          {
            CXTokenKind const kind{ clang_getTokenKind(*token) };
            clang::string const spell{ clang_getTokenSpelling(tu, *token) };
            auto const loc(clang_getTokenLocation(tu, *token));

            CXFile file{};
            unsigned line{}, column{}, offset{};
            clang_getFileLocation(loc, &file, &line, &column, &offset);

            auto const cur(*cursor);
            auto const cursor_kind(cur.kind);
            auto const cursor_type(clang_getCursorType(cur).kind);

            auto const mapped(clang::token::map_token_kind(kind, cursor_kind,
                                                           cursor_type));
            if(mapped.size())
            {
              //std::cout << spell.c_str() << " : " << mapped << std::endl;
              emplace_back(mapped, line, column, spell.c_str());
            }
            else
            { /* std::cout << "unmapped: " << spell.c_str() << std::endl; */ }
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
  }
}
