#pragma once

#include <string>

#include "highlight.hpp"

namespace color_coded
{
  namespace vim
  {
    struct buffer
    {
      std::string name;
      highlight_group group;
      std::size_t line{};
      std::size_t begin{}, end{};
      std::size_t visibility{ 100 };
    };

    inline void apply(buffer const &buf)
    {
      if(buf.group.empty())
      { return; }

      ruby::vim::clearmatches();

      auto const begin(buf.line - std::min(buf.visibility, buf.line));
      auto const end(std::min(buf.line + buf.visibility, buf.end));
      for(auto const &h : buf.group)
      {
        if(h.line >= begin && h.line <= end)
        { ruby::vim::matchaddpos(h.type, h.line, h.column, h.token.size()); }
      }
    }
  }
}
