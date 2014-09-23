#pragma once

#include <array>
#include <type_traits>

namespace color_coded
{
  namespace detail
  {
    template <typename T, typename... Ts>
    std::array<std::decay_t<T>, sizeof...(Ts) + 1> constexpr
      make_array(T &&t, Ts &&... ts)
    { return { { std::forward<T>(t), std::forward<Ts>(ts)... } }; }
  }
}
