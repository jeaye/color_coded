#pragma once

#include "lua/eval.hpp"

namespace color_coded
{
  namespace vim
  {
    inline void matchaddpos(std::string const &type, std::size_t const line,
                            std::size_t const col, std::size_t const len)
    {
      lua::eval("vim.command(\"call matchaddpos('"
           + type + "', [["
           + std::to_string(line) + ", "
           + std::to_string(col) + ", "
           + std::to_string(len)
           + "]], -1)\")");
    }
  }
}
