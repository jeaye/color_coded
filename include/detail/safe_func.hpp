#pragma once

#include <string>
#include <utility>
#include <stdexcept>

#include "ruby/vim.hpp"

namespace color_coded
{
  namespace detail
  {
    template <typename T, T F>
    struct safe_func;

    template <typename R, typename... Args, R (*F)(Args...)>
    struct safe_func<R (*)(Args...), F>
    {
      static R call(Args &&... args)
      try
      { return F(std::forward<Args>(args)...); }
      catch(std::exception const &e)
      { ruby::vim::message(std::string{"exception: "} + e.what()); }
      catch(...)
      { ruby::vim::message("unknown error"); }
    };
  }
  template <typename F, F f>
  auto constexpr safe_func()
  { return &detail::safe_func<F, f>::call; }
}
