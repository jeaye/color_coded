#pragma once

#include <algorithm>

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

      inline void message(std::string str)
      {
        std::transform(std::begin(str), std::end(str), std::begin(str),
        [](char const c){ return c == '"' ? '\'' : c; });
        eval("VIM::message(\"" + str + "\")");
      }
    }
  }
}
