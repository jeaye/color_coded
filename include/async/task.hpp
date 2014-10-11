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
      std::string name;
      std::string code;
    };
    struct result
    {
      std::string name;
      vim::highlight_group group;
    };
  }
}
