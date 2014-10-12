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
      template <typename... Args_>
      static R call(Args_... args)
      try
      { return F(std::forward<Args_>(args)...); }
      catch(std::exception const &e)
      { ruby::vim::message(std::string{"exception: "} + e.what()); }
      catch(...)
      { ruby::vim::message("unknown error"); }
    };

    template <typename T, T F>
    struct make_safe_func;

    template <typename R, typename... Args, R (*F)(Args...)>
    struct make_safe_func<R (*)(Args...), F>
    {
      static R constexpr (*value)(Args...)
      { &safe_func<R (*)(Args...), F>::template call<Args...> };
    };
  }
  template <typename F, F f>
  auto constexpr safe_func()
  { return detail::make_safe_func<F, f>::value; }
}
