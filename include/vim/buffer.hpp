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
      std::size_t line{}, lines{};
      std::size_t begin{}, end{};
      std::size_t height{};
    };

    /* Forcefully clears all highlighting and applies the latest. */
    void apply(buffer &buf)
    {
      if(buf.group.empty())
      { return; }

      vim::clearmatches();

      buf.begin = buf.line - std::min(buf.height, buf.line);
      buf.end = std::min(buf.line + buf.height, buf.lines);
      for(auto const &h : buf.group)
      {
        if(h.line >= buf.begin && h.line <= buf.end)
        { vim::matchaddpos(h.type, h.line, h.column, h.token.size()); }
      }
    }

    /* Will only highlight if we're outside of the previous highlight range. */
    void try_apply(buffer &buf)
    {
      if(buf.line - std::min(buf.height, buf.line) < buf.begin ||
         std::min(buf.line + buf.height, buf.lines) > buf.end)
      { apply(buf); }
    }
  }
}
