#pragma once

#include "eval.hpp"

namespace color_coded
{
  namespace ruby
  {
    namespace vim
    {
      inline void clearmatches()
      { eval("VIM::command(\"call clearmatches()\")"); }

      inline void matchaddpos(std::string const &type, std::size_t const line,
                              std::size_t const col, std::size_t const len)
      {
        eval("VIM::command(\"call matchaddpos('"
             + type + "', [["
             + std::to_string(line) + ", "
             + std::to_string(col) + ", "
             + std::to_string(len)
             + "]], -1)\")");
      }
    }
  }
}
