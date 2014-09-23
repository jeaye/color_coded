#pragma once

#include <juble/juble.hpp>

namespace color_coded
{
  namespace ruby
  {
    template <typename T = void>
    T eval(std::string const &str)
    { return script::ruby_system::eval<T>(str); }
  }
}
