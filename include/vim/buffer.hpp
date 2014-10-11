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
      /* TODO: Cursor position, dimensions, highlight range, etc. */
    };
  }
}
