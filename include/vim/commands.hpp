#pragma once

#include "lua/eval.hpp"

namespace color_coded
{
  namespace vim
  {
    inline void clear_matches()
    {
      lua::eval
      (
        "vim.command"
        "("
          "\"call color_coded#clear_matches(color_coded#get_buffer_name())\""
        ")"
      );
    }

    inline void matchaddpos(std::string const &type, std::size_t const line,
                            std::size_t const col, std::size_t const len)
    {
      lua::eval
      (
        "vim.command(\"call color_coded#add_match("
        "\'" + type + "\', "
        + std::to_string(line) + ", "
        + std::to_string(col) + ", "
        + std::to_string(len)
        + ")\")"
      );
    }
  }
}
