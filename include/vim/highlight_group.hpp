#pragma once

#include <string>

namespace color_coded
{
  namespace vim
  {
    struct highlight_group
    {
      highlight_group(std::string const &ty, std::size_t const l,
                      std::size_t const c, std::string const &tok)
        : type{ ty }
        , line{ l }
        , column{ c }
        , token{ tok }
      { }

      std::string const type;
      std::size_t const line, column;
      std::string const token;
    };
  }
}
