#pragma once

#include <algorithm>
#include <clang-c/CXString.h>

#include "detail/resource.hpp"

namespace color_coded
{
  namespace clang
  {
    class string
    {
      public:
        string() = default;
        string(string const&) = default;
        string(string &&) noexcept = default;
        string(CXString &&str)
          : str_{ std::move(str), &deleter }
        { str.data = nullptr; }

        string& operator =(string const&) = default;
        string& operator =(string &&) noexcept = default;
        string& operator =(CXString &&str) noexcept
        {
          str_ = std::move(str);
          return *this;
        }

        CXString& get()
        { return str_.get(); }
        CXString const& get() const
        { return str_.get(); }

        char const * c_str() const
        { return clang_getCString(str_.get()); }

        static void deleter(CXString &str)
        { clang_disposeString(str); }

      private:
        detail::resource<CXString> str_{ {0,0}, &deleter };
    };
  }
}
