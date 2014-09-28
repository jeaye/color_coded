#pragma once

#include <string>
#include <vector>

#include "vim/highlight.hpp"

namespace color_coded
{
  namespace async
  {
    struct task
    {
      std::string file;
      std::string code;
    };
    struct result
    { vim::highlight_group group; };
  }
}
